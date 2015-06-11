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
