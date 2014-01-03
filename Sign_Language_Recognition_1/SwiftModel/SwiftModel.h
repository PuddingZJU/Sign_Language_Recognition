/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/01/29                                                  */
/************************************************************************/

#pragma once

#ifndef _SWIFTMODEL_H_
#define _SWIFTMODEL_H_

#include <QObject>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtXML/QDomDocument>
#include <QtGui/QtGui>
#include <QtGui>
#include <QString>
#include <vector>
#include <map>
#include <string>
#include <assert.h>
//#include <fbxsdk.h>
//#include <fGlove.h>
#include "../Util/Singleton.h"
#include "../MotionRecognition/RuleManager.h"
#include "../MotionRecognition/SignSamples.h"
#include "../MotionRecognition/FeatureManager.h"


namespace Swift
{
	class CommonHandler;
	class FbxViewer;

	class SwiftModel
		: public QObject, public Singleton <SwiftModel>
	{
		Q_OBJECT

	public:
		SwiftModel();
		~SwiftModel();

		int getCurrentFrameNo();
		void setCurrentFrameNo(int n);
		int getMaxFrameNo();
		double getFrameRate();
		void setFrameRate(std::string strFrameRate);
		//void initializeFbxSDK();
		//void loadFBXScene(std::string strFileName);
		//void saveFBXScene(std::string strFileName);
		//void saveBVH(std::string strFileName);
		//void closeFBXScene();
		//void drawScene();
		void setCurrentTake(std::string strTakeName);
		void addNewTake(std::string strTakeName);
		//KTime::ETimeMode getCurrentTimeMode();
		//KTime getStartTime();
		void loadJointMapping(std::string strMappingFile);
		int findPartIndex(std::string strJointName);
		void delFStrongClassifiers();
		void loadRModel(std::string strFile);
		void loadFModel(std::string strFile);

	private:
		void parseNormal(const QDomElement &element);
		void parseSpecial(const QDomElement &element);
		void parseException(const QDomElement &element);

	signals:
		//void signalLoadFBXScene();
		//void signalCloseFBXScene();
		void needUpdateGL();

	private:
		/// Current frameno in the scene.
		int m_nCurrentFrameNo;
		/// Max frameno in scene.
		int m_nMaxFrameNo;
		//KTime m_timeStart;
		//KTime m_timeStop;
		//KTime m_timeCurrentTime;
		//KTime::ETimeMode m_TimeMode;
		//std::map<std::string, KTime::ETimeMode> m_mTimeMode;
		double m_dFrameRate;

	public:
		/// The last path the user visited.
		QString m_strLastPath;
		/// Pointer to the FBX SDK memory manager object.
		//KFbxSdkManager* m_pFbxSdkManager;
		/// Pointer to the FBX scene object.
		//KFbxScene* m_pFbxScene;

		/// Pointer to the common handler which is used to create and destroy FBX SDK manager and scene.
		//CommonHandler* m_pCommonHandler;
		/// Pointer to the FBX Viewer.
		//FbxViewer* m_pFbxViewer;

		std::vector<std::string> m_vTakeName;
		int m_nCurrentTakeIndex;
		bool m_bJointMapping;
		/// The mapping between the joints in the two skeletons.
		std::map<std::string, std::string> m_mJointMapping;
		/// The joint mapping of special part which is used to align the two reference skeletons.
		std::vector<std::map<std::string, std::string> > m_vJointMappingSpecial;
		/// The mapping of the except joints.
		std::map<std::string, std::string> m_mJointException;
		/// The 5DT data glove for left hand.
		//fdGlove *m_pLeftGlove;
		/// The 5DT data glove for right hand.
		//fdGlove *m_pRightGlove;

		///// All the training data. It's a map between class name and the training data in this class.
		//std::map<std::string, std::vector<PriFeatureClip*>> m_mTrainingData;
		///// The map between class name and its weak classifiers based on simple features.
		//std::map<std::string, std::vector<FWeakClassifier*>> m_mFWeakClassifier;

		/// The rule manager used to manage the classifying rules in the first layer recognizer.
		RuleManager* m_prm;

		/// The map between sign word name and corresponding data.
		std::map<std::string, SignSamples*> m_mSignSamples;
		/// The feature manager used to manage all the candidate features in the second layer recognizer.
		FeatureManager* m_pfm;
		/// The map between class name and its strong classifiers based on simple features.
		std::map<std::string, FStrongClassifier*> m_mFStrongClassifier;

	};	//SwiftModel

}	//Swift

#endif	//_SWIFTMODEL_H_

