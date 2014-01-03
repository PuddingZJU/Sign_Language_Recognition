/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/23                                                  */
/************************************************************************/

#pragma once

#ifndef _FEATUREPARSER_H_
#define _FEATUREPARSER_H_

#include "PriFeatureClip.h"
#include "Feature.h"

namespace Swift
{
	/// Parse some feature according to its unique ID.
	class FeatureParser
	{
	public:
		/// Constructor.
		FeatureParser();
		/// Destructor.
		~FeatureParser();

		/// Calculate the feature value of a specified feature.
		/** 
		* @param nID The unique ID of the feature.
		* @param ppfc The pointer to the input PriFeatureClip.
		* @param dValue The calculated feature value.
		* @return True if there is a feature with specified ID.
		*/
		bool calFeatureValue(int nID, PriFeatureClip* ppfc, double& dVaule);

		/// Find a feature given its ID.
		/** 
		* @nID The ID of the feature.
		* @return The pointer to the feature.
		*/
		Feature* findFeature(int nID);
	};
}

#endif