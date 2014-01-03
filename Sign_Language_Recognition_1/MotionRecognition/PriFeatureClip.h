/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/10/26                                                  */
/************************************************************************/

#pragma once

#ifndef _PRIFEATURECLIP_H_
#define _PRIFEATURECLIP_H_

#include <vector>
#include <map>
#include "../MotionData/MotionClip.h"

namespace Swift
{
	/// The flexion of one finger in a sign segment.
	class Flexion 
	{
	public:
		Flexion() : dFlexMean(0.0), dFlexVar(0.0), dFlexMar(0.0), nFlexTurn(0){};

		/// All the flexion values.
		std::vector<double> vFlex;
		/// The flexion mean.
		double dFlexMean;
		/// The flexion variance.
		double dFlexVar;
		/// The the flexion margin.
		double dFlexMar;
		/// The number of turning points.
		int nFlexTurn;
	};

	/// The abduction between fingers in a sign segment.
	class Abduction 
	{
	public:
		Abduction() : dAbdMean(0.0), dAbdVar(0.0), dAbdMar(0.0), nAbdTurn(0){};

		/// All the abduction values.
		std::vector<double> vAbd;
		/// The abduction mean.
		double dAbdMean;
		/// The abduction variance.
		double dAbdVar;
		/// The abduction margin.
		double dAbdMar;
		/// The number of turning points.
		int nAbdTurn;
	};

	/// The position of one joint in a sign segment.
	class Position 
	{
	public:
		Position() : dDistMean(0.0), dDistVar(0.0), dDistMar(0.0), nDistTurn(0){};

		/// All the relative position values.
		std::vector<Vector3> vPos;
		/// All the distance values.
		std::vector<double> vDist;
		/// The distance mean.
		double dDistMean;
		/// The distance variance.
		double dDistVar;
		/// The distance margin.
		double dDistMar;
		/// The number of turning points.
		int nDistTurn;
	};

	/// The orientation of one hand in a sign segment. The orientation is measured as the angle between the normal of palm and the axes of the world coordinate.
	class Orientation 
	{
	public:
		Orientation() : dXOriMean(0.0), dYOriMean(0.0), dZOriMean(0.0), dXOriVar(0.0), dYOriVar(0.0), dZOriVar(0.0), dXOriMar(0.0), dYOriMar(0.0), dZOriMar(0.0), nXOriTurn(0), nYOriTurn(0), nZOriTurn(0){};

		/// The angle between the normal of the palm and the X-axis of the world coordinate. 
		std::vector<double> vOriX;
		/// The angle between the normal of the palm and the Y-axis of the world coordinate. 
		std::vector<double> vOriY;
		/// The angle between the normal of the palm and the Z-axis of the world coordinate. 
		std::vector<double> vOriZ;
		/// The angle mean between the normal of the palm and the X-axis of the world coordinate.
		double dXOriMean;
		/// The angle mean between the normal of the palm and the Y-axis of the world coordinate.
		double dYOriMean;
		/// The angle mean between the normal of the palm and the Z-axis of the world coordinate.
		double dZOriMean;
		/// The angle variance between the normal of the palm and the X-axis of the world coordinate.
		double dXOriVar;
		/// The angle variance between the normal of the palm and the Y-axis of the world coordinate.
		double dYOriVar;
		/// The angle variance between the normal of the palm and the Z-axis of the world coordinate.
		double dZOriVar;
		/// The angle margin between the normal of the palm and the X-axis of the world coordinate.
		double dXOriMar;
		/// The angle margin between the normal of the palm and the Y-axis of the world coordinate.
		double dYOriMar;
		/// The angle margin between the normal of the palm and the Z-axis of the world coordinate.
		double dZOriMar;
		/// The number of turning points in the angle curve.
		int nXOriTurn;
		int nYOriTurn;
		int nZOriTurn;
	};

	/// The motion information of one joint in a sign segment.
	class Movement 
	{
	public:
		Movement() : dSpeedMean(0.0), dSpeedVar(0.0), dSpeedMar(0.0), nSpeedTurn(0), dAccMean(0.0), dAccVar(0.0), dAccMar(0.0), nAccTurn(0){};

		/// All the relative velocity values.
		std::vector<Vector3> vVel;
		/// All the relative speed values.
		std::vector<double> vSpeed;
		/// The speed mean.
		double dSpeedMean;
		/// The speed variance.
		double dSpeedVar;
		/// The speed margin.
		double dSpeedMar;
		/// The number of turning points.
		int nSpeedTurn;
		/// All the relative acceleration values (vector).
		std::vector<Vector3> vVectorAcc;
		/// All the relative acceleration values (scalar).
		std::vector<double> vAcc;
		/// The acceleration mean.
		double dAccMean;
		/// The acceleration variance.
		double dAccVar;
		/// The acceleration margin.
		double dAccMar;
		/// The number of turning points.
		int nAccTurn;
	};

	/// The primal feature calculated from each motion clip, which will be used for training and recognition.
	class PriFeatureClip
	{
	public:
		/// Constructor.
		/**
		* @param pmc The associated motion clip.
		* Note that a PriFeatureClip object is associated to one and only one MotionClip object.
		*/
		PriFeatureClip(MotionClip* pmc=NULL);
		/// Destructor.
		~PriFeatureClip();

	public:
		/// Calculate the information from motion clip.
		void calFromMC();
		/// Resample the data.
		void resample(int nFrameCount);
		/// Calculate the flexion information from motion clip.
		void calFlexion();
		/// Calculate the abduction information from motion clip.
		void calAbduction();
		/// Calculate the position information from motion clip.
		void calPosition();
		/// Calculate the orientation information from motion clip.
		void calOrientation();
		/// Calculate the movementz information from motion clip.
		void calMovement();

	private:
		/// Calculate the flex angle of one finger.
		/** 
		* @param pmj0-4 The five joints of one finger.
		* @param flex The reference to the calculated result.
		* @param dMaxFlexAngle The max flex angle of a finger.
		*/
		void calFingerFlex(MotionJoint *pmj0, MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, MotionJoint *pmj4, Flexion& flex, double dMaxFlexAngle=250);
		/// Calculate the abduction angle between two fingers.
		/** 
		* @param pmj1-2 The two joints of one finger.
		* @param pmj3-4 The two joints of the other finger.
		* @param abd The reference to the calculated result.
		* @param dMaxAbdAngle The max abduction angle between two fingers.
		*/
		void calFingerAbd(MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, MotionJoint *pmj4, Abduction& abd, double dMaxAbdAngle=40);
		/// Calculate the distance between two joints.
		/** 
		* @param pmj1-2 The involved two joints.
		* @param pos The reference to the calculated result.
		* @param dScale The scale factor.
		*/
		void calDistance(MotionJoint *pmj1, MotionJoint *pmj2, Position& pos, double dScale=1.0);
		/// Calculate the orientation of two palms.
		/** 
		* @param pmj1-3 The three joints used to calculate the normal of palm.
		* @param ori The reference to the calculated result.
		*/
		void calPalmOri(MotionJoint *pmj1, MotionJoint *pmj2, MotionJoint *pmj3, Orientation& ori);
		/// Calculate the velocity and acceleration vectors from the position vectors.
		/** 
		* @param vPos The position vector.
		* @param vVel The calculated velocity vector.
		* @param vAcc The calculated acceleration vector.
		*/
		void calVelAcc(std::vector<Vector3> vPos, std::vector<Vector3>& vVel, std::vector<Vector3>& vAcc);
		/// Calculate the speed and acceleration vectors from the position vectors.
		/** 
		* @param vPos The distance vector.
		* @param vVel The calculated speed vector.
		* @param vAcc The calculated acceleration vector.
		*/
		void calSpeedAcc(std::vector<double> vDist, std::vector<double>& vSpeed, std::vector<double>& vAcc);

		/// Calculate the normal vector from three points.
		Vector3 calNormal(Vector3 v1,Vector3 v2,Vector3 v3);
		/// Calculate the angle between two bones.
		double calAngle(Vector3 v1, Vector3 v2, Vector3 v3);
		/// Calculate the number of inflexions in an array of data.
		int calInflexion(std::vector<double> v, double dThreshold=0.1);

	public:
		/// The associated motion clip.
		MotionClip* m_pmc;
		/// The frame count in the clip. . Note that the value could be different from the frame count in motion clip because the data can be resampled.
		int m_nFrameCount;
		/// The flexion information in a sign segment.
		std::map<std::string, Flexion> m_mFlexion;
		/// The abduction information in a sign segment.
		std::map<std::string, Abduction> m_mAbduction;
		/// The position information in a sign segment.
		std::map<std::string, Position> m_mPosition;
		/// The orientation information in a sign segment.
		std::map<std::string, Orientation> m_mOrientation;
		/// The movement information in a sign segment.
		std::map<std::string, Movement> m_mMovement;
	};
}

#endif	// _PRIFEATURECLIP_H_