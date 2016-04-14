/*************************************
 * @file    AboutDialog.h
 * @date    15/06/15
 * @author  Manuel
 *
 * Déclarations de la classe AboutDialog
 * définissant la fenêtre d'informations.
 *************************************
*/

#ifndef __ABOUTDIALOG_H__
#define __ABOUTDIALOG_H__

#include <QDialog>

class AboutDialog : public QDialog
{
    public:

        AboutDialog(QWidget *parent = nullptr);
        virtual ~AboutDialog();
};

#endif  // __ABOUTDIALOG_H__

