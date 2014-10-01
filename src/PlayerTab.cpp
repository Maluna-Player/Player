/*************************************
 * @file    PlayerTab.cpp
 * @date    22/09/14
 * @author  Manuel
 *
 * Définitions de la classe PlayerTab.
 *************************************
*/

#include "PlayerTab.hpp"
#include "constants.hpp"
#include "FileLoadingException.hpp"
#include "Path.hpp"


PlayerTab::PlayerTab() : m_State(CLOSED_STATE)
{
  std::string tabTexturePath = std::string(IMAGES_SUBDIR) + Path::separator() + "tabs.png";

  if (!m_TabTexture.loadFromFile(tabTexturePath))
    throw FileLoadingException("PlayerTab::PlayerTab", tabTexturePath);

  m_TabHead.setTexture(m_TabTexture);
  m_TabHead.setTextureRect(sf::IntRect(0, 0, TAB_HEAD_W, TAB_HEAD_H));
  m_TabHead.setPosition(sf::Vector2f(TAB_HEAD_X, TAB_HEAD_Y));

  if (!m_BackgroundTexture.create(TAB_CONTENT_W, TAB_CONTENT_H))
    throw BaseException("PlayerTab::PlayerTab", "Error creating tab render texture");

  m_TabBackground.setOutlineThickness(-3);
  m_TabBackground.setOutlineColor(sf::Color(25, 25, 25));
  m_TabBackground.setTexture(&m_BackgroundTexture.getTexture());
  m_TabBackground.setSize(sf::Vector2f(TAB_CONTENT_W, TAB_CONTENT_H));
  m_TabBackground.setPosition(sf::Vector2f(TAB_CONTENT_X, TAB_CONTENT_Y));
}

// ==============================
// ==============================

PlayerTab::~PlayerTab()
{

}

// ==============================
// ==============================

void PlayerTab::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  // on applique la transformation
  states.transform *= getTransform();

  // on applique la texture du tileset
  states.texture = 0;

  // on dessine l'onglet
  target.draw(m_TabHead, states);

  if (m_State != CLOSED_STATE)
    target.draw(m_TabBackground, states);
}

// ==============================
// ==============================

const sf::Vector2f& PlayerTab::getPosition() const
{
  return m_TabBackground.getPosition();
}

// ==============================
// ==============================

bool PlayerTab::isOpened() const
{
  return (m_State != CLOSED_STATE);
}

// ==============================
// ==============================

bool PlayerTab::isMoving() const
{
  return (m_State == LEFT_MOVING_STATE || m_State == RIGHT_MOVING_STATE);
}

// ==============================
// ==============================

bool PlayerTab::collision(int x, int y) const
{
  return (m_TabHead.getGlobalBounds().contains(sf::Vector2f(x, y)));
}

// ==============================
// ==============================

void PlayerTab::changeState()
{
  if (m_State == CLOSED_STATE || m_State == RIGHT_MOVING_STATE)
  {
    m_State = LEFT_MOVING_STATE;
    m_TabHead.setTextureRect(sf::IntRect(TAB_HEAD_W, 0, TAB_HEAD_W, TAB_HEAD_H));
  }
  else if (m_State == OPENED_SATE || m_State == LEFT_MOVING_STATE)
  {
    m_State = RIGHT_MOVING_STATE;
    m_TabHead.setTextureRect(sf::IntRect(0, 0, TAB_HEAD_W, TAB_HEAD_H));
  }
}

// ==============================
// ==============================

void PlayerTab::moveTab()
{
  if (m_State == LEFT_MOVING_STATE)
  {
    if (m_TabHead.getPosition().x > OPENED_TAB_HEAD_X)
    {
      m_TabHead.move(sf::Vector2f(-TAB_MOVING_INTERVAL, 0));
      m_TabBackground.move(sf::Vector2f(-TAB_MOVING_INTERVAL, 0));
    }
    else
      m_State = OPENED_SATE;
  }
  else if (m_State == RIGHT_MOVING_STATE)
  {
    if (m_TabHead.getPosition().x < CLOSED_TAB_HEAD_X)
    {
      m_TabHead.move(sf::Vector2f(TAB_MOVING_INTERVAL, 0));
      m_TabBackground.move(sf::Vector2f(TAB_MOVING_INTERVAL, 0));
    }
    else
      m_State = CLOSED_STATE;
  }
}

// ==============================
// ==============================

void PlayerTab::add(sf::Drawable *object)
{
  mp_TabElements.push_back(object);
}

// ==============================
// ==============================

void PlayerTab::drawContentOnBackground()
{
  int i;

  m_BackgroundTexture.clear(sf::Color(50, 50, 50));

  for (i = 0; i < mp_TabElements.size(); i++)
    m_BackgroundTexture.draw(*mp_TabElements.at(i));

  m_BackgroundTexture.display();
}

