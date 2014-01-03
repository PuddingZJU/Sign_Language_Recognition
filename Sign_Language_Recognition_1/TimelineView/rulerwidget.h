/************************************************************************/
/* Created by ywpg                                                      */
/* Modified by xiubo.liang@gmail.com                                    */
/* Last Modified: 2010/01/30                                            */
/************************************************************************/

#ifndef _RULER_WIDGET_H_INCLUDED_
#define _RULER_WIDGET_H_INCLUDED_

#include <QtGui/QWidget>
#include <QMouseEvent>
#include <QtCore>

//namespace Swift{


class RulerWidget : public QWidget
{
	Q_OBJECT

//                   v--- current frame
//   _______________________________	
//   |                             |
//   |   |         |         |     |
//   | | | | | | | | | | | | | | | |
//   | | | | | | | |###| | | | | | |
//   |   5        10        15     |
//   -------------------------------

private:
	enum{
		MARGIN_H	= 3,
		MARGIN_V 	= 1,
		LINE_TOP_MARGIN_V	= 1,
		TEXT_HEIGHT = 10,
		TEXT_WIDTH 	= 5,
		LINE_TEXT_MARGIN_V 	= 2,
		LINE_BIG__CURRENT_HEIGHT_DELTA = 0,
		LINE_HEIGHT_DELTA 	= 5,
		//LINE_BIG__BLOCK_HEIGHT_DELTA = 5,
		FRAME_SPACE_H 	= 6,
		WIDTH_FRAME = FRAME_SPACE_H + 1,	// better be odd
		BLOCK_WIDTH = FRAME_SPACE_H + WIDTH_FRAME, 		// the block indicates current frame

		BLOCK_HEIGHT = 8,

		MAX_MAGNIFICATION 	= 5
		//TEXT_OFFSET 	= TEXT_HEIGHT + TEXT_HEIGHT/2	 // make the text in the middle
	};

	int m_nBeginFrame;
	int m_nMag; // magnification

	int m_nCurrentFrame;
	bool m_bHolding;
	QRect m_qrectBlock;
	QPoint m_pointLast;

public:
	RulerWidget(QWidget *parent = 0);
	~RulerWidget();


	//int GetRangeLength(void) const;


protected:
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent * event);
	void mousePressEvent (QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	int ReCalculateRange(void);
	int _CalcTextOffset(int frameno);

signals:
	void RangeChanged(int length);
	void CurrentFrameChanged(int frameno);

public slots:
	void SetMagnification(int n);
	void SetBeginFrameNumber(int n);
	void onSetCurrentFrame(int frameno);
};

//} // end of namespace Swift


#endif //_RULER_WIDGET_H_INCLUDED_

/* vim: set ft=cpp: */
