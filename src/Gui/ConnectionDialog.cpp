/*************************************
 * @file    ConnectionDialog.cpp
 * @date    12/08/15
 * @author  Manuel
 *
 * Définitions de la classe ConnectionDialog.
 *************************************
*/

#include "ConnectionDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Constants.h"
#include "../Util/Tools.h"


namespace gui {


ConnectionDialog::ConnectionDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Connexion à un pair");
    setFixedSize(400, 200);

    m_ConnectedIcon = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "connected_state.png");
    m_DisconnectedIcon = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "disconnected_state.png");

    QVBoxLayout *dialogLayout = new QVBoxLayout;

    QHBoxLayout *stateLayout = new QHBoxLayout;

    mp_StateIcon = new QLabel;
    mp_StateIcon->setPixmap(m_DisconnectedIcon);
    mp_ConnectionState = new PlayerLabel(QString("Non connecté"));
    mp_ConnectionState->setBold(true);

    stateLayout->addWidget(mp_StateIcon);
    stateLayout->addWidget(mp_ConnectionState);

    mp_ActionState = new PlayerLabel;
    mp_ActionState->setItalic(true);

    QHBoxLayout *inputLayout = new QHBoxLayout;

    mp_HostLine = new QLineEdit;
    mp_HostLine->setPlaceholderText("Hôte");

    mp_ListenButton = new QPushButton("Listen");
    mp_ConnectButton = new QPushButton("Connexion");
    mp_CancelButton = new QPushButton("Annuler");
    mp_DisconnectButton = new QPushButton("Déconnexion");

    mp_CancelButton->hide();
    mp_DisconnectButton->hide();

    inputLayout->addWidget(mp_HostLine);
    inputLayout->addWidget(mp_ListenButton);
    inputLayout->addWidget(mp_ConnectButton);
    inputLayout->addWidget(mp_CancelButton);
    inputLayout->addWidget(mp_DisconnectButton);

    QPushButton *closeButton = new QPushButton("Fermer");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);

    dialogLayout->addSpacing(10);
    dialogLayout->addLayout(stateLayout);
    dialogLayout->addWidget(mp_ActionState);
    dialogLayout->addStretch(1);
    dialogLayout->addLayout(inputLayout);
    dialogLayout->addStretch(1);
    dialogLayout->addWidget(closeButton);

    dialogLayout->setAlignment(stateLayout, Qt::AlignHCenter);
    dialogLayout->setAlignment(mp_ActionState, Qt::AlignHCenter);
    dialogLayout->setAlignment(closeButton, Qt::AlignHCenter);

    setLayout(dialogLayout);

    connect(mp_ListenButton, &QPushButton::clicked, this, &ConnectionDialog::listen);
    connect(mp_ConnectButton, &QPushButton::clicked, this, &ConnectionDialog::connectToHost);
    connect(mp_CancelButton, &QPushButton::clicked, this, &ConnectionDialog::cancel);
    connect(mp_DisconnectButton, &QPushButton::clicked, this, &ConnectionDialog::disconnect);
}

// ==============================
// ==============================

void ConnectionDialog::listen()
{
    mp_ActionState->setText("En attente de connexion...");

    mp_ListenButton->setText("Listening..");
    mp_ListenButton->setEnabled(false);
    mp_ConnectButton->hide();
    mp_CancelButton->show();

    emit listened();
}

// ==============================
// ==============================

void ConnectionDialog::connectToHost()
{
    mp_ActionState->setText("Connexion...");

    mp_ListenButton->hide();
    mp_ConnectButton->setText("Connexion..");
    mp_ConnectButton->setEnabled(false);
    mp_CancelButton->show();

    emit connectedToHost(mp_HostLine->text());
}

// ==============================
// ==============================

void ConnectionDialog::cancel()
{
    mp_ActionState->setText("");

    mp_ListenButton->show();
    mp_ConnectButton->show();

    mp_ListenButton->setEnabled(true);
    mp_ConnectButton->setEnabled(true);

    mp_ListenButton->setText("Listen");
    mp_ConnectButton->setText("Connect");

    mp_CancelButton->hide();

    emit canceled();
}

// ==============================
// ==============================

void ConnectionDialog::connected()
{
    mp_ConnectionState->setText("Connecté");
    mp_StateIcon->setPixmap(m_ConnectedIcon);
    mp_ActionState->setText("");

    mp_HostLine->hide();
    mp_ListenButton->setText("Connecté");
    mp_ListenButton->setEnabled(false);
    mp_ListenButton->show();

    mp_ConnectButton->hide();
    mp_CancelButton->hide();
    mp_DisconnectButton->show();
}

// ==============================
// ==============================

void ConnectionDialog::disconnect()
{
    mp_ConnectionState->setText("Non connecté");
    mp_StateIcon->setPixmap(m_DisconnectedIcon);
    mp_ActionState->setText("");

    mp_HostLine->show();
    mp_ListenButton->show();
    mp_ConnectButton->show();

    mp_ListenButton->setEnabled(true);
    mp_ConnectButton->setEnabled(true);

    mp_ListenButton->setText("Listen");
    mp_ConnectButton->setText("Connexion");

    mp_DisconnectButton->hide();

    emit disconnected();
}


} // gui
