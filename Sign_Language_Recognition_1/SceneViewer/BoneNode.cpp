#include "BoneNode.h"
#include "Skeleton.h"
using namespace Swift;

BoneNode::BoneNode(const QString& strName /* =  */, AbstractSceneManager * p_mgr /* = NULL*/)
: Node( (NULL == p_mgr) ? (&SceneManager::instance()) : (p_mgr) )
{
	if (strName != "")
	{
		m_strName = strName;
	}
	m_bBoneNode = true;
	m_pSkeleton = NULL;
	m_pMotionJoint = NULL;
}

BoneNode::~BoneNode()
{

}

Skeleton* BoneNode::getSkeleton()
{
	return m_pSkeleton;
}

void BoneNode::setSkeleton(Skeleton* pSkeleton)
{
	m_pSkeleton = pSkeleton;
}

const Vector3& BoneNode::getAbsolutePosition() const
{
	return m_vDerivedPosition;
}

void BoneNode::setAbsolutePosition(Vector3 vPosition)
{
	m_vDerivedPosition = vPosition /*+ m_pSkeleton->getRootBoneNode()->getAbsolutePosition()*/;
}

const Quaternion& BoneNode::getAbsoluteOrientation() const
{
	return m_qDerivedOrientation;
}

void BoneNode::setAbsoluteOrientation(Quaternion qOrientation)
{
	m_qDerivedOrientation = qOrientation;
}

int BoneNode::getIndex()
{
	return m_nIndex;
}

void BoneNode::setIndex(int nIndex)
{
	m_nIndex = nIndex;
}

MotionJoint* BoneNode::getMotionJoint() const
{
	return m_pMotionJoint;
}

void BoneNode::setMotionJoint(MotionJoint* pMotionJoint)
{
	m_pMotionJoint = pMotionJoint;
}