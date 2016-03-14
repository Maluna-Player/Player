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
#include "Tools.h"
#include "AboutDialog.h"

#include <QGridLayout>
#include <QPalette>
#include "MenuBar.h"
#include <QToolBar>
#include <QApplication>
#include <QFileDialog>
#include <QDesktopServices>


PlayerWindow::PlayerWindow(QWidget *parent)
    : QMainWindow(parent), m_TimerId(0), mp_Socket(0)
{
    setWindowTitle(tr(Constants::WINDOW_TITLE.c_str()));
    resize(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);

    QWidget *centralArea = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

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

    mp_Spectrum = new Spectrum(Constants::SPECTRUM_WIDTH);
    mp_SongPicture = new QLabel;
    mp_SongList = new SongList;

    connect(mp_SongList, SIGNAL(songPressed(int)), this, SLOT(changeSong(int)));

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
    mp_NetworkLoadBar->setGeometry(0, (Constants::PROGRESS_BACKGROUND_HEIGHT - Constants::LOADBAR_HEIGHT) / 2, mp_ProgressBackground->width(), 0);
    mp_ProgressBar = new ProgressBar(mp_ProgressBackground);
    mp_ProgressBar->setGeometry(0, (Constants::PROGRESS_BACKGROUND_HEIGHT - Constants::PROGRESSBAR_HEIGHT) / 2, mp_ProgressBackground->width(), 0);

    connect(mp_ProgressBar, SIGNAL(posChanged(int)), this, SLOT(setSongPosition(int)));


    /** Partie du bas **/

    mp_BottomPart = new QWidget;
    mp_BottomPart->setAutoFillBackground(true);

    QGridLayout *bottomLayout = new QGridLayout;

    mp_SongPos = new PlayerLabel(QColor(21, 191, 221), 15);
    mp_SongLength = new PlayerLabel(QColor(21, 191, 221), 15);

    mp_Buttons << new PlayerButton("play") << new PlayerButton("pause") << new PlayerButton("stop")
                << new PlayerButton("prev") << new PlayerButton("next") << new PlayerButton("volumem")
                << new PlayerButton("volumel");

    mp_Buttons.at(PAUSE_BUTTON)->hide();

    connect(mp_Buttons.at(PLAY_BUTTON), SIGNAL(clicked()), this, SLOT(play()));
    connect(mp_Buttons.at(PAUSE_BUTTON), SIGNAL(clicked()), this, SLOT(pause()));
    connect(mp_Buttons.at(STOP_BUTTON), SIGNAL(clicked()), this, SLOT(stop()));
    connect(mp_Buttons.at(PREV_BUTTON), SIGNAL(clicked()), this, SLOT(previousSong()));
    connect(mp_Buttons.at(NEXT_BUTTON), SIGNAL(clicked()), this, SLOT(nextSong()));
    connect(mp_Buttons.at(VOLUME_MORE_BUTTON), SIGNAL(clicked()), this, SLOT(increaseVolume()));
    connect(mp_Buttons.at(VOLUME_LESS_BUTTON), SIGNAL(clicked()), this, SLOT(decreaseVolume()));

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

    bottomLayout->addWidget(mp_Buttons.at(PLAY_BUTTON), 0, 5, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(PAUSE_BUTTON), 0, 5, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(STOP_BUTTON), 2, 5, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(PREV_BUTTON), 1, 4, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(NEXT_BUTTON), 1, 6, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(VOLUME_MORE_BUTTON), 0, 3, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(VOLUME_LESS_BUTTON), 2, 3, 2, 1);

    bottomLayout->addWidget(mp_SoundVolume, 1, 1, 2, 2);
    bottomLayout->addWidget(mp_ConnectionBox, 3, 8, 1, 2);

    bottomLayout->setColumnStretch(7, 1);
    mp_BottomPart->setLayout(bottomLayout);


    layout->addWidget(mp_TopPart);
    layout->addWidget(mp_ProgressBackground);
    layout->addWidget(mp_BottomPart);
    centralArea->setLayout(layout);

    setCentralWidget(centralArea);


    /** Démarrage du player **/

    refreshSongsList();
    setState(Constants::PLAY_STATE);
}

// ==============================
// ==============================

PlayerWindow::~PlayerWindow()
{
    FmodManager::deleteInstance();
}

// ==============================
// ==============================

void PlayerWindow::setState(Constants::PlayerState_t state)
{
    if (state == Constants::PLAY_STATE)
        m_Player.play();
    else if (state == Constants::PAUSE_STATE)
        m_Player.pause();
    else if (state == Constants::STOP_STATE)
    {
        m_Player.stop();
        mp_Spectrum->updateValues(m_Player.getCurrentSong().getSoundID());
        mp_NetworkLoadBar->setValue(0);
        mp_NetworkLoadBar->setStartPos(0);
        mp_ProgressBar->setValue(m_Player.getCurrentSong().getPosition());
        mp_SongPos->setText(Tools::msToString(0));
    }

    mp_Buttons.at(PLAY_BUTTON)->setHidden(m_Player.isPlaying());
    mp_Buttons.at(PAUSE_BUTTON)->setHidden(!m_Player.isPlaying());
}

// ==============================
// ==============================

void PlayerWindow::changeSong(int song)
{
    if (song != UNDEFINED_SONG)
    {
        try
        {
            m_Player.changeSong(song);

            mp_SongTitle->setText(m_Player.getCurrentSong().getTitle());
            mp_SongArtist->setText(m_Player.getCurrentSong().getArtist());
            mp_SongPicture->setPixmap(m_Player.getCurrentSong().buildPicture());

            if (!mp_SongPicture->pixmap()->isNull() && mp_SongPicture->pixmap()->width() > 400)
                mp_SongPicture->setPixmap(mp_SongPicture->pixmap()->scaledToWidth(400));

            mp_SongLength->setText(Tools::msToString(m_Player.getCurrentSong().getLength()));

            mp_ProgressBar->setValue(0);
            mp_ProgressBar->setMaximum(m_Player.getCurrentSong().getLength());
            mp_NetworkLoadBar->setValue(0);
            mp_NetworkLoadBar->setStartPos(0);

            if (m_Player.getCurrentSong().isRemote())
                mp_NetworkLoadBar->setMaximum(mp_Socket->getTotalCurrentSongData());

            mp_SongList->setCurrentSong(song);

            if (m_Player.isPaused())
                setState(Constants::STOP_STATE);
        }
        catch(FmodManager::StreamError_t error)
        {
            refreshSongsList();
        }
    }
    else
    {
        if (!m_Player.isStopped())
            setState(Constants::STOP_STATE);
    }
}

// ==============================
// ==============================

void PlayerWindow::refreshSongsList()
{
    m_Player.clearSongs();
    mp_SongList->clearList();

    SongTreeRoot *songTree = m_Player.loadSongs(Constants::SONGS_SUBDIR);
    mp_SongList->addTree(Constants::LOCAL_SONGS, songTree);

    changeSong(m_Player.first());

    if (!m_Player.isStopped())
      setState(Constants::STOP_STATE);
}

// ==============================
// ==============================

void PlayerWindow::play()
{
    if (!m_Player.isPlaying())
        setState(Constants::PLAY_STATE);
}

// ==============================
// ==============================

void PlayerWindow::pause()
{
    if (!m_Player.isPaused())
        setState(Constants::PAUSE_STATE);
}

// ==============================
// ==============================

void PlayerWindow::stop()
{
    if (!m_Player.isStopped())
        setState(Constants::STOP_STATE);
}

// ==============================
// ==============================

void PlayerWindow::previousSong()
{
    changeSong(m_Player.prev());
}

// ==============================
// ==============================

void PlayerWindow::nextSong()
{
    changeSong(m_Player.next());
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

    if (previousVolume < (Constants::NB_VOLUME_STATES - 1))
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
    if (!m_Player.isStopped())
    {
        SoundPos_t pos = value * m_Player.getCurrentSong().getLength() / 100;

        m_Player.getCurrentSong().setPosition(pos);
        mp_ProgressBar->setValue(pos);

        if (m_Player.getCurrentSong().isRemote())
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
        mp_SoundVolume->setImage(Constants::MUTE_STATE);
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
        SongListItem *songItem = m_Player.addNewSong(filePath);

        if (songItem)
            mp_SongList->addSong(Constants::LOCAL_SONGS, songItem);
    }
}

// ==============================
// ==============================

void PlayerWindow::openSongsDir() const
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(Constants::SONGS_SUBDIR).absoluteFilePath()));
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
    mp_Socket = new PlayerSocket;
    connect(mp_Socket, SIGNAL(connected()), this, SLOT(startConnection()));
    connect(mp_Socket, SIGNAL(disconnected()), this, SLOT(closeConnection()));

    mp_Socket->listen(QHostAddress::Any);
}

// ==============================
// ==============================

void PlayerWindow::connectToHost(const QString& host)
{
    mp_Socket = new PlayerSocket;
    connect(mp_Socket, SIGNAL(connected()), this, SLOT(startConnection()));
    connect(mp_Socket, SIGNAL(disconnected()), this, SLOT(closeConnection()));

    mp_Socket->connectToHost(host);
}

// ==============================
// ==============================

void PlayerWindow::startConnection()
{
    SongTreeRoot *songList = mp_Socket->exchangeSongList(mp_SongList->getSongHierarchy(), m_Player.songsCount());
    m_Player.addSongs(songList);
    mp_SongList->addTree(Constants::REMOTE_SONGS, songList);

    mp_ConnectionBox->connected();

    connect(mp_Socket, SIGNAL(commandReceived(CommandRequest*)), &m_Player, SLOT(executeNetworkCommand(CommandRequest*)));
    connect(&m_Player, SIGNAL(commandExecuted(CommandReply*)), mp_Socket, SLOT(sendCommandReply(CommandReply*)));
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
            if (m_Player.getCurrentSong().isRemote())
            {
                if (!m_Player.isStopped())
                    setState(Constants::STOP_STATE);

                changeSong(m_Player.first());
            }

            m_Player.removeRemoteSongs();
            mp_SongList->clearList(Constants::REMOTE_SONGS);

            delete mp_Socket;
            mp_Socket = 0;

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
        if (m_Player.isPlaying())
        {
            FmodManager::getInstance()->update();
            mp_Spectrum->updateValues(m_Player.getCurrentSong().getSoundID());
            mp_ProgressBar->setValue(m_Player.getCurrentSong().getPosition());
            mp_SongPos->setText(Tools::msToString(m_Player.getCurrentSong().getPosition()));

            if (m_Player.getCurrentSong().isRemote())
                mp_NetworkLoadBar->setValue(mp_Socket->getSongDataReceived());

            if (m_Player.getCurrentSong().isFinished())
                changeSong(m_Player.next());
        }

        if (mp_Socket && mp_Socket->isConnected())
            mp_Socket->processCommands();
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
    m_TimerId = startTimer(Constants::REFRESH_TIME_MS);
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
