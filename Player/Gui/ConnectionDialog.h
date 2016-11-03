/*************************************
 * @file    ConnectionDialog.h
 * @date    12/08/15
 * @author  Manuel
 *
 * Déclarations de la classe ConnectionDialog
 * contenant les widgets de
 * connexion/déconnexion avec un pair.
 *************************************
*/

#ifndef __CONNECTIONDIALOG_H__
#define __CONNECTIONDIALOG_H__

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "PlayerLabel.h"


namespace gui {


class ConnectionDialog : public QDialog
{
    Q_OBJECT

    private:

        QPixmap m_ConnectedIcon;
        QPixmap m_DisconnectedIcon;

        QLabel *mp_StateIcon;
        PlayerLabel *mp_ConnectionState;
        PlayerLabel *mp_ActionState;

        QLineEdit *mp_HostLine;

        QPushButton *mp_ListenButton;
        QPushButton *mp_ConnectButton;
        QPushButton *mp_CancelButton;
        QPushButton *mp_DisconnectButton;

    private slots:

        void listen();

        void connectToHost();

        void cancel();

    signals:

        void listened();

        void connectedToHost(QString);

        void canceled();

        void disconnected();

    public:

        ConnectionDialog(QWidget *parent = nullptr);
        virtual ~ConnectionDialog() = default;

        void connected();

    public slots:

        void disconnect();
};


} // gui

#endif  // __CONNECTIONDIALOG_H__

