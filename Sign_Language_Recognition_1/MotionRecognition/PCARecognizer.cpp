/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/04/08                                                  */
/************************************************************************/

#include <ostream>
#include <limits>
#include "../MotionData/MotionClip.h"
#include "../MotionData/BVHReader.h"
#include "PCARecognizer.h"

using namespace Swift;

int nOldDimension = 12;
int nNewDimension = 1;

PCARecognizer::PCARecognizer()
{

}

PCARecognizer::~PCARecognizer()
{

}

void PCARecognizer::test(std::vector<std::string> vFiles)
{
	for (std::map<std::string, pca*>::iterator it=m_mpca.begin(); it!=m_mpca.end(); it++)
	{
		delete it->second;
	}
	m_mpca.clear();
	for (std::map<std::string, MotionClip*>::iterator it=m_mMotion.begin(); it!=m_mMotion.end(); it++)
	{
		delete it->second;
	}
	m_mMotion.clear();

	m_mMatrix.clear();

	m_mMean.clear();

	MotionClip* pmc;
	MotionJoint* pmj;
	Vector3 vPos;
	for (int i=0; i<vFiles.size(); i++)
	{
		BVHReader reader(vFiles.at(i));
		reader.read();
		pmc = reader.result();

		if (!pmc)
		{
			continue;
		}
		pca *pcadreducer = new pca(nOldDimension);
		int nFrameCount = pmc->getFrameCount();
		float **pData = new float*[nFrameCount];
		mkm::CDynamicMatrix<double> originalMatrix(nOldDimension, nFrameCount);

		std::vector<double> vMean;
		for (int t=0; t<nOldDimension; t++)
		{
			vMean.push_back(0.0);
		}
		for (int j=0; j<nFrameCount; j++)
		{
			pData[j] = new float[nOldDimension];

			//pmj = pmc->findJoint("Hips");
			////pmj = pmc->findJoint("root");
			//pmj->setOrientation(Quaternion::IDENTITY, j);
			//pmj->setAbsolutePosition(Vector3::ZERO, j);

			//pmj = pmc->findJoint("LeftWrist");
			pmj = pmc->findJoint("l_wrist");
			vPos = pmj->getAbsolutePosition(j);
			pData[j][0] = vPos.x();
			pData[j][1] = vPos.y();
			pData[j][2] = vPos.z();
			originalMatrix[0][j] = vPos.x();
			originalMatrix[1][j] = vPos.y();
			originalMatrix[2][j] = vPos.z();
			vMean.at(0) += vPos.x();
			vMean.at(1) += vPos.y();
			vMean.at(2) += vPos.z();

			//pmj = pmc->findJoint("RightWrist");
			pmj = pmc->findJoint("l_elbow");
			vPos = pmj->getAbsolutePosition(j);
			pData[j][3] = vPos.x();
			pData[j][4] = vPos.y();
			pData[j][5] = vPos.z();
			originalMatrix[3][j] = vPos.x();
			originalMatrix[4][j] = vPos.y();
			originalMatrix[5][j] = vPos.z();
			vMean.at(3) += vPos.x();
			vMean.at(4) += vPos.y();
			vMean.at(5) += vPos.z();

			//pmj = pmc->findJoint("LeftAnkle");
			pmj = pmc->findJoint("r_wrist");
			vPos = pmj->getAbsolutePosition(j);
			pData[j][6] = vPos.x();
			pData[j][7] = vPos.y();
			pData[j][8] = vPos.z();
			originalMatrix[6][j] = vPos.x();
			originalMatrix[7][j] = vPos.y();
			originalMatrix[8][j] = vPos.z();
			vMean.at(6) += vPos.x();
			vMean.at(7) += vPos.y();
			vMean.at(8) += vPos.z();

			//pmj = pmc->findJoint("RightAnkle");
			pmj = pmc->findJoint("r_elbow");
			vPos = pmj->getAbsolutePosition(j);
			pData[j][9] = vPos.x();
			pData[j][10] = vPos.y();
			pData[j][11] = vPos.z();
			originalMatrix[9][j] = vPos.x();
			originalMatrix[10][j] = vPos.y();
			originalMatrix[11][j] = vPos.z();
			vMean.at(9) += vPos.x();
			vMean.at(10) += vPos.y();
			vMean.at(11) += vPos.z();

		}

		for (int t=0; t<nOldDimension; t++)
		{
			vMean.at(t) /= nFrameCount;
		}
		m_mMean.insert(std::pair<std::string, vector<double>>(vFiles.at(i), vMean));
		for (int s=0; s<nFrameCount; s++)
		{
			//for (int t=0; t<nOldDimension; t++)
			//{
			//	pData[s][t] -= vMean.at(t);
			//}
			pcadreducer->addVector(pData[s]);
		}

		pcadreducer->pcaCal();

		float* eigenvector = pcadreducer->getEigenvector();
		float* eigenvalue = pcadreducer->getEigenvalue();
		std::vector<float> vTest;
		for (int m=0; m<nOldDimension; m++)
		{
			vTest.push_back(eigenvalue[m]);
		}

		mkm::CDynamicMatrix<double> eigenvectorMatrix(nNewDimension, nOldDimension);
		for (int k=0; k<nNewDimension; k++)
		{
			for (int l=0; l<nOldDimension; l++)
			{
				eigenvectorMatrix[k][l] = eigenvector[k*nOldDimension+l];
			}
		}
		m_mMatrix.insert(std::pair<std::string, mkm::CDynamicMatrix<double>>(vFiles.at(i), eigenvectorMatrix));
		for (int k=0; k<nOldDimension; k++)
		{
			for (int l=0; l<nFrameCount; l++)
			{
				originalMatrix[k][l] = originalMatrix[k][l] - vMean.at(k);
			}
		}
		
		mkm::CDynamicMatrix<double> finalMatrix(nNewDimension, nFrameCount);
		finalMatrix = eigenvectorMatrix * originalMatrix;

		//float** b = new float*[nFrameCount];

		//for (int j=0; j<nFrameCount; j++)
		//{
		//	b[j] = new float[nNewDimension];

		//	for (int l=0; l<nNewDimension; l++)
		//	{
		//		b[j][l] = 0.0;
		//		for (int k=0; k<nOldDimension; k++)
		//		{
		//			b[j][l] += eigenvector[l*nOldDimension+k]*(pData[j][k]-pcadreducer->mean[k]);
		//		}
		//	}
		//}

		//////////////////////////////////////////////////////////////////////////
		//test code
		//mkm::CDynamicMatrix<double> inverseEigenMatrix(nOldDimension,nNewDimension);
		//inverseEigenMatrix = eigenvectorMatrix.transpose();
		//
		//mkm::CDynamicMatrix<double> originalMatrix2(nOldDimension, nFrameCount);
		//originalMatrix2 = inverseEigenMatrix * finalMatrix;
		//for (int k=0; k<nOldDimension; k++)
		//{
		//	for (int l=0; l<nFrameCount; l++)
		//	{
		//		originalMatrix[k][l] = originalMatrix[k][l] + vMean.at(k);
		//		originalMatrix2[k][l] = originalMatrix2[k][l] + vMean.at(k);
		//	}
		//}
		//std::ofstream file("tmp.txt", std::ios::app);
		//file << "Matrix: ";
		//for (int j=0; j<nFrameCount; j++)
		//{
		//	for (int i=0; i<nNewDimension; i++)
		//	{
		//		file << finalMatrix[i][j] << " ";
		//	}
		//}
		//file << std::endl;
		//file << "B     : ";
		//for (int i=0; i<nFrameCount; i++)
		//{
		//	for (int j=0; j<nNewDimension; j++)
		//	{
		//		file << b[i][j] << " ";
		//	}
		//}
		//file << std::endl;

		//file << "Original : ";
		//for (int j=0; j<nFrameCount; j++)
		//{
		//	for (int i=0; i<nOldDimension; i++)
		//	{
		//		file << originalMatrix[i][j] << " ";
		//	}
		//}
		//file << std::endl;
		//file << "Original2: ";
		//for (int j=0; j<nFrameCount; j++)
		//{
		//	for (int i=0; i<nOldDimension; i++)
		//	{
		//		file << originalMatrix2[i][j] << " ";
		//	}
		//}
		//file << std::endl;
		//file.close();
		//////////////////////////////////////////////////////////////////////////

		m_mpca.insert(std::pair<std::string, pca*>(pmc->getFileName(),pcadreducer));

		for(int n= 0; n<nFrameCount; n++)
		{
			//delete []b[n];
			delete []pData[n];
		}
		//delete []b;
		delete []pData;
	}
}

std::string PCARecognizer::recognize(std::string strFile)
{
	MotionClip* pmc;
	MotionJoint* pmj;
	Vector3 vPos;
	BVHReader reader(strFile);
	reader.read();
	pmc = reader.result();
	std::string strResult = "";

	if (!pmc)
	{
		return strResult;
	}
	int nFrameCount = pmc->getFrameCount();
	mkm::CDynamicMatrix<double> originalMatrix(nOldDimension, nFrameCount);
	for (int j=0; j<nFrameCount; j++)
	{
		//pmj = pmc->findJoint("Hips");
		////pmj = pmc->findJoint("root");
		//pmj->setOrientation(Quaternion::IDENTITY, j);
		//pmj->setAbsolutePosition(Vector3::ZERO, j);

		//pmj = pmc->findJoint("LeftWrist");
		pmj = pmc->findJoint("l_wrist");
		vPos = pmj->getAbsolutePosition(j);
		originalMatrix[0][j] = vPos.x();
		originalMatrix[1][j] = vPos.y();
		originalMatrix[2][j] = vPos.z();

		//pmj = pmc->findJoint("RightWrist");
		pmj = pmc->findJoint("l_elbow");
		vPos = pmj->getAbsolutePosition(j);
		originalMatrix[3][j] = vPos.x();
		originalMatrix[4][j] = vPos.y();
		originalMatrix[5][j] = vPos.z();

		//pmj = pmc->findJoint("LeftAnkle");
		pmj = pmc->findJoint("r_wrist");
		vPos = pmj->getAbsolutePosition(j);
		originalMatrix[6][j] = vPos.x();
		originalMatrix[7][j] = vPos.y();
		originalMatrix[8][j] = vPos.z();

		//pmj = pmc->findJoint("RightAnkle");
		pmj = pmc->findJoint("r_elbow");
		vPos = pmj->getAbsolutePosition(j);
		originalMatrix[9][j] = vPos.x();
		originalMatrix[10][j] = vPos.y();
		originalMatrix[11][j] = vPos.z();
	}


	std::ofstream file("tmp.txt", std::ios::app);
	file << "Input:  " << strFile << std::endl;

	double dMinDistance = DBL_MAX;
	for (std::map<std::string, vector<double>>::iterator it=m_mMean.begin(); it!=m_mMean.end(); it++)
	{
		mkm::CDynamicMatrix<double> tempMatrix = originalMatrix;
		for (int k=0; k<nOldDimension; k++)
		{
			for (int l=0; l<nFrameCount; l++)
			{
				tempMatrix[k][l] = originalMatrix[k][l] - it->second.at(k);
			}
		}

		std::map<std::string, mkm::CDynamicMatrix<double>>::iterator it2=m_mMatrix.find(it->first);

		mkm::CDynamicMatrix<double> finalMatrix(nNewDimension, nFrameCount);
		finalMatrix = it2->second * tempMatrix;

		mkm::CDynamicMatrix<double> inverseEigenMatrix(nOldDimension,nNewDimension);
		inverseEigenMatrix = it2->second.transpose();

		mkm::CDynamicMatrix<double> originalMatrix2(nOldDimension, nFrameCount);
		originalMatrix2 = inverseEigenMatrix * finalMatrix;

		double dDistance = 0.0;
		for (int k=0; k<nOldDimension; k++)
		{
			for (int l=0; l<nFrameCount; l++)
			{
				originalMatrix2[k][l] = originalMatrix2[k][l] + it->second.at(k);
				dDistance += (originalMatrix2[k][l] - originalMatrix[k][l]) * (originalMatrix2[k][l] - originalMatrix[k][l]);
			}
		}

		if (dDistance < dMinDistance)
		{
			dMinDistance = dDistance;
			strResult = it->first;

		}

		//file << it->first << ": " << dDistance << std::endl;
	}

	file << "Result: " << strResult << std::endl << std::endl;
	file.close();

	return strResult;
}