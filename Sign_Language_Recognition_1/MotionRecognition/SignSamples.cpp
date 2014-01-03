/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/15                                                  */
/************************************************************************/

#include "../SwiftModel/SwiftModel.h"
#include "FeatureParser.h"
#include "SignSamples.h"
using namespace Swift;

SignSamples::SignSamples()
: m_nAttribute(0)
{
	for (std::map<int, Feature*>::iterator it=SwiftModel::instance().m_pfm->m_mFeatureSet.begin(); it!=SwiftModel::instance().m_pfm->m_mFeatureSet.end(); it++)
	{
		std::vector<double> vFeatureValue;
		m_mFSamples.insert(std::pair<int, std::vector<double>>(it->first, vFeatureValue));
	}
}

SignSamples::~SignSamples()
{

}

void SignSamples::addSample(MotionClip *pmc)
{
	double dValue;
	FeatureParser fp;
	m_vWeight.push_back(1.0);
	m_vpmc.push_back(pmc);
	PriFeatureClip* ppfc = new PriFeatureClip(pmc);
	ppfc->calFromMC();
	m_vppfc.push_back(ppfc);
	for (std::map<int, std::vector<double>>::iterator it=m_mFSamples.begin(); it!=m_mFSamples.end(); it++)
	{
		if (fp.calFeatureValue(it->first, ppfc, dValue))
		{
			it->second.push_back(dValue);
		}
	}
}