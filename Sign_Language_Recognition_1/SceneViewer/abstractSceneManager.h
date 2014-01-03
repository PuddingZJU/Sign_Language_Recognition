
#ifndef ABSTRACT_SceneManager_H_
#define ABSTRACT_SceneManager_H_

#include <vector>
#include <map>
#include "../../Util/Singleton.h"
#include "SimpleObject.h"
#include "IDManager.h"
#include "TraceTable.h"
#include "SceneNode.h"
#include "BoneNode.h"
#include "RenderQueue.h"
#include "Ground.h"
#include "../MotionData/BVHWriter.h"

namespace Swift
{
	class MotionClip;
	class Skeleton;
/*
 * this class is a abstract class for class SceneManager
 * the latter is a Singleton ,but we need a new class which
 * needs some exiting codes in SceneManager, So we abstract 
 * the SceneManager's interface and create a new class inheriting
 * from the abstract class. It is a pure virtual class .
 *
 * @wangzhaoyang 
 * date : 2008/03/27
 * wzypaulzz@hotmail.com
 */
	class AbstractSceneManager : public QObject
	{
		Q_OBJECT

	public:
		/// Constructor.
		AbstractSceneManager();
		/// Destructor.
		virtual ~AbstractSceneManager() = 0;

		/// Create the skeleton of a humanoid.
		/**
		* @param nType The type of the skeleton, 0 for xm template skeleton, 1 for common motion skeleton, 2 for xm-captured motion skeleton.
		* @param sot The type of object attached to each joint.
		* @return The root node of the skeleton.
		*/
		virtual BoneNode* createSkeleton(MotionClip* pMotionClip, int nType=1, SimpleObjectType sot=SOT_SPHERE);

		/// Create a named scene node.
		/**
		* @return The SceneNode created.
		*/
		virtual SceneNode* createSceneNode(const QString& strName="", const Vector3& vTranslate=Vector3::ZERO, const Quaternion& qRotate=Quaternion::IDENTITY);

		/// Create the root scene node.
		/**
		* @return The root scene node created.
		*/
		virtual SceneNode* createRootSceneNode(const QString& strName="RootSceneNode", const Vector3& vTranslate=Vector3::ZERO, const Quaternion& qRotate=Quaternion::IDENTITY);

		///// Create the ground of the scene.
		///**
		//* @return The pointer to the ground created.
		//*/
		//virtual Ground* createGround();

		///// Get the pointer to the ground.
		///**
		//* @return The pointer to the ground created.
		//*/
		//virtual Ground* getGround();

		/// Return the root scene node.
		/**
		* @return The root scene node.
		*/
		virtual SceneNode* getRootSceneNode() const;

		/// Delete a scene node.
		/**
		* @remarks Delete the children of the node, and then delete this node.
		*/
		virtual void deleteSceneNode(SceneNode* pSceneNode);

		/// Delete the whole scene graph.
		/**
		* @remarks Delete all the children of the root node, and then delete the root node.
		*/
		virtual void deleteSceneGraph();

		/// Attach joint objects to all skeletons.
		/**
		* The joints of a skeleton may be a small cube or cube or other object.
		* @param sot Specify the type of object attached to the bone node.
		* @see also SimpleObject.
		*/
		virtual void attachAllSkeletonObjects(SimpleObjectType sot = SOT_SPHERE);

		/// Update the bone nodes' data from a motion clip. 
		/**
		* @param nFrameNo The current frame number.
		*/
		virtual void updateDataFromMotionClip(int nFrameNo);

		/// Update the bone nodes' data from a xm motion clip when capturing using xmdevice. 
		/**
		* @param nFrameNo The current frame number.
		*/
		virtual void updateDataFromXMMotionClip(int nFrameNo);

		/// Add scene object to the render queue.
		/**
		* @param pSceneObject The pointer to the scene object.
		*/
		virtual void addObjectToRenderQueue(SceneObject* pSceneObject);

		/// Add all scene objects to the render queue.
		/**
		* Add the object which is attached to the node to the render queue,
		* and also add the objects attached to children of this node.
		* @param pNode Pointer to a scene node.
		*/
		virtual void addObjectToRenderQueue(Node* pNode);
		
		/// Remove scene object from the render queue.
		/**
		* @param pSceneObject The pointer to the scene object.
		*/
		virtual void removeObjectFromRenderQueue(SceneObject* pSceneObject);

		/// Remove all scene objects from the render queue.
		/**
		* Remove the object which is attached to the node from the render queue,
		* and also add the objects attached to children of this node.
		* @param pNode Pointer to a scene node.
		*/
		virtual void removeObjectFromRenderQueue(Node* pNode);

		/// Return the pointer to the render queue.
		/**
		* @return Pointer to the render queue.
		*/
		virtual RenderQueue* getRenderQueue() const;

		/// Return the vector of the picked list
		virtual std::vector<SceneObject*> getPickedList() const;

		/// Render the bones of every skeleton in the scene.
		/**
		* The bone between two joints is just drawn as a simple line.
		* This function will call the recursive function renderBones(BoneNode* pBoneNode).
		*/
		virtual void renderBones();

		/// Draw the lines between a parent node and its children.
		/**
		* @param pNode Pointer to a bone Node.
		* This function will be called by renderBones().
		*/
		virtual void renderBones(Node* pNode);

		/// Insert the ID-pointer pair to the IDTraceableMap.
		/**
		* @param nID The ID of the traceable.
		* @param pTraceable The pointer to the traceable.
		*/
		virtual void insertIDTraceable(int nID,Traceable* pTraceable);

		/// Remove the ID-pointer pair from the IDTraceableMap.
		/**
		* @param nID The ID of the traceable.
		*/
		virtual void removeIDTraceable(int nID);

		/// Get the pointer paired to the ID.
		/**
		* @param nID The ID of the traceable.
		*/
		virtual Traceable* getPointerByID(int nID);

		/// Return the max Frame count of all the clips in the scene.
		virtual int getMaxFrameCount(void) const;

		/// Return the current frame number in the scene.
		virtual int getCurrentFrameNo(void) const;

		/// Set the current frame number in the scene.
		virtual void setCurrentFrameNo(int nFrameNo);

		/// Get the pointer to the latest motion clip.
		virtual MotionClip* getLatestMotionClip() const;

		/// Get the pointer to the latest XMTemplate motion clip.
		virtual MotionClip* getLatestXMTemplate() const;

		/// Get the pointer to the latest XM Skeleton.
		virtual Skeleton* getLatestXMSkeleton() const;

		/// Get the pointer to the capturing motion clip.
		virtual MotionClip* getCapturingMotionClip() const;

		/// Get the pointer to the skeleton of the capturing motion clip.
		virtual Skeleton* getCapturingSkeleton() const;

		/// Set the pointer to the capturing motion clip.
		virtual void setCapturingMotionClip(MotionClip* pMotionClip);

		/// Add the picked object to the picked list.
		virtual void addPickedObject(SceneObject* pSceneObject);

		/// Add a set of picked objects to the picked list.
		virtual void addPickedObjects(std::vector<SceneObject*>& vPickedObjects);

		/// Call this to emit an sceneObjectsChanged signal of this class.
		virtual void finishedCapture();

		/// Save the changes of the current motion to motion file.
		virtual bool saveMotion();

		/// Save the current motion to another motion file.
		virtual bool saveMotionAs(QString srtFilePath);


	signals:
		/// Emit this signal when importing a new motion.
		void importNewMotion(int nType, int nIndex);

		/// Emit this signal when the scene needs to update.
		void needUpdate(void);

		/// Emit this signal when several skeletons are picked by rect selection.
		void pickedSeveralSkeletons();

		/// Emit this signal when the state of a motion changed.
		void motionStateChanged(int nIndex, int state);

		/// Emit this signal when the scene objects have changed.
		void sceneObjectsChanged();


	public slots:
		/// Slot to process the remove motion signal.
		virtual void removeMotion(int nIndex);
		/// Slot to remove all the motions in this scene
		virtual void removeAllMotions();
	//private slots:
	//	/// Slot to process the mouse selecting of a joint on the tree view.
	//	virtual void setSelectedJoint(int nIndex, const QString &sJointName);

	//	/// Slot to process the mouse selecting of a motion on the tree view.
	//	virtual void onSelectMotion(int nIndex);

	//	/// Slot to process the property changing event in the property dialog.
	//	virtual void onPropertyChanged(QString strGroupName,QString strProperyName,QString strPropValue);
		
	public:
		/// Skeleton list in the scene.
		std::vector<Skeleton*> m_vSkeleton; 
	protected:
		/// Pointer to the root SceneNode.
		SceneNode* m_pRootSceneNode;
		/// Pointer to the RenderQueue
		RenderQueue* m_pRenderQueue;
		///// Pointer to the ground.
		//Ground* m_pGround;
		/// All the IDs and pointers to the traceable.
		typedef std::map<int, Traceable*> IDTraceableMap;
		IDTraceableMap m_mIDTraceableMap;
		/// The max frame number of all the clips in the scene.
		int m_nMaxFrameCount;
		/// The current frame number in the scene.
		int m_nCurrentFrameNo;
		/// All the picked objects.
		std::vector<SceneObject*> m_vPickedList;
		/// All the picked skeleton.
		std::set<Skeleton*> m_sPickedSkeleton;
		/// The pointer to the last xm-template.
		MotionClip* m_pLastestXMTemplate;
		/// The pointer to the last xm-skeleton.
		Skeleton* m_pLastestXMSkeleton;
		/// The pointer to the capturing motion clip by the xm device.
		MotionClip* m_pCapturingMotionClip;
		/// The pointer to the skeleton of the capturing motion clip by the xm device.
		Skeleton* m_pCapturingSkeleton;
		/// The index of the current selected motion.
		int m_nCurrentMotionIndex;
	};	//SceneManager
}	//Swift

#endif	//ABSTRACT_SceneManager_H_
