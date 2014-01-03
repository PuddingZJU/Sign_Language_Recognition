/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/09/03                                                  */
/* Last Modified: 2007/09/06                                            */
/************************************************************************/

#ifndef _Skeleton_H_
#define _Skeleton_H_

#include "BoneNode.h"
#include "../../MotionData/MotionClip.h"
#include "SimpleObject.h"
#include "SceneManager.h"

namespace Swift
{
	class Mesh;
	class FBXMesh;

	/// This class is developed to create BoneNodes of a skeleton.
	/**
	 * Since the skeleton of a humanoid is broken to joints and attached
	 * nodes(BoneNode inherits from Node) in the scene, we need a class 
	 * to create the BoneNodes and combine them to a whole object, we also
	 * use this to bound mesh to the skeleton.
	 * @see BoneNode.
	*/
	class Skeleton
	{
	public:
		/// Constructor.
		explicit Skeleton(AbstractSceneManager * p_mgr = &SceneManager::instance());

		/// Another Constructor.
		/** 
		* @param bDelMotionClip If bDelMotionClip is true, the related motion clip will be delete when destruct the skeleton.
		*/
		explicit Skeleton(MotionClip* pMotionClip, bool bDelMotionClip=true, AbstractSceneManager * p_mgr = &SceneManager::instance());

		/// Destructor.
		~Skeleton();

		/// Create the BoneNodes which buildup the skeleton by a MotionClip.
		/**
		 * By walking through the skeleton of the MotionClip,
		 * this function create the BoneNodes in the scene.
		 * @return The root node of the skeleton.
		 * @see MotionClip.
		*/
		BoneNode* createSkeleton();

		BoneNode* createSkeletonRecursively(MotionJoint* pRoot);

		/// Return the MotionClip associated to the skeleton.
		MotionClip* getMotionClip();

		/// Change the associated MotionClip.
		void setMotionClip(MotionClip* pMotionClip);

		/// Attach joint objects to the node in the skeleton.
		/** 
		* The joint objects may be small balls or other simple objects.
		*/
		void attachJointObjects(SimpleObjectType sot = SOT_SPHERE);

		/// Update the data of every joint from the motion clip according to the specified frame no.
		void updateDataFromMotionClip(int nFrameNo);

		/// Return the root node of the skeleton.
		BoneNode* getRootBoneNode() const;

		/// Set picking state when mouse selecting on the tree view.
		void setSelectedJoint(const QString& sJointName);

		/// Set the view state of the skeleton.
		void setViewState(MFileState state);

		/// Return the view state of the skeleton.
		MFileState getViewState() const;

		/// Set the visible state of all the joints of the skeleton.
		void setJointsVisiable(bool bVisible);

		/// Get the type of the skeleton
		/** 
		* @return 0 for xm template skeleton, 1 for motion skeleton, 2 for xm-captured motion skeleton.
		*/
		int getSkeletonType();

		/// Set the type of the skeleton, 0 for xm template skeleton, 1 for motion skeleton, 2 for xm-captured motion skeleton.
		/** 
		* @param nType 0 for xm template skeleton, 1 for motion skeleton, 2 for xm-captured motion skeleton.
		*/
		void setSkeletonType(int nType);

		/// Set whether to delete the related motion clip when being destructed
		void setDelMotionClip(bool to_del);

	private:
		/// Pointer to the MotionClip.
		MotionClip* m_pMotionClip;

		/// The root node of a skeleton.
		BoneNode* m_pRootBoneNode;

		/// Indicate whether deleting the related motion clip when destruct the skeleton.
		bool m_bDelMotionClip;

		///// Store the name-pointer pair of the BoneNodes.
		//std::map<std::string, BoneNode*> m_mBoneNodeSet;

		//typedef std::map<std::string, BoneNode*> BoneNodeMap;

		/// Store the index-pointer pair of the BoneNodes.
		std::map<int, BoneNode*> m_mBoneNodeSet;

		typedef std::map<int, BoneNode*> BoneNodeMap;

		/// Store the state of the motion which can be Invisiable, Visiable and Active.
		MFileState m_state;


		/// Store the type of the skeleton, 0 for standard skeleton used by XMDevice, and 1 for skeleton used in bvh.
		int m_nType;

		/// the scene manager
		AbstractSceneManager * m_pSceneManager;

	public:
		std::map<int, BoneNode*> GetNodeMap();

	};	//Skeleton
}

#endif	//Swift