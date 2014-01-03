#include "RenderQueue.h"
#include "SceneObject.h"
#include "SimpleObject.h"
using namespace Swift;

RenderQueue::RenderQueue()
{
	std::vector<SceneObject*> v1;
	std::vector<SceneObject*> v2;
	std::vector<SceneObject*> v3;
	m_mRenderGroup.insert(std::pair<RenderGroupID,std::vector<SceneObject*> >(RENDER_QUEUE_BACKGROUND,v1));
	m_mRenderGroup.insert(std::pair<RenderGroupID,std::vector<SceneObject*> >(RENDER_QUEUE_MAIN,v2));
	m_mRenderGroup.insert(std::pair<RenderGroupID,std::vector<SceneObject*> >(RENDER_QUEUE_OVERLAY,v3));
}

RenderQueue::~RenderQueue()
{
	if(m_mRenderGroup.size() > 0)
		m_mRenderGroup.clear();
}

void RenderQueue::addSceneObject(SceneObject* pSceneObject,RenderGroupID id/* = RENDER_QUEUE_MAIN*/)
{
	RenderGroupMap::iterator it = m_mRenderGroup.find(id);
	if (it != m_mRenderGroup.end())
	{
		it->second.push_back(pSceneObject);
	}
}

void RenderQueue::deleteSceneObject(SceneObject* pSceneObject,RenderGroupID id/* = RENDER_QUEUE_UNKNOWN*/)
{
	if (id == RENDER_QUEUE_UNKNOWN)
	{
		for (RenderGroupMap::iterator it1=m_mRenderGroup.begin(); it1!=m_mRenderGroup.end(); it1++)
		{
			std::vector<SceneObject*>::iterator it2=it1->second.begin();
			for (size_t j=0; j<it1->second.size(); it1++)
			{
				if (it1->second.at(j) == pSceneObject )
				{
					it1->second.erase(it2+j);
					delete pSceneObject;
					return;
				}
			}
		}
	}
	else
	{
		RenderGroupMap::iterator it1 = m_mRenderGroup.find(id);
		if (it1 != m_mRenderGroup.end())
		{
			std::vector<SceneObject*>::iterator it2=it1->second.begin();
			for (size_t j=0; j<it1->second.size(); j++)
			{
				if (it1->second.at(j) == pSceneObject )
				{
					it1->second.erase(it2+j);
					delete pSceneObject;
					return;
				}
			}
		}
	}
}

void RenderQueue::renderAll()
{
	for(RenderGroupMap::iterator it=m_mRenderGroup.begin(); it!=m_mRenderGroup.end(); it++)
	{
		for(size_t i=0; i<it->second.size(); i++)
		{
			SceneObject* pSceneObject = it->second.at(i);
			bool b = pSceneObject->getValidation();
			if(pSceneObject->getValidation())
			{
				pSceneObject->render();
			}
			else
			{
				deleteSceneObject(pSceneObject);
			}
		}
	}
}

void RenderQueue::renderSelected()
{
	glInitNames();
	glPushName(0);

	for(RenderGroupMap::iterator it=m_mRenderGroup.begin(); it!=m_mRenderGroup.end(); it++)
	{
		for(size_t i=0; i<it->second.size(); i++)
		{
			SceneObject* pSceneObject = it->second.at(i);
			if (IDManager::instance().getIDType(pSceneObject->getID()) == ID_Entity)
			{
				glLoadName(pSceneObject->getID());
				if(pSceneObject->getValidation())
				{
					pSceneObject->render();
				}
				else
				{
					deleteSceneObject(pSceneObject);
				}
			}
			//if (IDManager::instance().getIDType(pSceneObject->getID()) == ID_Axes)
			//{
			//	glLoadName(pSceneObject->getID());
			//	pSceneObject->render();

			//	glPushName(static_cast<TransformTool *>(pSceneObject)->getXID());
			//	static_cast<TransformTool *>(pSceneObject)->renderX();
			//	glPopName();

			//	glPushName(static_cast<TransformTool *>(pSceneObject)->getYID());
			//	static_cast<TransformTool *>(pSceneObject)->renderY();
			//	glPopName();

			//	glPushName(static_cast<TransformTool *>(pSceneObject)->getZID());
			//	static_cast<TransformTool *>(pSceneObject)->renderZ();
			//	glPopName();
			//}
		}
	}

	glPopName();
}