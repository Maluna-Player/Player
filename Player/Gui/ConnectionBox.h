/*************************************
 * @file    ConnectionBox.h
 * @date    12/08/15
 * @author  Manuel
 *
 * Déclarations de la classe ConnectionBox
 * contenant les widgets de
 * connexion/déconnexion avec un pair.
 *************************************
*/

#ifndef __CONNECTIONBOX_H__
#define __CONNECTIONBOX_H__

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class ConnectionBox : public QWidget
{
    Q_OBJECT

    private:

        QLineEdit *mp_HostLine;

        QPushButton *mp_ListenButton;
        QPushButton *mp_ConnectButton;
        QPushButton *mp_CancelButton;
        QPushButton *mp_DisconnectButton;

    private slots:

        virtual void listen();

        virtual void connectToHost();

        virtual void cancel();

    signals:

        void listened();

        void connectedToHost(QString);

        void canceled();

        void disconnected();

    public:

        ConnectionBox(QWidget *parent = nullptr);
        virtual ~ConnectionBox();

        virtual void connected();

    public slots:

        virtual void disconnect();
};

#endif  // __CONNECTIONBOX_H__

