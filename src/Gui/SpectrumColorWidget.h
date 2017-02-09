/*************************************
 * @file    SpectrumColorWidget.h
 * @date    03/02/17
 * @author  Manuel
 *
 * Déclarations de la classe
 * SpectrumColorWidget définissant une vue
 * graphique d'une couleur de spectre.
 *************************************
*/

#ifndef __SPECTRUMCOLORWIDGET_H__
#define __SPECTRUMCOLORWIDGET_H__

#include <QWidget>
#include "Spectrum.h"


namespace gui {


class SpectrumColorWidget : public QWidget
{
    private:

        static constexpr unsigned int WIDTH = 42;
        static constexpr unsigned int HEIGHT = 22;

        SpectrumColor m_Color;

    protected:

        virtual void paintEvent(QPaintEvent *event) override;

    public:

        SpectrumColorWidget(QWidget *parent = nullptr);
        SpectrumColorWidget(const SpectrumColor& color, QWidget *parent = nullptr);

        virtual ~SpectrumColorWidget() = default;

        /**
         * @brief getColor
         * @return Couleur représentée.
         */
        SpectrumColor getColor() const;

        /**
         * @brief Change la couleur représentée par le widget.
         * @param color Nouvelle couleur
         */
        void setColor(const SpectrumColor& color);
};


} // gui

#endif  // __SPECTRUMCOLORWIDGET_H__
