/*************************************
 * @file    Interface.cpp
 * @date    02/08/14
 * @author  Manuel
 *
 * Définitions de la classe Interface.
 *************************************
*/

#include <sstream>
#include "Interface.hpp"
#include "constants.hpp"
#include "Fmod.hpp"
#include "Path.hpp"
#include "FileLoadingException.hpp"

/** SFML Time includes **/
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>


Interface::Interface()
  : m_Textures(NB_TEXTURES),
    m_Buttons(NB_BUTTONS, CircleButton(BUTTON_SIZE / 2)),
    m_ProgressBackground(sf::Vector2f(WINDOW_WIDTH, PROGRESS_BACKGROUND_HEIGHT))
{
  m_Window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
  m_Window.setVerticalSyncEnabled(true);
}

// ==============================
// ==============================

Interface::~Interface()
{
  Fmod::deleteInstance();
}

// ==============================
// ==============================

void Interface::wait(int ms)
{
  const sf::Time timeInterval = sf::milliseconds(ms);

  if (m_Clock.getElapsedTime() < timeInterval)
    sf::sleep(timeInterval - m_Clock.getElapsedTime());

  m_Clock.restart();
}

// ==============================
// ==============================

void Interface::loadImages()
{
  int i;

  for (i = 0; i < NB_TEXTURES; i++)
  {
    std::stringstream texturePath;
    texturePath << IMAGES_SUBDIR << "/text" << i << ".png";

    if (!m_Textures[i].loadFromFile(texturePath.str()))
      throw FileLoadingException("Interface::loadImages", texturePath.str());
  }

  m_Textures[PROGRESS_BACKGROUND_TEXTURE].setRepeated(true);
  m_ProgressBackground.setTexture(&m_Textures[PROGRESS_BACKGROUND_TEXTURE]);
  m_ProgressBackground.setPosition(sf::Vector2f(PROGRESS_BACKGROUND_X, PROGRESS_BACKGROUND_Y));

  m_Textures[PROGRESSBAR_TEXTURE].setRepeated(true);
  m_ProgressBar.setTexture(&m_Textures[PROGRESSBAR_TEXTURE]);
  m_ProgressBar.setPosition(sf::Vector2f(PROGRESS_BACKGROUND_X, PROGRESSBAR_Y));

  for (i = 0; i < NB_BUTTONS; i++)
  {
    m_Buttons[i].setTextureRect(sf::IntRect(i * (BUTTON_SIZE + 1), 0, BUTTON_SIZE, BUTTON_SIZE));
    m_Buttons[i].setTexture(&m_Textures[BUTTONS_TEXTURE]);
  }

  m_Buttons[PLAY_BUTTON].setPosition(sf::Vector2f(PLAY_X, PLAY_Y));
  m_Buttons[PAUSE_BUTTON].setPosition(sf::Vector2f(PLAY_X, PLAY_Y));
  m_Buttons[STOP_BUTTON].setPosition(sf::Vector2f(STOP_X, STOP_Y));
  m_Buttons[PREV_BUTTON].setPosition(sf::Vector2f(PREV_X, PREV_Y));
  m_Buttons[NEXT_BUTTON].setPosition(sf::Vector2f(NEXT_X, NEXT_Y));
}

// ==============================
// ==============================

void Interface::drawWindowContent()
{
  if (!m_Player.isPlayed())
    m_Window.draw(m_Buttons[PLAY_BUTTON]);
  else
    m_Window.draw(m_Buttons[PAUSE_BUTTON]);

  m_Window.draw(m_Buttons[STOP_BUTTON]);
  m_Window.draw(m_Buttons[PREV_BUTTON]);
  m_Window.draw(m_Buttons[NEXT_BUTTON]);

  m_Window.draw(m_ProgressBackground);

  if (!m_Player.isStopped())
  {
    m_Window.draw(m_Spectrum);
    m_Window.draw(m_ProgressBar);
  }

  m_Window.draw(m_SongTitle);
}

// ==============================
// ==============================

void Interface::changeSong(int song)
{
  if (song != UNDEFINED_SONG)
  {
    m_Player.changeSong(song);
    m_SongTitle.setString(Path::baseName(m_Player.getCurrentSong().getFile()));
  }
  else
  {
    if (!m_Player.isStopped())
      m_Player.stop();
  }
}

// ==============================
// ==============================

void Interface::run()
{
  if (!m_Font.loadFromFile(FONT_FILE))
    throw FileLoadingException("Interface::run", FONT_FILE);

  m_SongTitle.setFont(m_Font);
  m_SongTitle.setColor(sf::Color::White);

  loadImages();
  m_Player.loadSongs(SONGS_SUBDIR);

  changeSong(m_Player.first());

  while (m_Window.isOpen())
  {
    while (m_Window.pollEvent(m_Event))
    {
      switch (m_Event.type)
      {
        case sf::Event::Closed:
          m_Window.close();
          break;

        case sf::Event::KeyPressed:
          if (m_Event.key.code == sf::Keyboard::N)
            changeSong(m_Player.next());
          else if (m_Event.key.code == sf::Keyboard::P)
            changeSong(m_Player.prev());
          else if (m_Event.key.code == sf::Keyboard::L)
            m_Player.setLoop(!m_Player.isLoop());
          break;

        case sf::Event::MouseButtonPressed:
          if (m_Event.mouseButton.button == sf::Mouse::Left)
          {
            if (m_Buttons[PLAY_BUTTON].collision(m_Event.mouseButton.x, m_Event.mouseButton.y))
            {
              if (!m_Player.isPlayed())
                m_Player.play();
              else
                m_Player.pause();
            }
            else if (m_Buttons[STOP_BUTTON].collision(m_Event.mouseButton.x, m_Event.mouseButton.y))
            {
              if (!m_Player.isStopped())
                m_Player.stop();
            }
            else if (m_Buttons[PREV_BUTTON].collision(m_Event.mouseButton.x, m_Event.mouseButton.y))
              changeSong(m_Player.prev());
            else if (m_Buttons[NEXT_BUTTON].collision(m_Event.mouseButton.x, m_Event.mouseButton.y))
              changeSong(m_Player.next());
          }
          break;

        default:
          break;
      }
    }

    wait(REFRESH_TIME_MS);

    if (m_Player.isPlayed())
    {
      m_Spectrum.update(m_Player.getCurrentSong().getSoundID());
      m_ProgressBar.setSize(sf::Vector2f(m_Player.getCurrentSong().getPosition() * WINDOW_WIDTH
                                          / m_Player.getCurrentSong().getLength(), PROGRESSBAR_HEIGHT));

      if (m_Player.getCurrentSong().isFinished())
        changeSong(m_Player.next());
    }

    m_Window.clear(sf::Color::Black);

    drawWindowContent();

    m_Window.display();
  }
}
