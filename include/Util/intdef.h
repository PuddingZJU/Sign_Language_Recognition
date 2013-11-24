#ifndef  ZZ_Swift_INTEGER_BASIC_DEFINITIONS__H___
#define  ZZ_Swift_INTEGER_BASIC_DEFINITIONS__H___
/**
 * @brief	The definition of integer types
 *
 *			If you want a integer to be fixed length, you will like it ^_^
 * 
 * @author  Thinapple (turingmc@gmail.com)
 * 
 */
namespace Swift
{
;
#ifdef __GNUC__	///  < compiled in GNU Evironment
}
#include <stdint.h>
namespace Swift
{
#define	int8_t		int8_t
#define int16_t 	int16_t
#define int32_t		int32_t
#define int64_t		int64_t
#define uint8_t		uint8_t
#define uint16_t 	uint16_t
#define uint32_t	uint32_t
#define uint64_t	uint64_t
#define byte8_t		int8_t
#define ubyte8_t	uint8_t

#elif defined(_MSC_VER)	///   < compiled in MS VC++

#ifdef WIN32	///  < Win32 enviroment
#define int8_t		char
#define int16_t		short
#define	int32_t		int
#define int64_t		__int64
#define uint8_t		unsigned char
#define uint16_t	unsigned short
#define uint32_t	unsigned int
#define uint64_t	unsigned __int64
#define byte8_t		char
#define ubyte8_t	unsigned char

#elif 1
	// other Windows Environment
	#error "Unknown Windows Environment!"

#endif

#else
	
	#error "Unknown Compile Environment!"

#endif

#ifndef byte_t
#define byte_t		byte8_t
#endif
#ifndef ubyte_t
#define ubyte_t		ubyte8_t
#endif 

} // namespace Swift

#endif
