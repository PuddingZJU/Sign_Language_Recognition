/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/03/20                                                  */
/************************************************************************/

#include "RuleManager.h"
using namespace Swift;

RuleManager::RuleManager()
: m_pRootRuleNode(NULL), m_dOneTwoHandsThres(20.0), m_dCloseFarHeadThres(30.0), m_dCloseFarHandsThres(20.0)
{
	createRuleTree();
}

RuleManager::~RuleManager()
{
	if (m_pRootRuleNode)
		delete m_pRootRuleNode;
}

void RuleManager::createRuleTree()
{
	m_pRootRuleNode = new RuleNode(ALL, ONETWOHANDS);
	m_pRootRuleNode->m_strName = "All";
	RuleNode* pRuleNodeOneHand = new RuleNode(ONEHAND, CLOSEFARHEAD);
	pRuleNodeOneHand->m_pParent = m_pRootRuleNode;
	pRuleNodeOneHand->m_strName = "OneHand";
	RuleNode* pRuleNodeTwoHands = new RuleNode(TWOHANDS, CLOSEFARHANDS);
	pRuleNodeTwoHands->m_pParent = m_pRootRuleNode;
	pRuleNodeTwoHands->m_strName = "TwoHands";
	m_pRootRuleNode->m_vChildren.push_back(pRuleNodeOneHand);
	m_pRootRuleNode->m_vChildren.push_back(pRuleNodeTwoHands);
	RuleNode* pRuleNodeCloseHead = new RuleNode(CLOSEHEAD);
	pRuleNodeCloseHead->m_pParent = pRuleNodeOneHand;
	pRuleNodeCloseHead->m_strName = "CloseHead";
	RuleNode* pRuleNodeFarHead = new RuleNode(FARHEAD);
	pRuleNodeFarHead->m_pParent = pRuleNodeOneHand;
	pRuleNodeFarHead->m_strName = "FarHead";
	pRuleNodeOneHand->m_vChildren.push_back(pRuleNodeCloseHead);
	pRuleNodeOneHand->m_vChildren.push_back(pRuleNodeFarHead);
	RuleNode* pRuleNodeCloseHands = new RuleNode(CLOSEHANDS);
	pRuleNodeCloseHands->m_pParent = pRuleNodeTwoHands;
	pRuleNodeCloseHands->m_strName = "CloseHands";
	RuleNode* pRuleNodeFarHands = new RuleNode(FARHANDS);
	pRuleNodeFarHands->m_pParent = pRuleNodeTwoHands;
	pRuleNodeFarHands->m_strName = "FarHands";
	pRuleNodeTwoHands->m_vChildren.push_back(pRuleNodeCloseHands);
	pRuleNodeTwoHands->m_vChildren.push_back(pRuleNodeFarHands);
	m_vLeafNode.push_back(pRuleNodeCloseHead);
	m_vLeafNode.push_back(pRuleNodeFarHead);
	m_vLeafNode.push_back(pRuleNodeCloseHands);
	m_vLeafNode.push_back(pRuleNodeFarHands);
}


RuleNode* RuleManager::findCandidateNode(MotionClip* pmc, RuleNode* pStartNode)
{
	RuleNode* pRuleNode=NULL;
	if (!pStartNode)
		pRuleNode=NULL;
	if (pStartNode->m_vChildren.size() == 0)
		pRuleNode=pStartNode;
	else
	{
		RuleNode* pNextNode = nextNode(pmc, pStartNode);
		if (pNextNode)
			pRuleNode = findCandidateNode(pmc, pNextNode);
		else
			pRuleNode=NULL;
	}

	return pRuleNode;
}

RuleNode* RuleManager::nextNode(MotionClip* pmc, RuleNode* pNode)
{
	RuleNode* pRuleNode=NULL;

	switch(pNode->m_RuleType)
	{
	case ONETWOHANDS:
		pRuleNode = judgeOneTwoHands(pmc, pNode);
		break;
	case CLOSEFARHEAD:
		pRuleNode = judgeCloseFarHead(pmc, pNode);
		break;
	case CLOSEFARHANDS:
		pRuleNode = judgeCloseFarHands(pmc, pNode);
		break;
	case NONE:
		pRuleNode = NULL;
		break;
	}

	return pRuleNode;
}

RuleNode* RuleManager::judgeOneTwoHands(MotionClip* pmc, RuleNode* pNode)
{
	RuleNode* pRuleNode=NULL;

	MotionJoint *pmjlw, *pmjle, *pmjrw, *pmjre;
	pmjlw = pmc->findJoint("l_wrist");
	pmjle = pmc->findJoint("l_elbow");
	pmjrw = pmc->findJoint("r_wrist");
	pmjre = pmc->findJoint("r_elbow");
	double dMeanlw=0.0, dMeanle=0.0, dMeanrw=0.0, dMeanre=0.0;
	double dVarlw=0.0, dVarle=0.0, dVarrw=0.0, dVarre=0.0;
	double dCoeflw=0.5, dCoefle=0.5, dCoefrw=0.5, dCoefre=0.5;
	int nFrameCount=pmc->getFrameCount();
	double d;
	for (int i=0; i<nFrameCount; i++)
	{
		dMeanlw += Distance(pmjlw->getAbsolutePosition(i), pmjlw->getAbsolutePosition(0));
		dMeanle += Distance(pmjle->getAbsolutePosition(i), pmjle->getAbsolutePosition(0));
		dMeanrw += Distance(pmjrw->getAbsolutePosition(i), pmjrw->getAbsolutePosition(0));
		dMeanre += Distance(pmjre->getAbsolutePosition(i), pmjre->getAbsolutePosition(0));
	}
	dMeanlw /= (double)nFrameCount;
	dMeanle /= (double)nFrameCount;
	dMeanrw /= (double)nFrameCount;
	dMeanre /= (double)nFrameCount;
	for (int i=0; i<nFrameCount; i++)
	{
		d = Distance(pmjlw->getAbsolutePosition(i), pmjlw->getAbsolutePosition(0));
		dVarlw += (d-dMeanlw)*(d-dMeanlw);
		d = Distance(pmjle->getAbsolutePosition(i), pmjle->getAbsolutePosition(0));
		dVarle += (d-dMeanle)*(d-dMeanle);
		d = Distance(pmjrw->getAbsolutePosition(i), pmjrw->getAbsolutePosition(0));
		dVarrw += (d-dMeanrw)*(d-dMeanrw);
		d = Distance(pmjre->getAbsolutePosition(i), pmjre->getAbsolutePosition(0));
		dVarre += (d-dMeanre)*(d-dMeanre);
	}
	dVarlw /= (double)nFrameCount;
	dVarle /= (double)nFrameCount;
	dVarrw /= (double)nFrameCount;
	dVarre /= (double)nFrameCount;

	double dVarl = dCoeflw*dVarlw+dCoefle*dVarle;
	double dVarr = dCoefrw*dVarrw+dCoefre*dVarre;
	if ((dVarl>m_dOneTwoHandsThres && dVarr<=m_dOneTwoHandsThres) || (dVarl<=m_dOneTwoHandsThres && dVarr>m_dOneTwoHandsThres))
	{
		for (int i=0; i<pNode->m_vChildren.size(); i++)
		{
			if (pNode->m_vChildren.at(i)->m_NodeType == ONEHAND)
			{
				pRuleNode=pNode->m_vChildren.at(i);
			}
		}
	}
	else if (dVarl>m_dOneTwoHandsThres && dVarr>m_dOneTwoHandsThres)
	{
		for (int i=0; i<pNode->m_vChildren.size(); i++)
		{
			if (pNode->m_vChildren.at(i)->m_NodeType == TWOHANDS)
			{
				pRuleNode=pNode->m_vChildren.at(i);
			}
		}
	}

	return pRuleNode;
}

RuleNode* RuleManager::judgeCloseFarHead(MotionClip* pmc, RuleNode* pNode)
{
	RuleNode* pRuleNode=NULL;

	MotionJoint *pmjskull, *pmjlw, *pmjrw;
	pmjskull = pmc->findJoint("skullbase");
	pmjlw = pmc->findJoint("l_wrist");
	pmjrw = pmc->findJoint("r_wrist");
	double dlw, drw;
	int nFrame=0;
	for (int i=0; i<pmc->getFrameCount(); i++)
	{
		dlw = Distance(pmjskull->getAbsolutePosition(i), pmjlw->getAbsolutePosition(i));
		drw = Distance(pmjskull->getAbsolutePosition(i), pmjrw->getAbsolutePosition(i));
		if (dlw<m_dCloseFarHeadThres || drw<m_dCloseFarHeadThres)
			nFrame++;
	}
	if (nFrame>10)
	{
		for (int i=0; i<pNode->m_vChildren.size(); i++)
		{
			if (pNode->m_vChildren.at(i)->m_NodeType == CLOSEHEAD)
			{
				pRuleNode=pNode->m_vChildren.at(i);
			}
		}
	}
	else
	{
		for (int i=0; i<pNode->m_vChildren.size(); i++)
		{
			if (pNode->m_vChildren.at(i)->m_NodeType == FARHEAD)
			{
				pRuleNode=pNode->m_vChildren.at(i);
			}
		}
	}

	return pRuleNode;
}

RuleNode* RuleManager::judgeCloseFarHands(MotionClip* pmc, RuleNode* pNode)
{
	RuleNode* pRuleNode=NULL;

	MotionJoint *pmjlw, *pmjrw;
	pmjlw = pmc->findJoint("l_wrist");
	pmjrw = pmc->findJoint("r_wrist");
	double d;
	int nFrame=0;
	for (int i=0; i<pmc->getFrameCount(); i++)
	{
		d = Distance(pmjlw->getAbsolutePosition(i), pmjrw->getAbsolutePosition(i));
		if (d<m_dCloseFarHandsThres)
			nFrame++;
	}
	if (nFrame>10)
	{
		for (int i=0; i<pNode->m_vChildren.size(); i++)
		{
			if (pNode->m_vChildren.at(i)->m_NodeType == CLOSEHANDS)
			{
				pRuleNode=pNode->m_vChildren.at(i);
			}
		}
	}
	else
	{
		for (int i=0; i<pNode->m_vChildren.size(); i++)
		{
			if (pNode->m_vChildren.at(i)->m_NodeType == FARHANDS)
			{
				pRuleNode=pNode->m_vChildren.at(i);
			}
		}
	}

	return pRuleNode;
}