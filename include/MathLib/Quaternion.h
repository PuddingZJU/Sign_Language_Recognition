#ifndef _QUATERNION_H_INCLUDED_
#define _QUATERNION_H_INCLUDED_

#include "Mathematics.h"
#include "Euler.h"
#include "Matrix3.h"
#include "Vector3.h"
#include "Vector4.h"

#include <iostream> // for debugging

class Quaternion
{
private:
	double x, y, z, w;
public:
	static const Quaternion ZERO;
	static const Quaternion IDENTITY;
	static const Quaternion QNAN;
	static const Quaternion SNAN;

public:
	// Constructors
	Quaternion()
		:x(0), y(0), z(0), w(0)
	{
	}

	Quaternion(const double* arrayData)
		:x(arrayData[0]), y(arrayData[1]), z(arrayData[2]), w(arrayData[3])
	{
	}

	Quaternion(double _x, double _y, double _z, double _w)
		:x(_x), y(_y), z(_z), w(_w)
	{
	}

	//getter and setter
	double get_x() const
	{
		return x;
	}

	double get_y() const
	{
		return y;
	}

	double get_z() const
	{
		return z;
	}

	double get_w() const
	{
		return w;
	}

	void set_x(double value)
	{
		x = value;
	}

	void set_y(double value)
	{
		y = value;
	}

	void set_z(double value)
	{
		z = value;
	}

	void set_w(double value)
	{
		w = value;
	}

	void set_values(double _x, double _y, double _z, double _w)
	{
		x = _x; y = _y; z = _z; w = _w;
	}

	explicit Quaternion(const Matrix3& rotMat)
	{
		fromRotateMatrix(rotMat);
	}

	void fromRotateMatrix(const Matrix3& rotMat)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".

		double dTrace = rotMat(0,0)+rotMat(1,1)+rotMat(2,2);
		double dRoot = 0;

		if ( dTrace > 0.0 )
		{
			// |w| > 1/2, may as well choose w > 1/2
			dRoot = sqrt(dTrace + 1.0);  // 2w
			w = 0.5 * dRoot;
			dRoot = 0.5 / dRoot;  // 1/(4w)
			x = (rotMat(2,1) - rotMat(1,2)) * dRoot;
			y = (rotMat(0,2) - rotMat(2,0)) * dRoot;
			z = (rotMat(1,0) - rotMat(0,1)) * dRoot;
		}
		else
		{
			// |w| <= 1/2
			static size_t s_iNext[3] = { 1, 2, 0 };
			size_t i = 0;
			if ( rotMat(1,1) > rotMat(0,0) )
				i = 1;
			if ( rotMat(2,2) > rotMat(i,i) )
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			dRoot = sqrt( rotMat(i,i) - rotMat(j,j) - rotMat(k,k) + 1.0 );

			double* apkQuat[3] = { &x, &y, &z };

			*apkQuat[i] = 0.5 * dRoot;

			dRoot = 0.5 / dRoot;

			w = ( rotMat(k,j) - rotMat(j,k) ) * dRoot;

			*apkQuat[j] = ( rotMat(j,i) + rotMat(i,j) ) * dRoot;
			*apkQuat[k] = ( rotMat(k,i) + rotMat(i,k) ) * dRoot;
		}
	}

	void toRotationMatrix(Matrix3& rotMat) const
	{
		double fTx  = 2.0 * x;
		double fTy  = 2.0 * y;
		double fTz  = 2.0 * z;
		double fTwx = fTx * w;
		double fTwy = fTy * w;
		double fTwz = fTz * w;
		double fTxx = fTx * x;
		double fTxy = fTy * x;
		double fTxz = fTz * x;
		double fTyy = fTy * y;
		double fTyz = fTz * y;
		double fTzz = fTz * z;

		rotMat(0,0) = 1.0 - (fTyy + fTzz);
		rotMat(0,1) = fTxy - fTwz;
		rotMat(0,2) = fTxz + fTwy;
		rotMat(1,0) = fTxy + fTwz;
		rotMat(1,1) = 1.0 - (fTxx + fTzz);
		rotMat(1,2) = fTyz - fTwx;
		rotMat(2,0) = fTxz - fTwy;
		rotMat(2,1) = fTyz + fTwx;
		rotMat(2,2) = 1.0 - (fTxx + fTyy);
	}

	void fromEuler(const Vector3& v, const Euler::RotateOrder& order)
	{
		fromEuler(v.x(), v.y(), v.z(), order);
	}

	void fromEuler(
		const double& x, 
		const double& y, 
		const double& z, 
		const Euler::RotateOrder& order
		)
	{
		Quaternion qx;
		Quaternion qy;
		Quaternion qz;

		qx.fromAngleAxis(x, 1.0, 0.0, 0.0);
		qy.fromAngleAxis(y, 0.0, 1.0, 0.0);
		qz.fromAngleAxis(z, 0.0, 0.0, 1.0);

		switch(order)
		{
		case Euler::ZXY:
			*this = qz * qx * qy;
			break;
		case Euler::XYZ:
			*this = qx * qy * qz;
			break;
		default:
			break;
		}
	}

	void fromEuler(const Euler& aEuler)
	{
		Quaternion qx;
		Quaternion qy;
		Quaternion qz;

		qx.fromAngleAxis(aEuler.x, 1.0, 0.0, 0.0);
		qy.fromAngleAxis(aEuler.y, 0.0, 1.0, 0.0);
		qz.fromAngleAxis(aEuler.z, 0.0, 0.0, 1.0);

		switch(aEuler.rotateOrder)
		{
		case Euler::ZXY:
			*this = qz * qx * qy;
			break;
		case Euler::XYZ:
			*this = qx * qy * qz;
			break;
		default:
			break;
		}
	}

	Euler Quaternion::toEuler(Euler::RotateOrder order) const
	{
		Euler aEuler;
		aEuler.rotateOrder = order;

		switch(order)
		{
		case Euler::ZXY:
			aEuler.x = Math::ASinD( 2 * w * x + 2 * y * z);
			aEuler.y = -Math::ATanD2(2 * x * z - 2 * w * y , 1 - 2 * x * x - 2 * y * y);
			aEuler.z = -Math::ATanD2(2 * x * y - 2 * w * z , 1 - 2 * x * x - 2 * z * z);	
			break;
		case Euler::XYZ:
			aEuler.x = -Math::ATanD2(2 * y * z - 2 * w * x , 1 - 2 * x * x - 2 * y * y);
			aEuler.y = Math::ASinD(2 * w * y + 2 * x * z);
			aEuler.z = -Math::ATanD2(2 * x * y - 2 * w * z , 1 - 2 * y * y - 2 * z * z);
			break;
		default:
			break;
		}
		return aEuler;
	}

	Quaternion& fromAngleAxis(double angle, Vector3 axis)
	{
		double s = Math::SinD(angle / 2.0);
		double c = Math::CosD(angle / 2.0);

		axis.normalise();

		x = axis.x() * s;
		y = axis.y() * s;
		z = axis.z() * s;
		w = c;

		return *this;
	}

	Quaternion& fromAngleAxis(double angle, double _x, double _y, double _z)
	{
		double s = Math::SinD(angle / 2.0);
		double c = Math::CosD(angle / 2.0);

		double f = 1.0 / sqrt(_x*_x + _y*_y + _z*_z);

		x = _x * f * s;
		y = _y * f * s;
		z = _z * f * s;
		w = c;

		return *this;
	}

	void toAngleAxis (double& angle, Vector3& axis) const
	{
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		double dSqrLength = x*x + y*y + z*z;
		if ( dSqrLength > 0.0 )
		{
			angle = 2.0 * Math::ACosD(w);
			double dInvLength = 1.0 / sqrt(dSqrLength);
			axis.set_x( x * dInvLength );
			axis.set_y( y * dInvLength);
			axis.set_z( z * dInvLength);
		}
		else
		{
			// angle is 0 (mod 2*pi), so any axis will do
			angle = 0.0;
			axis.set_x(1.0);
			axis.set_y(0.0);
			axis.set_z(0.0);
		}
	}

	double getNorm()
	{
		return w * w + x * x + y * y + z * z;
	}

	void normalize()
	{
		double d = 1.0 / sqrt(getNorm());
		x *= d; 
		y *= d; 
		z *= d; 
		w *= d;
	}

	inline Quaternion& operator= (const Quaternion& q)
	{
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
		return *this;
	}

	inline void Invert()
	{
		// w no change
		x = -x;
		y = -y;
		z = -z;
	}

	Quaternion operator - () const
	{
		return Quaternion(-x, -y, -z, -w);
	}

	Quaternion operator- (const Quaternion& rkQ) const
	{
		return Quaternion(x-rkQ.x,y-rkQ.y,z-rkQ.z,w-rkQ.w);
	}

	Quaternion operator+ (const Quaternion& rkQ) const
	{
		return Quaternion(x+rkQ.x,y+rkQ.y,z+rkQ.z,w+rkQ.w);
	}

	Quaternion operator* (double fScalar) const
	{
		return Quaternion(fScalar*x,fScalar*y,fScalar*z,fScalar*w);
	}

	Quaternion operator / (double fScalar) const{
		double sInv = 1.0 / fScalar;
		return Quaternion(sInv*x,sInv*y,sInv*z,sInv*w);
	}



	Quaternion TransitionFromCurrentTo(const Quaternion rkQ)
	{
		// change by ywpg, 080521
		Quaternion qTemp, qResult;
		qTemp = (*this);
		qTemp.normalize();
		qTemp.Invert();

		qResult = qTemp * rkQ;

		return qResult;

		//Quaternion returnQ;
		//this->Invert();
		//returnQ = (*this)*rkQ;
		//return returnQ;
	}

	static Quaternion nlerp(double fT, const Quaternion& rkP, 
		const Quaternion& rkQ, bool shortestPath)
	{
		Quaternion result;
		float fCos = Dot(rkP, rkQ);
		if (fCos < 0.0f && shortestPath)
		{
			result = rkP + ((-rkQ) - rkP) * fT;
		}
		else
		{
			result = rkP +  (rkQ - rkP) * fT;
		}
		result.getNorm();
		return result;
	}


	static float Dot(const Quaternion& q1, const Quaternion& q2)
	{
		return q1.get_w() * q2.get_w() + q1.get_x() * q2.get_x() + q1.get_y() * q2.get_y() + q1.get_z() * q2.get_z() ;
	}


	//对数运算 rukun.fan@gmail.com
	inline Vector3 toLOG () const
	{
		Vector3 res(0,0,0);

		float sinvalue=sqrt(x * x + y * y + z * z);
		float cosvalue=w;
		if(sinvalue==0||cosvalue==0)
			return res;	
		float alpha=atan(sinvalue/cosvalue);
		res.set_x(alpha*x/sinvalue);
		res.set_y(alpha*y/sinvalue);
		res.set_z(alpha*z/sinvalue);
		return res;
	}

	inline Quaternion log() const{
		Vector3 lo = toLOG();
		return Quaternion(lo.x(), lo.y(), lo.z(), 0);
	}

	inline Quaternion exp() const{
		float theta = sqrt(x*x + y*y + z*z) ;

		if (theta < 1e-6 )
			return Quaternion(x, y, z, 0);
		else
		{
			float coef = sin(theta)/ theta;
			return Quaternion(coef*x ,coef*y ,coef*z, cos(theta));
		}
	}

	static Quaternion pow(const Quaternion &q, double exponent){
		// 单位四元数，返回本身
		Quaternion tempQ = q;
		tempQ.normalize();
		if(fabs(tempQ.get_w()) > 0.99999/* && fabs(tempQ.get_w()) < 1.00001*/){
			return q;
		}

		//	return (q.log() * exponent).exp();

		// 提取半角
		double alpha = acos(tempQ.get_w());

		// 计算新alpha值
		double newAlpha = alpha * exponent;

		// 计算新w值
		Quaternion result;
		result.set_w(cos(newAlpha));

		// 计算新xyz值
		double mult = sin(newAlpha) / sin(alpha);
		result.set_x(tempQ.get_x() * mult);
		result.set_y(tempQ.get_y() * mult);
		result.set_z(tempQ.get_z() * mult);

		// 返回
		return result;

		//// 提取半角
		//double alpha = acos(q.get_w());

		//// 计算新alpha值
		//double newAlpha = alpha * exponent;

		//// 计算新w值
		//Quaternion result;
		//result.set_w(cos(newAlpha));

		//// 计算新xyz值
		//double mult = sin(newAlpha) / sin(alpha);
		//result.set_x(q.get_x() * mult);
		//result.set_y(q.get_y() * mult);
		//result.set_z(q.get_z() * mult);

		//// 返回
		//return result;
	}

	void setAlphaQuat(const Quaternion& quat2,const double& alpha)
	{
		if (alpha<=0)  
		{	
			x=0.0;
			y=0.0;
			z=0.0;
			w=1.0;
			return;	
		}
		if (alpha>=1)	
		{	
			x=quat2.get_x();
			y=quat2.get_y();
			z=quat2.get_z();
			w=quat2.get_w();
			return;
		}

		double		g( quat2.get_w() );
		bool		notinv( true );

		if ( g < 0 )
		{
			g = -g;
			notinv=false;
		}
		double fa(alpha);
		double fb(double(1) - alpha);

		if (double(1) - g > double(0.0001) )
		{
			double ft = std::acos( g );
			double k = double(1)/std::sin(ft);
			fb = std::sin( ft*fb ) * k;
			fa = std::sin( ft*fa ) * k;
		}

		if (notinv)
		{	
			x=     fa*quat2.get_x();
			y=     fa*quat2.get_y();
			z=     fa*quat2.get_z();
			w=fb + fa*quat2.get_w();
		}
		else
		{	
			x=     -fa*quat2.get_x();
			y=     -fa*quat2.get_y();
			z=     -fa*quat2.get_z();
			w=fb - fa*quat2.get_w();
		}
	}

	friend Quaternion operator*(const Quaternion& left, const Quaternion& right);
	friend Quaternion operator/(const Quaternion& q1, const Quaternion& q2);
	friend Vector3 operator* (const Quaternion& q, const Vector3& v3);
	friend Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, double t, bool bshortestslerp /* = true */); 
	friend Quaternion Squad(Quaternion& , Quaternion& , Quaternion& , Quaternion& , float);
	friend Quaternion Inverse(const Quaternion& q);
	friend Quaternion getRotationTo(const Vector3& origin,
		const Vector3& dest,
		const Vector3& fallbackAxis = Vector3::ZERO);
	friend Vector3 qLog(const Quaternion &q);
	friend Quaternion qExp(const Vector3 &v);
};

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, double t, bool bshortestslerp = true);



//Q = leftQ * rightQ, this function is carefully checked to be correct and efficient 
Quaternion operator*(const Quaternion& left, const Quaternion& right);

Quaternion operator/(const Quaternion& q1, const Quaternion& q2);

Quaternion Inverse(const Quaternion& q);

Vector3 operator*(const Quaternion& q, const Vector3& v3);

/** Gets the shortest arc quaternion to rotate this vector to the destination
vector.
@remarks
If you call this with a dest vector that is close to the inverse
of this vector, we will rotate 180 degrees around the 'fallbackAxis'
(if specified, or a generated axis if not) since in this case
ANY axis of rotation is valid.
*/
Quaternion getRotationTo(const Vector3& origin,
						 const Vector3& dest,
						 const Vector3& fallbackAxis/* = Vector3::ZERO*/);

Vector3 qLog(const Quaternion &q);
Quaternion qExp(const Vector3 &v);

namespace Math
{
	template<>
	inline bool IsNaN<Quaternion>(const Quaternion& x)
	{
		return (x.get_x() != x.get_x());
	}
}

#ifdef _DEBUG
std::ostream& operator<< (std::ostream& os, const Quaternion q);
#endif // _DEBUG


#endif //_QUATERNION_H_INCLUDED_
