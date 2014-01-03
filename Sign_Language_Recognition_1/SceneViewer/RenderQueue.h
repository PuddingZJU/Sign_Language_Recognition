/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/09/13                                                  */
/* Last Modified: 2007/11/25                                            */
/************************************************************************/

#ifndef _RenderQueue_H_
#define _RenderQueue_H_

#include <vector>
#include <map>
#include "TypeDef.h"

namespace Swift
{
	class SceneObject;


	/// Class which holds the render group and the object of every group.
	/**
	* @remark Rendering different kind of scene objects need to change rendering state,
	* which is time-consuming job. Before rendering, we should sort the scene objects
	* and put them in the render queue, such that the same kind of objects are rendered
	* together without changing state.
	*/
	class RenderQueue
	{
	public:
		/// Constructor.
		RenderQueue();
		/// Destructor.
		~RenderQueue();

		/// Add a scene object to a specified group.
		/**
		* @param id The specified render group.
		* @param pSceneObject The pointer to the scene object.
		*/
		void addSceneObject(SceneObject* pSceneObject,RenderGroupID id = RENDER_QUEUE_MAIN);

		/// Delete a scene object from the render queue.
		/**
		* @param id The specified render group.
		* @param pSceneObject The pointer to the scene object.
		*/
		void deleteSceneObject(SceneObject* pSceneObject,RenderGroupID id = RENDER_QUEUE_UNKNOWN);

		/// Render all scene objects.
		void renderAll();

		/// Render selected objects to select buffer.
		void renderSelected();


	public:
		typedef std::map<RenderGroupID,std::vector<SceneObject*> > RenderGroupMap;
		/// The map which stores render group and the pointers to the object of this group.
		RenderGroupMap m_mRenderGroup;

	};	//RenderQueue
}	//Swift

#endif	//_RenderQueue_H_