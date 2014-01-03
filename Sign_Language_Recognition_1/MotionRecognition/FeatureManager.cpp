/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/02                                                  */
/************************************************************************/

#include "../SwiftModel/SwiftModel.h"
#include "FeatureManager.h"

using namespace Swift;

FeatureManager::FeatureManager()
: m_nID(0), m_bFlex(true), m_bAbd(false), m_bPos(true), m_bOri(true), m_bMov(true)
{
	m_vRightFlexStr.push_back("RightThumb");
	m_vRightFlexStr.push_back("RightIndex");
	m_vRightFlexStr.push_back("RightMiddle");
	m_vRightFlexStr.push_back("RightRing");
	m_vRightFlexStr.push_back("RightLittle");

	m_vLeftFlexStr.push_back("LeftThumb");
	m_vLeftFlexStr.push_back("LeftIndex");
	m_vLeftFlexStr.push_back("LeftMiddle");
	m_vLeftFlexStr.push_back("LeftRing");
	m_vLeftFlexStr.push_back("LeftLittle");

	m_vRightAbdStr.push_back("RightThumbIndex");
	m_vRightAbdStr.push_back("RightIndexMiddle");
	m_vRightAbdStr.push_back("RightMiddleRing");
	m_vRightAbdStr.push_back("RightRingLittle");

	m_vLeftAbdStr.push_back("LeftThumbIndex");
	m_vLeftAbdStr.push_back("LeftIndexMiddle");
	m_vLeftAbdStr.push_back("LeftMiddleRing");
	m_vLeftAbdStr.push_back("LeftRingLittle");

	m_vPosStr.push_back("RightWristRoot");
	m_vPosStr.push_back("RightWristHead");
	m_vPosStr.push_back("RightWristLeftWrist");
	m_vPosStr.push_back("LeftWristRoot");
	m_vPosStr.push_back("LeftWristHead");

	m_vOriStr.push_back("RightPalm");
	m_vOriStr.push_back("LeftPalm");

	m_vIdentity.push_back(MEAN);
	m_vIdentity.push_back(VARIANCE);
	m_vIdentity.push_back(MARGIN);
	m_vIdentity.push_back(TURNINGPOINTS);

	genFeatureSet();
}

FeatureManager::~FeatureManager()
{
	for (std::map<int, Feature*>::iterator it=m_mFeatureSet.begin(); it!=m_mFeatureSet.end(); it++)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
}

void FeatureManager::genFeatureSet()
{
	m_mFeatureSet.clear();

	IndividualFeature *pif;
	RelationalFeature *prf;

	for (int t=0; t<m_vIdentity.size(); t++)
	{
		// 242 flexion features.
		if (m_bFlex)
		{
			// right hand flexion feature definition.
			// 5 individual features
			for (int i=0; i<m_vRightFlexStr.size(); i++)
			{
				pif = new IndividualFeature();
				pif->m_nID = ++m_nID;
				pif->m_nSubset = FLEXION;
				pif->m_nIdentity = m_vIdentity.at(t);
				pif->m_strName = m_vRightFlexStr.at(i);
				m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));
			}
			// 20 relational features with two fingers involved.
			for (int i=0; i<m_vRightFlexStr.size(); i++)
			{
				for (int j=i+1; j<m_vRightFlexStr.size(); j++)
				{
					prf = new RelationalFeature();
					prf->m_nID = ++m_nID;
					prf->m_nSubset = FLEXION;
					prf->m_nIdentity = m_vIdentity.at(t);
					prf->m_vGroup1.push_back(m_vRightFlexStr.at(i));
					prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
					prf->m_nOperation = 0;
					m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

					prf = new RelationalFeature();
					prf->m_nID = ++m_nID;
					prf->m_nSubset = FLEXION;
					prf->m_nIdentity = m_vIdentity.at(t);
					prf->m_vGroup1.push_back(m_vRightFlexStr.at(i));
					prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
					prf->m_nOperation = 1;
					m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
				}
			}
			// 40 relational features with three fingers involved.
			for (int i=0; i<m_vRightFlexStr.size(); i++)
			{
				for (int j=i+1; j<m_vRightFlexStr.size(); j++)
				{
					for (int k=j+1; k<m_vRightFlexStr.size(); k++)
					{
						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = FLEXION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vRightFlexStr.at(i));
						prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
						prf->m_vGroup2.push_back(m_vRightFlexStr.at(k));
						prf->m_nOperation = 0;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = FLEXION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vRightFlexStr.at(i));
						prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
						prf->m_vGroup2.push_back(m_vRightFlexStr.at(k));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = FLEXION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vRightFlexStr.at(j));
						prf->m_vGroup2.push_back(m_vRightFlexStr.at(i));
						prf->m_vGroup2.push_back(m_vRightFlexStr.at(k));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = FLEXION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vRightFlexStr.at(k));
						prf->m_vGroup2.push_back(m_vRightFlexStr.at(i));
						prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
					}
				}
			}
			// 40 relational features with four fingers involved.
			for (int i=0; i<m_vRightFlexStr.size(); i++)
			{
				for (int j=i+1; j<m_vRightFlexStr.size(); j++)
				{
					for (int k=j+1; k<m_vRightFlexStr.size(); k++)
					{
						for (int l=k+1; l<m_vRightFlexStr.size(); l++)
						{
							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(l));
							prf->m_nOperation = 0;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(l));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(k));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(i));
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(i));
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(i));
							prf->m_vGroup1.push_back(m_vRightFlexStr.at(l));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vRightFlexStr.at(k));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
						}
					}
				}
			}
			// 16 relational features with five fingers involved.
			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(2));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(3));
			prf->m_vGroup1.push_back(m_vRightFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vRightFlexStr.at(2));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));


			// left hand flexion feature definition.
			// 5 individual features
			for (int i=0; i<m_vLeftFlexStr.size(); i++)
			{
				pif = new IndividualFeature();
				pif->m_nID = ++m_nID;
				pif->m_nSubset = FLEXION;
				pif->m_nIdentity = m_vIdentity.at(t);
				pif->m_strName = m_vLeftFlexStr.at(i);
				m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));
			}
			// 20 relational features with two fingers involved.
			for (int i=0; i<m_vLeftFlexStr.size(); i++)
			{
				for (int j=i+1; j<m_vLeftFlexStr.size(); j++)
				{
					prf = new RelationalFeature();
					prf->m_nID = ++m_nID;
					prf->m_nSubset = FLEXION;
					prf->m_nIdentity = m_vIdentity.at(t);
					prf->m_vGroup1.push_back(m_vLeftFlexStr.at(i));
					prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
					prf->m_nOperation = 0;
					m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

					prf = new RelationalFeature();
					prf->m_nID = ++m_nID;
					prf->m_nSubset = FLEXION;
					prf->m_nIdentity = m_vIdentity.at(t);
					prf->m_vGroup1.push_back(m_vLeftFlexStr.at(i));
					prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
					prf->m_nOperation = 1;
					m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
				}
			}
			// 40 relational features with three fingers involved.
			for (int i=0; i<m_vLeftFlexStr.size(); i++)
			{
				for (int j=i+1; j<m_vLeftFlexStr.size(); j++)
				{
					for (int k=j+1; k<m_vLeftFlexStr.size(); k++)
					{
						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = FLEXION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vLeftFlexStr.at(i));
						prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
						prf->m_vGroup2.push_back(m_vLeftFlexStr.at(k));
						prf->m_nOperation = 0;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = FLEXION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vLeftFlexStr.at(i));
						prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
						prf->m_vGroup2.push_back(m_vLeftFlexStr.at(k));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = FLEXION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vLeftFlexStr.at(j));
						prf->m_vGroup2.push_back(m_vLeftFlexStr.at(i));
						prf->m_vGroup2.push_back(m_vLeftFlexStr.at(k));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = FLEXION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vLeftFlexStr.at(k));
						prf->m_vGroup2.push_back(m_vLeftFlexStr.at(i));
						prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
					}
				}
			}
			// 40 relational features with four fingers involved.
			for (int i=0; i<m_vLeftFlexStr.size(); i++)
			{
				for (int j=i+1; j<m_vLeftFlexStr.size(); j++)
				{
					for (int k=j+1; k<m_vLeftFlexStr.size(); k++)
					{
						for (int l=k+1; l<m_vLeftFlexStr.size(); l++)
						{
							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(l));
							prf->m_nOperation = 0;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(l));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(i));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(k));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(i));
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(i));
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(k));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(l));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

							prf = new RelationalFeature();
							prf->m_nID = ++m_nID;
							prf->m_nSubset = FLEXION;
							prf->m_nIdentity = m_vIdentity.at(t);
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(i));
							prf->m_vGroup1.push_back(m_vLeftFlexStr.at(l));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(j));
							prf->m_vGroup2.push_back(m_vLeftFlexStr.at(k));
							prf->m_nOperation = 1;
							m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
						}
					}
				}
			}
			// 16 relational features with five fingers involved.
			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(4));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(2));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = FLEXION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(3));
			prf->m_vGroup1.push_back(m_vLeftFlexStr.at(4));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftFlexStr.at(2));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
		}

		// 80 abduction features.
		if (m_bAbd)
		{
			// right hand abduction feature definition.
			// 4 individual abduction features
			for (int i=0; i<m_vRightAbdStr.size(); i++)
			{
				pif = new IndividualFeature();
				pif->m_nID = ++m_nID;
				pif->m_nSubset = ABDUCTION;
				pif->m_nIdentity = m_vIdentity.at(t);
				pif->m_strName = m_vRightAbdStr.at(i);
				m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));
			}
			// 12 relational features with two finger abductions involved.
			for (int i=0; i<m_vRightAbdStr.size(); i++)
			{
				for (int j=i+1; j<m_vRightAbdStr.size(); j++)
				{
					prf = new RelationalFeature();
					prf->m_nID = ++m_nID;
					prf->m_nSubset = ABDUCTION;
					prf->m_nIdentity = m_vIdentity.at(t);
					prf->m_vGroup1.push_back(m_vRightAbdStr.at(i));
					prf->m_vGroup2.push_back(m_vRightAbdStr.at(j));
					prf->m_nOperation = 0;
					m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

					prf = new RelationalFeature();
					prf->m_nID = ++m_nID;
					prf->m_nSubset = ABDUCTION;
					prf->m_nIdentity = m_vIdentity.at(t);
					prf->m_vGroup1.push_back(m_vRightAbdStr.at(i));
					prf->m_vGroup2.push_back(m_vRightAbdStr.at(j));
					prf->m_nOperation = 1;
					m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
				}
			}
			// 16 relational features with three finger abductions involved.
			for (int i=0; i<m_vRightAbdStr.size(); i++)
			{
				for (int j=i+1; j<m_vRightAbdStr.size(); j++)
				{
					for (int k=j+1; k<m_vRightAbdStr.size(); k++)
					{
						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = ABDUCTION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vRightAbdStr.at(i));
						prf->m_vGroup2.push_back(m_vRightAbdStr.at(j));
						prf->m_vGroup2.push_back(m_vRightAbdStr.at(k));
						prf->m_nOperation = 0;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = ABDUCTION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vRightAbdStr.at(i));
						prf->m_vGroup2.push_back(m_vRightAbdStr.at(j));
						prf->m_vGroup2.push_back(m_vRightAbdStr.at(k));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = ABDUCTION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vRightAbdStr.at(j));
						prf->m_vGroup2.push_back(m_vRightAbdStr.at(i));
						prf->m_vGroup2.push_back(m_vRightAbdStr.at(k));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = ABDUCTION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vRightAbdStr.at(k));
						prf->m_vGroup2.push_back(m_vRightAbdStr.at(i));
						prf->m_vGroup2.push_back(m_vRightAbdStr.at(j));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
					}
				}
			}
			// 8 relational features with four finger abductions involved.
			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(3));
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(3));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(2));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(0));
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(0));
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(0));
			prf->m_vGroup1.push_back(m_vRightAbdStr.at(3));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vRightAbdStr.at(2));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));


			// left hand abduction feature definition.
			// 4 individual abduction features
			for (int i=0; i<m_vLeftAbdStr.size(); i++)
			{
				pif = new IndividualFeature();
				pif->m_nID = ++m_nID;
				pif->m_nSubset = ABDUCTION;
				pif->m_nIdentity = m_vIdentity.at(t);
				pif->m_strName = m_vLeftAbdStr.at(i);
				m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));
			}
			// 12 relational features with two finger abductions involved.
			for (int i=0; i<m_vLeftAbdStr.size(); i++)
			{
				for (int j=i+1; j<m_vLeftAbdStr.size(); j++)
				{
					prf = new RelationalFeature();
					prf->m_nID = ++m_nID;
					prf->m_nSubset = ABDUCTION;
					prf->m_nIdentity = m_vIdentity.at(t);
					prf->m_vGroup1.push_back(m_vLeftAbdStr.at(i));
					prf->m_vGroup2.push_back(m_vLeftAbdStr.at(j));
					prf->m_nOperation = 0;
					m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

					prf = new RelationalFeature();
					prf->m_nID = ++m_nID;
					prf->m_nSubset = ABDUCTION;
					prf->m_nIdentity = m_vIdentity.at(t);
					prf->m_vGroup1.push_back(m_vLeftAbdStr.at(i));
					prf->m_vGroup2.push_back(m_vLeftAbdStr.at(j));
					prf->m_nOperation = 1;
					m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
				}
			}
			// 16 relational features with three finger abductions involved.
			for (int i=0; i<m_vLeftAbdStr.size(); i++)
			{
				for (int j=i+1; j<m_vLeftAbdStr.size(); j++)
				{
					for (int k=j+1; k<m_vLeftAbdStr.size(); k++)
					{
						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = ABDUCTION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vLeftAbdStr.at(i));
						prf->m_vGroup2.push_back(m_vLeftAbdStr.at(j));
						prf->m_vGroup2.push_back(m_vLeftAbdStr.at(k));
						prf->m_nOperation = 0;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = ABDUCTION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vLeftAbdStr.at(i));
						prf->m_vGroup2.push_back(m_vLeftAbdStr.at(j));
						prf->m_vGroup2.push_back(m_vLeftAbdStr.at(k));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = ABDUCTION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vLeftAbdStr.at(j));
						prf->m_vGroup2.push_back(m_vLeftAbdStr.at(i));
						prf->m_vGroup2.push_back(m_vLeftAbdStr.at(k));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

						prf = new RelationalFeature();
						prf->m_nID = ++m_nID;
						prf->m_nSubset = ABDUCTION;
						prf->m_nIdentity = m_vIdentity.at(t);
						prf->m_vGroup1.push_back(m_vLeftAbdStr.at(k));
						prf->m_vGroup2.push_back(m_vLeftAbdStr.at(i));
						prf->m_vGroup2.push_back(m_vLeftAbdStr.at(j));
						prf->m_nOperation = 1;
						m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
					}
				}
			}
			// 8 relational features with four finger abductions involved.
			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(3));
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(0));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(2));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(0));
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(0));
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(2));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(3));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ABDUCTION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(0));
			prf->m_vGroup1.push_back(m_vLeftAbdStr.at(3));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(1));
			prf->m_vGroup2.push_back(m_vLeftAbdStr.at(2));
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
		}

		// 9 Position features
		if (m_bPos)
		{
			// 5 individual feature.
			for (int i=0; i<m_vPosStr.size(); i++)
			{
				pif = new IndividualFeature();
				pif->m_nID = ++m_nID;
				pif->m_nSubset = POSITION;
				pif->m_nIdentity = m_vIdentity.at(t);
				pif->m_strName = m_vPosStr.at(i);
				m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));
			}

			// 4 relational features.
			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = POSITION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristRoot");
			prf->m_vGroup2.push_back("RightWristRoot");
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = POSITION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristRoot");
			prf->m_vGroup2.push_back("RightWristRoot");
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = POSITION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristHead");
			prf->m_vGroup2.push_back("RightWristHead");
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = POSITION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristHead");
			prf->m_vGroup2.push_back("RightWristHead");
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
		}

		// 12 orientation features.
		if (m_bOri)
		{
			// 3 individual features for right palm.
			pif = new IndividualFeature();
			pif->m_nID = ++m_nID;
			pif->m_nSubset = ORIENTATION;
			pif->m_nIdentity = m_vIdentity.at(t);
			pif->m_strName = "RightPalm";
			pif->m_strAddInfo = "X";
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));

			pif = new IndividualFeature();
			pif->m_nID = ++m_nID;
			pif->m_nSubset = ORIENTATION;
			pif->m_nIdentity = m_vIdentity.at(t);
			pif->m_strName = "RightPalm";
			pif->m_strAddInfo = "Y";
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));

			pif = new IndividualFeature();
			pif->m_nID = ++m_nID;
			pif->m_nSubset = ORIENTATION;
			pif->m_nIdentity = m_vIdentity.at(t);
			pif->m_strName = "RightPalm";
			pif->m_strAddInfo = "Z";
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));

			// 3 individual features for left palm.
			pif = new IndividualFeature();
			pif->m_nID = ++m_nID;
			pif->m_nSubset = ORIENTATION;
			pif->m_nIdentity = m_vIdentity.at(t);
			pif->m_strName = "LeftPalm";
			pif->m_strAddInfo = "X";
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));

			pif = new IndividualFeature();
			pif->m_nID = ++m_nID;
			pif->m_nSubset = ORIENTATION;
			pif->m_nIdentity = m_vIdentity.at(t);
			pif->m_strName = "LeftPalm";
			pif->m_strAddInfo = "Y";
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));

			pif = new IndividualFeature();
			pif->m_nID = ++m_nID;
			pif->m_nSubset = ORIENTATION;
			pif->m_nIdentity = m_vIdentity.at(t);
			pif->m_strName = "LeftPalm";
			pif->m_strAddInfo = "Z";
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));

			// 6 relational features with both palms.
			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ORIENTATION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftPalm");
			prf->m_vGroup2.push_back("RightPalm");
			prf->m_strAddInfo = "X";
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ORIENTATION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftPalm");
			prf->m_vGroup2.push_back("RightPalm");
			prf->m_strAddInfo = "X";
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ORIENTATION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftPalm");
			prf->m_vGroup2.push_back("RightPalm");
			prf->m_strAddInfo = "Y";
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ORIENTATION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftPalm");
			prf->m_vGroup2.push_back("RightPalm");
			prf->m_strAddInfo = "Y";
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ORIENTATION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftPalm");
			prf->m_vGroup2.push_back("RightPalm");
			prf->m_strAddInfo = "Z";
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = ORIENTATION;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftPalm");
			prf->m_vGroup2.push_back("RightPalm");
			prf->m_strAddInfo = "Z";
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
		}

		// 18 movement features.
		if (m_bMov)
		{
			// 5 individual speed feature.
			for (int i=0; i<m_vPosStr.size(); i++)
			{
				pif = new IndividualFeature();
				pif->m_nID = ++m_nID;
				pif->m_nSubset = MOVEMENT;
				pif->m_nIdentity = m_vIdentity.at(t);
				pif->m_strName = m_vPosStr.at(i);
				pif->m_strAddInfo = "Speed";
				m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));
			}

			// 4 relational speed features.
			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = MOVEMENT;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristRoot");
			prf->m_vGroup2.push_back("RightWristRoot");
			prf->m_strAddInfo = "Speed";
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = MOVEMENT;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristRoot");
			prf->m_vGroup2.push_back("RightWristRoot");
			prf->m_strAddInfo = "Speed";
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = MOVEMENT;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristHead");
			prf->m_vGroup2.push_back("RightWristHead");
			prf->m_strAddInfo = "Speed";
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = MOVEMENT;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristHead");
			prf->m_vGroup2.push_back("RightWristHead");
			prf->m_strAddInfo = "Speed";
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			// 5 individual acceleration feature.
			for (int i=0; i<m_vPosStr.size(); i++)
			{
				pif = new IndividualFeature();
				pif->m_nID = ++m_nID;
				pif->m_nSubset = MOVEMENT;
				pif->m_nIdentity = m_vIdentity.at(t);
				pif->m_strName = m_vPosStr.at(i);
				pif->m_strAddInfo = "Acceleration";
				m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, pif));
			}

			// 4 relational acceleration features.
			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = MOVEMENT;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristRoot");
			prf->m_vGroup2.push_back("RightWristRoot");
			prf->m_strAddInfo = "Acceleration";
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = MOVEMENT;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristRoot");
			prf->m_vGroup2.push_back("RightWristRoot");
			prf->m_strAddInfo = "Acceleration";
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = MOVEMENT;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristHead");
			prf->m_vGroup2.push_back("RightWristHead");
			prf->m_strAddInfo = "Acceleration";
			prf->m_nOperation = 0;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));

			prf = new RelationalFeature();
			prf->m_nID = ++m_nID;
			prf->m_nSubset = MOVEMENT;
			prf->m_nIdentity = m_vIdentity.at(t);
			prf->m_vGroup1.push_back("LeftWristHead");
			prf->m_vGroup2.push_back("RightWristHead");
			prf->m_strAddInfo = "Acceleration";
			prf->m_nOperation = 1;
			m_mFeatureSet.insert(std::pair<int, Feature*>(m_nID, prf));
		}
	}
}