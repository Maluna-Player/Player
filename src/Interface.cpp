/*************************************
 * @file    Interface.cpp
 * @date    02/08/14
 * @author  Manuel
 *
 * Définitions de la classe Interface.
 *************************************
*/

#include "Interface.hpp"
#include "constants.hpp"
#include "Fmod.hpp"
#include "Path.hpp"
#include "FileLoadingException.hpp"

/** SFML Time includes **/
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>


Interface::Interface() : m_Buttons(NB_BUTTONS, sf::CircleShape(BUTTON_SIZE / 2))
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

void Interface::loadImages()
{
  int i;

  std::string texturePath = std::string(IMAGES_SUBDIR) + "/text1.png";
  if (!m_Texture.loadFromFile(texturePath))
    throw FileLoadingException("Interface::loadImages", texturePath);

  for (i = 0; i < NB_BUTTONS; i++)
  {
    m_Buttons[i].setTextureRect(sf::IntRect(i * (BUTTON_SIZE + 1), 0, BUTTON_SIZE, BUTTON_SIZE));
    m_Buttons[i].setTexture(&m_Texture);
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
  if (!m_Player.isStopped())
    m_Window.draw(m_Spectrum);

  if (!m_Player.isPlayed())
    m_Window.draw(m_Buttons[PLAY_BUTTON]);
  else
    m_Window.draw(m_Buttons[PAUSE_BUTTON]);

  m_Window.draw(m_Buttons[STOP_BUTTON]);
  m_Window.draw(m_Buttons[PREV_BUTTON]);
  m_Window.draw(m_Buttons[NEXT_BUTTON]);

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
  sf::Clock clock;
  const sf::Time refreshTime = sf::milliseconds(REFRESH_TIME_MS);

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
            sf::Vector2f point(m_Event.mouseButton.x, m_Event.mouseButton.y);

            if (m_Buttons[PLAY_BUTTON].getGlobalBounds().contains(point))
            {
              if (!m_Player.isPlayed())
                m_Player.play();
              else
                m_Player.pause();
            }
            else if (m_Buttons[STOP_BUTTON].getGlobalBounds().contains(point))
            {
              if (!m_Player.isStopped())
                m_Player.stop();
            }
            else if (m_Buttons[PREV_BUTTON].getGlobalBounds().contains(point))
              changeSong(m_Player.prev());
            else if (m_Buttons[NEXT_BUTTON].getGlobalBounds().contains(point))
              changeSong(m_Player.next());
          }
          break;

        default:
          break;
      }
    }

    if (clock.getElapsedTime() < refreshTime)
      sf::sleep(refreshTime - clock.getElapsedTime());

    clock.restart();

    if (m_Player.isPlayed())
    {
      m_Spectrum.update(m_Player.getCurrentSong().getSoundID());

      if (m_Player.getCurrentSong().isFinished())
        changeSong(m_Player.next());
    }

    m_Window.clear(sf::Color::Black);

    drawWindowContent();

    m_Window.display();
  }
}
