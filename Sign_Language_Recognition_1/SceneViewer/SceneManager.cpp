#include "SceneManager.h"
#include "Skeleton.h"

namespace Swift
{
	SceneManager::SceneManager()
	{
	}

	SceneManager::~SceneManager()
	{
	}

	// scene manager for the preview dialog.
	PreviewSceneManager::PreviewSceneManager()
	{
		m_pSkeleton = NULL;
	}

	PreviewSceneManager::~PreviewSceneManager()
	{
		if (m_pSkeleton)
		{
			delete m_pSkeleton;
		}
	}

	BoneNode* PreviewSceneManager::createSkeleton(MotionClip* pMotionClip, bool bDelOldMotionClip/* = true*/, SimpleObjectType sot/*=SOT_SPHERE*/)
	{
		if (!pMotionClip)
		{
			return NULL;
		}
		if (m_pSkeleton)
		{
			delete m_pSkeleton;
		}
		m_pSkeleton = new Skeleton(pMotionClip, bDelOldMotionClip);
		BoneNode* pRoot = m_pSkeleton->createSkeleton();
		//if (m_nMaxFrameCount < pMotionClip->getFrameCount())
		//{
			m_nMaxFrameCount = pMotionClip->getFrameCount();
		//}

		if (m_nCurrentFrameNo >= m_nMaxFrameCount)
		{
			m_nCurrentFrameNo = m_nMaxFrameCount-1;
		}
		m_pSkeleton->updateDataFromMotionClip(m_nCurrentFrameNo);

		m_pRootSceneNode->addChild(pRoot);
		m_pSkeleton->attachJointObjects(sot);
		addObjectToRenderQueue(pRoot);

		return pRoot;
	}

	void PreviewSceneManager::updateDataFromMotionClip(int nFrameNo)
	{
		if(m_pSkeleton && nFrameNo<m_pSkeleton->getMotionClip()->getFrameCount())
			m_pSkeleton->updateDataFromMotionClip(nFrameNo);
	}

	void PreviewSceneManager::renderBones()
	{
		glColor4f(0.2f,0.2f,0.2f,1.0f);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		if (m_pSkeleton && m_pSkeleton->getViewState() != Invisible)
		{
			BoneNode* pBoneNode = m_pSkeleton->getRootBoneNode();
			AbstractSceneManager::renderBones(pBoneNode);
		}
		glEnd();
	}

}//Swift
