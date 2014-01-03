/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/10/26                                                  */
/************************************************************************/

#include "../SwiftModel/SwiftModel.h"
#include "PriFeatureClip.h"
#include "FeatureParser.h"
#include "FClassifier.h"

using namespace Swift;

FWeakClassifier::FWeakClassifier()
: m_pFeature(NULL), m_nParity(-1), m_dThreshold(-1.0), m_dError(-1.0)
{

}

FWeakClassifier::FWeakClassifier(FWeakClassifier* pfwcf)
{
	if (pfwcf)
	{
		m_pFeature = pfwcf->m_pFeature;
		m_nParity = pfwcf->m_nParity;
		m_dThreshold = pfwcf->m_dThreshold;
		m_dError = pfwcf->m_dError;
	}
	else
	{
		m_pFeature = NULL;
		m_nParity = 0;
		m_dThreshold = 0.0;
		m_dError = 0.0;
	}
}

FWeakClassifier::FWeakClassifier(FWeakClassifier& fwcf)
{
	m_pFeature = fwcf.m_pFeature;
	m_nParity = fwcf.m_nParity;
	m_dThreshold = fwcf.m_dThreshold;
	m_dError = fwcf.m_dError;
}

FWeakClassifier::~FWeakClassifier()
{

}

int FWeakClassifier::recognize(double dValue)
{
	if (m_nParity == 0)
	{
		if (dValue > m_dThreshold)
			return 1;
		else
			return 0;
	}
	else if (m_nParity == 1)
	{
		if (dValue < m_dThreshold)
			return 1;
		else
			return 0;
	}

	return 0;
}

FStrongClassifier::FStrongClassifier()
{

}

FStrongClassifier::~FStrongClassifier()
{
	for (int i=0; i<m_vCanFWeakClassifier.size(); i++)
	{
		if (m_vCanFWeakClassifier.at(i))
		{
			delete m_vCanFWeakClassifier.at(i);
		}
	}

	for (int i=0; i<m_vFWeakClassifier.size(); i++)
	{
		if (m_vFWeakClassifier.at(i))
		{
			delete m_vFWeakClassifier.at(i);
		}
	}
}

void FStrongClassifier::genCanFWeakClassifierSet()
{
	FWeakClassifier* pfwc;
	for (std::map<int, Feature*>::iterator it=SwiftModel::instance().m_pfm->m_mFeatureSet.begin(); it!=SwiftModel::instance().m_pfm->m_mFeatureSet.end(); it++)
	{
		pfwc = new FWeakClassifier();
		pfwc->m_pFeature = it->second;
		//pfwc->m_nParity = 0;
		m_vCanFWeakClassifier.push_back(pfwc);
	}
}

int FStrongClassifier::recognize(MotionClip* pmc)
{
	if (m_vWeight.size() != m_vFWeakClassifier.size())
		return -1;

	PriFeatureClip* ppfc = new PriFeatureClip(pmc);
	ppfc->calFromMC();

	double d1=0.0, d2=0.0;
	double dValue;
	FeatureParser fp;
	for (int i=0; i<m_vWeight.size(); i++)
	{
		fp.calFeatureValue(m_vFWeakClassifier.at(i)->m_pFeature->m_nID, ppfc, dValue);
		d1 += m_vWeight.at(i) * m_vFWeakClassifier.at(i)->recognize(dValue);
		d2 += m_vWeight.at(i);
	}
	d2 *= 0.5;

	if (d1 < d2)
		return 0;
	else
		return 1;
}