/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/03/20                                                  */
/************************************************************************/

#include "RuleNode.h"
using namespace Swift;

RuleNode::RuleNode(NodeType nodeType/*=ALL*/, RuleType ruleType/*=NONE*/, RuleNode* pParent/*=NULL*/)
: m_NodeType(nodeType), m_RuleType(ruleType), m_pParent(pParent)
{

}

RuleNode::~RuleNode()
{
	for (int i=0; i<m_vChildren.size(); i++)
	{
		if (m_vChildren.at(i))
		{
			delete m_vChildren.at(i);
		}
	}
}


void RuleNode::cleanSigns(bool bFlag/* =true */)
{
	m_vSigns.clear();
	if (bFlag)
	{
		for (int i=0; i<m_vChildren.size(); i++)
		{
			m_vChildren.at(i)->cleanSigns(bFlag);
		}
	}
}