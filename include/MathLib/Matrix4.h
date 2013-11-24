#ifndef _MATRIX4_H_INCLUDED_
#define _MATRIX4_H_INCLUDED_

#include "MatrixT.h"
#include "Vector3.h"

class Matrix4 
: public MatrixT<double,4,4>
{
public:
	static const Matrix4 ZERO;
	static const Matrix4 IDENTITY;
	static const Matrix4 CLIPSPACE2DTOIMAGESPACE;

	Matrix4(void)
	{
	}

	//Constructor: copy from an array
	Matrix4(double* arrayData)
	{
		copyFromArray(arrayData);
	}

	Matrix4(
			double d0,  double d1,  double d2,  double d3,
			double d4,  double d5,  double d6,  double d7,
			double d8,  double d9,  double d10, double d11,
			double d12, double d13, double d14, double d15
		   )
	{
		_data[0] = d0; _data[1] = d1;
		_data[2] = d2; _data[3] = d3;
		_data[4] = d4; _data[5] = d5;
		_data[6] = d6; _data[7] = d7;
		_data[8] = d8; _data[9] = d9;
		_data[10] = d10; _data[11] = d11;
		_data[12] = d12; _data[13] = d13;
		_data[14] = d14; _data[15] = d15;
	}

	//Do not implement Copy-constructor and Assignment-overloading

	//Copy constructor from a superclass object
	Matrix4(const MatrixT<double,4,4>& aMat)
	{
		for(int i=0; i<16; i++)
		{
			_data[i] = aMat[i];
		}
	}

	Matrix4& operator=(const MatrixT<double,4,4>& aMat)
	{
		if(this != &aMat)
		{
			for(int i=0; i<16; i++)
			{
				_data[i] = aMat[i];
			}
			
		}
		return *this;
	}

	/// Assignment from 3x3 matrix.
	inline void operator = (const Matrix3& mat3)
	{
		_data[0] = mat3[0];
		_data[1] = mat3[1];
		_data[2] = mat3[2];
		_data[3] = 0;
		_data[4] = mat3[3];
		_data[5] = mat3[4];
		_data[6] = mat3[5];
		_data[7] = 0;
		_data[8] = mat3[6];
		_data[9] = mat3[7];
		_data[10] = mat3[8];
		_data[11] = 0;
		_data[12] = 0;
		_data[13] = 0;
		_data[14] = 0;
		_data[15] = 0;
	}

	/// Sets the translation transformation part of the matrix.
	inline void setTrans(const Vector3& v)
	{
		_data[3] = v.x();
		_data[7] = v.y();
		_data[11] = v.z();
	}
};

#endif //_MATRIX4_H_INCLUDED_
