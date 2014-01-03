/************************************************************************/
/* Created by ywpg                                                      */
/* Modified by xiubo.liang@gmail.com                                    */
/* Last Modified: 2010/01/30                                            */
/************************************************************************/

#include <QtGui/QPainter>
#include "rangewidget.h"

//using namespace Swift;


RangeWidget::RangeWidget(QWidget *parent)
	: QWidget(parent)
{
	m_nLength = 0;
	m_nMax = 0;
	m_nLeft = 0;

	m_qrectBlock.setRect(0, 0, 0, 0);
}

RangeWidget::~RangeWidget()
{

}

void RangeWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPen pen;

	pen.setStyle(Qt::NoPen);

	painter.setPen(pen);

	// background
	painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
	painter.drawRect(0, 0, size().width(), size().height());

	// the block in the middle
	if (m_nLength > 0 && m_nMax > 0)
	{

		int w = size().width() - 2 * BLOCK_MARGIN_H;
		int h = size().height() - 2 * BLOCK_MARGIN_V;
		int leftpointX = BLOCK_MARGIN_H + w * m_nLeft / m_nMax;
		int rightpointX = BLOCK_MARGIN_H + w * (m_nLeft + m_nLength -1) / m_nMax;
		painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
		painter.drawRect(
			leftpointX, BLOCK_MARGIN_V, 
			rightpointX - leftpointX + 1, h
			);
		m_qrectBlock.setRect(
			leftpointX, BLOCK_MARGIN_V, 
			rightpointX - leftpointX + 1, h
			);


		// __________________________
		// |   |text1    text2|     |
		// --------------------------

		pen.setColor(Qt::white);
		pen.setStyle(Qt::SolidLine);
		painter.setPen(pen);

		int text_top_margin = (h - TEXT_HEIGHT)/2;
		// text1
		painter.drawText(
				leftpointX + LEFT_TEXT_START_OFFSET,
				BLOCK_MARGIN_V + text_top_margin + TEXT_HEIGHT,
				QString::number(m_nLeft)
				);
		
		// text2
		painter.drawText(
				rightpointX - RIGHT_TEXT_START_OFFSET,
				BLOCK_MARGIN_V + text_top_margin + TEXT_HEIGHT,
				QString::number(m_nLeft + m_nLength -1)
				);

		//painter.drawRect( 50, 20, 200, 50);
		//painter.drawRect( 70, -20, 240, 30);
	}

}

void RangeWidget::mousePressEvent(QMouseEvent * event)
{
	if (m_nMax == 0)
		return;

	if (event->button() == Qt::LeftButton
		&& m_qrectBlock.contains(event->pos()) )
	{
		m_pointLast = event->pos();
		m_bHolding = true;
	}
}

void RangeWidget::mouseMoveEvent(QMouseEvent * event)
{
	if (m_nMax == 0)
		return;

	if ((event->buttons() & Qt::LeftButton) && m_bHolding)
	{
		int w = size().width() - 2 * BLOCK_MARGIN_H;
		int delta = event->pos().x() - m_pointLast.x();
		int newLeft = m_nLeft + delta * m_nMax / w;
		if (newLeft < 0)
		{
			newLeft = 0;
		}
		if (newLeft + m_nLength -1 > m_nMax)
		{
			newLeft = m_nMax - m_nLength + 1;
		}
		
		m_nLeft = newLeft;

		m_pointLast = event->pos();
		update();

		emit LeftFrameChanged(m_nLeft);
	}
}

void RangeWidget::mouseReleaseEvent(QMouseEvent * event)
{
	m_bHolding = false;
}

void RangeWidget::setMaxFrameNo(int maxFrameNo)
{
	if (m_nMax != maxFrameNo)
	{
		m_nMax = maxFrameNo;
		this->update();
	}
}

void RangeWidget::setLeftFrame(int frameno)
{
	if (m_nLeft != frameno)
	{
		m_nLeft = frameno;
		this->update();
	}
}

void RangeWidget::setLength(int n)
{
	if (m_nLength != n)
	{
		m_nLength = n;

		//if (m_nMax> 0 && m_nLeft + m_nLength -1 > m_nMax)
		//{
		//	setLeftFrame(m_nMax - m_nLength + 1);
		//	emit LeftFrameChanged(m_nMax - m_nLength + 1);
		//}
		//else
		//{
			this->update();
		//}
	}
}

