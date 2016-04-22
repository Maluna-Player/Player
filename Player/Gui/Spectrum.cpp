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


namespace gui {


Spectrum::Spectrum(int width, QWidget *parent)
    : QWidget(parent), m_Width(width), m_Lines(m_Width)
{
    setMinimumSize(m_Width, Constants::SPECTRUM_HEIGHT);

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
        painter->fillRect(i, m_Lines[i].top(), 1, height() - m_Lines[i].top(), gradient);
    }
}

// ==============================
// ==============================

void Spectrum::paintEvent(QPaintEvent * /*event*/)
{
    QImage image(m_Width, height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);

    QPainter imagePainter(&image);
    imagePainter.initFrom(this);
    imagePainter.setRenderHint(QPainter::Antialiasing, true);

    draw(&imagePainter);
    imagePainter.end();

    QPainter widgetPainter(this);
    widgetPainter.drawImage(0, 0, image);
}

// ==============================
// ==============================

void Spectrum::clear()
{
    for (QRect& rect : m_Lines)
        rect.setTop(height());

    update();
}

// ==============================
// ==============================

void Spectrum::updateValues(audio::SoundID_t id)
{
    int i;
    std::vector<float> spectrumValues(m_Width);

    /* Récupération des valeurs spectrales de FMOD */
    audio::FmodManager::getInstance()->getChannelSpectrum(id, spectrumValues);

    for (i = 0; i < m_Width; i++)
    {
        int lineHeight = spectrumValues[i] * height() * Constants::SPECTRUM_RATIO;

        if (lineHeight > height())
          lineHeight = height();
        else if (lineHeight < 0)
          lineHeight = 0;

        int yPos = height() - lineHeight;

        /* Calcul de la position de la colonne */
        m_Lines[i].setTop(yPos);
    }

    update();
}


} // gui
