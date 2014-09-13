/*************************************
 * @file    Application.cpp
 * @date    20/08/14
 * @author  Manuel
 *
 * Définitions de la classe Application.
 *************************************
*/

#include "Application.hpp"

/** SFML Time includes **/
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

#include "Fmod.hpp"
#include "Path.hpp"
#include "constants.hpp"


Application::Application()
{
  m_Window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
  m_Window.setVerticalSyncEnabled(true);
  m_Window.setKeyRepeatEnabled(false);
}

// ==============================
// ==============================

Application::~Application()
{
  Fmod::deleteInstance();
}

// ==============================
// ==============================

void Application::wait(int ms)
{
  const sf::Time timeInterval = sf::milliseconds(ms);

  if (m_Clock.getElapsedTime() < timeInterval)
    sf::sleep(timeInterval - m_Clock.getElapsedTime());

  m_Clock.restart();
}

// ==============================
// ==============================

void Application::setState(State_t state)
{
  if (state == PLAY_STATE)
    m_Player.play();
  else if (state == PAUSE_STATE)
    m_Player.pause();
  else if (state == STOP_STATE)
    m_Player.stop();

  m_Interface.setPlayButtonTexture(m_Player.isPlayed());
}

// ==============================
// ==============================

void Application::changeSong(int song)
{
  if (song != UNDEFINED_SONG)
  {
    try
    {
      m_Player.changeSong(song);
      m_Interface.setSong(Path::baseName(m_Player.getCurrentSong().getFile()),
                            m_Player.getCurrentSong().getLength());

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

void Application::setSongPosition(int x)
{
  if (!m_Player.isStopped())
  {
    SoundPos_t pos = x * m_Player.getCurrentSong().getLength() / WINDOW_WIDTH;

    m_Player.getCurrentSong().setPosition(pos);
    m_Interface.setSoundPosition(pos, m_Player.getCurrentSong().getLength());
  }
}

// ==============================
// ==============================

void Application::setVolume(int volumeType)
{
  int previousVolume = m_Player.getVolumeState();

  if (volumeType == VOLUME_MORE_BUTTON)
  {
    if (previousVolume < (NB_VOLUME_STATES - 1))
    {
      m_Player.setVolume(previousVolume + 1);

      if (!m_Player.isMuted())
        m_Interface.setVolumeTexture(previousVolume + 1);
    }
  }
  else if (volumeType == VOLUME_LESS_BUTTON)
  {
    if (previousVolume > 0)
    {
      m_Player.setVolume(previousVolume - 1);

      if (!m_Player.isMuted())
        m_Interface.setVolumeTexture(previousVolume - 1);
    }
  }
}

// ==============================
// ==============================

void Application::setMute(bool mute)
{
  m_Player.mute(mute);

  if (m_Player.isMuted())
    m_Interface.setVolumeTexture(MUTE_STATE);
  else
    m_Interface.setVolumeTexture(m_Player.getVolumeState());
}

// ==============================
// ==============================

void Application::refreshSongsList()
{
  m_Player.loadSongs(SONGS_SUBDIR);
  m_Interface.getSongList().clear();
  m_Interface.addToSongList(m_Player.getSongDetails());

  changeSong(m_Player.first());

  if (!m_Player.isStopped())
    setState(STOP_STATE);
}

// ==============================
// ==============================

void Application::run()
{
  m_Interface.loadTexts();
  m_Interface.loadImages();
  m_Player.loadSongs(SONGS_SUBDIR);
  m_Interface.addToSongList(m_Player.getSongDetails());

  changeSong(m_Player.first());

  while (m_Window.isOpen())
  {
    m_In.update(m_Window);

    /* Raccourcis clavier */
    if      (m_In.keyPressed(sf::Keyboard::N))  changeSong(m_Player.next());
    else if (m_In.keyPressed(sf::Keyboard::P))  changeSong(m_Player.prev());
    else if (m_In.keyPressed(sf::Keyboard::L))  m_Player.setLoop(!m_Player.isLoop());

    /* Evénements souris */
    if (m_In.clic() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      int x = m_In.buttonX(), y = m_In.buttonY();

      if (m_Interface.button(PLAY_BUTTON).collision(x, y))
      {
        if (!m_Player.isPlayed())
          setState(PLAY_STATE);
        else
          setState(PAUSE_STATE);
      }
      else if (m_Interface.button(STOP_BUTTON).collision(x, y))
      {
        if (!m_Player.isStopped())
          setState(STOP_STATE);
      }
      else if (m_Interface.button(PREV_BUTTON).collision(x, y))
        changeSong(m_Player.prev());
      else if (m_Interface.button(NEXT_BUTTON).collision(x, y))
        changeSong(m_Player.next());
      else if (m_Interface.button(VOLUME_MORE_BUTTON).collision(x, y))
        setVolume(VOLUME_MORE_BUTTON);
      else if (m_Interface.button(VOLUME_LESS_BUTTON).collision(x, y))
        setVolume(VOLUME_LESS_BUTTON);
      else if (m_Interface.button(REFRESH_DIR_BUTTON).collision(x, y))
        refreshSongsList();
      else if (m_Interface.button(PROGRESSBAR).collision(x, y))
        m_Interface.button(PROGRESSBAR).m_Press = true;
      else if (m_Interface.button(PROGRESS_BACKGROUND).collision(x, y))
        setSongPosition(x);
      else if (m_Interface.button(VOLUME_VIEWER).collision(x, y))
        setMute(!m_Player.isMuted());
      else if (m_Interface.button(SONG_LIST).collision(x, y))
        changeSong(m_Interface.getSongList().getClickedSong(x, y));
    }
    else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
      m_Interface.button(PROGRESSBAR).m_Press = false;

    if (m_In.motion())
    {
      if (m_Interface.button(PROGRESSBAR).m_Press)
        setSongPosition(sf::Mouse::getPosition(m_Window).x);
    }

    wait(REFRESH_TIME_MS);

    if (m_Player.isPlayed())
    {
      m_Interface.updateSpectrum(m_Player.getCurrentSong().getSoundID());
      m_Interface.setSoundPosition(m_Player.getCurrentSong().getPosition(), m_Player.getCurrentSong().getLength());

      if (m_Player.getCurrentSong().isFinished())
        changeSong(m_Player.next());
    }

    m_Window.clear(sf::Color::Black);

    m_Interface.drawContent(m_Window, m_Player.isStopped());

    m_Window.display();
  }
}
