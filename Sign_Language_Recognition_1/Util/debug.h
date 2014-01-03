#ifndef ZZ_DEBUG_TOOLS_FOR_Recognition_PROJECT_2008033121__H___
#define ZZ_DEBUG_TOOLS_FOR_Recognition_PROJECT_2008033121__H___
#pragma once
/**
 * @author  Thinapple (turingmc@gmail.com)
 */

/**
 * @brief If you want to insert some code for debuging, you will like them.
 */

template <int file, int line>
class ZZ_a_tTMpLete___UsEd_fOr_DeGUB__DoNOT_UseIt____
{
};

#ifdef NDEBUG

// Not debugging	

// in a function
#define ZZ_START_DEBUG_IN_FUNCTION { while(0){ 
#define ZZ_END_DEBUG_IN_FUNCTION }}

// not in any function
#define ZZ_START_DEBUG \
	template <> class  ZZ_a_tTMpLete___UsEd_fOr_DeGUB__DoNOT_UseIt____ \
	< sizeof(__FILE__), __LINE__> {

#define ZZ_END_DEBUG };

#else

// Debugging

#define ZZ_START_DEBUG_IN_FUNCTION
#define ZZ_END_DEBUG_IN_FUNCTION

#define ZZ_START_DEBUG 
#define ZZ_END_DEBUG 

#endif

// debug dialog
#include "./debug_dialog.h"

#endif

