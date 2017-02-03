/*************************************
 * @file    PlayerToggleButton.cpp
 * @date    27/10/16
 * @author  Manuel
 *
 * Définitions de la classe PlayerToggleButton.
 *************************************
*/

#include "PlayerToggleButton.h"
#include "Constants.h"
#include "../Util/Tools.h"


namespace gui {


PlayerToggleButton::PlayerToggleButton(const QString& filename, bool enabled, QWidget *parent)
    : QPushButton(QIcon(util::Tools::loadImage(IMAGES_SUBDIR + filename)), "", parent)
{
    setCheckable(true);
    setChecked(enabled);
    setStyleSheet("QPushButton { background-color: rgba(75,75,75,255) }");
}


} // gui
