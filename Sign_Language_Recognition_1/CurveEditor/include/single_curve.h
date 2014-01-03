/* by yw, June,2006 */
/* Last Modified: 2006/08/21 04:07:41 . */

#pragma once

#ifndef _SINGLE_CURVE_H_INCLUDED_
#define _SINGLE_CURVE_H_INCLUDED_

#include "base_curve.h"


/**
 * Template class representing a linear-curve.
 */
template <typename Type>
class linear_curve : public base_curve<Type>
{
	protected:

		Type m_startpos; /**< starting position of the curve. */

		/**
		 * The data of the left-end of curve, in the range of 0 to 1.0
		 */
		double m_dLeftData;

		/**
		 * The data of the right-end of the curve, in the range of 0 to 1.0
		 */
		double m_dRightData;

		/**
		 * The ending position of the curve.
		 */
		Type m_endpos;

	public:
		linear_curve();

		linear_curve( Type startpos, double leftdata, Type endpos, double rightdata );

		linear_curve ( const linear_curve &other);

		virtual ~linear_curve();

		const linear_curve& operator = ( const linear_curve &other);

		virtual bool isEqual (const linear_curve* pOther) const;

		virtual Type getLength (void) const;

		virtual void clear (void);

	protected:

		virtual double _getDataAt(Type position);


	public:

		/**
		 * Set the starting position.
		 */
		virtual void setStartPosition (Type pos);

		/**
		 * Return the starting position.
		 */
		virtual Type getStartPosition (void) const;

		/**
		 * Set the starting position.
		 */
		virtual void setEndPosition (Type pos);

		/**
		 * Return the starting position.
		 */
		virtual Type getEndPosition (void) const;

		/**
		 * Set the height for the left end of the curve.
		 */
		void setLeftData (double d);

		/**
		 * Get the height for the left end of the curve.
		 */
		double getLeftData(void) const;

		/**
		 * Set the height for the right end of the curve.
		 */
		void setRightData(double d);

		/**
		* Get the height for the right end of the curve.
		*/
		double getRightData(void) const;

}; /* ----- end of class  linear_curve  ----- */


template <typename Type>
class cubic_bezier_curve : public base_curve<Type>
{
protected:

	/**
	* The starting position.
	*/
	Type m_startpos;

	/**
	* The factor at the starting position.
	*/
	double m_dStartData;

	/**
	* The ending position.
	*/
	Type m_endpos;

	/**
	* The factor at the ending position.
	*/
	double m_dEndData;

	/**
	* Offset of 1st-control-point.
	*/
	double m_dControl1X, m_dControl1Y;

	/**
	* Offset of 2nd-control-point.
	*/
	double m_dControl2X, m_dControl2Y;


public:

	cubic_bezier_curve();

	cubic_bezier_curve( 
		Type startpos, double startData, 
		Type endpos, double endData,
		double Control1X, double Control1Y,
		double Control2X, double Control2Y
		);

	cubic_bezier_curve( const cubic_bezier_curve &other);

	virtual ~cubic_bezier_curve();

	const cubic_bezier_curve& operator = (const cubic_bezier_curve &other);

	virtual bool isEqual ( const cubic_bezier_curve* pOther) const;

	virtual Type getLength (void) const;

	virtual void clear (void);

protected:

	virtual double _getDataAt( Type position);

	/**
	* Given the value X, calculate a approximate parameter t 
	* using Newton's method, from the equation: 
	*
	*           3       2                  2           3
	*   X = X0 t  + X1 t (1-t) + X2 t (1-t)  + X3 (1-t) 
	*
	* where 
	*   X0 = m_startpos, X1 = m_dControl1X,
	*   X2 = m_dControl2X , X3 = m_endpos .
	*
	*/
	double _getParameterFromX(double X);

private:
	double _Cubic_Bezier_X (double tn);

	double _Cubic_Bezier_Y (double tn);

	double _root3(double d)
	{
		if (d < 0)
		{
			return - pow( -d , 1.0/3);
		}
		else
		{
			return pow (d, 1.0/3);
		}
	}


public:

	virtual void setStartPosition (Type pos);

	virtual Type getStartPosition(void) const;

	virtual void setEndPosition (Type pos);

	virtual Type getEndPosition (void) const;

	void setStartData (double d);

	double getStartData (void) const;

	void setEndData (double d);

	double getEndData (void) const;

	void setControl1X (double d);

	double getControl1X (void) const;

	void setControl1Y (double d);

	double getControl1Y (void) const;

	void setControl2X (double d);

	double getControl2X (void) const;

	void setControl2Y (double d);

	double getControl2Y (void) const;

}; /* ----- end of class  cubic_bezier_curve ----- */



/* ---- implement of class  linear_curve  ----- */

template < typename Type >
linear_curve<Type>::linear_curve ()
: m_startpos(0), m_dLeftData(0.5), m_endpos(0), m_dRightData(0.5)
{
}

template < typename Type >
linear_curve<Type>::linear_curve (Type startpos, double leftdata, Type endpos, double rightdata)
: m_startpos(startpos), m_dLeftData(leftdata), m_endpos(endpos), m_dRightData(rightdata)
{
}

template < typename Type >
linear_curve<Type>::linear_curve ( const linear_curve &other )
{
	m_startpos = other.m_startpos;
	m_dLeftData = other.m_dLeftData;
	m_endpos = other.m_endpos;
	m_dRightData = other.m_dRightData;
}

template < typename Type >
linear_curve<Type>::~linear_curve ()
{
}

template < typename Type >
const linear_curve<Type>&
linear_curve<Type>::operator = ( const linear_curve &other )
{
	if(this!=&other)
	{
		this->m_startpos = other.m_startpos;
		this->m_dLeftData = other.m_dLeftData;
		this->m_endpos = other.m_endpos;
		this->m_dRightData = other.m_dRightData;
	}
	return *this;
}

template < typename Type >
void linear_curve<Type>::clear (void)
{
	m_startpos = 0;
	m_dLeftData = 0;
	m_endpos = 0;
	m_dRightData = 0;
}

template < typename Type >
Type linear_curve<Type>::getLength (void) const
{
	return m_endpos - m_startpos;
}


template < typename Type >
void linear_curve<Type>::setStartPosition(Type pos)
{
	m_startpos = pos;
}

template < typename Type >
Type linear_curve<Type>::getStartPosition(void) const
{
	return m_startpos;
}

template < typename Type >
void linear_curve<Type>::setEndPosition(Type pos)
{
	m_endpos = pos;
}

template < typename Type >
Type linear_curve<Type>::getEndPosition(void) const
{
	return m_endpos;
}

template < typename Type >
void linear_curve<Type>::setLeftData (double d)
{
	m_dLeftData = d;
}

template < typename Type >
double linear_curve<Type>::getLeftData (void) const
{
	return m_dLeftData;
}

template < typename Type >
void linear_curve<Type>::setRightData (double d)
{
	m_dRightData = d;
}

template < typename Type >
double linear_curve<Type>::getRightData (void) const
{
	return m_dRightData;
}

template < typename Type >
bool linear_curve<Type>::isEqual ( const linear_curve * pOther ) const
{
	if (
		this->m_startpos == pOther->m_startpos
		&& this->m_dLeftData == pOther->m_dLeftData
		&& this->m_endpos == pOther->m_endpos
		&& this->m_dRightData == pOther->m_dRightData
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template < typename Type >
double linear_curve<Type>::_getDataAt (Type position)
{
	double factor = static_cast<double>(position - getStartPosition()) / getLength() ;
	double off = m_dRightData - m_dLeftData;
	return ( m_dLeftData + off * factor );
}

/* ---- end of implement of class  linear_curve  ----- */


/* ---- implement of class  cubic_bezier_curve ----- */
template < typename Type >
cubic_bezier_curve<Type>::cubic_bezier_curve ()
: m_startpos(0), m_dStartData(0.5),
m_endpos(0), m_dEndData(0.5),
m_dControl1X(0.1), m_dControl1Y(0.0),
m_dControl2X(-0.1), m_dControl2Y(0.0)
{
}

template < typename Type >
cubic_bezier_curve<Type>::cubic_bezier_curve( 
	Type startpos, double startData, 
	Type endpos, double endData,
	double Control1X, double Control1Y,
	double Control2X, double Control2Y
	)
	: m_startpos(startpos), m_dStartData(startData),
	m_endpos(endpos), m_dEndData(endData),
	m_dControl1X(Control1X), m_dControl1Y(Control1Y),
	m_dControl2X(Control2X), m_dControl2Y(Control2Y)
{
}

template < typename Type >
cubic_bezier_curve<Type>::cubic_bezier_curve ( const cubic_bezier_curve &other )
{
	m_startpos = other.m_startpos;
	m_dStartData = other.m_dStartData;
	m_endpos = other.m_endpos;
	m_dEndData = other.m_dEndData;
	m_dControl1X = other.m_dControl1X;
	m_dControl1Y = other.m_dControl1Y;
	m_dControl2X = other.m_dControl2X;
	m_dControl2Y = other.m_dControl2Y;
}

template < typename Type >
cubic_bezier_curve<Type>::~cubic_bezier_curve()
{
}

template < typename Type >
void cubic_bezier_curve<Type>::clear(void)
{
	m_startpos = 0;
	m_dStartData = 0;
	m_endpos = 0;
	m_dEndData = 0;
	m_dControl1X = 0;
	m_dControl1Y = 0;
	m_dControl2X = 0;
	m_dControl2Y = 0;
}

template < typename Type >
const cubic_bezier_curve<Type>&
cubic_bezier_curve<Type>::operator = ( const cubic_bezier_curve &other )
{
	if(this!= &other)
	{
		this->m_startpos = other.m_startpos;
		this->m_dStartData = other.m_dStartData;
		this->m_endpos = other.m_endpos;
		this->m_dEndData = other.m_dEndData;
		this->m_dControl1X = other.m_dControl1X;
		this->m_dControl1Y = other.m_dControl1Y;
		this->m_dControl2X = other.m_dControl2X;
		this->m_dControl2Y = other.m_dControl2Y;
	}
	return *this;
}

template < typename Type >
Type cubic_bezier_curve<Type>::getLength (void) const
{
	return m_endpos - m_startpos;
}

template < typename Type >
bool cubic_bezier_curve<Type>::isEqual ( const cubic_bezier_curve * pOther ) const
{
	if (
		this->m_startpos == pOther->m_startpos
		&& this->m_dStartData == pOther->m_dStartData
		&& this->m_endpos == pOther->m_endpos
		&& this->m_dEndData == pOther->m_dEndData
		&& this->m_dControl1X == pOther->m_dControl1X
		&& this->m_dControl1Y == pOther->m_dControl1Y
		&& this->m_dControl2X == pOther->m_dControl2X
		&& this->m_dControl2Y == pOther->m_dControl2Y
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template < typename Type >
void cubic_bezier_curve<Type>::setStartPosition (Type pos)
{
	m_startpos = pos;
}

template < typename Type >
Type cubic_bezier_curve<Type>::getStartPosition(void) const
{
	return m_startpos;
}

template < typename Type >
void cubic_bezier_curve<Type>::setEndPosition (Type pos)
{
	m_endpos = pos;
}

template < typename Type >
double cubic_bezier_curve<Type>::getStartData (void) const
{
	return m_dStartData;
}

template < typename Type >
void cubic_bezier_curve<Type>::setStartData (double d)
{
	m_dStartData = d;
}

template < typename Type >
double cubic_bezier_curve<Type>::getEndData (void) const
{
	return m_dEndData;
}

template < typename Type >
void cubic_bezier_curve<Type>::setEndData (double d)
{
	m_dEndData = d;
}

template < typename Type >
Type cubic_bezier_curve<Type>::getEndPosition (void) const
{
	return m_endpos;
}

template < typename Type >
void cubic_bezier_curve<Type>::setControl1X (double d)
{
	m_dControl1X = d;
}

template < typename Type >
double cubic_bezier_curve<Type>::getControl1X (void) const
{
	return m_dControl1X;
}


template < typename Type >
void cubic_bezier_curve<Type>::setControl1Y (double d)
{
	m_dControl1Y = d;
}

template < typename Type >
double cubic_bezier_curve<Type>::getControl1Y (void) const
{
	return m_dControl1Y;
}

template < typename Type >
void cubic_bezier_curve<Type>::setControl2X (double d)
{
	m_dControl2X = d;
}

template < typename Type >
double cubic_bezier_curve<Type>::getControl2X (void) const
{
	return m_dControl2X;
}
template < typename Type >
void cubic_bezier_curve<Type>::setControl2Y (double d)
{
	m_dControl2Y = d;
}

template < typename Type >
double cubic_bezier_curve<Type>::getControl2Y (void) const
{
	return m_dControl2Y;
}


/**
* Given the value X, calculate a approximate parameter t.
*
* Solve the equation: 
*
*       3       2                  2           3
*   X0 t  + X1 t (1-t) + X2 t (1-t)  + X3 (1-t)  - X = 0
*
* where 
*   X0 = m_startpos, X1 = m_dControl1X,
*   X2 = m_dControl2X, X3 = m_endpos .
*
*/
template < typename Type >
double cubic_bezier_curve<Type>::_getParameterFromX 
(double X)
{
	double x0, x1, x2, x3;

	x0 = m_startpos;
	x1 = m_dControl1X;
	x2 = m_dControl2X;
	x3 = m_endpos;

	double f, aa, bb, cc;
	double a, b, c;
	double p, q;
	double k, j;
	double delta, theta;
	double r_yy1, theta_yy1;

	double t; // result
	double t1, t2;
	double y1, y2, y3;

	f = - x0 + 3 * x1 - 3 * x2 + x3;
	aa = 3 * x0 - 6 * x1 + 3 * x2;
	bb = - 3 * x0 + 3 * x1;
	cc = x0 - X;

	if ( f != 0 ) 
	{
		a = aa / f;
		b = bb / f;
		c = cc / f;

		p = - a * a / 3 + b;
		q = 2 * a * a * a / 27 - a * b / 3 + c;

		delta = q * q /4 + p * p * p / 27;

		if ( abs(delta) < _double_Tolerance )
		{
			// three real roots, two of which are the same.
			y2 = _root3(q/2);
			y1 = - 2 * y2;

			if ( 0 <= y1 - a/3 && y1 - a/3 <= 1 )
			{
				t = y1 - a/3;
			}
			else
			{
				t = y2 - a/3;
			}
		}
		else if (delta > 0)
		{
			// only one real root.
			y1 = _root3( - q / 2 + sqrt( delta ))+ _root3 ( - q / 2 - sqrt( delta ));
			t = y1 - a / 3;
		} 
		else
		{
			// delta < 0
			// three different real roots.

			// let   yy1 = pow ( (-q/2 + sqrt (delta)), 1.0/3)
			// or,   yy1 = j + k (%i)
			k = sqrt ( - delta );
			j = - q /2;

			r_yy1 = _root3( sqrt (j*j + k*k));

			if ( j != 0 )
			{
				theta = atan (k/j);

				// when theta is not in (-pi/2, pi/2)
				if ( theta > 0 && j < 0 )
					theta = theta + 3.141592654;
				if ( theta < 0 && k > 0 )
					theta = theta + 3.141592654;
			}
			else 
			{
				// just on the imaginary axis.
				if ( k > 0 )
				{
					theta = 3.141592654 / 2;
				}
				else 
				{
					theta = 3 * 3.141592654 / 2;
				}
			}

			theta_yy1 = theta / 3;

			y1 = 2 * r_yy1 * cos (theta_yy1);
			y2 = 2 * r_yy1 * cos (theta_yy1 + 2 * 3.141592654 / 3);
			y3 = 2 * r_yy1 * cos (theta_yy1 + 4 * 3.141592654 / 3);

			if ( 0 <= y1 - a/3 && y1 - a/3 <= 1 )
			{
				t = y1 - a/3;
			}
			else if ( 0 <= y2 - a/3 && y2 - a/3 <= 1 )
			{
				t = y2 - a/3;
			}
			else
			{
				t = y3 - a/3;
			}

		}

	}
	else
	{
		// Quadratic
		if ( aa != 0 )
		{
			delta = bb * bb - 4 * aa * cc;

			t1 = (- bb + sqrt(delta)) / 2 * aa;
			t2 = (- bb - sqrt(delta)) / 2 * aa;

			if ( 0 <= t1 &&  t1 <=1 )
			{
				t = t1;
			}
			else
			{
				t = t2;
			}
		}
		else
		{
			// Linear
			t = - cc / bb;
		}
	}

	return t;
}


template < typename Type >
double cubic_bezier_curve<Type>::_Cubic_Bezier_X(double tn)
{
	double x0, x1, x2, x3;
	x0 = m_startpos;
	x1 = m_dControl1X;
	x2 = m_dControl2X;
	x3 = m_endpos;

double t_Squared, t_Cubed; // t^2,  t^3
	double tt, tt_Squared, tt_Cubed; // (1-t), (1-t)^2, (1-t)^3 
	double t_tt, t_t_tt, t_tt_tt; // t*(1-t), t*t*(1-t), t*(1-t)*(1-t)

	double f; // result;

	t_Squared = tn * tn;
		t_Cubed = t_Squared * tn;
		tt = 1 - tn;
		tt_Squared = tt * tt;
		tt_Cubed = tt_Squared * tt;
		t_tt = tn * tt;
		t_t_tt = t_tt * tn;
		t_tt_tt = t_tt * tt;
	
	f = x3 * t_Cubed + 3 * x2 * t_t_tt + 3 * x1 * t_tt_tt + x0 * tt_Cubed;

	return f;
}

template < typename Type >
double cubic_bezier_curve<Type>::_Cubic_Bezier_Y(double tn)
{
	double y0, y1, y2, y3;
	y0 = m_dStartData;
	y1 = m_dControl1Y;
	y2 = m_dControl2Y;
	y3 = m_dEndData;

	double t_Squared, t_Cubed; // t^2,  t^3
	double tt, tt_Squared, tt_Cubed; // (1-t), (1-t)^2, (1-t)^3 
	double t_tt, t_t_tt, t_tt_tt; // t*(1-t), t*t*(1-t), t*(1-t)*(1-t)

	double f; // result;

	t_Squared = tn * tn;
	t_Cubed = t_Squared * tn;
	tt = 1 - tn;
	tt_Squared = tt * tt;
	tt_Cubed = tt_Squared * tt;
	t_tt = tn * tt;
	t_t_tt = t_tt * tn;
	t_tt_tt = t_tt * tt;
	
	f = y3 * t_Cubed + 3 * y2 * t_t_tt + 3 * y1 * t_tt_tt + y0 * tt_Cubed;

	return f;
}

template < typename Type >
double cubic_bezier_curve<Type>::_getDataAt (Type position)
{

	double t, result;

	if (abs(position-getStartPosition())<_double_Tolerance)
		return _Cubic_Bezier_Y(0.0);

	if (abs(position-getEndPosition())<_double_Tolerance)
		return _Cubic_Bezier_Y(1.0);
	
	t = _getParameterFromX(position);

//	double temp = _Cubic_Bezier_X(t);
	result = _Cubic_Bezier_Y (t);

	return result;
}



/* ---- end of implement of class  cubic_bezier_curve ----- */
#endif // _SINGLE_CURVE_H_INCLUDED_

