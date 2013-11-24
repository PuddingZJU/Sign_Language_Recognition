#ifndef  ZZ_Swift_BASIC_DEFINITIONS__H___
#define  ZZ_Swift_BASIC_DEFINITIONS__H___
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
#ifdef Swift_LABEL_THREAD_SAFE
#error "Some one is using Swift_LABLE_THREAD_SAFE, so you can not -_-"
#else
#define Swift_LABEL_THREAD_SAFE
#endif
// indicate a class or a function that can only run by one thread
#ifdef Swift_LABEL_SINGLE_THREAD
#error "Some one is using Swift_LABEL_SINGLE_THREAD, so you can not -_-"
#else
#define Swift_LABEL_SINGLE_THREAD
#endif
//==> We have no idea now whether it is thread-saft for who has no label.
/*
 Usage: 
 For a class :
		Swift_LABEL_THREAD_SAFE
		class MyClass 
		{
			...
		};

 For a function:
		Swift_LABLE_SINGLE_THREAD
		void foo( Foo f, FOo o, FOO & oo)
		{ 
			...
		}
 */



} // namespace Swift

#endif
