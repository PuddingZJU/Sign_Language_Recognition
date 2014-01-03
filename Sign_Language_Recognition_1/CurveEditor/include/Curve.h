/* by yw, June,2006 */
/* Last Modified: 2006/07/25 14:32:41 . */

#pragma once

#ifndef _CURVE_H_INCLUDED_
#define _CURVE_H_INCLUDED_

#include "single_curve.h"
#include "complex_curve.h"

typedef base_curve<double>			BaseCurve;
typedef linear_curve<double>		LinearCurve;
typedef cubic_bezier_curve<double>	CubicBezierCurve;
typedef complex_curve<double>		ComplexCurve;

typedef keypoint<double>			CurveKeyPoint;


typedef base_curve<int>				BaseCurveDiscrete;
typedef linear_curve<int>			LinearCurveDiscrete;
typedef cubic_bezier_curve<int>		CubicBezierCurveDiscrete;
typedef complex_curve<int>			ComplexCurveDiscrete;

typedef keypoint<int>				CurveKeyPointDiscrete;


#endif //_CURVE_H_INCLUDED_
