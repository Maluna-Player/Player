/*************************************
 * @file    PlayerLabel.h
 * @date    08/06/15
 * @author  Manuel
 *
 * Déclarations de la classe PlayerLabel
 * définissant un QLabel paramétrable (couleur, police)
 *************************************
*/

#ifndef __PLAYERLABEL_H__
#define __PLAYERLABEL_H__

#include <QLabel>

class PlayerLabel : public QLabel
{
    public:

        PlayerLabel(QWidget *parent = 0);
        PlayerLabel(const QColor& color, int size = -1, QWidget *parent = 0);
        PlayerLabel(const QColor& color, const QFont& font, QWidget *parent = 0);

        virtual ~PlayerLabel();


        /**
         * @brief Change la couleur du texte du label
         * @param color Couleur à affecter au texte
         */
        virtual void setColor(const QColor& color);

        /**
         * @brief Met ou retire l'effet gras sur le texte du label.
         * @param enable true si l'effet doit être activé
         */
        virtual void setBold(bool enable);

        /**
         * @brief Met ou retire l'effet italique sur le texte du label.
         * @param enable true si l'effet doit être activé
         */
        virtual void setItalic(bool enable);
};

#endif // __PLAYERLABEL_H__

