/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2011/01/27                                                  */
/************************************************************************/

#pragma once

#ifndef ISOLATEDWORDRECOGDIALOG_H
#define ISOLATEDWORDRECOGDIALOG_H

#include <QtGui>
#include <QWidget>
#include "ui_IsolatedWordRecogDialog.h"
#include <fstream>
#include "../SceneViewer/GLWidget.h"
#include "../MotionData/MotionClip.h"

namespace Swift
{
	class IsolatedWordRecogDialog : public QWidget
	{
		Q_OBJECT

	public:
		IsolatedWordRecogDialog(QWidget *parent = 0);
		~IsolatedWordRecogDialog();

		enum
		{
			CYCLING = 0,
			ONEROUND,
			ONETOCYCLING,
		};

	signals:
		void IsolatedWordRecogDlgClosed();

	protected:
		void closeEvent(QCloseEvent * event);
		void updatePlayControls();

		void recognize(MotionClip* pmc, int nStartFn, int nEndFn);

	private:
		GLWidget* m_glWidget;
		MotionClip* m_pCurrentMC;
		int m_nPlayState;
		QTimer  m_timerPreview;
		QTime 	m_time;
		float 	m_fPlaySpeed;
		int 	m_nFrameRate;
		int m_nStartFn, m_nEndFn, m_nState;


	private slots:
		void on_pushButton_Play_clicked();
		void on_pushButton_StopPlay_clicked();
		void on_radioButton_Stick_clicked();
		void on_radioButton_Ellipse_clicked();
		void on_pushButton_LoadMotion_clicked();
		void on_pushButton_LoadModel_clicked();
		void enablePlayTimer(bool start);
		bool setCurrentFrame(int n);
		bool changeCurrentFrame(int offset);
		void onPlayTimer();

	private:
		Ui::IsolatedWordRecogDialogClass ui;
	};
}

#endif // ISOLATEDWORDRECOGDIALOG_H
