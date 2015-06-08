/*************************************
 * @file    Spectrum.cpp
 * @date    03/08/14
 * @author  Manuel
 *
 * Définitions de la classe Spectrum.
 *************************************
*/

#include "Spectrum.h"
#include "Constants.h"

#include <QImage>
#include <QLinearGradient>

Spectrum::Spectrum(int width, QWidget *parent)
    : QWidget(parent), m_Width(width), m_Lines(m_Width)
{
    setMinimumSize(m_Width, SPECTRUM_HEIGHT);

    int i;

    for (i = 0; i < m_Width; i++)
    {
        m_Lines[i].setCoords(i, height(), i, height());
    }
}

// ==============================
// ==============================

Spectrum::~Spectrum()
{

}

// ==============================
// ==============================

void Spectrum::draw(QPainter *painter) const
{
    int i;

    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0, Qt::red);
    gradient.setColorAt(1.0, Qt::yellow);

    for (i = 0; i < m_Width; i++)
    {
        painter->fillRect(i, m_Lines[i].bottom(), 1, height() - m_Lines[i].bottom(), gradient);
    }
}

// ==============================
// ==============================

void Spectrum::paintEvent(QPaintEvent *event)
{
    QImage image(m_Width, height(), QImage::Format_ARGB32_Premultiplied);
    QPainter imagePainter(&image);
    imagePainter.initFrom(this);
    imagePainter.setRenderHint(QPainter::Antialiasing, true);
    imagePainter.eraseRect(rect());
    imagePainter.fillRect(event->rect(), Qt::black);

    draw(&imagePainter);
    imagePainter.end();

    QPainter widgetPainter(this);
    widgetPainter.drawImage(0, 0, image);
}

// ==============================
// ==============================

void Spectrum::updateValues(SoundID_t id)
{
    int i;
    float spectrumValues[m_Width] = {0.0};

    /* Récupération des valeurs spectrales de FMOD */
    FmodManager::getInstance()->getChannelSpectrum(id, spectrumValues);

    for (i = 0; i < m_Width; i++)
    {
        int lineHeight = spectrumValues[i] * height() * SPECTRUM_RATIO;

        if (lineHeight > height())
          lineHeight = height();
        else if (lineHeight < 0)
          lineHeight = 0;

        int yPos = height() - lineHeight;

        /* Calcul de la position de la colonne */
        m_Lines[i].setBottom(yPos);
    }

    update();
}
