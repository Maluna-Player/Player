/*************************************
 * @file    SpectrumColorDialog.h
 * @date    03/02/17
 * @author  Manuel
 *
 * Déclarations de la classe SpectrumColorDialog
 * définissant la fenêtre de personnalisation
 * des couleurs du spectre.
 *************************************
*/

#ifndef __SPECTRUMCOLORDIALOG_H__
#define __SPECTRUMCOLORDIALOG_H__

#include <QColorDialog>
#include <QButtonGroup>
#include <QRadioButton>
#include <QListWidget>
#include "SpectrumColorWidget.h"


namespace gui {


class SpectrumColorDialog : public QDialog
{
    Q_OBJECT

    private:

        QColorDialog *mp_ColorWidget;

        QListWidget *mp_CreatedColorsList;

        QButtonGroup *mp_ColorButtonsGroup;

        QPair<QRadioButton*, QRadioButton*> mp_ColorButtons;

        QPixmap m_DeleteIcon;


        /**
         * @brief getCurrentColorWidget
         * @return Widget de couleur de spectre sélectionné.
         */
        SpectrumColorWidget* getCurrentColorWidget() const;

        /**
         * @brief Ajoute une nouvelle couleur dans la liste.
         */
        void addNewColor(const SpectrumColor& color);

        /**
         * @brief Change la personnalisation pour la couleur courante.
         */
        void changeCurrentColor();

    private slots:

        /**
         * @brief Met à jour le color widget sélectionné à partir de la palette.
         * @param color Nouvelle couleur sélectionnée
         */
        void updateCurrentColor(const QColor& color);

        /**
         * @brief Supprime la couleur sélectionnée dans la liste.
         */
        void removeCurrentColor();

    signals:

        /**
         * @brief Signal émis lorsque l'utilisateur appuie sur le bouton Ok de la fenêtre.
         * @param index Index de la couleur
         */
        void colorSelected(int index);

        /**
         * @brief Signal émis lorsqu'une nouvelle couleur est ajoutée dans la liste.
         * @param color Couleur ajoutée
         */
        void colorAdded(const SpectrumColor& color);

        /**
         * @brief Signal émis lorsqu'une couleur est modifiée.
         * @param index Index de la couleur
         * @param color Nouvelle couleur
         */
        void colorChanged(int index, const SpectrumColor& color);

        /**
         * @brief Signal émis lorsqu'une couleur est supprimée de la liste.
         * @param index Index de la couleur
         */
        void colorRemoved(int index);

    public:

        SpectrumColorDialog(QWidget *parent = nullptr);
        SpectrumColorDialog(const QList<SpectrumColor>& colors, QWidget *parent = nullptr);
        virtual ~SpectrumColorDialog();

        /**
         * @brief Change la couleur courante dans la liste.
         * @param index Index de la nouvelle couleur
         */
        void setCurrentColor(int index);

    public slots:

        virtual void accept() override;
};


} // gui

#endif  // __SPECTRUMCOLORDIALOG_H__
