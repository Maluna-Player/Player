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


PlayerWindow::PlayerWindow(QWidget *parent)
    : QWidget(parent), m_TimerId(0)
{
    setWindowTitle(tr(WINDOW_TITLE));
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);


    /** Partie du haut **/

    mp_TopPart = new QWidget;
    QGridLayout *topLayout = new QGridLayout;

    mp_SongTitle = new PlayerLabel(Qt::white, 20);
    mp_Spectrum = new Spectrum(SPECTRUM_WIDTH);
    mp_SongList = new SongList;

    connect(mp_SongList, SIGNAL(songPressed(int)), this, SLOT(changeSong(int)));

    topLayout->setColumnStretch(0, 1);
    topLayout->addWidget(mp_SongTitle, 0, 0, 1, 2, Qt::AlignTop);
    topLayout->addWidget(mp_Spectrum, 0, 1);
    topLayout->addWidget(mp_SongList, 0, 2);

    mp_TopPart->setLayout(topLayout);

    mp_SongTitle->raise();


    mp_ProgressBackground = new ProgressBackground;
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

    ClickableLabel *informationButton = new ClickableLabel;
    informationButton->setPixmap(Tools::loadImage(IMAGES_SUBDIR + "info.png"));
    connect(informationButton, SIGNAL(clicked()), this, SLOT(openInformation()));

    mp_HostLine = new QLineEdit;
    mp_ListenButton = new QPushButton("Listen");
    mp_ConnectButton = new QPushButton("Connect");
    connect(mp_ListenButton, SIGNAL(clicked()), this, SLOT(listen()));
    connect(mp_ConnectButton, SIGNAL(clicked()), this, SLOT(connectToHost()));


    bottomLayout->setColumnStretch(3, 1);
    bottomLayout->addWidget(mp_SongPos, 0, 0);
    bottomLayout->addWidget(mp_SongLength, 0, 10);

    bottomLayout->addWidget(mp_Buttons.at(PLAY_BUTTON), 0, 5, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(PAUSE_BUTTON), 0, 5, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(STOP_BUTTON), 2, 5, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(PREV_BUTTON), 1, 4, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(NEXT_BUTTON), 1, 6, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(VOLUME_MORE_BUTTON), 0, 3, 2, 1);
    bottomLayout->addWidget(mp_Buttons.at(VOLUME_LESS_BUTTON), 2, 3, 2, 1);

    bottomLayout->addWidget(mp_SoundVolume, 1, 1, 2, 2);

    bottomLayout->addWidget(informationButton, 2, 10, 2, 1);

    bottomLayout->addWidget(mp_HostLine, 3, 7);
    bottomLayout->addWidget(mp_ListenButton, 3, 8);
    bottomLayout->addWidget(mp_ConnectButton, 3, 9);

    bottomLayout->setColumnStretch(7, 1);
    mp_BottomPart->setLayout(bottomLayout);


    layout->addWidget(mp_TopPart);
    layout->addWidget(mp_ProgressBackground);
    layout->addWidget(mp_BottomPart);
    setLayout(layout);


    /** Démarrage du player **/

    refreshSongsList();
    setState(PLAY_STATE);
}

// ==============================
// ==============================

PlayerWindow::~PlayerWindow()
{
    FmodManager::deleteInstance();
}

// ==============================
// ==============================

void PlayerWindow::setState(State_t state)
{
    if (state == PLAY_STATE)
        m_Player.play();
    else if (state == PAUSE_STATE)
        m_Player.pause();
    else if (state == STOP_STATE)
    {
        m_Player.stop();
        mp_Spectrum->updateValues(m_Player.getCurrentSong().getSoundID());
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
            mp_SongLength->setText(Tools::msToString(m_Player.getCurrentSong().getLength()));

            mp_ProgressBar->setValue(0);
            mp_ProgressBar->setMaximum(m_Player.getCurrentSong().getLength());

            mp_SongList->setCurrentSong(song);

            if (m_Player.isPaused())
                setState(STOP_STATE);
        }
        catch(FmodManager::StreamError_t error)
        {
            refreshSongsList();
        }
    }
    else
    {
        if (!m_Player.isStopped())
            setState(STOP_STATE);
    }
}

// ==============================
// ==============================

void PlayerWindow::refreshSongsList()
{
    m_Player.clearSongs();
    m_Player.loadSongs(SONGS_SUBDIR);

    mp_SongList->clearList();
    mp_SongList->add(SongList::LOCAL_SONGS, m_Player.getSongDetails());

    changeSong(m_Player.first());

    if (!m_Player.isStopped())
      setState(STOP_STATE);
}

// ==============================
// ==============================

void PlayerWindow::play()
{
    if (!m_Player.isPlaying())
        setState(PLAY_STATE);
}

// ==============================
// ==============================

void PlayerWindow::pause()
{
    if (!m_Player.isPaused())
        setState(PAUSE_STATE);
}

// ==============================
// ==============================

void PlayerWindow::stop()
{
    if (!m_Player.isStopped())
        setState(STOP_STATE);
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

void PlayerWindow::increaseVolume()
{
    int previousVolume = m_Player.getVolumeState();

    if (previousVolume < (NB_VOLUME_STATES - 1))
    {
        m_Player.setVolume(previousVolume + 1);

        if (!m_Player.isMuted())
           mp_SoundVolume->setImage(previousVolume + 1);
    }
}

// ==============================
// ==============================

void PlayerWindow::decreaseVolume()
{
    int previousVolume = m_Player.getVolumeState();

    if (previousVolume > 0)
    {
        m_Player.setVolume(previousVolume - 1);

        if (!m_Player.isMuted())
            mp_SoundVolume->setImage(previousVolume - 1);
    }
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
    mp_ListenButton->setEnabled(false);
    mp_ListenButton->setText("Listening..");
    mp_ConnectButton->hide();

    mp_Socket = new PlayerSocket;
    connect(mp_Socket, SIGNAL(connected()), this, SLOT(startConnection()));
    connect(mp_Socket, SIGNAL(endedConnection()), this, SLOT(closeConnection()));

    mp_Socket->listen(QHostAddress::Any);
}

// ==============================
// ==============================

void PlayerWindow::connectToHost()
{
    mp_ListenButton->hide();
    mp_ConnectButton->setText("Connexion...");
    mp_ConnectButton->setEnabled(false);

    mp_Socket = new PlayerSocket;
    connect(mp_Socket, SIGNAL(connected()), this, SLOT(startConnection()));
    connect(mp_Socket, SIGNAL(endedConnection()), this, SLOT(closeConnection()));

    mp_Socket->connectToHost(mp_HostLine->text());
}

// ==============================
// ==============================

void PlayerWindow::startConnection()
{
    mp_HostLine->hide();
    mp_ListenButton->hide();
    mp_ConnectButton->show();
    mp_ConnectButton->setEnabled(false);
    mp_ConnectButton->setText("Connecté");

    connect(mp_Socket, SIGNAL(songsRecieved(QList<QTreeWidgetItem*>)), this, SLOT(addRemoteSongList(QList<QTreeWidgetItem*>)));

    mp_Socket->send(mp_SongList->getSongHierarchy(), m_Player.songCount());
}

// ==============================
// ==============================

void PlayerWindow::closeConnection()
{
    delete mp_Socket;

    mp_HostLine->show();
    mp_ListenButton->show();
    mp_ConnectButton->show();

    mp_ListenButton->setEnabled(true);
    mp_ConnectButton->setEnabled(true);

    mp_ListenButton->setText("Listen");
    mp_ConnectButton->setText("Connect");

    mp_SongList->clearList(SongList::REMOTE_SONGS);
}

// ==============================
// ==============================

void PlayerWindow::addRemoteSongList(const QList<QTreeWidgetItem*>& songs)
{
    mp_SongList->add(SongList::REMOTE_SONGS, songs);
}

// ==============================
// ==============================

void PlayerWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_TimerId)
    {
        if (m_Player.isPlaying())
        {
            mp_Spectrum->updateValues(m_Player.getCurrentSong().getSoundID());
            mp_ProgressBar->setValue(m_Player.getCurrentSong().getPosition());
            mp_SongPos->setText(Tools::msToString(m_Player.getCurrentSong().getPosition()));

            if (m_Player.getCurrentSong().isFinished())
                changeSong(m_Player.next());
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