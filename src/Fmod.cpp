/*************************************
 * @file    Fmod.cpp
 * @date    30/07/14
 * @author  Manuel
 *
 * Définitions de la classe Fmod.
 *************************************
*/

#include "Fmod.hpp"
#include "LibException.hpp"


Fmod* Fmod::mp_Instance = 0;

// ==============================
// ==============================

Fmod::Fmod(int maxChannels) : mp_System(0), mp_Channels(maxChannels), mp_Sounds(maxChannels)
{
  FMOD_RESULT res;

  if ((res = FMOD_System_Create(&mp_System)) != FMOD_OK)
    throw LibException("Fmod::Fmod", "FMOD_System_Create", FMOD_ErrorString(res));

  if ((res = FMOD_System_Init(mp_System, maxChannels, FMOD_INIT_NORMAL, 0)) != FMOD_OK)
    throw LibException("Fmod::Fmod", "FMOD_System_Init", FMOD_ErrorString(res));
}

// ==============================
// ==============================

Fmod::~Fmod()
{
  unsigned int i;

  for (i = 0; i < mp_Sounds.size(); i++)
  {
    if (mp_Sounds.at(i))
      releaseSound(i);
  }

  FMOD_RESULT res;

  if ((res = FMOD_System_Release(mp_System)) != FMOD_OK)
    throw LibException("Fmod::~Fmod", "FMOD_System_Release", FMOD_ErrorString(res));
}

// ==============================
// ==============================

Fmod* Fmod::getInstance()
{
  if (!mp_Instance)
    mp_Instance = new Fmod;

  return mp_Instance;
}

// ==============================
// ==============================

void Fmod::deleteInstance()
{
  if (mp_Instance)
    delete mp_Instance;
}

// ==============================
// ==============================

SoundID_t Fmod::getSoundID() const
{
  return 0;
}

// ==============================
// ==============================

SoundID_t Fmod::openSound(const std::string& soundFile)
{
  SoundID_t id = getSoundID();

  if (mp_Sounds.at(id))
    releaseSound(id);

  FMOD_RESULT res;

  if ((res = FMOD_System_CreateStream(mp_System, soundFile.c_str(), FMOD_DEFAULT, 0, &mp_Sounds.at(id))) != FMOD_OK)
    throw LibException("Fmod::openSound", "FMOD_System_CreateStream", FMOD_ErrorString(res));

  return id;
}

// ==============================
// ==============================

void Fmod::releaseSound(SoundID_t id)
{
  FMOD_RESULT res;

  if ((res = FMOD_Sound_Release(mp_Sounds.at(id))) != FMOD_OK)
    throw LibException("Fmod::releaseSound", "FMOD_Sound_Release", FMOD_ErrorString(res));

  mp_Sounds.at(id) = 0;
}

// ==============================
// ==============================

void Fmod::playSound(SoundID_t id)
{
  FMOD_RESULT res;

  if ((res = FMOD_System_PlaySound(mp_System, FMOD_CHANNEL_FREE, mp_Sounds.at(id), false, &mp_Channels.at(id))) != FMOD_OK)
    throw LibException("Fmod::playSound", "FMOD_System_PlaySound", FMOD_ErrorString(res));
}

// ==============================
// ==============================

void Fmod::stopSound(SoundID_t id) const
{
  FMOD_RESULT res;

  if ((res = FMOD_Channel_Stop(mp_Channels.at(id))) != FMOD_OK)
    throw LibException("Fmod::stopSound", "FMOD_Channel_Stop", FMOD_ErrorString(res));
}

// ==============================
// ==============================

void Fmod::pauseSound(SoundID_t id, bool paused) const
{
  FMOD_RESULT res;

  if ((res = FMOD_Channel_SetPaused(mp_Channels.at(id), paused)) != FMOD_OK)
    throw LibException("Fmod::pauseSound", "FMOD_Channel_SetPaused", FMOD_ErrorString(res));
}

// ==============================
// ==============================

unsigned int Fmod::getSoundLength(SoundID_t id) const
{
  FMOD_RESULT res;
  unsigned int length;

  if ((res = FMOD_Sound_GetLength(mp_Sounds.at(id), &length, FMOD_TIMEUNIT_MS)) != FMOD_OK)
    throw LibException("Fmod::getSoundLength", "FMOD_Sound_GetLength", FMOD_ErrorString(res));

  return length;
}

// ==============================
// ==============================

unsigned int Fmod::getSoundPosition(SoundID_t id) const
{
  FMOD_RESULT res;
  unsigned int pos;

  if ((res = FMOD_Channel_GetPosition(mp_Channels.at(id), &pos, FMOD_TIMEUNIT_MS)) != FMOD_OK)
    throw LibException("Fmod::getSoundPosition", "FMOD_Channel_GetPosition", FMOD_ErrorString(res));

  return pos;
}

// ==============================
// ==============================

void Fmod::setSoundPosition(SoundID_t id, unsigned int pos)
{
  FMOD_RESULT res;

  if ((res = FMOD_Channel_SetPosition(mp_Channels.at(id), pos, FMOD_TIMEUNIT_MS)) != FMOD_OK)
    throw LibException("Fmod::setSoundPosition", "FMOD_Channel_SetPosition", FMOD_ErrorString(res));
}

// ==============================
// ==============================

bool Fmod::isPlaying(SoundID_t id) const
{
  FMOD_RESULT res;
  FMOD_BOOL playing;

  if ((res = FMOD_Channel_IsPlaying(mp_Channels.at(id), &playing)) != FMOD_OK)
    throw LibException("Fmod::isPlaying", "FMOD_Channel_IsPlaying", FMOD_ErrorString(res));

  return static_cast<bool>(playing);
}

// ==============================
// ==============================

float* Fmod::getChannelSpectrum(SoundID_t id, float *values) const
{
  FMOD_RESULT res;

  if ((res = FMOD_Channel_GetSpectrum(mp_Channels.at(id), values, SPECTRUM_WIDTH, 0, FMOD_DSP_FFT_WINDOW_RECT)) != FMOD_OK)
    throw LibException("Fmod::getChannelSpectrum", "FMOD_Channel_GetSpectrum", FMOD_ErrorString(res));

  return values;
}
