#include "Skeleton.h"
#include "SceneManager.h"
#include "Ellipse.h"
using namespace Swift;

Skeleton::Skeleton(AbstractSceneManager * p_mgr)
{
	m_pMotionClip = NULL;
	m_pRootBoneNode = NULL;
	m_state = Active;
	m_nType = 0;
	m_pSceneManager = p_mgr;
}

Skeleton::Skeleton(MotionClip* pMotionClip, bool bDelMotionClip /* = true*/, AbstractSceneManager * p_mgr)
{
	m_pMotionClip = pMotionClip;
	m_pRootBoneNode = NULL;
	m_state = Active;
	m_nType = 0;
	m_bDelMotionClip = bDelMotionClip;
	m_pSceneManager = p_mgr;
}

Skeleton::~Skeleton()
{
	if (m_pRootBoneNode)
	{
		Node * parent = m_pRootBoneNode->getParent();
		if( parent ){
			parent->deleteChild(m_pRootBoneNode);
		}else{
			m_pRootBoneNode ->deleteAllChildren();
			delete m_pRootBoneNode;
			m_pRootBoneNode = NULL;
		}
	}
	if (m_bDelMotionClip && m_pMotionClip)
	{
		delete m_pMotionClip;
		m_pMotionClip = NULL;
	}
}
std::map<int, BoneNode*> Skeleton::GetNodeMap()
{
	return m_mBoneNodeSet;
}
BoneNode* Skeleton::createSkeleton()
{
	if (!m_pMotionClip)
	{
		return NULL;
	}
	
	m_pRootBoneNode = createSkeletonRecursively(m_pMotionClip->getRoot());

	return m_pRootBoneNode;
}

BoneNode* Skeleton::createSkeletonRecursively(MotionJoint* pJoint)
{
	std::string strName = pJoint->getName();
	int nIndex = pJoint->getIndex();
	BoneNode* pBoneNode = new BoneNode(QString::fromLocal8Bit(strName.c_str()), m_pSceneManager);
	pBoneNode->setMotionJoint(pJoint);
	pBoneNode->setSkeleton(this);
	pBoneNode->setIndex(nIndex);
	m_mBoneNodeSet.insert(std::pair<int, BoneNode*>(nIndex,pBoneNode));
	std::map<int, BoneNode*>::iterator it=m_mBoneNodeSet.end();
	if (m_pMotionClip->getParent(pJoint))
	{
		it = m_mBoneNodeSet.find(m_pMotionClip->getParent(pJoint)->getIndex());
	}
	if (it!=m_mBoneNodeSet.end())
	{
		it->second->addChild(pBoneNode);
	}
	for (int i=0; i<m_pMotionClip->getChildrenCount(pJoint); i++)
	{
		createSkeletonRecursively(m_pMotionClip->getChild(pJoint,i));
	}

	return pBoneNode;
}

MotionClip* Skeleton::getMotionClip()
{
	return m_pMotionClip;
}

void Skeleton::setMotionClip(MotionClip* pMotionClip)
{
	m_pMotionClip = pMotionClip;
}

void Skeleton::attachJointObjects(SimpleObjectType sot /* = SOT_SPHERE */)
{
	SceneObject* pSceneObject;
	for(BoneNodeMap::iterator it=m_mBoneNodeSet.begin(); it!=m_mBoneNodeSet.end(); it++)
	{
		switch(sot)
		{
		case SOT_SPHERE:
			pSceneObject = new Sphere(1.0, m_pSceneManager);
			break;
		case SOT_CUBE:
			pSceneObject = new Cube(2.0, 2.0, 2.0, m_pSceneManager);
			break;
		case SOT_ELLIPSE:
			pSceneObject = new Ellipse(4.0, m_pSceneManager);
			break;
		}
		it->second->attachSceneObject(pSceneObject);
	}
}

void Skeleton::updateDataFromMotionClip(int nFrameNo)
{
	if (m_state == Active)
	{
		for(BoneNodeMap::iterator it=m_mBoneNodeSet.begin(); it!=m_mBoneNodeSet.end(); it++)
		{
			//MotionJoint* pJoint = m_pMotionClip->findJoint(it->first);
			MotionJoint* pJoint = it->second->getMotionJoint();
			if(pJoint)
			{
				it->second->setAbsolutePosition(pJoint->getAbsolutePosition(nFrameNo));
				it->second->setPosition(pJoint->getRelativePosition(nFrameNo));
				it->second->setAbsoluteOrientation(pJoint->_getInheritOrientation(nFrameNo));
				it->second->setOrientation(pJoint->getOrientation(nFrameNo));
				it->second->setRotation(Euler(pJoint->getXRotation(nFrameNo),pJoint->getYRotation(nFrameNo),pJoint->getZRotation(nFrameNo),pJoint->getEulerAnglesOrder()));
				//it->second->setAbsoluteScale(m_pMotionClip->findJoint(it->first)->getAbsoluteScale(nFrameNo));
			}
		}
	}
}

BoneNode* Skeleton::getRootBoneNode() const
{
	return m_pRootBoneNode;
}

void Skeleton::setSelectedJoint(const QString& sJointName)
{
	int nIndex = m_pMotionClip->getJointIndexByName(sJointName.toStdString());
	BoneNodeMap::iterator it = m_mBoneNodeSet.find(nIndex);
	if (it != m_mBoneNodeSet.end())
	{
		SceneObject* pSceneObject = it->second->getSceneObject(0);
		if (pSceneObject)
		{
			//pSceneObject->setPicked(true);
			if(IDManager::instance().getIDType(pSceneObject->getID()) == ID_Entity)
			{
				m_pSceneManager ->addPickedObject(pSceneObject);
			}
		}
	}
}

void Skeleton::setViewState(MFileState state)
{
	m_state = state;
	if (state == Invisible)
	{
		setJointsVisiable(false);
	}
	else
	{
		setJointsVisiable(true);
	}
}

MFileState Skeleton::getViewState() const
{
	return m_state;
}

void Skeleton::setJointsVisiable(bool bVisible)
{
	for (BoneNodeMap::iterator it=m_mBoneNodeSet.begin(); it!=m_mBoneNodeSet.end(); it++)
	{
		SceneObject* pObject = it->second->getSceneObject(0);
		if (pObject)
		{
			pObject->setVisible(bVisible);
		}
	}
}

int Skeleton::getSkeletonType()
{
	return m_nType;
}

void Skeleton::setSkeletonType(int nType)
{
	m_nType = nType;
}

void Skeleton::setDelMotionClip(bool to_del)
{
	m_bDelMotionClip = to_del;
}