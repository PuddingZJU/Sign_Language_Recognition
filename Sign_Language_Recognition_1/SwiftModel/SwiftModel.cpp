/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/01/29                                                  */
/************************************************************************/

#include <QStringList>
#include <fstream>
//#include "../FbxHandler/CommonHandler.h"
//#include "../FbxHandler/FbxViewer.h"
//#include "../MotionData/FBX2MC.h"
#include "../MotionData/BVHWriter.h"
#include "../MotionRecognition/FeatureParser.h"
#include "SwiftModel.h"

using namespace Swift;

SwiftModel::SwiftModel()
: m_nCurrentFrameNo(0), m_nMaxFrameNo(0), m_nCurrentTakeIndex(0), m_bJointMapping(false)
{
	/*m_strLastPath = QDir::currentPath();
	m_pCommonHandler = new CommonHandler();
	m_pFbxViewer = new FbxViewer();
	m_TimeMode = KTime::eDEFAULT_MODE;
	m_mTimeMode.insert(std::pair<std::string, KTime::ETimeMode>("120", KTime::eFRAMES120));
	m_mTimeMode.insert(std::pair<std::string, KTime::ETimeMode>("100", KTime::eFRAMES100));
	m_mTimeMode.insert(std::pair<std::string, KTime::ETimeMode>("60", KTime::eFRAMES60));
	m_mTimeMode.insert(std::pair<std::string, KTime::ETimeMode>("50", KTime::eFRAMES50));
	m_mTimeMode.insert(std::pair<std::string, KTime::ETimeMode>("48", KTime::eFRAMES48));
	m_mTimeMode.insert(std::pair<std::string, KTime::ETimeMode>("30", KTime::eFRAMES30));
	m_mTimeMode.insert(std::pair<std::string, KTime::ETimeMode>("25", KTime::ePAL));
	m_mTimeMode.insert(std::pair<std::string, KTime::ETimeMode>("24", KTime::eCINEMA));

	m_prm = new RuleManager();
	m_pfm = new FeatureManager();*/
}

SwiftModel::~SwiftModel()
{
	//m_vTakeName.clear();
	
	/*if (m_pCommonHandler)
	{
		if (m_pFbxSdkManager)
		{
			m_pCommonHandler->DestroySdkObjects(m_pFbxSdkManager);
		}
		delete m_pCommonHandler;
		m_pCommonHandler = NULL;
	}
	if (m_pFbxViewer)
	{
		delete m_pFbxViewer;
		m_pFbxViewer = NULL;
	}*/
	/*for (std::map<std::string, FStrongClassifier*>::iterator it=m_mFStrongClassifier.begin(); it!=m_mFStrongClassifier.end(); it++)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
	for (std::map<std::string, SignSamples*>::iterator it=m_mSignSamples.begin(); it!=m_mSignSamples.end(); it++)
	{
		if (it->second)
		{
			delete it->second;
		}
	}
	if (m_prm)
		delete m_prm;
	if (m_pfm)
		delete m_pfm;*/
}

int SwiftModel::getCurrentFrameNo()
{
	return m_nCurrentFrameNo;
}
/*
void SwiftModel::setCurrentFrameNo(int n)
{
	m_nCurrentFrameNo = n;
	if (n>m_nMaxFrameNo)
	{
		m_nMaxFrameNo = m_nMaxFrameNo;
	}
	KTime deltat;
	deltat.SetTime(0, 0, 0, n, 0, m_TimeMode);
	m_timeCurrentTime = m_timeStart + deltat;

	emit needUpdateGL();
}
*/
int SwiftModel::getMaxFrameNo()
{
	return m_nMaxFrameNo;
}

double SwiftModel::getFrameRate()
{
	return m_dFrameRate;
}
/*
void SwiftModel::setFrameRate(std::string strFrameRate)
{
	std::map<std::string, KTime::ETimeMode>::iterator it = m_mTimeMode.find(strFrameRate);
	if (it != m_mTimeMode.end())
	{
		m_TimeMode = it->second;

		KTimeSpan timespan;
		KFbxTakeInfo* lCurrentTakeInfo = m_pFbxScene->GetTakeInfo(m_pFbxScene->GetCurrentTakeName());
		if (lCurrentTakeInfo)
		{
			timespan = lCurrentTakeInfo->mLocalTimeSpan;
		}
		else
		{
			// Take the time line value
			m_pFbxScene->GetGlobalTimeSettings().GetTimelineDefautTimeSpan(timespan);
		}
		m_timeStart = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
		m_timeStop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
		m_timeCurrentTime = m_timeStart;

		m_dFrameRate = timespan.GetDuration().GetFrameRate(m_TimeMode);
		m_nMaxFrameNo = timespan.GetDuration().GetFrame(true, m_TimeMode);
	}
}

void SwiftModel::initializeFbxSDK()
{
	if (m_pCommonHandler)
	{
		m_pCommonHandler->InitializeSdkObjects(m_pFbxSdkManager, m_pFbxScene);
	}
}

void SwiftModel::loadFBXScene(std::string strFileName)
{
	m_vTakeName.clear();
	m_nCurrentTakeIndex = 0;
	if (m_pFbxSdkManager)
	{
		m_pCommonHandler->DestroySdkObjects(m_pFbxSdkManager);
	}
	initializeFbxSDK();

	bool bStatus = m_pCommonHandler->LoadScene(m_pFbxSdkManager, m_pFbxScene, strFileName.c_str());

	if (!bStatus)
	{
		return;
	}

	KTimeSpan timespan;
	KFbxTakeInfo* lCurrentTakeInfo = m_pFbxScene->GetTakeInfo(m_pFbxScene->GetCurrentTakeName());
	if (lCurrentTakeInfo)
	{
		timespan = lCurrentTakeInfo->mLocalTimeSpan;
	}
	else
	{
		// Take the time line value
		m_pFbxScene->GetGlobalTimeSettings().GetTimelineDefautTimeSpan(timespan);
	}
	m_timeStart = timespan.GetStart();
	m_timeStop = timespan.GetStop();
	m_timeCurrentTime = m_timeStart;

	m_dFrameRate = timespan.GetDuration().GetFrameRate(m_TimeMode);
	m_nMaxFrameNo = timespan.GetDuration().GetFrame(true, m_TimeMode);
	m_nCurrentFrameNo = 0;

	KArrayTemplate<KString*> lTakeNameArray;
	m_pFbxScene->FillTakeNameArray(lTakeNameArray);
	for (int i=0; i<lTakeNameArray.GetCount(); i++)
	{
		m_vTakeName.push_back(std::string(lTakeNameArray[i]->Buffer()));
		if (lTakeNameArray[i]->Compare(m_pFbxScene->GetCurrentTakeName()) == 0)
		{
			m_nCurrentTakeIndex = i;
		}
	}

	// Load the texture data in memory (for supported formats)
	m_pCommonHandler->LoadSupportedTextures(m_pFbxScene, m_pFbxViewer->gTextureArray, m_pFbxViewer->gTextureNameArray);

	emit signalLoadFBXScene();
}

void SwiftModel::saveFBXScene(std::string strFileName)
{
	m_pCommonHandler->SaveScene(m_pFbxSdkManager, m_pFbxScene, strFileName.c_str());
}

void SwiftModel::saveBVH(std::string strFileName)
{
	FBX2MC fbx2mc(m_pFbxScene);
	MotionClip* pmc = fbx2mc.convert();
	BVHWriter writer;
	writer.setFilePathName(strFileName);
	writer.setSource(pmc);
	writer.setSpacing("  ");
	writer.write();
	delete pmc;
}

void SwiftModel::closeFBXScene()
{
	m_vTakeName.clear();
	m_nCurrentTakeIndex = 0;
	m_nCurrentFrameNo = 0;
	m_nMaxFrameNo = 0;
	if (m_pFbxSdkManager)
	{
		m_pCommonHandler->DestroySdkObjects(m_pFbxSdkManager);
	}
	initializeFbxSDK();

	emit signalCloseFBXScene();
}

void SwiftModel::drawScene()
{
	if (m_pFbxScene)
	{
		m_pFbxViewer->DrawScene(m_pFbxScene, m_timeCurrentTime);
	}
}

void SwiftModel::setCurrentTake(std::string strTakeName)
{
	for (int i=0; i<m_vTakeName.size(); i++)
	{
		if (m_vTakeName.at(i) == strTakeName)
		{
			m_nCurrentTakeIndex = i;
			const char* pCharTakeName = strTakeName.c_str();
			char* tmp = new char[strlen(pCharTakeName)+1];
			strcpy(tmp, pCharTakeName);
			m_pFbxScene->SetCurrentTake(tmp);
			break;
		}
	}
	KTimeSpan timespan;
	KFbxTakeInfo* lCurrentTakeInfo = m_pFbxScene->GetTakeInfo(m_pFbxScene->GetCurrentTakeName());
	if (lCurrentTakeInfo)
	{
		timespan = lCurrentTakeInfo->mLocalTimeSpan;
	}
	else
	{
		// Take the time line value
		m_pFbxScene->GetGlobalTimeSettings().GetTimelineDefautTimeSpan(timespan);
	}
	m_timeStart = timespan.GetStart();
	m_timeStop = timespan.GetStop();
	//m_timeCurrentTime = m_timeStart;

	m_dFrameRate = timespan.GetDuration().GetFrameRate(m_TimeMode);
	m_nMaxFrameNo = timespan.GetDuration().GetFrame(true, m_TimeMode);
	
	// call setCurrentFrameNo() twice with different frame number 
	// to make fbx Node immediately update from fbx FCurve data.
	setCurrentFrameNo(1);
	setCurrentFrameNo(0);

	//emit needUpdateGL();
}*/

void SwiftModel::addNewTake(std::string strTakeName)
{
	/*m_vTakeName.push_back(strTakeName);
	int n = m_vTakeName.size();
	m_nCurrentTakeIndex = n-1;
	setCurrentTake(strTakeName);*/

	//emit signalLoadFBXScene();
}
/*
KTime::ETimeMode SwiftModel::getCurrentTimeMode()
{
	return m_TimeMode;
}

KTime SwiftModel::getStartTime()
{
	return m_timeStart;
}
*/
void SwiftModel::loadJointMapping(std::string strMappingFile)
{
	/*QFile file(QString::fromStdString(strMappingFile));
	if (!file.open(QFile::ReadOnly | QFile::Text)) 
		return;

	m_mJointMapping.clear();
	m_vJointMappingSpecial.clear();
	m_mJointException.clear();

	QString errorStr;
	int errorLine;
	int errorColumn;
	QDomDocument doc;
	if (!doc.setContent(&file, true, &errorStr, &errorLine,&errorColumn)) 
		return;
	
	QDomElement root = doc.documentElement();
	if (root.tagName() != "JointMapping")
		return;
	QDomNode node = root.firstChild();
	while (!node.isNull()) 
	{
		QString str = node.toElement().tagName();
		if (str == "normal")
			parseNormal(node.toElement());
		else if (str == "special")
			parseSpecial(node.toElement());
		else if (str == "exception")
			parseException(node.toElement());
		node = node.nextSibling();
	}*/
}

void SwiftModel::parseNormal(const QDomElement &element)
{
	/*QDomNode node = element.firstChild();
	while (!node.isNull()) 
	{
		if (node.toElement().tagName() == "item") 
		{
			QDomNamedNodeMap nodemap = node.attributes();
			if (nodemap.count() == 2 && nodemap.item(0).nodeName() == "Joint1" && nodemap.item(1).nodeName() == "Joint2" )
			{
				m_mJointMapping.insert(std::pair<std::string,std::string>(nodemap.item(0).nodeValue().toStdString(),nodemap.item(1).nodeValue().toStdString()));
			}
		}
		node = node.nextSibling();
	}*/
}

void SwiftModel::parseSpecial(const QDomElement &element)
{
	/*QDomNode node = element.firstChild();
	while (!node.isNull()) 
	{
		if (node.toElement().tagName() == "part") 
		{
			std::map<std::string, std::string> mJointMappingSpecial;
			QDomNode node2 = node.toElement().firstChild();
			{
				while (!node2.isNull())
				{
					QDomNamedNodeMap nodemap = node2.attributes();
					if (nodemap.count() == 2 && nodemap.item(0).nodeName() == "Joint1" && nodemap.item(1).nodeName() == "Joint2" )
					{
						mJointMappingSpecial.insert(std::pair<std::string,std::string>(nodemap.item(0).nodeValue().toStdString(),nodemap.item(1).nodeValue().toStdString()));
					}
					node2 = node2.nextSibling();
				}
			}
			m_vJointMappingSpecial.push_back(mJointMappingSpecial);
		}
		node = node.nextSibling();
	}*/
}

void SwiftModel::parseException(const QDomElement &element)
{
	/*QDomNode node = element.firstChild();
	while (!node.isNull()) 
	{
		if (node.toElement().tagName() == "item") 
		{
			QDomNamedNodeMap nodemap = node.attributes();
			if (nodemap.count() == 2 && nodemap.item(0).nodeName() == "Joint1" && nodemap.item(1).nodeName() == "Joint2" )
			{
				m_mJointException.insert(std::pair<std::string,std::string>(nodemap.item(0).nodeValue().toStdString(),nodemap.item(1).nodeValue().toStdString()));
			}
		}
		node = node.nextSibling();
	}*/
}

int SwiftModel::findPartIndex(std::string strJointName)
{
	int nIndex = -1;
	/*for (int i=0; i<m_vJointMappingSpecial.size(); i++)
	{
		std::map<std::string, std::string>::iterator it=m_vJointMappingSpecial.at(i).find(strJointName);
		if (it != m_vJointMappingSpecial.at(i).end())
		{
			nIndex = i;
			return nIndex;
		}
	}*/
	return nIndex;
}

void SwiftModel::delFStrongClassifiers()
{
	/*for (std::map<std::string, FStrongClassifier*>::iterator it=m_mFStrongClassifier.begin(); it!=m_mFStrongClassifier.end(); it++)
	{
		if (it->second)
			delete it->second;
	}
	m_mFStrongClassifier.clear();*/
}

void SwiftModel::loadRModel(std::string strFile)
{
	/*m_prm->m_pRootRuleNode->cleanSigns();

	std::ifstream in(strFile.c_str());
	std::string str;
	int nCount;
	double dThreshold;
	in >> str;
	if (str == "OneTwoHandsThreshold:")
	{
		in >> dThreshold;
		m_prm->m_dOneTwoHandsThres = dThreshold;
	}
	in >> str;
	if (str == "CloseFarHeadThreshold:")
	{
		in >> dThreshold;
		m_prm->m_dCloseFarHeadThres = dThreshold;
	}
	in >> str;
	if (str == "CloseFarHandsThreshold:")
	{
		in >> dThreshold;
		m_prm->m_dCloseFarHandsThres = dThreshold;
	}
	while (in >> str)
	{
		if (str == "NodeName:")
		{
			in >> str;
			for (int i=0; i<m_prm->m_vLeafNode.size(); i++)
			{
				if (str == m_prm->m_vLeafNode.at(i)->m_strName)
				{
					in >> str;
					if (str == "SignCount:")
					{
						in >> nCount;
						in >> str;
						if (str == "Signs:")
						{
							for (int j=0; j<nCount; j++)
							{
								in >> str;
								m_prm->m_vLeafNode.at(i)->m_vSigns.push_back(str);
							}
						}
					}
				}
			}
		}
	}

	in.close();*/
}

void SwiftModel::loadFModel(std::string strFile)
{
	/*std::ifstream in(strFile.c_str());

	delFStrongClassifiers();
	FStrongClassifier* pfsc;
	FWeakClassifier* pfwc;
	FeatureParser fp;
	std::string str;
	std::string strsc;
	double d;
	int nscc=0; // Strong classifier count.
	int nwcc=0; // Weak classifier count.
	int ntmp;
	in >> str;
	if (str == "StrongClassifierCount:")
		in >> nscc;
	for (int i=0; i<nscc; i++)
	{
		pfsc = new FStrongClassifier();
		in >> str;
		if (str == "StrongClassifier:")
			in >> strsc;
		in >> str;
		if (str == "WeakClassifierCount:")
			in >> nwcc;
		in >> str;
		if (str == "Weights:")
		{
			for (int j=0; j<nwcc; j++)
			{
				in >> d;
				pfsc->m_vWeight.push_back(d);
			}
		}
		in >> str;
		if (str == "WeakClassifiers:")
		{
			for (int j=0; j<nwcc; j++)
			{
				in >> ntmp;
				pfwc = new FWeakClassifier();
				pfwc->m_pFeature = fp.findFeature(ntmp);
				in >> ntmp;
				pfwc->m_nParity = ntmp;
				in >> d;
				pfwc->m_dThreshold = d;
				in >> d;
				pfwc->m_dError = d;
				pfsc->m_vFWeakClassifier.push_back(pfwc);
			}
		}
		m_mFStrongClassifier.insert(std::pair<std::string, FStrongClassifier*>(strsc, pfsc));
	}
	in.close();*/
}