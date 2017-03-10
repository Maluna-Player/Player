/*************************************
 * @file    ProfileDialog.cpp
 * @date    10/03/17
 * @author  Manuel
 *
 * Définitions de la classe ProfileDialog.
 *************************************
*/

#include "ProfileDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include "../Util/Tools.h"
#include "Constants.h"


namespace gui {


ProfileDialog::ProfileDialog(const ProfileManager& profile, QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *dialogLayout = new QVBoxLayout;

    QHBoxLayout *profileLayout = new QHBoxLayout;

    QLabel *profileIcon = new QLabel;
    profileIcon->setPixmap(util::Tools::loadImage(QString(MENU_SUBDIR) + "profile.png"));

    QGroupBox *profileBox = new QGroupBox("Profil");
    QVBoxLayout *infoLayout = new QVBoxLayout;

    QString strTime = util::Tools::msToString(profile.getListeningTime() * 1000, "hh:mm:ss");
    QLabel *listeningTimeLabel = new QLabel("<b>Temps d'écoute</b> : " + strTime);

    infoLayout->addWidget(listeningTimeLabel, 0, Qt::AlignTop);
    profileBox->setLayout(infoLayout);

    profileLayout->addWidget(profileIcon);
    profileLayout->addSpacing(40);
    profileLayout->addWidget(profileBox);

    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setDefault(true);
    connect(closeButton, &QPushButton::clicked, this, &ProfileDialog::close);

    dialogLayout->addLayout(profileLayout);
    dialogLayout->addSpacing(30);
    dialogLayout->addWidget(closeButton, 0, Qt::AlignHCenter);

    setLayout(dialogLayout);
}


} // gui
