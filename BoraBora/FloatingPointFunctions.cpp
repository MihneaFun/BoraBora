//#pragma once
//
//#include "FloatingPointFunctions.h"
//#include <cmath>
//
//float calcDistanta(float a, float b) {
//	return abs(a - b);
//}
//
//float my_floor(float x) {
//	int Integer = x;
//	int NearestInteger = Integer - 1;
//	if (calcDistanta(Integer, x) < calcDistanta(NearestInteger, x)) {
//		NearestInteger = Integer;
//	}
//	if (calcDistanta(Integer + 1, x) < calcDistanta(NearestInteger, x)) {
//		NearestInteger = Integer + 1;
//	}
//	float floatNearestInteger = NearestInteger;
//	if (NearestInteger <= x) {
//		return NearestInteger;
//	}
//	return NearestInteger - 1;
//}
//
//float my_ceil(float x) {
//	int Integer = x;
//	int NearestInteger = Integer - 1;
//	if (calcDistanta(Integer, x) < calcDistanta(NearestInteger, x)) {
//		NearestInteger = Integer;
//	}
//	if (calcDistanta(Integer + 1, x) < calcDistanta(NearestInteger, x)) {
//		NearestInteger = Integer + 1;
//	}
//	float floatNearestInteger = NearestInteger;
//	if (x <= NearestInteger) {
//		return NearestInteger;
//	}
//	return NearestInteger + 1;
//}