/*************************************
 * @file    ClickableLabel.h
 * @date    03/05/15
 * @author  Manuel
 *
 * Déclarations de la classe ClickableLabel
 * correspondant à une image cliquable.
 *************************************
*/

#ifndef __CLICKABLELABEL_H__
#define __CLICKABLELABEL_H__

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT

    signals:

        void clicked();

    protected:

        virtual void mousePressEvent(QMouseEvent *event);

        virtual void mouseReleaseEvent(QMouseEvent *event);

    public:

        ClickableLabel(QWidget *parent = 0);
        virtual ~ClickableLabel();
};

#endif // __CLICKABLELABEL_H__

