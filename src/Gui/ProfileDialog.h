/*************************************
 * @file    ProfileDialog.h
 * @date    10/03/17
 * @author  Manuel
 *
 * Déclarations de la classe ProfileDialog
 * définissant la fenêtre de gestion de profil.
 *************************************
*/


#ifndef __PROFILEDIALOG_H__
#define __PROFILEDIALOG_H__

#include <QDialog>
#include "ProfileManager.h"


namespace gui {


class ProfileDialog : public QDialog
{
    public:

        ProfileDialog(const ProfileManager& profile, QWidget *parent = nullptr);
        virtual ~ProfileDialog() = default;
};


} // gui

#endif  // __PROFILEDIALOG_H__
