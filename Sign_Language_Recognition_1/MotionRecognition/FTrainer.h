/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/10/26                                                  */
/************************************************************************/

#pragma once

#ifndef _FTRAINER_H_
#define _FTRAINER_H_

#include <string>
#include "../SwiftModel/SwiftModel.h"
#include "FClassifier.h"

namespace Swift
{
	/// Responsible for training weak classifiers and generate the strong classifier based on simple features.
	class FTrainer
	{
	public:
		/// Constructor.
		/**
		* @param strClassName The name of the specified class.
		*/
		FTrainer();
		/// Destructor.
		~FTrainer();

		/// Train the weak classifiers and generate the strong classifier based on AdaBoost.
		/** 
		* @param strPosSign The name of the sign whose samples are positive.
		* @param vNegSign The names of the signs whose samples are negative.
		* @param nNum The number of the selected weak classifiers.
		* @param dCoef The coefficient used in each weak classifier.
		* @return The generated strong classifier.
		*/
		FStrongClassifier* train(std::string strPosSign, std::vector<std::string> vNegSign, int nNum=100, double dCoef=0.2);

	protected:
		/// Train a weak classifier with positive and negative samples.
		/** 
		* @param pfwc The weak classifier needing to be trained.
		* @param strPosSign The name of the sign whose samples are positive.
		* @param vNegSign The names of the signs whose samples are negative.
		* @param dCoef The coefficient used in each weak classifier.
		*/
		void train(FWeakClassifier* pfwc, SignSamples* pPosSamples, std::vector<SignSamples*> vNegSamples, double dCoef=0.2);
		/// To judge if a weak classifier is already selected in a strong classifier.
		/**
		* @param pfwc The weak classifier needing to be judged.
		* @param pfsc The strong classifier composed of weak classifiers.
		* @return True if the weak classifier is already selected in the strong classifier.
		*/
		bool isSelected(FWeakClassifier* pfwc, FStrongClassifier* pfsc);
	};
}

#endif	// _FTRAINER_H_