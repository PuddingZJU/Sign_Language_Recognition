/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/08/15                                                  */
/* Last Modified: 2007/09/05                                             */
/************************************************************************/

#ifndef _Node_H_
#define _Node_H_

#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <vector>
#include <map>
#include <set>
#include "Traceable.h"
#include "Serializable.h"
#include "IDManager.h"
#include "SceneObject.h"
#include "../../MathLib/Math.h"

namespace Swift
{
	class AbstractSceneManager;
	
	/// Class representing a general-purpose node of an articulated scene graph.
	/**
		@remarks
		A node in the scene graph is a node in a structured tree. A node contains
		information about the transformation which will apply to it and all of 
		it's children. Child nodes can have transforms of their own, which are
		combined with their parent's transformations.
		@par
		This is an abstract class - concrete classes are based on this for specific purposes,
		e.g. SceneNode, Bone
	*/
	class Node : public Traceable,public Serializable
	{
	public:
		/// Enumeration denoting the spaces which a transform can be relative to.
		enum TransformSpace
		{
			/// Transform is relative to the local space
			TS_LOCAL,
			/// Transform is relative to the space of the parent node
			TS_PARENT,
			/// Transform is relative to world space
			TS_WORLD
		};


		/// Flag indicating that the node need to be updated..
		mutable bool m_bNeedUpdate;

		/// Collection of pointers to direct children.
		typedef std::map<unsigned long,Node*> ChildrenMap;
		typedef std::pair<unsigned long,Node*> IDNodePair;
		mutable ChildrenMap m_mChildren;

		///// Collection of the numbers of each name.
		//typedef std::map<QString,int> ChildrenNameNumMap;
		//typedef std::pair<QString,int> NameNumPair;
		//ChildrenNameNumMap m_mNameNum;

	protected:
		/// Pointer to the scene manager
		AbstractSceneManager * m_pSceneManager;
		/// Pointer to parent node.
		Node* m_pParent;

		///// Pointer to the attached object.
		//SceneObject* m_pSceneObject;

		/// All the objects attached to the node.
		std::vector<SceneObject*> m_vSceneObject;

		/// Stores the orientation of the node relative to it's parent.
		Quaternion m_qOrientation;

		/// Stores the rotation of the node relative to it's parent.
		Euler m_eRotation;

		/// Stores the position/translation of the node relative to its parent.
		Vector3 m_vPosition;

		/// Stores the scaling factor applied to this node
		Vector3 m_vScale;

		/// Stores whether this node inherits scale from it's parent
		bool m_bInheritScale;

		///  Stores whether this node is a BoneNode.
		bool m_bBoneNode;

		///// Material pointer should this node be rendered
		//mutable MaterialPtr mpMaterial;


		/// Cached combined orientation.
		/**
		 * This member is the orientation derived by combining the
		 * local transformations and those of it's parents.
		 * This is updated when _updateFromParent is called by the
		 * SceneManager or the nodes parent.
		*/
		mutable Quaternion m_qDerivedOrientation;

		/// Cached combined position.
		/**
		 * This member is the position derived by combining the
		 * local transformations and those of it's parents.
		 * This is updated when _updateFromParent is called by the
		 * SceneManager or the nodes parent.
		*/
		mutable Vector3 m_vDerivedPosition;

		/// Cached combined scale.
		/** 
		 * This member is the position derived by combining the
		 * local transformations and those of it's parents.
		 * This is updated when _updateFromParent is called by the
		 * SceneManager or the nodes parent.
		*/
		mutable Vector3 m_vDerivedScale;

		/// Cached derived transform as a 4x4 matrix.
		mutable Matrix4 m_mCachedTransform;
		/// Flag indicating whether the derived transform is out of date.
		mutable bool m_bCachedTransformOutOfDate;

		/// Triggers the node to update it's combined transforms.
		/** 
		 * @par
		 * This method is called internally by Ogre to ask the node
		 * to update it's complete transformation based on it's parents
		 * derived transform.
		*/
		virtual void _updateFromParent(void) const;

		///// Internal method for creating a new child node - must be overridden per subclass. 
		//virtual Node* createChildImpl(void) = 0;

		///// Internal method for creating a new child node - must be overridden per subclass. 
		//virtual Node* createChildImpl(const QString& name) = 0;

		/// Internal method for building a Matrix4 from orientation / scale / position. 
		/**
		 * @remarks
		 * Transform is performed in the order rotate, scale, translation, i.e. translation is independent
		 * of orientation axes, scale does not affect size of translation, rotation and scaling are always
		 * centered on the origin.
		*/
		void makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation,Matrix4& destMatrix) const;

		/// Internal method for building an inverse Matrix4 from orientation / scale / position. 
		/**
		 * @remarks
		 * As makeTransform except it build the inverse given the same data as makeTransform, so
		 * performing -translation, 1/scale, -rotate in that order.
		*/
		void makeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation,	Matrix4& destMatrix);

		///// Decrease the number of the specified Node.
		//void _decreaseNameNum(Node* pNode);
	public:
		/** Constructor.
		 * @remarks
		 * Generates a name.
		*/
		explicit Node(AbstractSceneManager * p_mgr);
		/** Constructor.
		 * @remarks
		 * Assigned a name.
		*/
		Node(const QString& name, AbstractSceneManager * p_mgr);
		/** Destructor.
		 * @remarks
		 * destroy a node.
		*/
		virtual ~Node();  

		/// Gets this node's parent (NULL if this is the root).
		virtual Node* getParent(void) const;

		/// Set the parent of the node.
		virtual void setParent(Node* parent);

		/// Return the flag which indicates whether the node is a bone node.
		virtual bool isBoneNode() const;

		/// Set the update flag of this node.
		virtual void needUpdate();

		/// Get the orientation of the node.
		virtual const Quaternion& getOrientation() const;

		///Sets the orientation of this node via quaternion parameters.
		virtual void setOrientation(double w, double x, double y, double z);

		/// Sets the orientation of this node via a quaternion.
		virtual void setOrientation(const Quaternion& q);

		/// Resets the nodes orientation (local axes as world axes, no rotation).
		virtual void resetOrientation(void);

		/// Get the rotation of the node.
		virtual Euler& getRotation();

		/// Get the rotation of the node.
		virtual void setRotation(Euler eRotation);

		/// Gets the position of the node relative to it's parent.
		virtual const Vector3& getPosition(void) const;

		/// Sets the position of the node relative to it's parent.
		virtual void setPosition(const Vector3& pos);

		/// Sets the position of the node relative to it's parent.
		virtual void setPosition(double x, double y, double z);

		/// Gets the scaling factor of this node.
		virtual const Vector3& getScale(void) const;

		/// Sets the scaling factor applied to this node.
		/** 
		 * @remarks
		 * Scaling factors, unlike other transforms, are not always inherited by child nodes. 
		 * Whether or not scalings affect both the size and position of the child nodes depends on
		 * the setInheritScale option of the child. In some cases you want a scaling factor of a parent node
		 * to apply to a child node (e.g. where the child node is a part of the same object, so you
		 * want it to be the same relative size and position based on the parent's size), but
		 * not in other cases (e.g. where the child node is just for positioning another object,
		 * you want it to maintain it's own size and relative position). The default is to inherit
		 * as with other transforms.
		 * @par
		 * Note that like rotations, scalings are oriented around the node's origin.
		*/
		virtual void setScale(const Vector3& vScale);

		/// Sets the scaling factor applied to this node.
		/**
		 * @remarks
		 * Scaling factors, unlike other transforms, are not always inherited by child nodes. 
		 * Whether or not scalings affect both the size and position of the child nodes depends on
		 * the setInheritScale option of the child. In some cases you want a scaling factor of a parent node
		 * to apply to a child node (e.g. where the child node is a part of the same object, so you
		 * want it to be the same relative size and position based on the parent's size), but
		 * not in other cases (e.g. where the child node is just for positioning another object,
		 * you want it to maintain it's own size and relative position). The default is to inherit
		 * as with other transforms.
		 * @par
		 * Note that like rotations, scalings are oriented around the node's origin.
		*/
		virtual void setScale(double x, double y, double z);

		/// Tells the node whether it should inherit scaling factors from it's parent node.
		/** 
		 * @remarks
		 * Scaling factors, unlike other transforms, are not always inherited by child nodes. 
		 * Whether or not scalings affect both the size and position of the child nodes depends on
		 * the setInheritScale option of the child. In some cases you want a scaling factor of a parent node
		 * to apply to a child node (e.g. where the child node is a part of the same object, so you
		 * want it to be the same relative size and position based on the parent's size), but
		 * not in other cases (e.g. where the child node is just for positioning another object,
		 * you want it to maintain it's own size and relative position). The default is to inherit
		 * as with other transforms.
		 * @param inherit If true, this node's scale and position will be affected by its parent's scale. If false,
		 * it will not be affected.
		*/
		virtual void setInheritScale(bool bInherit);

		/// Returns true if this node is affected by scaling factors applied to the parent node. 
		/**
		 * @remarks
		 * See setInheritScale for more info.
		*/
		virtual bool getInheritScale(void) const;

		/// Scales the node, combining it's current scale with the passed in scaling factor. 
		/**
		 * @remarks
		 * This method applies an extra scaling factor to the node's existing scale, (unlike setScale
		 * which overwrites it) combining it's current scale with the new one. E.g. calling this 
		 * method twice with Vector3(2,2,2) would have the same effect as setScale(Vector3(4,4,4)) if
		 * the existing scale was 1.
		 * @par
		 * Note that like rotations, scalings are oriented around the node's origin.
		*/
		virtual void scale(const Vector3& scale);

		/// Scales the node, combining it's current scale with the passed in scaling factor. 
		/** 
		 * @remarks
		 * This method applies an extra scaling factor to the node's existing scale, (unlike setScale
		 * which overwrites it) combining it's current scale with the new one. E.g. calling this 
		 * method twice with Vector3(2,2,2) would have the same effect as setScale(Vector3(4,4,4)) if
		 * the existing scale was 1.
		 * @par
		 * Note that like rotations, scalings are oriented around the node's origin.
		*/
		virtual void scale(double x, double y, double z);

		/// Moves the node along the cartesian axes.
		/**
		 * @par
		 * This method moves the node by the supplied vector along the
		 * world cartesian axes, i.e. along world x,y,z
		 * @param d Vector with x,y,z values representing the translation.
		 * @param relativeTo The space which this transform is relative to.
		*/
		virtual void translate(const Vector3& d, TransformSpace relativeTo = TS_PARENT);

		/// Moves the node along the Cartesian axes.
		/** 
		 * @par
		 * This method moves the node by the supplied vector along the
		 * world Cartesian axes, i.e. along world x,y,z
		 * @param x
		 * @param y
		 * @param z double x, y and z values representing the translation.
		 * @param relativeTo The space which this transform is relative to.
		*/
		virtual void translate(double x, double y, double z, TransformSpace relativeTo = TS_PARENT);
		
		/** Moves the node along arbitrary axes.
		 * @remarks
		 * This method translates the node by a vector which is relative to
		 * a custom set of axes.
		 * @param 
		 * axes A 3x3 Matrix containing 3 column vectors each representing the
		 * axes X, Y and Z respectively. In this format the standard Cartesian
		 * axes would be expressed as:
		 * <pre>
		 * 1 0 0
		 * 0 1 0
		 * 0 0 1
		 * </pre>
		 * i.e. the identity matrix.
		 * @param 
		 * move Vector relative to the axes above.
		 * @param
		 * relativeTo The space which this transform is relative to.
		*/
		virtual void translate(const Matrix3& axes, const Vector3& move, TransformSpace relativeTo = TS_PARENT);
		/** Moves the node along arbitrary axes.
		 * @remarks
		 * This method translates the node by a vector which is relative to
		 * a custom set of axes.
		 * @param 
		 * axes A 3x3 Matrix containing 3 column vectors each representing the
		 * axes X, Y and Z respectively. In this format the standard Cartesian
		 * axes would be expressed as
		 * <pre>
		 * 1 0 0
		 * 0 1 0
		 * 0 0 1
		 * </pre>
		 * i.e. the identity matrix.
		 * @param 
		 * x,y,z Translation components relative to the axes above.
		 * @param
		 * relativeTo The space which this transform is relative to.
		*/
		virtual void translate(const Matrix3& axes, double x, double y, double z, TransformSpace relativeTo = TS_PARENT);

		/// Rotate the node around the Z-axis.
		/**
		 * The unit of angle is radian.
		 * @param radian the rolling angle represented by radian.
		*/
		virtual void roll(double radian, TransformSpace relativeTo = TS_LOCAL);

		/// Rotate the node around the Z-axis.
		/**
		* The unit of angle is degree.
		* @param radian the rolling angle represented by degree.
		*/
		inline void rolld(double degree, TransformSpace relativeTo = TS_LOCAL) 
		{
			roll(Math::DegreeToRadian(degree), relativeTo);
		}

		/// Rotate the node around the X-axis.
		/**
		* The unit of angle is radian.
		* @param radian the rolling angle represented by radian.
		*/
		virtual void pitch(double radian, TransformSpace relativeTo = TS_LOCAL);


		/// Rotate the node around the X-axis.
		/**
		* The unit of angle is degree.
		* @param radian the rolling angle represented by degree.
		*/
		inline void pitchd(double degree, TransformSpace relativeTo = TS_LOCAL)
		{
			pitch(Math::DegreeToRadian(degree), relativeTo);
		}

		/// Rotate the node around the Y-axis.
		/**
		* The unit of angle is radian.
		* @param radian the rolling angle represented by radian.
		*/
		virtual void yaw(double radian, TransformSpace relativeTo = TS_LOCAL);

		/// Rotate the node around the Y-axis.
		/**
		* The unit of angle is degree.
		* @param radian the rolling angle represented by degree.
		*/
		inline void yawd(double degree, TransformSpace relativeTo = TS_LOCAL) 
		{
			yaw(Math::DegreeToRadian(degree), relativeTo);
		}

		/// Rotate the node around an arbitrary axis.
		/**
		* The unit of angle is radian.
		* @param radian the rolling angle represented by radian.
		*/
		virtual void rotate(const Vector3& axis, double radian, TransformSpace relativeTo = TS_LOCAL);

		/// Rotate the node around an arbitrary axis.
		/**
		* The unit of angle is degree.
		* @param radian the rolling angle represented by degree.
		*/
		inline void rotated(const Vector3& axis, double degree, TransformSpace relativeTo = TS_LOCAL) 
		{
			rotate(axis, Math::DegreeToRadian(degree), relativeTo);
		}

		/// Rotate the node around an arbitrary axis using a Quaternion.
		virtual void rotate(const Quaternion& q, TransformSpace relativeTo = TS_LOCAL);

		/// Gets a matrix whose columns are the local axes based on the nodes orientation relative to it's parent.
		virtual Matrix3 getLocalAxes(void) const;

		///// Creates an unnamed new Node as a child of this node.
		///** @param
		// * translate Initial translation offset of child relative to parent
		// * @param
		// * rotate Initial rotation relative to parent
		//*/
		//virtual Node* createChild(const Vector3& translate = Vector3::ZERO,	const Quaternion& rotate = Quaternion::IDENTITY );

		///// Creates a new named Node as a child of this node.
		///**
		// * @remarks
		// * This creates a child node with a given name, which allows you to look the node up from 
		// * the parent which holds this collection of nodes.
		// * @param
		// * translate Initial translation offset of child relative to parent
		// * @param
		// * rotate Initial rotation relative to parent
		//*/
		//virtual Node* createChild(const QString& name, const Vector3& translate = Vector3::ZERO, const Quaternion& rotate = Quaternion::IDENTITY);

		/// Adds a (precreated) child scene node to this node.
		/**
		 * @param child The Node which is to become a child node of this one
		*/
		virtual void addChild(Node* pChild);

		/// Reports the number of child nodes under this one.
		virtual unsigned short getChildrenNum(void) const;    

		/// Gets a pointer to a child node by it's ID.
		virtual Node* getChild(unsigned long nID);

		/// Drops the specified child from this node. 
		/**
		 * @remarks
		 * Does not delete the node, just detaches it from
		 * this parent, potentially to be reattached elsewhere. 
		 * There is also an alternate version which drops a named
		 * child from this node.
		*/
		virtual Node* removeChild(Node* pChild);

		/// Drops the child from this node by its ID. 
		/** 
		 * @remarks
		 * Does not delete the node, just detaches it from
		 * this parent, potentially to be reattached elsewhere.
		*/
		virtual Node* removeChild(unsigned long nID);

		/// Removes all child Nodes attached to this node.
		/** 
		 * Does not delete the nodes, just detaches them from
		 * this parent, potentially to be reattached elsewhere.
		*/
		virtual void removeAllChildren(void);

		/// Detach the specified child from this node and delete it. 
		virtual void deleteChild(Node* pChild);

		/// Detach the specified child from this node and delete it by its ID. 
		virtual void deleteChild(unsigned long nID);

		/// Detach all child node and delete them.
		virtual void deleteAllChildren(void);

		/// Attach a scene object to this node.
		void attachSceneObject(SceneObject* pSceneObject);

		/// Get the object count.
		int getObjectCount() const;

		/// Detach the attached scene object.
		/**
		* @param nIndex The index of the SceneObject
		* @return The pointer to the SceneObject.
		*/
		SceneObject* detachSceneObject(int nIndex);

		/// Detach all the attached scene objects.
		void detachAllSceneObjects();

		/// Return the attached object.
		/**
		 * @param nIndex The index of the scene object
		 * @return The pointer to the scene object.
		*/
		SceneObject* getSceneObject(int nIndex);

		/// Gets the orientation of the node as derived from all parents.
		virtual const Quaternion& _getDerivedOrientation(void) const;

		/// Gets the position of the node as derived from all parents.
		virtual const Vector3& _getDerivedPosition(void) const;

		/// Gets the scaling factor of the node as derived from all parents.
		virtual const Vector3& _getDerivedScale(void) const;

		/// Gets the full transformation matrix for this node.
		/**
		 * @remarks
		 * This method returns the full transformation matrix
		 * for this node, including the effect of any parent node
		 * This should only be called when knowing the derived
		 * transforms have been updated before calling this method.
		*/
		virtual Matrix4 _getFullTransform(void) const;

		//const MaterialPtr& getMaterial(void) const;

		//void getRenderOperation(RenderOperation& op);

		/// Gets the world transform matrix / matrices for this node.
		virtual void getAbsoluteTransforms(Matrix4* xform) const;

		///Gets the worldspace orientation of this node.
		virtual const Quaternion& getAbsoluteOrientation(void) const;

		/// Gets the worldspace position of this node. 
		virtual const Vector3& getAbsolutePosition(void) const;

		/// Sets the worldspace position of this node
		virtual void setAbsolutePosition(const Vector3 &pos);

		/// Sets the worldspace orientation of this node
		virtual void setAbsoluteOrientation(const Quaternion& q);


		//** Internal weighted transform method.
		//@remarks
		//This method transforms a Node by a weighted amount from it's
		//initial state. If weighted transforms have already been applied, 
		//the previous transforms and this one are blended together based
		//on their relative weight. This method should not be used in
		//combination with the unweighted rotate, translate etc methods.
		//*/
		//virtual void _weightedTransform(double weight, const Vector3& translate, 
		//	const Quaternion& rotate, const Vector3& scale);

		//** Overridden, see Renderable */
		//double getSquaredViewDepth(const Camera* cam) const;
	};  //class Node
}   //namespace Swift

#endif  //_Node_H_