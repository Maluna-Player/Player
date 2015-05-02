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
#include "Path.h"

#include <QGridLayout>
#include <QPalette>

PlayerWindow::PlayerWindow(QWidget *parent)
    : QWidget(parent), m_TimerId(0), m_Spectrum(SPECTRUM_WIDTH)
{
    setWindowTitle(tr(WINDOW_TITLE));
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);


    QWidget *topPart = new QWidget;
    QWidget *bottomPart = new QWidget;

    /** Création des backgrounds **/

    QPalette pal(palette());

    pal.setColor(QPalette::Background, Qt::black);
    topPart->setAutoFillBackground(true);
    topPart->setPalette(pal);

    QLinearGradient gradient(0, 0, 0, BOTTOM_BACKGROUND_H);
    gradient.setColorAt(0.0, QColor(66, 66, 66));
    gradient.setColorAt(0.5, QColor(69, 69, 69));
    gradient.setColorAt(0.6, QColor(60, 60, 60));
    gradient.setColorAt(1.0, QColor(24, 24, 24));

    pal.setBrush(QPalette::Window, gradient);
    bottomPart->setAutoFillBackground(true);
    bottomPart->setPalette(pal);


    /** Ajout du contenu **/

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addWidget(&m_Spectrum, 0, 0);
    topPart->setLayout(topLayout);

    layout->addWidget(topPart);
    layout->addWidget(bottomPart);
    setLayout(layout);

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
        m_Player.stop();
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

            if (m_Player.isPaused())
                setState(STOP_STATE);
        }
        catch(StreamError_t error)
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
    m_Player.loadSongs(SONGS_SUBDIR);

    changeSong(m_Player.first());

    if (!m_Player.isStopped())
      setState(STOP_STATE);
}

// ==============================
// ==============================

void PlayerWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_TimerId)
    {
        if (m_Player.isPlayed())
        {
            m_Spectrum.updateValues(m_Player.getCurrentSong().getSoundID());

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
