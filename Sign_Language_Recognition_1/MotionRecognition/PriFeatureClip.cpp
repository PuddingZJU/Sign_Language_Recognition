/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/10/26                                                  */
/************************************************************************/

#include "../MathLib/Mathematics.h"
#include "../MotionData/BVHReader.h"
#include "../MotionData/spline.h"
#include "PriFeatureClip.h"

using namespace Swift;

PriFeatureClip::PriFeatureClip(MotionClip* pmc/* = NULL*/)
: m_pmc(pmc), m_nFrameCount(0)
{
	////////////////////////////////////////////////////////////////////////////
	//// test
	//BVHReader reader("take_37.bvh");
	//reader.read();
	//m_pmc = reader.result();
	//calFromMC();
	////////////////////////////////////////////////////////////////////////////
}

PriFeatureClip::~PriFeatureClip()
{

}

void PriFeatureClip::calFromMC()
{
	if (m_pmc)
	{
		m_mFlexion.clear();
		m_mAbduction.clear();
		m_mPosition.clear();
		m_mOrientation.clear();
		m_mMovement.clear();
		m_nFrameCount = m_pmc->getFrameCount();

		calFlexion();
		calAbduction();
		calPosition();
		calOrientation();
		calMovement();
	}
}

void PriFeatureClip::calFlexion()
{
	MotionJoint *pmj0, *pmj1, *pmj2, *pmj3, *pmj4;

	// Right thumb finger
	pmj0 = m_pmc->findJoint("r_thumb0");	
	pmj1 = m_pmc->findJoint("r_thumb1");	
	pmj2 = m_pmc->findJoint("r_thumb2");	
	pmj3 = m_pmc->findJoint("r_thumb3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexRThumb;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexRThumb, 190);
	m_mFlexion.insert(std::pair<std::string, Flexion>("RightThumb", flexRThumb));

	// Right index finger
	pmj0 = m_pmc->findJoint("r_index0");	
	pmj1 = m_pmc->findJoint("r_index1");	
	pmj2 = m_pmc->findJoint("r_index2");	
	pmj3 = m_pmc->findJoint("r_index3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexRIndex;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexRIndex, 250);
	m_mFlexion.insert(std::pair<std::string, Flexion>("RightIndex", flexRIndex));

	// Right middle finger
	pmj0 = m_pmc->findJoint("r_middle0");	
	pmj1 = m_pmc->findJoint("r_middle1");	
	pmj2 = m_pmc->findJoint("r_middle2");	
	pmj3 = m_pmc->findJoint("r_middle3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexRMiddle;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexRMiddle, 250);
	m_mFlexion.insert(std::pair<std::string, Flexion>("RightMiddle", flexRMiddle));

	// Right ring finger
	pmj0 = m_pmc->findJoint("r_ring0");	
	pmj1 = m_pmc->findJoint("r_ring1");	
	pmj2 = m_pmc->findJoint("r_ring2");	
	pmj3 = m_pmc->findJoint("r_ring3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexRRing;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexRRing, 250);
	m_mFlexion.insert(std::pair<std::string, Flexion>("RightRing", flexRRing));

	// Right little finger
	pmj0 = m_pmc->findJoint("r_pinky0");	
	pmj1 = m_pmc->findJoint("r_pinky1");	
	pmj2 = m_pmc->findJoint("r_pinky2");	
	pmj3 = m_pmc->findJoint("r_pinky3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexRLittle;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexRLittle, 250);
	m_mFlexion.insert(std::pair<std::string, Flexion>("RightLittle", flexRLittle));

	// Left thumb finger
	pmj0 = m_pmc->findJoint("l_thumb0");	
	pmj1 = m_pmc->findJoint("l_thumb1");	
	pmj2 = m_pmc->findJoint("l_thumb2");	
	pmj3 = m_pmc->findJoint("l_thumb3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexLThumb;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexLThumb, 190);
	m_mFlexion.insert(std::pair<std::string, Flexion>("LeftThumb", flexLThumb));

	// Left index finger
	pmj0 = m_pmc->findJoint("l_index0");	
	pmj1 = m_pmc->findJoint("l_index1");	
	pmj2 = m_pmc->findJoint("l_index2");	
	pmj3 = m_pmc->findJoint("l_index3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexLIndex;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexLIndex, 250);
	m_mFlexion.insert(std::pair<std::string, Flexion>("LeftIndex", flexLIndex));

	// Left middle finger
	pmj0 = m_pmc->findJoint("l_middle0");	
	pmj1 = m_pmc->findJoint("l_middle1");	
	pmj2 = m_pmc->findJoint("l_middle2");	
	pmj3 = m_pmc->findJoint("l_middle3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexLMiddle;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexLMiddle, 250);
	m_mFlexion.insert(std::pair<std::string, Flexion>("LeftMiddle", flexLMiddle));

	// Left ring finger
	pmj0 = m_pmc->findJoint("l_ring0");	
	pmj1 = m_pmc->findJoint("l_ring1");	
	pmj2 = m_pmc->findJoint("l_ring2");	
	pmj3 = m_pmc->findJoint("l_ring3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexLRing;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexLRing, 250);
	m_mFlexion.insert(std::pair<std::string, Flexion>("LeftRing", flexLRing));

	// Left little finger
	pmj0 = m_pmc->findJoint("l_pinky0");	
	pmj1 = m_pmc->findJoint("l_pinky1");	
	pmj2 = m_pmc->findJoint("l_pinky2");	
	pmj3 = m_pmc->findJoint("l_pinky3");	
	pmj4 = m_pmc->getChild(pmj3, 0);
	Flexion flexLLittle;
	calFingerFlex(pmj0, pmj1, pmj2, pmj3, pmj4, flexLLittle, 250);
	m_mFlexion.insert(std::pair<std::string, Flexion>("LeftLittle", flexLLittle));

	//std::ofstream out("Flexion.txt");
	//out << "Flexion:" << std::endl;
	//for (std::map<std::string, Flexion>::iterator it=m_mFlexion.begin(); it!= m_mFlexion.end(); it++)
	//{
	//	out << it->first << std::endl;
	//	for (int i=0; i<it->second.vFlex.size(); i++)
	//	{
	//		out << it->second.vFlex.at(i) << " ";
	//	}
	//	out << "\nAverage: " << it->second.dFlexMean << "\nVariance: " << it->second.dFlexVar  << "\nMargin: " << it->second.dFlexMar << "\nTurningPoints: " << it->second.nFlexTurn << std::endl;
	//}
	//out.close();
}

void PriFeatureClip::calFingerFlex(MotionJoint *pmj0, MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, MotionJoint *pmj4, Flexion& flex, double dMaxFlexAngle/*=250*/)
{
	double d, d1, d2, d3;
	double dmin=1.0, dmax=0.0;
	for (int i=0; i<m_nFrameCount; i++)
	{
		d1 = Math::PI-calAngle(pmj0->getAbsolutePosition(i), pmj1->getAbsolutePosition(i), pmj2->getAbsolutePosition(i));
		d2 = Math::PI-calAngle(pmj1->getAbsolutePosition(i), pmj2->getAbsolutePosition(i), pmj3->getAbsolutePosition(i));
		d3 = Math::PI-calAngle(pmj2->getAbsolutePosition(i), pmj3->getAbsolutePosition(i), pmj4->getAbsolutePosition(i));
		d = (d1+d2+d3)*Math::RAD_TO_DEG / dMaxFlexAngle;
		if (d>1.0)
			d = 1.0;
		flex.vFlex.push_back(d);
		flex.dFlexMean += d;
		if (d<dmin)
			dmin = d;
		if (d>dmax)
			dmax = d;
	}
	flex.dFlexMean /= m_nFrameCount;
	for (int i=0; i<m_nFrameCount; i++)
	{
		flex.dFlexVar += (flex.vFlex.at(i)-flex.dFlexMean)*(flex.vFlex.at(i)-flex.dFlexMean);
	}
	flex.dFlexVar /= m_nFrameCount;
	flex.dFlexMar = dmax-dmin;
	flex.nFlexTurn = calInflexion(flex.vFlex);
}

void PriFeatureClip::calAbduction()
{
	MotionJoint* pmj1, *pmj2, *pmj3, *pmj4;

	// Abduction between right thumb and right index fingers.
	pmj1 = m_pmc->findJoint("r_thumb2");	
	pmj2 = m_pmc->findJoint("r_thumb3");	
	pmj3 = m_pmc->findJoint("r_index1");	
	pmj4 = m_pmc->findJoint("r_index2");
	Abduction abdRThumbIndex;
	calFingerAbd(pmj1, pmj2, pmj3, pmj4, abdRThumbIndex, 90);	// 90 is the max angle between thumb and index fingers.
	m_mAbduction.insert(std::pair<std::string, Abduction>("RightThumbIndex", abdRThumbIndex));

	// Abduction between right index and right middle fingers.
	pmj1 = pmj3;
	pmj2 = pmj4;
	pmj3 = m_pmc->findJoint("r_middle1");	
	pmj4 = m_pmc->findJoint("r_middle2");
	Abduction abdRIndexMiddle;
	calFingerAbd(pmj1, pmj2, pmj3, pmj4, abdRIndexMiddle, 40);	// 40 is the max angle between index and middle fingers.
	m_mAbduction.insert(std::pair<std::string, Abduction>("RightIndexMiddle", abdRIndexMiddle));

	// Abduction between right middle and right ring fingers.
	pmj1 = pmj3;
	pmj2 = pmj4;
	pmj3 = m_pmc->findJoint("r_ring1");	
	pmj4 = m_pmc->findJoint("r_ring2");
	Abduction abdRMiddleRing;
	calFingerAbd(pmj1, pmj2, pmj3, pmj4, abdRMiddleRing, 40);
	m_mAbduction.insert(std::pair<std::string, Abduction>("RightMiddleRing", abdRMiddleRing));

	// Abduction between right ring and right little fingers.
	pmj1 = pmj3;
	pmj2 = pmj4;
	pmj3 = m_pmc->findJoint("r_pinky1");	
	pmj4 = m_pmc->findJoint("r_pinky2");
	Abduction abdRRingLittle;
	calFingerAbd(pmj1, pmj2, pmj3, pmj4, abdRRingLittle, 40);
	m_mAbduction.insert(std::pair<std::string, Abduction>("RightRingLittle", abdRRingLittle));

	// Abduction between left thumb and left index fingers.
	pmj1 = m_pmc->findJoint("l_thumb2");	
	pmj2 = m_pmc->findJoint("l_thumb3");	
	pmj3 = m_pmc->findJoint("l_index1");	
	pmj4 = m_pmc->findJoint("l_index2");
	Abduction abdLThumbIndex;
	calFingerAbd(pmj1, pmj2, pmj3, pmj4, abdLThumbIndex, 90);
	m_mAbduction.insert(std::pair<std::string, Abduction>("LeftThumbIndex", abdLThumbIndex));

	// Abduction between left index and left middle fingers.
	pmj1 = pmj3;
	pmj2 = pmj4;
	pmj3 = m_pmc->findJoint("l_middle1");	
	pmj4 = m_pmc->findJoint("l_middle2");
	Abduction abdLIndexMiddle;
	calFingerAbd(pmj1, pmj2, pmj3, pmj4, abdLIndexMiddle, 40);
	m_mAbduction.insert(std::pair<std::string, Abduction>("LeftIndexMiddle", abdLIndexMiddle));

	// Abduction between left index and left middle fingers.
	pmj1 = pmj3;
	pmj2 = pmj4;
	pmj3 = m_pmc->findJoint("l_ring1");	
	pmj4 = m_pmc->findJoint("l_ring2");
	Abduction abdLMiddleRing;
	calFingerAbd(pmj1, pmj2, pmj3, pmj4, abdLMiddleRing, 40);
	m_mAbduction.insert(std::pair<std::string, Abduction>("LeftMiddleRing", abdLMiddleRing));

	// Abduction between left index and left middle fingers.
	pmj1 = pmj3;
	pmj2 = pmj4;
	pmj3 = m_pmc->findJoint("l_pinky1");	
	pmj4 = m_pmc->findJoint("l_pinky2");
	Abduction abdLRingLittle;
	calFingerAbd(pmj1, pmj2, pmj3, pmj4, abdLRingLittle, 40);
	m_mAbduction.insert(std::pair<std::string, Abduction>("LeftRingLittle", abdLRingLittle));

	//std::ofstream out("Abduction.txt");
	//out << "Abduction:" << std::endl;
	//for (std::map<std::string, Abduction>::iterator it=m_mAbduction.begin(); it!= m_mAbduction.end(); it++)
	//{
	//	out << it->first << std::endl;
	//	for (int i=0; i<it->second.vAbd.size(); i++)
	//	{
	//		out << it->second.vAbd.at(i) << " ";
	//	}
	//	out << "\nAverage: " << it->second.dAbdMean << "\nVariance: " << it->second.dAbdVar << "\nMargin: " << it->second.dAbdMar << "\nTurningPoints: " << it->second.nAbdTurn << std::endl;
	//}
	//out.close();
}

void PriFeatureClip::calFingerAbd(MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, MotionJoint *pmj4, Abduction& abd, double dMaxAbdAngle/*=40*/)
{
	double d;
	double dmin=1.0, dmax=0.0;
	Vector3 v0=Vector3::ZERO, v1, v2;
	for (int i=0; i<m_nFrameCount; i++)
	{
		v1 = pmj2->getAbsolutePosition(i) - pmj1->getAbsolutePosition(i);
		v2 = pmj4->getAbsolutePosition(i) - pmj3->getAbsolutePosition(i);
		d = calAngle(v1, v0, v2);
		d = d * Math::RAD_TO_DEG / dMaxAbdAngle;	
		if (d>1.0)
			d = 1.0;
		abd.vAbd.push_back(d);
		abd.dAbdMean += d;
		if (d<dmin)
			dmin = d;
		if (d>dmax)
			dmax = d;
	}
	abd.dAbdMean /= m_nFrameCount;
	for (int i=0; i<m_nFrameCount; i++)
	{
		abd.dAbdVar += (abd.vAbd.at(i)-abd.dAbdMean)*(abd.vAbd.at(i)-abd.dAbdMean);
	}
	abd.dAbdVar /= m_nFrameCount;
	abd.dAbdMar = dmax-dmin;
	abd.nAbdTurn = calInflexion(abd.vAbd);
}

void PriFeatureClip::calPosition()
{
	MotionJoint *pmj1, *pmj2;
	pmj1 = m_pmc->findJoint("r_wrist");
	pmj2 = m_pmc->findJoint("root");
	Position posRWristRoot;
	calDistance(pmj1, pmj2, posRWristRoot);
	m_mPosition.insert(std::pair<std::string, Position>("RightWristRoot", posRWristRoot));

	//pmj2 = m_pmc->findJoint("r_shoulder");
	//Position posRWristRShoulder;
	//calDistance(pmj1, pmj2, posRWristRShoulder);
	//m_mPosition.insert(std::pair<std::string, Position>("RightWristRightShoulder", posRWristRShoulder));

	//pmj2 = m_pmc->findJoint("l_shoulder");
	//Position posRWristLShoulder;
	//calDistance(pmj1, pmj2, posRWristLShoulder);
	//m_mPosition.insert(std::pair<std::string, Position>("RightWristLeftShoulder", posRWristLShoulder));

	pmj2 = m_pmc->findJoint("skullbase");
	Position posRWristHead;
	calDistance(pmj1, pmj2, posRWristHead);
	m_mPosition.insert(std::pair<std::string, Position>("RightWristHead", posRWristHead));

	pmj2 = m_pmc->findJoint("l_wrist");
	Position posRWristLWrist;
	calDistance(pmj1, pmj2, posRWristLWrist);
	m_mPosition.insert(std::pair<std::string, Position>("RightWristLeftWrist", posRWristLWrist));

	pmj1 = m_pmc->findJoint("l_wrist");
	pmj2 = m_pmc->findJoint("root");
	Position posLWristRoot;
	calDistance(pmj1, pmj2, posLWristRoot);
	m_mPosition.insert(std::pair<std::string, Position>("LeftWristRoot", posLWristRoot));

	//pmj2 = m_pmc->findJoint("r_shoulder");
	//Position posLWristRShoulder;
	//calDistance(pmj1, pmj2, posLWristRShoulder);
	//m_mPosition.insert(std::pair<std::string, Position>("LeftWristRightShoulder", posLWristRShoulder));

	//pmj2 = m_pmc->findJoint("l_shoulder");
	//Position posLWristLShoulder;
	//calDistance(pmj1, pmj2, posLWristLShoulder);
	//m_mPosition.insert(std::pair<std::string, Position>("LeftWristLeftShoulder", posLWristLShoulder));

	pmj2 = m_pmc->findJoint("skullbase");
	Position posLWristHead;
	calDistance(pmj1, pmj2, posLWristHead);
	m_mPosition.insert(std::pair<std::string, Position>("LeftWristHead", posLWristHead));

	//std::ofstream out("Position.txt");
	//out << "Position:" << std::endl;
	//for (std::map<std::string, Position>::iterator it=m_mPosition.begin(); it!= m_mPosition.end(); it++)
	//{
	//	out << it->first << std::endl;
	//	for (int i=0; i<it->second.vDist.size(); i++)
	//	{
	//		out << it->second.vDist.at(i) << " ";
	//	}
	//	out << "\nAverage: " << it->second.dDistMean << "\nVariance: " << it->second.dDistVar << "\nMargin: " << it->second.dDistMar << "\nTurningPoints: " << it->second.nDistTurn << std::endl;
	//}
	//out.close();
}

void PriFeatureClip::calDistance(MotionJoint *pmj1, MotionJoint *pmj2, Position& pos, double dScale/*=1.0*/)
{
	double d;
	double dmin=1.0e10, dmax=-1.0;
	Vector3 v, v1, v2;
	for (int i=0; i<m_nFrameCount; i++)
	{
		v1 = pmj1->getAbsolutePosition(i);
		v2 = pmj2->getAbsolutePosition(i);
		v = v1-v2;
		d = v.getNorm();
		d *= dScale;
		pos.vPos.push_back(v);
		pos.vDist.push_back(d);
		pos.dDistMean += d;
		if (d<dmin)
			dmin = d;
		if (d>dmax)
			dmax = d;
	}
	pos.dDistMean /= m_nFrameCount;
	for (int i=0; i<m_nFrameCount; i++)
	{
		pos.dDistVar += (pos.vDist.at(i)-pos.dDistMean)*(pos.vDist.at(i)-pos.dDistMean);
	}
	pos.dDistVar /= m_nFrameCount;
	pos.dDistMar = dmax-dmin;
	pos.nDistTurn = calInflexion(pos.vDist);
}

void PriFeatureClip::calOrientation()
{
	MotionJoint *pmj1, *pmj2, *pmj3, *pmjroot;
	pmjroot = m_pmc->findJoint("root");

	pmj1 = m_pmc->findJoint("r_wrist");
	pmj2 = m_pmc->findJoint("r_index1");
	pmj3 = m_pmc->findJoint("r_ring1");
	Orientation oriRPalm;
	calPalmOri(pmj1, pmj2, pmj3, oriRPalm);
	m_mOrientation.insert(std::pair<std::string, Orientation>("RightPalm",oriRPalm));

	pmj1 = m_pmc->findJoint("l_wrist");
	pmj2 = m_pmc->findJoint("l_ring1");
	pmj3 = m_pmc->findJoint("l_index1");
	Orientation oriLPalm;
	calPalmOri(pmj1, pmj2, pmj3, oriLPalm);
	m_mOrientation.insert(std::pair<std::string, Orientation>("LeftPalm",oriLPalm));

	//std::ofstream out("Orientation.txt");
	//out << "Orientation:" << std::endl;
	//for (std::map<std::string, Orientation>::iterator it=m_mOrientation.begin(); it!= m_mOrientation.end(); it++)
	//{
	//	out << it->first << std::endl;
	//	out << "X: " << std::endl;
	//	for (int i=0; i<it->second.vOriX.size(); i++)
	//	{
	//		out << it->second.vOriX.at(i) << " ";
	//	}
	//	out << "\nAverageX: " << it->second.dXOriMean << "\nVarianceX: " << it->second.dXOriVar << "\nMarginX: " << it->second.dXOriMar << "\nTurningPointsX: " << it->second.nXOriTurn << std::endl;
	//	
	//	out << "Y: " << std::endl;
	//	for (int i=0; i<it->second.vOriY.size(); i++)
	//	{
	//		out << it->second.vOriY.at(i) << " ";
	//	}
	//	out << "\nAverageY: " << it->second.dYOriMean << "\nVarianceY: " << it->second.dYOriVar << "\nMarginY: " << it->second.dYOriMar << "\nTurningPointsY: " << it->second.nYOriTurn << std::endl;

	//	out << "Z: " << std::endl;
	//	for (int i=0; i<it->second.vOriZ.size(); i++)
	//	{
	//		out << it->second.vOriZ.at(i) << " ";
	//	}
	//	out << "\nAverageZ: " << it->second.dZOriMean << "\nVarianceZ: " << it->second.dZOriVar << "\nMarginZ: " << it->second.dZOriMar << "\nTurningPointsZ: " << it->second.nZOriTurn << std::endl;
	//}
	//out.close();
}

void PriFeatureClip::calPalmOri(MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, Orientation& ori)
{
	Quaternion q = m_pmc->getRoot()->getOrientation(0);
	Vector3 dir1 = Vector3::UNIT_Z;
	Vector3 dir2 = q * dir1;
	dir2.set_y(0.0);
	double thetaD, temp;
	temp = Dot(dir2, dir1);
	thetaD = Math::ACosD(temp);
	Vector3 vTemp = Cross(dir2, dir1);
	if(vTemp.y() < 0)
	{
		thetaD = -thetaD;
	}
	Quaternion qAdjust;
	qAdjust = qAdjust.fromAngleAxis(thetaD, 0.0, 1.0, 0.0);

	double dx, dy, dz, thetax, thetay, thetaz;
	double dminx=1.0e10, dmaxx=-1.0, dminy=1.0e10, dmaxy=-1.0, dminz=1.0e10, dmaxz=-1.0;
	Vector3 vnormal, v1, v2, v3, axisx=Vector3::UNIT_X, axisy=Vector3::UNIT_Y, axisz=Vector3::UNIT_Z;
	for (int i=0; i<m_nFrameCount; i++)
	{
		v1 = pmj1->getAbsolutePosition(i);
		v2 = pmj2->getAbsolutePosition(i);
		v3 = pmj3->getAbsolutePosition(i);
		vnormal = calNormal(v1, v2, v3);
		vnormal = qAdjust * vnormal;
		dx = Dot(vnormal, axisx);
		thetax = Math::ACosD(dx);
		ori.vOriX.push_back(thetax);
		ori.dXOriMean += thetax;
		if (thetax<dminx)
			dminx = thetax;
		if (thetax>dmaxx)
			dmaxx = thetax;

		dy = Dot(vnormal, axisy);
		thetay = Math::ACosD(dy);
		ori.vOriY.push_back(thetay);
		ori.dYOriMean += thetay;
		if (thetay<dminy)
			dminy = thetay;
		if (thetay>dmaxy)
			dmaxy = thetay;

		dz = Dot(vnormal, axisz);
		thetaz = Math::ACosD(dz);
		ori.vOriZ.push_back(thetaz);
		ori.dZOriMean += thetaz;
		if (thetaz<dminz)
			dminz = thetaz;
		if (thetaz>dmaxz)
			dmaxz = thetaz;
	}
	ori.dXOriMean /= m_nFrameCount;
	for (int i=0; i<m_nFrameCount; i++)
	{
		ori.dXOriVar += (ori.vOriX.at(i)-ori.dXOriMean)*(ori.vOriX.at(i)-ori.dXOriMean);
	}
	ori.dXOriVar /= m_nFrameCount;
	ori.dXOriMar = dmaxx-dminx;
	ori.nXOriTurn = calInflexion(ori.vOriX);

	ori.dYOriMean /= m_nFrameCount;
	for (int i=0; i<m_nFrameCount; i++)
	{
		ori.dYOriVar += (ori.vOriY.at(i)-ori.dYOriMean)*(ori.vOriY.at(i)-ori.dYOriMean);
	}
	ori.dYOriVar /= m_nFrameCount;
	ori.dYOriMar = dmaxy-dminy;
	ori.nYOriTurn = calInflexion(ori.vOriY);

	ori.dZOriMean /= m_nFrameCount;
	for (int i=0; i<m_nFrameCount; i++)
	{
		ori.dZOriVar += (ori.vOriZ.at(i)-ori.dZOriMean)*(ori.vOriZ.at(i)-ori.dZOriMean);
	}
	ori.dZOriVar /= m_nFrameCount;
	ori.dZOriMar = dmaxz-dminz;
	ori.nZOriTurn = calInflexion(ori.vOriZ);
}

Vector3 PriFeatureClip::calNormal(Vector3 v1,Vector3 v2,Vector3 v3)
{
	Vector3 vec1;
	vec1.set_x(v2.x() - v1.x());
	vec1.set_y(v2.y() - v1.y());
	vec1.set_z(v2.z() - v1.z());

	Vector3 vec2;
	vec2.set_x(v3.x()- v2.x());
	vec2.set_y(v3.y() - v2.y());
	vec2.set_z(v3.z() - v2.z());

	Vector3 vec;
	vec.set_x(vec1.y() * vec2.z() - vec1.z() * vec2.y());
	vec.set_y(vec1.z() * vec2.x() - vec1.x() * vec2.z());
	vec.set_z(vec1.x() * vec2.y() - vec1.y() * vec2.x());

	double d = sqrt (vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z());
	vec.set_values(vec.x()/d, vec.y()/d, vec.z()/d);

	return vec;
}

void PriFeatureClip::calMovement()
{
	for (std::map<std::string, Position>::iterator it=m_mPosition.begin(); it!=m_mPosition.end(); it++)
	{
		double dspeed, dacc, dminspeed=1.0e10, dmaxspeed=-1.0, dminacc=1.0e10, dmaxacc=-1.0;
		Movement mov;
		calVelAcc(it->second.vPos, mov.vVel, mov.vVectorAcc);
		calSpeedAcc(it->second.vDist, mov.vSpeed, mov.vAcc);
		for (int i=0; i<m_nFrameCount; i++)
		{
			dspeed = mov.vSpeed.at(i);
			mov.dSpeedMean += dspeed;
			if (dspeed<dminspeed)
				dminspeed = dspeed;
			if (dspeed>dmaxspeed)
				dmaxspeed = dspeed;
			dacc = mov.vAcc.at(i);
			mov.dAccMean += dacc;
			if (dacc<dminacc)
				dminacc = dacc;
			if (dacc>dmaxacc)
				dmaxacc = dacc;
		}
		mov.dSpeedMean /= m_nFrameCount;
		mov.dAccMean /= m_nFrameCount;
		for (int i=0; i<m_nFrameCount; i++)
		{
			mov.dSpeedVar += (mov.vSpeed.at(i)-mov.dSpeedMean)*(mov.vSpeed.at(i)-mov.dSpeedMean);
			mov.dAccVar += (mov.vAcc.at(i)-mov.dAccMean)*(mov.vAcc.at(i)-mov.dAccMean);
		}
		mov.dSpeedVar /= m_nFrameCount;
		mov.dAccVar /= m_nFrameCount;
		mov.dSpeedMar = dmaxspeed-dminspeed;
		mov.dAccMar = dmaxacc-dminacc;
		mov.nSpeedTurn = calInflexion(mov.vSpeed);
		mov.nAccTurn = calInflexion(mov.vAcc);

		m_mMovement.insert(std::pair<std::string, Movement>(it->first, mov));
	}

	//std::ofstream out("Movement.txt");
	//out << "Movement:" << std::endl;
	//for (std::map<std::string, Movement>::iterator it=m_mMovement.begin(); it!= m_mMovement.end(); it++)
	//{
	//	out << it->first << std::endl;
	//	out << "Speed:" << std::endl;
	//	for (int i=0; i<it->second.vSpeed.size(); i++)
	//	{
	//		out << it->second.vSpeed.at(i) << " ";
	//	}
	//	out << "\nAverage: " << it->second.dSpeedMean << "\nVariance: " << it->second.dSpeedVar << "\nMargin: " << it->second.dSpeedMar << "\nTurningPoints: " << it->second.nSpeedTurn << std::endl;
	//	
	//	out << "Acceleration:" << std::endl;
	//	for (int i=0; i<it->second.vAcc.size(); i++)
	//	{
	//		out << it->second.vAcc.at(i) << " ";
	//	}
	//	out << "\nAverage: " << it->second.dAccMean << "\nVariance: " << it->second.dAccVar << "\nMargin: " << it->second.dAccMar << "\nTurningPoints: " << it->second.nAccTurn << std::endl;
	//}
	//out.close();
}

void PriFeatureClip::calVelAcc(std::vector<Vector3> vPos, std::vector<Vector3>& vVel, std::vector<Vector3>& vAcc)
{
	int n = vPos.size();
	if (n == 0)
		return;

	double *pvalx = new double[n];
	double *pvaly = new double[n];
	double *pvalz = new double[n];
	double *t = new double[n];
	for (int i=0; i<n; i++)
	{
		pvalx[i] = vPos.at(i).x();
		pvaly[i] = vPos.at(i).y();
		pvalz[i] = vPos.at(i).z();
		t[i] = i;
	}


	// 计算各已知点的二阶导数
	double *pddvalx, *pddvaly, *pddvalz;
	pddvalx = spline_cubic_set(n, t, pvalx, 0, 0, 0, 0);		// 端点二阶导数待测试
	pddvaly = spline_cubic_set(n, t, pvaly, 0, 0, 0, 0);
	pddvalz = spline_cubic_set(n, t, pvalz, 0, 0, 0, 0);

	double dvalx=0, dvaly=0, dvalz=0;
	double ddvalx=0, ddvaly=0, ddvalz=0;

	for(int j=0; j<n; j++)
	{
		spline_cubic_val(n, t, j, pvalx, pddvalx, &dvalx, &ddvalx);
		spline_cubic_val(n, t, j, pvaly, pddvaly, &dvaly, &ddvaly);
		spline_cubic_val(n, t, j, pvalz, pddvalz, &dvalz, &ddvalz);
		vVel.push_back(Vector3(dvalx, dvaly, dvalz));
		vAcc.push_back(Vector3(pddvalx[j], pddvaly[j], pddvalz[j]));
	}

	delete []pvalx;
	delete []pvaly;
	delete []pvalz;
	delete []pddvalx;
	delete []pddvaly;
	delete []pddvalz;
	delete []t;
}

void PriFeatureClip::calSpeedAcc(std::vector<double> vDist, std::vector<double>& vSpeed, std::vector<double>& vAcc)
{
	int n = vDist.size();
	if (n == 0)
		return;

	double *pval = new double[n];
	double *t = new double[n];
	for (int i=0; i<n; i++)
	{
		pval[i] = vDist.at(i);
		t[i] = i;
	}

	// 计算各已知点的二阶导数
	double *pddval;
	pddval = spline_cubic_set(n, t, pval, 0, 0, 0, 0);		// 端点二阶导数待测试

	double dval = 0;
	double ddval = 0;

	for(int j=0; j<n; j++)
	{
		spline_cubic_val(n, t, j, pval, pddval, &dval, &ddval);
		vSpeed.push_back(dval);
		vAcc.push_back(pddval[j]);
	}

	delete []pval;
	delete []pddval;
	delete []t;
}

double PriFeatureClip::calAngle(Vector3 v1, Vector3 v2, Vector3 v3)
{
	double d1, d2, d3, angle;
	d1 = (v2.x()-v3.x())*(v2.x()-v3.x()) + (v2.y()-v3.y())*(v2.y()-v3.y()) + (v2.z()-v3.z())*(v2.z()-v3.z());
	d2 = (v1.x()-v3.x())*(v1.x()-v3.x()) + (v1.y()-v3.y())*(v1.y()-v3.y()) + (v1.z()-v3.z())*(v1.z()-v3.z());
	d3 = (v1.x()-v2.x())*(v1.x()-v2.x()) + (v1.y()-v2.y())*(v1.y()-v2.y()) + (v1.z()-v2.z())*(v1.z()-v2.z());
	angle = std::acos((d1+d3-d2)/(2*std::sqrt(d1)*std::sqrt(d3)));

	return angle;
}

int PriFeatureClip::calInflexion(std::vector<double> v, double dThreshold/*=0.1*/)
{
	if (v.size()<3)
		return 0;

	int n=0;
	double d1, d2, d3;
	d1=v.at(0);
	int nSize = v.size();
	for (int i=1; i<nSize; i++)
	{
		d2=v.at(i);
		while ((std::abs(d2-d1)<dThreshold))
		{
			i++;
			if (i >= nSize)
				return n;
			d2=v.at(i);
		}
		int j=i+1;
		if (j >= nSize)
			return n;
		d3=v.at(j);
		while ((std::abs(d3-d2)<dThreshold))
		{
			j++;
			if (j >= nSize)
				return n;
			d3=v.at(j);
		}
		if ((d2-d1)*(d3-d2) < 0)
			n++;
		d1=d2;
		i=j-1;
	}

	return n;
}

void PriFeatureClip::resample(int nFrameCount)
{
	m_nFrameCount = nFrameCount;
}