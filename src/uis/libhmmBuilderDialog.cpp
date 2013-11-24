/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2011/03/04                                                  */
/************************************************************************/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "SwiftModel/SwiftModel.h"
//#include "MotionData/BVHReader.h"
//#include "MotionData/MotionEditor.h"
//#include "MotionData/BVHWriter.h"
//#include "MotionRecognition/InfoCalculator.h"
//#include "MotionAnnotation/AnnotationDialog.h"
#include "libhmmBuilderDialog.h"
using namespace Swift;
using namespace std;

#define Number 3
#define Dimention 20
#define MIX 1

#define TRAINING 14
#define SAMPLE 6
#define CLASS 200//1542

#define Only_Check 0

double check2(){
	ifstream fin;
	char str[80];
	int aaa = 0;
	int bbb = 0;
	int result = 0;
	double tmp;
	double val[50*SAMPLE];
	double max_val[50*SAMPLE];
	int max_val_n[50*SAMPLE];

	for(int i=0;i<50;i++){
		sprintf(str, "./NResult%d_%d/Result_%d.txt", Number, MIX, i*4+1);
		fin.open(str);
		if(!fin){
			cout << "Can't open input file: " << str << "\n";
			return 1;
		}


		fin >> aaa;
		if(aaa) bbb++;


		for(int j=0;j<50;j++){
			for(int k=0;k<SAMPLE;k++){
				fin >> val[j*SAMPLE+k];
			}
			for(int l=0;l<3*SAMPLE;l++) fin >> tmp;
		}

		fin.close();

		for(int j=0;j<50*SAMPLE;j++){
			if(!i){
				max_val[j] = val[j];
				max_val_n[j]=0;
			}
			else{
				if(max_val[j] < val[j]){
					max_val[j] = val[j];
					max_val_n[j] = i;
				}
			}
		}
	}

	for(int j=0;j<50*SAMPLE;j++)
		if(max_val_n[j] == (int) j / SAMPLE) result++;

	cout << (double) bbb / 50 << "\n";
	return (double) result / (50*SAMPLE);
}

double check(){
	ifstream fin;
	int result=0;
	int aaa = 0;
	int bbb = 0;
	char str[80];
	double val[CLASS*SAMPLE];
	double max_val[CLASS*SAMPLE];
	int max_val_n[CLASS*SAMPLE];

	for(int i=0;i<CLASS;i++){
		sprintf(str, "./AResult%d_%d/Result_%d.txt", Number, MIX, i+1);
		fin.open(str);

		if(!fin){
			cout << "Can't open input file: " << str << "\n";
			return 1;
		}


		fin >> aaa;
		if(aaa) bbb++;
		for(int j=0;j<CLASS*SAMPLE;j++) fin >> val[j];

		fin.close();

		for(int j=0;j<CLASS*SAMPLE;j++){
			if(!i){
				max_val[j] = val[j];
				max_val_n[j]=0;
			}
			else{
				if(max_val[j] < val[j]){
					max_val[j] = val[j];
					max_val_n[j] = i;
				}
			}
		}
	}

	for(int j=0;j<CLASS*SAMPLE;j++)
		if(max_val_n[j] == (int) j / SAMPLE) result++;

	cout << (double) bbb / CLASS << "\n";
	return (double) result / (CLASS*SAMPLE);
}


libhmmBuilderDialog::libhmmBuilderDialog(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

libhmmBuilderDialog::~libhmmBuilderDialog()
{

}

void libhmmBuilderDialog::closeEvent(QCloseEvent * event)
{
	emit libhmmBuilderDlgClosed();
}

void libhmmBuilderDialog::on_pushButton_Generate_clicked()
{
	m_mHMM.clear();

	ColumnVector P(Number);
	Matrix T(Number,Number);

	if(Only_Check)
	{
		cout << check2() << "\n";
	}

	/*  left-to-right*/
	for(int i=0;i<Number;i++){
		if(i) P.element(i) = 0.0;
		else P.element(i) = 1.0;
	}
	for(int i=0;i<Number;i++){
		for(int j=0;j<Number;j++){
			T.element(i,j) = 0.0;
			if(i==j-1) T.element(i,j) = 1.0;
			else if(i==j) T.element(i,j) = 1.0;
		}
	}

	QString strPath = ui.lineEdit_InputFilePath->text();
	if (!strPath.endsWith('/') && !strPath.endsWith('\\'))
	{
		strPath += "/";
	}
	QStringList filters;
	filters << "*.bvh";
	QDir dir(strPath);
	QStringList list = dir.entryList(filters);

	MotionClip *pmc1;
	BVHReader reader1("initial.bvh");
	reader1.read();
	pmc1 = reader1.result();

	InfoCalcualtor calculator;

	for (int l=0; l<list.count(); l++)
	{
		QString strFileName, strMotionName;
		strFileName = strMotionName = list.at(l);
		strMotionName.remove(".bvh");
		std::string strtmp = strPath.toLocal8Bit()+strFileName.toLocal8Bit();
		BVHReader reader(strtmp);
		reader.read();
		MotionClip* pmc = reader.result();
		if (pmc)
		{
			MotionJoint *pmjlw, *pmjle, *pmjrw, *pmjre, *pmjlw1, *pmjle1, *pmjrw1, *pmjre1;
			pmjlw = pmc->findJoint("l_wrist");
			pmjle = pmc->findJoint("l_elbow");
			pmjrw = pmc->findJoint("r_wrist");
			pmjre = pmc->findJoint("r_elbow");
			pmjlw1 = pmc1->findJoint("l_wrist");
			pmjle1 = pmc1->findJoint("l_elbow");
			pmjrw1 = pmc1->findJoint("r_wrist");
			pmjre1 = pmc1->findJoint("r_elbow");
			std::vector<AnnoSeg> vAnnoSeg;
			int nStartFn=0, nEndFn=0, nState=0, nFrameCount;
			double dist,d1,d2,d3,d4,dthreshold=20.0;
			for (int i=0; i<pmc->getFrameCount(); i++)
			{
				d1 = Distance(pmjlw->getAbsolutePosition(i), pmjlw1->getAbsolutePosition(0));
				d2 = Distance(pmjle->getAbsolutePosition(i), pmjle1->getAbsolutePosition(0));
				d3 = Distance(pmjrw->getAbsolutePosition(i), pmjrw1->getAbsolutePosition(0));
				d4 = Distance(pmjre->getAbsolutePosition(i), pmjre1->getAbsolutePosition(0));
				dist = d1+d2+d3+d4;
				if (dist > dthreshold)
				{
					if (nState == 0)
					{
						nStartFn = nEndFn = i;
						nState = 1;
					}
					else if (nState == 1)
					{
						nEndFn++;
					}
				}
				else
				{
					if (nState == 1)
					{
						nEndFn = i;
						nState = 0;
						AnnoSeg as;
						if ((nEndFn-nStartFn) > 30)
						{
							as.nStartFn = nStartFn;
							as.nEndFn = nEndFn;
							as.strAnno = strMotionName.toStdString();
							vAnnoSeg.push_back(as);
						}
					}
				}
			}

			int nSize = vAnnoSeg.size();
			if (strMotionName != "AJOUTER")
				nSize -= 1;

			Matrix* train = new Matrix[nSize-1];

			for (int i=1; i<nSize; i++)
			{
				long nSamples = vAnnoSeg.at(i).nEndFn-vAnnoSeg.at(i).nStartFn+1;
				train[i-1].ReSize(Dimention,nSamples);

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
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 190);
				int k=0;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Right index finger
				pmj0 = pmc->findJoint("r_index0");	
				pmj1 = pmc->findJoint("r_index1");	
				pmj2 = pmc->findJoint("r_index2");	
				pmj3 = pmc->findJoint("r_index3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Right middle finger
				pmj0 = pmc->findJoint("r_middle0");	
				pmj1 = pmc->findJoint("r_middle1");	
				pmj2 = pmc->findJoint("r_middle2");	
				pmj3 = pmc->findJoint("r_middle3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Right ring finger
				pmj0 = pmc->findJoint("r_ring0");	
				pmj1 = pmc->findJoint("r_ring1");	
				pmj2 = pmc->findJoint("r_ring2");	
				pmj3 = pmc->findJoint("r_ring3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Right little finger
				pmj0 = pmc->findJoint("r_pinky0");	
				pmj1 = pmc->findJoint("r_pinky1");	
				pmj2 = pmc->findJoint("r_pinky2");	
				pmj3 = pmc->findJoint("r_pinky3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left thumb finger
				pmj0 = pmc->findJoint("l_thumb0");	
				pmj1 = pmc->findJoint("l_thumb1");	
				pmj2 = pmc->findJoint("l_thumb2");	
				pmj3 = pmc->findJoint("l_thumb3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 190);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left index finger
				pmj0 = pmc->findJoint("l_index0");	
				pmj1 = pmc->findJoint("l_index1");	
				pmj2 = pmc->findJoint("l_index2");	
				pmj3 = pmc->findJoint("l_index3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left middle finger
				pmj0 = pmc->findJoint("l_middle0");	
				pmj1 = pmc->findJoint("l_middle1");	
				pmj2 = pmc->findJoint("l_middle2");	
				pmj3 = pmc->findJoint("l_middle3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left ring finger
				pmj0 = pmc->findJoint("l_ring0");	
				pmj1 = pmc->findJoint("l_ring1");	
				pmj2 = pmc->findJoint("l_ring2");	
				pmj3 = pmc->findJoint("l_ring3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				// Left little finger
				pmj0 = pmc->findJoint("l_pinky0");	
				pmj1 = pmc->findJoint("l_pinky1");	
				pmj2 = pmc->findJoint("l_pinky2");	
				pmj3 = pmc->findJoint("l_pinky3");	
				pmj4 = pmc->getChild(pmj3, 0);
				calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vFlex.at(j); 
				}
				vFlex.clear();

				pmj1 = pmc->findJoint("r_wrist");
				pmj2 = pmc->findJoint("root");
				calculator.calDist(pmj1, pmj2, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vDist);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vDist.at(j); 
				}
				vDist.clear();

				pmj2 = pmc->findJoint("skullbase");
				calculator.calDist(pmj1, pmj2, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vDist);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vDist.at(j); 
				}
				vDist.clear();

				pmj1 = pmc->findJoint("l_wrist");
				pmj2 = pmc->findJoint("root");
				calculator.calDist(pmj1, pmj2, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vDist);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vDist.at(j); 
				}
				vDist.clear();

				pmj2 = pmc->findJoint("skullbase");
				calculator.calDist(pmj1, pmj2, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vDist);
				k++;
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k,j)=vDist.at(j); 
				}
				vDist.clear();

				pmj1 = pmc->findJoint("r_wrist");
				pmj2 = pmc->findJoint("r_index1");
				pmj3 = pmc->findJoint("r_ring1");
				calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vOriX, vOriY, vOriZ);
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k+1,j)=vOriX.at(j);
					train[i-1].element(k+2,j)=vOriY.at(j);
					train[i-1].element(k+3,j)=vOriZ.at(j);
				}
				k+=3;
				vOriX.clear();
				vOriY.clear();
				vOriZ.clear();

				pmj1 = pmc->findJoint("l_wrist");
				pmj2 = pmc->findJoint("l_index1");
				pmj3 = pmc->findJoint("l_ring1");
				calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vOriX, vOriY, vOriZ);
				for (int j=0; j<nSamples; j++)
				{
					train[i-1].element(k+1,j)=vOriX.at(j);
					train[i-1].element(k+2,j)=vOriY.at(j);
					train[i-1].element(k+3,j)=vOriZ.at(j);
				}
				k+=3;
				vOriX.clear();
				vOriY.clear();
				vOriZ.clear();
			}

			HMM* phmm = new HMM(Number,Dimention,MIX,P,T);
			phmm->set_HMM();
			phmm->init_segmental(train,nSize-1);
			phmm->estimations(train,nSize-1,1);
			m_mHMM.insert(std::pair<std::string, HMM*>(strMotionName.toStdString(), phmm));
		}
	}
}

void libhmmBuilderDialog::on_pushButton_Save_clicked()
{
	std::string str, strtmp;
	str = ui.lineEdit_OutputFilePath->text().toStdString();
	str += "/";
	for (std::map<std::string, HMM*>::iterator it=m_mHMM.begin(); it!=m_mHMM.end(); it++)
	{
		strtmp = str + it->first;
		const char* pCharCom = strtmp.c_str();
		char* tmp = new char[strlen(pCharCom)+1];
		strcpy(tmp, pCharCom);
		it->second->Save(tmp);
	}
}

void libhmmBuilderDialog::on_pushButton_Test1_clicked()
{
	MotionClip *pmc1;
	BVHReader reader1("initial.bvh");
	reader1.read();
	pmc1 = reader1.result();

	InfoCalcualtor calculator;

	QString strFileName = QFileDialog::getOpenFileName(this,tr("Open Motion File"),SwiftModel::instance().m_strLastPath,tr("Motion Files (*.bvh)"));
	if (strFileName.isEmpty())
		return;
	int n1 = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n1+1);

	QString strMotionName;
	int nSize = strFileName.size();
	strMotionName = strFileName.right(nSize-n1-1);
	strMotionName.remove(".bvh");

	BVHReader reader(strFileName.toStdString());
	reader.read();
	MotionClip* pmc = reader.result();
	if (pmc)
	{
		MotionJoint *pmjlw, *pmjle, *pmjrw, *pmjre, *pmjlw1, *pmjle1, *pmjrw1, *pmjre1;
		pmjlw = pmc->findJoint("l_wrist");
		pmjle = pmc->findJoint("l_elbow");
		pmjrw = pmc->findJoint("r_wrist");
		pmjre = pmc->findJoint("r_elbow");
		pmjlw1 = pmc1->findJoint("l_wrist");
		pmjle1 = pmc1->findJoint("l_elbow");
		pmjrw1 = pmc1->findJoint("r_wrist");
		pmjre1 = pmc1->findJoint("r_elbow");
		std::vector<AnnoSeg> vAnnoSeg;
		int nStartFn=0, nEndFn=0, nState=0, nFrameCount;
		double dist,d1,d2,d3,d4,dthreshold=20.0;
		for (int i=0; i<pmc->getFrameCount(); i++)
		{
			d1 = Distance(pmjlw->getAbsolutePosition(i), pmjlw1->getAbsolutePosition(0));
			d2 = Distance(pmjle->getAbsolutePosition(i), pmjle1->getAbsolutePosition(0));
			d3 = Distance(pmjrw->getAbsolutePosition(i), pmjrw1->getAbsolutePosition(0));
			d4 = Distance(pmjre->getAbsolutePosition(i), pmjre1->getAbsolutePosition(0));
			dist = d1+d2+d3+d4;
			if (dist > dthreshold)
			{
				if (nState == 0)
				{
					nStartFn = nEndFn = i;
					nState = 1;
				}
				else if (nState == 1)
				{
					nEndFn++;
				}
			}
			else
			{
				if (nState == 1)
				{
					nEndFn = i;
					nState = 0;
					AnnoSeg as;
					if ((nEndFn-nStartFn) > 30)
					{
						as.nStartFn = nStartFn;
						as.nEndFn = nEndFn;
						as.strAnno = strMotionName.toStdString();
						vAnnoSeg.push_back(as);
					}
				}
			}
		}

		int nSize = vAnnoSeg.size();
		if (strMotionName != "AJOUTER")
			nSize -= 1;

		Matrix test;

		for (int i=1; i<nSize; i++)
		{
			long nSamples = vAnnoSeg.at(i).nEndFn-vAnnoSeg.at(i).nStartFn+1;
			test.ReSize(Dimention,nSamples);

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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 190);
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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 190);
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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
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
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vFlex, 250);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			pmj1 = pmc->findJoint("r_wrist");
			pmj2 = pmc->findJoint("root");
			calculator.calDist(pmj1, pmj2, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vDist);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vDist.at(j); 
			}
			vDist.clear();

			pmj2 = pmc->findJoint("skullbase");
			calculator.calDist(pmj1, pmj2, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vDist);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vDist.at(j); 
			}
			vDist.clear();

			pmj1 = pmc->findJoint("l_wrist");
			pmj2 = pmc->findJoint("root");
			calculator.calDist(pmj1, pmj2, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vDist);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vDist.at(j); 
			}
			vDist.clear();

			pmj2 = pmc->findJoint("skullbase");
			calculator.calDist(pmj1, pmj2, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vDist);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vDist.at(j); 
			}
			vDist.clear();

			pmj1 = pmc->findJoint("r_wrist");
			pmj2 = pmc->findJoint("r_index1");
			pmj3 = pmc->findJoint("r_ring1");
			calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vOriX, vOriY, vOriZ);
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
			calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn, vOriX, vOriY, vOriZ);
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
		}

		double dmax = -1.0e100, d;
		std::string str;
		for (std::map<std::string, HMM*>::iterator it=m_mHMM.begin(); it!=m_mHMM.end(); it++)
		{
			d = it->second->viterbi_p(test);
			if (d > dmax)
			{
				dmax = d;
				str = it->first;
			}
		}

		int nn;
	}
}

void libhmmBuilderDialog::on_pushButton_Test2_clicked()
{
	QString strPath = ui.lineEdit_InputFilePath->text();
	if (!strPath.endsWith('/') && !strPath.endsWith('\\'))
	{
		strPath += "/";
	}
	QStringList filters;
	filters << "*.bvh";
	QDir dir(strPath);
	QStringList list = dir.entryList(filters);

	MotionClip *pmc1;
	BVHReader reader1("initial.bvh");
	reader1.read();
	pmc1 = reader1.result();

	InfoCalcualtor calculator;

	for (int l=0; l<list.count(); l++)
	{
		QString strFileName, strMotionName;
		strFileName = strMotionName = list.at(l);
		strMotionName.remove(".bvh");
		std::string strtmp = strPath.toLocal8Bit()+strFileName.toLocal8Bit();
		BVHReader reader(strtmp);
		reader.read();
		MotionClip* pmc = reader.result();
		if (pmc)
		{
			MotionJoint *pmjlw, *pmjle, *pmjrw, *pmjre, *pmjlw1, *pmjle1, *pmjrw1, *pmjre1;
			pmjlw = pmc->findJoint("l_wrist");
			pmjle = pmc->findJoint("l_elbow");
			pmjrw = pmc->findJoint("r_wrist");
			pmjre = pmc->findJoint("r_elbow");
			pmjlw1 = pmc1->findJoint("l_wrist");
			pmjle1 = pmc1->findJoint("l_elbow");
			pmjrw1 = pmc1->findJoint("r_wrist");
			pmjre1 = pmc1->findJoint("r_elbow");
			std::vector<AnnoSeg> vAnnoSeg;
			int nStartFn=0, nEndFn=0, nState=0, nFrameCount;
			double dist,d1,d2,d3,d4,dthreshold=30.0;
			for (int i=0; i<pmc->getFrameCount(); i++)
			{
				d1 = Distance(pmjlw->getAbsolutePosition(i), pmjlw1->getAbsolutePosition(0));
				d2 = Distance(pmjle->getAbsolutePosition(i), pmjle1->getAbsolutePosition(0));
				d3 = Distance(pmjrw->getAbsolutePosition(i), pmjrw1->getAbsolutePosition(0));
				d4 = Distance(pmjre->getAbsolutePosition(i), pmjre1->getAbsolutePosition(0));
				dist = d1+d2+d3+d4;
				if (dist > dthreshold)
				{
					if (nState == 0)
					{
						nStartFn = nEndFn = i;
						nState = 1;
					}
					else if (nState == 1)
					{
						nEndFn++;
					}
				}
				else
				{
					if (nState == 1)
					{
						nEndFn = i;
						nState = 0;
						AnnoSeg as;
						if ((nEndFn-nStartFn) > 50)
						{
							double dMeanlw=0.0, dMeanle=0.0, dMeanrw=0.0, dMeanre=0.0;
							double dVarlw=0.0, dVarle=0.0, dVarrw=0.0, dVarre=0.0;
							double dCoeflw=0.5, dCoefle=0.5, dCoefrw=0.5, dCoefre=0.5;
							int nFrameCount=nEndFn-nStartFn+1;
							double d;
							for (int i=nStartFn; i<=nEndFn; i++)
							{
								dMeanlw += Distance(pmjlw->getAbsolutePosition(i), pmjlw->getAbsolutePosition(nStartFn));
								dMeanle += Distance(pmjle->getAbsolutePosition(i), pmjle->getAbsolutePosition(nStartFn));
								dMeanrw += Distance(pmjrw->getAbsolutePosition(i), pmjrw->getAbsolutePosition(nStartFn));
								dMeanre += Distance(pmjre->getAbsolutePosition(i), pmjre->getAbsolutePosition(nStartFn));
							}
							dMeanlw /= (double)nFrameCount;
							dMeanle /= (double)nFrameCount;
							dMeanrw /= (double)nFrameCount;
							dMeanre /= (double)nFrameCount;
							for (int i=nStartFn; i<=nEndFn; i++)
							{
								d = Distance(pmjlw->getAbsolutePosition(i), pmjlw->getAbsolutePosition(nStartFn));
								dVarlw += (d-dMeanlw)*(d-dMeanlw);
								d = Distance(pmjle->getAbsolutePosition(i), pmjle->getAbsolutePosition(nStartFn));
								dVarle += (d-dMeanle)*(d-dMeanle);
								d = Distance(pmjrw->getAbsolutePosition(i), pmjrw->getAbsolutePosition(nStartFn));
								dVarrw += (d-dMeanrw)*(d-dMeanrw);
								d = Distance(pmjre->getAbsolutePosition(i), pmjre->getAbsolutePosition(nStartFn));
								dVarre += (d-dMeanre)*(d-dMeanre);
							}
							dVarlw /= (double)nFrameCount;
							dVarle /= (double)nFrameCount;
							dVarrw /= (double)nFrameCount;
							dVarre /= (double)nFrameCount;

							double dVarl = dCoeflw*dVarlw+dCoefle*dVarle;
							double dVarr = dCoefrw*dVarrw+dCoefre*dVarre;
							double dThreshold=20.0;
							if (dVarl+dVarr>dThreshold)
							{
								nStartFn -= 30;
								if (nStartFn < 0)
									nStartFn = 0;
								nEndFn += 30;
								if (nEndFn > pmc->getFrameCount()-1)
									nEndFn = pmc->getFrameCount()-1;
								as.nStartFn = nStartFn;
								as.nEndFn = nEndFn;
								as.strAnno = strMotionName.toStdString();
								vAnnoSeg.push_back(as);
							}
						}
					}
				}
			}
			int nSize = vAnnoSeg.size();
			if (strMotionName != "AJOUTER")
				nSize -= 1;

			MotionEditor editor;
			BVHWriter writer;
			MotionClip* pmctmp;
			QString strOutPath;
			int nHalf = nSize/2;
			for (int i=1; i<nSize; i++)
			{
				pmctmp = editor.CreateSubMotion(pmc, vAnnoSeg.at(i).nStartFn, vAnnoSeg.at(i).nEndFn);
				strOutPath = ui.lineEdit_OutputFilePath->text();
				if (!strOutPath.endsWith('/') && !strOutPath.endsWith('\\'))
				{
					strOutPath += "/";
				}
				//writer.setFilePathName(strOutPath.toStdString()+vAnnoSeg.at(i).strAnno+"_"+QString::number(i).toStdString()+".bvh");
				//writer.setSource(pmctmp);
				//writer.setSpacing("  ");
				//writer.write();
				if (i<=nHalf)
					writer.setFilePathName("D:/Projects/SignCom/Material/Data/TrainAndTest/Train/"+vAnnoSeg.at(i).strAnno+"_"+QString::number(i).toStdString()+".bvh");
				else
					writer.setFilePathName("D:/Projects/SignCom/Material/Data/TrainAndTest/Test/"+vAnnoSeg.at(i).strAnno+"_"+QString::number(i).toStdString()+".bvh");
				writer.setSource(pmctmp);
				writer.setSpacing("  ");
				writer.write();
				delete pmctmp;
			}
		}
	}
}