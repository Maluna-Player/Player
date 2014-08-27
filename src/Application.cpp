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
    m_Player.changeSong(song);
    m_Interface.setTitle(Path::baseName(m_Player.getCurrentSong().getFile()));
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

void Application::run()
{
  m_Interface.loadTexts();
  m_Interface.loadImages();
  m_Player.loadSongs(SONGS_SUBDIR);

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

      if (m_Interface.button(PLAY).collision(x, y))
      {
        if (!m_Player.isPlayed())
          setState(PLAY_STATE);
        else
          setState(PAUSE_STATE);
      }
      else if (m_Interface.button(STOP).collision(x, y))
      {
        if (!m_Player.isStopped())
          setState(STOP_STATE);
      }
      else if (m_Interface.button(PREV).collision(x, y))
        changeSong(m_Player.prev());
      else if (m_Interface.button(NEXT).collision(x, y))
        changeSong(m_Player.next());
      else if (m_Interface.button(PROGRESSBAR).collision(x, y))
        m_Interface.button(PROGRESSBAR).m_Press = true;
      else if (m_Interface.button(PROGRESS_BACKGROUND).collision(x, y))
        setSongPosition(x);
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
