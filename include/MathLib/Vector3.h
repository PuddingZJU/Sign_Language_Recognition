#ifndef Vector3_h__
#define Vector3_h__

#include "Util/floatdef.h"
#include "VectorT.h"
#include "Vector4.h"
#include "Euler.h"


#include <iostream> // for debugging

class Euler;

class Vector3 : public VectorT<double,3>
{
public:
	static const Vector3 ZERO;
	static const Vector3 UNIT_SCALE;
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;
	static const Vector3 NEGATIVE_UNIT_X;
	static const Vector3 NEGATIVE_UNIT_Y;
	static const Vector3 NEGATIVE_UNIT_Z;
	static const Vector3 QNAN;
	static const Vector3 SNAN;

public:
	Vector3(void)
	{
	}

	//Constructor:copy from an array
	Vector3(const double* arrayData)
	{
		copyFromArray(arrayData);
	}

	Vector3(double x,double y,double z)
	{
		_data[0] = x;
		_data[1] = y;
		_data[2] = z;
	}
	
	// from class Euler            add by ywpg
	explicit Vector3(const Euler& aEuler)
	{
		_data[0] = aEuler.x;
		_data[1] = aEuler.y;
		_data[2] = aEuler.z;
	}

	//Superclass Copy constructor
	Vector3(const VectorT<double,3>& aVec)
	{
		for(int i=0; i<3; i++)
		{
			_data[i] = aVec[i];
		}
	}

	//Superclass Assignment operator
	Vector3& operator=(const VectorT<double,3>& aVec)
	{
		if(this != &aVec)
		{
			for(int i=0; i<3; i++)
			{
				_data[i] = aVec[i];
			}
		}
		return *this;
	}

	bool operator == (const VectorT<double,3>& aVec) const
	{
		return Swift::DoubleEqual( _data[0], aVec[0] )
			&& Swift::DoubleEqual( _data[1], aVec[1] )
			&& Swift::DoubleEqual( _data[2], aVec[2] );
	}
	//Vector3 operator*(float num)
	//{
	//	Vector3 v;
	//	v.x(_data[0]*num);
	//	v.y(_data[1]*num);
	//	v.z(_data[2]*num);

	//	return v;
	//}
	Vector3 operator+(const VectorT<double,3>& aVec) const
	{
		Vector3 v;
		v.x(_data[0] + aVec[0]);
		v.y(_data[1] + aVec[1]);
		v.z(_data[2] + aVec[2]);

		return v;
	}

	Vector3 operator-(const VectorT<double,3>& aVec) const
	{
		Vector3 v;
		v.x(_data[0] - aVec[0]);
		v.y(_data[1] - aVec[1]);
		v.z(_data[2] - aVec[2]);

		return v;
	}

	Vector3 operator+=(const VectorT<double,3>& aVec)
	{
		for (int i=0; i<3; i++)
		{
			_data[i] += aVec[i];
		}

		return *this;
	}

	Vector3 operator-=(const VectorT<double,3>& aVec)
	{
		for (int i=0; i<3; i++)
		{
			_data[i] -= aVec[i];
		}

		return *this;
	}


	//getter and setter
	double x() const
	{
		return _data[0];
	}

	double y() const
	{
		return _data[1];
	}

	double z() const
	{
		return _data[2];
	}

	void x(double value)
	{
		_data[0] = value;
	}

	void y(double value)
	{
		_data[1] = value;
	}

	void z(double value)
	{
		_data[2] = value;
	}
	
	void set_x(double value)
	{
		_data[0] = value;
	}

	void set_y(double value)
	{
		_data[1] = value;
	}

	void set_z(double value)
	{
		_data[2] = value;
	}
	
	void set_values(double d1, double d2, double d3)
	{
		_data[0] = d1;
		_data[1] = d2;
		_data[2] = d3;
	}
	
	Vector4 toVector4()
	{
		return Vector4(_data[0], _data[1], _data[2], 1);
	}


	Vector3 scaling(const Vector3& other) const
	{
		Vector3 v;
		v._data[0] = _data[0] * other._data[0];
		v._data[1] = _data[1] * other._data[1];
		v._data[2] = _data[2] * other._data[2];

		return v;
	}
};

//Cross multiply
inline Vector3 Cross(const Vector3& left,const Vector3& right)
{
	return Vector3(
		left.y()*right.z()-left.z()*right.y(),
		left.z()*right.x()-left.x()*right.z(),
		left.x()*right.y()-left.y()*right.x()
		);
}

namespace Math
{
	template <>
	inline bool IsNaN<Vector3>(const Vector3& x)
	{
		//  ;)
		return (x.x() != x.x());
	}
}

#ifdef _DEBUG
std::ostream& operator<< (std::ostream& os, const Vector3& vec);
#endif // _DEBU

#endif // Vector3_h__
