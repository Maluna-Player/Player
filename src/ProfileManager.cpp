/*************************************
 * @file    ProfileManager.cpp
 * @date    06/03/17
 * @author  Manuel
 *
 * Définitions de la classe ProfileManager.
 *************************************
*/

#include "ProfileManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "Constants.h"


bool ProfileManager::load()
{
    m_TotalListeningSeconds = 0;

    QFile profileFile(PROFILE_FILEPATH);
    if (!profileFile.open(QIODevice::ReadOnly))
    {
        if (profileFile.exists())
            return false;

        return true;
    }

    QByteArray profileData = profileFile.readAll();
    QJsonDocument profileDoc = QJsonDocument::fromJson(profileData);
    QJsonObject json = profileDoc.object();

    m_TotalListeningSeconds = json["listeningSeconds"].toInt();

    return true;
}

// ==============================
// ==============================

bool ProfileManager::save() const
{
    QFile profileFile(PROFILE_FILEPATH);
    if (!profileFile.open(QIODevice::WriteOnly))
        return false;

    QJsonObject profileObject;
    profileObject["listeningSeconds"] = static_cast<int>(m_TotalListeningSeconds);

    QJsonDocument profileDoc(profileObject);
    profileFile.write(profileDoc.toJson());

    return true;
}

// ==============================
// ==============================

unsigned int ProfileManager::getListeningTime() const
{
    return m_TotalListeningSeconds;
}

// ==============================
// ==============================

void ProfileManager::setListeningTime(unsigned int seconds)
{
    m_TotalListeningSeconds = seconds;
}
