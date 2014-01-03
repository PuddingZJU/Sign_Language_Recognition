/* by yw, June,2006 */
/* Last Modified: 2006/07/29 14:46:09 . */

#pragma once 

#ifndef _BASE_CURVE_H_INCLUDED_
#define _BASE_CURVE_H_INCLUDED_


#include <exception>
#include <typeinfo>
#include <math.h>

const double _double_Tolerance = 0.000001 ;


/**
 * Base template class for representing a curve. \n Given the curve-length and something else, we can get the factor at certain position in the curve.
 */
template < typename Type >
class base_curve
{

	public:

		/**
		 * Default constructor.
		 */
		base_curve ();

		/**
		 * Copy constructor.
		 */
		base_curve ( const base_curve &other );

		/**
		 * Destructor.
		 */
		virtual ~base_curve ();

		/**
		 * Assignment operator.
		 */
		const base_curve& operator = ( const base_curve &other );

		/**
		 * Return the length of the curve.
		 */
		virtual Type getLength ( void ) const = 0;

		/**
		 * Set the starting position.
		 */
		virtual void setStartPosition (Type pos) = 0;

		/**
		 * Return the starting position.
		 */
		virtual Type getStartPosition (void) const = 0;

		/**
		 * Check to see if 2 curves are the same.
		 */
		virtual bool isEqual ( const base_curve* pOther ) const;

		/**
		 * Return the data (blending factor) at given position.
		 */
		double getDataAt ( Type position );

		virtual void clear( void ) = 0;

	protected:

		virtual double _getDataAt(Type position)=0;

		void _out_of_range_down_handling(Type position) /* position smaller then starting position */
		{
			throw std::exception(); // /*::out_of_range*/("position smaller then starting position.");
		}
		void _out_of_range_up_handling(Type position) /* position pass end of curve */
		{
			throw std::exception(); // /*::out_of_range*/("position pass end of curve.");
		}

}; /* -----  end of class  base_curve  ----- */


/* ----- implement of class  base_curve ----- */

template < typename Type >
base_curve<Type>::base_curve ()
{
}

template < typename Type >
base_curve<Type>::base_curve ( const base_curve &other )
{
}

template < typename Type >
base_curve<Type>::~base_curve ()
{
}

template < typename Type >
const base_curve<Type>&
base_curve<Type>::operator = ( const base_curve &other )
{
  if(this!=&other)
  {
  }
  return *this;
}

template < typename Type >
bool base_curve<Type>::isEqual ( const base_curve * pOther ) const
{
	if ( this->getStartPosition() == pOther->getStartPosition() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

template < typename Type >
double base_curve<Type>::getDataAt ( Type position )
{
	if ( getStartPosition() - position > _double_Tolerance )
	{
		// position is smaller then starting position
		// do something if necessary.
		_out_of_range_up_handling (position);
	}

	if ( position - getStartPosition() - getLength() > _double_Tolerance )
	{
		// pass the end of the curve.
		// do something if necessary.
		_out_of_range_down_handling (position);
	}

	return _getDataAt( position );
}


/* ----- end of implement of class  base_curve ----- */

#endif // _BASE_CURVE_H_INCLUDED_
