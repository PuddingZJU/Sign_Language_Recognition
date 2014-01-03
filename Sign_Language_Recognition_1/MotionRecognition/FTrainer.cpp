/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/10/26                                                  */
/************************************************************************/

#include <fstream>
#include "FWeakClassifierManager.h"
#include "FTrainer.h"

using namespace Swift;

FTrainer::FTrainer()
{

}

FTrainer::~FTrainer()
{

}

FStrongClassifier* FTrainer::train(std::string strPosSign, std::vector<std::string> vNegSign, int nNum/*=100*/, double dCoef/*=0.2*/)
{
	FStrongClassifier* pfsc = new FStrongClassifier();
	pfsc->genCanFWeakClassifierSet();

	SignSamples* pPosSamples;
	std::vector<SignSamples*> vNegSamples;
	std::map<std::string, SignSamples*>::iterator it;

	// Set attribute of the samples, where 0,1 for negative and positive examples.
	it = SwiftModel::instance().m_mSignSamples.find(strPosSign);
	if (it == SwiftModel::instance().m_mSignSamples.end())
		return NULL;
	pPosSamples = it->second;
	pPosSamples->m_nAttribute = 1;
	for (int i=0; i<vNegSign.size(); i++)
	{
		it = SwiftModel::instance().m_mSignSamples.find(vNegSign.at(i));
		if (it != SwiftModel::instance().m_mSignSamples.end())
		{
			it->second->m_nAttribute = 0;
			vNegSamples.push_back(it->second);
		}
	}

	// Initialize weights.
	for (int i=0; i<pPosSamples->m_vWeight.size(); i++)
	{
		pPosSamples->m_vWeight.at(i) = 0.5 / (double)pPosSamples->m_vWeight.size();
	}
	int nSize=0;
	for (int i=0; i<vNegSamples.size(); i++)
	{
		nSize += vNegSamples.at(i)->m_vWeight.size();
	}
	for (int i=0; i<vNegSamples.size(); i++)
	{
		for (int j=0; j<vNegSamples.at(i)->m_vWeight.size(); j++)
		{
			vNegSamples.at(i)->m_vWeight.at(j) = 0.5 / (double)nSize;
		}
	}

	// Train each weak classifier.
	for (int i=0; i<pfsc->m_vCanFWeakClassifier.size(); i++)
	{
		train(pfsc->m_vCanFWeakClassifier.at(i), pPosSamples, vNegSamples, dCoef);
	}

	for (int t=0; t<nNum; t++)
	{
		double dSumWeight = 0.0;
		// Normalize the weights, so that wi is a probability distribution.
		// Calculate sum weight of positive samples.
		for (int i=0; i<pPosSamples->m_vWeight.size(); i++)
		{
			dSumWeight += pPosSamples->m_vWeight.at(i);
		}
		// Calculate sum weight of negative samples.
		for (int i=0; i<vNegSamples.size(); i++)
		{
			for (int j=0; j<vNegSamples.at(i)->m_vWeight.size(); j++)
			{
				dSumWeight += vNegSamples.at(i)->m_vWeight.at(j);
			}
		}

		// Normalize the weights for positive samples.
		for (int i=0; i<pPosSamples->m_vWeight.size(); i++)
		{
			pPosSamples->m_vWeight.at(i) /= dSumWeight;
		}
		// Normalize the weights for negative samples.
		for (int i=0; i<vNegSamples.size(); i++)
		{
			for (int j=0; j<vNegSamples.at(i)->m_vWeight.size(); j++)
			{
				vNegSamples.at(i)->m_vWeight.at(j) /= dSumWeight;
			}
		}

		// Choose the best weak classifier.
		double dminerror = 1.0e10;
		FWeakClassifier *pfwc, *ptmpfwc=NULL, *pselectedfwc=NULL;
		for (int i=0; i<pfsc->m_vCanFWeakClassifier.size(); i++)
		{
			pfwc = pfsc->m_vCanFWeakClassifier.at(i);
			if (isSelected(pfwc,pfsc))
				continue;
			//train(pfwc, pPosSamples, vNegSamples);
			std::map<int, std::vector<double>>::iterator it = pPosSamples->m_mFSamples.find(pfwc->m_pFeature->m_nID);
			std::map<int, std::vector<double>>::iterator it2;
			pfwc->m_dError = 0.0;
			for (int i=0; i<it->second.size(); i++)
			{
				pfwc->m_dError += pPosSamples->m_vWeight.at(i) * std::abs(pfwc->recognize(it->second.at(i)) - pPosSamples->m_nAttribute);
			}
			for (int i=0; i<vNegSamples.size(); i++)
			{
				it2 = vNegSamples.at(i)->m_mFSamples.find(pfwc->m_pFeature->m_nID);
				for (int j=0; j<it2->second.size(); j++)
				{
					pfwc->m_dError += vNegSamples.at(i)->m_vWeight.at(j) * std::abs(pfwc->recognize(it2->second.at(j)) - vNegSamples.at(i)->m_nAttribute);
				}
			}
			if (pfwc->m_dError < dminerror)
			{
				if (pfwc->m_dError < 1.0e-10)
					pfwc->m_dError = 1.0e-10;
				dminerror = pfwc->m_dError;
				ptmpfwc = pfwc;
			}
		}

		if (!ptmpfwc)
		{
			return pfsc;
		}
		pselectedfwc = new FWeakClassifier(ptmpfwc);

		double beta = dminerror / (1-dminerror);
		int r;

		std::map<int, std::vector<double>>::iterator it = pPosSamples->m_mFSamples.find(pselectedfwc->m_pFeature->m_nID);
		std::map<int, std::vector<double>>::iterator it2;
		for (int i=0; i<it->second.size(); i++)
		{
			r = pselectedfwc->recognize(it->second.at(i));
			if (r == 1)
				pPosSamples->m_vWeight.at(i) =  pPosSamples->m_vWeight.at(i) * beta;
		}
		for (int i=0; i<vNegSamples.size(); i++)
		{
			it2 = vNegSamples.at(i)->m_mFSamples.find(pselectedfwc->m_pFeature->m_nID);
			for (int j=0; j<it2->second.size(); j++)
			{
				r = pselectedfwc->recognize(it2->second.at(j));
				if (r == 1)	
					vNegSamples.at(i)->m_vWeight.at(j) = vNegSamples.at(i)->m_vWeight.at(j) * beta;
			}
		}

		double alpha = std::log(1.0 / beta);
		pfsc->m_vFWeakClassifier.push_back(pselectedfwc);
		pfsc->m_vWeight.push_back(alpha);
	}

	//std::string str = strPosSign+"_error.txt";
	//std::ofstream out(str.c_str());
	//out << "strPosSign:" << std::endl;
	//for (int i=0; i<pfsc->m_vFWeakClassifier.size(); i++)
	//{
	//	out << pfsc->m_vFWeakClassifier.at(i)->m_pFeature->m_nID << ": " << pfsc->m_vWeight.at(i) << ": " << pfsc->m_vFWeakClassifier.at(i)->m_dError << std::endl;
	//}
	//out.close();

	return pfsc;
}

void FTrainer::train(FWeakClassifier* pfwc, SignSamples* pPosSamples, std::vector<SignSamples*> vNegSamples, double dCoef/*=0.2*/)
{
	// Train the weak classifier.
	std::map<int, std::vector<double>>::iterator it = pPosSamples->m_mFSamples.find(pfwc->m_pFeature->m_nID);
	std::map<int, std::vector<double>>::iterator it2;

	double dmin=1.0e10, dmax=-1.0e10;
	for (int i=0; i<it->second.size(); i++)
	{
		if (dmin > it->second.at(i))
			dmin = it->second.at(i);
		if (dmax < it->second.at(i))
			dmax = it->second.at(i);
	}

	double dError1=0.0, dError2=0.0;
	double delta = dmax-dmin;
	if (delta<1.0e-5)
		delta = 1.0e-5;

	pfwc->m_nParity = 0;
	pfwc->m_dThreshold = dmin-dCoef*delta;
	for (int i=0; i<it->second.size(); i++)
	{
		dError1 += pPosSamples->m_vWeight.at(i) * std::abs(pfwc->recognize(it->second.at(i)) - pPosSamples->m_nAttribute);
	}
	for (int i=0; i<vNegSamples.size(); i++)
	{
		it2 = vNegSamples.at(i)->m_mFSamples.find(pfwc->m_pFeature->m_nID);
		for (int j=0; j<it2->second.size(); j++)
		{
			dError1 += vNegSamples.at(i)->m_vWeight.at(j) * std::abs(pfwc->recognize(it2->second.at(j)) - vNegSamples.at(i)->m_nAttribute);
		}
	}

	pfwc->m_nParity = 1;
	pfwc->m_dThreshold = dmax+dCoef*delta;
	for (int i=0; i<it->second.size(); i++)
	{
		dError2 += pPosSamples->m_vWeight.at(i) * std::abs(pfwc->recognize(it->second.at(i)) - pPosSamples->m_nAttribute);
	}
	for (int i=0; i<vNegSamples.size(); i++)
	{
		it2 = vNegSamples.at(i)->m_mFSamples.find(pfwc->m_pFeature->m_nID);
		for (int j=0; j<it2->second.size(); j++)
		{
			dError2 += vNegSamples.at(i)->m_vWeight.at(j) * std::abs(pfwc->recognize(it2->second.at(j)) - vNegSamples.at(i)->m_nAttribute);
		}
	}

	if (dError1<dError2)
	{
		pfwc->m_nParity = 0;
		pfwc->m_dThreshold = dmin-dCoef*delta;
		pfwc->m_dError = dError1;
	}
	else
	{
		pfwc->m_nParity = 1;
		pfwc->m_dThreshold = dmax+dCoef*delta;
		pfwc->m_dError = dError2;
	}
	

	//if (pfwc->m_nParity == 0)
	//	pfwc->m_dThreshold = dmin-0.1*(dmax-dmin);
	//else
	//	pfwc->m_dThreshold = dmax+0.1*(dmax-dmin);

	//// Calculate the error of the weak classifier.
	//pfwc->m_dError = 0.0;
	//for (int i=0; i<it->second.size(); i++)
	//{
	//	pfwc->m_dError += pPosSamples->m_vWeight.at(i) * std::abs(pfwc->recognize(it->second.at(i)) - pPosSamples->m_nAttribute);
	//}
	//std::map<int, std::vector<double>>::iterator it2;
	//for (int i=0; i<vNegSamples.size(); i++)
	//{
	//	it2 = vNegSamples.at(i)->m_mFSamples.find(pfwc->m_pFeature->m_nID);
	//	for (int j=0; j<it2->second.size(); j++)
	//	{
	//		pfwc->m_dError += vNegSamples.at(i)->m_vWeight.at(j) * std::abs(pfwc->recognize(it2->second.at(j)) - vNegSamples.at(i)->m_nAttribute);
	//	}
	//}
}

bool FTrainer::isSelected(FWeakClassifier* pfwc, FStrongClassifier* pfsc)
{
	if(!pfwc || !pfsc)
		return false;

	int nwid = pfwc->m_pFeature->m_nID;
	for (int i=0; i<pfsc->m_vFWeakClassifier.size(); i++)
	{
		if (nwid == pfsc->m_vFWeakClassifier.at(i)->m_pFeature->m_nID)
			return true;
	}

	return false;
}