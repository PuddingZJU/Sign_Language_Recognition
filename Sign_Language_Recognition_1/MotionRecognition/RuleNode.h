/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/03/20                                                  */
/************************************************************************/

#pragma once

#ifndef _RULENODE_H_
#define _RULENODE_H_

#include <vector>
#include <string>
#include "../MotionData/MotionClip.h"

namespace Swift
{
	typedef enum
	{
		NONE=0,
		ONETWOHANDS,
		CLOSEFARHEAD,
		CLOSEFARHANDS,
	}RuleType;

	typedef enum
	{
		ALL=0,
		ONEHAND,
		TWOHANDS,
		CLOSEHEAD,
		FARHEAD,
		CLOSEHANDS,
		FARHANDS,
	}NodeType;

	/// One node in the rule index tree.
	class RuleNode
	{
	public:
		/// Constructor.
		/**
		* @param nodeType The type of this node.
		* @param ruleType The rule type of this node, which is used to decide which child node the input motion belongs to.
		* @param pParent The parent node of this node.
		*/
		RuleNode(NodeType nodeType=ALL, RuleType ruleType=NONE, RuleNode* pParent=NULL);
		/// Destructor.
		~RuleNode();

		/// Clean the classified signs in this node.
		/**
		* @param bFlag If true, clean the signs in both this node and its child nodes. If false, only clean signs in this node. 
		*/
		void cleanSigns(bool bFlag=true);

	public:
		/// The name of the node.
		std::string m_strName;
		/// The type of rule in this node.
		RuleType m_RuleType;
		/// The type of the node.
		NodeType m_NodeType;
		/// The parent node.
		RuleNode* m_pParent;
		/// All the child nodes.
		std::vector<RuleNode*> m_vChildren;

		/// All the signs containing in this node. Only the leaf node should fill this vector.
		std::vector<std::string> m_vSigns;
	}; // RuleNode
}

#endif // _RULENODE_H_