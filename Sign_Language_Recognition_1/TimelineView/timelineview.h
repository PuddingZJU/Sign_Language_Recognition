/************************************************************************/
/* Created by ywpg                                                      */
/* Modified by xiubo.liang@gmail.com                                    */
/* Last Modified: 2010/01/30                                            */
/************************************************************************/

#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QWidget>
#include "ui_timelineview.h"

namespace Swift{				
//
//  ^^^ auto-generated code (from *.ui) fail with namespace

class TimeLineView : public QWidget
{
	Q_OBJECT

public:
	TimeLineView(QWidget *parent = 0);
	~TimeLineView();

private:
	Ui::TimeLineViewClass ui;

private:
	QTimer timeline_timerPlay;
	QTime m_time;

	int timeline_nPlayDirection; // 1 for forward, -1 for backward
	double timeline_dFrameRate; 
	double timeline_dPlaySpeed; // 1.0f  by default
	int nCurrentFrame;
	bool m_bAllowOnSelectTakeSlot;

private:
	/**
	* Change the value of nCurrentFrame.
	* @return Whether the value of nCurrentFrame + offset is *WITH IN* range.
	* @note If the result is out of range, it will be set to 0 or max frame number.
	*/
	bool ChangeCurrentFrame(int offset);

	void set_timeline_nPlayDirection(int n);
	/**
	* Go forward / backward for ONE frame.
	*/
	void timeline_play_take_a_step(int direction);

	void enable_play_timer(bool start);
	void set_timeline_dFrameRate(double d);
	void set_timeline_dPlaySpeed(double d);

	void setIcons();


private slots:
	bool SetCurrentFrame(int n);
	void on_toolButtonEnd_clicked();
	void on_toolButtonHead_clicked();
	void on_toolButtonPrevframe_clicked();
	void on_toolButtonNextframe_clicked();
	void on_toolButtonBack_clicked();
	void on_toolButtonPause_clicked();
	void on_toolButtonPlay_clicked();
	/**
	* Go forward / backward.  Frame-drop is allowed.
	*/
	void timeline_play(void);

public slots:
	void on_UpdateCurrentFrame(int frameno);
	void on_SceneObjectChanged();
	void set_timeline_dFrameRate(QString strFrameRate);
	void set_timeline_dPlaySpeed(QString strPlaySpeed);
	void onSelectTake(QString strTakeName);

signals:
	void CurrentFrameChanged(int frameno);
};

} // end of namespace Swift

#endif // TIMELINEVIEW_H

/* vim: set ft=cpp: */
