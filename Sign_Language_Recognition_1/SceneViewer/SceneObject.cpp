#include "SceneObject.h"
#include "abstractSceneManager.h"
using namespace Swift;

SceneObject::SceneObject(AbstractSceneManager * p_mgr, IDType type)
: m_pSceneManager(p_mgr)
{
	// Register the ID-pointer pair to the SceneManager.
	if( NULL != m_pSceneManager ){
		m_nID = IDManager::instance().getID(type);
		m_pSceneManager ->insertIDTraceable( m_nID, this);
	}
	m_pNode = NULL;
	m_nRenderGroupID = RENDER_QUEUE_MAIN;
	m_bIsPicked = false;
	m_rgba.r = 0.8;
	m_rgba.g = 0.8;
	m_rgba.b = 0.2;
	m_rgba.a = 1.0;
	m_nIndex = -1;
	m_bVisible = true;
	m_bValidation = true;
}

SceneObject::~SceneObject()
{
	// Remove the ID-pointer pair to the SceneManager.
	if(NULL != m_pSceneManager){
		m_pSceneManager ->removeIDTraceable( m_nID );
	}
}

void SceneObject::regiterToSceneManager(AbstractSceneManager *p_mgr, IDType type)
{
	if( NULL != m_pSceneManager ){
		m_pSceneManager ->removeIDTraceable( m_nID );
	}
	
	m_pSceneManager = p_mgr;

	if( NULL != m_pSceneManager ){
		m_nID = IDManager::instance().getID(type);
		m_pSceneManager ->insertIDTraceable( m_nID, this);
	}

	return ;
}

void SceneObject::attachToNode(Node* pNode)
{
	m_pNode = pNode;
}

Node* SceneObject::detachFromNode()
{
	Node* pNode = m_pNode;
	m_pNode = NULL;

	return pNode;
}

RenderGroupID SceneObject::getRenderGroupID()
{
	return m_nRenderGroupID;
}

void SceneObject::setRenderGroupID(RenderGroupID nID)
{
	m_nRenderGroupID = nID;
}

Color SceneObject::getColor()
{
	return m_rgba;
}

void SceneObject::setColor(Color rgba)
{
	m_rgba = rgba;
}

bool SceneObject::isPicked()
{
	return m_bIsPicked;
}

void SceneObject::setPicked(bool bFlag)
{
	m_bIsPicked = bFlag;
	if (m_bIsPicked)
	{
		m_rgba.r = 0.0;
		m_rgba.g = 0.8;
		m_rgba.b = 0.0;
		m_rgba.a = 1.0;
	}
	else
	{
		m_rgba.r = 0.8;
		m_rgba.g = 0.8;
		m_rgba.b = 0.2;
		m_rgba.a = 1.0;
	}
}

int SceneObject::getIndex()
{
	return m_nIndex;
}

void SceneObject::setIndex(int vIndex)
{
	m_nIndex = vIndex;
}

void SceneObject::setVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

bool SceneObject::getValidation()
{
	return m_bValidation;
}

void SceneObject::setValidation(bool bValidation)
{
	m_bValidation = bValidation;
}