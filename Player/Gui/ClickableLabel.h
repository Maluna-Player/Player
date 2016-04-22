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
#include <QMouseEvent>


namespace gui {


class ClickableLabel : public QLabel
{
    Q_OBJECT

    signals:

        void clicked();

    protected:

        virtual void mousePressEvent(QMouseEvent *event) override;

        virtual void mouseReleaseEvent(QMouseEvent *event) override;

    public:

        ClickableLabel(QWidget *parent = nullptr);
        virtual ~ClickableLabel();
};


} // gui

#endif // __CLICKABLELABEL_H__

