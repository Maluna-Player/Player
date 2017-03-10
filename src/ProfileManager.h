/*************************************
 * @file    ProfileManager.h
 * @date    06/03/17
 * @author  Manuel
 *
 * Déclarations de la classe
 * ProfileManager gérant les données
 * de profil de l'utilisateur.
 *************************************
*/

#ifndef __PROFILEMANAGER_H__
#define __PROFILEMANAGER_H__


class ProfileManager
{
    private:

        unsigned int m_TotalListeningSeconds;

    public:

        ProfileManager() = default;
        virtual ~ProfileManager() = default;

        bool load();

        bool save() const;

        unsigned int getListeningTime() const;

        void setListeningTime(unsigned int seconds);
};

#endif  // __PROFILEMANAGER_H__
