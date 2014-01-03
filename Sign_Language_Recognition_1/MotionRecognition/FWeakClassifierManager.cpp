/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/02                                                  */
/************************************************************************/

#include "../SwiftModel/SwiftModel.h"
#include "FeatureManager.h"
#include "FWeakClassifierManager.h"

using namespace Swift;

FWeakClassifierManager::FWeakClassifierManager()
{
	genFWeakClassifierSet();
}

FWeakClassifierManager::~FWeakClassifierManager()
{
	for (int i=0; i<m_vFWeakClassifier.size(); i++)
	{
		if (m_vFWeakClassifier.at(i))
		{
			delete m_vFWeakClassifier.at(i);
		}
	}
}

void FWeakClassifierManager::genFWeakClassifierSet()
{
	FWeakClassifier* pfwc;
	for (std::map<int, Feature*>::iterator it=SwiftModel::instance().m_pfm->m_mFeatureSet.begin(); it!=SwiftModel::instance().m_pfm->m_mFeatureSet.end(); it++)
	{
		pfwc = new FWeakClassifier();
		pfwc->m_pFeature = it->second;
		//pfwc->m_nParity = 0;
		m_vFWeakClassifier.push_back(pfwc);

		//pfwc = new FWeakClassifier();
		//pfwc->m_pFeature = it->second;
		//pfwc->m_nParity = 1;
		//m_vFWeakClassifier.push_back(pfwc);
	}
}