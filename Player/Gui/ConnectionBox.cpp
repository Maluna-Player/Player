/*************************************
 * @file    ConnectionBox.cpp
 * @date    12/08/15
 * @author  Manuel
 *
 * Définitions de la classe ConnectionBox.
 *************************************
*/

#include "ConnectionBox.h"
#include <QHBoxLayout>


namespace gui {


ConnectionBox::ConnectionBox(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;

    mp_HostLine = new QLineEdit;
    mp_HostLine->setPlaceholderText("Host");

    mp_ListenButton = new QPushButton("Listen");
    mp_ConnectButton = new QPushButton("Connexion");
    mp_CancelButton = new QPushButton("Annuler");
    mp_DisconnectButton = new QPushButton("Déconnexion");

    mp_CancelButton->hide();
    mp_DisconnectButton->hide();

    layout->addWidget(mp_HostLine);
    layout->addWidget(mp_ListenButton);
    layout->addWidget(mp_ConnectButton);
    layout->addWidget(mp_CancelButton);
    layout->addWidget(mp_DisconnectButton);

    setLayout(layout);

    connect(mp_ListenButton, SIGNAL(clicked()), this, SLOT(listen()));
    connect(mp_ConnectButton, SIGNAL(clicked()), this, SLOT(connectToHost()));
    connect(mp_CancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(mp_DisconnectButton, SIGNAL(clicked()), this, SLOT(disconnect()));
}

// ==============================
// ==============================

ConnectionBox::~ConnectionBox()
{

}

// ==============================
// ==============================

void ConnectionBox::listen()
{
    mp_ListenButton->setText("Listening..");
    mp_ListenButton->setEnabled(false);
    mp_ConnectButton->hide();
    mp_CancelButton->show();

    emit listened();
}

// ==============================
// ==============================

void ConnectionBox::connectToHost()
{
    mp_ListenButton->hide();
    mp_ConnectButton->setText("Connexion..");
    mp_ConnectButton->setEnabled(false);
    mp_CancelButton->show();

    emit connectedToHost(mp_HostLine->text());
}

// ==============================
// ==============================

void ConnectionBox::cancel()
{
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

void ConnectionBox::connected()
{
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

void ConnectionBox::disconnect()
{
    mp_HostLine->show();
    mp_ListenButton->show();
    mp_ConnectButton->show();

    mp_ListenButton->setEnabled(true);
    mp_ConnectButton->setEnabled(true);

    mp_ListenButton->setText("Listen");
    mp_ConnectButton->setText("Connect");

    mp_DisconnectButton->hide();

    emit disconnected();
}


} // gui
