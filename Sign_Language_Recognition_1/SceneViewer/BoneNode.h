/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/08/15                                                  */
/* Last Modified: 2007/09/05                                            */
/************************************************************************/

#ifndef _BoneNode_H_
#define _BoneNode_H_

#include "Node.h"
#include "SceneObject.h"

namespace Swift
{
	class Skeleton;
	class MotionJoint;

	/// Class representing a bone node in the scene.
	/**
	 * The skeleton of a humanoid is broken into joints
	 * which are attached to the BoneNodes in the scene.
	 * The BoneNode is part of the scene graph tree.
	 * @remark
	 * For a ball-stick model of a humanoid, a small is 
	 * attached to a BoneNode to represent a joint, and 
	 * thin stick is drawn between BoneNodes to represent 
	 * a bone.
	*/
	class BoneNode : public Node
	{
	public:
		/// Constructor.
		BoneNode(const QString& strName, AbstractSceneManager * p_mgr);
		/// Destuctor.
		~BoneNode();

		/// Return the creator of the BoneNode.
		/**
		 * @see Skeleton.
		*/
		Skeleton* getSkeleton();

		/// Set the creator of the BoneNode.
		void setSkeleton(Skeleton* pSkeleton);

		/// Get the absolute position of the bone node.
		/**
		* @return The position vector the bone node.
		*/
		const Vector3& getAbsolutePosition() const;

		/// Set the absolute position of the bone node.
		/** 
		* @param vPosition The position vector the bone node.
		*/
		void setAbsolutePosition(Vector3 vPosition);

		/// Get the absolute orientation of the bone node.
		/**
		* @return The orientation quaternion the bone node.
		*/
		const Quaternion& getAbsoluteOrientation() const;

		/// Set the absolute orientation of the bone node.
		/** 
		* @param vPosition The orientation quaternion the bone node.
		*/
		void setAbsoluteOrientation(Quaternion qOrientation);

		/// Get the index of the bone node.
		int getIndex();

		/// Set the index of the bone node.
		void setIndex(int nIndex);

		MotionJoint* getMotionJoint() const;

		void setMotionJoint(MotionJoint* pMotionJoint);

	private:
		/// Pointer to the skeleton which create the BoneNode.
		Skeleton* m_pSkeleton;

		/// Pointer to the joint in a motionclip.
		MotionJoint* m_pMotionJoint;

		/// The index of the bone node.
		int m_nIndex;

	};	//BoneNode

}	//Swift

#endif	//_BoneNode_H_