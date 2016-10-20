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
#include "AboutDialog.h"

#include <QGridLayout>
#include <QPalette>
#include "MenuBar.h"
#include <QToolBar>
#include <QApplication>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMimeData>
#include <QStackedLayout>
#include <QPropertyAnimation>


namespace gui {


PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent), m_TimerId(0), mp_Socket(nullptr)
{
    setWindowTitle(tr(WINDOW_TITLE));
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    setAcceptDrops(true);

    QWidget *centralArea = new QWidget;
    QStackedLayout *mainStackedLayout = new QStackedLayout;

    QWidget *playerWidget = new QWidget;
    QVBoxLayout *playerLayout = new QVBoxLayout;
    playerLayout->setSpacing(0);
    playerLayout->setContentsMargins(0, 0, 0, 0);

    connect(&m_Player, SIGNAL(songChanged()), this, SLOT(updateCurrentSong()));
    connect(&m_Player, SIGNAL(stateChanged(PlayerState)), this, SLOT(setState(PlayerState)));
    connect(&m_Player, SIGNAL(previewFinished()), this, SLOT(stopPreview()));

    /** Menu **/

    setMenuBar(new MenuBar());

    QToolBar *toolbar = addToolBar("toolbar");
    toolbar->setStyleSheet("background-color: grey;");

    toolbar->addAction(static_cast<MenuBar*>(menuBar())->getAddingSongAction());
    toolbar->addAction(static_cast<MenuBar*>(menuBar())->getOpenAction());
    toolbar->addAction(static_cast<MenuBar*>(menuBar())->getQuitAction());
    toolbar->addAction(static_cast<MenuBar*>(menuBar())->getAboutAction());

    connect(static_cast<MenuBar*>(menuBar())->getAddingSongAction(), SIGNAL(triggered()), this, SLOT(importSong()));
    connect(static_cast<MenuBar*>(menuBar())->getOpenAction(), SIGNAL(triggered()), this, SLOT(openSongsDir()));
    connect(static_cast<MenuBar*>(menuBar())->getAboutAction(), SIGNAL(triggered()), this, SLOT(openInformation()));
    connect(static_cast<MenuBar*>(menuBar())->getQuitAction(), SIGNAL(triggered()), qApp, SLOT(quit()));


    /** Partie du haut **/

    mp_TopPart = new QWidget;
    QGridLayout *topLayout = new QGridLayout;

    mp_SongTitle = new PlayerLabel(Qt::white, 20);
    mp_SongArtist = new PlayerLabel(Qt::white, 14);
    mp_SongTitle->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    mp_SongArtist->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    mp_Spectrum = new Spectrum(SPECTRUM_WIDTH);
    mp_SongPicture = new QLabel;
    mp_SongList = new SongList;

    connect(mp_SongList, SIGNAL(songPressed(audio::Player::SongId)), &m_Player, SLOT(changeSong(audio::Player::SongId)));
    connect(&m_Player, SIGNAL(streamError(audio::Player::SongId)), mp_SongList, SLOT(disableSong(audio::Player::SongId)));


    topLayout->setColumnStretch(0, 1);
    topLayout->addWidget(mp_SongTitle, 0, 0, 1, 2, Qt::AlignTop);
    topLayout->addWidget(mp_SongArtist, 1, 0, 1, 2, Qt::AlignTop);
    topLayout->addWidget(mp_SongPicture, 0, 1, 10, 1, Qt::AlignHCenter);
    topLayout->addWidget(mp_Spectrum, 0, 1, 10, 1);
    topLayout->addWidget(mp_SongList, 0, 2, 10, 1);

    mp_TopPart->setLayout(topLayout);

    mp_SongTitle->raise();
    mp_SongArtist->raise();


    mp_ProgressBackground = new ProgressBackground;
    mp_NetworkLoadBar = new NetworkLoadBar(mp_ProgressBackground);
    mp_NetworkLoadBar->setGeometry(0, (PROGRESS_BACKGROUND_HEIGHT - LOADBAR_HEIGHT) / 2, mp_ProgressBackground->width(), 0);
    mp_ProgressBar = new ProgressBar(mp_ProgressBackground);
    mp_ProgressBar->setGeometry(0, (PROGRESS_BACKGROUND_HEIGHT - PROGRESSBAR_HEIGHT) / 2, mp_ProgressBackground->width(), 0);

    connect(mp_ProgressBar, SIGNAL(posChanged(int)), this, SLOT(setSongPosition(int)));


    /** Partie du bas **/

    mp_BottomPart = new QWidget;
    mp_BottomPart->setAutoFillBackground(true);

    QGridLayout *bottomLayout = new QGridLayout;

    mp_SongPos = new PlayerLabel(QColor(21, 191, 221), 15);
    mp_SongLength = new PlayerLabel(QColor(21, 191, 221), 15);

    mp_Buttons << new PlayerButton("play") << new PlayerButton("pause") << new PlayerButton("stop")
                << new PlayerButton("prev") << new PlayerButton("next") << new PlayerButton("volumem")
                << new PlayerButton("volumel") << new PlayerButton("refresh");

    getButton(ButtonId::PAUSE)->hide();

    connect(getButton(ButtonId::PLAY), SIGNAL(clicked()), this, SLOT(play()));
    connect(getButton(ButtonId::PAUSE), SIGNAL(clicked()), this, SLOT(pause()));
    connect(getButton(ButtonId::STOP), SIGNAL(clicked()), this, SLOT(stop()));
    connect(getButton(ButtonId::PREV), SIGNAL(clicked()), this, SLOT(previousSong()));
    connect(getButton(ButtonId::NEXT), SIGNAL(clicked()), this, SLOT(nextSong()));
    connect(getButton(ButtonId::VOLUME_MORE), SIGNAL(clicked()), this, SLOT(increaseVolume()));
    connect(getButton(ButtonId::VOLUME_LESS), SIGNAL(clicked()), this, SLOT(decreaseVolume()));
    connect(getButton(ButtonId::REFRESH), SIGNAL(clicked()), this, SLOT(refreshSongsList()));

    mp_SoundVolume = new VolumeViewer;
    mp_SoundVolume->setImage(m_Player.getVolumeState());
    connect(mp_SoundVolume, SIGNAL(stateChanged()), this, SLOT(setMute()));

    mp_ConnectionBox = new ConnectionBox;
    connect(mp_ConnectionBox, SIGNAL(listened()), this, SLOT(listen()));
    connect(mp_ConnectionBox, SIGNAL(connectedToHost(QString)), this, SLOT(connectToHost(QString)));
    connect(mp_ConnectionBox, SIGNAL(canceled()), this, SLOT(closeConnection()));
    connect(mp_ConnectionBox, SIGNAL(disconnected()), this, SLOT(closeConnection()));


    bottomLayout->setColumnStretch(3, 1);
    bottomLayout->addWidget(mp_SongPos, 0, 0);
    bottomLayout->addWidget(mp_SongLength, 0, 9, Qt::AlignRight);

    bottomLayout->addWidget(getButton(ButtonId::PLAY), 0, 5, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::PAUSE), 0, 5, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::STOP), 2, 5, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::PREV), 1, 4, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::NEXT), 1, 6, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::VOLUME_MORE), 0, 3, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::VOLUME_LESS), 2, 3, 2, 1);
    bottomLayout->addWidget(getButton(ButtonId::REFRESH), 1, 9, 2, 1);

    bottomLayout->addWidget(mp_SoundVolume, 1, 1, 2, 2);
    bottomLayout->addWidget(mp_ConnectionBox, 3, 8, 1, 2);

    bottomLayout->setColumnStretch(8, 1);
    mp_BottomPart->setLayout(bottomLayout);


    playerLayout->addWidget(mp_TopPart);
    playerLayout->addWidget(mp_ProgressBackground);
    playerLayout->addWidget(mp_BottomPart);

    playerWidget->setLayout(playerLayout);

    createPreviewWidget();

    mainStackedLayout->addWidget(playerWidget);
    mainStackedLayout->addWidget(mp_ShadowWidget);

    centralArea->setLayout(mainStackedLayout);
    setCentralWidget(centralArea);

    /** Démarrage du player **/

    refreshSongsList();
    setState(PlayerState::PLAY);
}

// ==============================
// ==============================

PlayerWindow::~PlayerWindow()
{
    audio::FmodManager::deleteInstance();
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
    connect(&m_PreviewTimer, SIGNAL(timeout()), this, SLOT(startPreview()));
}

// ==============================
// ==============================

PlayerButton* PlayerWindow::getButton(ButtonId id) const
{
    return mp_Buttons.at(static_cast<int>(id));
}

// ==============================
// ==============================

void PlayerWindow::setState(PlayerState state)
{
    if (state == PlayerState::PLAY)
        m_Player.play();
    else if (state == PlayerState::PAUSE)
        m_Player.pause();
    else if (state == PlayerState::STOP)
    {
        m_Player.stop();
        mp_Spectrum->clear();
        mp_NetworkLoadBar->setValue(0);
        mp_NetworkLoadBar->setStartPos(0);
        mp_ProgressBar->setValue(0);
        mp_SongPos->setText(util::Tools::msToString(0));
    }

    getButton(ButtonId::PLAY)->setHidden(m_Player.isPlaying());
    getButton(ButtonId::PAUSE)->setHidden(!m_Player.isPlaying());
}

// ==============================
// ==============================

void PlayerWindow::updateCurrentSong()
{
    if (m_Player.getCurrentSong())
    {
        mp_SongTitle->setText(m_Player.getCurrentSong()->getTitle());
        mp_SongArtist->setText(m_Player.getCurrentSong()->getArtist());

        if (m_Player.getCurrentSong()->isAvailable())
        {
            mp_SongPicture->setPixmap(m_Player.getCurrentSong()->buildPicture());

            if (!mp_SongPicture->pixmap()->isNull() && mp_SongPicture->pixmap()->width() > 400)
                mp_SongPicture->setPixmap(mp_SongPicture->pixmap()->scaledToWidth(400));
        }
        else
            mp_SongPicture->clear();

        mp_SongLength->setText(util::Tools::msToString(m_Player.getCurrentSong()->getLength()));

        mp_ProgressBar->setValue(0);
        mp_ProgressBar->setMaximum(m_Player.getCurrentSong()->getLength());
        mp_NetworkLoadBar->setValue(0);
        mp_NetworkLoadBar->setStartPos(0);

        if (m_Player.getCurrentSong()->isRemote())
            mp_NetworkLoadBar->setMaximum(mp_Socket->getTotalCurrentSongData());

        mp_SongList->setCurrentSong(m_Player.getCurrentSong()->getId());
    }

    if (m_Player.isPaused())
        setState(PlayerState::STOP);
}

// ==============================
// ==============================

void PlayerWindow::refreshSongsList()
{
    mp_SongList->clearList(DIRECTORY_SONGS);

    SongTreeRoot *songTree = m_Player.reloadSongs(SONGS_SUBDIR);
    mp_SongList->addTree(LOCAL_SONGS, songTree);

    m_Player.firstSong();

    if (!m_Player.isStopped())
      setState(PlayerState::STOP);
}

// ==============================
// ==============================

void PlayerWindow::play()
{
    setState(PlayerState::PLAY);
}

// ==============================
// ==============================

void PlayerWindow::pause()
{
    setState(PlayerState::PAUSE);
}

// ==============================
// ==============================

void PlayerWindow::stop()
{
    setState(PlayerState::STOP);
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
        mp_SoundVolume->setImage(volume);
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

void PlayerWindow::setSongPosition(int value)
{
    if (!m_Player.isStopped() && m_Player.getCurrentSong())
    {
        audio::SoundPos_t pos = value * m_Player.getCurrentSong()->getLength() / 100;

        m_Player.getCurrentSong()->setPosition(pos);
        mp_ProgressBar->setValue(pos);
        mp_SongPos->setText(util::Tools::msToString(pos));

        if (m_Player.getCurrentSong()->isRemote())
        {
            mp_NetworkLoadBar->setValue(0);
            mp_NetworkLoadBar->setStartPos(value);
        }
    }
}

// ==============================
// ==============================

void PlayerWindow::setMute()
{
    m_Player.mute(!m_Player.isMuted());

    if (m_Player.isMuted())
        mp_SoundVolume->setImage(MUTE_STATE);
    else
        mp_SoundVolume->setImage(m_Player.getVolumeState());
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

void PlayerWindow::listen()
{
    mp_Socket = std::make_unique<network::PlayerSocket>(&m_Player);
    connect(mp_Socket.get(), SIGNAL(connected()), this, SLOT(startConnection()));
    connect(mp_Socket.get(), SIGNAL(disconnected()), this, SLOT(closeConnection()));

    mp_Socket->listen(QHostAddress::Any);
}

// ==============================
// ==============================

void PlayerWindow::connectToHost(const QString& host)
{
    mp_Socket = std::make_unique<network::PlayerSocket>(&m_Player);
    connect(mp_Socket.get(), SIGNAL(connected()), this, SLOT(startConnection()));
    connect(mp_Socket.get(), SIGNAL(disconnected()), this, SLOT(closeConnection()));

    mp_Socket->connectToHost(host);
}

// ==============================
// ==============================

void PlayerWindow::startConnection()
{
    SongTreeRoot *remoteSongList = mp_Socket->exchangeSongList(mp_SongList->getSongHierarchy());
    mp_SongList->addTree(SongList_t::REMOTE_SONGS, remoteSongList);

    mp_ConnectionBox->connected();

    connect(mp_Socket.get(), SIGNAL(commandReceived(std::shared_ptr<network::commands::CommandRequest>)), &m_Player, SLOT(executeNetworkCommand(std::shared_ptr<network::commands::CommandRequest>)));
    connect(&m_Player, SIGNAL(commandExecuted(std::shared_ptr<network::commands::CommandReply>)), mp_Socket.get(), SLOT(sendCommandReply(std::shared_ptr<network::commands::CommandReply>)));
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
                    setState(PlayerState::STOP);
            }
            else if (m_Player.getCurrentSong()->isRemote())
                m_Player.firstSong();

            // close clientFile (if current song is remote only ? do we close file after remote song playing ?)

            m_Player.clearSongs(SongList_t::REMOTE_SONGS);
            mp_SongList->clearList(SongList_t::REMOTE_SONGS);

            mp_Socket.reset(nullptr);
            mp_ConnectionBox->disconnect();
        }
    }
}

// ==============================
// ==============================

void PlayerWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_TimerId)
    {
        m_Player.update();

        if (m_Player.isPlaying())
        {
            mp_Spectrum->updateValues(m_Player.getCurrentSong()->getSoundID());
            mp_ProgressBar->setValue(m_Player.getCurrentSong()->getPosition());

            mp_SongPos->setText(util::Tools::msToString(m_Player.getCurrentSong()->getPosition()));

            if (m_Player.getCurrentSong()->isRemote())
                mp_NetworkLoadBar->setValue(mp_Socket->getSongDataReceived());
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
    }
    else
    {
        QWidget::timerEvent(event);
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
}

// ==============================
// ==============================

void PlayerWindow::resizeEvent(QResizeEvent* event)
{
    mp_NetworkLoadBar->resize(event->size().width(), mp_NetworkLoadBar->height());
    mp_ProgressBar->resize(event->size().width(), mp_ProgressBar->height());
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


} // gui
