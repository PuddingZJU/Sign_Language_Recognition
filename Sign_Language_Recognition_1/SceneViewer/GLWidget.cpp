/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/01/28                                                  */
/************************************************************************/
#define GLUT_DISABLE_ATEXIT_HACK
#include <QtOpenGL/QtOpenGL>
#include <GL/glut.h>
#include <GL/GLAux.h>
#include <vector>

#include "GLWidget.h"
#include "../SceneViewer/Ground.h"
#include "../SwiftModel/SwiftModel.h"
#include "SceneWindow.h"
#pragma comment( lib, "glaux.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "OpenGL32.lib" )
using namespace Swift;

const GLfloat RotateStep = 0.1f;
const GLfloat TransStep = 1.0f;
const GLfloat ScaleStep = 0.001f;
const GLfloat MinScale = 0.1f;
const GLfloat MaxScale = 10.0f;
const GLfloat OrthoSize = 200.0f;
// The length of the three axes.
const GLfloat AxisLength = 100.0f;
// The radius of the axis arrow.
const GLfloat AxisArrowRadius = 10.0f;
// The height of the axis arrow.
const GLfloat AxisArrowHeight = 30.0f;
// The left of the view name rect.
const int NameRectLeft = 10;
// The top of the view name rect.
const int NameRectTop = 6;
// The width of the view name rect.
const int NameRectWidth = 56;
// The height of the view name rect.
const int NameRectHeight = 20;

extern const GLWidgetMode GLWidgetModes_Selection[] = {
	CommonMode,
	DraftRetrievalMode,
	GLWidgetMode(-1) // the sentinel at the end of the list
};

AbstractSceneManager * SceneManagerFactory::createSceneManager(GLWidgetMode mode)
{
	if( mode < GLWidgetMode_START || mode >= GLWidgetMode_END){
		return NULL;
	}

	switch( mode ){
	case CommonMode:
		return & SceneManager::instance();
		break;
	case RetrievalMode:
		//return & RetrievalSceneManager::instance();
		break;
	case PreviewMode:
		return new PreviewSceneManager();
		break;
	case ExagerationMode:
		//return new ExagerationSceneManager();
		break;
	case DraftRetrievalMode:
		//return new DraftRetrievalSceneMgr();
		break;
	default:
		assert(0);
		break;
	}

	return NULL;
}

void SceneManagerFactory::recycle(AbstractSceneManager * mgr)
{
	if(NULL == mgr)
		return ;

	{
		SceneManager * scenemgr = dynamic_cast<SceneManager*>(mgr);
		if(NULL != scenemgr){
			return ;
		}
	}

	//{
	//	RetrievalSceneManager * scenemgr = dynamic_cast<RetrievalSceneManager*>(mgr);
	//	if(NULL != scenemgr){
	//		return ;
	//	}
	//}

	delete mgr;
	return ;
}

GLWidget::GLWidget(QWidget* parent, QGLWidget* shareWidget, GLWidgetMode mode/* = CommonMode*/)
	: QGLWidget(parent,shareWidget), m_pSceneManager(NULL)
{
	m_GLWidgetMode = mode;
	createScene();

	m_clearColor.setRgbF(0.63f,0.63f,0.63f,1.0f);

	//rotation and tranlation
    m_xRot = 0.0f;
    m_yRot = 0.0f;
    m_zRot = 0.0f;
	m_xTrans = 0.0f;
	m_yTrans = 0.0f;
	m_zTrans = 0.0f;

	//camera
	m_fScale = 1.0f;
	m_fEyeX = 0.0f;
	m_fEyeY = 100.0f;
	m_fEyeZ = 300.0f;
	m_fCenterX = 0.0f;
	m_fCenterY = 0.0f;
	m_fCenterZ = 0.0f;
	m_fUpX = 0.0f;
	m_fUpY = 1.0f;
	m_fUpZ = 0.0f;

	m_pGround = new Ground;

	m_viewMode = PERSPECTIVE;
	m_bSelectedFlag = false;
	m_bPickedRectFlag = false;
	m_bShowNameRect = false;
	m_bFillNameRect = false;
	m_strViewMode = "Persp";
	m_bSelect = false;
	m_pParent = NULL;

	//new instance of FBXScene.
	//m_FBXScene = new FBXSceneViewer;

	m_currentFrame = 0;
	m_bPlaying = false;

	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{
	//if(m_FBXScene)
	//	delete m_FBXScene;
	//m_FBXScene = NULL;

	if (m_pGround)
	{
		delete m_pGround;
	}
}

void GLWidget::rotateByEularAngle(GLfloat xAngle, GLfloat yAngle, GLfloat zAngle)
{
	m_xRot += xAngle;
    m_yRot += yAngle;
    m_zRot += zAngle;
	if(m_xRot >= 360)
	{
		m_xRot -= 360;
	}
	else if(m_xRot <= -360)
	{
		m_xRot += 360;
	}
	if(m_yRot >= 360)
	{
		m_yRot -= 360;
	}
	else if(m_yRot <= -360)
	{
		m_yRot += 360;
	}
	if(m_zRot >= 360)
	{
		m_zRot -= 360;
	}
	else if(m_zRot <= -360)
	{
		m_zRot += 360;
	}
    updateGL();
}

void GLWidget::setClearColor(const QColor& color)
{
	m_clearColor = color;
    updateGL();
}

void GLWidget::setSelectedFlag(bool flag)
{
	if (m_bSelectedFlag != flag)
	{
		m_bSelectedFlag = flag;
		makeCurrent();
		updateGL();
	}
}

void GLWidget::setViewMode(GLViewMode viewMode)
{
	m_viewMode = viewMode;
	if (m_pGround)
	{
		m_pGround->setViewMode(viewMode);
	}
	switch(viewMode)
	{
	case PERSPECTIVE:
		m_strViewMode = "Persp";
		break;
	case ORTHO_FRONT:
		m_strViewMode = "Front";
		break;
	case ORTHO_BACK:
		m_strViewMode = "Back";
		break;
	case ORTHO_LEFT:
		m_strViewMode = "Left";
		break;
	case ORTHO_RIGHT:
		m_strViewMode = "Right";
		break;
	case ORTHO_TOP:
		m_strViewMode = "Top";
		break;
	case ORTHO_BOTTOM:
		m_strViewMode = "Bottom";
		break;
	}

	changeViewMode(width(),height());
}

void GLWidget::setParent(SceneWindow* pParent)
{
	m_pParent = pParent;
}

GLWidgetMode GLWidget::getGLWidgetMode() const
{
	return m_GLWidgetMode;
}

void GLWidget::setGLWidgetMode(GLWidgetMode mode)
{
	if(m_GLWidgetMode == mode){
		return ;
	}
	m_GLWidgetMode = mode;
	createScene();
}

AbstractSceneManager* GLWidget::getSceneManager() const
{
	return m_pSceneManager;
}

//void GLWidget::updateGL()
//{
//	QGLWidget::updateGL();
//}

void GLWidget::initializeGL()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	qglClearColor(m_clearColor);

	//glClearColor(0.63f,0.63f,0.63f,1.0);
	GLfloat lightAmbient[] = {0.3f,0.3f,0.3f,1.0f};
	GLfloat lightDiffuse[] = {0.7f,0.7f,0.7f,1.0f};
	GLfloat lightPosition[] = {0.0f,0.0f,1.0f,0.0f};


	// Light values and coordinates
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glEnable(GL_CULL_FACE);		// Do not calculate insides
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);

	//Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	//Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glShadeModel(GL_SMOOTH);

//	glEnable(GL_TEXTURE_2D);

	if (m_GLWidgetMode == CommonMode)
	{
		//SwiftModel::instance().initializeFbxSDK();
	}
}

void GLWidget::createScene()
{
	SceneManagerFactory::recycle(m_pSceneManager);
	m_pSceneManager = SceneManagerFactory::createSceneManager(m_GLWidgetMode);
	if(NULL == m_pSceneManager){
		return ;
	}
	m_pSceneManager ->createRootSceneNode();
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat fEyeX = (m_fEyeX - m_fCenterX) * m_fScale + m_fCenterX;
	GLfloat fEyeY = (m_fEyeY - m_fCenterY) * m_fScale + m_fCenterY;
	GLfloat fEyeZ = (m_fEyeZ - m_fCenterZ) * m_fScale + m_fCenterZ;

	glTranslatef(m_xTrans,m_yTrans,m_zTrans);
	switch(m_viewMode)
	{
	case PERSPECTIVE:
		gluLookAt(fEyeX, fEyeY, fEyeZ, m_fCenterX, m_fCenterY, m_fCenterZ,m_fUpX,m_fUpY,m_fUpZ);
		glRotatef(m_xRot, 1.0, 0.0, 0.0);
		glRotatef(m_yRot, 0.0, 1.0, 0.0);
		glRotatef(m_zRot, 0.0, 0.0, 1.0);
		break;
	case ORTHO_FRONT:
		gluLookAt(0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);
		break;
	case ORTHO_BACK:
		gluLookAt(0.0,0.0,-1.0,0.0,0.0,0.0,0.0,1.0,0.0);
		break;
	case ORTHO_RIGHT:
		gluLookAt(1.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0);
		break;
	case ORTHO_LEFT:
		gluLookAt(-1.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0);
		break;
	case ORTHO_TOP:
		gluLookAt(0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,-1.0);
		break;
	case ORTHO_BOTTOM:
		gluLookAt(0.0,-1.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0);
		break;
	default:
		break;
	}

	/*if (m_GLWidgetMode == CommonMode)
	{
		SwiftModel::instance().drawScene();
	}*/
	m_pSceneManager->getRenderQueue()->renderAll();
	m_pSceneManager->renderBones();
	m_pGround->render();

	drawAxes();

	if (m_bShowNameRect)
	{
		Color color;
		color.r = 1.0;
		color.g = 0.5;
		color.b = 0.05;
		color.a = 1.0;
		drawMyRect(m_rNameRect,color);
	}
	drawText(15,height()-20,m_strViewMode);

	glFlush();
}

void GLWidget::renderSelected()
{
}

void GLWidget::resizeGL(int width,int height)
{
	m_rNameRect = QRect(NameRectLeft,height-NameRectTop-NameRectHeight,NameRectWidth,NameRectHeight);

	switch(m_viewMode)
	{
	case PERSPECTIVE: 
		setPerspective(width,height);
		break;
	case ORTHO_FRONT:
	case ORTHO_BACK: 
	case ORTHO_RIGHT: 
	case ORTHO_LEFT: 
	case ORTHO_TOP:
	case ORTHO_BOTTOM: 
		setOrthographic(width,height);
		break;
	}
	updateGL();
}

void GLWidget::drawAxes()
{
	//save viewport parameter
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);

	//set viewport for axes' rendering 
	glViewport(0,0,100,100);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPerspective(60.0f,1.0f,1.0f,5000.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if(m_viewMode == PERSPECTIVE) //透视投影
	{
		glLoadIdentity();
		gluLookAt(m_fEyeX, m_fEyeY, m_fEyeZ, m_fCenterX, m_fCenterY, m_fCenterZ,m_fUpX,m_fUpY,m_fUpZ);
		glRotatef(m_xRot, 1.0,0.0,0.0);
		glRotatef(m_yRot, 0.0,1.0,0.0);
	}

	else //正交投影
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		GLfloat fLength = AxisLength+AxisArrowHeight;
		glOrtho(-fLength,fLength,-fLength,fLength,-fLength,fLength);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		switch(m_viewMode)
		{
		case ORTHO_FRONT:
			gluLookAt(0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0);
			break;
		case ORTHO_BACK:
			gluLookAt(0.0,0.0,-1.0,0.0,0.0,0.0,0.0,1.0,0.0);
			break;
		case ORTHO_LEFT:
			gluLookAt(-1.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0);
			break;
		case ORTHO_RIGHT:
			gluLookAt(1.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0);
			break;
		case ORTHO_TOP:
			gluLookAt(0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,-1.0);
			break;
		case ORTHO_BOTTOM:
			gluLookAt(0.0,-1.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0);
			break;
		}
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glLineWidth(1.0f);
	glBegin(GL_LINES);
		//x axis
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(AxisLength,0.0f,0.0f);
		//y axis
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,AxisLength,0.0f);
		//z axis
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,AxisLength);
	glEnd();

	GLubyte bitX[12] = {0x00,0xe7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xe7};
	GLubyte bitY[12] = {0x00,0x38,0x10,0x10,0x10,0x10,0x10,0x28,0x28,0x44,0x44,0xee};
	GLubyte bitZ[12] = {0x00,0xfc,0x42,0x42,0x20,0x20,0x10,0x08,0x08,0x04,0x84,0x7e};

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glColor3f(1.0f,0.0f,0.0f);
	glRasterPos3f(AxisLength+AxisArrowRadius+15.0f,10.0,0.0);
	glBitmap(8,12,0.0f,0.0f,10.0,15.0,bitX);

	glColor3f(0.0f,1.0f,0.0f);
	glRasterPos3f(0.0,AxisLength+AxisArrowRadius+15.0f,0.0);
	glBitmap(8,12,0.0f,0.0f,10.0,15.0,bitY);

	glColor3f(0.0f,0.0f,1.0f);
	glRasterPos3f(0.0,10.0,AxisLength+AxisArrowRadius+15.0f);
	glBitmap(8,12,0.0f,0.0f,10.0,15.0,bitZ);

	//z axis arrow
	glTranslatef(0.0f,0.0f,AxisLength);
	auxSolidCone(AxisArrowRadius,AxisArrowHeight);

	//y axis arrow
	glColor3f(0.0f,1.0f,0.0f);
	glTranslatef(0.0f,AxisLength,-AxisLength);
	glRotatef(-90.0f,1.0f,0.0f,0.0f);
	auxSolidCone(AxisArrowRadius,AxisArrowHeight);

	//x axis arrow
	glColor3f(1.0f,0.0f,0.0f);
	glTranslatef(AxisLength,0.0f,-AxisLength);
	glRotatef(90.0f,0.0f,1.0f,0.0f);
	auxSolidCone(AxisArrowRadius,AxisArrowHeight);
	

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	//restore viewport
	glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GLWidget::drawText(int x,int y, std::string str)
{
	glDisable(GL_LIGHTING);

	//save viewport parameter
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);

	//save projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	//save modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();


	//set viewport for text rendering 
	glViewport(0,0,width(),height());

	//set projection matrix for text rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width(),0,height(),-1.0,1.0);

	//set modelview matrix for text rendering
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//set text color
	glColor3f(1.0,1.0,1.0);

	//text position
	glRasterPos2i(x,y);

	for(unsigned int i = 0; i < str.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	}

	//restore viewport
	glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

	glEnable(GL_LIGHTING);

	//restore projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	//restore modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GLWidget::drawMyRect(QRect rect,Color color)
{
	//save viewport parameter
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);

	//save projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	//save modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//set viewport for text rendering 
	glViewport(0,0,width(),height());

	//set projection matrix for text rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width(),0,height(),-1.0,1.0);

	//set modelview matrix for text rendering
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//set text color
	//glColor3f(1.0,0.0,0.0);
	glColor3f(color.r,color.g,color.b);

	glLineWidth(1.2f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(rect.left(),rect.top());
	glVertex2f(rect.right(),rect.top());
	glVertex2f(rect.right(),rect.bottom());
	glVertex2f(rect.left(),rect.bottom());
	glEnd();

	//restore viewport
	glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

	//restore projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	//restore modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GLWidget::drawPickRect()
{
	//save viewport parameter
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);

	//save projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	//save modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//set viewport for text rendering 
	glViewport(0,0,width(),height());

	//set projection matrix for text rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,width(),0,height(),-1.0,1.0);

	//set modelview matrix for text rendering
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0,1.0,1.0);
	glLineWidth(1.2f);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(3,0xAAAA);
	glBegin(GL_LINES);
		glVertex2f(m_rPickRect.left(),height()-m_rPickRect.top());
		glVertex2f(m_rPickRect.right(),height()-m_rPickRect.top());
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(m_rPickRect.right(),height()-m_rPickRect.top());
		glVertex2f(m_rPickRect.right(),height()-m_rPickRect.bottom());
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(m_rPickRect.left(),height()-m_rPickRect.top());
		glVertex2f(m_rPickRect.left(),height()-m_rPickRect.bottom());
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(m_rPickRect.left(),height()-m_rPickRect.bottom());
		glVertex2f(m_rPickRect.right(),height()-m_rPickRect.bottom());
	glEnd();

	glDisable(GL_LINE_STIPPLE);

	//restore viewport
	glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

	//restore projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	//restore modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GLWidget::hideEvent ( QHideEvent *event)
{
	emit hidden();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	//Send clicking signal.
	emit clicked();  
	//Get the clicking position.
	m_startPos = m_endPos = m_lastPos = event->pos();

	//Set the flag of the name rect which indicates whether the rect should be shown..
	if (m_bShowNameRect && !m_bFillNameRect)
	{
		m_bFillNameRect = true;
		repaint(m_rNameRect);
	}
}

void GLWidget::mouseDoubleClickEvent ( QMouseEvent * event )
{
	if(isFullScreen())
	{
		hide();
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    m_lastPos = event->pos();

	if (event->x()>NameRectLeft && event->x()<NameRectLeft+NameRectWidth && event->y()>NameRectTop && event->y()<NameRectTop+NameRectHeight)
	{
		if (!m_bShowNameRect)
		{
			m_bShowNameRect = true;
			makeCurrent();
			//updateGL();
			repaint(m_rNameRect);
		}
	}
	else 
	{	
		if(m_bShowNameRect)
		{
			m_bShowNameRect = false;
			m_bFillNameRect = false;
			makeCurrent();
			//updateGL();
			repaint(m_rNameRect);
		}
	}

	if (event->modifiers() == Qt::AltModifier)
	{
		processMouseMove(event->buttons(), dx, dy);
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_bShowNameRect)
	{
		if (event->button() == Qt::LeftButton)
		{
			changeToNextViewMode();
		}
		m_bFillNameRect = false;
		repaint(m_rNameRect);
	}

	if(NULL != m_pParent)
	{
		m_pParent->updateAll();
	}

	QGLWidget::mouseReleaseEvent(event);
}

void GLWidget::leaveEvent(QEvent * event)
{
	m_bShowNameRect = false;
	repaint(m_rNameRect);

	QGLWidget::leaveEvent(event);
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{
	if(isFullScreen())
	{
		switch(event->key())
		{
		case Qt::Key_Escape:
			if(isFullScreen())
			{
				hide();
			}
			break;
		case Qt::Key_Space:
			if(m_bPlaying)
				emit pause();
			else
				emit play();
			break;
		case Qt::Key_Left:
			emit backPlay();
			break;
		case Qt::Key_Right:
			emit fastPlay();
			break;
		case Qt::Key_Up:
			emit playPreviese();
			break;
		case Qt::Key_Down:
			emit playNext();
			break;
		}
	}

	QWidget::keyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent * event)
{
	this->updateGL();
	if(m_pParent)
	{
		m_pParent->setFocus(Qt::ActiveWindowFocusReason);
	}
}

void GLWidget::setPerspective(GLsizei width,GLsizei height)
{
	glViewport(0,0,width,height);
	if (height == 0)
	{
		height = 1;
	}
	GLfloat fAspect = (GLfloat)width/(GLfloat)height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f,fAspect,1.0f,5000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::setOrthographic(GLsizei width,GLsizei height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (height == 0)
	{
		height = 1;
	}
	GLfloat fAspect = (GLfloat)width/(GLfloat)height;
	glOrtho(-OrthoSize*fAspect*m_fScale,OrthoSize*fAspect*m_fScale,-OrthoSize*m_fScale,OrthoSize*m_fScale,-OrthoSize,OrthoSize);
	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::processMouseMove(Qt::MouseButtons buttons, int dx, int dy)
{
	if (buttons & Qt::LeftButton && m_viewMode == PERSPECTIVE) 
	{
		double xAngle = dy * RotateStep;
		double yAngle = dx * RotateStep;
		//double zAngle = dz * m_fRotateFactor;
		m_xRot += xAngle;
		m_yRot += yAngle;
		//m_zRot += zAngle;
		if(m_xRot >= 360)
		{
			m_xRot -= 360;
		}
		else if(m_xRot <= -360)
		{
			m_xRot += 360;
		}
		if(m_yRot >= 360)
		{
			m_yRot -= 360;
		}
		else if(m_yRot <= -360)
		{
			m_yRot += 360;
		}
		//updateGL();
	}
	else if(buttons & Qt::MidButton)
	{
		m_xTrans += dx * TransStep * m_fScale;
		m_yTrans -= dy * TransStep * m_fScale;
	}
	else if(buttons & Qt::RightButton)
	{
		m_fScale *= (1 - (dy+dx) * ScaleStep);
		if(m_fScale < MinScale)
			m_fScale = MinScale;
		if (m_fScale > MaxScale)
			m_fScale = MaxScale;
	}

	makeCurrent();
	if(m_viewMode >= ORTHO_FRONT && m_viewMode <= ORTHO_BOTTOM)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		GLdouble d = OrthoSize * m_fScale;
		GLfloat fAspect = (GLfloat)width()/(GLfloat)height();
		glOrtho(-d*fAspect,d*fAspect,-d,d,-OrthoSize,OrthoSize);
	}

	updateGL();
}

void GLWidget::changeViewMode(int nWidth,int nHeight)
{
	switch(m_viewMode)
	{
	case PERSPECTIVE:
		setPerspective(nWidth,nHeight);
		break;
	case ORTHO_FRONT:
	case ORTHO_BACK:
	case ORTHO_RIGHT:
	case ORTHO_LEFT:
	case ORTHO_TOP:
	case ORTHO_BOTTOM:
		setOrthographic(nWidth,nHeight);
		break;
	default:
		break;
	}
	updateGL();
}

void GLWidget::changeToNextViewMode()
{
	makeCurrent();
	if (m_viewMode == 6)
	{
		m_viewMode = static_cast<GLViewMode>(0);
	}
	else
	{
		m_viewMode = static_cast<GLViewMode>(m_viewMode+1);
	}

	setViewMode(m_viewMode);

	changeViewMode(width(),height());
}

int GLWidget::processSelection(int xPos, int yPos)
{
	return 0;
}

void GLWidget::markSelection(int nHits, GLuint selectBuffer[])
{
}

void GLWidget::markRectSelection()
{
}


bool GLWidget::pointInRegion(QPoint point,QRect rect)
{
	if (point.x()>rect.left() && point.x()<rect.right() && point.y()>rect.bottom() && point.y()<rect.top())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//FBXSceneViewer* GLWidget::getScene() const
//{
//	return m_FBXScene;
//}

void GLWidget::LoadFbxModel(const char* pFilename)
{
	this->updateGL();
	//m_FBXScene->LoadScene(pFilename);
}

void GLWidget::copyInfo(GLWidget *source)
{
	if(source)
	{
		m_clearColor = source->m_clearColor;

		//rotation and tranlation
		m_xRot = source->m_xRot;
		m_yRot = source->m_yRot;
		m_zRot = source->m_zRot;
		m_xTrans = source->m_xTrans;
		m_yTrans = source->m_yTrans;
		m_zTrans = source->m_zTrans;

		m_fScale = source->m_fScale;

		//camera
		m_fEyeX = source->m_fEyeX;
		m_fScale = source->m_fScale;
		m_fEyeY = source->m_fEyeY;
		m_fEyeZ = source->m_fEyeZ;
		m_fCenterX = source->m_fCenterX;
		m_fCenterY = source->m_fCenterY;
		m_fCenterZ = source->m_fCenterZ;
		m_fUpX = source->m_fUpX;
		m_fUpY = source->m_fUpY;
		m_fUpZ = source->m_fUpZ;


		//m_viewMode = source->m_viewMode;
		m_bSelectedFlag = source->m_bSelectedFlag;
		m_bPickedRectFlag = source->m_bPickedRectFlag;
		//m_strViewMode = source->m_strViewMode;
		m_bSelect = source->m_bSelect;
		m_currentFrame = source->m_currentFrame;
		m_bPlaying = source->m_bPlaying;

//		m_FBXScene = source->m_FBXScene;
//		if(source->m_FBXScene->GetScene())
//		{
//			m_FBXScene->LoadScene(source->m_FBXScene->GetModelFileName());
//		}
	}
}

void GLWidget::setCurrentFrame(int nFrame)
{
	if(nFrame < m_frameCount)
		m_currentFrame = nFrame;
}

void GLWidget::setFrameCount(int nFrame)
{
	m_frameCount = nFrame;
}

void GLWidget::setPlayState(bool p)
{
	m_bPlaying = p;
}

void GLWidget::changeCurrentFrame(int nCount)
{
	if(m_currentFrame + nCount < m_frameCount)
		m_currentFrame += nCount;
}

