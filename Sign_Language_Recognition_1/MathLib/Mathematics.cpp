/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/09/14                                                  */
/* Last Modified: 2008/01/05 05:32:53 .                                 */
/************************************************************************/

#include <iostream> // for debugging

#include <limits>

#include "Mathematics.h"

#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix3.h"
#include "Matrix4.h"


// Vector3
const Vector3 Vector3::ZERO(0.0, 0.0, 0.0);
const Vector3 Vector3::UNIT_SCALE(1.0, 1.0, 1.0);
const Vector3 Vector3::UNIT_X(1.0, 0, 0);
const Vector3 Vector3::UNIT_Y(0, 1.0, 0);
const Vector3 Vector3::UNIT_Z(0, 0, 1.0);
const Vector3 Vector3::NEGATIVE_UNIT_X(-1.0, 0, 0);
const Vector3 Vector3::NEGATIVE_UNIT_Y(0, -1.0, 0);
const Vector3 Vector3::NEGATIVE_UNIT_Z(0, 0, -1.0);
const Vector3 Vector3::QNAN(
							std::numeric_limits<double>::quiet_NaN(),
							std::numeric_limits<double>::quiet_NaN(),
							std::numeric_limits<double>::quiet_NaN()
							);
const Vector3 Vector3::SNAN(
							std::numeric_limits<double>::signaling_NaN(),
							std::numeric_limits<double>::signaling_NaN(),
							std::numeric_limits<double>::signaling_NaN()
							);

#ifdef _DEBUG
std::ostream& operator<< (std::ostream& os, const Vector3& vec)
{
	return os << "(" 
		<< vec.x() << ", " 
		<< vec.y() << ", " 
		<< vec.z() << ")" ;
}
#endif // _DEBUG
// End of Vector3

// Matrix3
const Matrix3 Matrix3::ZERO(0,0,0,0,0,0,0,0,0);
const Matrix3 Matrix3::IDENTITY(1,0,0,0,1,0,0,0,1);
// End of Matrix3

// Matrix4
const Matrix4 Matrix4::ZERO
(
 0, 0, 0, 0,
 0, 0, 0, 0,
 0, 0, 0, 0,
 0, 0, 0, 0 );

const Matrix4 Matrix4::IDENTITY
(
 1, 0, 0, 0,
 0, 1, 0, 0,
 0, 0, 1, 0,
 0, 0, 0, 1 );

const Matrix4 Matrix4::CLIPSPACE2DTOIMAGESPACE
(
 0.5,    0,  0, 0.5, 
 0, -0.5,  0, 0.5, 
 0,    0,  1,   0,
 0,    0,  0,   1);
// End of Matrix4

// Quaternion
const Quaternion Quaternion::ZERO(0.0, 0.0, 0.0, 0.0);
const Quaternion Quaternion::IDENTITY(0.0, 0.0, 0.0, 1.0);
const Quaternion Quaternion::QNAN(
								  std::numeric_limits<double>::quiet_NaN(),
								  std::numeric_limits<double>::quiet_NaN(),
								  std::numeric_limits<double>::quiet_NaN(),
								  std::numeric_limits<double>::quiet_NaN()
								  );
const Quaternion Quaternion::SNAN(
								  std::numeric_limits<double>::signaling_NaN(),
								  std::numeric_limits<double>::signaling_NaN(),
								  std::numeric_limits<double>::signaling_NaN(),
								  std::numeric_limits<double>::signaling_NaN()
								  );

Quaternion Inverse(const Quaternion& q)
{
	Quaternion retQ = q;
	retQ.normalize();
	retQ.x = -retQ.x;
	retQ.y = -retQ.y;
	retQ.z = -retQ.z;

	return retQ;
}

Quaternion operator*(const Quaternion& left, const Quaternion& right)
{
	Quaternion retQ;
	retQ.x = left.y * right.z - left.z * right.y + left.x * right.w + left.w * right.x;
	retQ.y = left.z * right.x - left.x * right.z + left.y * right.w + left.w * right.y;
	retQ.z = left.x * right.y - left.y * right.x + left.z * right.w + left.w * right.z;
	retQ.w = left.w * right.w - left.x * right.x - left.y *  right.y - left.z * right.z;
	return retQ;
}

Quaternion operator/(const Quaternion& q1, const Quaternion& q2)
{
	double n(1),magnit2 ( q2.x * q2.x + q2.y * q2.y + q2.z * q2.z + q2.w * q2.w);
	if (( magnit2 >= 0.000000001 )&&( magnit2 != double(1))) n=double(1)/std::sqrt(magnit2);

	return Quaternion (
		( q2.w * q1.x - q2.x * q1.w - q2.y * q1.z + q2.z * q1.y )*n,
		( q2.w * q1.y - q2.y * q1.w - q2.z * q1.x + q2.x * q1.z )*n,
		( q2.w * q1.z - q2.z * q1.w - q2.x * q1.y + q2.y * q1.x )*n,
		( q2.w * q1.w + q2.x * q1.x + q2.y * q1.y + q2.z * q1.z )*n);
}


Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, double t, bool bshortestslerp /* = true */)
{
	Quaternion SlerpedQuaternion;
	double cosom = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
	double tmp0, tmp1, tmp2, tmp3;
	if (cosom < 0.0f && bshortestslerp)
	{
		cosom = -cosom;
		tmp0 = -q1.x;
		tmp1 = -q1.y;
		tmp2 = -q1.z;
		tmp3 = -q1.w;
	}
	else
	{
		tmp0 = q1.x;
		tmp1 = q1.y;
		tmp2 = q1.z;
		tmp3 = q1.w;
	}

	/* calc coeffs */
	double scale0, scale1;

	if ((1.0 - cosom) > 0.0000001f)
	{
		// standard case (slerp)
	//	double omega = (double) acos (cosom);
	//	double sinom = (double) sin (omega);
		double sinom = sqrt(1.0 - cosom * cosom);
		double omega = atan2(sinom, cosom);

		scale0 = (double) sin ((1.0 - t) * omega) / sinom;
		scale1 = (double) sin (t * omega) / sinom;
	}
	else
	{
		/* just lerp */
		scale0 = 1.0f - t;
		scale1 = t;
	}

	SlerpedQuaternion.x = scale0 * q0.x + scale1 * tmp0;
	SlerpedQuaternion.y = scale0 * q0.y + scale1 * tmp1;
	SlerpedQuaternion.z = scale0 * q0.z + scale1 * tmp2;
	SlerpedQuaternion.w = scale0 * q0.w + scale1 * tmp3;

	return SlerpedQuaternion;
}

// Perform symmetric quadric interpolation.
// Refer for graphcial explanation Watt: Adv Animation and Rendering techniques.
Quaternion Squad(Quaternion& q, Quaternion& s_1, Quaternion& s_2, Quaternion& r, float t)
{
	Quaternion sl1 = Slerp(q,r,t);
	Quaternion sl2 = Slerp(s_1,s_2,t);        
	return Quaternion(Slerp(sl1,sl2, 2*t*(1-t)) );
}

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
						 const Vector3& fallbackAxis/* = Vector3::ZERO*/)
{
	// Based on Stan Melax's article in Game Programming Gems
	Quaternion q;
	// Copy, since cannot modify local
	Vector3 v0 = origin;
	Vector3 v1 = dest;
	v0.normalise();
	v1.normalise();

	double d = Dot(v0, v1);
	// If dot == 1, vectors are the same
	if (d >= 1.0f)
	{
		return Quaternion::IDENTITY;
	}
	Vector3 axis = Cross(v0, v1);
	double angle = Math::ACosD(d);//std::acos(dot);
//	double angle = std::acos(d);
	q.fromAngleAxis(angle /** 180 / Math::PI*/, axis);
	
	//double s = std::sin(angle / 2.0);//Math::SinD(angle / 2.0);
	//double c = std::cos(angle / 2.0);//Math::CosD(angle / 2.0);

	//axis.normalise();

	//q.set_x(axis.x() * s);
	//q.set_y(axis.y() * s);
	//q.set_z(axis.z() * s);
	//q.set_w(c);

	/*x = axis.x() * s;
	y = axis.y() * s;
	z = axis.z() * s;
	w = c;*/
	q.normalize();
	//if (d < (1e-6f - 1.0f))
	//{
	//	if (fallbackAxis.x() != 0 && fallbackAxis.y() != 0 && fallbackAxis.z() != 0)
	//	{
	//		// rotate 180 degrees about the fallback axis
	//		q.fromAngleAxis(180, fallbackAxis);
	//	}
	//	else
	//	{
	//		// Generate an axis
	//		Vector3 axis = Cross(Vector3::UNIT_X, (origin));
	//		if (axis.getNorm() == 0) // pick another if colinear
	//			axis = Cross(Vector3::UNIT_Y, (origin));
	//		axis.normalise();
	//		q.fromAngleAxis(180, axis);
	//	}
	//}
	//else
	//{
	//	double s = sqrt( (1+d)*2 );
	//	double invs = 1 / s;

	//	Vector3 c = Cross(v0, v1);

	//	q.set_values(c.x() * invs, c.y() * invs, c.z() * invs, s * 0.5);

	///*	q.x = c.x * invs;
	//	q.y = c.y * invs;
	//	q.z = c.z * invs;
	//	q.w = s * 0.5;*/
	//	q.normalize();
	//}
	return q;
}

Vector3 qLog(const Quaternion &q){
	Vector3 res(0.0, 0.0, 0.0);

	float sinvalue = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
	float cosvalue = q.w;
	if(Math::IsZero(sinvalue))
		return res;	

	double alpha = 0.0;
	if(Math::IsZero(cosvalue))
		alpha = Math::PI / 2.0;
	else
		alpha = atan(sinvalue / cosvalue);
	res.set_x(alpha * q.x / sinvalue);
	res.set_y(alpha * q.y / sinvalue);
	res.set_z(alpha * q.z / sinvalue);
	return res;
}

Quaternion qExp(const Vector3 &v){
	double x = v.x();
	double y = v.y();
	double z = v.z();
	double theta = sqrt(x * x + y * y + z * z) ;

	if (theta < 1e-6 )
		return Quaternion(x, y, z, 0);
	else{
		float coef = sin(theta) / theta;
		return Quaternion(coef * x ,coef * y ,coef * z, cos(theta));
	}
}

Vector3 operator*(const Quaternion& q, const Vector3& v3)
{
	Quaternion vecQ(v3.x(), v3.y(), v3.z(), 1.0);
	vecQ = q * vecQ * Inverse(q);
	return Vector3(vecQ.get_x() / vecQ.get_w(), vecQ.get_y() / vecQ.get_w(), vecQ.get_z() / vecQ.get_w());
}

#ifdef _DEBUG
std::ostream& operator<< (std::ostream& os, const Quaternion q)
{
	return os << "("
		<< q.get_x() << ", "
		<< q.get_y() << ", "
		<< q.get_z() << ", "
		<< q.get_w() << ")" ;
}
#endif //_DEBUG
// End of Quaternion
