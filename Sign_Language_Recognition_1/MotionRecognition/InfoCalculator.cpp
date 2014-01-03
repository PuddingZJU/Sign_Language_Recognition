/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2011/01/27                                                  */
/************************************************************************/

#include "InfoCalculator.h"

using namespace Swift;

InfoCalcualtor::InfoCalcualtor()
{

}

InfoCalcualtor::~InfoCalcualtor()
{

}

void InfoCalcualtor::calFingerFlex(MotionJoint *pmj0, MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, MotionJoint *pmj4, int nStartFn, int nEndFn, std::vector<double> &vFlex, double dMaxFlexAngle/* =250 */)
{
	double d, d1, d2, d3;
	double dmin=1.0, dmax=0.0;
	for (int i=nStartFn; i<=nEndFn; i++)
	{
		d1 = Math::PI-calAngle(pmj0->getAbsolutePosition(i), pmj1->getAbsolutePosition(i), pmj2->getAbsolutePosition(i));
		d2 = Math::PI-calAngle(pmj1->getAbsolutePosition(i), pmj2->getAbsolutePosition(i), pmj3->getAbsolutePosition(i));
		d3 = Math::PI-calAngle(pmj2->getAbsolutePosition(i), pmj3->getAbsolutePosition(i), pmj4->getAbsolutePosition(i));
		d = (d1+d2+d3)*Math::RAD_TO_DEG / dMaxFlexAngle;
		if (d>1.0)
			d = 1.0;
		vFlex.push_back(d);
	}
}

double InfoCalcualtor::calAngle(Vector3 v1, Vector3 v2, Vector3 v3)
{
	double d1, d2, d3, angle;
	d1 = (v2.x()-v3.x())*(v2.x()-v3.x()) + (v2.y()-v3.y())*(v2.y()-v3.y()) + (v2.z()-v3.z())*(v2.z()-v3.z());
	d2 = (v1.x()-v3.x())*(v1.x()-v3.x()) + (v1.y()-v3.y())*(v1.y()-v3.y()) + (v1.z()-v3.z())*(v1.z()-v3.z());
	d3 = (v1.x()-v2.x())*(v1.x()-v2.x()) + (v1.y()-v2.y())*(v1.y()-v2.y()) + (v1.z()-v2.z())*(v1.z()-v2.z());
	angle = std::acos((d1+d3-d2)/(2*std::sqrt(d1)*std::sqrt(d3)));

	return angle;
}

void InfoCalcualtor::calDist(MotionJoint *pmj1, MotionJoint *pmj2, int nStartFn, int nEndFn, std::vector<double> &vDist)
{
	Vector3 v1, v2;
	double d;
	for (int i=nStartFn; i<=nEndFn; i++)
	{
		v1 = pmj1->getAbsolutePosition(i);
		v2 = pmj2->getAbsolutePosition(i);
		d = Distance(v1, v2);
		vDist.push_back(d);
	}
}

void InfoCalcualtor::calPalmOri(MotionClip* pmc, MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, int nStartFn, int nEndFn, std::vector<double> &vOriX, std::vector<double> &vOriY, std::vector<double> &vOriZ)
{
	Quaternion q = pmc->getRoot()->getOrientation(0);
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
	Vector3 vnormal, v1, v2, v3, axisx=Vector3::UNIT_X, axisy=Vector3::UNIT_Y, axisz=Vector3::UNIT_Z;
	for (int i=nStartFn; i<=nEndFn; i++)
	{
		v1 = pmj1->getAbsolutePosition(i);
		v2 = pmj2->getAbsolutePosition(i);
		v3 = pmj3->getAbsolutePosition(i);
		vnormal = calNormal(v1, v2, v3);
		vnormal = qAdjust * vnormal;

		dx = Dot(vnormal, axisx);
		thetax = Math::ACosD(dx);
		vOriX.push_back(thetax);

		dy = Dot(vnormal, axisy);
		thetay = Math::ACosD(dy);
		vOriY.push_back(thetay);

		dz = Dot(vnormal, axisz);
		thetaz = Math::ACosD(dz);
		vOriZ.push_back(thetaz);
	}
}

Vector3 InfoCalcualtor::calNormal(Vector3 v1,Vector3 v2,Vector3 v3)
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