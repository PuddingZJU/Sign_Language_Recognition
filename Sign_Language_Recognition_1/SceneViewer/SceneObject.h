/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/09/03                                                  */
/* Last Modified: 2007/11/25                                            */
/************************************************************************/

#ifndef _Renderable_H_
#define _Renderable_H_
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>
#include "Traceable.h"
#include "Serializable.h"
#include "TypeDef.h"
#include "IDManager.h"
#pragma comment( lib, "glaux.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "OpenGL32.lib" )
namespace Swift
{
	class AbstractSceneManager;
	class Node;

	/// Base class representing a scene object.
	/**
	 * SceneObject can be attached to a scene node,
	 * and also can be detached from a scene node.
	 * SceneObject should be renderable.
	 * This class should inherited from Traceable and Serializable.
	 * @remark This is a pure abstract class.
	 * You can't make an instance of this class directly.
	*/
	class SceneObject : public Traceable, public Serializable
	{
	public:
		/// Constructor.
		SceneObject(AbstractSceneManager * p_mgr, Swift::IDType type);
		/// Destructor.
		~SceneObject();
		/// register to a scene manager (removed from the old one)
		void regiterToSceneManager(AbstractSceneManager * p_mgr, Swift::IDType type);

		/// Attach this object to a node.
		/**
		 * @param pNode The node this object attached to.
		*/
		void attachToNode(Node* pNode);

		/// Detach this object from the node.
		/**
		 * @return The node this object has just detached from.
		*/
		Node* detachFromNode();

		/// Render the object to the scene.
		/**
		 * @remark
		 * Every subclass of this class should implement this abstract function.
		*/
		virtual void render() = 0;

		/// Get the render group ID of the object.
		RenderGroupID getRenderGroupID();

		/// Set the render group this object belonging to.
		void setRenderGroupID(RenderGroupID nID);

		/// Return the color of the object.
		Color getColor();

		/// Set the color of the object.
		void setColor(Color rgba);

		/// Return the picking flag of the object.
		bool isPicked();

		/// Set the picked flag of the object.
		void setPicked(bool bFlag);

		/// Return the index in the attached node.
		int getIndex();

		/// Set the index in the attached node.
		void setIndex(int vIndex);

		/// Set the visible state of the object.
		void setVisible(bool bVisible);

		/// Return the validation of the object.
		bool getValidation();

		/// Set the validation of the object.
		void setValidation(bool bValidation);
	
		/// Pointer to the node which the object will be attached to.
		Node* m_pNode;

	protected:
		/// the scene manager it belongs to
		AbstractSceneManager * m_pSceneManager;
		
		/// An integer indicates the render group this object belonging to.
		RenderGroupID m_nRenderGroupID;

		/// Indicates whether the object is visible.
		bool m_bVisible;

		/// Indicate whether scene object is picked.
		bool m_bIsPicked;

		/// Stores the color of the scene object.
		Color m_rgba;

		/// The index of the object in the attached node.
		int m_nIndex;

		/// Indicates whether the scene object is valid, that is to say the object is not deleted.
		bool m_bValidation;

	};	//SceneObject

}	//Swift

#endif	//_Renderable_H_