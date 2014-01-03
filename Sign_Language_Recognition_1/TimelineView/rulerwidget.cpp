/************************************************************************/
/* Created by ywpg                                                      */
/* Modified by xiubo.liang@gmail.com                                    */
/* Last Modified: 2010/01/30                                            */
/************************************************************************/

#include <QtGui/QPainter>
#include <cassert>


#include "rulerwidget.h"

//using namespace Swift;


RulerWidget::RulerWidget(QWidget *parent)
	: QWidget(parent)
{
	m_nBeginFrame = 0;
	m_nMag = 1;

	m_nCurrentFrame = 0;

	m_qrectBlock.setRect(0, 0, 0, 0);
}

RulerWidget::~RulerWidget()
{

}

void RulerWidget::paintEvent(QPaintEvent *event)
{
//                   v--- current frame
//   _______________________________	
//   |                             |
//   |   |         |         |     |
//   | | | | | | | | | | | | | | | |
//   | | | | | | | |###| | | | | | |
//   |   5        10        15     |
//   -------------------------------

	QPainter painter(this);
	QPen pen;
	QBrush brush;

	pen.setStyle(Qt::NoPen);
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(Qt::gray);

	painter.setPen(pen);
	painter.setBrush(brush);
	painter.drawRect(0, 0, size().width(), size().height());

	int w = size().width() - 2 * MARGIN_H;
	int h = size().height() - 2 * MARGIN_V;
	int count = w / (WIDTH_FRAME * m_nMag);
	int startpos = MARGIN_H + FRAME_SPACE_H / 2;

	pen.setStyle(Qt::SolidLine);
	pen.setColor(Qt::black);
	painter.setPen(pen);

	// lines 
	int line_bottom_y = MARGIN_V + h - TEXT_HEIGHT - LINE_TEXT_MARGIN_V;
	int line_long_top_y = MARGIN_V + LINE_TOP_MARGIN_V;
	int line_short_top_y = line_long_top_y + LINE_HEIGHT_DELTA;
	if (line_short_top_y > line_bottom_y)
	{
		line_short_top_y = line_bottom_y - 1;
	}

	for (int i = 0; i < count; ++i)
	{
		if ( 0 == (m_nBeginFrame + i) % 5 )
		{
			// long line
			painter.drawLine(
				startpos + i * (WIDTH_FRAME * m_nMag),
				line_bottom_y,
				startpos + i * (WIDTH_FRAME * m_nMag),
				line_long_top_y
				);

			painter.drawText(
				startpos + i * (WIDTH_FRAME * m_nMag) - _CalcTextOffset(m_nBeginFrame + i),
				MARGIN_V + h,
				QString::number(m_nBeginFrame + i)
				);
		}
		else
		{
			// short line
			painter.drawLine(
				startpos + i * (WIDTH_FRAME * m_nMag),
				line_bottom_y,
				startpos + i * (WIDTH_FRAME * m_nMag),
				line_short_top_y
				);
		}
	}  // for each frame


	// "Current Frame"
	if (m_nCurrentFrame >= m_nBeginFrame 
		&& m_nCurrentFrame <= m_nBeginFrame + count   // yes, +1, when the current frame is just one frame on the right, the block still need to be shown
	   )
	{
//		pen.setStyle(Qt::NoPen);
		pen.setColor(Qt::blue);
		pen.setStyle(Qt::SolidLine);
		painter.setPen(pen);
		brush.setColor(Qt::blue);
		painter.setBrush(brush);

		int xx = (m_nCurrentFrame -1 - m_nBeginFrame) * (WIDTH_FRAME * m_nMag);
		int block_top_y = line_bottom_y - BLOCK_HEIGHT;
		int line_current_frame_top_y = line_long_top_y + LINE_BIG__CURRENT_HEIGHT_DELTA;

		painter.drawRect(
			startpos + xx + 1,
			block_top_y,
			BLOCK_WIDTH -1,
			line_bottom_y - block_top_y 
			);
		m_qrectBlock.setRect(
			startpos + xx + 1,
			block_top_y,
			BLOCK_WIDTH -1,
			line_bottom_y - block_top_y 
			);

		pen.setStyle(Qt::SolidLine);
		pen.setColor(Qt::red);
		painter.setPen(pen);

		painter.drawLine(
			startpos + xx + FRAME_SPACE_H + 1,
			line_current_frame_top_y,
			startpos + xx + FRAME_SPACE_H + 1,
			block_top_y - 1
			);

	}

}

void RulerWidget::resizeEvent(QResizeEvent * event)
{
	ReCalculateRange();
}

void RulerWidget::SetMagnification(int n)
{
	if (n > 0 && n <= MAX_MAGNIFICATION)
	{
		int m_nMag = n;
		ReCalculateRange();
		update();
	}
}

int RulerWidget::ReCalculateRange(void)
{
	int w = size().width() - 2 * MARGIN_H;
	int count = w / (WIDTH_FRAME * m_nMag);

	emit RangeChanged(count);
	return count;
}

void RulerWidget::SetBeginFrameNumber(int n)
{
	n > 0 ? n : 0;  // just in case ...

	m_nBeginFrame = n;

	update();
}

int RulerWidget::_CalcTextOffset(int frameno)
{
	// lazy method, but faster
	int n;

	if (frameno > 99 )
		n = 3;
	else if (frameno > 9)
		n = 2;
	else 
		n = 1;

	return static_cast<int>(n * TEXT_WIDTH /2);

}

void RulerWidget::onSetCurrentFrame(int frameno)
{
	// check range?
	assert(frameno >= 0);
	m_nCurrentFrame = frameno;

	update();

}

void RulerWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton
//		&& m_qrectBlock.contains(event->pos())
		)
	{
		m_pointLast = event->pos();
		m_bHolding = true;

		int startpos = MARGIN_H + FRAME_SPACE_H / 2;

		//int delta = event->pos().x() - m_pointLast.x();
		int newframeno = 
			(event->pos().x() - startpos - (WIDTH_FRAME * m_nMag /2) ) / (WIDTH_FRAME * m_nMag);
		newframeno += m_nBeginFrame;

		if (newframeno < 0)
		{
			newframeno = 0;
		}

		m_nCurrentFrame = newframeno;

		update();

		emit CurrentFrameChanged(m_nCurrentFrame);
	}
}

void RulerWidget::mouseMoveEvent(QMouseEvent * event)
{

	// TODO: stop animation

	if ((event->buttons() & Qt::LeftButton) && m_bHolding)
	{
		//int w = size().width() - 2 * MARGIN_H;
		int startpos = MARGIN_H + FRAME_SPACE_H / 2;

		//int delta = event->pos().x() - m_pointLast.x();
		int newframeno = 
			(event->pos().x() - startpos - (WIDTH_FRAME * m_nMag /2) ) / (WIDTH_FRAME * m_nMag);
		newframeno += m_nBeginFrame;

		if (newframeno < 0)
		{
			newframeno = 0;
		}


		m_nCurrentFrame = newframeno;

		m_pointLast = event->pos();
		update();

		emit CurrentFrameChanged(m_nCurrentFrame);
	}
}

void RulerWidget::mouseReleaseEvent(QMouseEvent * event)
{
	m_bHolding = false;
}