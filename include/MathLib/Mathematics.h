#ifndef _MATH_H_INCLUDED_
#define _MATH_H_INCLUDED_

#include <cmath>
#include <cassert>

namespace Math
{
	const double PI			= 3.14159265358979323846;
	const double DEG_TO_RAD	= 0.0174532925199432957692;
	const double RAD_TO_DEG	= 57.2957795130823208768;
	const double TOLERANCE	= 0.000001;
	// ------------------------------------------------------------
	// basic functions
	// ------------------------------------------------------------

	template<class T>
	inline bool IsEven(const T& aInt)
	{
		return ! (aInt & 1); 
	}

	template<class T>
	inline bool IsOdd(const T& aInt)
	{
		return (aInt & 1); 
	}

	template<class T>
	inline T Clamp(const T& value, const T& min, const T& max)
	{
		return min > value ? min : (max > value ? value : max);
	}

	template<class T>
	inline T Lerp(double factor, const T& from, const T& to)
	{
		return from + (to - from) * factor;
	}

	template<class T>
	inline T Min(const T& value1, const T& value2)
	{
		return value1 < value2 ? value1 : value2;
	}

	template<class T>
	inline T Max(const T& value1, const T& value2)
	{
		return value1 > value2 ? value1 : value2;
	}

	template<class T>
	inline T Min(const T* arrayData, int size)
	{
		assert(size>0);
		T minValue = arrayData[0];
		for(int i=1;i<size;i++)
		{
			if(arrayData[i] < minValue)
			{
				minValue = arrayData[i];
			}
		}
		return minValue;
	}

	template<class T>
	inline T Max(const T* arrayData, int size)
	{
		assert(size>0);
		T maxValue = arrayData[0];
		for(int i=1;i<size;i++)
		{
			if(arrayData[i] > maxValue)
			{
				maxValue = arrayData[i];
			}
		}
		return maxValue;
	}

	//Trigonometric functions
	inline double SinD(double degree)
	{
		return sin(degree * DEG_TO_RAD);
	}

	inline double ASinD(double value)
	{
		if (-1.0 < value)
		{
			if ( value < 1.0 )
				return (std::asin(value)*RAD_TO_DEG);
			else
				return -90.0;
		}
		else
		{
			return 90.0;
		}
	}

	inline double CosD(double degree)
	{
		return std::cos(degree * DEG_TO_RAD);
	}

	inline double ACosD(double value)
	{
		if ( -1.0 < value )
		{
			if ( value < 1.0 )
				return (std::acos(value)*RAD_TO_DEG);
			else
				return 0.0;
		}
		else
		{
			return PI;
		}
	}

	inline double TanD(double degree)
	{
		return std::tan(degree * DEG_TO_RAD);
	}

	inline double ATanD(double degree)
	{
		return std::atan(degree * RAD_TO_DEG);
	}

	inline double ATanD2(double valueUp, double valueDown)
	{
		return std::atan2(valueUp, valueDown) * RAD_TO_DEG;
	}

	inline double RadianToDegree(double radian)
	{
		return radian * RAD_TO_DEG;
	}

	inline double DegreeToRadian(double degree)
	{
		return degree * DEG_TO_RAD;
	}

	inline bool IsZero(double aDouble)
	{
		return std::fabs(aDouble) < TOLERANCE;
	}
	
	// this is not right, because: 1. rand() in in stdlib   2.we need srand()
//
//	inline double Rand(double max)
//	{
//		return double(std::rand()) / double(std::RAND_MAX) * max;
//	}
//
//	inline double Rand(double min, double max)
//	{
//		return Rand(max - min) + min; 
//	}

	template <typename T>
	inline bool IsNaN(const T& x)
	{
		// nothing strange. If you are confused, search "NaN" in web.
		return (x != x);
	}


}

#endif // _MATH_H_INCLUDED_
