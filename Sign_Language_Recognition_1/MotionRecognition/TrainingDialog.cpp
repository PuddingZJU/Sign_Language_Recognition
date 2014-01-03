/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/22                                                  */
/************************************************************************/

#include "../MotionData/BVHReader.h"
#include "../SwiftModel/HMMModel.h"
#include "../MotionRecognition/InfoCalculator.h"
#include "SignSamples.h"
#include "FeatureParser.h"
#include "FTrainer.h"
#include "TrainingDialog.h"
#include <QFileDialog>
using namespace Swift;

int nDimention = 20;

TrainingDialog::TrainingDialog(QWidget *parent)
	: QWidget(parent), m_prn(NULL), m_nBestN(1)
{
	ui.setupUi(this);
}

TrainingDialog::~TrainingDialog()
{

}

void TrainingDialog::closeEvent(QCloseEvent * event)
{
	emit trainingDlgClosed();
}

void TrainingDialog::on_radioButton_RuleSpecified_clicked()
{
	ui.lineEdit_OneTwoHands->setEnabled(true);
	ui.lineEdit_CloseFarHead->setEnabled(true);
	ui.lineEdit_CloseFarHands->setEnabled(true);
}

void TrainingDialog::on_radioButton_RuleAutomatic_clicked()
{
	ui.lineEdit_OneTwoHands->setEnabled(false);
	ui.lineEdit_CloseFarHead->setEnabled(false);
	ui.lineEdit_CloseFarHands->setEnabled(false);
}

void TrainingDialog::on_pushButton_TrainRModel_clicked()
{
	SwiftModel::instance().m_prm->m_pRootRuleNode->cleanSigns();
	QString strPath = ui.lineEdit_BvhDataPath_1->text();
	if (!strPath.endsWith('/') && !strPath.endsWith('\\'))
	{
		strPath += "/"; 
	}
	QString strThres;
	strThres = ui.lineEdit_OneTwoHands->text();
	SwiftModel::instance().m_prm->m_dOneTwoHandsThres = strThres.toDouble();
	strThres = ui.lineEdit_CloseFarHead->text();
	SwiftModel::instance().m_prm->m_dCloseFarHeadThres = strThres.toDouble();
	strThres = ui.lineEdit_CloseFarHands->text();
	SwiftModel::instance().m_prm->m_dCloseFarHandsThres = strThres.toDouble();
	QStringList filters;
	filters << "*.bvh";
	QDir dir(strPath);
	QStringList list = dir.entryList(filters);
	QString strMotionName;
	std::string strtmp;
	MotionClip* pmc;
	RuleNode *prn=NULL;
	int nPos;
	std::string strSignName;
	for (int i=0; i<list.count(); i++)
	{
		strMotionName = list.at(i);
		strtmp = strPath.toLocal8Bit()+strMotionName.toLocal8Bit();
		BVHReader reader(strtmp);
		reader.read();
		pmc = reader.result();
		if (pmc)
		{
			prn = SwiftModel::instance().m_prm->findCandidateNode(pmc, SwiftModel::instance().m_prm->m_pRootRuleNode);
			if (prn)
			{
				nPos = strMotionName.lastIndexOf("_");
				strSignName = strMotionName.left(nPos).toStdString();
				//if (strtemp!=strSignName)
				//{
				//	prn->m_vSigns.push_back(strSignName);
				//	strtemp = strSignName;
				//}
				if (prn->m_vSigns.size()==0 || strSignName!=prn->m_vSigns.back())
					prn->m_vSigns.push_back(strSignName);
			}
		}
	}
}

void TrainingDialog::on_pushButton_SaveRModel_clicked()
{
	QString strFileName = QFileDialog::getSaveFileName(this, tr("Save Model File"), SwiftModel::instance().m_strLastPath+"/RuleModel.rmo", tr("Rule Model Files (*.rmo)"));
	if (strFileName.isEmpty())
		return;
	int n = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n+1);

	std::ofstream out(strFileName.toStdString().c_str());
	out << "OneTwoHandsThreshold: " << SwiftModel::instance().m_prm->m_dOneTwoHandsThres << std::endl;
	out << "CloseFarHeadThreshold: " << SwiftModel::instance().m_prm->m_dCloseFarHeadThres << std::endl;
	out << "CloseFarHandsThreshold: " << SwiftModel::instance().m_prm->m_dCloseFarHandsThres << std::endl;
	for (int i=0; i<SwiftModel::instance().m_prm->m_vLeafNode.size(); i++)
	{
		RuleNode* pRuleNode = SwiftModel::instance().m_prm->m_vLeafNode.at(i);
		out << "NodeName: " << pRuleNode->m_strName << std::endl;
		out << "SignCount: " << pRuleNode->m_vSigns.size() << std::endl;
		out << "Signs: ";
		for (int j=0; j<pRuleNode->m_vSigns.size(); j++)
		{
			out << pRuleNode->m_vSigns.at(j) << " ";
		}
		out << std::endl;
	}
	out.close();
}

void TrainingDialog::on_pushButton_LoadRModel_clicked()
{
	QString strFileName = QFileDialog::getOpenFileName(this,tr("Open Model File"),SwiftModel::instance().m_strLastPath,tr("Rule Model Files (*.rmo)"));
	if(strFileName.isEmpty())
		return;
	int n = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n+1);

	SwiftModel::instance().loadRModel(strFileName.toStdString());

	//SwiftModel::instance().m_prm->m_pRootRuleNode->cleanSigns();
	//std::ifstream in(strFileName.toStdString().c_str());
	//std::string str;
	//int nCount;
	//double dThreshold;
	//in >> str;
	//if (str == "OneTwoHandsThreshold:")
	//{
	//	in >> dThreshold;
	//	SwiftModel::instance().m_prm->m_dOneTwoHandsThres = dThreshold;
	//}
	//in >> str;
	//if (str == "CloseFarHeadThreshold:")
	//{
	//	in >> dThreshold;
	//	SwiftModel::instance().m_prm->m_dCloseFarHeadThres = dThreshold;
	//}
	//in >> str;
	//if (str == "CloseFarHandsThreshold:")
	//{
	//	in >> dThreshold;
	//	SwiftModel::instance().m_prm->m_dCloseFarHandsThres = dThreshold;
	//}
	//while (in >> str)
	//{
	//	if (str == "NodeName:")
	//	{
	//		in >> str;
	//		for (int i=0; i<SwiftModel::instance().m_prm->m_vLeafNode.size(); i++)
	//		{
	//			if (str == SwiftModel::instance().m_prm->m_vLeafNode.at(i)->m_strName)
	//			{
	//				in >> str;
	//				if (str == "SignCount:")
	//				{
	//					in >> nCount;
	//					in >> str;
	//					if (str == "Signs:")
	//					{
	//						for (int j=0; j<nCount; j++)
	//						{
	//							in >> str;
	//							SwiftModel::instance().m_prm->m_vLeafNode.at(i)->m_vSigns.push_back(str);
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//in.close();
}

void TrainingDialog::on_pushButton_OpenFile_1_clicked()
{
	QString strFileName = QFileDialog::getOpenFileName(this,tr("Open Motion File"),SwiftModel::instance().m_strLastPath,tr("Motion Files (*.bvh)"));
	if (strFileName.isEmpty())
		return;
	int n1 = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n1+1);

	MotionClip* pmc;
	ui.lineEdit_InputFile_1->setText(strFileName);
	BVHReader reader(strFileName.toStdString());
	reader.read();
	pmc = reader.result();
	if (pmc)
	{
		ui.textBrowser_Info->append(QString::fromStdString("********** First Layer **********"));
		ui.textBrowser_Info->append(QString::fromStdString("Candidate signs:"));
		m_prn = SwiftModel::instance().m_prm->findCandidateNode(pmc, SwiftModel::instance().m_prm->m_pRootRuleNode);
		if (m_prn)
		{
			//ui.textBrowser_Info->append(QString::fromStdString(m_prn->m_strName));
			for (int i=0; i<m_prn->m_vSigns.size(); i++)
			{
				ui.textBrowser_Info->append(QString::fromStdString("  " + m_prn->m_vSigns.at(i)));
			}
		}
		ui.textBrowser_Info->append("");
	}
}

void TrainingDialog::on_pushButton_GenFSamples_clicked()
{
	SwiftModel::instance().m_mSignSamples.clear();
	//double dValue;
	//FeatureParser fp;

	QString strPath = ui.lineEdit_BvhDataPath_2->text();
	if (!strPath.endsWith('/') && !strPath.endsWith('\\'))
	{
		strPath += "/"; 
	}
	QStringList filters;
	filters << "*.bvh";
	QDir dir(strPath);
	QStringList list = dir.entryList(filters);
	for (int i=0; i<list.count(); i++)
	{
		QString strMotionName = list.at(i);
		std::string strtmp = strPath.toLocal8Bit()+strMotionName.toLocal8Bit();
		BVHReader reader(strtmp);
		reader.read();
		MotionClip* pmc = reader.result();
		if (pmc)
		{
			strMotionName.remove(".bvh");
			int nPos = strMotionName.lastIndexOf("_");
			QString strSignName = strMotionName.left(nPos);
			std::map<std::string, SignSamples*>::iterator it=SwiftModel::instance().m_mSignSamples.find(strSignName.toStdString());
			if (it!=SwiftModel::instance().m_mSignSamples.end())
			{
				it->second->addSample(pmc);
				//it->second->m_vpmc.push_back(pmc);
				//PriFeatureClip* ppfc = new PriFeatureClip(pmc);
				//ppfc->calFromMC();
				//it->second->m_vppfc.push_back(ppfc);
				//for (std::map<int, std::vector<double>>::iterator it2=it->second->m_mFSamples.begin(); it2!=it->second->m_mFSamples.end(); it2++)
				//{
				//	if (fp.calFeatureValue(it2->first, ppfc, dValue))
				//	{
				//		it2->second.push_back(dValue);
				//	}
				//}
			}
			else
			{
				SignSamples* pss = new SignSamples();
				pss->addSample(pmc);
				//pss->m_vpmc.push_back(pmc);
				//PriFeatureClip* ppfc = new PriFeatureClip(pmc);
				//ppfc->calFromMC();
				//pss->m_vppfc.push_back(ppfc);
				//for (std::map<int, std::vector<double>>::iterator it2=pss->m_mFSamples.begin(); it2!=pss->m_mFSamples.end(); it2++)
				//{
				//	if (fp.calFeatureValue(it2->first, ppfc, dValue))
				//	{
				//		it2->second.push_back(dValue);
				//	}
				//}
				SwiftModel::instance().m_mSignSamples.insert(std::pair<std::string, SignSamples*>(strSignName.toStdString(), pss));
			}
		}
	}
}

void TrainingDialog::on_pushButton_SaveFSamples_clicked()
{
	QString strFileName = QFileDialog::getSaveFileName(this, tr("Save Sample File"), SwiftModel::instance().m_strLastPath+"/SignSamples.txt", tr("Sample Files (*.txt)"));
	if (strFileName.isEmpty())
		return;
	int n = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n+1);

	std::ofstream out(strFileName.toStdString().c_str());
	out << "Movement:" << std::endl;
	for (std::map<std::string, SignSamples*>::iterator it=SwiftModel::instance().m_mSignSamples.begin(); it!= SwiftModel::instance().m_mSignSamples.end(); it++)
	{
		out << it->first << std::endl;
		for (int i=0; i<it->second->m_vpmc.size(); i++)
		{
			out << it->second->m_vpmc.at(i)->getFileName() << " ";
		}
		out << std::endl;
		for (std::map<int, std::vector<double>>::iterator it2=it->second->m_mFSamples.begin(); it2!=it->second->m_mFSamples.end(); it2++)
		{
			out << it2->first << ": ";
			for (int i=0; i<it2->second.size(); i++)
			{
				out << it2->second.at(i) << " ";
			}
			out << std::endl;
		}
	}
	out.close();
}

void TrainingDialog::on_radioButton_FeatureSpecified_clicked()
{
	ui.lineEdit_FWCCount->setEnabled(true);
	ui.lineEdit_Coefficient->setEnabled(true);
}

void TrainingDialog::on_radioButton_FeatureAutomatic_clicked()
{
	ui.lineEdit_FWCCount->setEnabled(false);
	ui.lineEdit_Coefficient->setEnabled(false);
}

void TrainingDialog::on_pushButton_TrainFModels_clicked()
{
	QString str = ui.lineEdit_FWCCount->text();
	int nFWCCount = str.toInt();
	str = ui.lineEdit_Coefficient->text();
	double dCoef = str.toDouble();
	SwiftModel::instance().delFStrongClassifiers();
	FStrongClassifier* pfsc;
	FTrainer trainer;
	for (std::map<std::string, SignSamples*>::iterator it=SwiftModel::instance().m_mSignSamples.begin(); it!=SwiftModel::instance().m_mSignSamples.end(); it++)
	{
		std::string strPosSign = it->first;
		std::vector<std::string> vNegSign;
		for (std::map<std::string, SignSamples*>::iterator it2=SwiftModel::instance().m_mSignSamples.begin(); it2!=SwiftModel::instance().m_mSignSamples.end(); it2++)
		{
			if (it2 != it)
			{
				vNegSign.push_back(it2->first);
			}
		}
		pfsc = trainer.train(strPosSign, vNegSign, nFWCCount, dCoef);
		SwiftModel::instance().m_mFStrongClassifier.insert(std::pair<std::string, FStrongClassifier*>(strPosSign, pfsc));
	}
}

void TrainingDialog::on_pushButton_SaveFModels_clicked()
{
	QString strFileName = QFileDialog::getSaveFileName(this, tr("Save FSC File"), SwiftModel::instance().m_strLastPath+"/FeatureModel.fsc", tr("FSC Files (*.fsc)"));
	if (strFileName.isEmpty())
		return;
	int n = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n+1);

	std::ofstream out(strFileName.toStdString().c_str());
	out << "StrongClassifierCount: " << SwiftModel::instance().m_mFStrongClassifier.size() << std::endl;
	for (std::map<std::string, FStrongClassifier*>::iterator it=SwiftModel::instance().m_mFStrongClassifier.begin(); it!= SwiftModel::instance().m_mFStrongClassifier.end(); it++)
	{
		out << "StrongClassifier: " << it->first << std::endl;
		out << "WeakClassifierCount: " << it->second->m_vFWeakClassifier.size() << std::endl;
		out << "Weights: " << std::endl;
		for (int i=0; i<it->second->m_vWeight.size(); i++)
		{
			out << it->second->m_vWeight.at(i) << " ";
		}
		out << std::endl;
		out << "WeakClassifiers: " << std::endl;
		for (int i=0; i<it->second->m_vFWeakClassifier.size(); i++)
		{
			out << it->second->m_vFWeakClassifier.at(i)->m_pFeature->m_nID << " " << it->second->m_vFWeakClassifier.at(i)->m_nParity << " " << it->second->m_vFWeakClassifier.at(i)->m_dThreshold << " " << it->second->m_vFWeakClassifier.at(i)->m_dError << std::endl;
		}
	}
	out.close();
}

void TrainingDialog::on_pushButton_LoadFModels_clicked()
{
	QString strFileName = QFileDialog::getOpenFileName(this,tr("Open FSC File"),SwiftModel::instance().m_strLastPath,tr("FSC Files (*.fsc)"));
	if(strFileName.isEmpty())
		return;
	int n = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n+1);

	SwiftModel::instance().loadFModel(strFileName.toStdString());

	//std::ifstream in(strFileName.toStdString().c_str());

	//SwiftModel::instance().delFStrongClassifiers();
	//FStrongClassifier* pfsc;
	//FWeakClassifier* pfwc;
	//FeatureParser fp;
	//std::string str;
	//std::string strsc;
	//double d;
	//int nscc=0; // Strong classifier count.
	//int nwcc=0; // Weak classifier count.
	//int ntmp;
	//in >> str;
	//if (str == "StrongClassifierCount:")
	//	in >> nscc;
	//for (int i=0; i<nscc; i++)
	//{
	//	pfsc = new FStrongClassifier();
	//	in >> str;
	//	if (str == "StrongClassifier:")
	//		in >> strsc;
	//	in >> str;
	//	if (str == "WeakClassifierCount:")
	//		in >> nwcc;
	//	in >> str;
	//	if (str == "Weights:")
	//	{
	//		for (int j=0; j<nwcc; j++)
	//		{
	//			in >> d;
	//			pfsc->m_vWeight.push_back(d);
	//		}
	//	}
	//	in >> str;
	//	if (str == "WeakClassifiers:")
	//	{
	//		for (int j=0; j<nwcc; j++)
	//		{
	//			in >> ntmp;
	//			pfwc = new FWeakClassifier();
	//			pfwc->m_pFeature = fp.findFeature(ntmp);
	//			in >> ntmp;
	//			pfwc->m_nParity = ntmp;
	//			in >> d;
	//			pfwc->m_dThreshold = d;
	//			in >> d;
	//			pfwc->m_dError = d;
	//			pfsc->m_vFWeakClassifier.push_back(pfwc);
	//		}
	//	}
	//	SwiftModel::instance().m_mFStrongClassifier.insert(std::pair<std::string, FStrongClassifier*>(strsc, pfsc));
	//}
	//in.close();
}

void TrainingDialog::on_pushButton_OpenFile_2_clicked()
{
	QString strFileName = QFileDialog::getOpenFileName(this,tr("Open Motion File"),SwiftModel::instance().m_strLastPath,tr("Motion Files (*.bvh)"));
	if (strFileName.isEmpty())
		return;
	int n1 = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n1+1);

	MotionClip* pmc;
	ui.lineEdit_InputFile_2->setText(strFileName);
	BVHReader reader(strFileName.toStdString());
	reader.read();
	pmc = reader.result();
	if (pmc)
	{
		ui.textBrowser_Info->append(QString::fromStdString("********** Second Layer **********"));
		ui.textBrowser_Info->append(QString::fromStdString("Candidate signs:"));
		m_vFResult.clear();
		int result=0;
		if (ui.radioButton_UseLayer1->isChecked())
		{
			if (m_prn)
			{
				std::map<std::string, FStrongClassifier*>::iterator it;
				for (int i=0; i<m_prn->m_vSigns.size(); i++)
				{
					it = SwiftModel::instance().m_mFStrongClassifier.find(m_prn->m_vSigns.at(i));
					result = it->second->recognize(pmc);
					if (result == 1)
					{
						m_vFResult.push_back(it->first);
						ui.textBrowser_Info->append("  "+QString::fromStdString(it->first));
					}
				}
			}
		}
		else if (ui.radioButton_Directly->isChecked())
		{
			for (std::map<std::string, FStrongClassifier*>::iterator it=SwiftModel::instance().m_mFStrongClassifier.begin(); it!=SwiftModel::instance().m_mFStrongClassifier.end(); it++)
			{
				result = it->second->recognize(pmc);
				if (result == 1)
				{
					m_vFResult.push_back(it->first);
					ui.textBrowser_Info->append("  "+QString::fromStdString(it->first));
				}
			}
		}
		ui.textBrowser_Info->append("");
	}
}

void TrainingDialog::on_radioButton_HMMSpecified_clicked()
{
	ui.lineEdit_StateNum->setEnabled(true);
	ui.lineEdit_MixNum->setEnabled(true);
}

void TrainingDialog::on_radioButton_HMMAutomatic_clicked()
{
	ui.lineEdit_StateNum->setEnabled(false);
	ui.lineEdit_MixNum->setEnabled(false);
}

void TrainingDialog::on_pushButton_LoadSampleInfo_clicked()
{
	m_mSign2File.clear();

	QString strPath = ui.lineEdit_BvhDataPath_3->text();
	if (!strPath.endsWith('/') && !strPath.endsWith('\\'))
	{
		strPath += "/";
	}
	QStringList filters;
	filters << "*.bvh";
	QDir dir(strPath);
	QStringList list = dir.entryList(filters);

	QString strFileName;
	int nPos;
	std::string strSignName, strPathFile;
	std::map<std::string, std::vector<std::string>>::iterator it;
	for (int l=0; l<list.count(); l++)
	{
		strFileName = list.at(l);
		nPos = strFileName.lastIndexOf("_");
		strSignName = strFileName.left(nPos).toStdString();
		strPathFile = strPath.toLocal8Bit()+strFileName.toLocal8Bit();
		it = m_mSign2File.find(strSignName);
		if (it == m_mSign2File.end())
		{
			std::vector<std::string> v;
			v.push_back(strPathFile);
			m_mSign2File.insert(std::pair<std::string, std::vector<std::string>>(strSignName, v));
		}
		else
		{
			it->second.push_back(strPathFile);
		}
	}

	ui.pushButton_GenHMMs->setEnabled(true);
}

void TrainingDialog::on_pushButton_GenHMMs_clicked()
{
	HMMModel::instance().cleanHMMs();

	QString strStateNum = ui.lineEdit_StateNum->text();
	int nStateNum = strStateNum.toInt();

	QString strMixNum = ui.lineEdit_MixNum->text();
	int nMixNum = strMixNum.toInt();

	ColumnVector P(nStateNum);
	Matrix T(nStateNum,nStateNum);

	/*left-to-right*/
	for(int i=0;i<nStateNum;i++){
		if(i) P.element(i) = 0.0;
		else P.element(i) = 1.0;
	}
	for(int i=0;i<nStateNum;i++){
		for(int j=0;j<nStateNum;j++){
			T.element(i,j) = 0.0;
			if(i==j-1) T.element(i,j) = 1.0;
			else if(i==j) T.element(i,j) = 1.0;
		}
	}

	for (std::map<std::string, std::vector<std::string>>::iterator it=m_mSign2File.begin(); it!=m_mSign2File.end(); it++)
	{
		HMM* phmm = new HMM(nStateNum,nDimention,nMixNum,P,T);
		phmm->set_HMM();
		HMMModel::instance().m_mHMM.insert(std::pair<std::string, HMM*>(it->first, phmm));
	}

	ui.pushButton_TrainHMMs->setEnabled(true);
	ui.pushButton_SaveHMMs->setEnabled(true);
}

void TrainingDialog::on_pushButton_TrainHMMs_clicked()
{
	InfoCalcualtor calculator;

	int nSize;
	HMM* phmm;
	for (std::map<std::string, std::vector<std::string>>::iterator it=m_mSign2File.begin(); it!=m_mSign2File.end(); it++)
	{
		std::map<std::string, HMM*>::iterator it2 = HMMModel::instance().m_mHMM.find(it->first);
		if (it2 == HMMModel::instance().m_mHMM.end())
			continue;
		phmm = it2->second;
		if (!phmm)
			continue;

		nSize = it->second.size();
		Matrix* train = new Matrix[nSize];
		MotionClip* pmc;
		for (int i=0; i<nSize; i++)
		{
			BVHReader reader(it->second.at(i));
			reader.read();
			pmc = reader.result();
			if (pmc)
			{
				long nSamples = pmc->getFrameCount();
				train[i].ReSize(nDimention,nSamples);

				std::vector<double> vFlex;
				std::vector<double> vDist;
				std::vector<double> vOriX, vOriY, vOriZ;
				MotionJoint *pmj0, *pmj1, *pmj2, *pmj3, *pmj4;
				// Right thumb finger
				pmj0 = pmc->findJoint("r_thumb0");	
				pmj1 = pmc->findJoint("r_thumb1");	
				pmj2 = pmc->findJoint("r_thumb2");	
				pmj3 = pmc->findJoint("r_thumb3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 190);
				int k=0;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Right index finger
				pmj0 = pmc->findJoint("r_index0");	
				pmj1 = pmc->findJoint("r_index1");	
				pmj2 = pmc->findJoint("r_index2");	
				pmj3 = pmc->findJoint("r_index3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Right middle finger
				pmj0 = pmc->findJoint("r_middle0");	
				pmj1 = pmc->findJoint("r_middle1");	
				pmj2 = pmc->findJoint("r_middle2");	
				pmj3 = pmc->findJoint("r_middle3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Right ring finger
				pmj0 = pmc->findJoint("r_ring0");	
				pmj1 = pmc->findJoint("r_ring1");	
				pmj2 = pmc->findJoint("r_ring2");	
				pmj3 = pmc->findJoint("r_ring3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Right little finger
				pmj0 = pmc->findJoint("r_pinky0");	
				pmj1 = pmc->findJoint("r_pinky1");	
				pmj2 = pmc->findJoint("r_pinky2");	
				pmj3 = pmc->findJoint("r_pinky3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left thumb finger
				pmj0 = pmc->findJoint("l_thumb0");	
				pmj1 = pmc->findJoint("l_thumb1");	
				pmj2 = pmc->findJoint("l_thumb2");	
				pmj3 = pmc->findJoint("l_thumb3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 190);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left index finger
				pmj0 = pmc->findJoint("l_index0");	
				pmj1 = pmc->findJoint("l_index1");	
				pmj2 = pmc->findJoint("l_index2");	
				pmj3 = pmc->findJoint("l_index3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left middle finger
				pmj0 = pmc->findJoint("l_middle0");	
				pmj1 = pmc->findJoint("l_middle1");	
				pmj2 = pmc->findJoint("l_middle2");	
				pmj3 = pmc->findJoint("l_middle3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left ring finger
				pmj0 = pmc->findJoint("l_ring0");	
				pmj1 = pmc->findJoint("l_ring1");	
				pmj2 = pmc->findJoint("l_ring2");	
				pmj3 = pmc->findJoint("l_ring3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left little finger
				pmj0 = pmc->findJoint("l_pinky0");	
				pmj1 = pmc->findJoint("l_pinky1");	
				pmj2 = pmc->findJoint("l_pinky2");	
				pmj3 = pmc->findJoint("l_pinky3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				pmj1 = pmc->findJoint("r_wrist");
				pmj2 = pmc->findJoint("root");
				calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vDist.at(j); 
				}
				vDist.clear();

				pmj2 = pmc->findJoint("skullbase");
				calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vDist.at(j); 
				}
				vDist.clear();

				pmj1 = pmc->findJoint("l_wrist");
				pmj2 = pmc->findJoint("root");
				calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vDist.at(j); 
				}
				vDist.clear();

				pmj2 = pmc->findJoint("skullbase");
				calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k,j)=vDist.at(j); 
				}
				vDist.clear();

				pmj1 = pmc->findJoint("r_wrist");
				pmj2 = pmc->findJoint("r_index1");
				pmj3 = pmc->findJoint("r_ring1");
				calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, 0, nSamples-1, vOriX, vOriY, vOriZ);
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k+1,j)=vOriX.at(j);
					train[i].element(k+2,j)=vOriY.at(j);
					train[i].element(k+3,j)=vOriZ.at(j);
				}
				k+=3;
				vOriX.clear();
				vOriY.clear();
				vOriZ.clear();

				pmj1 = pmc->findJoint("l_wrist");
				pmj2 = pmc->findJoint("l_index1");
				pmj3 = pmc->findJoint("l_ring1");
				calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, 0, nSamples-1, vOriX, vOriY, vOriZ);
				for (int j=0; j<nSamples; j++)
				{
					train[i].element(k+1,j)=vOriX.at(j);
					train[i].element(k+2,j)=vOriY.at(j);
					train[i].element(k+3,j)=vOriZ.at(j);
				}
				k+=3;
				vOriX.clear();
				vOriY.clear();
				vOriZ.clear();
			}
		}

		phmm->init_segmental(train,nSize);
		phmm->estimations(train,nSize,1);
	}
}

void TrainingDialog::on_pushButton_SaveHMMs_clicked()
{
	QString strPath = ui.lineEdit_OutputPath->text();
	if (!strPath.endsWith('/') && !strPath.endsWith('\\'))
	{
		strPath += "/";
	}
	std::string str, strtmp;
	str = strPath.toStdString();
	for (std::map<std::string, HMM*>::iterator it=HMMModel::instance().m_mHMM.begin(); it!=HMMModel::instance().m_mHMM.end(); it++)
	{
		strtmp = str+it->first+".hmm";
		const char* pCharCom = strtmp.c_str();
		char* tmp = new char[strlen(pCharCom)+1];
		strcpy(tmp, pCharCom);
		it->second->Save(tmp);
	}
}

void TrainingDialog::on_pushButton_LoadHMMs_clicked()
{
	QString strPath = ui.lineEdit_HMMsPath->text();
	HMMModel::instance().loadHMMs(strPath);

	//if (!strPath.endsWith('/') && !strPath.endsWith('\\'))
	//{
	//	strPath += "/";
	//}
	//QStringList filters;
	//filters << "*.hmm";
	//QDir dir(strPath);
	//QStringList list = dir.entryList(filters);

	//HMMModel::instance().cleanHMMs();
	//QString strFileName;
	//int nPos;
	//std::string strSignName, strPathFile;
	//HMM* phmm;
	//const char* pCharCom;
	//char* tmp;
	//for (int l=0; l<list.count(); l++)
	//{
	//	strFileName = list.at(l);
	//	nPos = strFileName.lastIndexOf(".hmm");
	//	strSignName = strFileName.left(nPos).toStdString();
	//	strPathFile = strPath.toLocal8Bit()+strFileName.toLocal8Bit();
	//	pCharCom = strPathFile.c_str();
	//	tmp = new char[strlen(pCharCom)+1];
	//	strcpy(tmp, pCharCom);

	//	phmm = new HMM();
	//	phmm->Load(tmp);
	//	HMMModel::instance().m_mHMM.insert(std::pair<std::string, HMM*>(strSignName, phmm));
	//}
}

void TrainingDialog::on_pushButton_OpenFile_3_clicked()
{
	QString strFileName = QFileDialog::getOpenFileName(this,tr("Open Motion File"),SwiftModel::instance().m_strLastPath,tr("Motion Files (*.bvh)"));
	if (strFileName.isEmpty())
		return;
	int n1 = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n1+1);

	MotionClip* pmc;
	ui.lineEdit_InputFile_3->setText(strFileName);
	BVHReader reader(strFileName.toStdString());
	reader.read();
	pmc = reader.result();
	if (pmc)
	{
		MotionJoint *pmjlw, *pmjle, *pmjrw, *pmjre, *pmjlw1, *pmjle1, *pmjrw1, *pmjre1;
		pmjlw = pmc->findJoint("l_wrist");
		pmjle = pmc->findJoint("l_elbow");
		pmjrw = pmc->findJoint("r_wrist");
		pmjre = pmc->findJoint("r_elbow");

		Matrix test;

		long nSamples = pmc->getFrameCount();
		test.ReSize(nDimention,nSamples);

		std::vector<double> vFlex;
		std::vector<double> vDist;
		std::vector<double> vOriX, vOriY, vOriZ;
		MotionJoint *pmj0, *pmj1, *pmj2, *pmj3, *pmj4;
		InfoCalcualtor calculator;
		// Right thumb finger
		pmj0 = pmc->findJoint("r_thumb0");	
		pmj1 = pmc->findJoint("r_thumb1");	
		pmj2 = pmc->findJoint("r_thumb2");	
		pmj3 = pmc->findJoint("r_thumb3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 190);
		int k=0;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		// Right index finger
		pmj0 = pmc->findJoint("r_index0");	
		pmj1 = pmc->findJoint("r_index1");	
		pmj2 = pmc->findJoint("r_index2");	
		pmj3 = pmc->findJoint("r_index3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		// Right middle finger
		pmj0 = pmc->findJoint("r_middle0");	
		pmj1 = pmc->findJoint("r_middle1");	
		pmj2 = pmc->findJoint("r_middle2");	
		pmj3 = pmc->findJoint("r_middle3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		// Right ring finger
		pmj0 = pmc->findJoint("r_ring0");	
		pmj1 = pmc->findJoint("r_ring1");	
		pmj2 = pmc->findJoint("r_ring2");	
		pmj3 = pmc->findJoint("r_ring3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		// Right little finger
		pmj0 = pmc->findJoint("r_pinky0");	
		pmj1 = pmc->findJoint("r_pinky1");	
		pmj2 = pmc->findJoint("r_pinky2");	
		pmj3 = pmc->findJoint("r_pinky3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		// Left thumb finger
		pmj0 = pmc->findJoint("l_thumb0");	
		pmj1 = pmc->findJoint("l_thumb1");	
		pmj2 = pmc->findJoint("l_thumb2");	
		pmj3 = pmc->findJoint("l_thumb3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 190);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		// Left index finger
		pmj0 = pmc->findJoint("l_index0");	
		pmj1 = pmc->findJoint("l_index1");	
		pmj2 = pmc->findJoint("l_index2");	
		pmj3 = pmc->findJoint("l_index3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		// Left middle finger
		pmj0 = pmc->findJoint("l_middle0");	
		pmj1 = pmc->findJoint("l_middle1");	
		pmj2 = pmc->findJoint("l_middle2");	
		pmj3 = pmc->findJoint("l_middle3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		// Left ring finger
		pmj0 = pmc->findJoint("l_ring0");	
		pmj1 = pmc->findJoint("l_ring1");	
		pmj2 = pmc->findJoint("l_ring2");	
		pmj3 = pmc->findJoint("l_ring3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		// Left little finger
		pmj0 = pmc->findJoint("l_pinky0");	
		pmj1 = pmc->findJoint("l_pinky1");	
		pmj2 = pmc->findJoint("l_pinky2");	
		pmj3 = pmc->findJoint("l_pinky3");	
		pmj4 = pmc->getChild(pmj3, 0);
		calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vFlex.at(j); 
		}
		vFlex.clear();

		pmj1 = pmc->findJoint("r_wrist");
		pmj2 = pmc->findJoint("root");
		calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vDist.at(j); 
		}
		vDist.clear();

		pmj2 = pmc->findJoint("skullbase");
		calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vDist.at(j); 
		}
		vDist.clear();

		pmj1 = pmc->findJoint("l_wrist");
		pmj2 = pmc->findJoint("root");
		calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vDist.at(j); 
		}
		vDist.clear();

		pmj2 = pmc->findJoint("skullbase");
		calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
		k++;
		for (int j=0; j<nSamples; j++)
		{
			test.element(k,j)=vDist.at(j); 
		}
		vDist.clear();

		pmj1 = pmc->findJoint("r_wrist");
		pmj2 = pmc->findJoint("r_index1");
		pmj3 = pmc->findJoint("r_ring1");
		calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, 0, nSamples-1, vOriX, vOriY, vOriZ);
		for (int j=0; j<nSamples; j++)
		{
			test.element(k+1,j)=vOriX.at(j);
			test.element(k+2,j)=vOriY.at(j);
			test.element(k+3,j)=vOriZ.at(j);
		}
		k+=3;
		vOriX.clear();
		vOriY.clear();
		vOriZ.clear();

		pmj1 = pmc->findJoint("l_wrist");
		pmj2 = pmc->findJoint("l_index1");
		pmj3 = pmc->findJoint("l_ring1");
		calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, 0, nSamples-1, vOriX, vOriY, vOriZ);
		for (int j=0; j<nSamples; j++)
		{
			test.element(k+1,j)=vOriX.at(j);
			test.element(k+2,j)=vOriY.at(j);
			test.element(k+3,j)=vOriZ.at(j);
		}
		k+=3;
		vOriX.clear();
		vOriY.clear();
		vOriZ.clear();

		double dmax = -1.0e100, d;
		std::string str;
		RecogResult rr;
		std::vector<RecogResult> vResult;
		QString strBestN = ui.lineEdit_BestN->text();
		m_nBestN = strBestN.toInt();
		if (ui.radioButton_UseLayer2->isChecked())
		{
			std::map<std::string, HMM*>::iterator it;
			for (int i=0; i<m_vFResult.size(); i++)
			{
				it = HMMModel::instance().m_mHMM.find(m_vFResult.at(i));
				if (it != HMMModel::instance().m_mHMM.end())
				{
					d = it->second->viterbi_p(test);
					//if (d > dmax)
					//{
					//	dmax = d;
					//	str = it->first;
					//}
					rr.strSign = it->first;
					rr.dScore = d;				
					if(vResult.size() == m_nBestN)
					{
						HMMModel::instance().sortResut(vResult);
						if (vResult.back().dScore < d)
						{
							vResult.erase(vResult.end()-1);
							vResult.push_back(rr);	
						}
					}
					else
					{
						vResult.push_back(rr);
					}
				}
			}
		}
		else if (ui.radioButton_HMMDirectly->isChecked())
		{
			for (std::map<std::string, HMM*>::iterator it=HMMModel::instance().m_mHMM.begin(); it!=HMMModel::instance().m_mHMM.end(); it++)
			{
				d = it->second->viterbi_p(test);
				//if (d > dmax)
				//{
				//	dmax = d;
				//	str = it->first;
				//}
				rr.strSign = it->first;
				rr.dScore = d;				
				if(vResult.size() == m_nBestN)
				{
					HMMModel::instance().sortResut(vResult);
					if (vResult.back().dScore < d)
					{
						vResult.erase(vResult.end()-1);
						vResult.push_back(rr);	
					}
				}
				else
				{
					vResult.push_back(rr);
				}
			}
		}

		HMMModel::instance().sortResut(vResult);
		ui.textBrowser_Info->append(QString::fromStdString("********** Third Layer **********"));
		ui.textBrowser_Info->append("Best "+QString::number(vResult.size())+" candidates and their scores:");
		for (int i=0; i<vResult.size(); i++)
		{
			ui.textBrowser_Info->append(QString::fromStdString(vResult.at(i).strSign)+": "+QString::number(vResult.at(i).dScore));
		}
		ui.textBrowser_Info->append("");
	}
}
