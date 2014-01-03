/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/01/28                                                  */
/************************************************************************/

#include <QtGui>
#include "scenewindow.h"
#include "../SwiftModel/SwiftModel.h"
#include <QtWidgets>

using namespace Swift;

SceneWindow::SceneWindow(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	setFocusPolicy(Qt::StrongFocus);

	m_pMainGridLayout = new QGridLayout;
	m_pMainGridLayout->setMargin(0);//modified by rukun.fan@gmail.com, 11.13,2007

	m_pSubGridLayout = NULL;

	m_nWidgetCount = 1;
	
	m_glWidgets[0] = 0;

	m_pCurrentGlWidget = NULL;
	
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j) 
		{
			//QColor clearColor;
			//clearColor.setRgbF(0.63f,0.63f,0.63f,1.0f);

			m_glWidgets[2*i+j] = new GLWidget(0, m_glWidgets[0]);
			//m_glWidgets[2*i+j]->setClearColor(clearColor);
			m_glWidgets[2*i+j]->setParent(this);
			m_pMainGridLayout->addWidget(m_glWidgets[2*i+j], i, j);

			connect(m_glWidgets[2*i+j], SIGNAL(clicked()), this, SLOT(clickGlWidget()));
		}
	}
	setLayout(m_pMainGridLayout);


	m_glWidgets[0]->show();
	m_glWidgets[1]->hide();
	m_glWidgets[2]->hide();
	m_glWidgets[3]->hide();

	m_glWidgets[0]->setViewMode(PERSPECTIVE);
	m_glWidgets[1]->setViewMode(ORTHO_FRONT);
	m_glWidgets[2]->setViewMode(ORTHO_TOP);
	m_glWidgets[3]->setViewMode(ORTHO_RIGHT);

	setWindowTitle(tr("SceneWindow"));

	QObject::connect(&SwiftModel::instance(), SIGNAL(needUpdateGL()), this, SLOT(updateAll()));
}

SceneWindow::~SceneWindow()
{

}


void SceneWindow::clickGlWidget()
{
	setCurrentGLWidget(qobject_cast<GLWidget *>(sender()));
}

void SceneWindow::setCurrentGLWidget(GLWidget* pGLWidget)
{
	if (m_nWidgetCount > 1)
	{
		if (m_pCurrentGlWidget != pGLWidget)
		{
			m_pCurrentGlWidget = pGLWidget;			
			for (int i=0; i<GLWidgetCount; i++)
			{
				if (m_glWidgets[i] == m_pCurrentGlWidget)
				{
					m_glWidgets[i]->setSelectedFlag(true);
				}
				else
				{
					m_glWidgets[i]->setSelectedFlag(false);
				}
			}		
		}
	}
}

void SceneWindow::updateAll()
{
	for (int i=0; i<4; i++)
	{
		if (m_glWidgets[i]->isVisible())
		{
			m_glWidgets[i]->makeCurrent();
			m_glWidgets[i]->updateGL();
			//m_glWidgets[i]->repaint();
		}
	}
}

void SceneWindow::setCurrentFrame(int nFrameNo)
{
	if (nFrameNo != SwiftModel::instance().getCurrentFrameNo())
	{
		SwiftModel::instance().setCurrentFrameNo(nFrameNo);

		// update
		for (int i=0; i<4; i++)
		{
			if (m_glWidgets[i]->isVisible())
			{
				m_glWidgets[i]->makeCurrent();
				m_glWidgets[i]->updateGL();
			}
		}
	}

}

void SceneWindow::changeToOneGrid()
{
	m_nWidgetCount = 1;
	for (int i=0; i<GLWidgetCount; i++)
	{
		m_glWidgets[i]->setSelectedFlag(false);
	}

	m_glWidgets[0]->hide();
	m_glWidgets[1]->hide();
	m_glWidgets[2]->hide();
	m_glWidgets[3]->hide();
	if (m_pSubGridLayout)
	{
		delete m_pSubGridLayout;
		m_pSubGridLayout = NULL;
	}
	delete m_pMainGridLayout;

	m_pMainGridLayout = new QGridLayout;
	m_pSubGridLayout = new QGridLayout;
	m_pMainGridLayout->addWidget(m_glWidgets[0],0,0);
	m_glWidgets[0]->show();
	setLayout(m_pMainGridLayout);
}

void SceneWindow::changeToTwoGrid()
{
	m_nWidgetCount = 2;
	setCurrentGLWidget(m_glWidgets[0]);
	m_glWidgets[0]->setSelectedFlag(true);

	m_glWidgets[0]->hide();
	m_glWidgets[1]->hide();
	m_glWidgets[2]->hide();
	m_glWidgets[3]->hide();
	if (m_pSubGridLayout)
	{
		delete m_pSubGridLayout;
		m_pSubGridLayout = NULL;
	}	
	delete m_pMainGridLayout;

	m_pMainGridLayout = new QGridLayout;
	m_pSubGridLayout = new QGridLayout;
	for (int i = 0; i < 2; i++)
	{
		m_pMainGridLayout->addWidget(m_glWidgets[i], 0, i);
	}
	m_glWidgets[0]->show();
	m_glWidgets[1]->show();
	setLayout(m_pMainGridLayout);
}

void SceneWindow::changeToThreeGrid()
{
	m_nWidgetCount = 3;
	setCurrentGLWidget(m_glWidgets[0]);
	m_glWidgets[0]->setSelectedFlag(true);

	m_glWidgets[0]->hide();
	m_glWidgets[1]->hide();
	m_glWidgets[2]->hide();
	m_glWidgets[3]->hide();
	if (m_pSubGridLayout)
	{
		delete m_pSubGridLayout;
		m_pSubGridLayout = NULL;
	}
	delete m_pMainGridLayout;

	m_pMainGridLayout = new QGridLayout;
	m_pSubGridLayout = new QGridLayout;
	m_pMainGridLayout->addWidget(m_glWidgets[0],0,0);
	m_pSubGridLayout->addWidget(m_glWidgets[1],0,0);
	m_pSubGridLayout->addWidget(m_glWidgets[2],1,0);
	m_pMainGridLayout->addLayout(m_pSubGridLayout,0,1);
	m_glWidgets[0]->show();
	m_glWidgets[1]->show();
	m_glWidgets[2]->show();
	setLayout(m_pMainGridLayout);
}

void SceneWindow::changeToFourGrid1()
{
	m_nWidgetCount = 4;
	setCurrentGLWidget(m_glWidgets[0]);
	m_glWidgets[0]->setSelectedFlag(true);

	m_glWidgets[0]->hide();
	m_glWidgets[1]->hide();
	m_glWidgets[2]->hide();
	m_glWidgets[3]->hide();
	if (m_pSubGridLayout)
	{
		delete m_pSubGridLayout;
		m_pSubGridLayout = NULL;
	}	
	delete m_pMainGridLayout;
	m_pMainGridLayout = NULL;

	m_pMainGridLayout = new QGridLayout;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++) 
		{
			m_pMainGridLayout->addWidget(m_glWidgets[2*i+j], i, j);
		}
	}

	m_glWidgets[0]->show();
	m_glWidgets[1]->show();
	m_glWidgets[2]->show();
	m_glWidgets[3]->show();
	setLayout(m_pMainGridLayout);
}

void SceneWindow::changeToFourGrid2()
{
	m_nWidgetCount = 4;
	setCurrentGLWidget(m_glWidgets[0]);
	m_glWidgets[0]->setSelectedFlag(true);

	m_glWidgets[0]->hide();
	m_glWidgets[1]->hide();
	m_glWidgets[2]->hide();
	m_glWidgets[3]->hide();
	if (m_pSubGridLayout)
	{
		delete m_pSubGridLayout;
		m_pSubGridLayout = NULL;
	}	
	delete m_pMainGridLayout;

	m_pMainGridLayout = new QGridLayout;
	m_pSubGridLayout = new QGridLayout;
	m_pMainGridLayout->addWidget(m_glWidgets[0],0,0);
	m_pSubGridLayout->addWidget(m_glWidgets[1],0,0);
	m_pSubGridLayout->addWidget(m_glWidgets[2],1,0);
	m_pSubGridLayout->addWidget(m_glWidgets[3],2,0);
	m_pMainGridLayout->addLayout(m_pSubGridLayout,0,1);
	m_glWidgets[0]->show();
	m_glWidgets[1]->show();
	m_glWidgets[2]->show();
	m_glWidgets[3]->show();
	setLayout(m_pMainGridLayout);
}

void SceneWindow::keyPressEvent(QKeyEvent * event)
{
	if (event->modifiers() == Qt::ControlModifier)
	{
		switch(event->key())
		{
		case Qt::Key_1:
			changeToOneGrid();
			break;
		case Qt::Key_2:
			changeToTwoGrid();
			break;
		case Qt::Key_3:
			changeToThreeGrid();
			break;
		case Qt::Key_4:
			changeToFourGrid1();
			break;
		case Qt::Key_5:
			changeToFourGrid2();
			break;
		}
	}

	QWidget::keyPressEvent(event);
}