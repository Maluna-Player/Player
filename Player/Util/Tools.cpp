/*************************************
 * @file    Tools.cpp
 * @date    11/06/15
 * @author  Manuel
 *
 * Définitions de la classe Tools.
 *************************************
*/

#include "Tools.h"
#include <QTime>
#include "Exceptions/FileLoadingException.h"


namespace util {


QString Tools::msToString(int ms)
{
    QTime baseTime(0, 0, 0);
    QTime labelTime = baseTime.addMSecs(ms);

    QString format;
    if (labelTime.hour() >= 1)
        format = "hh:mm:ss";
    else
        format = "mm:ss";

    return labelTime.toString(format);
}

// ==============================
// ==============================s

QPixmap Tools::loadImage(const QString &fileName)
{
    QPixmap image(fileName);
    if (image.isNull())
        throw exceptions::FileLoadingException("Tools::loadImage", fileName.toStdString());

    return image;
}


} // util
