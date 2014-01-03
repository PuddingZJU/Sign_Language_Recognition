/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/02                                                  */
/************************************************************************/

#pragma once

#ifndef _FWEAKCLASSIFIERMANAGER_H_
#define _FWEAKCLASSIFIERMANAGER_H_

#include <vector>
#include "FClassifier.h"

namespace Swift
{
	class FWeakClassifierManager
	{
	public:
		FWeakClassifierManager();
		~FWeakClassifierManager();

		void genFWeakClassifierSet();

		/// All the weak classifiers based on the simple features.
		std::vector<FWeakClassifier*> m_vFWeakClassifier;
	};
}

#endif	// _FCLASSIFIERMANAGER_H_