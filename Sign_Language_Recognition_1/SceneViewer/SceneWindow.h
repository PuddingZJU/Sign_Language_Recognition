/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2010/01/28                                                  */
/************************************************************************/

#pragma once

#ifndef _SCENEWINDOW_H_
#define _SCENEWINDOW_H_

#include <QtGui>
#include "GLWidget.h"
#pragma comment( lib, "glaux.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "OpenGL32.lib" )
//class GLWidget;
class QGridLayout;

namespace Swift
{
	/// The main working area in the main window.
	/**
	* The window can be partitioned into one, two, three or four sub grids.
	* One grid's layout:                
	*                  *********** 
	*                  *		 *
	*                  *		 *
	*                  *		 *
	*				   ***********
	* Two grids' layout:                
	*                  *********** 
	*                  *	*	 *
	*                  *	*	 *
	*                  *	*	 *
	*				   ***********
	* Three grids' layout:                
	*                  *********** 
	*                  *	*	 *
	*                  *	******
	*                  *	*	 *
	*				   ***********
	* Four grids' layout:              and   
	*                  *************           ***************
	*                  *	 *	   *           *	  *      *
	*                  *     *     *           *      ********
	*                  *************           *	  *	     *  
	*				   *     *     *           *      ********  
	*                  *	 *	   *           *	  *	     *  
	*				   *************           ***************  
	*
	*/
	class SceneWindow : public QWidget
	{
		Q_OBJECT

	public:
		/// Constructor.
		SceneWindow(QWidget *parent = 0);
		/// Destructor.
		~SceneWindow();

		/// Change the scene window into one grid.
		void changeToOneGrid();

		/// Change the scene window into two grids.
		void changeToTwoGrid();

		/// Change the scene window into three grids.
		void changeToThreeGrid();

		/// Change the scene window into four grids.
		void changeToFourGrid1();

		/// Change the scene window into another four grids.
		void changeToFourGrid2();

		/// Set the flag of the widget to true.
		/** 
		* @param pGLWidget The widget to be set as the current. 
		*/
		void setCurrentGLWidget(GLWidget* pGLWidget);



	public slots:
		/// Update all the child widgets.
		void updateAll();

	private slots:
		/// Slot to process the click event.
		void clickGlWidget();

	public slots:
		/// Slot to process the set current frame no. signal.
		void setCurrentFrame(int nFrameNo);
			
	protected:
		///  Processing function for dealing key press event.
		void keyPressEvent (QKeyEvent * event);

	private:
		/// Enum defining the row and column of the grids.
		enum { GLWidgetCount = 4 };

		/// Array stores the all of the OpenGL widgets.
		GLWidget *m_glWidgets[GLWidgetCount];
		/// The current selected GL widget.
		GLWidget *m_pCurrentGlWidget;
		/// The main QGridLayout to hold the widgets and sub layouts.
		QGridLayout* m_pMainGridLayout;
		/// The sub layout to hold widgets.
		QGridLayout* m_pSubGridLayout;
		/// The number of the widgets.
		int m_nWidgetCount;

	//private:
		//Ui::SceneWindowClass ui;
	};
}

#endif // SCENEWINDOW_H
