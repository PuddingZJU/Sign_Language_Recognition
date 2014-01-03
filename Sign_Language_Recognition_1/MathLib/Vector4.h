#ifndef Vector4_h__
#define Vector4_h__

#include "VectorT.h"

class Vector4 : public VectorT<double,4>
{
public:
	Vector4(void)
	{
	}

	//Constructor:copy from an array
	Vector4(const double* arrayData)
	{
		copyFromArray(arrayData);
	}

	Vector4(double x, double y, double z, double w)
	{
		_data[0] = x;
		_data[1] = y;
		_data[2] = z;
		_data[3] = w;
	};

	//Do not implement Copy-constructor and Assignment-overloading

	//Superclass Copy constructor
	Vector4(const VectorT<double,4>& aVec)
	{
		for(int i=0; i<4; i++)
		{
			_data[i] = aVec[i];
		}
	}

	//Superclass Assignment operator
	Vector4& operator=(const VectorT<double,4>& aVec)
	{
		if(this != &aVec)
		{
			for(int i=0; i<4; i++)
			{
				_data[i] = aVec[i];
			}
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

	double w() const
	{
		return _data[3];
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

	void w(double value)
	{
		_data[3] = value;
	}
};

#endif // Vector4_h__
