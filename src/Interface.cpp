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
#include "FileLoadingException.hpp"
#include "ArrayAccessException.hpp"


Interface::Interface()
  : m_Textures(NB_TEXTURES),
    m_Buttons(NB_BUTTONS, CircleButton(BUTTON_SIZE / 2)),
    mp_ProgressBackground(0), mp_ProgressBar(0),
    mp_ClickableObjects(NB_CLICKABLES), mp_MovableObjects(NB_MOVABLES)
{
  int i;

  for (i = 0; i < NB_BUTTONS; i++)
    mp_ClickableObjects[i] = &m_Buttons[i];
}

// ==============================
// ==============================

Interface::~Interface()
{
  delete mp_ProgressBar;
  delete mp_ProgressBackground;
}

// ==============================
// ==============================

Clickable& Interface::button(Clickable_t index) const
{
  if (index >= mp_ClickableObjects.size())
    throw ArrayAccesException("Interface::button", mp_ClickableObjects.size(), index);

  return *mp_ClickableObjects.at(index);
}

// ==============================
// ==============================

Movable& Interface::button(Movable_t index) const
{
  if (index >= mp_MovableObjects.size())
    throw ArrayAccesException("Interfacce::button", mp_MovableObjects.size(), index);

  return *mp_MovableObjects.at(index);
}

// ==============================
// ==============================

void Interface::loadTexts()
{
  if (!m_Font.loadFromFile(FONT_FILE))
    throw FileLoadingException("Interface::run", FONT_FILE);

  m_SongTitle.setFont(m_Font);
  m_SongTitle.setColor(sf::Color::White);
}

// ==============================
// ==============================

void Interface::loadImages()
{
  int i;

  /* Chargement des textures */
  for (i = 0; i < NB_TEXTURES; i++)
  {
    std::stringstream texturePath;
    texturePath << IMAGES_SUBDIR << "/text" << i << ".png";

    if (!m_Textures[i].loadFromFile(texturePath.str()))
      throw FileLoadingException("Interface::loadImages", texturePath.str());
  }

  /* Création des boutons */
  for (i = 0; i < NB_BUTTONS; i++)
  {
    m_Buttons[i].setTextureRect(sf::IntRect((i+1) * (BUTTON_SIZE + 1), 0, BUTTON_SIZE, BUTTON_SIZE));
    m_Buttons[i].setTexture(&m_Textures[BUTTONS_TEXTURE]);
  }

  m_Buttons[PLAY_BUTTON].setPosition(sf::Vector2f(PLAY_X, PLAY_Y));
  m_Buttons[STOP_BUTTON].setPosition(sf::Vector2f(STOP_X, STOP_Y));
  m_Buttons[PREV_BUTTON].setPosition(sf::Vector2f(PREV_X, PREV_Y));
  m_Buttons[NEXT_BUTTON].setPosition(sf::Vector2f(NEXT_X, NEXT_Y));


  /* Création de la barre de progression */
  mp_ProgressBackground = new ProgressBackground(sf::Vector2f(WINDOW_WIDTH, PROGRESS_BACKGROUND_HEIGHT));
  mp_ClickableObjects[PROGRESS_BACKGROUND] = mp_ProgressBackground;

  mp_ProgressBar = new ProgressBar;
  mp_MovableObjects[PROGRESSBAR] = mp_ProgressBar;
}

// ==============================
// ==============================

void Interface::drawContent(sf::RenderTarget& target, bool stopped)
{
  int i;

  /* Affichage des boutons */
  for (i = 0; i < NB_BUTTONS; i++)
    target.draw(m_Buttons[i]);

  target.draw(*mp_ProgressBackground);

  if (!stopped)
  {
    target.draw(m_Spectrum);
    target.draw(*mp_ProgressBar);
  }

  target.draw(m_SongTitle);
}

// ==============================
// ==============================

void Interface::setTitle(const std::string& title)
{
  m_SongTitle.setString(title);
}

// ==============================
// ==============================

void Interface::setPlayButtonTexture(bool play)
{
  int textX = (play ? (BUTTON_SIZE + 1) : 0);
  m_Buttons[PLAY_BUTTON].setTextureRect(sf::IntRect(textX, 0, BUTTON_SIZE, BUTTON_SIZE));
}

// ==============================
// ==============================

void Interface::setProgressBar(int x)
{
  mp_ProgressBar->resize(x);
}

// ==============================
// ==============================

void Interface::updateSpectrum(SoundID_t id)
{
  m_Spectrum.update(id);
}
