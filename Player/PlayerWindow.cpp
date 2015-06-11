/*************************************
 * @file    PlayerWindow.cpp
 * @date    26/04/15
 * @author  Manuel
 *
 * Définitions de la classe PlayerWindow.
 *************************************
*/

#include "PlayerWindow.h"
#include "FmodManager.h"

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

    topLayout->addWidget(mp_SongTitle, 0, 0, 1, 2, Qt::AlignTop);
    topLayout->addWidget(mp_Spectrum, 0, 1);

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
        mp_SongPos->setTime(0);
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
            mp_SongLength->setTime(m_Player.getCurrentSong().getLength());

            mp_ProgressBar->setValue(0);
            mp_ProgressBar->setMaximum(m_Player.getCurrentSong().getLength());

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

void PlayerWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_TimerId)
    {
        if (m_Player.isPlaying())
        {
            mp_Spectrum->updateValues(m_Player.getCurrentSong().getSoundID());
            mp_ProgressBar->setValue(m_Player.getCurrentSong().getPosition());
            mp_SongPos->setTime(m_Player.getCurrentSong().getPosition());

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
