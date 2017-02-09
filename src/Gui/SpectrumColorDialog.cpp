/*************************************
 * @file    SpectrumColorDialog.cpp
 * @date    03/02/17
 * @author  Manuel
 *
 * Définitions de la classe SpectrumColorDialog.
 *************************************
*/

#include "SpectrumColorDialog.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include "ClickableLabel.h"
#include "../Util/Tools.h"


namespace gui {


SpectrumColorDialog::SpectrumColorDialog(QWidget *parent)
    : SpectrumColorDialog({}, parent)
{

}

// ==============================
// ==============================

SpectrumColorDialog::SpectrumColorDialog(const QList<SpectrumColor>& colors, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Couleurs du spectre");

    m_DeleteIcon = util::Tools::loadImage(QString(IMAGES_SUBDIR) + "delete.png").scaledToWidth(16);

    QHBoxLayout *dialogLayout = new QHBoxLayout;

    mp_ColorWidget = new QColorDialog;
    mp_ColorButtonsGroup = new QButtonGroup;

    QVBoxLayout *customColorsLayout = new QVBoxLayout;

    QGroupBox *createdColorsBox = new QGroupBox("Couleurs de spectre");
    QVBoxLayout *createdColorsLayout = new QVBoxLayout;
    createdColorsLayout->setContentsMargins(0, 0, 0, 0);

    mp_CreatedColorsList = new QListWidget;
    mp_CreatedColorsList->setStyleSheet("QListWidget { background-color: rgb(221, 220, 216); }"
                                        "QListWidget::item { border-bottom: 1px solid black; }"
                                        "QListWidget::item::selected { background-color: rgb(203, 220, 255); }");

    connect(mp_CreatedColorsList, &QListWidget::currentRowChanged, [this](int row) {
        if (row > -1)
            mp_ColorButtonsGroup->button(row)->setChecked(true);
    });

    createdColorsLayout->addWidget(mp_CreatedColorsList);
    createdColorsBox->setLayout(createdColorsLayout);

    QPushButton *saveColorButton = new QPushButton("Créer une nouvelle\n couleur de spectre");
    saveColorButton->setMaximumWidth(150);

    customColorsLayout->addWidget(createdColorsBox);
    customColorsLayout->addWidget(saveColorButton);

    dialogLayout->addLayout(customColorsLayout);
    dialogLayout->addSpacing(10);
    dialogLayout->addWidget(mp_ColorWidget);
    setLayout(dialogLayout);

    for (const SpectrumColor& color : colors)
        addNewColor(color);

    connect(mp_ColorWidget, &QColorDialog::accepted, this, &SpectrumColorDialog::accept);
    connect(mp_ColorWidget, &QColorDialog::rejected, this, &SpectrumColorDialog::reject);
    connect(mp_ColorWidget, &QColorDialog::finished, this, &SpectrumColorDialog::done);
    connect(mp_ColorWidget, &QColorDialog::currentColorChanged, this, &SpectrumColorDialog::updateCurrentColor);

    connect(saveColorButton, &QPushButton::clicked, [this](){
        addNewColor({mp_ColorWidget->currentColor(), mp_ColorWidget->currentColor()});
    });

    setFixedSize(955, mp_ColorWidget->height() - 60);
}

// ==============================
// ==============================

SpectrumColorWidget* SpectrumColorDialog::getCurrentColorWidget() const
{
    int checkedId = mp_ColorButtonsGroup->checkedId();
    if (checkedId > -1)
    {
        QWidget *itemWidget = mp_CreatedColorsList->itemWidget(mp_CreatedColorsList->item(checkedId));
        QLayout *colorLayout = itemWidget->layout();

        SpectrumColorWidget *colorPreview = static_cast<SpectrumColorWidget*>(colorLayout->itemAt(1)->widget());
        return colorPreview;
    }

    return nullptr;
}

// ==============================
// ==============================

void SpectrumColorDialog::addNewColor(const SpectrumColor& color)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(item->sizeHint().width(), 60));

    QHBoxLayout *colorLayout = new QHBoxLayout;

    QRadioButton *colorButton = new QRadioButton;
    colorButton->setEnabled(false);
    mp_ColorButtonsGroup->addButton(colorButton, mp_ColorButtonsGroup->buttons().size());

    connect(colorButton, &QRadioButton::toggled, [this, colorLayout](bool checked) {
        if (checked)
            changeCurrentColor();
        else
        {
            QLayoutItem* item = colorLayout->takeAt(2);
            delete item->widget();
            delete item;

            if (colorLayout->count() > 3)
            {
                item = colorLayout->takeAt(2);
                delete item->widget();
                delete item;
            }
        }
    });

    colorLayout->setSpacing(20);
    colorLayout->addWidget(colorButton, 0, Qt::AlignVCenter);
    colorLayout->addWidget(new SpectrumColorWidget(color));
    colorLayout->addStretch(1);

    QWidget *colorWidget = new QWidget;
    colorWidget->setLayout(colorLayout);

    mp_CreatedColorsList->addItem(item);
    mp_CreatedColorsList->setItemWidget(item, colorWidget);
    mp_CreatedColorsList->setCurrentItem(item);

    emit colorAdded(color);
}

// ==============================
// ==============================

void SpectrumColorDialog::changeCurrentColor()
{
    QFrame *box = new QFrame;
    box->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QHBoxLayout *boxLayout = new QHBoxLayout;

    mp_ColorButtons = qMakePair(new QRadioButton("Couleur haut"), new QRadioButton("Couleur bas"));
    mp_ColorButtons.first->setChecked(true);

    boxLayout->addWidget(mp_ColorButtons.first);
    boxLayout->addWidget(mp_ColorButtons.second);

    box->setLayout(boxLayout);

    QWidget *colorWidget = mp_CreatedColorsList->itemWidget(mp_CreatedColorsList->currentItem());
    QHBoxLayout *colorLayout = static_cast<QHBoxLayout*>(colorWidget->layout());

    colorLayout->insertWidget(colorLayout->count() - 1, box);

    if (mp_CreatedColorsList->currentRow() > 0)
    {
        ClickableLabel *removeButton = new ClickableLabel;
        removeButton->setPixmap(m_DeleteIcon);
        connect(removeButton, &ClickableLabel::clicked, this, &SpectrumColorDialog::removeCurrentColor);

        colorLayout->insertWidget(colorLayout->count() - 1, removeButton);
    }
}

// ==============================
// ==============================

void SpectrumColorDialog::updateCurrentColor(const QColor& color)
{
    SpectrumColorWidget *colorPreview = getCurrentColorWidget();
    if (colorPreview != nullptr)
    {
        if (mp_ColorButtons.first->isChecked())
            colorPreview->setColor({color, colorPreview->getColor().bottomColor});
        else if (mp_ColorButtons.second->isChecked())
            colorPreview->setColor({colorPreview->getColor().topColor, color});

        colorPreview->update();

        emit colorChanged(mp_ColorButtonsGroup->checkedId(), colorPreview->getColor());
    }
}

// ==============================
// ==============================

void SpectrumColorDialog::removeCurrentColor()
{
    int row = mp_CreatedColorsList->currentRow();
    QListWidgetItem *item = mp_CreatedColorsList->currentItem();

    mp_ColorButtonsGroup->removeButton(mp_ColorButtonsGroup->button(row));
    for (int i = row; i < mp_ColorButtonsGroup->buttons().size(); ++i)
        mp_ColorButtonsGroup->setId(mp_ColorButtonsGroup->buttons().at(i), i);

    mp_CreatedColorsList->setCurrentRow(0);
    mp_CreatedColorsList->removeItemWidget(item);

    item = mp_CreatedColorsList->takeItem(row);
    if (item)
        delete item;

    emit colorRemoved(row);
}

// ==============================
// ==============================

void SpectrumColorDialog::setCurrentColor(int index)
{
    if (index < mp_CreatedColorsList->count())
        mp_CreatedColorsList->setCurrentRow(index);
}

// ==============================
// ==============================

void SpectrumColorDialog::accept()
{
    int checkedId = mp_ColorButtonsGroup->checkedId();
    if (checkedId > -1)
        emit colorSelected(checkedId);

    QDialog::accept();
}


} // gui
