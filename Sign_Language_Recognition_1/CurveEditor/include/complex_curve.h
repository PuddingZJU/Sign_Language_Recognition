/* by yw, June,2006 */
/* Last Modified: 2006/08/21 04:15:27 . */

#ifndef _COMPLEX_CURVE_H_INCLUDED_
#define _COMPLEX_CURVE_H_INCLUDED_

#include "base_curve.h"
#include "single_curve.h"

#include <vector>
//#include <algorithm>
//#include <functional>


/**
 * Template class representing a keypoint on curve, with data of 2 control points.
 */
template <typename Type>
class keypoint
{
    public:
        enum key_point_type_t
		{
			Ignored = 0,
			//Connected,
			Linear,
			Smooth
		};

	protected:
		enum key_point_type_t in_type, out_type;

		Type m_position;
		double m_data;

		double dControlPoint1X, dControlPoint1Y;
		double dControlPoint2X, dControlPoint2Y;

	public:
		keypoint()
			: in_type ( /*key_point_type_t::*/Ignored ), out_type ( /*key_point_type_t::*/Ignored ) , m_position ( 0 )
		{
		}

		keypoint( Type position )
			: in_type ( /*key_point_type_t::*/Ignored ), out_type ( /*key_point_type_t::*/Ignored ), m_position ( position )
		{
		}

		keypoint ( key_point_type_t in, key_point_type_t out, Type position, double data)
			: in_type ( in ), out_type ( out ), m_position ( position ), m_data ( data )
		{
		}

		key_point_type_t getInType(void) const
		{
			return in_type;
		}

		key_point_type_t getOutType(void) const
		{
			return out_type;
		}

		void setInType( key_point_type_t in )
		{
			in_type = in;
		}

		void setOutType( key_point_type_t out )
		{
			out_type = out;
		}

		Type getPosition (void) const
		{
			return m_position;
		}

		void setPosition (Type position)
		{
			m_position = position;
		}

		double getData (void) const
		{
			return m_data;
		}

		void setData (double data)
		{
			m_data = data;
		}

		double getControl1X (void) const
		{
			return dControlPoint1X;
		}

		void setControl1X (double x)
		{
			dControlPoint1X = x;
		}

		double getControl1Y (void) const
		{
			return dControlPoint1Y;
		}

		void setControl1Y (double y)
		{
			dControlPoint1Y = y;
		}

		double getControl2X (void) const
		{
			return dControlPoint2X;
		}

		void setControl2X (double x)
		{
			dControlPoint2X = x;
		}

		double getControl2Y (void) const
		{
			return dControlPoint2Y;
		}

		void setControl2Y (double y)
		{
			dControlPoint2Y = y;
		}

		bool isEqual(const keypoint& other) const
		{
			if ( in_type != other.in_type )
				return false;
			if ( out_type != other.out_type )
				return false;
			if ( m_position != other.m_position )
				return false;
			if ( ! ( m_data - other.m_data < _double_Tolerance
						&& m_data - other.m_data > - _double_Tolerance))
				return false;

			if ( ! ( dControlPoint1X - other.dControlPoint1X < _double_Tolerance
						&& dControlPoint1X - other.dControlPoint1X > - _double_Tolerance))
				return false;
			if ( ! ( dControlPoint1Y - other.dControlPoint1Y < _double_Tolerance
						&& dControlPoint1Y - other.dControlPoint1Y > - _double_Tolerance))
				return false;
			if ( ! ( dControlPoint2X - other.dControlPoint2X < _double_Tolerance
						&& dControlPoint2X - other.dControlPoint2X > - _double_Tolerance))
				return false;
			if ( ! ( dControlPoint2Y - other.dControlPoint2Y < _double_Tolerance
						&& dControlPoint2Y - other.dControlPoint2Y > - _double_Tolerance))
				return false;


			return true;
		}

		bool operator== (const keypoint& other) const
		{
			return isEqual(other);
		}

		bool operator!= (const keypoint& other) const
		{
			return ! isEqual(other);
		}

		bool operator< (const keypoint& other) const
		{
			return this->m_position < other.m_position;
		}

		void clear()
		{
			this->in_type = Ignored;
			this->out_type = Ignored;
			this->m_position = 0;
			this->m_data = 0;
			this->dControlPoint1X = 0;
			this->dControlPoint1Y = 0;
			this->dControlPoint2X = 0;
			this->dControlPoint2Y = 0;
		}
};

/**
 * Template class representing a complex-curve, which is composed of pieces of curves.
 */
template <typename Type>
class complex_curve : public base_curve<Type>
{


	class curve_record
	{
		public:
			base_curve<Type> * pCurve;
			bool bNeedUpdate;

			curve_record()
				: bNeedUpdate(true)
			{
				pCurve = NULL;
			}

			~curve_record()
			{
				if (pCurve)
					delete pCurve;
			}
	};



	public:
	std::vector< keypoint<Type> > m_keypoints;

	protected:
	std::vector< curve_record > m_curve_pieces;

	public:
	complex_curve();

	complex_curve( keypoint<Type> k );

	complex_curve ( const complex_curve &other );

	virtual ~complex_curve();

	const complex_curve& operator = ( const complex_curve &other);

	virtual bool isEqual (const complex_curve* pOther) const;

	virtual Type getLength (void) const;

	virtual void clear (void);

	/**
	 * Set the starting position.
	 */
	virtual void setStartPosition (Type pos);

	/**
	 * Return the starting position.
	 */
	virtual Type getStartPosition (void) const;


	protected:
	virtual double _getDataAt(Type position);

	void _updateCurve (size_t index);

	double _getDataOnCurve (size_t index, Type position);

	public:

	size_t getPointCount(void) const;

	void add_keypoint(keypoint<Type> k);

	void delete_curve_at_position(Type pos, bool bLeftKeypointRemain = true);

	void delete_keypoint (size_t index);

	keypoint<Type> get_keypoint(size_t index) const;

	void set_keypoint (keypoint<Type> k, size_t index);

}; /* ----- end of class  complex_curve  ----- */


/* ---- implement of class  complex_curve  ----- */

template < typename Type >
complex_curve<Type>::complex_curve ()
{
	keypoint<Type> k( 0 );
	k.setData(0.5);

	m_keypoints.reserve(4);
	m_curve_pieces.reserve(4);

	if (m_keypoints.size() < 1)
	{
		m_keypoints.push_back( k );
	}
	else
	{
		m_keypoints[0] = k;
	}
}

template < typename Type >
complex_curve<Type>::complex_curve ( keypoint<Type> k )
{
	m_keypoints.reserve(4);
	m_curve_pieces.reserve(4);

	if (m_keypoints.size() < 1)
	{
		m_keypoints.push_back( k );
	}
	else
	{
		m_keypoints[0] = k;
	}
}

template < typename Type >
complex_curve<Type>::complex_curve ( const complex_curve &other )
{
	//m_startpos = other.m_startpos;
	m_keypoints.assign( other.m_keypoints.begin(), other.m_keypoints.end() );

	curve_record data;
	data.bNeedUpdate = true;
	size_t c = m_keypoints.size() - 1;
	m_curve_pieces.assign( c, data );

}

template < typename Type >
complex_curve<Type>::~complex_curve ()
{
	//m_curve_pieces.~vector<curve_record>();
}

template < typename Type >
const complex_curve<Type>&
complex_curve<Type>::operator = ( const complex_curve &other )
{
  if(this!=&other)
  {
	  this->m_keypoints.assign (other.m_keypoints.begin(), other.m_keypoints.end() );

	  curve_record data;
	  data.bNeedUpdate = true;
	  size_t c = this->m_keypoints.size() - 1;
	  this->m_curve_pieces.assign( c, data );
  }
  return *this;
}

template < typename Type >
void complex_curve<Type>::clear(void)
{
	m_keypoints.clear();
	m_curve_pieces.clear();
}

template < typename Type >
Type complex_curve<Type>::getLength (void) const
{
	return m_keypoints.at( m_keypoints.size() - 1 ).getPosition()
		- m_keypoints.at(0).getPosition() ;
}

template < typename Type >
void complex_curve<Type>::setStartPosition (Type pos)
{
	keypoint<Type> k (pos);
	m_keypoints[0] = k;
}

template < typename Type >
Type complex_curve<Type>::getStartPosition (void) const
{
	return m_keypoints.at(0).getPosition();
}



template < typename Type >
bool complex_curve<Type>::isEqual ( const complex_curve * pOther ) const
{
	if ( ! (this->getLength() == pOther->getLength() ) )
	{
		return false;
	}

	size_t c = m_keypoints.size();
	size_t cc = pOther->m_keypoints.size();

	if ( c != cc )
		return false;

	for (size_t i=0; i<c; ++i)
	{
		if (m_keypoints.at(i) != pOther->m_keypoints.at(i))
			return false;
	}

	return true;
}

template < typename Type >
double complex_curve<Type>::_getDataAt (Type position)
{
//	typename std::vector< keypoint<Type> >::iterator it
//        = std::find_if (
//            m_keypoints.begin(), m_keypoints.end(),
//            std::bind2nd( std::greater<Type>(), position )
//            );

	typename std::vector< keypoint<Type> >::iterator it;
    for (it = m_keypoints.begin();
        it != m_keypoints.end(); ++it)
    {
		if ( (*it).getPosition() > position ) 
		{
			break;
		}
    }

	if (it == m_keypoints.end())
	{
		//if ((position - (this->getStartPosition() + this->getLength())) < _double_Tolerance)
		
		// range has already been checked in getDataAt() in class base_curve,
		// so it shoule be the end point.

		return m_keypoints.at(m_keypoints.size()-1).getData();
		
	}
	size_t index = it - m_keypoints.begin();

	return _getDataOnCurve (index-1, position /*(position - m_keypoints.at(index-1).getPosition() )*/ );

}

template < typename Type >
void complex_curve<Type>::_updateCurve (size_t index)
{
	base_curve<Type> * pc;

	// create a new one
	typename keypoint<Type>::key_point_type_t typeStart, typeEnd;
	typeStart = m_keypoints.at(index).getOutType();
	typeEnd   = m_keypoints.at(index+1).getInType();

	int countIgnored = 0, countLinear = 0, countSmooth = 0;
	switch (typeStart)
	{
		case keypoint<Type>::/*key_point_type_t::*/Ignored:
			countIgnored ++;
			break;
		case keypoint<Type>::/*key_point_type_t::*/Linear:
			countLinear ++;
			break;
		case keypoint<Type>::/*key_point_type_t::*/Smooth:
			countSmooth ++;
			break;
		default:
			throw std::exception(); // ("Unknown keypoint type encounted.");
	}

	switch (typeEnd)
	{
		case keypoint<Type>::/*key_point_type_t::*/Ignored:
			countIgnored ++;
			break;
		case keypoint<Type>::/*key_point_type_t::*/Linear:
			countLinear ++;
			break;
		case keypoint<Type>::/*key_point_type_t::*/Smooth:
			countSmooth ++;
			break;
		default:
			throw std::exception(); // ("Unknown keypoint type encounted.");
	}


	if (! m_curve_pieces.at(index).pCurve )
	{
		if ( 2 == countIgnored )
		{
			// a linear curve that exactly on x axis, or rather, all-zero.
			pc = new linear_curve<Type> (
					m_keypoints.at(index).getPosition(),
					0.0,
					m_keypoints.at(index+1).getPosition(),
					0.0
					);
		}
		else if ( 0 == countSmooth && countLinear > 0 )
		{
			// connect two keypoints using linear curve.
			pc = new linear_curve<Type> (
					m_keypoints.at(index).getPosition(),
					m_keypoints.at(index).getData(),
					m_keypoints.at(index+1).getPosition(),
					m_keypoints.at(index+1).getData()
					);
		}
		else if ( countSmooth > 0 )
		{
			// TODO : Bezier curve.
			pc = new cubic_bezier_curve<Type> (
				m_keypoints.at(index).getPosition(),
				m_keypoints.at(index).getData(),
				m_keypoints.at(index+1).getPosition(),
				m_keypoints.at(index+1).getData(),
				m_keypoints.at(index).getControl2X(),
				m_keypoints.at(index).getControl2Y(),
				m_keypoints.at(index+1).getControl1X(),
				m_keypoints.at(index+1).getControl1Y()					
				);		
			
		}

		m_curve_pieces.at(index).pCurve = pc;

	}
	else
	{
		// curve existed. Check the curve-type.
		// RTTI required.

		if ( 2 == countIgnored )
		{
			// a linear curve that exactly on x axis, or rather, all-zero.

			if ( typeid(m_curve_pieces.at(index).pCurve)
					== typeid(linear_curve<Type>) )
			{
				linear_curve<Type>* pc;
				pc = dynamic_cast< linear_curve<Type>* >(
						m_curve_pieces.at(index).pCurve
						);
				pc->setStartPosition(
						m_keypoints.at(index).getPosition()
						);
				pc->setLeftData( 0.0 );

				pc->setEndPosition(
						m_keypoints.at(index+1).getPosition()
						);
				pc->setRightData( 0.0 );

			}
			else
			{
				delete m_curve_pieces.at(index).pCurve;

				m_curve_pieces.at(index).pCurve = new linear_curve<Type> (
						m_keypoints.at(index).getPosition(),
						0.0,
						m_keypoints.at(index+1).getPosition(),
						0.0
						);
			}
		}
		else if ( 0 == countSmooth && countLinear > 0 )
		{
			// connect two keypoints using linear curve.
			if ( typeid(m_curve_pieces.at(index).pCurve)
					== typeid ( linear_curve<Type> ))
			{
			    linear_curve<Type>* pc;
			    pc = dynamic_cast< linear_curve<Type>* >(
                    m_curve_pieces.at(index).pCurve
					);
				pc->setStartPosition(
						m_keypoints.at(index).getPosition()
						);
				pc->setLeftData(
						m_keypoints.at(index).getData()
						);
				pc->setEndPosition(
						m_keypoints.at(index+1).getPosition()
						);
				pc->setRightData(
						m_keypoints.at(index+1).getData()
						);
			}
			else
			{
				delete m_curve_pieces.at(index).pCurve;

				m_curve_pieces.at(index).pCurve = new linear_curve<Type> (
						m_keypoints.at(index).getPosition(),
						m_keypoints.at(index).getData(),
						m_keypoints.at(index+1).getPosition(),
						m_keypoints.at(index+1).getData()
						);
			}
		}
		else if ( countSmooth > 0 )
		{
			// connect two keypoints using cubic_Bezier curve.


			if ( typeid (m_curve_pieces.at(index).pCurve)
				== typeid ( cubic_bezier_curve<Type> ))
			{	
				cubic_bezier_curve<Type>* pc;
				pc = dynamic_cast < cubic_bezier_curve<Type>* >(
					m_curve_pieces.at(index).pCurve
					);

				pc->setStartPosition(
					m_keypoints.at(index).getPosition()
					);
				pc->setStartData(
					m_keypoints.at(index).getData()
					);
				pc->setEndPosition(
					m_keypoints.at(index+1).getPosition()
					);
				pc->setEndData(
					m_keypoints.at(index+1).getData()
					);
				pc->setControl1X(
					m_keypoints.at(index).getControl2X()
					);
				pc->setControl1Y(
					m_keypoints.at(index).getControl2Y()
					);
				pc->setControl2X(
					m_keypoints.at(index+1).getControl1X()
					);
				pc->setControl2Y(
					m_keypoints.at(index+1).getControl1Y()
					);
			}
			else
			{
				delete m_curve_pieces.at(index).pCurve;

				m_curve_pieces.at(index).pCurve = new cubic_bezier_curve<Type> (
					m_keypoints.at(index).getPosition(),
					m_keypoints.at(index).getData(),
					m_keypoints.at(index+1).getPosition(),
					m_keypoints.at(index+1).getData(),
					m_keypoints.at(index).getControl2X(),
					m_keypoints.at(index).getControl2Y(),
					m_keypoints.at(index+1).getControl1X(),
					m_keypoints.at(index+1).getControl1Y()					
					);								
			}
		}
	}

	m_curve_pieces.at(index).bNeedUpdate = false;
}


template < typename Type >
double complex_curve<Type>::_getDataOnCurve (size_t index, Type position)
{
	if (m_curve_pieces.size() < 1 || m_curve_pieces.at(index).bNeedUpdate)
	{
		_updateCurve (index);
	}

	return m_curve_pieces.at(index).pCurve->getDataAt( position );

}

template < typename Type >
size_t complex_curve<Type>::getPointCount(void) const
{
	return m_keypoints.size();
}

template < typename Type >
void complex_curve<Type>::add_keypoint (keypoint<Type> k)
{
//	typename std::vector<keypoint<Type> >::iterator it =
//		std::find_if (
//				m_keypoints.begin(), m_keypoints.end(),
//				std::bind2nd( std::greater<Type>(), k.getPosition() )
//				);

	typename std::vector< keypoint<Type> >::iterator it;
    for (it = m_keypoints.begin();
        it != m_keypoints.end(); ++it)
    {
		if ( (*it).getPosition() > k.getPosition() )
		{
			break;
		}
    }

	if (it == m_keypoints.end())
	{
		m_keypoints.push_back( k );
		curve_record rec;
		m_curve_pieces.push_back (rec);
	}
	else
	{
		size_t index = it - m_keypoints.begin();

		m_keypoints.insert( it, k );
		if (index > 0){
			m_curve_pieces.at(index-1).bNeedUpdate = true;
		}

		curve_record rec;
		m_curve_pieces.insert( m_curve_pieces.begin()+index, rec );
	}
}

template < typename Type >
void complex_curve<Type>::delete_curve_at_position (Type pos, bool bLeftKeypointRemain /* = true */)
{
//	typename std::vector<keypoint<Type> >::iterator it =
//		std::find_if (
//				m_keypoints.begin(), m_keypoints.end(),
//				std::bind2nd( std::greater<Type>(), pos )
//				);

	typename std::vector< keypoint<Type> >::iterator it;
    for (it = m_keypoints.begin();
        it != m_keypoints.end(); ++it)
    {
		if ( (*it).getPosition() > pos )
		{
			break;
		}
    }

	if (it == m_keypoints.end())
	{
		return ; // nothing to delete
	}
	else
	{
		size_t index = it - m_keypoints.begin();

		if (bLeftKeypointRemain)
		{
			if (index < m_curve_pieces.size())
			{
				m_curve_pieces.at (index).bNeedUpdate = true;
			}
			m_curve_pieces.erase( m_curve_pieces.begin() + index - 1);

			m_keypoints.erase(it);
		}
		else
		{
			if(index >= 2)
			{
				m_curve_pieces.at (index -2).bNeedUpdate = true;
			}
			m_curve_pieces.erase( m_curve_pieces.begin() + index - 1);

			m_keypoints.erase(it-1);
		}

	}
}

template < typename Type >
void complex_curve<Type>::delete_keypoint (size_t index)
{
	if (index == 0)
	{
		m_keypoints.erase( m_keypoints.begin() );
		m_curve_pieces.erase( m_curve_pieces.begin() );
	}
	else if ( index == m_keypoints.size()-1)
	{
		m_keypoints.pop_back();
		m_curve_pieces.pop_back();
	}
	else
	{
		// in the middle
		m_curve_pieces.at(index-1).bNeedUpdate = true;
		m_curve_pieces.erase (m_curve_pieces.begin()+index);

		m_keypoints.erase(m_keypoints.begin()+index);
	}
}

template < typename Type >
keypoint<Type> complex_curve<Type>::get_keypoint(size_t index) const
{
	return m_keypoints.at(index);
}

template < typename Type >
void complex_curve<Type>::set_keypoint(keypoint<Type> k, size_t index)
{
	m_keypoints.at(index) = k;

	if (index != 0)
	{
		m_curve_pieces.at(index -1).bNeedUpdate = true;
	}

	if (index != m_keypoints.size() -1)
	{
		m_curve_pieces.at(index).bNeedUpdate = true;
	}	

}

/* ----- end of implement of class  complex_curve  ----- */

#endif // _COMPLEX_CURVE_H_INCLUDED_

