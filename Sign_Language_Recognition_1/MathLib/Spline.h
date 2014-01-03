#ifndef ZZ_CUBIC_SPLINE_2008032719__H___
#define ZZ_CUBIC_SPLINE_2008032719__H___
#pragma once
/**
 * @author Thinapple (turingmc@gmail.com)
 */
#include <vector>
namespace Swift
{
;
/**
 * @brief Cubic Spline interpolation
 *
 * 	It refers to William H. Press's Numerical Recipes in C++ 2nd Edition
 *	Usage:  1 - input data by calling push_back#() or set#()
 *		    2 - do prepare()
 *		    3 - call Y = line_spline(X) to get the interpolated value
 */
template <typename DataType>
class CubicSpline
{
public:
	CubicSpline(){} //
	~CubicSpline()
	{
		clear_all();

		return ;
	} //
private:
	CubicSpline(const CubicSpline &);
	const CubicSpline & operator = (const CubicSpline & );
public:
	typedef typename std::vector<DataType> DataList;
public:
	// input data
	void clear_all()
	{
		m_x_data.clear();
		m_y_data.clear();
		m_d2.clear();

		return ;
	}
	 
	void push_backX(DataType x){ m_x_data.push_back(x); }
	void setX(const DataList & x_vals) { m_x_data = x_vals; }
	void setX(const DataType x_vals[], unsigned int n)
	{
		do_data_input( x_vals, n, m_x_data);
	}
	void push_backY(DataType y) { m_y_data.push_back(y); }
	void setY(const DataList & y_vals) { m_y_data = y_vals; }
	void setY(const DataType y_vals[], unsigned int n)
	{
		do_data_input( y_vals, n, m_y_data );
	}
	// do prepare
	int prepare()
	{
		size_t size_x = m_x_data.size();
		size_t size_y = m_y_data.size();
		if( size_x < 2 || size_y < 2){
			return -1;
		}
		return prepare( (m_y_data[1]-m_y_data[0])/(m_x_data[1]-m_x_data[0]),
						(m_y_data[size_y-1]-m_y_data[size_y-2]) / (m_x_data[size_x-1]-m_x_data[size_x-2]) );
	}
	int prepare(const DataType d1, const DataType dn)
	{
		return compute_d2( m_x_data, m_y_data, m_d2, d1, dn );
	}

	// do interpolation
	DataType line_spline(const DataType x) const
	{
		return do_spline(m_x_data, m_y_data, m_d2, x);
	}

private:
	void do_data_input(const DataType data[], unsigned int n, DataList & list)
	{
		list.clear();
		for(unsigned int i = 0; i < n; ++i){
			list.push_back(data[i]);
		}

		return ;
	} // do_data_input(data, n, list)

	int compute_d2(const DataList & xs, const DataList & ys, DataList & ds,
			const DataType d1, const DataType dn)
	{
		size_t size = xs.size();

		DataList u(size-1);
		ds.resize(size);

		if(d1 > 0.99e99){
			ds[0] = u[0] = 0;
		}else{
			ds[0] = -0.5;
			u[0] = ( 3.0/(xs[1]-xs[0]) ) * ( (ys[1]-ys[0])/(xs[1]-xs[0]) - d1 );
		}
		
		for(size_t i = 1; i < size-1; ++i){
			DataType sig = (xs[i] - xs[i-1]) / (xs[i+1] - xs[i]);
			DataType p = sig * ds[i-1] + 2;
			ds[i] = (sig - 1.0) / p;
			u[i] = (ys[i+1]-ys[i])/(xs[i+1]-xs[i]) - (ys[i]-ys[i-1])/(xs[i]-xs[i-1]);
			u[i] = (6.0*u[i]/(xs[i+1]-xs[i-1]) - sig*u[i-1]) / p;
		}

		DataType qn(0.5), un(0);
		if(dn > 0.99e99){
			qn = un = 0;
		}else{
			qn = 0.5;
			un = ( 3.0/(xs[size-1]-xs[size-2]) ) 
					* ( dn - (ys[size-1]-ys[size-2])/(xs[size-1]-xs[size-2]) );
		}

		ds[size-1] = (un - qn*u[size-2]) / (qn*ds[size-2] + 1.0);
		for(int i = size-2; i >= 0; --i){
			ds[i] = ds[i]*ds[i+1] + u[i];
		}

		return 0;
	} // compute_d2(x, y, d, d1, dn)

	DataType do_spline(const DataList & xs, const DataList & ys,
					const DataList & ds, const DataType x) const
	{
		DataType h, b, a;
		size_t size = xs.size();
		
		size_t low = 0;
		size_t high = size - 1;
		size_t mid(0);
		while( high - low > 1 ){
			mid = (high + low) >> 1;
			if(xs[mid] > x)
				high = mid;
			else
				low = mid;
		}
		h = xs[high] - xs[low];
		if(0.0 == h) throw ( "bad input" );
		a = (xs[high] - x) / h;
		b = (x - xs[low]) / h;
		
		return 	  a * ys[low]
				+ b * ys[high]
				+ ( (a*a*a - a)*ds[low] + (b*b*b - b)*ds[high] ) * (h*h)/6.0;
	} // do_spline( x )
protected:
	DataList	m_x_data;  ///< the x's of f(x)
	DataList	m_y_data;  ///< the y's of y = f(x)
	DataList	m_d2;   ///< the second derivatives
};

} // namespace Swift
#endif
