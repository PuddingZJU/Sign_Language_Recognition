/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/22                                                  */
/************************************************************************/

#pragma once

#ifndef TRAININGDIALOG_H
#define TRAININGDIALOG_H

#include <QtGui>
#include <QWidget>
#include <string>
#include <vector>
#include <map>
#include "../SwiftModel/SwiftModel.h"
#include "RuleNode.h"
#include "ui_TrainingDialog.h"

namespace Swift
{
	class TrainingDialog : public QWidget
	{
		Q_OBJECT

	public:
		TrainingDialog(QWidget *parent = 0);
		~TrainingDialog();

	signals:
		void trainingDlgClosed();

	protected:
		void closeEvent(QCloseEvent * event);
		

	private:
		/// The candidate rule node given by the first layer.
		RuleNode* m_prn;
		/// The candidate signs given by layer2.
		std::vector<std::string> m_vFResult;
		/// The map between signs and their corresponding sample file's names.
		std::map<std::string, std::vector<std::string>> m_mSign2File;
		/// Specify the maximum number of the best candidate signs.
		int m_nBestN;
	
	private slots:
		void on_radioButton_RuleSpecified_clicked();
		void on_radioButton_RuleAutomatic_clicked();
		void on_pushButton_TrainRModel_clicked();
		void on_pushButton_SaveRModel_clicked();
		void on_pushButton_LoadRModel_clicked();
		void on_pushButton_OpenFile_1_clicked();
		void on_pushButton_GenFSamples_clicked();
		void on_pushButton_SaveFSamples_clicked();
		void on_radioButton_FeatureSpecified_clicked();
		void on_radioButton_FeatureAutomatic_clicked();
		void on_pushButton_TrainFModels_clicked();
		void on_pushButton_SaveFModels_clicked();
		void on_pushButton_LoadFModels_clicked();
		void on_pushButton_OpenFile_2_clicked();
		void on_radioButton_HMMSpecified_clicked();
		void on_radioButton_HMMAutomatic_clicked();
		void on_pushButton_LoadSampleInfo_clicked();
		void on_pushButton_GenHMMs_clicked();
		void on_pushButton_TrainHMMs_clicked();
		void on_pushButton_SaveHMMs_clicked();
		void on_pushButton_LoadHMMs_clicked();
		void on_pushButton_OpenFile_3_clicked();

	private:
		Ui::TrainingDialogClass ui;
	};
}	// Swift

#endif // TRAININGDIALOG_H
