/*************************************
 * @file    Fmod.cpp
 * @date    30/07/14
 * @author  Manuel
 *
 * Définitions de la classe Fmod.
 *************************************
*/

#include "FmodManager.h"
#include "../Exceptions/LibException.h"


FmodManager* FmodManager::mp_Instance = 0;

// ==============================
// ==============================

FmodManager::FmodManager(int maxChannels)
    : mp_System(0), mp_Channels(maxChannels), mp_Sounds(maxChannels), mp_ChannelGroup(0), mp_Dsp(0)
{
    FMOD_RESULT res;

    /* Création du système */
    if ((res = FMOD_System_Create(&mp_System)) != FMOD_OK)
        throw LibException("FmodManager::FmodManager", "FMOD_System_Create", FMOD_ErrorString(res));

    /* Initialisation */
    if ((res = FMOD_System_Init(mp_System, maxChannels, FMOD_INIT_NORMAL, 0)) != FMOD_OK)
        throw LibException("FmodManager::FmodManager", "FMOD_System_Init", FMOD_ErrorString(res));

    /* Récupération du groupe de canaux */
    if ((res = FMOD_System_GetMasterChannelGroup(mp_System, &mp_ChannelGroup)) != FMOD_OK)
        throw LibException("FmodManager::FmodManager", "FMOD_System_GetMasterChannelGroup", FMOD_ErrorString(res));

    /* Création du DSP */
    if ((res = FMOD_System_CreateDSPByType(mp_System, FMOD_DSP_TYPE_FFT, &mp_Dsp)) != FMOD_OK)
        throw LibException("FmodManager::FmodManager", "FMOD_System_CreateDSPByType", FMOD_ErrorString(res));

    if ((res = FMOD_DSP_SetParameterInt(mp_Dsp, FMOD_DSP_FFT_WINDOWSIZE, Constants::SPECTRUM_WIDTH)) != FMOD_OK)
        throw LibException("FmodManager::FmodManager", "FMOD_DSP_SetParameterInt", FMOD_ErrorString(res));

    if ((res = FMOD_ChannelGroup_AddDSP(mp_ChannelGroup, 0, mp_Dsp)) != FMOD_OK)
        throw LibException("FmodManager::FmodManager", "FMOD_ChannelGroup_AddDSP", FMOD_ErrorString(res));
}

// ==============================
// ==============================

FmodManager::~FmodManager()
{
    unsigned int i;

    for (i = 0; i < mp_Sounds.size(); i++)
        releaseSound(i);

    FMOD_RESULT res;

    if ((res = FMOD_System_Release(mp_System)) != FMOD_OK)
        throw LibException("FmodManager::~FmodManager", "FMOD_System_Release", FMOD_ErrorString(res));
}

// ==============================
// ==============================

FmodManager* FmodManager::getInstance()
{
    if (!mp_Instance)
        mp_Instance = new FmodManager;

    return mp_Instance;
}

// ==============================
// ==============================

void FmodManager::deleteInstance()
{
    if (mp_Instance)
    {
        delete mp_Instance;
        mp_Instance = 0;
    }
}

// ==============================
// ==============================

SoundID_t FmodManager::getSoundID(bool mainCanal) const
{
    SoundID_t id = 0;

    if (mainCanal)
        return id;

    while (id < mp_Sounds.size() && isPlaying(id))
        id++;

    return (id < mp_Sounds.size()) ? id : 0;
}

// ==============================
// ==============================

void FmodManager::update() const
{
    FMOD_RESULT res;

    if ((res = FMOD_System_Update(mp_System)) != FMOD_OK)
        throw LibException("FmodManager::update", "FMOD_System_Update", FMOD_ErrorString(res));
}

// ==============================
// ==============================

SoundID_t FmodManager::openFromFile(const std::string& soundFile, bool mainCanal, SoundSettings *settings) throw (StreamError_t)
{
    SoundID_t id = getSoundID(mainCanal);

    releaseSound(id);

    FMOD_RESULT res;

    if (!settings)
        res = FMOD_System_CreateStream(mp_System, soundFile.c_str(), FMOD_DEFAULT, 0, &mp_Sounds.at(id));
    else
    {
        FMOD_CREATESOUNDEXINFO *soundSettings = new FMOD_CREATESOUNDEXINFO();

        soundSettings->cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        soundSettings->fileuseropen = settings->openCallback;
        soundSettings->fileuserclose = settings->closeCallback;
        soundSettings->fileuserread = settings->readCallback;
        soundSettings->fileuserseek = settings->seekCallback;
        soundSettings->fileuserdata = settings->userdata;

        res = FMOD_System_CreateStream(mp_System, soundFile.c_str(), FMOD_DEFAULT, soundSettings, &mp_Sounds.at(id));

        delete soundSettings;
    }

    if (res == FMOD_ERR_FORMAT)
        throw FORMAT_ERROR;
    else if (res != FMOD_OK)
        throw FILE_ERROR;

    return id;
}

// ==============================
// ==============================

void FmodManager::releaseSound(SoundID_t id)
{
    if (mp_Sounds.at(id))
    {
        FMOD_RESULT res;

        if (mp_Channels.at(id))
            stopSound(id);

        if ((res = FMOD_Sound_Release(mp_Sounds.at(id))) != FMOD_OK)
            throw LibException("FmodManager::releaseSound", "FMOD_Sound_Release", FMOD_ErrorString(res));

        mp_Sounds.at(id) = 0;
    }
}

// ==============================
// ==============================

void FmodManager::playSound(SoundID_t id)
{
    FMOD_RESULT res;

    if ((res = FMOD_System_PlaySound(mp_System, mp_Sounds.at(id), 0, false, &mp_Channels.at(id))) != FMOD_OK)
        throw LibException("FmodManager::playSound", "FMOD_System_PlaySound", FMOD_ErrorString(res));
}

// ==============================
// ==============================

bool FmodManager::isChannelUsed(SoundID_t id) const
{
    return (mp_Channels.at(id) != 0);
}

// ==============================
// ==============================

void FmodManager::stopSound(SoundID_t id)
{
    FMOD_RESULT res;

    if ((res = FMOD_Channel_Stop(mp_Channels.at(id))) != FMOD_OK)
        throw LibException("FmodManager::stopSound", "FMOD_Channel_Stop", FMOD_ErrorString(res));

    mp_Channels.at(id) = 0;
}

// ==============================
// ==============================

void FmodManager::pauseSound(SoundID_t id, bool paused) const
{
    if (isChannelUsed(id))
    {
        FMOD_RESULT res;

        if ((res = FMOD_Channel_SetPaused(mp_Channels.at(id), paused)) != FMOD_OK)
            throw LibException("FmodManager::pauseSound", "FMOD_Channel_SetPaused", FMOD_ErrorString(res));
    }
}

// ==============================
// ==============================

SoundPos_t FmodManager::getSoundLength(SoundID_t id) const
{
    FMOD_RESULT res;
    SoundPos_t length = 0;

    if ((res = FMOD_Sound_GetLength(mp_Sounds.at(id), &length, FMOD_TIMEUNIT_MS)) != FMOD_OK)
        throw LibException("FmodManager::getSoundLength", "FMOD_Sound_GetLength", FMOD_ErrorString(res));

    return length;
}

// ==============================
// ==============================

SoundPos_t FmodManager::getSoundPosition(SoundID_t id) const
{
    SoundPos_t pos = 0;

    if (isChannelUsed(id))
    {
        FMOD_RESULT res;

        if ((res = FMOD_Channel_GetPosition(mp_Channels.at(id), &pos, FMOD_TIMEUNIT_MS)) != FMOD_OK)
            throw LibException("FmodManager::getSoundPosition", "FMOD_Channel_GetPosition", FMOD_ErrorString(res));
    }

    return pos;
}

// ==============================
// ==============================

void FmodManager::setSoundPosition(SoundID_t id, SoundPos_t pos)
{
    if (isChannelUsed(id))
    {
        FMOD_RESULT res;

        if ((res = FMOD_Channel_SetPosition(mp_Channels.at(id), pos, FMOD_TIMEUNIT_MS)) != FMOD_OK)
            throw LibException("FmodManager::setSoundPosition", "FMOD_Channel_SetPosition", FMOD_ErrorString(res));
    }
}

// ==============================
// ==============================

bool FmodManager::isPlaying(SoundID_t id) const
{
    FMOD_BOOL playing = false;

    if (isChannelUsed(id))
    {
        FMOD_RESULT res;

        if ((res = FMOD_Channel_IsPlaying(mp_Channels.at(id), &playing)) != FMOD_OK)
            throw LibException("FmodManager::isPlaying", "FMOD_Channel_IsPlaying", FMOD_ErrorString(res));
    }

    return static_cast<bool>(playing);
}

// ==============================
// ==============================

float* FmodManager::getChannelSpectrum(SoundID_t id, float *values) const
{
    if (isChannelUsed(id))
    {
        FMOD_RESULT res;
        FMOD_DSP_PARAMETER_FFT *fft;

        if ((res = FMOD_DSP_GetParameterData(mp_Dsp, FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fft, 0, 0, 0)) != FMOD_OK)
            throw LibException("FmodManager::getChannelSpectrum", "FMOD_DSP_GetParameterData", FMOD_ErrorString(res));

        for (int bin = 0; bin < fft->length; bin++)
            values[bin] = (fft->spectrum[0][bin] + fft->spectrum[1][bin]) / 2.0;
    }

    return values;
}

// ==============================
// ==============================

float FmodManager::getVolume(SoundID_t id) const
{
    float volume = 0.0;

    if (isChannelUsed(id))
    {
        FMOD_RESULT res;

        if ((res = FMOD_Channel_GetVolume(mp_Channels.at(id), &volume)) != FMOD_OK)
            throw LibException("FmodManager::getVolume", "FMOD_Channel_GetVolume", FMOD_ErrorString(res));
    }

    return volume;
}

// ==============================
// ==============================

void FmodManager::setVolume(SoundID_t id, float volume) const
{
    if (isChannelUsed(id))
    {
        FMOD_RESULT res;

        if ((res = FMOD_Channel_SetVolume(mp_Channels.at(id), volume)) != FMOD_OK)
            throw LibException("FmodManager::setVolume", "FMOD_Channel_SetVolume", FMOD_ErrorString(res));
    }
}

// ==============================
// ==============================

void FmodManager::setVolume(float volume) const
{
    FMOD_RESULT res;

    if ((res = FMOD_ChannelGroup_SetVolume(mp_ChannelGroup, volume)) != FMOD_OK)
        throw LibException("FmodManager::setVolume", "FMOD_ChannelGroup_SetVolume", FMOD_ErrorString(res));
}

// ==============================
// ==============================

void FmodManager::setMute(bool mute) const
{
    FMOD_RESULT res;

    if ((res = FMOD_ChannelGroup_SetMute(mp_ChannelGroup, static_cast<FMOD_BOOL>(mute))) != FMOD_OK)
        throw LibException("FmodManager::setMute", "FMOD_ChannelGroup_SetMute", FMOD_ErrorString(res));
}

// ==============================
// ==============================

std::string FmodManager::getSongTag(SoundID_t id, const std::string& tagName) const
{
    FMOD_RESULT res;
    FMOD_TAG tag;

    res = FMOD_Sound_GetTag(mp_Sounds.at(id), tagName.c_str(), 0, &tag);

    if (res == FMOD_ERR_TAGNOTFOUND)
        return "";
    else if (res != FMOD_OK)
        throw LibException("FmodManager::getSongTag", "FMOD_Sound_GetTag", FMOD_ErrorString(res));
    else
    {
        std::string tagData = static_cast<char*>(tag.data);
        return tagData;
    }
}

// ==============================
// ==============================

char* FmodManager::getSongPictureData(SoundID_t id, unsigned int *dataLength) const
{
    FMOD_RESULT res;
    FMOD_TAG tag;

    res = FMOD_Sound_GetTag(mp_Sounds.at(id), "APIC", 0, &tag);

    if (res == FMOD_ERR_TAGNOTFOUND)
        return 0;
    else if (res != FMOD_OK)
        throw LibException("FmodManager::getSongPictureData", "FMOD_Sound_GetTag", FMOD_ErrorString(res));
    else
    {
        char *tagData = static_cast<char*>(tag.data);

        while(*(++tagData));        // Skip MIME msg
        ++tagData;                  // Skip content type
        while(*(++tagData));        // Skip description
        ++tagData;                  // tagData is now pointing to the first byte of jpeg img

        if (dataLength)
            *dataLength = tag.datalen - (tagData - static_cast<char*>(tag.data));

        return tagData;
    }
}
