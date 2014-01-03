/* by ywpg */
/* Last Modified: 2007/08/31 05:51:50 . */

#ifndef _MOTION_JOINT_H_INCLUDED_
#define _MOTION_JOINT_H_INCLUDED_

#include <QtCore/QString>
#include <vector>

#include "../MathLib/Math.h"

namespace Swift
{
	///A class to store the primary information for each joint in a motion clip.
	/**
	* The member variables record information for each joint in a very motion frame. Such as offset to parent, Euler angle rotation, etc.
	*/
	class   MotionJoint
	{
		friend class MotionClip;

	private:
		/**
		 * Copy constructor is set to be private. \n
		 * Call CloneAll() or CloneSkeleton() instead.
		 * @sa CloneAll, CloneSkeleton
		 */
		MotionJoint(const MotionJoint&) // NOT copy construction function.
		{
		}

		/**
		 * Copy constructor is set to be private. \n
		 * Call CloneAll() or CloneSkeleton() instead.
		 * @sa CloneAll, CloneSkeleton
		 */
		MotionJoint operator=(const MotionJoint&);
	

	protected:
		///**
		// * This function checks if the input frame number is in the range.\n
		// * If the frame number is equal to (maximum frame number + 1), then each data vector within the joint will have an all-zero new value inserted in the end, which expands it and makes space for a new value.
		// * @param frameno The frameno we want to check.
		// */
		//void checkFrameNumberRange(int frameno);
		
	public:
		/**
		 * Constructor. Do some initialization.
		 */
		MotionJoint();

		/**
		 * Cleans up.
		 */
		virtual ~MotionJoint();


		/**
		* Set the name of this joint. \n
		* This function is always called by the setJointName function of friend class MotionClip, 
		* @param name The new name.
		*/
		void setName(std::string name);  

		/**
		 * Copy the whole joint. Except m_pParent and m_pChildren.
		 * @return The result of cloning. Pointer to the new MotionJoint object.
		 */
		MotionJoint* CloneAll();

		/**
		 * Copy the joint, but DO NOT keep data of frames. And not keep m_pParent and m_pChildren.
		 * @return The pointer to the new MotionJoint object.
		 */
		MotionJoint* CloneSketelon();

		/**
		 * Get the name of this joint.
		 * @return A std::string object containing the name.
		 */
		std::string getName() const;

		/**
		 * Get the relative position of this joint to its parent in given frame.
		 * @param frameno The frame number.
		 * @return A Vector3 object representing the position.
		 * @sa setRelativePosition
		 */
		Vector3 getRelativePosition(int frameno) ;

		/**
		 * Set the relative position of this joint to its parent in given frame.
		 * @param pos The new position.
		 * @param frameno The frame number.
		 * @sa getRelativePosition
		 */
		void setRelativePosition(Vector3 pos, int frameno);

		/**
		 * Get the absolute position of this joint in given frame.
		 * @param frameno The frame number.
		 * @return A Vector3 object representing the position.
		 * @sa setAbsolutePosition
		 */
		Vector3 getAbsolutePosition(int frameno) ;

		/**
		 * Set the absolute position of this joint in given frame.
		 * @param pos The new position.
		 * @param frameno The frame number.
		 * @sa getAbsolutePosition
		 */
		void setAbsolutePosition(Vector3 pos, int frameno);

		/**
		 * Set the x absolute position of this joint in given frame.
		 * @param posX The new x position.
		 * @param frameno The frame number.
		 * @sa getAbsolutePositionX
		 */
		void setAbsolutePositionX(double posX, int frameno);

		/**
		 * Set the y absolute position of this joint in given frame.
		 * @param posY The new y position.
		 * @param frameno The frame number.
		 * @sa getAbsolutePositionY
		 */
		void setAbsolutePositionY(double posY, int frameno);

		/**
		 * Set the z absolute position of this joint in given frame.
		 * @param posZ The new z position.
		 * @param frameno The frame number.
		 * @sa getAbsolutePositionZ
		 */
		void setAbsolutePositionZ(double posZ, int frameno);


		/**
		 * Get the quaternion representing the rotation with respect to parent joint in given frame.
		 * @param frameno The frame number.
		 * @return The Quaternion representing the rotation.
		 * @sa setOrientation
		 */
		Quaternion getOrientation(int frameno) ;

		/**
		 * Set the quaternion representing the rotation with respect to parent joint in given frame.
		 * @param quat The new Quaternion.
		 * @param frameno The frame number.
		 * @sa getOrientation
		 */
		void setOrientation(Quaternion quat, int frameno);

		/**
		 * Get the offset to parent joint on X axis. \n
		 * This value will not change unless the length of bone in skeleton is change.
		 * @return The offset.
		 */
		
		double getXOffset();//int frameno=0) ;

		/**
		 * Set the offset to parent joint on X axis. \n
		 * @param off The offset.
		 * This value will not change unless the length of bone in skeleton is change.
		 */
		void setXOffset(double off); //, int frameno=0);

		/**
		 * Get the offset to parent joint on Y axis. \n
		 * This value will not change unless the length of bone in skeleton is change.
		 * @return The offset.
		 */
		double getYOffset();//int frameno=0) ;

		/**
		 * Set the offset to parent joint on Y axis. \n
		 * This value will not change unless the length of bone in skeleton is change.
		 * @param off The offset.
		 */
		void setYOffset(double off); //, int frameno=0);

		/**
		 * Get the offset to parent joint on Z axis. \n
		 * This value will not change unless the length of bone in skeleton is change.
		 * @return The offset.
		 */
		double getZOffset(); //int frameno=0) ;

		/**
		 * Set the offset to parent joint on Z axis. \n
		 * This value will not change unless the length of bone in skeleton is change.
		 * @param off The offset.
		 */
		void setZOffset(double off); //, int frameno=0);

		/**
		 * Get the offset to parent joint . \n
		 * This value will not change unless the length of bone in skeleton is change.
		 * @return The offset on 3 axes, in the form of Vector3.
		 */
		Vector3 getXYZOffset(); //int frameno=0) ;

		/**
		 * Get the offset to parent joint on X axis. \n
		 * This value will not change unless the length of bone in skeleton is change.
		 * @param vec The offset, in a form of Vector3.
		 */
		void setXYZOffset(Vector3& vec); //, int frameno=0);

		/**
		 * Get the joint index.
		 * @return The index.
		 */
		int getIndex() const;

		/**
		 * Get the rotation on this joint for the given frame, on X axis.
		 * @param frameno Frame number we want.
		 * @return Euler angle data of X axis.
		 */
		double getXRotation(int frameno);
		
		/**
		 * Get the rotation on this joint for the given frame, on Y axis.
		 * @param frameno Frame number we want.
		 * @return Euler angle data of Y axis.
		 */
		double getYRotation(int frameno);
		
		/**
		 * Get the rotation on this joint for the given frame, on Z axis.
		 * @param frameno Frame number we want.
		 * @return Euler angle data of Z axis.
		 */
		double getZRotation(int frameno); 
		
		/**
		 * Get the rotation on this joint for the given frame.
		 * @param frameno Frame number we want.
		 * @return Euler angle data, in an order of ZXY, store in a Vector3.
		 */
		Vector3 getRotation(int frameno) ;
		
		/**
		 * Set the rotation on this joint for the given frame. \n
		 * Accept Euler angle data, in an order of ZXY.
		 * @param x The data.
		 * @param y The data.
		 * @param z The data.
		 * @param frameno The frame number.
		 */
		void setRotation(double x, double y, double z, int frameno);

		/**
		 * Set the rotation on this joint for the given frame. \n
		 * Accept Euler angle data, in an order of ZXY, store in a Vector3.
		 * @param rot The rotation data described above.
		 * @param frameno The frame number.
		 */
		void setRotation(Vector3 rot, int frameno);

		/**
		 * Set the rotation on this joint for the given frame. \n
		 * Accept Euler angle data, in an order of ZXY.
		 * @param x The data.
		 * @param frameno The frame number.
		 */
		void setRotationX(double x, int frameno);

		/**
		 * Set the rotation on this joint for the given frame. \n
		 * Accept Euler angle data, in an order of ZXY.
		 * @param y The data.
		 * @param frameno The frame number.
		 */
		void setRotationY(double y, int frameno);

		/**
		 * Set the rotation on this joint for the given frame. \n
		 * Accept Euler angle data, in an order of ZXY.
		 * @param z The data.
		 * @param frameno The frame number.
		 */
		void setRotationZ(double z, int frameno);

		/**
		 * This function will return pointer to the vector which stores Euler-angle-rotation data for each frame.
		 * @return Pointer to the vector which stores Euler-angle-rotation data for each frame.
		 * @sa setRotationDataPointer, setPositionDataPointer
		 */
		std::vector<Vector3>* getRotationDataPointer();

		/**
		 * Change pointer to the vector which stores Euler-angle-rotation data for each frame to the given one.
		 * @param pvec The pointer we want to set.
		 * @sa getRotationDataPointer, setPositionPointer
		 */
		void setRotationDataPointer(std::vector<Vector3>* pvec);

		/**
		 * This function will return pointer to the vector which stores position data of this joint for each frame.
		 * @return Pointer to the vector which stores position data of this joint for each frame.
		 * @sa setRotationDataPointer, setPositionDataPointer
		 */
		std::vector<Vector3>* getPositionDataPointer();

		/**
		 * Change pointer to the vector which stores position data of this joint for each frame.
		 * @param pvec The pointer we want to set.
		 * @sa getRotationDataPointer, setPositionPointer
		 */
		void setPositionDataPointer(std::vector<Vector3>* pvec);

		/**
		 * This function will return pointer to the vector which stores rotation data in quaternion-form of this joint for each frame.
		 * @return Pointer to the vector which stores position data of this joint for each frame.
		 * @sa setQuaternionDataPointer, getRotationDataPointer
		 */
		std::vector<Quaternion>* getQuaternionDataPointer();

		/**
		 * Change pointer to the vector which stores rotation data in quaternion-form of this joint for each frame.
		 * @param pvec The pointer we want to set.
		 * @sa getQuaternionDataPointer, setRotationDataPointer
		 */
		void setQuaternionDataPointer(std::vector<Quaternion>* pvec);

		/**
		 * This function returns pointer to the vector which stores scaling factor data.
		 * @return Pointer to the vector which stores scaling factor data.
		 * @sa setScaleDataPointer
		 */
		std::vector<Vector3>* getScaleDataPointer();

		/**
		 * Change pointer to the vector which stores scaling factor data.
		 * @param pvec The pointer we want to set.
		 * @sa getScaleDataPointer
		 */
		void setScaleDataPointer(std::vector<Vector3>* pvec);

		/**
		 * Transform the data from within the vector contains Euler-Angle rotation data into the vector contains the quaternion data.
		 * @sa updateQuaternionToEulerAngle
		 */
		void updateEulerAngleToQuaternion();

		/**
		 * Transform the data from within the vector contains quaternion data into the vector contains Euler-Angle rotation data.
		 * @sa updateEulerAngleToQuaternion
		 */
		void updateQuaternionToEulerAngle();



		/**
		 * Set the current frame number to the given one, and of course read the data of this joint to the given frame, put them into corresponding member variable.
		 * Before data is changed, this function will check if the data in member variable has been changed. If so, the data will be written back to the std::vector which holds the data.
		 * @param frameno The frame number.
		 */
		void setCurrentFrame(int frameno);

		/**
		 * Set the flag-variable of whether this joint has been selected.
		 * @param IsSelected The state we want to set.
		 */
		void setSelected(bool IsSelected)
		{
			this->m_bSelected = IsSelected;
		}

		/**
		 * See if the joint has been selected.
		 * @return Whether the joint has been selected.
		 */
		bool IsSelected()
		{
			return this->m_bSelected;
		}

		/**
		 * Set whether this joint would inherit orientation from parent.
		 * @param inherit The bool value we want.
		 */
		void setInheritOrientation(bool inherit);

		/**
		 * See if this joint would inherit orientation from parent.
		 * @return A bool value result.
		 */
		bool getInheritOrientation(void) ;

		/**
		 * Set whether this joint would inherit scaling factor from parent.
		 * @param inherit The bool value we want.
		 */
		void setInheritScale(bool inherit);

		/**
		 * See if this joint would inherit scaling factor from parent.
		 * @return A bool value result.
		 */
		bool getInheritScale(void) ;

		/**
		 * Get the value of scaling factor to be derived.
		 */ 
		const Vector3& _getInheritScale(int frameno) ;
		
		/**
		 * Get the value of positon to be derived.
		 */
		const Vector3& _getInheritPosition(int frameno) ;

		/**
		 * Get the value of orientation to be derived.
		 */
		const Quaternion& _getInheritOrientation(int frameno) ;

		/**
		 * Update data for given frame.
		 * @param frameno Frame number.
		 */
		void update(int frameno);

		/**
		 * Get the orientation from root joint till parent joint.
		 */
		Quaternion OrientationTillParent(int frameno);

		/**
		 * Set the m_bNeedUpdate flag.
		 */
		void NeedUpdate(void);


		/**
		* Set the Rotation Sequence in Euler Angles.
		*/
		void setEulerAnglesOrder(Euler::RotateOrder seq);

		/**
		* Get the Rotation Sequence in Euler Angles.
		*/
		Euler::RotateOrder getEulerAnglesOrder(void) const;

		//void setRotationAndQuaternion(Vector3 v,int frameno);
		Vector3 getUpdatedAbsolutePosition(int frameno);

	protected:
		/**
		 * Deal with all the derived values, etc., when parent is changed.
		 */
		void _updateDataFromParent(int frameno) ;

		/**
		 * Set the update flag of all the children of this joint.
		 */
		void _setUpdateFlagRecursively(bool bNeedUpdate);
		
	public:
		/**
		 * A enum type to indicate the type of joint.
		 */
		enum JointType
		{ 
			NORMAL_JOINT = 0 , 
			ROOT_JOINT,
			END_SITE
		} m_JointType; /**< Type of this joint. */

	protected:
		/**
		* Indicate the Rotation Sequence in Euler Angles.
		*/
		Euler::RotateOrder m_RotationOrder;

		/**
		 * Name of this joint.
		 */
		std::string m_strName;

		/**
		 * Index of this joint. \n
		 * This variable is often accessed by function addJoint, form friend class MotionClip.
		 */
		int m_nIndex; 

		/**
		 * Pointer to parent joint. \n
		 * @note This pointer should be got through a call from class MotionClip.
		 */
public:		MotionJoint* m_pParent;
protected:
		/**
		 * Pointer to a std::vector holding pointers to child-joints. \n
		 * @note The pointers to children should be got through a call from class MotionClip.
		 */
		public: std::vector<MotionJoint*> m_pChildren;

		/**
		 * Pointer to a std::vector containing the rotation data for each frame, in the form of quaternion. \n
		 */
		std::vector<Quaternion>* m_pQuaternionData; 

		/**
		 * Pointer to a std::vector containing the Euler-angle-rotation data.
		 */
		std::vector<Vector3>* m_pRotationData;

		/**
		 * Pointer to a std::vector containing the position data for each frame. \n
		 * @note Usually need when this is a root joint.
		 */
		std::vector<Vector3>* m_pPositionData; 

		/**
		 * Pointer to a std::vector containing the scaling factor data for each frame. \n
		 * @note Usually need when this is a root joint.
		 */
		std::vector<Vector3>* m_pScaleData; 

		/**
		 * Offset to parent, on X axis.
		 */
		double m_fXOffset;

		/**
		 * Offset to parent, on Y axis.
		 */
		double m_fYOffset;

		/**
		 * Offset to parent, on Z axis.
		 */
		double m_fZOffset;

		/**
		 * Current frame number for this joint.
		 */
		mutable unsigned int m_currentFrame;

		///**
		// * Record that if the data of current frame has been changed. If so, it would be written back when frame is changed or when data is being export to a file.
		// */
		//bool m_bDataChanged;

		/**
		 * The position of this joint, in current frame.
		 * @note The position here is the T-POSE OFFSET position with respect to parent joint.  
		 */
		mutable Vector3 m_Position; 
		
		/**
		* The position of this joint, in current frame.
		* @note The position here is the T-POSE OFFSET position with respect to parent joint.  
		*/
		mutable Vector3 m_RelativePosition; // relative to parent

		/**
		 * Temporarily store the position to be derived.
		 */
		mutable Vector3 m_DerivedPosition;
		
		///**
		// * Rotation data in quaternion form, in current frame.\n
		// * It is calculated from the Euler-angle-rotation data when changing frame.
		// */
		//Quaternion m_currentQuaternion;

		///**
		// * Rotation data in Euler-angle-rotation form, in current frame.\n
		// */
		//Vector3 m_currentRotation;

		/**
		 * Indicate whether this joint has been selected.
		 */
		bool m_bSelected;

		/**
		 * The scaling factor on this joint.
		 */
		mutable Vector3 m_Scale;

		/**
		 * Temporarily store the scaling factor to be derived.
		 */
		mutable Vector3 m_DerivedScale;

		/**
		 * Whether this joint inherits scale from parent joint.
		 */
		bool m_bInheritScale;

		/**
		 * The orientation on this joint.
		 */
		mutable Quaternion m_Orientation;
		
		/**
		 * Temporarily store the orientation to be derived.
		 */
		mutable Quaternion m_DerivedOrientation;
		
		/**
		 * Whether this joint inherits orientation from parenet joint.
		 */
		bool m_bInheritOrientation;

		/**
		 * Indicate whether this joint needs to be update.
		 */
		mutable bool m_bNeedUpdate;

	};
}

#endif //_MOTION_JOINT_H_INCLUDED_
