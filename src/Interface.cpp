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
#include "Path.hpp"


Interface::Interface()
  :  m_Texts(NB_TEXTS), m_Textures(NB_TEXTURES),
    m_Buttons(NB_BUTTONS, CircleButton(BUTTON_SIZE / 2)),
    mp_ProgressBackground(0), mp_ProgressBar(0), mp_VolumeViewer(0), mp_SongList(0), mp_Tab(0),
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
  delete mp_VolumeViewer;
  delete mp_SongList;
  delete mp_Tab;
}

// ==============================
// ==============================

const std::string Interface::timeToString(int seconds) const
{
  std::stringstream time;
  time << seconds / 60 << ":" << seconds % 60 / 10 << seconds % 10;

  return time.str();
}

// ==============================
// ==============================

const sf::Vector2f Interface::getTabRelativePosition(const sf::Vector2f& pos) const
{
  return sf::Vector2f(pos.x - mp_Tab->getPosition().x,
                      pos.y - mp_Tab->getPosition().y);
}

// ==============================
// ==============================

SongList& Interface::getSongList()
{
  return *mp_SongList;
}

// ==============================
// ==============================

bool Interface::collision(Clickable_t index, int x, int y) const
{
  if (index >= mp_ClickableObjects.size())
    throw ArrayAccesException("Interface::button", mp_ClickableObjects.size(), index);

  if (index != SONG_LIST)
    return mp_ClickableObjects[index]->collision(x, y);
  else
  {
    sf::Vector2f relativePos = getTabRelativePosition(sf::Vector2f(x, y));
    return mp_SongList->collision(relativePos.x, relativePos.y);
  }
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
  int i;

  if (!m_Font.loadFromFile(FONT_FILE))
    throw FileLoadingException("Interface::run", FONT_FILE);

  for (i = 0; i < NB_TEXTS; i++)
    m_Texts[i].setFont(m_Font);

  m_Texts[TITLE_TEXT].setColor(sf::Color::White);

  for (i = POS_TEXT; i <= LENGTH_TEXT; i++)
  {
    m_Texts[i].setColor(sf::Color(21, 191, 221));
    m_Texts[i].setCharacterSize(18);
  }

  m_Texts[TITLE_TEXT].setPosition(sf::Vector2f(0, 0));
  m_Texts[POS_TEXT].setPosition(sf::Vector2f(POS_TEXT_X, POS_TEXT_Y));
  m_Texts[LENGTH_TEXT].setPosition(sf::Vector2f(LENGTH_TEXT_X, LENGTH_TEXT_Y));
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
    texturePath << IMAGES_SUBDIR << Path::separator() << "text" << i << ".png";

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
  m_Buttons[VOLUME_MORE_BUTTON].setPosition(sf::Vector2f(VOLUME_M_X, VOLUME_M_Y));
  m_Buttons[VOLUME_LESS_BUTTON].setPosition(sf::Vector2f(VOLUME_L_X, VOLUME_L_Y));
  m_Buttons[REFRESH_DIR_BUTTON].setPosition(sf::Vector2f(REFRESH_DIR_X, REFRESH_DIR_Y));


  /* Création de la barre de progression */
  mp_ProgressBackground = new ProgressBackground(sf::Vector2f(WINDOW_WIDTH, PROGRESS_BACKGROUND_HEIGHT));
  mp_ClickableObjects[PROGRESS_BACKGROUND] = mp_ProgressBackground;

  mp_ProgressBar = new ProgressBar;
  mp_MovableObjects[PROGRESSBAR] = mp_ProgressBar;

  /* Création du visualisateur du volume */
  mp_VolumeViewer = new VolumeViewer;
  mp_ClickableObjects[VOLUME_VIEWER] = mp_VolumeViewer;

  /* Liste des musiques */
  mp_SongList = new SongList;
  mp_ClickableObjects[SONG_LIST] = mp_SongList;

  /* Onglet */
  mp_Tab = new PlayerTab;
  mp_ClickableObjects[TAB] = mp_Tab;
  mp_Tab->add(mp_SongList);
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
  target.draw(*mp_VolumeViewer);

  if (!stopped)
  {
    target.draw(m_Spectrum);
    target.draw(*mp_ProgressBar);
    target.draw(m_Texts[POS_TEXT]);
  }

  target.draw(m_Texts[LENGTH_TEXT]);
  target.draw(m_Texts[TITLE_TEXT]);

  if (mp_Tab->isOpened())
    mp_Tab->drawContentOnBackground();

  target.draw(*mp_Tab);
}

// ==============================
// ==============================

void Interface::setSong(int num, const std::string& title, SoundPos_t length)
{
  m_Texts[TITLE_TEXT].setString(title);
  m_Texts[LENGTH_TEXT].setString(timeToString(length / 1000));

  mp_SongList->setCurrentSong(num - 1);
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

void Interface::setVolumeTexture(int volumeState)
{
  int textX = volumeState * VOLUME_SPRITE_W;
  mp_VolumeViewer->setTextureRect(sf::IntRect(textX, 0, VOLUME_SPRITE_W, VOLUME_SPRITE_H));
}

// ==============================
// ==============================

void Interface::setSoundPosition(SoundPos_t pos, SoundPos_t length)
{
  mp_ProgressBar->resize(pos * WINDOW_WIDTH / length);
  m_Texts[POS_TEXT].setString(timeToString(pos / 1000));
}

// ==============================
// ==============================

void Interface::updateSpectrum(SoundID_t id)
{
  m_Spectrum.update(id);
}

// ==============================
// ==============================

void Interface::addToSongList(const std::vector<std::pair<std::string, int> >& songs)
{
  std::vector<std::pair<std::string, std::string> > songList;
  std::vector<std::pair<std::string, int> >::const_iterator songIt;

  for (songIt = songs.begin(); songIt != songs.end(); ++songIt)
  {
    std::string title = songIt->first;

    if (title.size() > TITLE_LIST_MAX_WIDTH)
      title.resize(TITLE_LIST_MAX_WIDTH);

    songList.push_back(std::make_pair(title, timeToString(songIt->second / 1000)));
  }

  mp_SongList->add(songList);
}

// ==============================
// ==============================

void Interface::changeTabState()
{
  mp_Tab->changeState();
}

// ==============================
// ==============================

void Interface::moveTab()
{
  if (mp_Tab->isMoving())
    mp_Tab->moveTab();
}

// ==============================
// ==============================

int Interface::getSongInList(int x, int y) const
{
  sf::Vector2f relativePos = getTabRelativePosition(sf::Vector2f(x, y));
  return mp_SongList->getSong(relativePos.x, relativePos.y);
}
