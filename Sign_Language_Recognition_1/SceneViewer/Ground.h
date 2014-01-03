/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2007/09/20                                                  */
/************************************************************************/

#pragma once

#ifndef _Ground_H_
#define _Ground_H_
#define GLUT_DISABLE_ATEXIT_HACK
#include <string>
#include <gl/glut.h>
#include "TypeDef.h"

namespace Swift
{
	/// Class representing the flat ground.
	/**
	* @note This class also inherits from the SceneObject class.
	* And it is usually attached to the root scene node.
	*/
	class Ground
	{
	public:
		///Constructor.
		explicit Ground();
		///~Destructor.
		~Ground();

		/// Render the ground.
		void render();

		/// Get the current view mode.
		GLViewMode getViewMode();
		/// Set the current view mode.
		void setViewMode(GLViewMode viewMode);

	private:
		void drawGrid();
		void drawTextPerspective();
		void drawTextOrthoFront();
		void drawTextOrthoBack();
		void drawTextOrthoLeft();
		void drawTextOrthoRight();
		void drawTextOrthoTop();
		void drawTextOrthoBottom();

	private:
		/// Grid lines of the flat ground.
		GLfloat m_gridLines[51][51][3]; 
		/// current view mode.
		GLViewMode m_viewMode;
		std::string m_strHorizontal;
		int m_nHorizontalSign;
		std::string m_strVertical;
		int m_nVertitalSign;
	};
}	//Swift

#endif	//_Ground_H_

