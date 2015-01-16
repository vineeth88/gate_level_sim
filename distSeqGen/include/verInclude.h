#ifndef __VERI_INCL_H__
#define __VERI_INCL_H__

#if defined(__b01)
	#define benchCkt "b01"
	#define benchDir "b01/"
	#include "b01Int.h"

#elif defined(__b06)
	#define benchCkt "b06"
	#define benchDir "b06/"
	#include "b06Int.h"

#elif defined(__b07)
	#define benchDir "b07/"
	#define benchCkt "b07"
	#include "b07Int.h"

#elif defined(__b10)
	#define benchCkt "b10"
	#define benchDir "b10/"
	#include "b10Int.h"

#elif defined(__b11)
	#define benchCkt "b11"
	#define benchDir "b11/"
	#include "b11Int.h"

#elif defined(__b12)
	#define benchCkt "b12"
	#define benchDir "b12/"
	#define rstInput 1
	#include "b12Int.h"

#elif defined(__b14)
	#define benchCkt "b14"
	#define benchDir "b14/"
	#include "b14Int.h"

#elif defined(__or1200_0)
	#define benchCkt "or1200_0"
	#define benchDir "or1200_0/"
	#include "or1200_0Int.h"

#endif

#endif	// __VERI_INCL_H__
