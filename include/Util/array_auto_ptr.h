#ifndef ZZ_AUTO_PTR_FOR_ARRAY_POINTERS_LIKE_STD_AUTO_PTR__H___
#define ZZ_AUTO_PTR_FOR_ARRAY_POINTERS_LIKE_STD_AUTO_PTR__H___
#pragma once
/**
 * @author  Thinapple (turingmc@gmail.com)
 */

#include <memory>

namespace Swift
{
;
/**
 * @brief auto_ptr for array pointers
 *
 *	It has the same interface as the std::auto_ptr
 */
template <typename T>
class array_auto_ptr : public std::auto_ptr<T>
{
public:
	array_auto_ptr(T * p = 0) throw() : auto_ptr<T>(p) {} ;
	~array_auto_ptr() throw () { delete [] (release()); };
private:
    array_auto_ptr(const array_auto_ptr & );
    const array_auto_ptr & operator = (const array_auto_ptr & );
public:
	const T & operator[] (size_t i) const throw()
	{
		return get()[i];
	}

	T & operator[] (size_t i) throw()
	{
		return get()[i];
	}

    void reset(T * p = 0) throw()
    {
        if( p == get()){
            return ;
        }
        delete [] (release());
        auto_ptr<T>::reset(p);
    }
};

} // namespace Swift
#endif
