/************************************************************************/
/* Created by ywpg                                                      */
/* Modified by xiubo.liang@gmail.com                                    */
/* Last Modified: 2010/01/30                                            */
/************************************************************************/

#include "timelineview.h"
#include "../SwiftModel/SwiftModel.h"


using namespace Swift;

TimeLineView::TimeLineView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setAutoFillBackground(true);
	setPalette(QPalette(QColor(160,160,160)));

	QObject::connect(
		&(SwiftModel::instance()), SIGNAL(signalLoadFBXScene()),
		this, SLOT(on_SceneObjectChanged())
		);

	QObject::connect(
		findChild<RulerWidget *> ("widgetRuler"), SIGNAL(RangeChanged(int)),
		findChild<RangeWidget *> ("widgetRange"), SLOT(setLength(int))
		);

	QObject::connect(
		findChild<RangeWidget *> ("widgetRange"), SIGNAL(LeftFrameChanged(int)),
		findChild<RulerWidget *> ("widgetRuler"), SLOT(SetBeginFrameNumber(int))
		);

	QObject::connect(
		this, SIGNAL(CurrentFrameChanged(int)),
		findChild<RulerWidget *> ("widgetRuler"), SLOT(SetCurrentFrame(int))
		);

	QObject::connect(
		findChild<RulerWidget *> ("widgetRuler"), SIGNAL(CurrentFrameChanged(int)),
		this, SLOT(SetCurrentFrame(int))
		);

	QObject::connect(
		&(SwiftModel::instance()), SIGNAL(signalCloseFBXScene()), 
		this, SLOT(on_SceneObjectChanged())
		);

	QObject::connect(
		&(SwiftModel::instance()), SIGNAL(signalTakeChanged()), 
		this, SLOT(on_SceneObjectChanged())
		);

	ui.comboBoxFPS->addItem("120 fps");
	ui.comboBoxFPS->addItem("100 fps");
	ui.comboBoxFPS->addItem("60 fps");
	ui.comboBoxFPS->addItem("50 fps");
	ui.comboBoxFPS->addItem("48 fps");
	ui.comboBoxFPS->addItem("30 fps");
	ui.comboBoxFPS->addItem("25 fps");
	ui.comboBoxFPS->addItem("24 fps");

	ui.comboBoxFPS->setCurrentIndex(5);
	QObject::connect(ui.comboBoxFPS, SIGNAL(currentIndexChanged(QString)), this, SLOT(set_timeline_dFrameRate(QString)));

	ui.comboBoxSPD->addItem("1/10 x");
	ui.comboBoxSPD->addItem("1/5 x");
	ui.comboBoxSPD->addItem("1/4 x");
	ui.comboBoxSPD->addItem("1/3 x");
	ui.comboBoxSPD->addItem("1/2 x");
	ui.comboBoxSPD->addItem("1 x");
	ui.comboBoxSPD->addItem("2 x");
	ui.comboBoxSPD->addItem("3 x");
	ui.comboBoxSPD->addItem("4 x");
	ui.comboBoxSPD->addItem("5 x");
	ui.comboBoxSPD->addItem("10 x");
	ui.comboBoxSPD->setCurrentIndex(5);
	QObject::connect(ui.comboBoxSPD, SIGNAL(currentIndexChanged(QString)), this, SLOT(set_timeline_dPlaySpeed(QString)));

	QObject::connect(ui.comboBoxTake, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSelectTake(QString)));

	timeline_timerPlay.setInterval(static_cast<int>(1000/30));
	timeline_timerPlay.stop();
	QObject::connect(&timeline_timerPlay, SIGNAL(timeout()), this, SLOT(timeline_play()));
	timeline_dFrameRate = 30;
	timeline_dPlaySpeed = 1.0;
	nCurrentFrame = 0;
	m_bAllowOnSelectTakeSlot = true;

	setIcons();
}

TimeLineView::~TimeLineView()
{

}

void TimeLineView::setIcons()
{
	ui.toolButtonHead->setIcon(QIcon("../resources/button_timeline_head.png"));
	ui.toolButtonPrevframe->setIcon(QIcon("../resources/button_timeline_prevframe.png"));
	ui.toolButtonBack->setIcon(QIcon("../resources/button_timeline_back.png"));
	ui.toolButtonPause->setIcon(QIcon("../resources/button_timeline_pause.png"));
	ui.toolButtonPlay->setIcon(QIcon("../resources/button_timeline_play.png"));
	ui.toolButtonNextframe->setIcon(QIcon("../resources/button_timeline_nextframe.png"));
	ui.toolButtonEnd->setIcon(QIcon("../resources/button_timeline_end.png"));
}

void TimeLineView::on_toolButtonPlay_clicked()
{
	set_timeline_nPlayDirection(1);
	enable_play_timer(true);
}

void TimeLineView::on_UpdateCurrentFrame(int frameno)
{
	//Ui::TimeLineViewClass.label_6->setNum(frameno); // label_CurrentFrame
	findChild<QLabel *>("label_CurrentFrameNo")->setNum(frameno);
}

void TimeLineView::on_toolButtonPause_clicked()
{
	enable_play_timer(false);
}

void TimeLineView::on_toolButtonBack_clicked()
{
	set_timeline_nPlayDirection(-1);
	enable_play_timer(true);
}

void TimeLineView::on_toolButtonNextframe_clicked()
{
	timeline_play_take_a_step(1);
}

void TimeLineView::on_toolButtonPrevframe_clicked()
{
	timeline_play_take_a_step(-1);
}

void TimeLineView::on_toolButtonHead_clicked()
{
	enable_play_timer(false);
	SetCurrentFrame(0);
}

void TimeLineView::on_toolButtonEnd_clicked()
{
	enable_play_timer(false);
	SetCurrentFrame(SwiftModel::instance().getMaxFrameNo());
}

void TimeLineView::on_SceneObjectChanged()
{
	// change the content of label_maxSceneFrameNo
	findChild<QLabel *>("label_maxSceneFrameNo")->setNum(
		SwiftModel::instance().getMaxFrameNo());
	
	// signal rangeWidget, rulerWidget
	findChild<RangeWidget *> ("widgetRange")->setMaxFrameNo(
		SwiftModel::instance().getMaxFrameNo());
	
	timeline_dFrameRate = SwiftModel::instance().getFrameRate();

	nCurrentFrame = 0;
	findChild<QLabel *>("label_CurrentFrameNo")->setNum(nCurrentFrame);
	findChild<RulerWidget *>("widgetRuler")->onSetCurrentFrame(nCurrentFrame);

	m_bAllowOnSelectTakeSlot = false;
	ui.comboBoxTake->clear();
	for (int i=0; i<SwiftModel::instance().m_vTakeName.size(); i++)
	{
		ui.comboBoxTake->addItem(QString::fromStdString(SwiftModel::instance().m_vTakeName.at(i)));
	}
	ui.comboBoxTake->setCurrentIndex(SwiftModel::instance().m_nCurrentTakeIndex);
	m_bAllowOnSelectTakeSlot = true;
}

void TimeLineView::set_timeline_dFrameRate(QString strFrameRate)
{
	strFrameRate.remove(" fps", Qt::CaseInsensitive);

	SwiftModel::instance().setFrameRate(strFrameRate.toStdString());

	// change the content of label_maxSceneFrameNo
	findChild<QLabel *>("label_maxSceneFrameNo")->setNum(
		SwiftModel::instance().getMaxFrameNo());

	// signal rangeWidget, rulerWidget
	findChild<RangeWidget *> ("widgetRange")->setMaxFrameNo(
		SwiftModel::instance().getMaxFrameNo());

	timeline_dFrameRate = SwiftModel::instance().getFrameRate();
}

void TimeLineView::set_timeline_dPlaySpeed(QString strPlaySpeed)
{
	strPlaySpeed.remove(" x", Qt::CaseInsensitive);

	float fPlaySpeed;
	if (strPlaySpeed == "1/10")
		fPlaySpeed = 0.1;
	else if (strPlaySpeed == "1/5")
		fPlaySpeed = 0.2;
	else if (strPlaySpeed == "1/4")
		fPlaySpeed = 0.25;
	else if (strPlaySpeed == "1/3")
		fPlaySpeed = 0.33;
	else if (strPlaySpeed == "1/2")
		fPlaySpeed = 0.5;
	else
		fPlaySpeed = strPlaySpeed.toDouble();
	set_timeline_dPlaySpeed(fPlaySpeed);
}

void TimeLineView::onSelectTake(QString strTakeName)
{
	if (m_bAllowOnSelectTakeSlot)
	{
		SwiftModel::instance().setCurrentTake(strTakeName.toStdString());

		// change the content of label_maxSceneFrameNo
		findChild<QLabel *>("label_maxSceneFrameNo")->setNum(
			SwiftModel::instance().getMaxFrameNo());

		// signal rangeWidget, rulerWidget
		findChild<RangeWidget *> ("widgetRange")->setMaxFrameNo(
			SwiftModel::instance().getMaxFrameNo());

		timeline_dFrameRate = SwiftModel::instance().getFrameRate();

		nCurrentFrame = 0;
		findChild<QLabel *>("label_CurrentFrameNo")->setNum(nCurrentFrame);
		findChild<RulerWidget *>("widgetRuler")->onSetCurrentFrame(nCurrentFrame);
	}
}

void TimeLineView::set_timeline_nPlayDirection(int n)
{
	if (n > 0)
	{
		timeline_nPlayDirection = 1;
	}
	else 
	{
		timeline_nPlayDirection = -1;
	}
}

void TimeLineView::set_timeline_dFrameRate(double d)
{
	timeline_dFrameRate = d;
}

void TimeLineView::set_timeline_dPlaySpeed(double d)
{
	timeline_dPlaySpeed = d;
}

void TimeLineView::timeline_play_take_a_step(int direction)
{
	// stop timer
	enable_play_timer(false);
	//	nCurrentFrame += 1 * (direction>0?1:-1) ;
	ChangeCurrentFrame((direction>0?1:-1));
}

void TimeLineView::enable_play_timer(bool start)
{
	if (start)
	{
		m_time.restart(); // start timing
		timeline_timerPlay.start();
	}
	else
	{
		timeline_timerPlay.stop();
	}
}

void TimeLineView::timeline_play(void)
{
	int nDeltaTime = m_time.elapsed(); // get delta, and start again. see Qt doc.
	static float fAccumlatedDeltaFrame = 0;
	float fDeltaFrame = 
		timeline_dPlaySpeed * timeline_dFrameRate * nDeltaTime / 1000.0f;
	fAccumlatedDeltaFrame += fDeltaFrame;
	int nRoundedDeltaFrame = static_cast<int>(fAccumlatedDeltaFrame);

	if (nRoundedDeltaFrame > 0)
	{
		m_time.restart();

		if (! ChangeCurrentFrame( 
			timeline_nPlayDirection * nRoundedDeltaFrame
			))
		{
			enable_play_timer(false);
		}
		fAccumlatedDeltaFrame -= nRoundedDeltaFrame;
	}
}

bool TimeLineView::ChangeCurrentFrame(int offset)
{
	return SetCurrentFrame(nCurrentFrame + offset);
}

bool TimeLineView::SetCurrentFrame(int n)
{
	nCurrentFrame = n;

	// check for head
	if (nCurrentFrame < 0)
	{
		nCurrentFrame = 0;
		SwiftModel::instance().setCurrentFrameNo(nCurrentFrame);
		return false;
	}

	// check for max frameno in scene.
	int nMaxFrame = SwiftModel::instance().getMaxFrameNo();

	if (nCurrentFrame > nMaxFrame)
	{
		nCurrentFrame = nMaxFrame;
		nCurrentFrame = (nCurrentFrame < 0 ? 0 : nCurrentFrame);
		SwiftModel::instance().setCurrentFrameNo(nCurrentFrame);
		return false;
	}

	SwiftModel::instance().setCurrentFrameNo(nCurrentFrame);

	findChild<QLabel *>("label_CurrentFrameNo")->setNum(nCurrentFrame);
	findChild<RulerWidget *>("widgetRuler")->onSetCurrentFrame(nCurrentFrame);

	return true;
}