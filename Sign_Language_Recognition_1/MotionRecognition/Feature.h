/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/10/28                                                  */
/************************************************************************/

#pragma once

#ifndef _FEATURE_H_
#define _FEATURE_H_

#include <vector>
#include <string>

namespace Swift
{
	enum
	{
		FLEXION = 0,
		ABDUCTION,
		POSITION,
		ORIENTATION,
		MOVEMENT,
	};

	enum
	{
		MEAN = 10,
		VARIANCE,
		MARGIN,
		TURNINGPOINTS,
	};

	class Feature
	{
	public:
		Feature();
		virtual ~Feature()=0;

		/// The unique ID of the feature.
		int m_nID;
		/// The subset the feature belongs to. Its value could be FLEXION, ABDUCTION, POSITION, ORIENTATION, MOVEMENT.
		int m_nSubset;
		/// The type of the feature.0 indicates individual feature and 1 indicates relational feature.
		int m_nType;
		/// The identity of the feature. Its value could be MEAN, VARIANCE, MARGIN, TURNINGPOINTS.
		int m_nIdentity;
		/// The additional information, which will be used by orientation feature and movement feature.
		std::string m_strAddInfo;
	};	// Feature

	class IndividualFeature : public Feature
	{
	public:
		IndividualFeature();
		~IndividualFeature();

		/// Feature name.
		std::string m_strName;
	};

	class RelationalFeature : public Feature
	{
	public:
		RelationalFeature();
		~RelationalFeature();

		/// The first group of individual features.
		std::vector<std::string> m_vGroup1;
		/// The second group of individual features.
		std::vector<std::string> m_vGroup2;
		/// The operation between group1 and group2. There are only two possible values, 0 and 1, where 0 indicates plus and 1 indicates subtract.
		int m_nOperation;
	};
}

#endif	// _FEATURE_H_