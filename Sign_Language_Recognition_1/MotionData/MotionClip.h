/* by ywpg */
/* Last Modified: 2007/08/28 21:23:29 . */

#ifndef _MOTION_CLIP_H_INCLUDED_
#define _MOTION_CLIP_H_INCLUDED_


#include <QtCore/QString>
#include <map>

#include "../MathLib/Math.h"

#include "MotionJoint.h"

//#include "MotionKeyframeSeries.h"

#ifdef _DEBUG
#include <iostream>
#endif

namespace Swift
{

	/// A class to representation one piece of motion clip. 
	/** 
	 * This class handles relation ship between joints (which is represented by class MotionJoint), and keep some parameters such as frame-count, frame-time, etc.
	 * @author YangWei
	 * @date 2006-4-5
	 */
	class   MotionClip
	{
		//friend class MotionDataReader;
		//friend class MotionDataWriter;

	private:
		/**
		 * Copy constructor is set to be private.\n
		 * Call CloneAll() or CloneSkeleton() when you need this.
		 * @sa CloneAll(), CloneSkeleton()
		 */
		MotionClip(const MotionClip&)
		{
		}

		/**
		 * Copy constructer is set to be private.\n
		 * Call CloneAll() or CloneSkeleton() when you need this.
		 * @sa CloneAll(), CloneSkeleton()
		 */
		MotionClip operator=(const MotionClip&);
	

	protected:
		/**
		 * Set corresponding data for each joint recursively.\n
		 * Called by setCurrentFrame()
		 * @param pj Pointer to joint on which we will operate.
		 * @param frameno The frame number we want to set.
		 * @sa setCurrentFrame()
		 */
		void setCurrentFrameRecursively(MotionJoint* pj, int frameno);

		/**
		 * Clone every joint recursively.\n
		 * Called by CloneAll()
		 * @param pmc Pointer to the new MotionClip
		 * @param pmj Pointer to the old joint we want to clone.
		 * @param pParenetJoint Pointer to the parent of the NEW joint.
		 * @sa CloneAll()
		 */
		void CloneAllRecursively(MotionClip* pmc, MotionJoint* pmj, MotionJoint* pParentJoint);

		/**
		 * Clone every joint recursively.\n
		 * Called by CloneSkeleton()
		 * @param pmc Pointer to the new MotionClip
		 * @param pmj Pointer to the old joint we want to clone.
 		 * @param pParenetJoint Pointer to the parent of the NEW joint.
		 * @sa CloneSkeleton()
		 */
		void CloneSkeletonRecursively(MotionClip* pmc, MotionJoint* pmj, MotionJoint* pParentJoint);

	public:
		/**
		 * The construction function, which initialize some member variables.
		 */ 
		MotionClip();

		/**
		 * Cleans up.
		 */
		virtual ~MotionClip();

		/**
		 * Copy the whole motion clip.
		 * @return The result of the cloning. Pointer to the new MotionClip object.
		 */
		MotionClip* CloneAll();

		/**
		 * Copy the skeleton information of motion clip, and put it into a new MotionClip object. 
		 * @return The pointer to the new MotionClip object.
		 */ 
		MotionClip* CloneSkeleton();

		/**
		 * Get the time (seconds) interval between two motion frames.
		 * @return The time interval.
		 */ 
		double getFrameTime() const;

		/**
		 * Set the time (seconds) interval between two motion frames.
		 * @param fTime The time interval.
		 */
		void setFrameTime(double fTime); 

		/**
		 * Get the number of frames.
		 * @return The number of frames.
		 */
		int getFrameCount() const;

		/**
		 * Set the number of frames.
		 * @param nCount the new frame-count to set.
		 */
		void setFrameCount(int nCount);

		/**
		 * Get the number of joints within this MotionClip object.
		 * @return The number of joints.
		 */
		int getJointCount() const;

		/**
		 * Set the number of joints within this MotionClip object.
		 * @param nCount The number of joints.
		 */
		void setJointCount(int nCount);

		/**
		 * Get the name of the input file corresponding to this MotionClip object.
		 * It is also the default filename used when this MotionClip object is exported into a file.
		 * @return The name.
		 */
		std::string getFileName() const;

		/**
		 * Set the name of the input file corresponding to this MotionClip object.
		 * It is also the default filename used when this MotionClip object is exported into a file.
		 * @param name The name to set.
		 */
		void setFileName(std::string name);

		/**
		 * Wipe up the data within this MotionClip.
		 * @return 0 if everything is fine.
		 */
		int clearAll();

		/**
		 * Add a joint in this MotionClip.
		 * @return The pointer to the new MotionJoint.
		 */
		MotionJoint* addJoint();

		/**
		* Add a existed joint into this MotionClip. Do nothing if p is NULL.
		*/
		void addExistedJoint(MotionJoint *p);

		/**
		 * Add a joint in this MotionClip.
		 * @param name The name of the new joint.
		 * @param off Offset to parent joint.
		 * @param parent Pointer to parent joint, can be NULL (e.g.:when adding the first joint).
		 * @return The pointer to the new MotionJoint.
		 */
		MotionJoint* addJoint(std::string name, Vector3 off, MotionJoint* parent = NULL);

		/**
		 * Add a joint in this MotionClip.
		 * @param name The name of the new joint.
		 * @param xoff Offset on the X axis, with respect to parent joint.
		 * @param yoff Offset on the Y axis, with respect to parent joint.
		 * @param zoff Offset on the Z axis, with respect to parent joint.
		 * @param parent Pointer to parent joint, can be NULL (e.g.:when adding the first joint).
		 * @return The pointer to the new MotionJoint.
		 */
		MotionJoint* addJoint(std::string name, double xoff, double yoff, double zoff, MotionJoint* parent = NULL);

		/**
		 * Delete a given joint from this MotionClip.
		 * @param pJoint Pointer to the MotionJoint to be deleted.
		 * @return 0 if everything is fine.
		 */
		int deleteJoint(MotionJoint* pJoint);

		/**
		 * Set parent joint for the given joint.
		 * @param pJoint The joint to be child.
		 * @param pParent The parent joint.
		 */
		void setParent(MotionJoint* pJoint, MotionJoint* pParent);
	    
		/**
		 * Change the state of the whole sketelon to given frame. And return the root joint.
		 * @param frameno The frame number, of the frame we want.
		 * @return Pointer to root joint.
		 * @note The current frame of the whole sketelon but not only the root joint will be changed.
		 */
		MotionJoint* getRoot(int frameno);

		/**
		* Return the root joint.
		* @return Pointer to root joint.
		*/
		MotionJoint* getRoot(void);

		/**
		 * Set the ONLY root joint of this MotionClip.
		 * @param root Pointer to the joint which we would like to be root.
		 * @return 0 if everything is fine.
		 */
		int setRoot (MotionJoint* root);

		/**
		 * Set name of the given joint.
		 * @param name Name.
		 * @param pJoint Pointer to joint of which we want to change name.
		 * @return 0 if everything if fine.
		 */
		int setJointName (std::string name, MotionJoint* pJoint);

		/**
		 * Set the current frame.\n
		 * This function will update the data of the whole skeleton to the given frame, and of course, previous modification will be write back to the data set corresponding to this MotionClip.
		 * @param frameno The frame number we want it to be.
		 * @return 0 if everything is fine.
		 */
		int setCurrentFrame(int frameno);

		/**
		 * To look up a joint with given name in this MotionClip.
		 * @note The data of WHOLE skeleton will be changed to data of the given frame number.
		 * @param name Name of the joint.
		 * @param frameno The frame number we want.
		 * @return Pointer to the joint.
		 */
		MotionJoint* findJoint(std::string name, int frameno);
	  
		/**
		 * To look up a joint with given joint index in this MotionClip.
		 * @note The data of WHOLE skeleton will be changed to data of the given frame number.
		 * @param index The joint index.
		 * @param frameno The frame number we want.
		 * @return Pointer to the joint, NULL when not found.
		 */
		MotionJoint* findJoint(int index, int frameno);

		/**
		 * To look up a joint with given joint index in this MotionClip.
		 * @note This function will not change the current frame of this joint.
		 * @param index The joint index.
		 * @return Pointer to the joint, NULL when not found.
		 */
		MotionJoint* findJoint(int index);

		/**
		* To look up a joint with given joint name in this MotionClip.
		* @note This function will not change the current frame of this joint.
		* @param name Name of the joint.
		* @return Pointer to the joint, NULL when not found.
		*/
		MotionJoint* findJoint(std::string name);

		/**
		 * Find out a joint's parent.
		 * @param pJoint Pointer to the child joint.
		 * @return Pointer to the parent joint.
		 */
		MotionJoint* getParent(MotionJoint* pJoint);

		/**
		 * Find out a joint's parent.
		 * @param index The joint index of the child joint.
		 * @return Pointer to the parent joint.
		 */
		MotionJoint* getParent(int index);
	  
		/**
		 * Find out a joint's parent, return the index of the parent.
		 * @param pJoint Pointer to the child joint.
		 * @return Joint index of the parent joint.
		 */
		int getParentIndex(MotionJoint* pJoint);

		/**
		 * Find out a joint's parent, return the index of the parent.
		 * @param index The joint index of the child joint.
		 * @return Joint index of the parent joint.
		 */
		int getParentIndex(int index);
	  
		/**
		 * Find out a joint's index.
		 * @param pJoint Pointer to the joint.
		 * @return Joint index of the joint.
		 */
		int getJointIndex(MotionJoint* pJoint);

		/**
		 * Find out a joint's index by its name.
		 * @param name Name of the joint.
		 * @return Joint index.
		 */
		int getJointIndexByName (std::string name);

		/**
		 * Find out how many child-joints does a joint have.
		 * @param pJoint Pointer to the joint into which we want to look.
		 * @return The number of children.
		 */
		int getChildrenCount(MotionJoint* pJoint);

		/**
		 * Find out how many child-joints does a joint have.
		 * @param index Joint index.
		 * @return The number of children.
		 */
		int getChildrenCount(int index);

		/**
		 * Get the child of some joint with given number.
		 * @param pJoint Pointer to the joint.
		 * @param childno Child-number.
		 * @return Pointer to the very Child.
		 */
		MotionJoint* getChild(MotionJoint* pJoint, int childno);

		/**
		 * Get the child of some joint with given number.
		 * @param index Index of the joint.
		 * @param childno Child-number.
		 * @return Pointer to the very Child.
		 */
		MotionJoint* getChild(int index, int childno);

		/**
		 * Remove the parent-child relationship between two joints.
		 * @param pj Pointer to the child.
		 * @param pParent Pointer to the parent.
		 * @return 0 if everything is fine.
		 * @warning Only the relationship is removed. Both joints are remained.
		 */
		int deleteChild(MotionJoint *pj, MotionJoint* pParent);

		/**
		 * Add frames to this MotionClip.
		 * @param beforewhich Frames will be insert BEFORE which frame.
		 * @param count The number of frames to insert.
		 * @return The total frame-count after the insertion.
		 * @note Frame-count begins from 0.
		 * @sa insertFrameBehind
		 */
		int insertFrame(int beforewhich, int count);

		/**
		 * Add frames to this MotionClip.
		 * @param behindwhich Frames will be insert BEHIND which frame.
		 * @param count The number of frames to insert.
		 * @return The total frame-count after the insertion.
		 * @note Frame-count begins from 0.
		 * @sa insertFrame
		 */
		int insertFrameBehind (int behindwhich, int count);

		/**
		* Add frames to this MotionClip.
		* @param behindwhich Frames will be insert BEHIND which frame.
		* @param count The number of frames to insert.
		* @param frameno The number of frame to be copied to the added frames.
		* @return The total frame-count after the insertion.
		* @note Frame-count begins from 0.
		* @sa insertFrame
		*/
		int insertFrameBehind (int behindwhich, int count, int frameno);
		
		/**
		 * Delete frames from this MotionClip.
		 * @param start The index of frame to start the deletion
		 * @param count How many frames to delete.
		 * @note Frame-count begins from 0.
		 * @sa insertFrame, insertFrameBehind
		 */
		void deleteFrame(int start, int count);
	  
		/**
		 * Re-generate joint index for every joint in this MotionClip.
		 * @sa deleteFrame
		 */
		void updateIndex();

		/**
		 * Re-generate joint index for every joint in this MotionClip, recursively.\n
		 * This function is called by updateIndex
		 * @param pmj Pointer to joint with which we are going to deal.
		 * @sa updateIndex, deleteFrame
		 */
		void updateIndexRecursively(MotionJoint* pmj);


		/**
		 * Compare two skeletons to see if their structure are the same.
		 * @param pmc Pointer to first MotionClip we want to compare.
		 * @param pmc2 Pointer to second MotionClip we want to compare. If this is not given, take this MotionClip.
		 * @return true if they are the same.
		 */
		bool compareSkeleton(MotionClip* pmc, MotionClip* pmc2= NULL);

		/**
		 * Compare two skeletons, recursively.
		 * @param pmc Pointer to first MotionClip we want to compare.
		 * @param pmc2 Pointer to second MotionClip we want to compare. If this is not given, take this MotionClip.
		 * @param pmj Pointer to first MotionJoint we are comparing.
		 * @param pmj2 Pointer to second MotionJoint we are comparing.
		 * @return true if they are the same.
		 */
		bool compareSkeletonRecursively(MotionClip* pmc, MotionClip* pmc2, MotionJoint* pmj, MotionJoint* pmj2); 

		/**
		* Compare two skeletons to see if the joint names are the same.
		* @param pmc Pointer to first MotionClip we want to compare.
		* @param pmc2 Pointer to second MotionClip we want to compare. If this is not given, take this MotionClip.
		* @return true if the joint names are the same.
		*/
		bool compareJointNames(MotionClip* pmc, MotionClip* pmc2= NULL);

		/**
		* Compare two skeletons, recursively.
		* @param pmc Pointer to first MotionClip we want to compare.
		* @param pmc2 Pointer to second MotionClip we want to compare. If this is not given, take this MotionClip.
		* @param pmj Pointer to first MotionJoint we are comparing.
		* @param pmj2 Pointer to second MotionJoint we are comparing.
		* @return true if the joint names are the same.
		*/
		bool compareJointNamesRecursively(MotionClip* pmc, MotionClip* pmc2, MotionJoint* pmj, MotionJoint* pmj2); 


//		/**
//		 * Get pointer to the MotionKeyframeSeries Object.
//		 */
//		MotionKeyframeSeries* getKeyframeSeries(void) const;
//
//		/**
//		 * Create a MotionKeyframeSeries Object with two keyframe at the beginning frame and ending frame of the motion-clip.And then, return the pointer to the newly created MotionKeyframeSeries object.
//		 */
//		MotionKeyframeSeries* initKeyframeSeries(void);
//
//		/**
//		 * Call copy constructor of class MotionKeyframeSeries, to create a copy of given MotionKeyframeSeries.
//		 */
//		void copyKeyframeSeries(MotionKeyframeSeries & kfs);

		/**
		 * Lock this MotionClip, make it read-only.
		 * @sa unlock(), IsLocked()
		 */
		void lock()
		{
			m_bLocked=true;
		}
	  
		/**
		 * Unlock this MotionClip.
		 * @sa lock(), IsLocked()
		 */
		void unlock()
		{
			m_bLocked=false;
		}

		/**
		 * See if this MotionClip is locked, or rather, read-only
		 * @sa lock(), unlock()
		 */
		bool IsLocked()
		{
			return m_bLocked;
		}

	#ifdef _DEBUG
		/**
		 * Dump information. Used when debugging.
		 * @sa dumpJoint()
		 */
		void dump();
		
		/**
		 * Dump information for each joint, recursively.
		 * Call by dump()
		 * @sa dump()
		 */
		void dumpJoint(MotionJoint* pj);
	#endif // _DEBUG

		int GetIndexCount();

	  protected:
		/**
		 * The default file name for this MotionClip. Can be empty.
		 */
		std::string m_strFileName;

		/**
		 * A map from joint names to pointers to joints.
		 */
		std::map<std::string, MotionJoint*> m_JointsNameMap;

		/**
		 * A map from joint indexes to pointers to joints.
		 */
		std::map<int, MotionJoint*> m_JointsIndexMap;

		/**
		 * Member to keep the pointer to the root joint.
		 */
		MotionJoint* m_pRoot;

		/**
		 * How many joints are there in this MotionClip.
		 */
		int m_nJointCount;

		/**
		 * How many frames are there in this MotionClip.
		 */
		int m_nFrameCount;

		/**
		 * The current frame number. This indicates the frame to which data of skeleton corresponding to.
		 */
		int m_currentFrame;

		/**
		 * The time interval (seconds) between two frames.
		 */
		double m_fFrameTime;
	    
		/**
		 * Record if this MotionClip is locked.
		 */
		bool m_bLocked;

		/**
		 * Because the index number for each joint is generated automaticly, we need a variable to record the next available serial number.
		 */
		int m_nNextIndex;

//		/**
//		 * Pointer to MotionKeyframeSeries.
//		 */
//		MotionKeyframeSeries * m_pKeyframeSeries;

	};
}
#endif //_MOTION_CLIP_H_INCLUDED_
