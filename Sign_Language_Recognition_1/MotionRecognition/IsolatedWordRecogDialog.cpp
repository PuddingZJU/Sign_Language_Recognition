/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2011/01/27                                                  */
/************************************************************************/

#include <fstream>
#include <ostream>
#include "../SwiftModel/SwiftModel.h"
#include "../SwiftModel/HMMModel.h"
#include "../SceneViewer/Skeleton.h"
#include "../MotionData/BVHReader.h"
#include "../MotionData/MotionEditor.h"
//#include "../HTKBuilder/LVite.h"
//#include "../HTKBuilder/HTKWriter.h"
#include "../MotionRecognition/InfoCalculator.h"
#include "IsolatedWordRecogDialog.h"
#include <QFileDialog>
using namespace Swift;

IsolatedWordRecogDialog::IsolatedWordRecogDialog(QWidget *parent)
	: QWidget(parent), m_pCurrentMC(NULL), m_nPlayState(ONEROUND), m_nStartFn(0), m_nEndFn(0), m_nState(0)
{
	ui.setupUi(this);

	// Add OpenGL view
	m_glWidget = new GLWidget(0, 0, PreviewMode);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_glWidget);
	ui.widget_View->setLayout(layout);

	// play controls
	m_timerPreview.setInterval(1000/60);
	m_nFrameRate = 30;
	m_fPlaySpeed = 1.0f;

	connect(&m_timerPreview, SIGNAL(timeout()), this, SLOT(onPlayTimer()));
	connect(ui.radioButton_Stick, SIGNAL(clicked()), this, SLOT(on_radioButton_Stick_clicked()));
	connect(ui.radioButton_Ellipse, SIGNAL(clicked()), this, SLOT(on_radioButton_Ellipse_clicked()));
	connect(ui.slider_Play, SIGNAL(sliderMoved(int)), this, SLOT(setCurrentFrame(int)));
}

IsolatedWordRecogDialog::~IsolatedWordRecogDialog()
{

}

void IsolatedWordRecogDialog::closeEvent(QCloseEvent * event)
{
	emit IsolatedWordRecogDlgClosed();
}

void IsolatedWordRecogDialog::on_pushButton_Play_clicked()
{
	enablePlayTimer(true);
}

void IsolatedWordRecogDialog::on_pushButton_StopPlay_clicked()
{
	enablePlayTimer(false);
}

void IsolatedWordRecogDialog::on_radioButton_Stick_clicked()
{
	if (ui.radioButton_Stick->isChecked())
	{
		if(m_pCurrentMC)
		{
			PreviewSceneManager* pSceneManager = (PreviewSceneManager*) m_glWidget->getSceneManager();
			pSceneManager->createSkeleton(m_pCurrentMC,false,SOT_SPHERE);

			updatePlayControls();
			m_glWidget->updateGL();
		}
	}
}

void IsolatedWordRecogDialog::on_radioButton_Ellipse_clicked()
{
	if (ui.radioButton_Ellipse->isChecked())
	{
		if(m_pCurrentMC)
		{
			PreviewSceneManager* pSceneManager = (PreviewSceneManager*) m_glWidget->getSceneManager();
			pSceneManager->createSkeleton(m_pCurrentMC,false,SOT_ELLIPSE);

			updatePlayControls();
			m_glWidget->updateGL();
		}
	}
}

void IsolatedWordRecogDialog::on_pushButton_LoadMotion_clicked()
{
	//QString strFileName = QFileDialog::getOpenFileName(this,tr("Open Motion File"),QDir::currentPath(),tr("Motion Files (*.bvh)"));
	QString strFileName = QFileDialog::getOpenFileName(this,tr("Open Motion File"),SwiftModel::instance().m_strLastPath,tr("Motion Files (*.bvh)"));
	if (strFileName.isEmpty())
		return;
	int n1 = strFileName.lastIndexOf("/");
	SwiftModel::instance().m_strLastPath = strFileName.left(n1+1);

	ui.lineEdit_MotionFile->setText(strFileName);
	BVHReader reader(strFileName.toStdString());
	reader.read();
	m_pCurrentMC = reader.result();

	if(m_pCurrentMC)
	{
		m_nFrameRate = 1.0/m_pCurrentMC->getFrameTime();
		m_timerPreview.setInterval(1000/m_nFrameRate);

		PreviewSceneManager* pSceneManager = (PreviewSceneManager*) m_glWidget->getSceneManager();
		if (ui.radioButton_Ellipse->isChecked())
			pSceneManager->createSkeleton(m_pCurrentMC,false,SOT_ELLIPSE);
		else
			pSceneManager->createSkeleton(m_pCurrentMC,false);

		updatePlayControls();
		m_glWidget->updateGL();
	}
}

void IsolatedWordRecogDialog::on_pushButton_LoadModel_clicked()
{
	SwiftModel::instance().loadRModel("./MLModel/RuleModel.rmo");
	SwiftModel::instance().loadFModel("./MLModel/FeatureModel.fsc");
	HMMModel::instance().loadHMMs(QString("./MLModel/HMM/"));
}

void IsolatedWordRecogDialog::updatePlayControls()
{
	int last = 0;
	if (m_pCurrentMC)
	{
		last = m_pCurrentMC->getFrameCount() - 1;
	}

	ui.slider_Play->setMinimum(0);
	ui.slider_Play->setMaximum(last);
}

bool IsolatedWordRecogDialog::changeCurrentFrame(int offset)
{
	PreviewSceneManager* pSceneManager = (PreviewSceneManager*)m_glWidget->getSceneManager();
	int n = pSceneManager->getCurrentFrameNo() + offset;
	MotionClip* pmc = pSceneManager->m_pSkeleton->getMotionClip();
	if(pmc)
	{
		int last = pmc->getFrameCount()-1;

		if (m_nPlayState == CYCLING)
		{
			if (n > last)
			{
				n = 0;

				MotionEditor editor;
				Vector3 preTransition (0.0, 0.0, 0.0);
				Quaternion preOrientation = Quaternion::IDENTITY;

				editor.Align(
					m_pCurrentMC,
					m_pCurrentMC->getFrameCount()-1,
					m_pCurrentMC,
					0,
					preTransition,
					preOrientation
					);

				preTransition.set_y(0);

				editor.TransformMotion(
					m_pCurrentMC,
					false,
					0,
					m_pCurrentMC->getFrameCount()-1,
					preTransition,
					preOrientation 
					);
			}
		}

	}
	std::string strFile = "test.txt";
	std::ofstream out(strFile.c_str(),ios::app);
	out<<offset<<"frames  "<<endl;
	return setCurrentFrame(n);
}

bool IsolatedWordRecogDialog::setCurrentFrame(int n)
{
	PreviewSceneManager* pSceneManager = (PreviewSceneManager*)m_glWidget->getSceneManager();
	if (pSceneManager && pSceneManager->m_pSkeleton)
	{
		MotionClip* pmc = pSceneManager->m_pSkeleton->getMotionClip();
		if(pmc)
		{
			int last = pmc->getFrameCount()-1;
			if (n>=0 && n <= last)
			{
				pSceneManager->setCurrentFrameNo(n);
				pSceneManager->updateDataFromMotionClip(n);
				m_glWidget->updateGL();
				ui.label_Frame->setNum(n);
				ui.slider_Play->setSliderPosition(n);

				MotionClip *pmc1;
				BVHReader reader1("initial.bvh");
				reader1.read();
				pmc1 = reader1.result();

				MotionJoint *pmjlw, *pmjle, *pmjrw, *pmjre, *pmjlw1, *pmjle1, *pmjrw1, *pmjre1;
				pmjlw = pmc->findJoint("l_wrist");
				pmjle = pmc->findJoint("l_elbow");
				pmjrw = pmc->findJoint("r_wrist");
				pmjre = pmc->findJoint("r_elbow");
				pmjlw1 = pmc1->findJoint("l_wrist");
				pmjle1 = pmc1->findJoint("l_elbow");
				pmjrw1 = pmc1->findJoint("r_wrist");
				pmjre1 = pmc1->findJoint("r_elbow");
				double dist,d1,d2,d3,d4,dthreshold=20.0;
				d1 = Distance(pmjlw->getAbsolutePosition(n), pmjlw1->getAbsolutePosition(0));
				d2 = Distance(pmjle->getAbsolutePosition(n), pmjle1->getAbsolutePosition(0));
				d3 = Distance(pmjrw->getAbsolutePosition(n), pmjrw1->getAbsolutePosition(0));
				d4 = Distance(pmjre->getAbsolutePosition(n), pmjre1->getAbsolutePosition(0));
				dist = d1+d2+d3+d4;

				/*
				std::string strFile = "test.txt";
				std::ofstream out(strFile.c_str(),ios::app);
	            out<<n<<"frames  "<<dist<<"dists"<<endl;
				*/

				if (dist > dthreshold)
				{
					ui.radioButton_Signing->setChecked(true);
					if (m_nState == 0)
					{
						m_nStartFn = m_nEndFn = n;
						m_nState = 1;
					}
					else if (m_nState == 1)
					{
						m_nEndFn++;
					}
				}
				else
				{
					ui.radioButton_Idle->setChecked(true);
					if (m_nState == 1)
					{
						m_nEndFn = n;
						m_nState = 0;
						//if ((m_nEndFn-m_nStartFn) > 30)
						//{
						//	recognize(m_pCurrentMC, m_nStartFn, m_nEndFn);
						//}
						if ((m_nEndFn-m_nStartFn) > 50)
						{
							double dMeanlw=0.0, dMeanle=0.0, dMeanrw=0.0, dMeanre=0.0;
							double dVarlw=0.0, dVarle=0.0, dVarrw=0.0, dVarre=0.0;
							double dCoeflw=0.5, dCoefle=0.5, dCoefrw=0.5, dCoefre=0.5;
							int nFrameCount=m_nEndFn-m_nStartFn+1;
							double d;
							for (int i=m_nStartFn; i<=m_nEndFn; i++)
							{
								dMeanlw += Distance(pmjlw->getAbsolutePosition(i), pmjlw->getAbsolutePosition(m_nStartFn));
								dMeanle += Distance(pmjle->getAbsolutePosition(i), pmjle->getAbsolutePosition(m_nStartFn));
								dMeanrw += Distance(pmjrw->getAbsolutePosition(i), pmjrw->getAbsolutePosition(m_nStartFn));
								dMeanre += Distance(pmjre->getAbsolutePosition(i), pmjre->getAbsolutePosition(m_nStartFn));
							}
							dMeanlw /= (double)nFrameCount;
							dMeanle /= (double)nFrameCount;
							dMeanrw /= (double)nFrameCount;
							dMeanre /= (double)nFrameCount;
							for (int i=m_nStartFn; i<=m_nEndFn; i++)
							{
								d = Distance(pmjlw->getAbsolutePosition(i), pmjlw->getAbsolutePosition(m_nStartFn));
								dVarlw += (d-dMeanlw)*(d-dMeanlw);
								d = Distance(pmjle->getAbsolutePosition(i), pmjle->getAbsolutePosition(m_nStartFn));
								dVarle += (d-dMeanle)*(d-dMeanle);
								d = Distance(pmjrw->getAbsolutePosition(i), pmjrw->getAbsolutePosition(m_nStartFn));
								dVarrw += (d-dMeanrw)*(d-dMeanrw);
								d = Distance(pmjre->getAbsolutePosition(i), pmjre->getAbsolutePosition(m_nStartFn));
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
								m_nStartFn -= 30;
								if (m_nStartFn < 0)
									m_nStartFn = 0;

								m_nEndFn += 30;
								if (m_nEndFn > last)
									m_nEndFn = last;
								recognize(m_pCurrentMC, m_nStartFn, m_nEndFn);
							}
						}
					}
				}

				return true;
			}
		}
	}
	return false;
}

void IsolatedWordRecogDialog::enablePlayTimer(bool start)
{
	if (start)
	{
		m_time.restart();
		m_timerPreview.start();
	}
	else
	{
		m_timerPreview.stop();
	}
}

void IsolatedWordRecogDialog::onPlayTimer()
{
	int nDeltaTime = m_time.elapsed(); // get delta, and start again. see Qt doc.
	static float fAccumlatedDeltaFrame =0;
	float fDeltaFrame = 
		m_fPlaySpeed * m_nFrameRate * nDeltaTime / 1000.0f;
	fAccumlatedDeltaFrame+=fDeltaFrame;

	int nRoundedDeltaFrame = static_cast<int>(fAccumlatedDeltaFrame);

	if (nRoundedDeltaFrame > 0)
	{
		m_time.restart();
		int nDeltaTime = m_time.elapsed(); // get delta, and start again. see Qt doc.
		static float fAccumlatedDeltaFrame =0;
		float fDeltaFrame = 
			m_fPlaySpeed * m_nFrameRate * nDeltaTime / 1000.0f;
		fAccumlatedDeltaFrame+=fDeltaFrame;

		int nRoundedDeltaFrame = static_cast<int>(fAccumlatedDeltaFrame);//这样保证每一帧都能显示出来

		if (nRoundedDeltaFrame > 0)        //可以保证每帧都能显示出来
		{
			//在这里计算一下当前这五帧（步长先设为5）的方差小于某一阈值，就认为他是静止片段（过渡片段）
			int nFrameCount=3;
			PreviewSceneManager* pSceneManager = (PreviewSceneManager*) m_glWidget->getSceneManager();
			MotionClip* pmc = pSceneManager->m_pSkeleton->getMotionClip();
			MotionClip *pmcRe;
			int n = pSceneManager->getCurrentFrameNo();
			n-=nFrameCount;
			if(n>=0)            //前面没有5帧,直接返回
			{
				MotionEditor edior;
				pmcRe=edior.CreateSubMotion(pmc,n,n+nFrameCount);
				//对这nFrameCount帧进行计算
				MotionJoint *pmjlw, *pmjle, *pmjrw, *pmjre;
				pmjlw = pmcRe->findJoint("l_wrist");//当前动作和初始动作之间的距离
				pmjle = pmcRe->findJoint("l_elbow");
				pmjrw = pmcRe->findJoint("r_wrist");
				pmjre = pmcRe->findJoint("r_elbow");

				int i;
				double dMeanlw=0.0, dMeanle=0.0, dMeanrw=0.0, dMeanre=0.0;
				double dVarlw=0.0, dVarle=0.0, dVarrw=0.0, dVarre=0.0;
				double dCoeflw=0.5, dCoefle=0.5, dCoefrw=0.5, dCoefre=0.5;
				for(i=0;i<nFrameCount;i++)
				{//把第一帧看做是0，后面的是相对第一帧的距离，如0,1,2,3,4
					dMeanlw += Distance(pmjlw->getAbsolutePosition(i), pmjlw->getAbsolutePosition(0));//这个函数下标从零开始
					dMeanle += Distance(pmjle->getAbsolutePosition(i), pmjle->getAbsolutePosition(0));//第一帧和第一帧的距离肯定是0
					dMeanrw += Distance(pmjrw->getAbsolutePosition(i), pmjrw->getAbsolutePosition(0));
					dMeanre += Distance(pmjre->getAbsolutePosition(i), pmjre->getAbsolutePosition(0));
				}

				dMeanlw /= (double)nFrameCount;
				dMeanle /= (double)nFrameCount;
				dMeanrw /= (double)nFrameCount;
				dMeanre /= (double)nFrameCount;
				double d;
				for (int i=0; i<nFrameCount; i++)
				{
					d = Distance(pmjlw->getAbsolutePosition(i), pmjlw->getAbsolutePosition(0));//
					dVarlw += (d-dMeanlw)*(d-dMeanlw);
					d = Distance(pmjle->getAbsolutePosition(i), pmjle->getAbsolutePosition(0));
					dVarle += (d-dMeanle)*(d-dMeanle);
					d = Distance(pmjrw->getAbsolutePosition(i), pmjrw->getAbsolutePosition(0));
					dVarrw += (d-dMeanrw)*(d-dMeanrw);
					d = Distance(pmjre->getAbsolutePosition(i), pmjre->getAbsolutePosition(0));
					dVarre += (d-dMeanre)*(d-dMeanre);
				}
				dVarlw /= (double)nFrameCount;
				dVarle /= (double)nFrameCount;
				dVarrw /= (double)nFrameCount;
				dVarre /= (double)nFrameCount;
				//计算这一系列帧的方差
				double dVarl = dCoeflw*dVarlw+dCoefle*dVarle;
				double dVarr = dCoefrw*dVarrw+dCoefre*dVarre;
				double dThreshold=0.002;//刚才20

				std::string strFile = "test.txt";
				std::ofstream out(strFile.c_str(),ios::app);
				out<<pSceneManager->getCurrentFrameNo()<<"frames  "<<dVarl+dVarr<<"vars"<<endl;

				ui.textBrowser_Result->setText(QString::number(dVarl+dVarr));
		
	
				if (dVarl+dVarr<dThreshold && pSceneManager->getCurrentFrameNo() >= 140)//方差小于一个阈值，
				{
					enablePlayTimer(false);
				}
			}
//***************************************************************************************************		
			m_time.restart();
       

			if (! changeCurrentFrame( nRoundedDeltaFrame))//nRoundedDeltaFrame代表位移(int offset)
			{
				enablePlayTimer(false);
			}
			fAccumlatedDeltaFrame -= nRoundedDeltaFrame;
		}
	}
}

//void IsolatedWordRecogDialog::recognize(MotionClip* pmc, int nStartFn, int nEndFn)
//{
//	long nSamples = nEndFn-nStartFn+1;
//	short nDimension = 20;
//	float fPeriodms = 1000*pmc->getFrameTime();
//	short kind=H_USER;
//	float **pOriginalData;
//	pOriginalData = new float*[nSamples];
//	for (int t=0; t<nSamples; t++)
//	{
//		pOriginalData[t] = new float[nDimension];
//	}
//
//	InfoCalcualtor calculator;
//	std::vector<double> vFlex;
//	std::vector<double> vDist;
//	std::vector<double> vOriX, vOriY, vOriZ;
//	MotionJoint *pmj0, *pmj1, *pmj2, *pmj3, *pmj4;
//	// Right thumb finger
//	pmj0 = pmc->findJoint("r_thumb0");	
//	pmj1 = pmc->findJoint("r_thumb1");	
//	pmj2 = pmc->findJoint("r_thumb2");	
//	pmj3 = pmc->findJoint("r_thumb3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 190);
//	int k=0;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	// Right index finger
//	pmj0 = pmc->findJoint("r_index0");	
//	pmj1 = pmc->findJoint("r_index1");	
//	pmj2 = pmc->findJoint("r_index2");	
//	pmj3 = pmc->findJoint("r_index3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 250);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	// Right middle finger
//	pmj0 = pmc->findJoint("r_middle0");	
//	pmj1 = pmc->findJoint("r_middle1");	
//	pmj2 = pmc->findJoint("r_middle2");	
//	pmj3 = pmc->findJoint("r_middle3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 250);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	// Right ring finger
//	pmj0 = pmc->findJoint("r_ring0");	
//	pmj1 = pmc->findJoint("r_ring1");	
//	pmj2 = pmc->findJoint("r_ring2");	
//	pmj3 = pmc->findJoint("r_ring3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 250);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	// Right little finger
//	pmj0 = pmc->findJoint("r_pinky0");	
//	pmj1 = pmc->findJoint("r_pinky1");	
//	pmj2 = pmc->findJoint("r_pinky2");	
//	pmj3 = pmc->findJoint("r_pinky3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 250);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	// Left thumb finger
//	pmj0 = pmc->findJoint("l_thumb0");	
//	pmj1 = pmc->findJoint("l_thumb1");	
//	pmj2 = pmc->findJoint("l_thumb2");	
//	pmj3 = pmc->findJoint("l_thumb3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 190);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	// Left index finger
//	pmj0 = pmc->findJoint("l_index0");	
//	pmj1 = pmc->findJoint("l_index1");	
//	pmj2 = pmc->findJoint("l_index2");	
//	pmj3 = pmc->findJoint("l_index3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 250);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	// Left middle finger
//	pmj0 = pmc->findJoint("l_middle0");	
//	pmj1 = pmc->findJoint("l_middle1");	
//	pmj2 = pmc->findJoint("l_middle2");	
//	pmj3 = pmc->findJoint("l_middle3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 250);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	// Left ring finger
//	pmj0 = pmc->findJoint("l_ring0");	
//	pmj1 = pmc->findJoint("l_ring1");	
//	pmj2 = pmc->findJoint("l_ring2");	
//	pmj3 = pmc->findJoint("l_ring3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 250);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	// Left little finger
//	pmj0 = pmc->findJoint("l_pinky0");	
//	pmj1 = pmc->findJoint("l_pinky1");	
//	pmj2 = pmc->findJoint("l_pinky2");	
//	pmj3 = pmc->findJoint("l_pinky3");	
//	pmj4 = pmc->getChild(pmj3, 0);
//	calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, nStartFn, nEndFn, vFlex, 250);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vFlex.at(j); 
//	}
//	vFlex.clear();
//
//	pmj1 = pmc->findJoint("r_wrist");
//	pmj2 = pmc->findJoint("root");
//	calculator.calDist(pmj1, pmj2, nStartFn, nEndFn, vDist);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vDist.at(j); 
//	}
//	vDist.clear();
//
//	pmj2 = pmc->findJoint("skullbase");
//	calculator.calDist(pmj1, pmj2, nStartFn, nEndFn, vDist);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vDist.at(j); 
//	}
//	vDist.clear();
//
//	pmj1 = pmc->findJoint("l_wrist");
//	pmj2 = pmc->findJoint("root");
//	calculator.calDist(pmj1, pmj2, nStartFn, nEndFn, vDist);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vDist.at(j); 
//	}
//	vDist.clear();
//
//	pmj2 = pmc->findJoint("skullbase");
//	calculator.calDist(pmj1, pmj2, nStartFn, nEndFn, vDist);
//	k++;
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k]=vDist.at(j); 
//	}
//	vDist.clear();
//
//	pmj1 = pmc->findJoint("r_wrist");
//	pmj2 = pmc->findJoint("r_index1");
//	pmj3 = pmc->findJoint("r_ring1");
//	calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, nStartFn, nEndFn, vOriX, vOriY, vOriZ);
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k+1]=vOriX.at(j);
//		pOriginalData[j][k+2]=vOriY.at(j);
//		pOriginalData[j][k+3]=vOriZ.at(j);
//	}
//	k+=3;
//	vOriX.clear();
//	vOriY.clear();
//	vOriZ.clear();
//
//	pmj1 = pmc->findJoint("l_wrist");
//	pmj2 = pmc->findJoint("l_index1");
//	pmj3 = pmc->findJoint("l_ring1");
//	calculator.calPalmOri(pmc, pmj1, pmj2, pmj3, nStartFn, nEndFn, vOriX, vOriY, vOriZ);
//	for (int j=0; j<nSamples; j++)
//	{
//		pOriginalData[j][k+1]=vOriX.at(j);
//		pOriginalData[j][k+2]=vOriY.at(j);
//		pOriginalData[j][k+3]=vOriZ.at(j);
//	}
//	k+=3;
//	vOriX.clear();
//	vOriY.clear();
//	vOriZ.clear();
//
//	HTKWriter writer;
//	std::string strFile = "temp.user";
//	writer.write(strFile, pOriginalData, nSamples, nDimension, fPeriodms, kind);
//
//	for (int i=0; i<nSamples; i++)
//	{
//		delete[] pOriginalData[i];
//	}
//	delete[] pOriginalData;
//
//	QString strN = ui.lineEdit_BestN->text();
//	int n = strN.toInt();
//	LVite vite(20, n);
//	vite.recognize(strFile);
//
//	QString strResult = "The " + strN + " best candidates with their scores: \n\n";
//	for (int i=0; i<vite.m_vResult.size(); i++)
//	{
//		strResult += QString::fromStdString(vite.m_vResult.at(i).strLabel) + "	"  + QString::number(vite.m_vResult.at(i).fScore) + "\n";
//	}
//	ui.textBrowser_Result->setText(strResult);
//}

void IsolatedWordRecogDialog::recognize(MotionClip* pmc, int nStartFn, int nEndFn)
{
	MotionEditor editor;
	MotionClip* pmctmp = editor.CreateSubMotion(pmc, nStartFn, nEndFn);
	if (pmctmp)
	{
		// The first layer
		RuleNode* prn = SwiftModel::instance().m_prm->findCandidateNode(pmctmp, SwiftModel::instance().m_prm->m_pRootRuleNode);
		if (prn)
		{
			std::map<std::string, FStrongClassifier*>::iterator itfsc;
			std::vector<std::string> vFResult;
			// The second layer
			int result=0;
			for (int i=0; i<prn->m_vSigns.size(); i++)
			{
				itfsc = SwiftModel::instance().m_mFStrongClassifier.find(prn->m_vSigns.at(i));
				result = itfsc->second->recognize(pmctmp);
				if (result == 1)
				{
					vFResult.push_back(itfsc->first);
				}
			}
			// The third layer
			MotionJoint *pmjlw, *pmjle, *pmjrw, *pmjre, *pmjlw1, *pmjle1, *pmjrw1, *pmjre1;
			pmjlw = pmctmp->findJoint("l_wrist");
			pmjle = pmctmp->findJoint("l_elbow");
			pmjrw = pmctmp->findJoint("r_wrist");
			pmjre = pmctmp->findJoint("r_elbow");

			Matrix test;
			long nSamples = pmctmp->getFrameCount();
			int nDimention = 20;
			test.ReSize(nDimention,nSamples);

			std::vector<double> vFlex;
			std::vector<double> vDist;
			std::vector<double> vOriX, vOriY, vOriZ;
			MotionJoint *pmj0, *pmj1, *pmj2, *pmj3, *pmj4;
			InfoCalcualtor calculator;
			// Right thumb finger
			pmj0 = pmctmp->findJoint("r_thumb0");	
			pmj1 = pmctmp->findJoint("r_thumb1");	
			pmj2 = pmctmp->findJoint("r_thumb2");	
			pmj3 = pmctmp->findJoint("r_thumb3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 190);
			int k=0;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			// Right index finger
			pmj0 = pmctmp->findJoint("r_index0");	
			pmj1 = pmctmp->findJoint("r_index1");	
			pmj2 = pmctmp->findJoint("r_index2");	
			pmj3 = pmctmp->findJoint("r_index3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			// Right middle finger
			pmj0 = pmctmp->findJoint("r_middle0");	
			pmj1 = pmctmp->findJoint("r_middle1");	
			pmj2 = pmctmp->findJoint("r_middle2");	
			pmj3 = pmctmp->findJoint("r_middle3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			// Right ring finger
			pmj0 = pmctmp->findJoint("r_ring0");	
			pmj1 = pmctmp->findJoint("r_ring1");	
			pmj2 = pmctmp->findJoint("r_ring2");	
			pmj3 = pmctmp->findJoint("r_ring3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			// Right little finger
			pmj0 = pmctmp->findJoint("r_pinky0");	
			pmj1 = pmctmp->findJoint("r_pinky1");	
			pmj2 = pmctmp->findJoint("r_pinky2");	
			pmj3 = pmctmp->findJoint("r_pinky3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			// Left thumb finger
			pmj0 = pmctmp->findJoint("l_thumb0");	
			pmj1 = pmctmp->findJoint("l_thumb1");	
			pmj2 = pmctmp->findJoint("l_thumb2");	
			pmj3 = pmctmp->findJoint("l_thumb3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 190);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			// Left index finger
			pmj0 = pmctmp->findJoint("l_index0");	
			pmj1 = pmctmp->findJoint("l_index1");	
			pmj2 = pmctmp->findJoint("l_index2");	
			pmj3 = pmctmp->findJoint("l_index3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			// Left middle finger
			pmj0 = pmctmp->findJoint("l_middle0");	
			pmj1 = pmctmp->findJoint("l_middle1");	
			pmj2 = pmctmp->findJoint("l_middle2");	
			pmj3 = pmctmp->findJoint("l_middle3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			// Left ring finger
			pmj0 = pmctmp->findJoint("l_ring0");	
			pmj1 = pmctmp->findJoint("l_ring1");	
			pmj2 = pmctmp->findJoint("l_ring2");	
			pmj3 = pmctmp->findJoint("l_ring3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			// Left little finger
			pmj0 = pmctmp->findJoint("l_pinky0");	
			pmj1 = pmctmp->findJoint("l_pinky1");	
			pmj2 = pmctmp->findJoint("l_pinky2");	
			pmj3 = pmctmp->findJoint("l_pinky3");	
			pmj4 = pmctmp->getChild(pmj3, 0);
			calculator.calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, 0, nSamples-1, vFlex, 250);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vFlex.at(j); 
			}
			vFlex.clear();

			pmj1 = pmctmp->findJoint("r_wrist");
			pmj2 = pmctmp->findJoint("root");
			calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vDist.at(j); 
			}
			vDist.clear();

			pmj2 = pmctmp->findJoint("skullbase");
			calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vDist.at(j); 
			}
			vDist.clear();

			pmj1 = pmctmp->findJoint("l_wrist");
			pmj2 = pmctmp->findJoint("root");
			calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vDist.at(j); 
			}
			vDist.clear();

			pmj2 = pmctmp->findJoint("skullbase");
			calculator.calDist(pmj1, pmj2, 0, nSamples-1, vDist);
			k++;
			for (int j=0; j<nSamples; j++)
			{
				test.element(k,j)=vDist.at(j); 
			}
			vDist.clear();

			pmj1 = pmctmp->findJoint("r_wrist");
			pmj2 = pmctmp->findJoint("r_index1");
			pmj3 = pmctmp->findJoint("r_ring1");
			calculator.calPalmOri(pmctmp, pmj1, pmj2, pmj3, 0, nSamples-1, vOriX, vOriY, vOriZ);
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

			pmj1 = pmctmp->findJoint("l_wrist");
			pmj2 = pmctmp->findJoint("l_index1");
			pmj3 = pmctmp->findJoint("l_ring1");
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
			int nBestN = strBestN.toInt();

			std::map<std::string, HMM*>::iterator it;
			for (int i=0; i<vFResult.size(); i++)
			{
				it = HMMModel::instance().m_mHMM.find(vFResult.at(i));
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
					if(vResult.size() == nBestN)
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
			ui.textBrowser_Result->setText("The best "+QString::number(vResult.size())+" candidates and their scores:");
			for (int i=0; i<vResult.size(); i++)
			{
				ui.textBrowser_Result->append(QString::fromStdString(vResult.at(i).strSign)+": "+QString::number(vResult.at(i).dScore));
			}
			ui.textBrowser_Result->append("");
		}
	}
}