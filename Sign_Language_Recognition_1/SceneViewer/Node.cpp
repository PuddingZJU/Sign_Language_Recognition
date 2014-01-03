#include "Node.h"
#include "SceneManager.h"
#include <cassert>

using namespace Swift;

Node::Node(AbstractSceneManager * p_mgr)
{
	assert(NULL != p_mgr);
	m_pSceneManager = p_mgr;
	m_nID = IDManager::instance().getID(ID_Node);
	// Register the ID-pointer pair to the SceneManager.
	m_pSceneManager ->insertIDTraceable(m_nID,this);
	m_pParent = NULL;
	m_qOrientation = m_qDerivedOrientation = Quaternion::IDENTITY;
	m_vPosition = m_vDerivedPosition = Vector3::ZERO;
	m_vScale = m_vDerivedScale = Vector3::UNIT_SCALE;
	m_bInheritScale = false;
	m_bBoneNode = false;
	//m_pSceneObject = NULL;

	// Generate a name
	//QTextStream(&m_strName) << "UnnamedNode_" << m_nUnnamedIndex++;
	m_strName = "UnnamedNode";

	needUpdate();
}

Node::Node(const QString& strName, AbstractSceneManager * p_mgr)
{
	assert(NULL != p_mgr);
	m_pSceneManager = p_mgr;
	m_nID = IDManager::instance().getID(ID_Node);
	// Register the ID-pointer pair to the SceneManager.
	m_pSceneManager ->insertIDTraceable(m_nID,this);
	m_pParent = NULL;
	m_qOrientation = m_qDerivedOrientation = Quaternion::IDENTITY;
	m_vPosition = m_vDerivedPosition = Vector3::ZERO;
	m_vScale = m_vDerivedScale = Vector3::UNIT_SCALE;
	m_bInheritScale = false;
	m_bBoneNode = false;
	m_strName = strName;
	//m_pSceneObject = NULL;

	needUpdate();
}

Node::~Node()
{
	// Remove the ID-pointer pair from the SceneManager.
	m_pSceneManager ->removeIDTraceable(m_nID);
	m_pSceneManager = NULL;
	//if (m_pSceneObject)
	//{
	//	SceneManager::instance().getRenderQueue()->deleteSceneObject(m_pSceneObject,RENDER_QUEUE_MAIN);
	//	delete m_pSceneObject;
	//}
	for (int i=0; i<m_vSceneObject.size(); i++)
	{
		if (m_vSceneObject.at(i))
		{
			//SceneManager::instance().getRenderQueue()->deleteSceneObject(m_vSceneObject.at(i),RENDER_QUEUE_MAIN);
			//delete m_vSceneObject.at(i);
			m_vSceneObject.at(i)->setValidation(false);
		}
	}
	detachAllSceneObjects();
	m_vSceneObject.clear();
}

void Node::makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation, Matrix4& destMatrix) const
{
	destMatrix = Matrix4::IDENTITY;
	// Ordering:
	//    1. Scale
	//    2. Rotate
	//    3. Translate

	// Parent scaling is already applied to derived position
	// Own scale is applied before rotation
	Matrix3 rot3x3, scale3x3;
	orientation.toRotationMatrix(rot3x3);
	scale3x3 = Matrix3::ZERO;
	scale3x3[0] = scale.x();
	scale3x3[4] = scale.y();
	scale3x3[8] = scale.z();

	destMatrix = rot3x3 * scale3x3;
	destMatrix.setTrans(position);
}

void Node::makeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation, Matrix4& destMatrix)
{
	destMatrix = Matrix4::IDENTITY;

	// Invert the parameters
	Vector3 invTranslate = position.negative();
	Vector3 invScale;
	invScale.set_x(1/scale.x());
	invScale.set_y(1/scale.y());
	invScale.set_z(1/scale.z());

	Quaternion invRot = Inverse(orientation);

	// Because we're inverting, order is translation, rotation, scale
	// So make translation relative to scale & rotation
	invTranslate.set_x(invTranslate.x() * invScale.x()); // scale
	invTranslate.set_y(invTranslate.y() * invScale.y()); // scale
	invTranslate.set_z(invTranslate.z() * invScale.z()); // scale
	invTranslate = invRot * invTranslate; // rotate

	// Next, make a 3x3 rotation matrix and apply inverse scale
	Matrix3 rot3x3, scale3x3;
	invRot.toRotationMatrix(rot3x3);
	scale3x3 = Matrix3::ZERO;
	scale3x3[0] = invScale.x();
	scale3x3[4] = invScale.y();
	scale3x3[8] = invScale.z();

	// Set up final matrix with scale & rotation
	destMatrix = scale3x3 * rot3x3;

	destMatrix.setTrans(invTranslate);
}

//void Node::_decreaseNameNum(Node* pNode)
//{
//	QString strName = pNode->getName();
//
//}

void Node::_updateFromParent(void) const
{
	if (m_pParent)
	{
		if (!m_bNeedUpdate && !m_pParent->m_bNeedUpdate)
		{
			return;
		}
		// Combine orientation with that of parent
		Quaternion qParent = m_pParent->_getDerivedOrientation();
		m_qDerivedOrientation = qParent * m_qOrientation;

		// Change position vector based on parent's orientation & scale
		m_vDerivedPosition = qParent * (m_vPosition.scaling(m_pParent->_getDerivedScale()));

		// Update scale
		if (m_bInheritScale)
		{
			// Scale own position by parent scale
			Vector3 vParentScale = m_pParent->_getDerivedScale();
			// Set own scale, NB just combine as equivalent axes, no shearing
			m_vDerivedScale = m_vScale.scaling(vParentScale);
		}
		else
		{
			// No inheritence
			m_vDerivedScale = m_vScale;
		}

		// Add altered position vector to parents
		m_vDerivedPosition += m_pParent->_getDerivedPosition();
	}
	else
	{
		// Root node, no parent
		if (!m_bNeedUpdate)
		{
			return;
		}
		m_qDerivedOrientation = m_qOrientation;
		m_vDerivedPosition = m_vPosition;
		m_vDerivedScale = m_vScale;
	}

	m_bNeedUpdate = false;

	for(ChildrenMap::iterator it=m_mChildren.begin(); it!=m_mChildren.end(); it++)
	{
		it->second->m_bNeedUpdate = true;
	}
	//??
	//m_bCachedTransformOutOfDate = true;
}

Node* Node::getParent() const
{
	return m_pParent;
}

void Node::setParent(Node* parent)
{
	m_pParent = parent;
}

bool Node::isBoneNode() const
{
	return m_bBoneNode;
}

void Node::needUpdate()
{
	m_bCachedTransformOutOfDate = true;
	m_bNeedUpdate = true;
}

const Quaternion& Node::getOrientation() const
{
	return m_qOrientation;
}

void Node::setOrientation(double w, double x, double y, double z)
{
	m_qOrientation.set_x(x);
	m_qOrientation.set_y(y);
	m_qOrientation.set_z(z);
	m_qOrientation.set_w(w);

	needUpdate();
}

void Node::setOrientation(const Quaternion& q)
{
	m_qOrientation = q;

	needUpdate();
}

void Node::resetOrientation(void)
{
	m_qOrientation = Quaternion::IDENTITY;
	needUpdate();
}

Euler& Node::getRotation()
{
	return m_eRotation;
}

void Node::setRotation(Euler eRotation)
{
	m_eRotation = eRotation;
}

const Vector3& Node::getPosition() const
{
	return m_vPosition;
}

void Node::setPosition(double x, double y, double z)
{
	m_vPosition.set_values(x,y,z);
	needUpdate();
}

void Node::setPosition(const Vector3& pos)
{
	m_vPosition = pos;
	needUpdate();
}

const Vector3& Node::getScale() const
{
	return m_vScale;
}

void Node::setScale(double x, double y, double z)
{
	m_vScale.set_values(x,y,z);
	needUpdate();
}

void Node::setScale(const Vector3& vScale)
{
	m_vScale = vScale;
}

void Node::setInheritScale(bool bInherit)
{
	m_bInheritScale = bInherit;
	needUpdate();
}

bool Node::getInheritScale(void) const
{
	return m_bInheritScale;
}

void Node::scale(const Vector3& scale)
{
	m_vScale.scaling(scale);
	needUpdate();
}

void Node::scale(double x, double y, double z)
{
	m_vScale.set_x(m_vScale.x() * x);
	m_vScale.set_y(m_vScale.y() * y);
	m_vScale.set_z(m_vScale.z() * z);
	needUpdate();
}

void Node::translate(const Vector3& d, TransformSpace relativeTo /* = TS_PARENT */)
{
	Vector3 adjusted;
	switch(relativeTo) 
	{
	case TS_LOCAL:
		// position is relative to parent so transform downwards
		m_vPosition += m_qOrientation * d;
		break;
	case TS_WORLD:
		// position is relative to parent so transform upwards
		if (m_pParent)
		{
			m_vPosition += Inverse(m_pParent->_getDerivedOrientation()) * d; 
		}
		else
		{
			m_vPosition += d;
		}
		break;
	case TS_PARENT:
		m_vPosition += d;
		break;
	}
	needUpdate();
}

void Node::translate(double x, double y, double z, TransformSpace relativeTo /* = TS_PARENT */)
{
	Vector3 v(x,y,z);
	translate(v, relativeTo);
}

void Node::translate(const Matrix3& axes, const Vector3& move, TransformSpace relativeTo /* = TS_PARENT */)
{
	Vector3 derived = axes * move;
	translate(derived, relativeTo);
}

void Node::translate(const Matrix3& axes, double x, double y, double z, TransformSpace relativeTo /* = TS_PARENT */)
{
	Vector3 d(x,y,z);
	translate(axes,d,relativeTo);
}

void Node::roll(double radian, TransformSpace relativeTo /* = TS_LOCAL */)
{
	rotate(Vector3::UNIT_Z, radian, relativeTo);
}

void Node::pitch(double radian, TransformSpace relativeTo /* = TS_LOCAL */)
{
	rotate(Vector3::UNIT_X, radian, relativeTo);
}

void Node::yaw(double radian, TransformSpace relativeTo /* = TS_LOCAL */)
{
	rotate(Vector3::UNIT_Y, radian, relativeTo);
}

void Node::rotate(const Vector3& axis, double radian, TransformSpace relativeTo /* = TS_LOCAL */)
{
	Quaternion q;
	q.fromAngleAxis(radian,axis);
	rotate(q, relativeTo);
}

void Node::rotate(const Quaternion& q, TransformSpace relativeTo /* = TS_LOCAL */)
{
	switch(relativeTo) 
	{
	case TS_PARENT:
		// Rotations are normally relative to local axes, transform up
		m_qOrientation = q * m_qOrientation;
		break;
	case TS_WORLD:
		// Rotations are normally relative to local axes, transform up
		m_qOrientation = m_qOrientation * Inverse(_getDerivedOrientation()) 
			* q * _getDerivedOrientation();
		break;
	case TS_LOCAL:
		// Note the order of the mult, i.e. q comes after
		m_qOrientation = m_qOrientation * q;
		break;
	}
	needUpdate();
}

Matrix3 Node::getLocalAxes() const
{
	Vector3 axisX = Vector3::UNIT_X;
	Vector3 axisY = Vector3::UNIT_Y;
	Vector3 axisZ = Vector3::UNIT_Z;

	axisX = m_qOrientation * axisX;
	axisY = m_qOrientation * axisY;
	axisZ = m_qOrientation * axisZ;

	return Matrix3(axisX.x(), axisY.x(), axisZ.x(),
		axisX.y(), axisY.y(), axisZ.y(),
		axisX.z(), axisY.z(), axisZ.z());
}

//Node* Node::createChild(const Vector3& translate, const Quaternion& rotate)
//{
//	Node* newNode = createChildImpl();
//	newNode->translate(translate);
//	newNode->rotate(rotate);
//	this->addChild(newNode);
//
//	return newNode;
//}
//
//Node* Node::createChild(const QString& name, const Vector3& translate, const Quaternion& rotate)
//{
//	Node* newNode = createChildImpl(name);
//	newNode->translate(translate);
//	newNode->rotate(rotate);
//	this->addChild(newNode);
//
//	return newNode;
//}

void Node::addChild(Node* pChild)
{
	if (!pChild)
	{
		return;
	}
	//ChildrenNameNumMap::iterator it = m_mNameNum.find(pChild->getName());
	//if (it == m_mNameNum.end())
	//{
	//	m_mNameNum.insert(NameNumPair(pChild->getName(),1));
	//}
	//else
	//{
	//	QString strName;
	//	QTextStream(&strName) << pChild->getName() << "_" << it->second;
	//	pChild->setName(strName);
	//	it->second++;
	//}
	m_mChildren.insert(IDNodePair(pChild->getID(),pChild));
	pChild->setParent(this);
}

unsigned short Node::getChildrenNum() const
{
	return static_cast<unsigned short>(m_mChildren.size());
}

Node* Node::getChild(unsigned long nID)
{
	ChildrenMap::iterator it = m_mChildren.find(nID);
	if (it != m_mChildren.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

Node* Node::removeChild(Node* pChild)
{
	if (!pChild)
	{
		return NULL;
	}
	ChildrenMap::iterator it = m_mChildren.begin();
	while (it != m_mChildren.end())
	{
		if (it->second == pChild)
		{
			m_mChildren.erase(it);
			pChild->setParent(NULL);
			break;
		}
		it++;
	}

	return pChild;
}

Node* Node::removeChild(unsigned long nID)
{
	ChildrenMap::iterator it = m_mChildren.find(nID);
	Node* pNode=NULL;
	if (it != m_mChildren.end())
	{
		pNode = it->second;
		m_mChildren.erase(it);
		pNode->setParent(NULL);
	}

	return pNode;
}

void Node::removeAllChildren()
{
	ChildrenMap::iterator it;
	for (it=m_mChildren.begin(); it!= m_mChildren.end(); it++)
	{
		it->second->setParent(NULL);
	}
	m_mChildren.clear();
}

void Node::deleteChild(Node* pChild)
{
	removeChild(pChild);
	//for (ChildrenMap::iterator it=pChild->m_mChildren.begin(); it<pChild->getChildrenNum(); it++)
	//{
	//	deleteChild(it->second);
	//}
	pChild->deleteAllChildren();
	delete pChild;
	pChild = NULL;
}

void Node::deleteChild(unsigned long nID)
{
	Node* pChild = removeChild(nID);
	//for (ChildrenMap::iterator it=pChild->m_mChildren.begin(); it<pChild->getChildrenNum(); it++)
	//{
	//	deleteChild(it->first);
	//}
	pChild->deleteAllChildren();
	delete pChild;
	pChild = NULL;
}

void Node::deleteAllChildren()
{
	for (ChildrenMap::iterator it=m_mChildren.begin(); it!= m_mChildren.end(); it++)
	{
		//deleteChild(it->first);
		it->second->deleteAllChildren();
		delete it->second;
		it->second = NULL;
	}
	if(m_mChildren.size() > 0)
	{
		m_mChildren.clear();
	}
}

void Node::attachSceneObject(SceneObject* pSceneObject)
{
	//m_pSceneObject = pSceneObject;
	m_vSceneObject.push_back(pSceneObject);
	pSceneObject->attachToNode(this);
	pSceneObject->setIndex(m_vSceneObject.size()-1);
}

SceneObject* Node::detachSceneObject(int nIndex)
{
	if (nIndex>=0 && nIndex <m_vSceneObject.size())
	{
		SceneObject* pSceneObject = m_vSceneObject.at(nIndex);
		m_vSceneObject.at(nIndex)->setIndex(-1);
		m_vSceneObject.at(nIndex)->detachFromNode();
		for(int i=nIndex+1; i<m_vSceneObject.size(); i++)
		{
			m_vSceneObject.at(i)->setIndex(m_vSceneObject.at(i)->getIndex()-1);
		}
		m_vSceneObject.erase(m_vSceneObject.begin()+nIndex);
		return pSceneObject;
	}
	else
	{
		return NULL;
	}
}

void Node::detachAllSceneObjects()
{
	for (int i=0; i<m_vSceneObject.size(); i++)
	{
		m_vSceneObject.at(i)->detachFromNode();
	}
	m_vSceneObject.clear();
}

SceneObject* Node::getSceneObject(int nIndex)
{
	if (nIndex>=0 && nIndex <m_vSceneObject.size())
	{
		return m_vSceneObject.at(nIndex);
	}
	else
	{
		return NULL;
	}
}

int Node::getObjectCount() const
{
	return m_vSceneObject.size();
}

const Quaternion& Node::_getDerivedOrientation() const
{
	_updateFromParent();
	return m_qDerivedOrientation;
}

const Vector3& Node::_getDerivedPosition() const
{
	_updateFromParent();
	return m_vDerivedPosition;
}

const Vector3& Node::_getDerivedScale() const
{
	_updateFromParent();
	return m_vDerivedScale;
}

Matrix4 Node::_getFullTransform() const
{
	if (m_bCachedTransformOutOfDate)
	{
		// Use derived values 
		makeTransform( 
			_getDerivedPosition(), _getDerivedScale(), 
			_getDerivedOrientation(), m_mCachedTransform);
		m_bCachedTransformOutOfDate = false;
	}
	return m_mCachedTransform;
}

void Node::getAbsoluteTransforms(Matrix4* xform) const
{
	*xform = this->_getFullTransform();
}

const Quaternion& Node::getAbsoluteOrientation() const
{
	return _getDerivedOrientation();
}

const Vector3& Node::getAbsolutePosition() const
{
	return _getDerivedPosition();
}

void Node::setAbsolutePosition(const Vector3 &pos){
	m_vDerivedPosition = pos;
}

void Node::setAbsoluteOrientation(const Quaternion &q){
	m_qDerivedOrientation = q;
}