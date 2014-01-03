/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/08/15                                                  */
/* Last Modified: 2007/11/25                                            */
/************************************************************************/


#ifndef _SCENE_MANAGER_H__
#define _SCENE_MANAGER_H__
/*
#include <vector>
#include <map>
#include "../../Util/Singleton.h"
#include "IDManager.h"
#include "TraceTable.h"
#include "SceneNode.h"
#include "Skeleton.h"
#include "RenderQueue.h"
#include "Ground.h"
#include "../../PropertyDialog/src/propdlgmanager.h"
#include "../../MotionRetrieval/include/MotionIndex.h"
*/
#include "abstractSceneManager.h"

namespace Swift
{
	class MotionClip ;

	/// This class is used to manage the scene graph of the main window.
	/**
	 * The Scene Manager holds all the resources at runtime 
	 * and manages the structure and life cycle of these resources. 
	 * It also supports the serialization of the resources and structure.
	 * @remark
	 * This class is of singleton pattern, you can get singleton instance
	 * by SceneManager::instance();
	 * @see Singleton.
	*/
	//class SceneManager : public QObject,public Singleton<SceneManager>
	class SceneManager : public AbstractSceneManager, public Singleton<SceneManager>
	{
		Q_OBJECT

	public:
		SceneManager();

		virtual ~SceneManager();


	}; // SceneManager

	class PreviewSceneManager : public AbstractSceneManager
	{
	public:
		PreviewSceneManager();

		~PreviewSceneManager();

	public:
		/// Create the skeleton of a humanoid for the preview dialog.
		/**
		* @param pMotionClip The pointer to the new motion clip.
		* @param bDelOldMotionClip Indicates whether needing to delete the old motion clip.
		* @param sot The type of object attached to each joint.
		* @return The root node of the skeleton.
		*/
		BoneNode* createSkeleton(MotionClip* pMotionClip, bool bDelOldMotionClip = true, SimpleObjectType sot=SOT_SPHERE);

		/// Update the bone nodes' data from a motion clip for the recognition dialog. 
		/**
		* @param nFrameNo The current frame number.
		*/
		void updateDataFromMotionClip(int nFrameNo);

		/// Render the bones of every skeleton in the scene for the recognition dialog.
		/**
		* The bone between two joints is just drawn as a simple line.
		* This function will call the recursive function renderBones(BoneNode* pBoneNode).
		*/
		void renderBones();

	public:
		/// The skeleton in the preview dialog.
		Skeleton* m_pSkeleton;

	}; //PreviewSceneManager

}// Swift


#endif // _SCENE_MANAGER_H__
