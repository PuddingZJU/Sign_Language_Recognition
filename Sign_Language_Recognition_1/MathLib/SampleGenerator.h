/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/08/18                                                  */
/************************************************************************/

#ifndef _SAMPLEGENERATOR_H_
#define _SAMPLEGENERATOR_H_

namespace Swift
{
	class SampleGenerator
	{
	public:
		SampleGenerator();
		~SampleGenerator();

	public:
		/// Generate a random number between [-threshold, threshold]
		float random(float dThreshold);
		/// Generate random number which expectation is 0 and variance is dDeviation.
		float gaussrand(float dSigma);
		/// Compute the range of uniform noise given the SNR.
		void computeUniformNoiseThreshold(float** pData, int nSamples, int nDimension, float* pThreshold, float dUniSNR=5.0);
		/// Compute the deviation of guass noise given the SNR.
		void computeGuassNoiseSigma(float** pData, int nSamples, int nDimension, float* pSigma, float dGuassSNR=3.0);
		void generateByAddingUniformNoise(float** pOriginalData, float** pData, int nSamples, int nDimension, float* pThreshold);
		void generateByAddingGuassNoise(float** pOriginalData, float** pData, int nSamples, int nDimension, float* pSigma);

	protected:
		float deviation(float* pData, int nSize);

	};	// SampleGenerator
}	// Swift

#endif	// _SAMPLEGENERATOR_H_