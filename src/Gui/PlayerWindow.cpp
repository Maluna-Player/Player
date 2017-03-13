/*************************************
 * @file    PlayerWindow.cpp
 * @date    26/04/15
 * @author  Manuel
 *
 * Définitions de la classe PlayerWindow.
 *************************************
*/

#include "PlayerWindow.h"
#include "../Audio/FmodManager.h"
#include "../Audio/Song.h"
#include "../Util/Tools.h"

#include <QGridLayout>
#include <QPalette>
#include "MenuBar.h"
#include <QApplication>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMimeData>
#include <QStackedLayout>
#include <QPropertyAnimation>
#include <QMessageBox>
#include "PlayerToggleButton.h"
#include "AboutDialog.h"
#include "SpectrumColorDialog.h"
#include "ProfileDialog.h"


namespace gui {


PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent), m_TimerId(-1), mp_Socket(nullptr)
{
    setWindowTitle(tr(WINDOW_TITLE));
    qApp->setWindowIcon(util::Tools::loadImage(QString(IMAGES_SUBDIR) + "icon.ico"));
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    connect(&m_Player, &audio::Player::songChanged, this, &PlayerWindow::updateCurrentSong);
    connect(&m_Player, &audio::Player::stateChanged, this, &PlayerWindow::setState);
    connect(&m_Player, &audio::Player::previewFinished, this, &PlayerWindow::stopPreview);

    SpectrumColor baseColor { Qt::red, Qt::yellow };
    m_SpectrumColors.append(baseColor);
    m_CurrentSpectrumColor = 0;

    mp_SongTitle = new PlayerLabel(Qt::white, 20);
    mp_SongArtist = new PlayerLabel(Qt::white, 14);
    mp_SongTitle->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    mp_SongArtist->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    mp_SongList = new SongList;
    connect(mp_SongList, &SongList::songPressed, &m_Player, qOverload<audio::Player::SongId>(&audio::Player::changeSong));
    connect(mp_SongList, &SongList::songRemoved, &m_Player, &audio::Player::removeSong);
    connect(&m_Player, &audio::Player::streamError, mp_SongList, &SongList::disableSong);

    createMenuBar();
    createOptionsBar();
    createBottomWindowPart();

    setDesktopWindow();

    connect(&m_ConnectionDialog, &ConnectionDialog::listened, this, &PlayerWindow::listen);
    connect(&m_ConnectionDialog, &ConnectionDialog::connectedToHost, this, &PlayerWindow::connectToHost);
    connect(&m_ConnectionDialog, &ConnectionDialog::canceled, this, &PlayerWindow::closeConnection);
    connect(&m_ConnectionDialog, &ConnectionDialog::disconnected, this, &PlayerWindow::closeConnection);

    if (!m_ProfileManager.load())
        QMessageBox::warning(this, "Erreur de chargement", "Le profil n'a pas pu être chargé.");

    /** Démarrage du player **/

    refreshSongsList();
    m_Player.play();
}

// ==============================
// ==============================

PlayerWindow::~PlayerWindow()
{
    m_Player.stop();
    audio::FmodManager::deleteInstance();

    if (!mp_SongList->parent())
        delete mp_SongList;
}

// ==============================
// ==============================

void PlayerWindow::createMenuBar()
{
    MenuBar *menuBar = new MenuBar;

    mp_Toolbar = addToolBar("toolbar");
    mp_Toolbar->setStyleSheet("QToolBar { background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                        "stop: 0 rgb(150, 150, 150), stop: 0.5 rgb(155, 155, 155),"
                                        "stop: 0.6 rgb(130, 130, 130), stop: 1.0 rgb(85, 85, 85)); border: 0px; }");

    mp_Toolbar->addAction(menuBar->getAddingSongAction());
    mp_Toolbar->addAction(menuBar->getOpenAction());
    mp_Toolbar->addAction(menuBar->getOpenConnectionAction());
    mp_Toolbar->addAction(menuBar->getQuitAction());
    mp_Toolbar->addAction(menuBar->getAboutAction());

    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mp_Toolbar->addWidget(spacer);

    m_ConnectedIcon = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "connected_state.png");
    m_DisconnectedIcon = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "disconnected_state.png");

    mp_ConnectionState = new QLabel;
    mp_ConnectionState->setPixmap(m_DisconnectedIcon);
    mp_ConnectionState->setToolTip("Déconnecté");
    mp_Toolbar->addWidget(mp_ConnectionState);

    connect(menuBar->getAddingSongAction(), &QAction::triggered, this, &PlayerWindow::importSong);
    connect(menuBar->getOpenAction(), &QAction::triggered, this, &PlayerWindow::openSongsDir);
    connect(menuBar->getOpenConnectionAction(), &QAction::triggered, this, &PlayerWindow::openConnection);
    connect(menuBar->getChangeSpectrumColorAction(), &QAction::triggered, this, &PlayerWindow::openSpectrumColorDialog);
    connect(menuBar->getProfileAction(), &QAction::triggered, this, &PlayerWindow::openProfileDialog);

    connect(menuBar->getAboutAction(), &QAction::triggered, this, &PlayerWindow::openInformation);
    connect(menuBar->getQuitAction(), &QAction::triggered, qApp, &QApplication::quit);

    setMenuBar(menuBar);
}

// ==============================
// ==============================

void PlayerWindow::createOptionsBar()
{
    mp_OptionsBar = new OptionBar;

    PlayerToggleButton *resizeButton = new PlayerToggleButton;

    resizeButton->setToolTip(true, "Agrandir la fenêtre");
    resizeButton->setToolTip(false, "Réduire la fenêtre");
    resizeButton->setIcon(true, util::Tools::loadImage(QString(IMAGES_SUBDIR) + "extend.png"));
    resizeButton->setIcon(false, util::Tools::loadImage(QString(IMAGES_SUBDIR) + "reduce.png"));
    resizeButton->setAction(true, std::bind(&PlayerWindow::setMiniatureWindow, this));
    resizeButton->setAction(false, std::bind(&PlayerWindow::setDesktopWindow, this));

    mp_OptionsBar->addButton(0, resizeButton);
}

// ==============================
// ==============================

void PlayerWindow::createTopWindowPart()
{
    mp_TopPart = new QWidget;
    QGridLayout *topLayout = new QGridLayout;

    mp_Spectrum = new Spectrum(SPECTRUM_WIDTH);
    mp_Spectrum->setColor(m_SpectrumColors.at(m_CurrentSpectrumColor));

    mp_SongPicture = new QLabel;
    mp_SongPicture->setMinimumHeight(SPECTRUM_HEIGHT);
    m_DefaultSongPicture = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "default_picture.png");

    topLayout->setColumnStretch(0, 1);
    topLayout->addWidget(mp_SongTitle, 0, 0, 1, 2, Qt::AlignTop);
    topLayout->addWidget(mp_SongArtist, 1, 0, 1, 2, Qt::AlignTop);
    topLayout->addWidget(mp_SongPicture, 0, 1, 10, 1, Qt::AlignRight);
    topLayout->addWidget(mp_Spectrum, 0, 1, 10, 1);
    topLayout->addWidget(mp_OptionsBar, 0, 2, 10, 1);
    topLayout->addWidget(mp_SongList, 0, 3, 10, 1);

    mp_TopPart->setLayout(topLayout);

    mp_SongTitle->raise();
    mp_SongArtist->raise();
}

// ==============================
// ==============================

void PlayerWindow::createProgressBar()
{
    mp_ProgressBackground = new ProgressBackground;
    mp_NetworkLoadBar = new NetworkLoadBar(mp_ProgressBackground);
    mp_NetworkLoadBar->setGeometry(0, (PROGRESS_BACKGROUND_HEIGHT - LOADBAR_HEIGHT) / 2, mp_ProgressBackground->width(), 0);
    mp_ProgressBar = new ProgressBar(mp_ProgressBackground);
    mp_ProgressBar->setGeometry(0, (PROGRESS_BACKGROUND_HEIGHT - PROGRESSBAR_HEIGHT) / 2, mp_ProgressBackground->width(), 0);

    connect(mp_ProgressBar, &ProgressBar::posChanged, this, &PlayerWindow::setSongPosition);
}

// ==============================
// ==============================

void PlayerWindow::createBottomWindowPart()
{
    mp_BottomPart = new QWidget;
    mp_BottomPart->setAutoFillBackground(true);

    QGridLayout *bottomLayout = new QGridLayout;

    mp_SongPos = new PlayerLabel(QColor(21, 191, 221), 15);
    mp_SongLength = new PlayerLabel(QColor(21, 191, 221), 15);

    mp_Buttons.insert(ButtonId::PLAY, new PlayerButton("play"));
    mp_Buttons.insert(ButtonId::PAUSE, new PlayerButton("pause"));
    mp_Buttons.insert(ButtonId::STOP, new PlayerButton("stop"));
    mp_Buttons.insert(ButtonId::PREV, new PlayerButton("prev"));
    mp_Buttons.insert(ButtonId::NEXT, new PlayerButton("next"));
    mp_Buttons.insert(ButtonId::VOLUME_MORE, new PlayerButton("volumem"));
    mp_Buttons.insert(ButtonId::VOLUME_LESS, new PlayerButton("volumel"));
    mp_Buttons.insert(ButtonId::REFRESH, new PlayerButton("refresh"));

    getButton(ButtonId::PAUSE)->hide();
    getButton(ButtonId::PREV)->setPressable(true);
    getButton(ButtonId::NEXT)->setPressable(true);

    connect(getButton(ButtonId::PLAY), &PlayerButton::clicked, this, &PlayerWindow::play);
    connect(getButton(ButtonId::PAUSE), &PlayerButton::clicked, this, &PlayerWindow::pause);
    connect(getButton(ButtonId::STOP), &PlayerButton::clicked, this, &PlayerWindow::stop);
    connect(getButton(ButtonId::PREV), &PlayerButton::clicked, this, &PlayerWindow::previousSong);
    connect(getButton(ButtonId::NEXT), &PlayerButton::clicked, this, &PlayerWindow::nextSong);
    connect(getButton(ButtonId::VOLUME_MORE), &PlayerButton::clicked, this, &PlayerWindow::increaseVolume);
    connect(getButton(ButtonId::VOLUME_LESS), &PlayerButton::clicked, this, &PlayerWindow::decreaseVolume);
    connect(getButton(ButtonId::REFRESH), &PlayerButton::clicked, this, &PlayerWindow::refreshSongsList);

    mp_SoundVolume = new VolumeViewer;
    mp_SoundVolume->setState(m_Player.getVolumeState());
    connect(mp_SoundVolume, &VolumeViewer::stateChanged, this, &PlayerWindow::setMute);

    bottomLayout->setColumnStretch(4, 1);
    bottomLayout->addWidget(mp_SongPos, 3, 0);
    bottomLayout->addWidget(mp_SongLength, 3, 10, Qt::AlignRight);

    bottomLayout->addWidget(getButton(ButtonId::PLAY), 3, 6, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::PAUSE), 3, 6, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::STOP), 5, 6, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::PREV), 4, 5, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::NEXT), 4, 7, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::VOLUME_MORE), 3, 3, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::VOLUME_LESS), 5, 3, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::REFRESH), 4, 10, 2, 1);

    bottomLayout->addWidget(mp_SoundVolume, 4, 1, 2, 2);

    bottomLayout->setColumnStretch(8, 1);
    mp_BottomPart->setLayout(bottomLayout);
}

// ==============================
// ==============================

void PlayerWindow::setDesktopOptions()
{
    using namespace std::placeholders;

    PlayerToggleButton *spectrumButton = new PlayerToggleButton("spectrum.png", true);
    spectrumButton->setToolTip(true, "Masquer le spectre");
    spectrumButton->setToolTip(false, "Afficher le spectre");
    spectrumButton->setAction([this](bool enabled) {
        mp_Spectrum->setVisible(enabled);
        mp_Spectrum->lower();
    });

    PlayerToggleButton *pictureButton = new PlayerToggleButton("picture.png", true);
    pictureButton->setToolTip(true, "Masquer la pochette");
    pictureButton->setToolTip(false, "Afficher la pochette");
    pictureButton->setAction([this](bool enabled) {
        mp_SongPicture->setVisible(enabled);
        mp_SongPicture->lower();
    });

    PlayerToggleButton *listButton = new PlayerToggleButton("list.png", true);
    listButton->setToolTip(true, "Masquer la liste");
    listButton->setToolTip(false, "Afficher la liste");
    listButton->setAction(std::bind(&PlayerWindow::setListVisible, this, _1));

    for (int i = 0; i < 2; ++i)
        mp_OptionsBar->removeButton(1);

    mp_OptionsBar->addButton(0, spectrumButton);
    mp_OptionsBar->addButton(1, pictureButton);
    mp_OptionsBar->addButton(2, listButton);
}

// ==============================
// ==============================

void PlayerWindow::setMiniatureOptions()
{
    using namespace std::placeholders;

    PlayerToggleButton *toolbarButton = new PlayerToggleButton("toolbar.png", true);
    toolbarButton->setToolTip(true, "Masquer la barre d'outils");
    toolbarButton->setToolTip(false, "Afficher la barre d'outils");
    toolbarButton->setAction([this](bool enabled) {
        mp_Toolbar->setVisible(enabled);
        adjustSize();
    });

    PlayerToggleButton *frameButton = new PlayerToggleButton("border.png", true);
    frameButton->setToolTip(true, "Masquer les bordures de la fenêtre");
    frameButton->setToolTip(false, "Afficher les bordures de la fenêtre");
    frameButton->setAction(std::bind(&PlayerWindow::setWindowFrameVisible, this, _1));

    for (int i = 0; i < 3; ++i)
        mp_OptionsBar->removeButton(0);

    mp_OptionsBar->addButton(1, toolbarButton);
    mp_OptionsBar->addButton(2, frameButton);
}

// ==============================
// ==============================

void PlayerWindow::createPreviewWidget()
{
    mp_ShadowWidget = new ShadowWidget;

    mp_PreviewBar = new QProgressBar(mp_ShadowWidget);
    mp_PreviewBar->setTextVisible(false);
    mp_PreviewBar->setMaximumWidth(300);

    QVBoxLayout *shadowLayout = new QVBoxLayout;
    shadowLayout->setAlignment(Qt::AlignHCenter);
    shadowLayout->addWidget(mp_PreviewBar);

    mp_ShadowWidget->setLayout(shadowLayout);
    mp_ShadowWidget->hide();

    m_PreviewTimer.setSingleShot(true);
    m_PreviewTimer.setInterval(PREVIEW_DELAY);
    connect(&m_PreviewTimer, &QTimer::timeout, this, &PlayerWindow::startPreview);
}

// ==============================
// ==============================

void PlayerWindow::createDesktopWindow()
{
    QWidget *centralArea = new QWidget;
    QStackedLayout *mainStackedLayout = new QStackedLayout;

    QWidget *playerWidget = new QWidget;
    QVBoxLayout *playerLayout = new QVBoxLayout;
    playerLayout->setSpacing(0);
    playerLayout->setContentsMargins(0, 0, 0, 0);

    playerLayout->addWidget(mp_TopPart);
    playerLayout->addWidget(mp_ProgressBackground);
    playerLayout->addWidget(mp_BottomPart);

    playerWidget->setLayout(playerLayout);

    mainStackedLayout->addWidget(playerWidget);
    mainStackedLayout->addWidget(mp_ShadowWidget);

    centralArea->setLayout(mainStackedLayout);
    setCentralWidget(centralArea);
}

// ==============================
// ==============================

void PlayerWindow::setDesktopLayout()
{
    createTopWindowPart();
    createProgressBar();
    createPreviewWidget();
    createDesktopWindow();
    setDesktopOptions();

    mp_Toolbar->setVisible(true);
    mp_Toolbar->setMovable(true);

    getButton(ButtonId::REFRESH)->show();
    mp_SongList->setFixedWidth(LIST_WIDTH);

    mp_BottomPart->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    setWindowFrameVisible(true);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    show();
}

// ==============================
// ==============================

void PlayerWindow::setMiniatureLayout()
{
    QGridLayout *bottomLayout = static_cast<QGridLayout*>(mp_BottomPart->layout());

    setMiniatureOptions();

    bottomLayout->addWidget(mp_SongTitle, 0, 0, 1, bottomLayout->columnCount());
    bottomLayout->addWidget(mp_SongArtist, 1, 0, 1, bottomLayout->columnCount());
    bottomLayout->addWidget(mp_OptionsBar, 0, bottomLayout->columnCount(), bottomLayout->rowCount(), 1);

    mp_Toolbar->setMovable(false);
    getButton(ButtonId::REFRESH)->hide();

    setCentralWidget(mp_BottomPart);
    mp_BottomPart->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    adjustSize();
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(minimumSize());
    show();
}

// ==============================
// ==============================

PlayerButton* PlayerWindow::getButton(ButtonId id) const
{
    return mp_Buttons[id];
}

// ==============================
// ==============================

void PlayerWindow::setListVisible(bool visible)
{
    int w = (visible ? LIST_WIDTH : HIDDEN_LIST_WIDTH);
    int ms = std::abs(w - mp_SongList->width()) / TAB_ANIMATION_SPEED;      // t = d/v

    QPropertyAnimation *moveAnimation = new QPropertyAnimation(mp_SongList, "width");
    moveAnimation->setDuration(ms);
    moveAnimation->setStartValue(mp_SongList->width());
    moveAnimation->setEndValue(w);

    moveAnimation->start();
}

// ==============================
// ==============================

void PlayerWindow::setWindowFrameVisible(bool visible)
{
    if (visible)
        setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
    else
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    show();
}

// ==============================
// ==============================

void PlayerWindow::setState(PlayerState state)
{
    if (state == PlayerState::PLAY)
    {
        m_Player.play();
        m_PlayerTimer.start();
    }
    else
    {
        if (m_PlayerTimer.isValid())
        {
            saveListeningTime();
            m_PlayerTimer.invalidate();
        }

        if (state == PlayerState::PAUSE)
            m_Player.pause();
        else if (state == PlayerState::STOP)
        {
            m_Player.stop();
            mp_SongPos->setText(util::Tools::msToString(0));

            if (m_CurrentMode != PlayerMode::MINIATURE)
            {
                mp_Spectrum->clear();
                mp_NetworkLoadBar->setValue(0);
                mp_NetworkLoadBar->setStartPos(0);
                mp_ProgressBar->setValue(0);
            }
        }
    }

    getButton(ButtonId::PLAY)->setHidden(m_Player.isPlaying());
    getButton(ButtonId::PAUSE)->setHidden(!m_Player.isPlaying());
}

// ==============================
// ==============================

void PlayerWindow::updateCurrentSong()
{
    if (m_CurrentMode != PlayerMode::MINIATURE)
    {
        mp_SongPicture->clear();
        mp_ProgressBar->setValue(0);
        mp_NetworkLoadBar->setValue(0);
        mp_NetworkLoadBar->setStartPos(0);
    }

    if (m_Player.getCurrentSong())
    {
        mp_SongTitle->setText(m_Player.getCurrentSong()->getTitle());
        mp_SongArtist->setText(m_Player.getCurrentSong()->getArtist());

        if (m_CurrentMode != PlayerMode::MINIATURE)
        {
            if (m_Player.getCurrentSong()->isAvailable())
            {
                mp_SongPicture->setPixmap(m_Player.getCurrentSong()->buildPicture());

                if (!mp_SongPicture->pixmap()->isNull() && mp_SongPicture->pixmap()->width() > 400)
                    mp_SongPicture->setPixmap(mp_SongPicture->pixmap()->scaledToWidth(400));
            }

            if (!mp_SongPicture->pixmap() || mp_SongPicture->pixmap()->isNull())
                mp_SongPicture->setPixmap(m_DefaultSongPicture);

            mp_ProgressBar->setMaximum(m_Player.getCurrentSong()->getLength());

            if (m_Player.getCurrentSong()->isRemote())
                mp_NetworkLoadBar->setMaximum(mp_Socket->getTotalCurrentSongData());

            mp_SongList->setCurrentSong(m_Player.getCurrentSong()->getId());
        }

        mp_SongLength->setText(util::Tools::msToString(m_Player.getCurrentSong()->getLength()));
    }
    else
    {
        mp_SongTitle->clear();
        mp_SongArtist->clear();
        mp_SongPos->setText(util::Tools::msToString(0));
        mp_SongLength->setText(util::Tools::msToString(0));
    }

    if (m_Player.isPaused())
        m_Player.stop();

    getButton(ButtonId::PREV)->unpress();
    getButton(ButtonId::NEXT)->unpress();
}

// ==============================
// ==============================

void PlayerWindow::refreshSongsList()
{
    mp_SongList->clearList(SongList_t::DIRECTORY_SONGS);

    SongTreeRoot *songTree = m_Player.reloadSongs(SONGS_SUBDIR);
    mp_SongList->addTree(SongList_t::DIRECTORY_SONGS, songTree);

    m_Player.firstSong();

    if (!m_Player.isStopped())
        m_Player.stop();
}

// ==============================
// ==============================

void PlayerWindow::play()
{
    m_Player.play();
}

// ==============================
// ==============================

void PlayerWindow::pause()
{
    m_Player.pause();
}

// ==============================
// ==============================

void PlayerWindow::stop()
{
    m_Player.stop();
}

// ==============================
// ==============================

void PlayerWindow::previousSong()
{
    m_Player.previousSong();
}

// ==============================
// ==============================

void PlayerWindow::nextSong()
{
    m_Player.nextSong();
}

// ==============================
// ==============================

void PlayerWindow::setVolume(int volume)
{
    m_Player.setVolume(volume);

    if (!m_Player.isMuted())
        mp_SoundVolume->setState(volume);
}

// ==============================
// ==============================

void PlayerWindow::increaseVolume()
{
    int previousVolume = m_Player.getVolumeState();

    if (previousVolume < (NB_VOLUME_STATES - 1))
        setVolume(previousVolume + 1);
}

// ==============================
// ==============================

void PlayerWindow::decreaseVolume()
{
    int previousVolume = m_Player.getVolumeState();

    if (previousVolume > 0)
        setVolume(previousVolume - 1);
}

// ==============================
// ==============================

void PlayerWindow::setSongPosition(double value)
{
    if (!m_Player.isStopped() && m_Player.getCurrentSong())
    {
        audio::SoundPos_t pos = value * m_Player.getCurrentSong()->getLength() / 100;

        m_Player.getCurrentSong()->setPosition(pos);
        mp_SongPos->setText(util::Tools::msToString(pos));

        if (m_CurrentMode != PlayerMode::MINIATURE)
        {
            mp_ProgressBar->setValue(pos);

            if (m_Player.getCurrentSong()->isRemote())
            {
                mp_NetworkLoadBar->setValue(0);
                mp_NetworkLoadBar->setStartPos(value);
            }
        }
    }
}

// ==============================
// ==============================

void PlayerWindow::moveSongPosition(int offset)
{
    int newAudioPos = m_Player.getCurrentSong()->getPosition() + offset;

    double newPos = newAudioPos * 100.0 / m_Player.getCurrentSong()->getLength();
    newPos = std::max(0.0, std::min(newPos, 100.0));

    setSongPosition(newPos);
}

// ==============================
// ==============================

void PlayerWindow::setMute()
{
    m_Player.mute(!m_Player.isMuted());

    if (m_Player.isMuted())
        mp_SoundVolume->setState(MUTE_STATE);
    else
        mp_SoundVolume->setState(m_Player.getVolumeState());
}

// ==============================
// ==============================

void PlayerWindow::importSong()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Ajouter une musique", QString(), "Musiques (*.mp3 *.wav *.ogg *.wma)");

    if (!filePath.isNull())
    {
        SongListItem *songItem = m_Player.addNewSong(IMPORTED_SONGS, filePath);

        if (songItem)
            mp_SongList->addSong(LOCAL_SONGS, songItem);
    }
}

// ==============================
// ==============================

void PlayerWindow::openSongsDir() const
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(SONGS_SUBDIR).absoluteFilePath()));
}

// ==============================
// ==============================

void PlayerWindow::openInformation()
{
    AboutDialog informationWindow(this);
    informationWindow.setWindowTitle("A propos");
    informationWindow.exec();
}

// ==============================
// ==============================

void PlayerWindow::openConnection()
{
    m_ConnectionDialog.exec();
}

// ==============================
// ==============================

void PlayerWindow::openSpectrumColorDialog()
{
    SpectrumColorDialog colorDialog(m_SpectrumColors);
    colorDialog.setCurrentColor(m_CurrentSpectrumColor);

    connect(&colorDialog, &SpectrumColorDialog::colorSelected, [this](int index) {
        m_CurrentSpectrumColor = index;
        mp_Spectrum->setColor(m_SpectrumColors.at(index));
    });

    connect(&colorDialog, &SpectrumColorDialog::colorAdded, [this](const SpectrumColor& color) {
        m_SpectrumColors.append(color);
    });

    connect(&colorDialog, &SpectrumColorDialog::colorChanged, [this](int index, const SpectrumColor& color) {
        m_SpectrumColors[index] = color;
        if (m_CurrentSpectrumColor == index)
            mp_Spectrum->setColor(m_SpectrumColors.at(index));
    });

    connect(&colorDialog, &SpectrumColorDialog::colorRemoved, [this](int index) {
        m_SpectrumColors.removeAt(index);

        if (m_CurrentSpectrumColor == index)
        {
            m_CurrentSpectrumColor = 0;
            mp_Spectrum->setColor(m_SpectrumColors.at(m_CurrentSpectrumColor));
        }
        else if (m_CurrentSpectrumColor > index)
            m_CurrentSpectrumColor--;
    });

    colorDialog.exec();
    mp_Spectrum->update();
}

// ==============================
// ==============================

void PlayerWindow::openProfileDialog()
{
    ProfileDialog profileDialog(m_ProfileManager, this);
    profileDialog.setWindowTitle("Profil");
    profileDialog.exec();
}

// ==============================
// ==============================

void PlayerWindow::listen()
{
    mp_Socket = std::make_unique<network::PlayerSocket>(&m_Player);
    connect(mp_Socket.get(), &network::PlayerSocket::connected, this, &PlayerWindow::startConnection);
    connect(mp_Socket.get(), &network::PlayerSocket::disconnected, this, &PlayerWindow::closeConnection);

    mp_Socket->listen(QHostAddress::Any);
}

// ==============================
// ==============================

void PlayerWindow::connectToHost(const QString& host)
{
    mp_Socket = std::make_unique<network::PlayerSocket>(&m_Player);
    connect(mp_Socket.get(), &network::PlayerSocket::connected, this, &PlayerWindow::startConnection);
    connect(mp_Socket.get(), &network::PlayerSocket::disconnected, this, &PlayerWindow::closeConnection);

    mp_Socket->connectToHost(host);
}

// ==============================
// ==============================

void PlayerWindow::startConnection()
{
    SongTreeRoot *remoteSongList = mp_Socket->exchangeSongList(mp_SongList->getSongHierarchy());
    mp_SongList->addTree(SongList_t::REMOTE_SONGS, remoteSongList);

    m_ConnectionDialog.connected();
    mp_ConnectionState->setPixmap(m_ConnectedIcon);
    mp_ConnectionState->setToolTip("Connecté");

    connect(mp_Socket.get(), &network::PlayerSocket::commandReceived, &m_Player, &audio::Player::executeNetworkCommand);
    connect(&m_Player, &audio::Player::commandExecuted, mp_Socket.get(), &network::PlayerSocket::sendCommandReply);
}

// ==============================
// ==============================

void PlayerWindow::closeConnection()
{
    if (mp_Socket)
    {
        if (mp_Socket->isConnected())
            mp_Socket->disconnection();
        else
        {
            if (!m_Player.getCurrentSong())
            {
                if (!m_Player.isStopped())
                    m_Player.stop();
            }
            else if (m_Player.getCurrentSong()->isRemote())
                m_Player.firstSong();

            // close clientFile (if current song is remote only ? do we close file after remote song playing ?)

            m_Player.clearSongs(SongList_t::REMOTE_SONGS);
            mp_SongList->clearList(SongList_t::REMOTE_SONGS);

            mp_Socket.reset(nullptr);

            m_ConnectionDialog.disconnect();
            mp_ConnectionState->setPixmap(m_DisconnectedIcon);
            mp_ConnectionState->setToolTip("Déconnecté");
        }
    }
}

// ==============================
// ==============================

void PlayerWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != m_TimerId)
    {
        QMainWindow::timerEvent(event);
        return;
    }

    m_Player.update();

    if (m_Player.isPlaying())
    {
        if (m_CurrentMode != PlayerMode::MINIATURE)
        {
            if (mp_Spectrum->isVisible())
                mp_Spectrum->updateValues(m_Player.getCurrentSong()->getSoundID());

            mp_ProgressBar->setPosition(m_Player.getCurrentSong()->getPosition());

            if (m_Player.getCurrentSong()->isRemote())
                mp_NetworkLoadBar->setValue(mp_Socket->getSongDataReceived());
        }

        mp_SongPos->setText(util::Tools::msToString(m_Player.getCurrentSong()->getPosition()));

        if (getButton(ButtonId::PREV)->isPressed())
            moveSongPosition(-MOVE_INTERVAL);
        else if (getButton(ButtonId::NEXT)->isPressed())
            moveSongPosition(MOVE_INTERVAL);
    }

    if (mp_Socket && mp_Socket->isConnected())
        mp_Socket->processCommands();

    if (m_Player.isPreviewing())
    {
        if (m_Player.getPreviewPosition() < PREVIEW_LENGTH)
            mp_PreviewBar->setValue(m_Player.getPreviewPosition());
        else
            stopPreview();
    }

    if (m_PlayerTimer.isValid() && m_PlayerTimer.elapsed() > 60000)
    {
        saveListeningTime();
        m_PlayerTimer.restart();
    }
}

// ==============================
// ==============================

void PlayerWindow::showEvent(QShowEvent* /*event*/)
{
    m_TimerId = startTimer(REFRESH_TIME_MS);
}

// ==============================
// ==============================

void PlayerWindow::hideEvent(QHideEvent* /*event*/)
{
    killTimer(m_TimerId);
    m_TimerId = -1;
}

// ==============================
// ==============================

void PlayerWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if (m_CurrentMode != PlayerMode::MINIATURE)
    {
        mp_NetworkLoadBar->resize(event->size().width(), mp_NetworkLoadBar->height());
        mp_ProgressBar->resize(event->size().width(), mp_ProgressBar->height());
    }
}

// ==============================
// ==============================

void PlayerWindow::setDesktopWindow()
{
    m_CurrentMode = PlayerMode::DESKTOP;

    setDesktopLayout();
    menuBar()->show();

    updateCurrentSong();
    setAcceptDrops(true);
}

// ==============================
// ==============================

void PlayerWindow::setMiniatureWindow()
{
    m_CurrentMode = PlayerMode::MINIATURE;
    mp_SongList->setParent(0);
    m_DefaultSongPicture = QPixmap();

    setMiniatureLayout();
    menuBar()->hide();

    setAcceptDrops(false);
}

// ==============================
// ==============================

void PlayerWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(event->rect(), Qt::black);

    QPalette pal(mp_BottomPart->palette());

    QLinearGradient gradient(0, 0, 0, mp_BottomPart->height());
    gradient.setColorAt(0.0, QColor(66, 66, 66));
    gradient.setColorAt(0.5, QColor(69, 69, 69));
    gradient.setColorAt(0.6, QColor(60, 60, 60));
    gradient.setColorAt(1.0, QColor(24, 24, 24));

    pal.setBrush(QPalette::Window, gradient);
    mp_BottomPart->setPalette(pal);
}

// ==============================
// ==============================

void PlayerWindow::startPreview()
{
    if (m_Player.isPreviewing())
        return;

    m_Player.startPreview(m_PreviewPath);

    mp_PreviewBar->setValue(0);
    mp_PreviewBar->setMaximum(std::min(m_Player.getPreviewLength(), PREVIEW_LENGTH));
    mp_ShadowWidget->show();
    mp_ShadowWidget->setGeometry(QRect(0, 0, centralWidget()->width(), centralWidget()->height()));

    QPropertyAnimation *shadowAnimation = new QPropertyAnimation(mp_ShadowWidget, "opacity");
    shadowAnimation->setDuration(PREVIEW_ANIMATION_LENGTH);
    shadowAnimation->setStartValue(0);
    shadowAnimation->setEndValue(200);
    shadowAnimation->start();
}

// ==============================
// ==============================

void PlayerWindow::stopPreview()
{
    m_PreviewTimer.stop();

    if (m_Player.isPreviewing())
        m_Player.stopPreview();

    mp_ShadowWidget->hide();
}

// ==============================
// ==============================

void PlayerWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->urls().size() == 1)
    {
        const QString& filepath = event->mimeData()->urls().first().toLocalFile();

        if (util::Tools::getMimeType(filepath).startsWith("audio/"))
        {
            event->acceptProposedAction();

            m_PreviewTimer.start();
            m_PreviewPath = filepath;
        }
    }
}

// ==============================
// ==============================

void PlayerWindow::dragLeaveEvent(QDragLeaveEvent* /*event*/)
{
    stopPreview();
}

// ==============================
// ==============================

void PlayerWindow::dropEvent(QDropEvent *event)
{
    for (const QUrl& url : event->mimeData()->urls())
    {
        SongListItem *songItem = m_Player.addNewSong(IMPORTED_SONGS, url.toLocalFile());

        if (songItem)
            mp_SongList->addSong(LOCAL_SONGS, songItem);
    }

    event->acceptProposedAction();
    stopPreview();
}

// ==============================
// ==============================

void PlayerWindow::saveListeningTime()
{
    unsigned int elapsedSeconds = m_PlayerTimer.elapsed() / 1000;
    m_ProfileManager.setListeningTime(m_ProfileManager.getListeningTime() + elapsedSeconds);

    if (!m_ProfileManager.save())
        QMessageBox::warning(this, "Erreur de sauvegarde", "Le profil n'a pas pu être sauvegardé.");
}


} // gui
