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

PlayerWindow::PlayerWindow(QWidget *parent)
    : QWidget(parent), m_TimerId(0)
{
    setWindowTitle(tr(WINDOW_TITLE));
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

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
