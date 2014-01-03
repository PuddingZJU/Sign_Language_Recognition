/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/01/28                                                  */
/************************************************************************/

#pragma once

#ifndef _GLWIDGET_H_
#define _GLWIDGET_H_

#include <QtOpenGL/QGLWidget>
#include <string>
//#include "Texture.h"
#include "TypeDef.h"
#include "SceneManager.h"

namespace Swift
{
	class Ground;
	class FBXSceneViewer;
	class SceneWindow;

	/// This enum type gives the mode of the GLWidget.
	/* 
	* CommonMode for the common GLWidget, the GLWidgets in the main window.
	* PreviewMode for the retrieval dialog.
	* RecongnitionMode for the recognition dialog.
	*/
	typedef enum
	{
		GLWidgetMode_START = 0,
		// singleton
		CommonMode = GLWidgetMode_START,
		RetrievalMode,
		GLWidgetMode_SINGLETON_LINE,
		// not a singleton
		PreviewMode,
		ExagerationMode,
		DraftRetrievalMode,

		GLWidgetMode_END,
		GLWidgetMode_CNT = GLWidgetMode_END,
	}GLWidgetMode;

	/// list of modes which can do selection of nodes (end with -1)
	// the list is defined in GLWidget.cpp:41
	extern const GLWidgetMode GLWidgetModes_Selection[];

	/// This class creates SceneManagers for a GLWidget in different Mode
	class SceneManagerFactory
	{
		SceneManagerFactory();
	public:
		static AbstractSceneManager * 
			createSceneManager(GLWidgetMode mode = CommonMode);
		static void recycle(AbstractSceneManager * mgr);
	};

	/** This class represents an OpenGL window to displace the scene.
	 * It inherits from the QGLWidget which encapsulates the OpenGL library.
	 */
	class GLWidget : public QGLWidget
	{
		Q_OBJECT

	public:
		GLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0, GLWidgetMode mode = CommonMode);
		~GLWidget();

		//rotate the scene by eular angle.
		void rotateByEularAngle(GLfloat xAngle,GLfloat yAngle,GLfloat zAngle);

		//set the openGL clear color.
		void setClearColor(const QColor& color);
		
		/**set the select flag;
		 * if flag is not equal to the current flag,
		 * make the pixelbuffer be current OpenGL rendering context and
		 * update the openGL.
		 */
		void setSelectedFlag(bool flag);

		//set the view mode to viewMode.
		void setViewMode(GLViewMode viewMode);

		/// Set the parent window of the scene window.
		void setParent(SceneWindow* pParent);

		/// Return the GLWidgetMode of the GLWidget.
		GLWidgetMode getGLWidgetMode() const;

		/// Set the GLWidgetMode of the GLWidget.
		void setGLWidgetMode(GLWidgetMode mode);

		/// Get the pointer to the scene manager.
		AbstractSceneManager* getSceneManager() const;

	signals:
		//mouse click signal.
		void clicked();
		void hidden();

		//play control.
		void play();
		void pause();
		void fastPlay();
		void backPlay();
		void playNext();
		void playPreviese();
//	public slots:
//		void updateGL();

	protected:
		// Initialize the OpenGL.
		void initializeGL();

		/// Create the initial nodes and objects in the scene.
		void createScene();

		// Render the OpenGL scene.
		void paintGL();

		// Render the selected objects to the OpenGL select buffer.
		void renderSelected();

		// Processing function for dealing window resizing event.
		void resizeGL(int width, int height);

		// Render the x, y, and z axes.
		void drawAxes();

		/**Render a string.
		 * @param x The x-position of the string.
		 * @param y The y-position of the string.
		 * @param str The string needed to be rendered.
		 */
		void drawText(int x,int y, std::string str);

		/**Draw the outer specified rectangle.
		 * @param rect The rectangle to draw.
		 * @param color The color of the rectangle's edge.
		 */
		void drawMyRect(QRect rect,Color color);

		// Draw the picking rectangle.
		void drawPickRect();

		/**Draw the outer specified filled rectangle.
		 * @param rect The rectangle to draw.
		 * @param color The color of the rectangle.
		 */
		//void drawFilledRect(QRect rect,Color color);

		//sent hidden singal when hidden.
		void hideEvent(QHideEvent *event);

		// Processing function for dealing mouse press event.
		void mousePressEvent(QMouseEvent *event);

		//Dealing with mouse double click event.
		void mouseDoubleClickEvent(QMouseEvent *event);

		// Processing function for dealing mouse move event.
		void mouseMoveEvent(QMouseEvent *event);

		// Processing function for dealing mouse release event.
		void mouseReleaseEvent(QMouseEvent *event);

		// Processing function for dealing mouse leave event.
		void leaveEvent(QEvent * event);

		// Processing the paint event.
		//void paintEvent(QPaintEvent* event);

		// Processing the key press event
		void keyPressEvent(QKeyEvent * event);

		// Processing the key release event
		void keyReleaseEvent(QKeyEvent * event);

		/**Set perspective projection of the scene.
		 * @param width The width of the viewport.
		 * @param height The height of the viewport.
		 */
		void setPerspective(GLsizei width,GLsizei height);

		/**Set orthographic projection of the scene.
		 * @param width The width of the viewport.
		 * @param height The height of the viewport.
		 */
		void setOrthographic(GLsizei width,GLsizei height);

		/**Process the mouse move event .
		 * @param buttons Indicate which mouse button is clicked.
		 * @param dx The x-position difference between the last and the last but one mouse clicking position.
		 * @param dy The y-position difference between the last and the last but one mouse clicking position.
		 */
		void processMouseMove(Qt::MouseButtons buttons, int dx, int dy);

		/**Change the projection mode of the scene window.
		 * @param nWidth The width of the new secne window.
		 * @param nHeight The height of the new scene window.
		 */
		void changeViewMode(int nWidth,int nHeight);

		// Change the next projection mode of the scene window.
		void changeToNextViewMode();

		/**Processing the mouse-picking of the scene objects.
		 * @param xPos The x-position of the clicking point.
		 * @param yPos The y-position of the clicking point.
		 */
		int processSelection(int xPos, int yPos);

		/**Marking the selected objects.
		 * @param nHits The number of objects hit by a mouse clicking.
		 * @param selectBuffer The select buffer which stores the picked objects.
		 */
		void markSelection(int nHits, GLuint selectBuffer[]);

		// Marking the selected objects through mouse drawing rectangle.
		void markRectSelection();
		
		// Indicates whether the point is in the specified region.
		bool pointInRegion(QPoint point,QRect rect);

	public:
		//FBXSceneViewer* getScene() const;
		void LoadFbxModel(const char* pFilename);
		void copyInfo(GLWidget* source);
		void setCurrentFrame(int nFrame);
		void setFrameCount(int nFrame);
		void setPlayState(bool p);
		void changeCurrentFrame(int nCount);

	private:

		// the clear Color.
		QColor m_clearColor;

		// Stores the last mouse position.
		QPoint m_lastPos;
		// The start position of the pick rectangle.
		QPoint m_startPos;
		// The end position of the pick rectangle.
		QPoint m_endPos;

		// Rotate attributes.
		GLfloat m_xRot;
		GLfloat m_yRot;
		GLfloat m_zRot;

		// Translation attributes.
		GLfloat m_xTrans;
		GLfloat m_yTrans;
		GLfloat m_zTrans;

		// Stores the scaling factor of the scene.
		GLfloat m_fScale;

		// Camera attributes.
		GLfloat m_fEyeX;
		GLfloat m_fEyeY;
		GLfloat m_fEyeZ;
		GLfloat m_fCenterX;
		GLfloat m_fCenterY;
		GLfloat m_fCenterZ;
		GLfloat m_fUpX;
		GLfloat m_fUpY;
		GLfloat m_fUpZ;

		SceneWindow* m_pParent;

		/// The mode of the GLWidgetMode.
		GLWidgetMode m_GLWidgetMode;

		Ground* m_pGround;

		// The current view mode of the widget.
		GLViewMode m_viewMode;

		// Indicates whether the widget is selected.
		bool m_bSelectedFlag;

		// Indicates whether the name rect is shown.
		bool m_bShowNameRect;
		// Indicates whether the name rect is filled with a certain color..
		bool m_bFillNameRect;
		/// The region of the view mode string.
		QRect m_rNameRect;

		/// The region of the pick rectangle.
		QRect m_rPickRect;
		/// Indicates whether the pick rect is shown.
		bool m_bPickedRectFlag;

		// The string stands the view mode
		std::string m_strViewMode;

		// current node pos in window
		//int m_dWinXn, m_dWinYn;

		// if in gl_select mode
		bool m_bSelect;

		/// Pointer to the scenemanager of the widget
		/**
		* If the mode of the widget is not CommonMode, we use this scenemanager, instead of the default singleton secnemanager.
		*/
		AbstractSceneManager* m_pSceneManager;

		// Stores the object list.
		//static GLuint sharedObject;
		// Stores the reference count of the window.
		//static int refCount;

		//FBXSceneViewer* m_FBXScene;
		int m_currentFrame;
		int m_frameCount;

		//play control.
		bool m_bPlaying;
	};	// end of class GLWidget definition.
}	//Swift

#endif	//_GLWIDGET_H_