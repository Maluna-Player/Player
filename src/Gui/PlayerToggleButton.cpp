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
    : QPushButton(parent), m_CommonAction(nullptr)
{
    setCheckable(true);
    setChecked(enabled);
    setStyleSheet("QPushButton { background-color: rgb(90, 90, 90); }"
                  "QPushButton::checked { background-color: rgb(75, 75, 75); }");

    if (!filename.isEmpty())
        QPushButton::setIcon(util::Tools::loadImage(IMAGES_SUBDIR + filename));

    connect(this, &QPushButton::toggled, this, &PlayerToggleButton::changeState);
}

// ==============================
// ==============================

void PlayerToggleButton::setIcon(const QIcon& icon)
{
    m_Icons.clear();
    QPushButton::setIcon(icon);
}

// ==============================
// ==============================

void PlayerToggleButton::setIcon(bool checked, const QIcon& icon)
{
    m_Icons[checked] = icon;
    QPushButton::setIcon(m_Icons[isChecked()]);
}

// ==============================
// ==============================

void PlayerToggleButton::setToolTip(const QString& tooltip)
{
    m_Tooltips.clear();
    QPushButton::setToolTip(tooltip);
}

// ==============================
// ==============================

void PlayerToggleButton::setToolTip(bool checked, const QString& tooltip)
{
    m_Tooltips[checked] = tooltip;
    QPushButton::setToolTip(m_Tooltips[isChecked()]);
}

// ==============================
// ==============================

void PlayerToggleButton::setAction(const std::function<void(bool)>& f)
{
    m_Actions.clear();
    m_CommonAction = f;
}

// ==============================
// ==============================

void PlayerToggleButton::setAction(bool checked, const std::function<void(bool)>& f)
{
    m_CommonAction = nullptr;
    m_Actions[checked] = f;
}

// ==============================
// ==============================

void PlayerToggleButton::changeState(bool newState)
{
    if (m_Icons.size() > 0)
        QPushButton::setIcon(m_Icons[newState]);

    if (m_Tooltips.size() > 0)
        QPushButton::setToolTip(m_Tooltips[newState]);

    if (m_Actions.size() > 0)
    {
        if (m_Actions[newState] != nullptr)
            m_Actions[newState](newState);
    }
    else if (m_CommonAction != nullptr)
        m_CommonAction(newState);
}


} // gui
