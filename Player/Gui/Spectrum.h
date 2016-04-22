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


class Spectrum : public QWidget
{
    private:

        int m_Width;

        QVector<QRect> m_Lines;


        /**
         * @brief Dessine le contenu du spectre dans le painter passé en paramètre.
         * @param painter Cible du dessin
         */
        virtual void draw(QPainter *painter) const;

    protected:

        virtual void paintEvent(QPaintEvent *event) override;

    public:

        Spectrum(int width, QWidget *parent = nullptr);
        virtual ~Spectrum();

        /**
         * @brief Remet à 0 les valeurs du spectre.
         */
        virtual void clear();

        /**
         * Récupère les fréquences du son joué
         * et met à jour les valeurs des vertices.
         * @param id Identifiant du canal à tester
        */
        virtual void updateValues(audio::SoundID_t id);
};


} // gui

#endif  // __SPECTRUM_H__
