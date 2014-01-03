/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/15                                                  */
/************************************************************************/

#pragma once

#ifndef _SIGNSAMPLES_H_
#define _SIGNSAMPLES_H_

#include <vector>
#include <map>
#include "../MotionData/MotionClip.h"
#include "PriFeatureClip.h"
#include "FClassifier.h"

namespace Swift
{
	/// This class holds the training data and trained result for one sign word.
	class SignSamples
	{
	public:
		/// Constructor.
		SignSamples();
		/// Destructor.
		~SignSamples();

		void addSample(MotionClip* pmc);

	public:
		/// The attribute of the samples, where 0,1 for negative and positive examples.
		int m_nAttribute;
		/// The weights for each sample, which will be used in AdaBoost algorithm.
		std::vector<double> m_vWeight;
		/// The captured training motion clips of this sign.
		std::vector<MotionClip*> m_vpmc;
		/// The primary feature clips of this sign.
		std::vector<PriFeatureClip*> m_vppfc;
		/// The training samples for all features of this sign. It's a map between a feature ID and a vector of feature values.
		std::map<int, std::vector<double>> m_mFSamples;
	};	// SignSamples
}	// Swift

#endif	// _SIGNSAMPLES_H_