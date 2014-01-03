/************************************************************************/
/* Created by ywpg                                                      */
/* Modified by xiubo.liang@gmail.com                                    */
/* Last Modified: 2010/01/30                                            */
/************************************************************************/

#ifndef _RANGE_WIDGET_H_INCLUDED_
#define _RANGE_WIDGET_H_INCLUDED_

#include <QtGui/QWidget>
#include <QMouseEvent>
#include <QtCore>

//namespace Swift{


class RangeWidget : public QWidget
{
	Q_OBJECT

private:
	int m_nMax;
	int m_nLeft;
	int m_nLength;

	QRect m_qrectBlock;
	QPoint m_pointLast;
	bool m_bHolding;

	// __________________________
	// |   |text1    text2|     |
	// --------------------------
	enum {
		BLOCK_MARGIN_H	= 5,
		BLOCK_MARGIN_V	= 3,
		//TEXT_TOP_MARGIN = 17,
		TEXT_HEIGHT		= 10,
		LEFT_TEXT_START_OFFSET	= 6,  // offset between left of text1 and the left edge of central block
		RIGHT_TEXT_START_OFFSET = 24 // offset between left of text2 and the right edge of central block
	};

public:
	RangeWidget(QWidget *parent = 0);
	~RangeWidget();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent (QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

signals:
	void LeftFrameChanged(int frameno);

public slots:
	/**
	 * Input the max frame number in the scene.
	 */
	void setMaxFrameNo(int maxFrameNo);

	/**
	 * Input the frame number of left-most frame in rulerWidget.
	 */
	void setLeftFrame(int frameno);

	/**
	 * Input the frame count for frames visable in rulerWidget.
	 */
	void setLength(int n);

};

//} // end of namespace Swift


#endif //_RANGE_WIDGET_H_INCLUDED_

/* vim: set ft=cpp: */
