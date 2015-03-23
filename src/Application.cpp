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
  m_Window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close);
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
      m_Interface.setSong(m_Player.getCurrentSong().getNum(),
                            m_Player.getCurrentSong().getTitle(),
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

void Application::moveInCurrentSong(int offset)
{
  int pos = m_Player.getCurrentSong().getPosition() / 1000;
  int length = m_Player.getCurrentSong().getLength() / 1000;

  int newPos = (pos + offset) * WINDOW_WIDTH / length;
  if (newPos < 0)
    newPos = 0;
  else if (newPos > WINDOW_WIDTH)
    newPos = WINDOW_WIDTH;

  setSongPosition(newPos);
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
  int delay = 0;

  m_Interface.loadTexts();
  m_Interface.loadImages();
  refreshSongsList();

  setState(PLAY_STATE);

  while (m_Window.isOpen())
  {
    m_In.update(m_Window);

    int motionX = sf::Mouse::getPosition(m_Window).x,
        motionY = sf::Mouse::getPosition(m_Window).y;

    /* Raccourcis clavier */
    if      (m_In.keyPressed(sf::Keyboard::N))  changeSong(m_Player.next());
    else if (m_In.keyPressed(sf::Keyboard::P))  changeSong(m_Player.prev());
    else if (m_In.keyPressed(sf::Keyboard::L))  m_Player.setLoop(!m_Player.isLoop());

    /* Clic souris */
    if (m_In.clic() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      int x = m_In.buttonX(), y = m_In.buttonY();

      if (m_Interface.button(PROGRESSBAR).collision(x, y))
        m_Interface.button(PROGRESSBAR).m_Press = true;
      else
      {
        Clickable_t object = m_Interface.getClickedObject(x, y);

        if (object < NB_BUTTONS)
          m_Interface.pressButton(object);
        else
        {
          switch (object)
          {
            case PROGRESS_BACKGROUND: setSongPosition(x);                             break;
            case VOLUME_VIEWER:       setMute(!m_Player.isMuted());                   break;
            case TAB:                 m_Interface.changeTabState();                   break;
            case SONG_LIST:           changeSong(m_Interface.getSongInList(x, y));    break;
            default: break;
          }
        }
      }
    }
    else if (m_In.unclic())
    {
      Clickable_t object = m_Interface.getClickedObject(m_In.buttonX(), m_In.buttonY());
      if (object < NB_BUTTONS && m_Interface.isPressed(object))
      {
        switch(object)
        {
          case PLAY_BUTTON:
            if (!m_Player.isPlayed())
              setState(PLAY_STATE);
            else
              setState(PAUSE_STATE);
            break;
          case STOP_BUTTON:
            if (!m_Player.isStopped())
              setState(STOP_STATE);
            break;
          case PREV_BUTTON:
            if (delay < BUTTON_ACTIVATION_DELAY)
              changeSong(m_Player.prev());
            break;
          case NEXT_BUTTON:
            if (delay < BUTTON_ACTIVATION_DELAY)
              changeSong(m_Player.next());
            break;
          case VOLUME_MORE_BUTTON:  setVolume(VOLUME_MORE_BUTTON);     break;
          case VOLUME_LESS_BUTTON:  setVolume(VOLUME_LESS_BUTTON);     break;
          case REFRESH_DIR_BUTTON:  refreshSongsList();                break;
          default: break;
        }
      }

      delay = 0;
      m_Interface.releaseButtons();
    }

    /* Roulette souris */
    if (m_In.wheelDelta() != 0 && m_Interface.collision(SONG_LIST, motionX, motionY))
    {
      m_Interface.getSongList().scroll(m_In.wheelDelta());
      m_Interface.getSongList().setPointedSong(m_Interface.getSongInList(motionX, motionY));
    }

    /* Mouvement souris */
    if (m_In.motion())
    {
      if (m_Interface.button(PROGRESSBAR).m_Press)
        setSongPosition(motionX);
      else
        m_Interface.getSongList().setPointedSong(m_Interface.getSongInList(motionX, motionY));
    }

    wait(REFRESH_TIME_MS);

    m_Interface.moveTab();

    if (m_Player.isPlayed())
    {
      m_Interface.updateSpectrum(m_Player.getCurrentSong().getSoundID());
      m_Interface.setSoundPosition(m_Player.getCurrentSong().getPosition(), m_Player.getCurrentSong().getLength());

      if (m_Player.getCurrentSong().isFinished())
        changeSong(m_Player.next());

      if (m_Interface.isPressed(PREV_BUTTON))
      {
        if (delay < BUTTON_ACTIVATION_DELAY)  delay++;
        else                                  moveInCurrentSong(-MOVE_INTERVAL);
      }
      else if (m_Interface.isPressed(NEXT_BUTTON))
      {
        if (delay < BUTTON_ACTIVATION_DELAY)  delay++;
        else                                  moveInCurrentSong(MOVE_INTERVAL);
      }
    }

    m_Window.clear(sf::Color::Black);

    m_Interface.drawContent(m_Window, m_Player.isStopped());

    m_Window.display();
  }
}
