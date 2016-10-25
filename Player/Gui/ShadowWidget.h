/*************************************
 * @file    ShadowWidget.h
 * @date    02/09/16
 * @author  Manuel
 *
 * Déclarations de la classe ShadowWidget
 * définissant un widget transparent.
 *************************************
*/

#ifndef __SHADOWWIDGET_H__
#define __SHADOWWIDGET_H__

#include <QWidget>
#include <QColor>
#include <QPaintEvent>


namespace gui {


class ShadowWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int opacity READ getOpacity WRITE setOpacity)

    private:

        QColor m_Color;

    protected:

        virtual void paintEvent(QPaintEvent *event) override;

    public:

        ShadowWidget(int r, int g, int b, int alpha = 255, QWidget *parent = nullptr);

        ShadowWidget(int alpha = 255, QWidget *parent = nullptr);

        ShadowWidget(const QColor& color, QWidget *parent = nullptr);

        virtual ~ShadowWidget();

        int getOpacity() const;

        void setOpacity(int alpha);
};


} // gui

#endif  // __SHADOWWIDGET_H__
