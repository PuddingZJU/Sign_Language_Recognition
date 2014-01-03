#include "SceneNode.h"
#include "SceneManager.h"
using namespace Swift;

SceneNode::SceneNode(const QString& strName /* =  */, AbstractSceneManager * p_mgr /* = NULL */)
: Node( (NULL == p_mgr) ? (&SceneManager::instance()) : p_mgr )
{
	if (strName != "")
	{
		m_strName = strName;
	}
}

SceneNode::~SceneNode()
{

}