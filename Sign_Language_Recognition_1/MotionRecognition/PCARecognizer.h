/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/04/08                                                  */
/************************************************************************/

#pragma once

#ifndef _PCARECOGNIZER_H_
#define _PCARECOGNIZER_H_

#include <map>
#include "../MathLib/Math.h"
#include "../MathLib/pca/pca.h"
#include "../MathLib/Filter/mkCDynamicMatrix.h"

namespace Swift
{
	class MotionClip;

	class PCARecognizer
	{
	public:
		PCARecognizer();
		~PCARecognizer();

		void test(std::vector<std::string> vFiles);
		std::string recognize(std::string strFile);

	private:
		std::map<std::string, pca*> m_mpca;
		std::map<std::string, MotionClip*> m_mMotion;
		std::map<std::string, mkm::CDynamicMatrix<double>> m_mMatrix;
		std::map<std::string, vector<double>> m_mMean;
	};

}	// Swift

#endif	// _PCARECOGNIZER_H_