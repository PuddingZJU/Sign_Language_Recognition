#include "AbstractSceneManager.h"
#include "Skeleton.h"
using namespace Swift;

AbstractSceneManager::AbstractSceneManager()
{
	m_pRootSceneNode = NULL;
	m_pRenderQueue = new RenderQueue();
	//m_pGround = NULL;
	m_nMaxFrameCount = 0;
	m_nCurrentFrameNo = 0;
	m_pLastestXMTemplate = NULL;
	m_pLastestXMSkeleton = NULL;
	m_pCapturingMotionClip = NULL;
	m_pCapturingSkeleton = NULL;
	m_nCurrentMotionIndex = 0;
}

AbstractSceneManager::~AbstractSceneManager()
{
	for (size_t i=0; i<m_vSkeleton.size(); i++)
	{
		delete m_vSkeleton.at(i);
	}
	m_vSkeleton.clear();


	if (m_pRenderQueue)
	{
		delete m_pRenderQueue;
		m_pRenderQueue = NULL;
	}

	//if (m_pGround)
	//{
	//	delete m_pGround;
	//}

	if (m_pRootSceneNode)
	{
		m_pRootSceneNode->deleteAllChildren();
		//delete m_pRootSceneNode;
	}
}

BoneNode* AbstractSceneManager::createSkeleton(MotionClip* pMotionClip,int nType/*=1*/, SimpleObjectType sot/*=SOT_SPHER*/)
{
	if (!pMotionClip)
	{
		return NULL;
	}
	if (m_nMaxFrameCount < pMotionClip->getFrameCount())
	{
		m_nMaxFrameCount = pMotionClip->getFrameCount();
	}
	Skeleton* pSkeleton = new Skeleton(pMotionClip, true, this);
	if (!pSkeleton)
	{
		return NULL;
	}
	BoneNode* pRoot = pSkeleton->createSkeleton();
	if (m_nCurrentFrameNo > m_nMaxFrameCount-1)
	{
		m_nCurrentFrameNo = m_nMaxFrameCount-1;
	}
	if (m_nCurrentFrameNo < pMotionClip->getFrameCount())
	{
		pSkeleton->updateDataFromMotionClip(m_nCurrentFrameNo);
	}
	else
	{
		pSkeleton->updateDataFromMotionClip(pMotionClip->getFrameCount()-1);
	}
	pSkeleton->setSkeletonType(nType);
	m_vSkeleton.push_back(pSkeleton);
	if (nType == 0)
	{
		m_pLastestXMTemplate = pMotionClip;
		m_pLastestXMSkeleton = pSkeleton;
	}
	else if (nType == 2)
	{
		m_pCapturingMotionClip = pMotionClip;
		m_pCapturingSkeleton = pSkeleton;
	}

	m_pRootSceneNode->addChild(pRoot);
	pSkeleton->attachJointObjects(sot);
	addObjectToRenderQueue(pRoot);
	
	int nIndex = m_vSkeleton.size()-1;
	emit importNewMotion(nType,nIndex);
	return pRoot;
}

SceneNode* AbstractSceneManager::createSceneNode(const QString& strName/* = */, const Vector3& vTranslate/* =Vector3::ZERO */, const Quaternion& qRotate/* =Quaternion::IDENTITY */)
{
	SceneNode* pSceneNode = new SceneNode(strName, this);

	pSceneNode->translate(vTranslate);
	pSceneNode->rotate(qRotate);

	return pSceneNode;
}

SceneNode* AbstractSceneManager::createRootSceneNode(const QString& strName/* = "RootSceneNode"*/, const Vector3& vTranslate/* =Vector3::ZERO */, const Quaternion& qRotate/* =Quaternion::IDENTITY */)
{
	m_pRootSceneNode = createSceneNode(strName,vTranslate,qRotate);

	return m_pRootSceneNode;
}

//Ground* AbstractSceneManager::createGround()
//{
//	m_pGround = new Ground(this);
//	m_pRenderQueue->addSceneObject(m_pGround, RENDER_QUEUE_BACKGROUND);
//
//	return m_pGround;
//}
//
//Ground* AbstractSceneManager::getGround()
//{
//	return m_pGround;
//}

SceneNode* AbstractSceneManager::getRootSceneNode() const
{
	return m_pRootSceneNode;
}

void AbstractSceneManager::deleteSceneNode(SceneNode* pSceneNode)
{
	pSceneNode->deleteAllChildren();
	delete pSceneNode;
}

void AbstractSceneManager::deleteSceneGraph()
{
	deleteSceneNode(m_pRootSceneNode);
}

void AbstractSceneManager::attachAllSkeletonObjects(SimpleObjectType sot /* = SOT_SPHERE */)
{
	for (size_t i=0; i<m_vSkeleton.size(); i++)
	{
		m_vSkeleton.at(i)->attachJointObjects(sot);
	}
}

void AbstractSceneManager::updateDataFromMotionClip(int nFrameNo)
{
	for (size_t i=0; i<m_vSkeleton.size(); i++)
	{
		if(nFrameNo<m_vSkeleton.at(i)->getMotionClip()->getFrameCount())
		{
			m_vSkeleton.at(i)->updateDataFromMotionClip(nFrameNo);
		}
	}
}

void AbstractSceneManager::updateDataFromXMMotionClip(int nFrameNo)
{
	for (size_t i=0; i<m_vSkeleton.size(); i++)
	{
		if(nFrameNo<m_vSkeleton.at(i)->getMotionClip()->getFrameCount() && m_vSkeleton.at(i)->getSkeletonType() != 1)
			m_vSkeleton.at(i)->updateDataFromMotionClip(nFrameNo);
	}
}

void AbstractSceneManager::addObjectToRenderQueue(SceneObject* pSceneObject)
{
	assert( NULL != pSceneObject );
	if(NULL == pSceneObject){
		return ;
	}

	RenderGroupID nID = pSceneObject->getRenderGroupID();
	m_pRenderQueue->addSceneObject(pSceneObject,nID);
}

void AbstractSceneManager::addObjectToRenderQueue(Node* pNode)
{
	assert( NULL != pNode );
	if( NULL == pNode ){
		return ;
	}

	for (int i=0; i<pNode->getObjectCount(); i++)
	{
		SceneObject* pSceneObject = pNode->getSceneObject(i);
		if(pSceneObject)
		{
			addObjectToRenderQueue(pSceneObject);
		}
	}
	for(std::map<unsigned long,Node*>::iterator it=pNode->m_mChildren.begin(); 
		it!=pNode->m_mChildren.end(); 
		it++)
	{
		addObjectToRenderQueue(it->second);
	}
}

void AbstractSceneManager::removeObjectFromRenderQueue(SceneObject * pSceneObject)
{
	assert( NULL != pSceneObject );
	if( NULL == pSceneObject ){
		return ;
	}

	RenderGroupID nID = pSceneObject ->getRenderGroupID();
	m_pRenderQueue ->deleteSceneObject( pSceneObject, nID);

	return ;
}

void AbstractSceneManager::removeObjectFromRenderQueue(Node * pNode)
{
	assert( NULL != pNode);
	if( NULL == pNode ){
		return ;
	}
	
	for( int i = 0; i < pNode->getObjectCount(); ++i){
		SceneObject * pSceneObject = pNode ->getSceneObject(i);
		if( NULL != pSceneObject ){
			removeObjectFromRenderQueue(pSceneObject);
		}
	}

	for(std::map<unsigned long, Node*>::iterator it = pNode ->m_mChildren.begin();
		it != pNode ->m_mChildren.end();
		++it)
	{
		removeObjectFromRenderQueue( it ->second );
	}

	return ;
}


RenderQueue* AbstractSceneManager::getRenderQueue() const
{
	return m_pRenderQueue;
}

std::vector<SceneObject*> AbstractSceneManager::getPickedList() const{
	return m_vPickedList;
}

void AbstractSceneManager::renderBones()
{
	glColor4f(0.2f,0.2f,0.2f,1.0f);
//	glColor4f(0.988, 0.722, 0.318, 1.0);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		for (size_t i=0; i<m_vSkeleton.size(); i++)
		{
			if (m_vSkeleton.at(i)->getViewState() != Invisible)
			{
				BoneNode* pBoneNode = m_vSkeleton.at(i)->getRootBoneNode();
				renderBones(pBoneNode);
			}
		}
	glEnd();
}

void AbstractSceneManager::renderBones(Node* pNode)
{
	Vector3 vParentPos = pNode->getAbsolutePosition();
	for (std::map<unsigned long,Node*>::iterator it=pNode->m_mChildren.begin(); it!=pNode->m_mChildren.end(); it++)
	{
		// Latest modified by zhangxiang, check if pNode's child is a BoneNode.
		if(it->second->isBoneNode()){
			/*if(it->second->getName() == "RightElbow" ||
				it->second->getName() == "RightWrist"){*/
			Vector3 vChildPos = it->second->getAbsolutePosition();
			glVertex3f(vParentPos.x(),vParentPos.y(),vParentPos.z());
			glVertex3f(vChildPos.x(),vChildPos.y(),vChildPos.z());
		//	}
			renderBones(it->second);
		}else{
			int i;
		}
	}
}

void AbstractSceneManager::insertIDTraceable(int nID,Traceable* pTraceable)
{
	m_mIDTraceableMap.insert(std::pair<int,Traceable*>(nID,pTraceable));
}

void AbstractSceneManager::removeIDTraceable(int nID)
{
	m_mIDTraceableMap.erase(nID);
}

Traceable* AbstractSceneManager::getPointerByID(int nID)
{
	IDTraceableMap::iterator it = m_mIDTraceableMap.find(nID);
	if (it != m_mIDTraceableMap.end())
	{
		return it->second;
	}

	return NULL;
}

int AbstractSceneManager::getMaxFrameCount() const
{
	return m_nMaxFrameCount;
}

int AbstractSceneManager::getCurrentFrameNo() const
{
	return m_nCurrentFrameNo;
}

void AbstractSceneManager::setCurrentFrameNo(int nFrameNo)
{
	m_nCurrentFrameNo = nFrameNo;

	if (m_vPickedList.size() == 1)
	{
		Node* pNode = m_vPickedList.at(0)->m_pNode;
		if (pNode)
		{
			//MotionJointProp mjp;
			//mjp.strName = pNode->getName();
			//mjp.vAbsolutePosition = pNode->getAbsolutePosition();
			//mjp.vRelativePosition = pNode->getPosition();
			//mjp.eRotation = pNode->getRotation();
			//emit pickedOneObject(mjp);	
		}
	}
}

MotionClip* AbstractSceneManager::getLatestMotionClip() const
{
	if (m_vSkeleton.size() > 0)
	{
		return m_vSkeleton.back()->getMotionClip();
	}
	else
	{
		return NULL;
	}
}

void AbstractSceneManager::removeMotion(int nIndex)
{
	if (nIndex>=0 && nIndex<m_vSkeleton.size())
	{
		if(m_vSkeleton.at(nIndex)->getSkeletonType() == 0)
		{
			m_pLastestXMSkeleton = NULL;
			m_pLastestXMTemplate = NULL;
		}
		Skeleton * skeleton = m_vSkeleton.at(nIndex);
		delete skeleton; skeleton = NULL;
		m_vSkeleton.erase(m_vSkeleton.begin()+nIndex);
		m_nMaxFrameCount = 0;
		for (size_t i=0; i<m_vSkeleton.size(); i++)
		{
			if(m_vSkeleton.at(i)->getMotionClip()->getFrameCount()>m_nMaxFrameCount)
				m_nMaxFrameCount = m_vSkeleton.at(i)->getMotionClip()->getFrameCount();
		}
		emit needUpdate();
	}
}

void AbstractSceneManager::removeAllMotions()
{
	for(int index = 0; index < m_vSkeleton.size(); ++index){
		removeMotion(index);
	}

	return ;
}

//void AbstractSceneManager::setSelectedJoint(int nIndex, const QString &sJointName)
//{
//	if (nIndex>=0 && nIndex<m_vSkeleton.size())
//	{
//		m_vSkeleton.at(nIndex)->setSelectedJoint(sJointName);
//		//emit needUpdate();
//	}
//}

//void AbstractSceneManager::onSelectMotion(int nIndex)
//{
//	if (nIndex>=0 && nIndex<m_vSkeleton.size())
//	{
//		m_nCurrentMotionIndex = nIndex;
//		if (m_vSkeleton.at(nIndex))
//		{
//			MotionClip* pMotionClip = m_vSkeleton.at(nIndex)->getMotionClip();
//			if (pMotionClip)
//			{
//				MotionFileProp mfp;
//				mfp.strPath = QString::fromStdString(pMotionClip->getFileName());
//				mfp.fFrameTime = pMotionClip->getFrameTime();
//				mfp.nFrameCount = pMotionClip->getFrameCount();
//				mfp.nJointCount = pMotionClip->getJointCount();
//				mfp.mstate = m_vSkeleton.at(nIndex)->getViewState();
//				mfp.bshowmesh = m_vSkeleton.at(nIndex)->isShowMesh();
//				m_sPickedSkeleton.clear();
//				m_sPickedSkeleton.insert(m_vSkeleton.at(nIndex));
//				emit pickedOneSkeleton(mfp);	
//			}
//		}
//	}
//}

void AbstractSceneManager::addPickedObject(SceneObject* pSceneObject)
{
	if (pSceneObject)
	{
		for (size_t i=0; i<m_vPickedList.size(); i++)
		{
			m_vPickedList.at(i)->setPicked(false);
		}
		m_vPickedList.clear();
		pSceneObject->setPicked(true);
		m_vPickedList.push_back(pSceneObject);

		//Node* pNode = pSceneObject->m_pNode;
		//if (pNode)
		//{
		//	MotionJointProp mjp;
		//	mjp.strName = pNode->getName();
		//	mjp.vAbsolutePosition = pNode->getAbsolutePosition();
		//	mjp.vRelativePosition = pNode->getPosition();
		//	mjp.eRotation = pNode->getRotation();
		//	emit pickedOneObject(mjp);	
		//}
	}
}

void AbstractSceneManager::addPickedObjects(std::vector<SceneObject*>& vPickedObjects)
{
	for (size_t i=0; i<m_vPickedList.size(); i++)
	{
		m_vPickedList.at(i)->setPicked(false);
	}
	m_vPickedList.clear();
	m_vPickedList.insert(m_vPickedList.begin(),vPickedObjects.begin(),vPickedObjects.end());
	for (size_t i=0; i<m_vPickedList.size(); i++)
	{
		m_vPickedList.at(i)->setPicked(true);
	}
	if (m_vPickedList.empty())
	{
		emit pickedSeveralSkeletons();
	}
	else if (m_vPickedList.size() == 1)
	{
		//Node* pNode = m_vPickedList.at(0)->m_pNode;
		//if (pNode)
		//{
		//	MotionJointProp mjp;
		//	mjp.strName = pNode->getName();
		//	mjp.vAbsolutePosition = pNode->getAbsolutePosition();
		//	mjp.vRelativePosition = pNode->getPosition();
		//	mjp.eRotation = pNode->getRotation();
		//	emit pickedOneObject(mjp);	
		//}
	}
	else
	{
		int n = 0;
		Skeleton* pSkeleton = NULL;
		m_sPickedSkeleton.clear();
		for (size_t i=1; i<m_vPickedList.size(); i++)
		{
			if (m_vPickedList.at(i)->m_pNode->isBoneNode())
			{
				BoneNode* pBoneNode = (BoneNode*)m_vPickedList.at(i)->m_pNode;
				if (pSkeleton != pBoneNode->getSkeleton())
				{
					n++;
					pSkeleton = pBoneNode->getSkeleton();
					m_sPickedSkeleton.insert(pSkeleton);
				}
			}
		}
		if (n == 1)
		{
			if (pSkeleton)
			{
				MotionClip* pMotionClip = pSkeleton->getMotionClip();
				//if (pMotionClip)
				//{
				//	MotionFileProp mfp;
				//	mfp.strPath = QString::fromStdString(pMotionClip->getFileName());
				//	mfp.fFrameTime = pMotionClip->getFrameTime();
				//	mfp.nFrameCount = pMotionClip->getFrameCount();
				//	mfp.nJointCount = pMotionClip->getJointCount();
				//	mfp.mstate = pSkeleton->getViewState();
				//	mfp.bshowmesh = pSkeleton->isShowMesh();
				//	emit pickedOneSkeleton(mfp);	
				//}
			}
		}
		else
		{
			emit pickedSeveralSkeletons();
		}
	}
}

//void AbstractSceneManager::onPropertyChanged(QString strGroupName,QString strProperyName,QString strPropValue)
//{
//	if (strGroupName == "Motion File" && strProperyName == "State")
//	{
//		MFileState state = Active;
//		if (strPropValue == "Invisible")
//		{
//			state = Invisible;
//		}
//		else if (strPropValue == "Visible")
//		{
//			state = Visible;
//		}
//		//for (std::set<Skeleton*>::iterator it=m_sPickedSkeleton.begin(); it!=m_sPickedSkeleton.end(); it++)
//		//{
//		//	(*it)->setViewState(state);
//		//}
//		if (m_sPickedSkeleton.size() == 1)
//		{
//			(*m_sPickedSkeleton.begin())->setViewState(state);
//			int nIndex = 0;
//			for (size_t i=0; i<m_vSkeleton.size(); i++)
//			{
//				if (m_vSkeleton.at(i) == (*m_sPickedSkeleton.begin()))
//				{
//					nIndex = i;
//					break;
//				}
//			}
//			emit motionStateChanged(nIndex,state);	
//		}
//		emit needUpdate();
//	}else if(strGroupName == "Motion File" && strProperyName == "Show Mesh"){
//		bool bshow = false;
//		if(strPropValue == "Show")
//			bshow = true;
//		if (m_sPickedSkeleton.size() == 1)
//		{
//			(*m_sPickedSkeleton.begin())->setShowMesh(bshow);
//			int nIndex = 0;
//			for (size_t i=0; i<m_vSkeleton.size(); i++)
//			{
//				if (m_vSkeleton.at(i) == (*m_sPickedSkeleton.begin()))
//				{
//					nIndex = i;
//					break;
//				}
//			}
//			// change parameter type by zhangxiang on 09-01-15
//			// may be a bug ..
//			// but now it's ok!
//			emit motionStateChanged(nIndex,bshow);	
//		}
//		emit needUpdate();
//	}
//}

MotionClip* AbstractSceneManager::getLatestXMTemplate() const
{
	return m_pLastestXMTemplate;
}

Skeleton* AbstractSceneManager::getLatestXMSkeleton() const
{
	return m_pLastestXMSkeleton;
}

MotionClip* AbstractSceneManager::getCapturingMotionClip() const
{
	return m_pCapturingMotionClip;
}

Skeleton* AbstractSceneManager::getCapturingSkeleton() const
{
	return m_pCapturingSkeleton;
}

void AbstractSceneManager::setCapturingMotionClip(MotionClip* pMotionClip)
{
	m_pCapturingMotionClip = pMotionClip;
}

void AbstractSceneManager::finishedCapture()
{
	if (m_pCapturingMotionClip)
	{
		if (m_nMaxFrameCount < m_pCapturingMotionClip->getFrameCount())
		{
			m_nMaxFrameCount = m_pCapturingMotionClip->getFrameCount();
		}
		m_pCapturingMotionClip = NULL;
		m_pCapturingSkeleton = NULL;
		emit sceneObjectsChanged();	
	}
}

bool AbstractSceneManager::saveMotion()
{
	MotionClip* pMotionClip = NULL;
	if (m_nCurrentMotionIndex>=0 && m_nCurrentMotionIndex<m_vSkeleton.size())
	{
		pMotionClip = m_vSkeleton.at(m_nCurrentMotionIndex)->getMotionClip();
	}
	if (pMotionClip)
	{
		BVHWriter writer;
		writer.setFilePathName(pMotionClip->getFileName());
		writer.setSource(pMotionClip);
		writer.setSpacing("  ");
		writer.write();
		return true;
	}

	return false;
}

bool AbstractSceneManager::saveMotionAs(QString srtFilePath)
{
	MotionClip* pMotionClip = NULL;
	if (m_nCurrentMotionIndex>=0 && m_nCurrentMotionIndex<m_vSkeleton.size())
	{
		pMotionClip = m_vSkeleton.at(m_nCurrentMotionIndex)->getMotionClip();
	}
	if (pMotionClip)
	{
		BVHWriter writer;
		writer.setFilePathName(srtFilePath.toStdString());
		writer.setSource(pMotionClip);
		writer.setSpacing("  ");
		writer.write();
		return true;
	}

	return false;
}