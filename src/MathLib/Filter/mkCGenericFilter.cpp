#include <sstream>

#include "MathLib/Filter/mkCGenericFilter.h"
#include "MathLib/Mathematics.h"

//#include <System/mkCMKMException.h>



namespace mkm
{
	//------------------------------------------------------------
	CGenericFilter::CGenericFilter() :
		_initialized(false), _cut_frequency(0), _sample_frequency(0), _order(0)
	{
	}

	//------------------------------------------------------------
	CGenericFilter::CGenericFilter(const unsigned int fc, const unsigned int fe, const unsigned int order) :
	_initialized(false), _cut_frequency(fc), _sample_frequency(fe), _order(order)
	{
		_initialized = true;
	}

	//------------------------------------------------------------
	CGenericFilter::~CGenericFilter()
	{
		close();
	}

	//------------------------------------------------------------
	void CGenericFilter::close()
	{
		if(_initialized)
		{
			_A_coefficients.clear();
			_B_coefficients.clear();
			_initialized = false;
		}
	}

	//------------------------------------------------------------
	const std::vector<double>& CGenericFilter::getACoefficients() const
	{
		//mkASSERT(_initialized, "CGenericFilter::getACoefficients : filter not initialized");
		return _A_coefficients;
	}

	//------------------------------------------------------------
	const std::vector<double>& CGenericFilter::getBCoefficients() const
	{
		//mkASSERT(_initialized, "CGenericFilter::getBCoefficients : filter not initialized");
		return _B_coefficients;
	}
	
	//------------------------------------------------------------
	void CGenericFilter::setFilterParameters(const unsigned int fc, const unsigned int fe, const unsigned int order)
	{
		if(_cut_frequency != fc || _sample_frequency != fe || _order != order || !_initialized)
		{
			_cut_frequency = fc;
			_sample_frequency = fe;
			_order = order;
			computeFilterCoefficients();
			_initialized = true;
		}
	}

	//---------------------------------------------------------
	double CGenericFilter::computeCurrentFilteredSampleFromRawSample(
		const double& sample, 
		const std::deque<double>& previousRaw, 
		const std::deque<double>& previousFiltered
		) const 
	{
		return computeCurrentFilteredSampleFromRawSample(sample, previousRaw, previousFiltered, _A_coefficients, _B_coefficients);
	}

	//------------------------------------------------------------
	Vector3 CGenericFilter::computeCurrentFilteredSampleFromRawSample(
		const Vector3& sample, 
		const std::deque<Vector3>& previousRaw, 
		const std::deque<Vector3>& previousFiltered
		) const
	{
		return computeCurrentFilteredSampleFromRawSample(sample, previousRaw, previousFiltered, _A_coefficients, _B_coefficients);
	}

	////------------------------------------------------------------
	//CQuaternion CGenericFilter::computeCurrentFilteredSampleFromRawSample(
	//	const CQuaternion& sample, 
	//	const std::deque<CQuaternion>& previousRaw, 
	//	const std::deque<CQuaternion>& previousFiltered
	//	) const
	//{
	//	return computeCurrentFilteredSampleFromRawSample(sample, previousRaw, previousFiltered, _A_coefficients, _B_coefficients);
	//}

	//---------------------------------------------------------
	double CGenericFilter::computeCurrentFilteredSampleFromRawSample(
		const double& sample, 
		const std::deque<double>& previousRaw, 
		const std::deque<double>& previousFiltered, 
		const std::vector<double>& coeffsA, 
		const std::vector<double>& coeffsB)
	{
		double filteredSample;

		unsigned int nbSamples = (unsigned int)previousRaw.size();
		unsigned int minFilter = Math::Min((int)nbSamples, (int)coeffsA.size()-1);

		filteredSample = coeffsB[0]*sample;
		for(unsigned int i = 1; i <= minFilter; ++i)
			filteredSample += coeffsB[i]*previousRaw[nbSamples-i] - coeffsA[i]*previousFiltered[nbSamples-i];
		
		return filteredSample;
	}

	//------------------------------------------------------------
	Vector3 CGenericFilter::computeCurrentFilteredSampleFromRawSample(
		const Vector3& sample, 
		const std::deque<Vector3>& previousRaw, 
		const std::deque<Vector3>& previousFiltered, 
		const std::vector<double>& coeffsA, 
		const std::vector<double>& coeffsB)
	{
		Vector3 filteredSample;

		unsigned int nbSamples = (unsigned int)previousRaw.size();
		unsigned int minFilter = Math::Min((int)nbSamples, (int)coeffsA.size()-1);

		filteredSample = coeffsB[0]*sample;
		for(unsigned int i = 1; i <= minFilter; ++i)
			filteredSample += coeffsB[i]*previousRaw[nbSamples-i] - coeffsA[i]*previousFiltered[nbSamples-i];
		
		return filteredSample;
	}

	////------------------------------------------------------------
	//CQuaternion CGenericFilter::computeCurrentFilteredSampleFromRawSample(
	//	const CQuaternion& sample, 
	//	const std::deque<CQuaternion>& previousRaw, 
	//	const std::deque<CQuaternion>& previousFiltered, 
	//	const std::vector<double>& coeffsA, 
	//	const std::vector<double>& coeffsB)
	//{
	//	CQuaternion tmpRaw, tmpFilt, tmp;

	//	unsigned int nbSamples = (unsigned int)previousRaw.size();
	//	unsigned int minFilter = Math::Min((int)nbSamples, (int)coeffsA.size()-1);

	//	tmpRaw.setAlphaQuat(sample, coeffsB[0]);
	//	for(unsigned int i = 1; i <= minFilter; ++i)
	//	{
	//		tmp.setAlphaQuat(previousRaw[nbSamples-i], coeffsB[i]);
	//		CQuaternion a = tmpRaw*tmp;
	//		tmpRaw *= tmp;
	//		tmp.setAlphaQuat(previousFiltered[nbSamples-i], coeffsA[i]);
	//		tmpFilt *= tmp;
	//	}
	//	return tmpRaw/tmpFilt;
	//}
} // end namespace mkm

