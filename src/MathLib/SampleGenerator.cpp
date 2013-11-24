/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/08/18                                                  */
/************************************************************************/
#include <ctime>
#include <stdlib.h>
#include <math.h>
#include "MathLib/SampleGenerator.h"
using namespace Swift;

SampleGenerator::SampleGenerator()
{

}

SampleGenerator::~SampleGenerator()
{

}

float SampleGenerator::random(float dThreshold)
{
	//time_t t;     
	static unsigned a = 0;

	if(a == 0)
		srand((unsigned)time(NULL)); 
	//return dThreshold * (float)rand() / (float)RAND_MAX - dThreshold * (float)rand() / (float)RAND_MAX;
	a =1;

	return 2 * dThreshold * (float)rand() / (float)RAND_MAX - dThreshold;
}

float SampleGenerator::gaussrand(float dSigma)
{
	static unsigned a = 0;

	if(a == 0)
		srand((unsigned)time(NULL)); 
	a =1;

	static float V1, V2, S;
	static int phase = 0;
	float X;

	if (phase == 0) 
	{
		do 
		{
			float U1 = (float)rand() / RAND_MAX;
			float U2 = (float)rand() / RAND_MAX;
			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} 
	else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	return dSigma * X;
}


void SampleGenerator::computeUniformNoiseThreshold(float** pData, int nSamples, int nDimension, float* pThreshold, float dUniSNR/* =5.0 */)
{
	float* pValue=new float[nSamples];
	for (int j=0; j<nDimension; j++)
	{
		for (int i=0; i<nSamples; i++)
		{
			pValue[i] = pData[i][j];
		}
		float d = deviation(pValue, nSamples);
		//均匀分布的方差为 a*a/3, 信噪比（即样本方差除噪音方差）取值为dUniSNR时计算得到a。
		pThreshold[j] = sqrt(3.0 * d / dUniSNR);
	}
	delete[] pValue;
}

void SampleGenerator::computeGuassNoiseSigma(float** pData, int nSamples, int nDimension, float* pSigma, float dGuassSNR/* =3.0 */)
{
	float* pValue=new float[nSamples];
	for (int j=0; j<nDimension; j++)
	{
		for (int i=0; i<nSamples; i++)
		{
			pValue[i] = pData[i][j];
		}
		float d = deviation(pValue, nSamples);
		//高斯分布的方差为 sigma*sigma,本高斯随机数函数生成的是[-1,+1]内的数，需要确定放大系数sigma。 信噪比（即样本方差除以噪音方差）取值为dGuassSNR时计算得到sigma。
		pSigma[j] = sqrt(d / dGuassSNR); 
	}
	delete[] pValue;
}

void SampleGenerator::generateByAddingUniformNoise(float** pOriginalData, float** pData, int nSamples, int nDimension, float* pThreshold)
{
	for (int j=0; j<nDimension; j++)
	{
		for (int i=0; i<nSamples; i++)
		{
			pData[i][j] = pOriginalData[i][j]+random(pThreshold[j]);
		}
	}
}

void SampleGenerator::generateByAddingGuassNoise(float** pOriginalData, float** pData, int nSamples, int nDimension, float* pSigma)
{
	for (int j=0; j<nDimension; j++)
	{
		for (int i=0; i<nSamples; i++)
		{
			pData[i][j] = pOriginalData[i][j]+gaussrand(pSigma[j]);
		}
	}
}

float SampleGenerator::deviation(float* pData, int nSize)
{
	float d1=0,d2=0;
	float x,y,z;
	for (int i=0; i<nSize; i++)
	{
		d1 += pData[i]*pData[i];
		d2 += pData[i];
	}

	return (d1 - d2*d2/nSize) / nSize;
}