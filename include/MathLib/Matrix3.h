#ifndef Matrix3_h__
#define Matrix3_h__

#include "MatrixT.h"

class Matrix3 : public MatrixT<double,3,3>
{ 
public:
	static const Matrix3 ZERO;
	static const Matrix3 IDENTITY;

public:
	Matrix3(void)
	{
	}

	//Constructor:copy from an array
	Matrix3(double* arrayData)
	{
		copyFromArray(arrayData);
	}

	Matrix3(
		double d0,  double d1,  double d2,  
		double d3,	double d4,  double d5,  
		double d6,  double d7,	double d8
		)
	{
		_data[0] = d0; _data[1] = d1;
		_data[2] = d2; _data[3] = d3;
		_data[4] = d4; _data[5] = d5;
		_data[6] = d6; _data[7] = d7;
		_data[8] = d8; 
	}

	//Do not implement Copy-constructor and Assignment-overloading

	//Copy constructor from a superclass object
	Matrix3(const MatrixT<double,3,3>& aMat)
	{
		for(int i=0; i<9; i++)
		{
			_data[i] = aMat[i];
		}
	}

	Matrix3& operator=(const MatrixT<double,3,3>& aMat)
	{
		if(this != &aMat)
		{
			for(int i=0; i<9; i++)
			{
				_data[i] = aMat[i];
			}
		}
		return *this;
	}

	void inverse(Matrix3 &vmInverse){
		// Invert a 3x3 using cofactors.  This is about 8 times faster than
		// the Numerical Recipes code which uses Gaussian elimination.
		vmInverse[0] = _data[4] * _data[8] - _data[5] * _data[7];
		vmInverse[1] = _data[2] * _data[7] - _data[1] * _data[8];
		vmInverse[2] = _data[1] * _data[5] - _data[2] * _data[4];
		vmInverse[3] = _data[5] * _data[6] - _data[3] * _data[8];
		vmInverse[4] = _data[0] * _data[8] - _data[2] * _data[6];
		vmInverse[5] = _data[2] * _data[3] - _data[0] * _data[5];
		vmInverse[6] = _data[3] * _data[7] - _data[4] * _data[6];
		vmInverse[7] = _data[1] * _data[6] - _data[0] * _data[7];
		vmInverse[8] = _data[0] * _data[4] - _data[1] * _data[3];

		float fDet = static_cast<float>(
			_data[0] * vmInverse[0] +
			_data[1] * vmInverse[3]+
			_data[2] * vmInverse[6]   );

		float fInvDet = 1.0f / fDet;
		for (size_t iRow = 0; iRow < 3; iRow++){
			for (size_t iCol = 0; iCol < 3; iCol++){
				vmInverse[iRow * 3 + iCol] *= fInvDet;

			} //#### end for iCol
		} //#### end for iRow
	};
};

#endif // Matrix3_h__
