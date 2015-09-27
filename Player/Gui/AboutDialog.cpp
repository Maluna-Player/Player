/*************************************
 * @file    AboutDialog.cpp
 * @date    15/06/15
 * @author  Manuel
 *
 * Définitions de la classe AboutDialog.
 *************************************
*/

#include "AboutDialog.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include "Tools.h"
#include "Constants.h"


AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    QGridLayout *informationLayout = new QGridLayout;

    QLabel *iconLabel = new QLabel;
    iconLabel->setPixmap(Tools::loadImage(Constants::IMAGES_SUBDIR + "icone.ico"));

    QLabel *qtLabel = new QLabel;
    qtLabel->setPixmap(Tools::loadImage(Constants::IMAGES_SUBDIR + "qt.png").scaled(80, 80, Qt::KeepAspectRatio));

    QLabel *mainLabel = new QLabel("<b>Maluna's Player</b><br/>"
                               "Logiciel libre créé par Maluna");

    QLabel *toolsLabel = new QLabel("Réalisé à l'aide du framework Qt");

    QLabel *thanksLabel = new QLabel("<b>Remerciements</b><br/><br/>"
                                     "Lokra pour les graphismes des boutons<br/>"
                                     "<i>http://www.illustration.lokra.fr/</i>");

    informationLayout->addWidget(iconLabel, 1, 0, 2, 1);
    informationLayout->addWidget(mainLabel, 0, 1);
    informationLayout->addWidget(toolsLabel, 1, 1);
    informationLayout->addWidget(thanksLabel, 4, 1);
    informationLayout->addWidget(qtLabel, 1, 2, 2, 1);


    QPushButton *closeButton = new QPushButton("Fermer");
    closeButton->setDefault(true);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    layout->addLayout(informationLayout);
    layout->addLayout(buttonsLayout);

    setLayout(layout);
}

// ==============================
// ==============================

AboutDialog::~AboutDialog()
{

}
