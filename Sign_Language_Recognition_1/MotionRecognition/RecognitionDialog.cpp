#include <fstream>
#include "../SceneViewer/Skeleton.h"
#include "../MotionData/BVHReader.h"
#include "../MotionData/MotionEditor.h"
#include "../SwiftModel/SwiftModel.h"
#include "PCARecognizer.h"
#include "../HTKBuilder/LVite.h"
#include "PriFeatureClip.h"
#include "FeatureManager.h"
#include "RecognitionDialog.h"
#include <qfiledialog.h>

using namespace Swift;

RecognitionDialog::RecognitionDialog(QWidget *parent)
	: QWidget(parent), m_pCurrentMC(NULL), m_modelAll(NULL), m_nPlayState(ONEROUND), m_strCurMotion("stance_1"), m_currentType(SOT_SPHERE)
{
	ui.setupUi(this);

	// Add OpenGL view
	m_glWidget = new GLWidget(0, 0, PreviewMode);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_glWidget);
	ui.widget_View->setLayout(layout);

	initTreeViews();
	m_strMotionDBPath = "./MotionDB/";

	// play controls
	m_timerPreview.setInterval(1000/60);
	m_nFrameRate = 30;
	m_fPlaySpeed = 1.0f;

	connect(&m_timerPreview, SIGNAL(timeout()), this, SLOT(onPlayTimer()));
	connect(ui.slider_Play, SIGNAL(sliderMoved (int)), this, SLOT(setCurrentFrame(int)));

	//test();

	//LVite vite(20, 5);
	//vite.recognize("HMM/test/user/SALADE_18.user");

	//PriFeatureClip pfc;
	//FeatureManager fm;
	//fm.genFeatureSet();
	//int n = SwiftModel::instance().m_pfm->m_vFeatureSet.size();
}

void RecognitionDialog::test()
{
	std::vector<std::string> vec;
	QStringList filters;
	filters << "*.bvh";
	//QDir dir("D:/Projects/Pulsar/bin/samples/bvh/");
	QDir dir("D:/Projects/Heron/bin/motion/Backup/ManipsXiubo/EntireFiles/");
	QStringList list = dir.entryList(filters);
	for (int i=0; i<list.count(); i++)
	{
		//vec.push_back("D:/Projects/Pulsar/bin/samples/bvh/" + list.at(i).toStdString());
		vec.push_back("D:/Projects/Heron/bin/motion/Backup/ManipsXiubo/EntireFiles/" + list.at(i).toStdString());
	}

	PCARecognizer rec;
	//vec.push_back("D:/Projects/Pulsar/bin/samples/bvh/left_punch_gouquan_1.bvh");
	rec.test(vec);
	//std::string strResult = rec.recognize("D:/Projects/Pulsar/bin/samples/bvh/left_punch_baiquan_1.bvh");
	for (int i=0; i<list.count(); i++)
	{
		//rec.recognize("D:/Projects/Pulsar/bin/samples/bvh/" + list.at(i).toStdString());
		rec.recognize("D:/Projects/Heron/bin/motion/Backup/ManipsXiubo/EntireFiles/" + list.at(i).toStdString());
	}
}

void RecognitionDialog::initTreeViews()
{
	QTreeView* pTreeViewAll = ui.treeView_All;

	pTreeViewAll->setAlternatingRowColors(true);
	connect(pTreeViewAll, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onClickOneRecordOfAll(const QModelIndex &)));

	if (m_modelAll)
	{
		pTreeViewAll->setModel(0);
		delete m_modelAll;
	}
	m_modelAll = new QStandardItemModel(0, 3, this);

	//m_modelAll->setHeaderData(0, Qt::Horizontal, QObject::tr("MotionID"));
	m_modelAll->setHeaderData(0, Qt::Horizontal, QObject::tr("MotionName"));
	m_modelAll->setHeaderData(1, Qt::Horizontal, QObject::tr("FrameCount"));
	m_modelAll->setHeaderData(2, Qt::Horizontal, QObject::tr("FrameRate"));
	pTreeViewAll->setModel(m_modelAll);
}

RecognitionDialog::~RecognitionDialog()
{

}

void RecognitionDialog::closeEvent(QCloseEvent * event)
{
	emit recognitionDlgClosed();
}

void RecognitionDialog::on_pushButton_LoadConfig_clicked()
{
	//QString strFileName = QFileDialog::getOpenFileName(this,tr("Load Configuration"),QDir::currentPath(),tr("Config Files (*.conf)"));
	QString strFileName = QFileDialog::getOpenFileName(this,tr("Load Configuration"),SwiftModel::instance().m_strLastPath,tr("Config Files (*.conf)"));

	if (!strFileName.isEmpty())
	{
		int n = strFileName.lastIndexOf("/");
		SwiftModel::instance().m_strLastPath = strFileName.left(n+1);

		//std::string dir = strFileName.left(strFileName.lastIndexOf('/')+1).toStdString();
		std::ifstream input(strFileName.toStdString().c_str());

		std::string strData;
		input >> strData;
		while (!input.eof())
		{
			if (!strData.compare("[MotionPath]"))
			{
				input >> m_strMotionDBPath;
				int n = m_strMotionDBPath.find_last_of("//");
				if (n != m_strMotionDBPath.size()-1)
				{
					m_strMotionDBPath += "/";
				}
			}

			input >> strData;
		}

		input.close();
	}

	QString str = "Succeed in loading the config file!";
	ui.textBrowser_Info->append(str);
}

void RecognitionDialog::on_pushButton_LoadMotions_clicked()
{
	m_mMotions.clear();
	int nRowCount = m_modelAll->rowCount();
	for (int i=0; i<nRowCount; i++)
	{
		m_modelAll->takeRow(0);
	}
	QStringList filters;
	filters << "*.bvh";
	QDir dir(QString::fromStdString(m_strMotionDBPath));
	QStringList list = dir.entryList(filters);
	for (int i=0; i<list.count(); i++)
	{
		QString strMotionName = list.at(i);
		std::string strtmp = strMotionName.toLocal8Bit();
		//BVHReader reader(m_strMotionDBPath+strMotionName.toStdString());
		BVHReader reader(m_strMotionDBPath+strtmp);
		reader.read();
		MotionClip* pMotionClip = reader.result();
		if (pMotionClip)
		{
			strMotionName = strMotionName.left(strMotionName.lastIndexOf(".bvh"));
			m_mMotions.insert(std::pair<QString, MotionClip*>(strMotionName, pMotionClip));

			m_modelAll->insertRow(0);
			m_modelAll->setData(m_modelAll->index(0, 0), strMotionName);
			m_modelAll->setData(m_modelAll->index(0, 1), pMotionClip->getFrameCount());
			m_modelAll->setData(m_modelAll->index(0, 2), pMotionClip->getFrameTime());	
		}
	}

	QString str = "Succeed in loading " + QString::number(list.count()) + " file(s)!";
	ui.textBrowser_Info->append(str);
}

void RecognitionDialog::on_pushButton_Play_clicked()
{
	enablePlayTimer(true);
}

void RecognitionDialog::on_pushButton_StopPlay_clicked()
{
	enablePlayTimer(false);
}

void RecognitionDialog::on_radioButton_Stick_clicked()
{
	if (ui.radioButton_Stick->isChecked())
	{
		std::map<QString, MotionClip*>::iterator it = m_mMotions.find(m_strCurMotion);
		if(it != m_mMotions.end())
		{
			m_pCurrentMC = it->second;
			m_currentType = SOT_SPHERE;
			PreviewSceneManager* pSceneManager = (PreviewSceneManager*) m_glWidget->getSceneManager();
			pSceneManager->createSkeleton(m_pCurrentMC,false,m_currentType);

			updatePlayControls();
			m_glWidget->updateGL();
		}
	}
}

void RecognitionDialog::on_radioButton_Ellipse_clicked()
{
	if (ui.radioButton_Ellipse->isChecked())
	{
		std::map<QString, MotionClip*>::iterator it = m_mMotions.find(m_strCurMotion);
		if(it != m_mMotions.end())
		{
			m_pCurrentMC = it->second;
			m_currentType = SOT_ELLIPSE;
			PreviewSceneManager* pSceneManager = (PreviewSceneManager*) m_glWidget->getSceneManager();
			pSceneManager->createSkeleton(m_pCurrentMC,false,m_currentType);

			updatePlayControls();
			m_glWidget->updateGL();
		}
	}
}

void RecognitionDialog::onClickOneRecordOfAll(const QModelIndex & index)
{
	QStandardItem* pItem = m_modelAll->itemFromIndex(index);
	m_strCurMotion = pItem->text();

	std::map<QString, MotionClip*>::iterator it = m_mMotions.find(m_strCurMotion);
	if(it != m_mMotions.end())
	{
		m_pCurrentMC = it->second;

		PreviewSceneManager* pSceneManager = (PreviewSceneManager*) m_glWidget->getSceneManager();
		pSceneManager->createSkeleton(m_pCurrentMC,false,m_currentType);

		updatePlayControls();
		m_glWidget->updateGL();

		setCurrentFrame(0);
		enablePlayTimer(true);
		m_nPlayState = ONEROUND;
	}
}

void RecognitionDialog::updatePlayControls()
{
	int last = 0;
	if (m_pCurrentMC)
	{
		last = m_pCurrentMC->getFrameCount() - 1;
	}

	ui.slider_Play->setMinimum(0);
	ui.slider_Play->setMaximum(last);
}

bool RecognitionDialog::changeCurrentFrame(int offset)
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

				//editor.InplaceBlending(
				//	m_pCurrentMC,

				//	);
			}
		}
		else if (m_nPlayState == ONETOCYCLING)
		{
			if (n > last)
			{
				std::map<QString, MotionClip*>::iterator it = m_mMotions.find(m_strCurMotion);
				if(it != m_mMotions.end())
				{
					MotionClip* pPreviousMC = m_pCurrentMC; 
					m_pCurrentMC = it->second;

					MotionEditor editor;
					Vector3 preTransition (0.0, 0.0, 0.0);
					Quaternion preOrientation = Quaternion::IDENTITY;

					editor.Align(
						pPreviousMC,
						pPreviousMC->getFrameCount()-1,
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

					pSceneManager->createSkeleton(m_pCurrentMC,false);

					updatePlayControls();
					m_glWidget->updateGL();
					m_nPlayState = CYCLING;
					return setCurrentFrame(0);
				}
			}
		}
	}

	return setCurrentFrame(n);
}

bool RecognitionDialog::setCurrentFrame(int n)
{
	PreviewSceneManager* pSceneManager = (PreviewSceneManager*)m_glWidget->getSceneManager();
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

			return true;
		}
	}

	return false;
}

void RecognitionDialog::enablePlayTimer(bool start)
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

void RecognitionDialog::onPlayTimer()
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

		if (! changeCurrentFrame( nRoundedDeltaFrame))
		{
			enablePlayTimer(false);
		}
		fAccumlatedDeltaFrame -= nRoundedDeltaFrame;
	}
}
