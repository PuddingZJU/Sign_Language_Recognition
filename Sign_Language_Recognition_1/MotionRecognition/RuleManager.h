/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/03/20                                                  */
/************************************************************************/

#pragma once

#ifndef _RULEMANAGER_H_
#define _RULEMANAGER_H_

#include <string>
#include <vector>
#include "RuleNode.h"

namespace Swift
{
	/// This is designed to manage the rule index tree.
	class RuleManager
	{
	public:
		/// Constructor.
		RuleManager();
		/// Destructor.
		~RuleManager();

		/// Create the initialized rule index tree.
		void createRuleTree();

		/// Find the candidate node by going down one path in the rule index tree.
		/**
		* @param pmc The input motion data.
		* @param pStartNode The start node on the rule index tree.
		* @return The found node. Note that the found node must be one of the children of the current node.
		*/
		RuleNode* findCandidateNode(MotionClip* pmc, RuleNode* pStartNode);

	protected:
		/// Find the next node according to the rule in this node.
		/** 
		* @param pmc The input motion data.
		* @param pNode The input node.
		* @return The found node. Note that the found node must be one of the children of the current node.
		*/
		RuleNode* nextNode(MotionClip* pmc, RuleNode* pNode);

		/// Find the node according to the rule "one or two hands".
		/** 
		* @param pmc The input motion data.
		* @param pNode The input node.
		* @return The found node.
		*/
		RuleNode* judgeOneTwoHands(MotionClip* pmc, RuleNode* pNode);

		/// Find the node according to the rule "close or far from head".
		/** 
		* @param pmc The input motion data.
		* @param pNode The input node.
		* @return The found node.
		*/
		RuleNode* judgeCloseFarHead(MotionClip* pmc, RuleNode* pNode);

		/// Find the node according to the rule "two hands are close or far from each other".
		/** 
		* @param pmc The input motion data.
		* @param pNode The input node.
		* @return The found node.
		*/
		RuleNode* judgeCloseFarHands(MotionClip* pmc, RuleNode* pNode);

	public:
		/// The root of the rule index tree.
		RuleNode* m_pRootRuleNode;

		/// All the leaf nodes in the rule index tree.
		std::vector<RuleNode*> m_vLeafNode;

		/// The threshold for the rule "one or two hands".
		double m_dOneTwoHandsThres;
		/// The threshold for the rule "close or far from head".
		double m_dCloseFarHeadThres;
		/// The threshold for the rule "two hands are close or far from each other".
		double m_dCloseFarHandsThres;

	}; // RuleManager
} // Swift

#endif // _RULEMANAGER_H_