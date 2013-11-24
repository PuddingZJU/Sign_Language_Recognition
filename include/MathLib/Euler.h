#ifndef	_EULER_H_INCLUDED_ 
#define _EULER_H_INCLUDED_ 

//#include "Vector3.h"
#include "Mathematics.h"
#include "Matrix3.h"

class Euler
{
public:
	
	enum RotateOrder{
		UNKNOWN,
		ZXY,
		XYZ
	};
	
	double x;
	double y;
	double z;
	RotateOrder rotateOrder;

	Euler()
		:x(0), y(0), z(0), rotateOrder(UNKNOWN)
	{
	}

	Euler(const Euler& euler)
	{
		x = euler.x;
		y = euler.y;
		z = euler.z;
		rotateOrder = euler.rotateOrder;
	}

	Euler(double _x, double _y, double _z, RotateOrder order)
		:x(_x), y(_y), z(_z), rotateOrder(order)
	{
	}

	Euler(double* arrayData, RotateOrder order)
		:x(arrayData[0]), y(arrayData[1]), z(arrayData[2]), rotateOrder(order)
	{
	}

	Euler(const Matrix3& rotMat, RotateOrder order)
	{
		fromRotateMatrix(rotMat, order);
	}

	void fromArray(double* arrayData, RotateOrder order)
	{
		x = arrayData[0];
		y = arrayData[1];
		z = arrayData[2];
		rotateOrder = order;
	}

	void fromRotateMatrix(const Matrix3& rotMat, RotateOrder order)
	{
		this->rotateOrder = order;
		switch(rotateOrder)
		{
		case ZXY:
			x = Math::ASinD(rotMat[7]);
			y = Math::ATanD2(-rotMat[6], rotMat[8]);
			z = Math::ATanD2(-rotMat[1], rotMat[4]);
			break;
		case XYZ:
			//TODO
			break;
		default:
			break;
		}
	}

	Matrix3 toRotateMatrix()
	{
		Matrix3 rotMat;
		double cosx = Math::CosD(x);
		double cosy = Math::CosD(y);
		double cosz = Math::CosD(z);
		double sinx = Math::SinD(x);
		double siny = Math::SinD(y);
		double sinz = Math::SinD(z);
		switch(rotateOrder)
		{
		case ZXY:
			rotMat[0] = cosy * cosz - sinx * siny * sinz;
			rotMat[1] = - cosx * sinz;
			rotMat[2] = sinx * cosy * sinz + siny * cosz;
			rotMat[3] = cosy * sinz + sinx * siny * cosz;
			rotMat[4] = cosx * cosz;
			rotMat[5] = siny * sinz - sinx * cosy * cosz;
			rotMat[6] = - cosx * siny;
			rotMat[7] = sinx;
			rotMat[8] = cosx * cosy;
			break;
		case XYZ:
			//TODO
			break;
		default:
			break;
		}
		return rotMat;
	}
};
#endif //_EULER_H_INCLUDED_ 
