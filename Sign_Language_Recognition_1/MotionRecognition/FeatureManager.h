/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/02                                                  */
/************************************************************************/

#pragma once

#ifndef _FEATUREMANAGER_H_
#define _FEATUREMANAGER_H_

#include <string>
#include <vector>
#include <map>
#include "../Util/Singleton.h"
#include "Feature.h"

namespace Swift
{
	/// This class responds to manage the whole feature set. There should be only one instance of it.
	class FeatureManager
	{
	public:
		FeatureManager();
		~FeatureManager();

		/// Generate all the features.
		void genFeatureSet();

		int m_nID;
		bool m_bFlex;
		bool m_bAbd;
		bool m_bPos;
		bool m_bOri;
		bool m_bMov;
		std::vector<int> m_vIdentity;
		std::vector<std::string> m_vRightFlexStr;
		std::vector<std::string> m_vLeftFlexStr;
		std::vector<std::string> m_vRightAbdStr;
		std::vector<std::string> m_vLeftAbdStr;
		std::vector<std::string> m_vPosStr;
		std::vector<std::string> m_vOriStr;

		/// All the simple features. The map between feature IDs and features.
		std::map<int, Feature*> m_mFeatureSet;
	};

}	// Swift

#endif	// _FEATUREMANAGER_H_