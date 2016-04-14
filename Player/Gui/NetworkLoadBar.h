/*************************************
 * @file    NetworkLoadBar.h
 * @date    18/08/15
 * @author  Manuel
 *
 * Déclarations de la classe NetworkLoadBar
 * correspondant à la barre de chargement réseau.
 *************************************
*/

#ifndef __NETWORKLOADBAR_H__
#define __NETWORKLOADBAR_H__

#include <QProgressBar>

class NetworkLoadBar : public QProgressBar
{
    private:

        unsigned int m_StartPos;

    protected:

        virtual void paintEvent(QPaintEvent * event);

        virtual void resizeEvent(QResizeEvent *event);

    public:

        NetworkLoadBar(QWidget *parent = nullptr);
        virtual ~NetworkLoadBar();

        /**
         * @brief Modifie la valeur de position de départ de la barre.
         * @param pos Position de départ en pourcentage
         */
        virtual void setStartPos(unsigned int pos);
};

#endif  // __NETWORKLOADBAR_H__

