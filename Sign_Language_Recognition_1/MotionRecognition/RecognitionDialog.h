/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/04/24                                                  */
/************************************************************************/

#ifndef RECOGNITIONDIALOG_H
#define RECOGNITIONDIALOG_H

#include <map>
#include <string>
#include <QtGui>
#include <QWidget>
#include "ui_RecognitionDialog.h"

#include "../SceneViewer/GLWidget.h"
#include "../MotionData/MotionClip.h"

namespace Swift
{
	class RecognitionDialog : public QWidget
	{
		Q_OBJECT

		enum
		{
			CYCLING = 0,
			ONEROUND,
			ONETOCYCLING,
		};

	public:
		RecognitionDialog(QWidget *parent = 0);
		~RecognitionDialog();

	signals:
		void recognitionDlgClosed();

	protected:
		void closeEvent(QCloseEvent * event);

	protected:
		void initTreeViews();
		void updatePlayControls();
		void test();

	private:
		GLWidget* m_glWidget;

		std::string m_strMotionDBPath;
		QStandardItemModel* m_modelAll;
		std::map<QString, MotionClip*> m_mMotions;
		MotionClip* m_pCurrentMC;
		int m_nPlayState;
		QString m_strCurMotion;
		QTimer  m_timerPreview;
		QTime 	m_time;
		float 	m_fPlaySpeed;
		int 	m_nFrameRate;
		SimpleObjectType m_currentType;

	private slots:
		void on_pushButton_StopPlay_clicked();
		void on_pushButton_Play_clicked();
		void on_pushButton_LoadMotions_clicked();
		void on_pushButton_LoadConfig_clicked();
		void on_radioButton_Stick_clicked();
		void on_radioButton_Ellipse_clicked();
		void onClickOneRecordOfAll(const QModelIndex & index);
		void enablePlayTimer(bool start);
		bool setCurrentFrame(int n);
		bool changeCurrentFrame(int offset);
		void onPlayTimer();

	private:
		Ui::RecognitionDialogClass ui;

	}; // RecognitionDialog

} //Swift

#endif // RECOGNITIONDIALOG_H
