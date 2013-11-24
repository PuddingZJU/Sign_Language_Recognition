/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2011/03/04                                                  */
/************************************************************************/

#pragma once

#ifndef LIBHMMBUILDERDIALOG_H
#define LIBHMMBUILDERDIALOG_H

#include <QWidget>
#include <QtGui>
#include <QProcess>
#include <string>
#include <map>
#include "HMM.h"
#include "ui_libhmmBuilderDialog.h"

namespace Swift
{
	class libhmmBuilderDialog : public QWidget
	{
		Q_OBJECT

	public:
		libhmmBuilderDialog(QWidget *parent = 0);
		~libhmmBuilderDialog();

	signals:
		void libhmmBuilderDlgClosed();

	protected:
		void closeEvent(QCloseEvent * event);

	private slots:
		void on_pushButton_Generate_clicked();
		void on_pushButton_Save_clicked();
		void on_pushButton_Test1_clicked();
		void on_pushButton_Test2_clicked();

	private:
		std::map<std::string, HMM*> m_mHMM;

	private:
		Ui::libhmmBuilderDialogClass ui;
	}; // libhmmBuilderDialog
}; // Swift


#endif // LIBHMMBUILDERDIALOG_H
