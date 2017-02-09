/*************************************
 * @file    Spectrum.h
 * @date    03/08/14
 * @author  Manuel
 *
 * Déclarations de la classe Spectrum
 * contenant les propriétés du spectre
 * du son.
 *************************************
*/

#ifndef __SPECTRUM_H__
#define __SPECTRUM_H__

#include "../Audio/FmodManager.h"
#include <QWidget>
#include <QVector>
#include <QRect>
#include <QPainter>
#include <QPaintEvent>


namespace gui {


struct SpectrumColor
{
    QColor topColor;
    QColor bottomColor;
};

class Spectrum : public QWidget
{
    private:

        int m_Width;

        QVector<QRect> m_Lines;

        SpectrumColor m_Color;


        /**
         * @brief Dessine le contenu du spectre dans le painter passé en paramètre.
         * @param painter Cible du dessin
         */
        void draw(QPainter *painter) const;

    protected:

        virtual void paintEvent(QPaintEvent *event) override;

    public:

        Spectrum(int width, QWidget *parent = nullptr);
        virtual ~Spectrum() = default;

        /**
         * @brief Change la couleur du spectre.
         * @param color Nouvelle couleur du spectre
         */
        void setColor(const SpectrumColor& color);

        /**
         * @brief Remet à 0 les valeurs du spectre.
         */
        void clear();

        /**
         * @brief Récupère les fréquences du son joué et met à jour les valeurs des vertices.
         * @param id Identifiant du canal à tester
        */
        void updateValues(audio::SoundID_t id);
};


} // gui

#endif  // __SPECTRUM_H__
