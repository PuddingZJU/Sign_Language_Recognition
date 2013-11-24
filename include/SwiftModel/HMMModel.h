/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/03/25                                                  */
/************************************************************************/

#pragma once

#ifndef _HMMMODEL_H_
#define _HMMMODEL_H_

#include <QObject>
#include <vector>
#include <map>
#include <string>
#include "Util/Singleton.h"
#include "libhmm/HMM.h"


namespace Swift
{

	class RecogResult
	{
	public:
		RecogResult(){};
		RecogResult& operator=(const RecogResult& other)
		{
			strSign=other.strSign; 
			dScore=other.dScore;
			return *this;
		};

		std::string strSign;
		double dScore;
	};

	class HMMModel
		: public QObject, public Singleton <HMMModel>
	{
		Q_OBJECT

	public:
		HMMModel();
		~HMMModel();

		void cleanHMMs();

		void loadHMMs(QString strPath);

		void sortResut(std::vector<RecogResult>& vResult);

		/// The map between sign name and corresponding HMM.
		std::map<std::string, HMM*> m_mHMM;
	};	//HMMModel

}	//Swift

#endif	//_HMMMODEL_H_

