/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2011/01/27                                                  */
/************************************************************************/

#pragma once

#ifndef _INFOCALCULATOR_H_
#define _INFOCALCULATOR_H_

#include "../MotionData/MotionClip.h"

namespace Swift
{
	class InfoCalcualtor
	{
	public:
		InfoCalcualtor();
		~InfoCalcualtor();


		/// Calculate the flex angle of one finger.
		/** 
		* @param pmj0-4 The five joints of one finger.
		* @param nStartFn The start frame of the motion segment.
		* @param nEndFn The end frame of the motion segment.
		* @param vFlex The calculated flexion.
		* @param dMaxFlexAngle The max flex angle of a finger.
		*/
		void calFingerFlex(MotionJoint *pmj0, MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, MotionJoint *pmj4, int nStartFn, int nEndFn, std::vector<double> &vFlex, double dMaxFlexAngle=250);

		void calDist(MotionJoint *pmj1, MotionJoint *pmj2, int nStartFn, int nEndFn, std::vector<double> &vDist);

		void calPalmOri(MotionClip* pmc, MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, int nStartFn, int nEndFn, std::vector<double> &vOriX, std::vector<double> &vOriY, std::vector<double> &vOriZ);

		/// Calculate the normal vector from three points.
		Vector3 calNormal(Vector3 v1,Vector3 v2,Vector3 v3);
		/// Calculate the angle between two bones.
		double calAngle(Vector3 v1, Vector3 v2, Vector3 v3);
	};
}

#endif	// _INFOCALCULATOR_H_