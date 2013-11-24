#ifndef Vector2_h__
#define Vector2_h__

//#include "Euler.h"
#include "VectorT.h"
//#include "Vector4.h"

#include <iostream> // for debugging

//class Euler;

class Vector2 : public VectorT<double,2>
{
public:
	//static const Vector3 ZERO;
	//static const Vector3 UNIT_SCALE;
	//static const Vector3 UNIT_X;
	//static const Vector3 UNIT_Y;
	//static const Vector3 UNIT_Z;
	//static const Vector3 NEGATIVE_UNIT_X;
	//static const Vector3 NEGATIVE_UNIT_Y;
	//static const Vector3 NEGATIVE_UNIT_Z;

public:
	Vector2(void)
	{
	}

	//Constructor:copy from an array
	Vector2(const double* arrayData)
	{
		copyFromArray(arrayData);
	}

	Vector2(double x,double y)
	{
		_data[0] = x;
		_data[1] = y;
	}

	//Superclass Copy constructor
	Vector2(const VectorT<double,2>& aVec)
	{
		for(int i=0; i<2; i++)
		{
			_data[i] = aVec[i];
		}
	}

	//Superclass Assignment operator
	Vector2& operator=(const VectorT<double,2>& aVec)
	{
		if(this != &aVec)
		{
			for(int i=0; i<2; i++)
			{
				_data[i] = aVec[i];
			}
		}
		return *this;
	}

	Vector2 operator+(const VectorT<double,2>& aVec)
	{
		for (int i=0; i<2; i++)
		{
			_data[i] += aVec[i];
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

	void x(double value)
	{
		_data[0] = value;
	}

	void y(double value)
	{
		_data[1] = value;
	}

	void set_x(double value)
	{
		_data[0] = value;
	}

	void set_y(double value)
	{
		_data[1] = value;
	}
	
	void set_values(double d1, double d2)
	{
		_data[0] = d1;
		_data[1] = d2;
	}
	
	Vector2 scaling(const Vector2& other) const
	{
		Vector2 v;
		v._data[0] = _data[0] * other._data[0];
		v._data[1] = _data[1] * other._data[1];

		return v;
	}
};

////Cross multiply
//inline Vector2 Cross(const Vector2& left,const Vector2& right)
//{
//	return Vector3(
//		left.y()*right.z()-left.z()*right.y(),
//		left.z()*right.x()-left.x()*right.z(),
//		left.x()*right.y()-left.y()*right.x()
//		);
//}

#ifdef _DEBUG
std::ostream& operator<< (std::ostream& os, const Vector2& vec);
#endif // _DEBU

#endif // Vector2_h__
