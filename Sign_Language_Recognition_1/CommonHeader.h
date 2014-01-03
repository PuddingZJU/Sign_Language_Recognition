#ifndef _COMMON_HEADER_H_INCLUDED_
#define _COMMON_HEADER_H_INCLUDED_

// disable some warnings

//"boost-1_33_1\boost\format\alt_sstream_impl.hpp(252) : warning C4996: 
//  'std::char_traits<wchar_t>::copy': Function call with parameters that
//   may be unsafe - this call relies on the caller to check that the 
//   passed values are correct. To disable this warning, 
//   use -D_SCL_SECURE_NO_WARNINGS. 
//   See documentation on how to use Visual C++ 'Checked Iterators' "
#define _SCL_SECURE_NO_WARNINGS

// "The file contains a character that 
// cannot be represented in the current code page (936) ."
#pragma warning( disable: 4819 ) 

#ifdef _DEBUG
	#pragma comment(lib, "Qt5Cored.lib" )
#else
	#pragma comment(lib, "Qt5Cored.lib" )
#endif

#endif //_COMMON_HEADER_H_INCLUDED_ 

