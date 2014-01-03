/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/10/26                                                  */
/************************************************************************/

#pragma once

#ifndef _FCLASSIFIER_H_
#define _FCLASSIFIER_H_

#include <vector>
#include <string>
#include "Feature.h"
#include "../MotionData/MotionClip.h"

namespace Swift
{
	/// The weak classifier based on simple features, e.g. features of hand shape, position, orientation and motion.
	/**
	* A weak classifier consists of a feature, a threshold, an operation and a parity.
	*/
	class FWeakClassifier
	{
	public:
		/// Constructor.
		FWeakClassifier();
		/// Constructor.
		FWeakClassifier(FWeakClassifier* pfwcf);
		/// Copy constructor.
		FWeakClassifier(FWeakClassifier& fwcf);
		/// Destructor.
		~FWeakClassifier();

		/// Recognize the feature value.
		/** 
		* @param dValue The feature value to be recognized.
		* @return The recognition result. 0, 1 mean the recognition results are negative or positive respectively.
		*/
		int recognize(double dValue);

		/// The simple feature used in this weak classifier.
		Feature* m_pFeature;
		/// This variable indicates the direction of the inequality sign. There are only two possible values, 0 and 1, where 0 means bigger than and 1 means smaller than.
		int m_nParity;
		/// The threshold value in the inequality. The best value will be selected during the training process.
		double m_dThreshold;
		/// The error of the weak classifier on the training data.
		double m_dError;
	};

	class FStrongClassifier
	{
	public:
		/// Constructor.
		FStrongClassifier();
		/// Destructor.
		~FStrongClassifier();

		/// Generate all the weak classifiers based on the simple features.
		void genCanFWeakClassifierSet();

		/// Recognize the feature value.
		/** 
		* @param pmc The motion to be recognized.
		* @return The recognition result. 0, 1 mean the recognition results are negative or positive respectively.
		*/
		int recognize(MotionClip* pmc);

		/// The weights of the selected weak classifiers.
		std::vector<double> m_vWeight;
		/// The selected weak classifiers to construct the strong classifier.
		std::vector<FWeakClassifier*> m_vFWeakClassifier;
		/// All the candidate weak classifiers.
		std::vector<FWeakClassifier*> m_vCanFWeakClassifier;
	};
}

#endif	// _FCLASSIFIER_H_