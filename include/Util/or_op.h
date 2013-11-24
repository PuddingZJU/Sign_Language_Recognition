#ifndef ZZ_THE_TAGS_CHECKED_BY_BITWISE_OR_AND_BITWISE_AND_2008032622__H___
#define ZZ_THE_TAGS_CHECKED_BY_BITWISE_OR_AND_BITWISE_AND_2008032622__H___
#pragma once
/**
 * @author  Thinapple (turingmc@gmail.com)
 */
namespace Swift
{
;
namespace OR_OP
{
;

template <typename IntType>
inline bool isSet(IntType tag, IntType var)
{
	return ( 0 != (tag & var) );
}

template <typename IntType>
inline void set(IntType tag, IntType & var)
{
	var = static_cast<IntType>( var | tag );
	return ;
}

template <typename IntType>
inline void unset(IntType tag, IntType & var)
{
	var = static_cast<IntType>( var & ~tag);
	return ;
}

template <typename IntType>
inline bool checkAndSet(IntType tag, IntType & var)
{
	bool ret = isSet(tag, var)
	set(tag, var);
	return ret;
}

} // namespace OR_OP
} // namespace Swift
#endif
