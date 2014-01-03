#ifndef  ZZ_Recognition_FLOAT_NUMBER_BASIC_DEFINITIONS__H___
#define  ZZ_Recognition_FLOAT_NUMBER_BASIC_DEFINITIONS__H___
#pragma once
/**
 * @author  Thinapple (turingmc@gmail.com)
 */

namespace Swift
{
;
#define  Recognition_CMP_MIN_DOUBLE (1e-10)
#define  Recognition_CMP_MAX_DOUBLE (1e+10)

/// Compare tow float numbers

inline bool DoubleEqual(const double & l, const double & r)
{
	return ((l - r) <= Recognition_CMP_MIN_DOUBLE) && ((l - r) >= -Recognition_CMP_MIN_DOUBLE);
}

inline bool DoubleLargerThan(const double & l, const double & r)
{
	return (l - r) > Recognition_CMP_MIN_DOUBLE;
}

inline bool DoubleSmallarThan(const double & l, const double & r)
{
	return (l - r) < -Recognition_CMP_MIN_DOUBLE;
}

// if some confliction found, define the Macto EQUAL_FUNC_CONFLICT
#ifndef EQUAL_FUNC_CONFLICT
#if ((!defined(EqualTo)) && (!defined(LargerThan)) && (!defined(SmallerThan)))
inline bool EqualTo(const double & l, const double & r)
{
	return DoubleEqual(l, r);
}

inline bool LargerThan(const double & l, const double & r)
{
	return DoubleLargerThan(l, r);
}

inline bool SmallerThan(const double & l, const double & r)
{
	return DoubleSmallarThan(l, r);
}

#endif
#endif

/**
 * @brief  round a float(double) number, keep f_amt numbers after the point
 * eg:  RoundTo(0.123456, 5) is 0.12346, RoundTo(0.123456, 2) is 0.12
 */
inline double RoundTo(const double & num, unsigned int f_amt)
{
	static double factors[] = {1, 1e1, 1e2, 1e3, 1e4, 1e6};
	double factor = 1e6;
	if(f_amt <= 6){
		factor = factors[f_amt];
	}else{
		for(unsigned int i=6; i < f_amt; ++i)
				factor*=10;
	}
	return ( static_cast<double>( static_cast<long>(num*factor+0.5)) / factor );
}


} // namespace Recognition


#endif
