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
#include <QMimeDatabase>
#include "Exceptions/FileLoadingException.h"


namespace util {


QString Tools::msToString(int ms, const QString& format)
{
    QTime baseTime(0, 0, 0);
    QTime labelTime = baseTime.addMSecs(ms);

    QString timeFormat = format;
    if (timeFormat.isEmpty())
    {
        if (labelTime.hour() >= 1)
            timeFormat = "hh:mm:ss";
        else
            timeFormat = "mm:ss";
    }

    return labelTime.toString(timeFormat);
}

// ==============================
// ==============================

QPixmap Tools::loadImage(const QString &fileName)
{
    QPixmap image(fileName);
    if (image.isNull())
        throw exceptions::FileLoadingException("Tools::loadImage", fileName.toStdString());

    return image;
}

// ==============================
// ==============================

QString Tools::getMimeType(const QString& fileName)
{
    QMimeDatabase mimeDb;
    return mimeDb.mimeTypeForFile(fileName).name();
}


} // util
