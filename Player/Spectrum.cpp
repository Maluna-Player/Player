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

Spectrum::Spectrum(int width) : m_Width(width), m_Lines(m_Width)
{
    setMinimumSize(m_Width, SPECTRUM_HEIGHT);

    int i;

    for (i = 0; i < m_Width; i++)
    {
        m_Lines[i].setCoords(i, SPECTRUM_HEIGHT, i, SPECTRUM_HEIGHT);
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

    QLinearGradient gradient(0, 0, 0, SPECTRUM_HEIGHT);
    gradient.setColorAt(0.0, Qt::red);
    gradient.setColorAt(1.0, Qt::yellow);

    for (i = 0; i < m_Width; i++)
    {
        painter->fillRect(i, m_Lines[i].bottom(), 1, SPECTRUM_HEIGHT - m_Lines[i].bottom(), gradient);
    }
}

// ==============================
// ==============================

void Spectrum::paintEvent(QPaintEvent *event)
{
    QImage image(m_Width, SPECTRUM_HEIGHT, QImage::Format_ARGB32_Premultiplied);
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
        int lineHeight = spectrumValues[i] * SPECTRUM_HEIGHT * SPECTRUM_RATIO;

        if (lineHeight > SPECTRUM_HEIGHT)
          lineHeight = SPECTRUM_HEIGHT;
        else if (lineHeight < 0)
          lineHeight = 0;

        int yPos = SPECTRUM_HEIGHT - lineHeight;

        /* Calcul de la position de la colonne */
        m_Lines[i].setBottom(yPos);
    }

    update();
}
