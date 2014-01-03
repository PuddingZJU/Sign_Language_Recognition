/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/03/25                                                  */
/************************************************************************/

#include <QStringList>
#include <QDir>
#include "HMMModel.h"
using namespace Swift;

HMMModel::HMMModel()
{

}

HMMModel::~HMMModel()
{
	cleanHMMs();
}

void HMMModel::cleanHMMs()
{
	for (std::map<std::string, HMM*>::iterator it=m_mHMM.begin(); it!=m_mHMM.end(); it++)
	{
		if (it->second)
			delete it->second;
	}
	m_mHMM.clear();
}

void HMMModel::loadHMMs(QString strPath)
{
	if (!strPath.endsWith('/') && !strPath.endsWith('\\'))
	{
		strPath += "/";
	}
	QStringList filters;
	filters << "*.hmm";
	QDir dir(strPath);
	QStringList list = dir.entryList(filters);

	cleanHMMs();
	QString strFileName;
	int nPos;
	std::string strSignName, strPathFile;
	HMM* phmm;
	const char* pCharCom;
	char* tmp;
	for (int l=0; l<list.count(); l++)
	{
		strFileName = list.at(l);
		nPos = strFileName.lastIndexOf(".hmm");
		strSignName = strFileName.left(nPos).toStdString();
		strPathFile = strPath.toLocal8Bit()+strFileName.toLocal8Bit();
		pCharCom = strPathFile.c_str();
		tmp = new char[strlen(pCharCom)+1];
		strcpy(tmp, pCharCom);

		phmm = new HMM();
		phmm->Load(tmp);
		m_mHMM.insert(std::pair<std::string, HMM*>(strSignName, phmm));
	}
}

void HMMModel::sortResut(std::vector<RecogResult>& vResult)
{
	for (int i=0; i<vResult.size(); i++)
	{
		for (int j=i+1; j<vResult.size(); j++)
		{
			if (vResult.at(j).dScore > vResult.at(i).dScore)
			{
				RecogResult rr = vResult.at(i);
				vResult.at(i) = vResult.at(j);
				vResult.at(j) = rr;
			}		
		}
	}
}