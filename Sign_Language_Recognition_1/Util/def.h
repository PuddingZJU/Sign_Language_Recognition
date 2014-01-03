#ifndef  ZZ_Recognition_BASIC_DEFINITIONS__H___
#define  ZZ_Recognition_BASIC_DEFINITIONS__H___
#pragma once
/**
 * @author  Thinapple (turingmc@gmail.com)
 */

//  the basic types
#include "./intdef.h"
#include "./floatdef.h"

// Labels 
namespace Swift
{
;
// the null pointer
#ifndef NULL 
#define NULL (0)
#endif

// the OK return value
#ifndef RET_OK
#define RET_OK (0)
#endif

// indicate a class or a function that is already Thread-Safe
#ifdef Recognition_LABEL_THREAD_SAFE
#error "Some one is using Recognition_LABLE_THREAD_SAFE, so you can not -_-"
#else
#define Recognition_LABEL_THREAD_SAFE
#endif
// indicate a class or a function that can only run by one thread
#ifdef Recognition_LABEL_SINGLE_THREAD
#error "Some one is using Recognition_LABEL_SINGLE_THREAD, so you can not -_-"
#else
#define Recognition_LABEL_SINGLE_THREAD
#endif
//==> We have no idea now whether it is thread-saft for who has no label.
/*
 Usage: 
 For a class :
		Recognition_LABEL_THREAD_SAFE
		class MyClass 
		{
			...
		};

 For a function:
		Recognition_LABLE_SINGLE_THREAD
		void foo( Foo f, FOo o, FOO & oo)
		{ 
			...
		}
 */



} // namespace Recognition

#endif
