


#ifndef MATH_H
#define MATH_H

#define PI 3.141592653589793238f
#define Square(x) ((x)*(x))

#include <math.h>
inline f32 Cos(f32 x){
	f32 result = cos(x);
	return result;
}
inline f32 Sin(f32 x){
	f32 result = sin(x);
	return result;
}
inline f32 SquareRoot(f32 x){
	f32 result = sqrt(x);
	return result;
}
inline f32 SquareRootFast(f32 x){
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	
	x2 = x * 0.5F;
	y  = x;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );
	return x * y;
}

inline f32 Lerp(f32 a, f32 b, f32 t){
	f32 result = (1.f - t)*a + b*t;
	return result;
}
inline f32 SLerp(f32 a, f32 b, f32 t){
	f32 result = Lerp(a, b, 6*t*t*t*t*t - 15*t*t*t*t + 10*t*t*t);
	return result;
}
inline f32 Smoothstep(f32 x){
	Assert(x >= 0.0f && x <= 1.0f);
	f32 y = x*x*(3.0f - 2.0f*x);
	return y;
}
inline f32 SmoothstepLerp(f32 a, f32 b, f32 t){
	f32 result = Lerp(a, b, Smoothstep(t));
	return result;
}

inline f32 ModF32(f32 x, f32 modBy){
	Assert(modBy >= 0.f);
	f32 intPart = modBy * (x >= 0 ? 1.f : -1.f);
	f32 result = modf(x, &modBy);
	return result;
}

// Towards -infinity
inline f32 Floor(f32 x){
	f32 result = (f32)(s32)x;
	if (x < 0 && ModF32(x, 1.f)){
		result -= 1.f;
	}
	return result;
}

inline f32 Ceil(f32 x){
	f32 result = (f32)(s32)x;
	if (x > 0 && ModF32(x, 1.f)){
		result += 1.f;
	}
	return result;
}

inline f32 Frac(f32 x){
	return x - (f32)(s32)x;
}

inline f32 Round(f32 x){
	f32 result = Floor(x + .5f);
	return result;
}

// Max
inline f32 Max(f32 a, f32 b){
	if (a >= b)
		return a;
	return b;
}
inline s32 MaxS32(s32 a, s32 b){
	if (a >= b)
		return a;
	return b;
}
inline u32 MaxU32(u32 a, u32 b){
	if (a >= b)
		return a;
	return b;
}

// Min
inline f32 Min(f32 a, f32 b){
	if (a <= b)
		return a;
	return b;
}
inline s32 MinS32(s32 a, s32 b){
	if (a <= b)
		return a;
	return b;
}
inline u32 MinU32(u32 a, u32 b){
	if (a <= b)
		return a;
	return b;
}

// Clamp
inline f32 Clamp(f32 value, f32 min, f32 max){
	if (value > max) return max;
	if (value < min) return min;
	return value;
}
inline f32 Clamp01(f32 value){
	return Clamp(value, 0, 1.f);
}
inline s32 ClampS32(s32 value, s32 min, s32 max){
	if (value > max) return max;
	if (value < min) return min;
	return value;
}
inline u32 ClampU32(u32 value, u32 min, u32 max){
	if (value > max) return max;
	if (value < min) return min;
	return value;
}
inline f32 LerpClamp(f32 a, f32 b, f32 t){
	return Lerp(a, b, Clamp(t, 0, 1.f));
}

inline f32 Abs(f32 value){
	if (value < 0)
		return -value;
	return value;
}
inline s32 AbsS32(s32 value){
	if (value < 0)
		return -value;
	return value;
}



// Return range [-pi, pi]
inline f32 NormalizeAngle(f32 a){
	a = fmod(a, 2*PI);
	if (a < -PI)
		a += 2*PI;
	else if (a > PI)
		a -= 2*PI;
	return a;
}
inline f32 MoveAngleTowards(f32 from, f32 to, f32 angularSpeed){
	f32 diff = NormalizeAngle(to - from);
	f32 result;
	if (angularSpeed > Abs(diff)){
		result = to;
	}else if (diff > 0){
		result = from + angularSpeed;
	}else{
		result = from - angularSpeed;
	}
	result = NormalizeAngle(result);
	return result;
}

inline f32 MoveValueTowards(f32 value, f32 target, f32 speed){
	f32 result;
	if (value < target){
		result = Min(target, value + speed);
	}else{
		result = Max(target, value - speed);
	}
	return result;
}



//
// V2
//
struct v2{
	union {
		struct{
			f32 x;
			f32 y;
		};
		f32 asArray[2];
	};
};

inline v2 V2(f32 x, f32 y){
	v2 result = {x, y};
	return result;
}
inline v2 V2(f32 xy){
	v2 result = {xy, xy};
	return result;
}

inline v2 operator+(v2 a, v2 b){
	v2 result = {a.x + b.x, a.y + b.y};
	return result;
}

inline v2 operator-(v2 a, v2 b){
	v2 result = {a.x - b.x, a.y - b.y};
	return result;
}

inline v2 operator-(v2 a){
	v2 result = {-a.x, -a.y};
	return result;
}

inline v2 operator/(v2 a, f32 scalar){
	v2 result = {a.x/scalar, a.y/scalar};
	return result;
}
inline v2 operator*(v2 a, f32 scalar){
	v2 result = {a.x*scalar, a.y*scalar};
	return result;
}
inline v2 operator/(f32 scalar, v2 a){
	v2 result = {scalar/a.x, scalar/a.y};
	return result;
}
inline v2 operator*(f32 scalar, v2 a){
	v2 result = {a.x*scalar, a.y*scalar};
	return result;
}

inline void operator+=(v2 &a, v2 b){
	a = a + b;
}
inline void operator*=(v2 &a, f32 scalar){
	a = a * scalar;
}
inline void operator-=(v2 &a, v2 b){
	a = a - b;
}
inline void operator/=(v2 &a, f32 scalar){
	a = a / scalar;
}

inline f32 Dot(v2 a, v2 b){
	f32 result = a.x*b.x + a.y*b.y;
	return result;
}

inline f32 Cross(v2 a, v2 b){
	f32 result = a.x*b.y - a.y*b.x;
	return result;
}

inline v2 Hadamard(v2 a, v2 b){
	v2 result = {a.x*b.x, a.y*b.y};
	return result;
}

inline f32 Length(v2 a){
	f32 result = SquareRoot(a.x*a.x + a.y*a.y);
	return result;
}
inline f32 LengthSqr(v2 a){
	f32 result = a.x*a.x + a.y*a.y;
	return result;
}
inline f32 LengthFast(v2 a){
	f32 result = SquareRootFast(a.x*a.x + a.y*a.y);
	return result;
}


inline b32 PointInRect(v2 p, v2 r0, v2 r1){
	if (p.x < r0.x || p.x > r1.x || p.y < r0.y || p.y > r1.y)
		return false;
	return true;
}
inline b32 PointInCircle(v2 p, v2 c, f32 r){
	f32 distanceSqr = LengthSqr(p - c);
	if (distanceSqr <= r*r)
		return true;
	return false;
}

// Angle and direction stuff

inline f32 AngleOf(v2 a){
	f32 result = 0;
	f32 length = Length(a);
	if (length){
		result = acos(a.x/length);
		if (a.y <= 0){
			result = 2*PI - result;
		}
	}
	return result;
}

inline v2 V2LengthDir(f32 length, f32 direction){
	v2 result = {Cos(direction)*length, Sin(direction)*length};
	return result;
}


inline v2 LerpV2(v2 a, v2 b, f32 t){
	v2 result = {Lerp(a.x, b.x, t), Lerp(a.y, b.y, t)};
	return result;
}
inline v2 RoundV2(v2 a){
	v2 result = {Round(a.x), Round(a.y)};
	return result;
}

// Returns the dimension of the biggest rectangle with x/y ratio equal to 'ratio' that
// fits 'rect'.
v2 FitRatioInRect(f32 ratio, v2 rect){
	Assert(rect.y > 0);
	f32 rectRatio = rect.x/rect.y;
	v2 result; 
	if (ratio > rectRatio){
		result.x = rect.x;
		result.y = rect.x/ratio;
	}else{
		result.y = rect.y;
		result.x = rect.y*ratio;
	}
	return result;
}


//
// V2S
//
struct v2s{
	s32 x;
	s32 y;
};

inline v2s V2S(s32 x, s32 y){
	v2s result = {x, y};
	return result;
}
inline v2s V2S(s32 xy){
	v2s result = {xy, xy};
	return result;
}
inline v2s V2S(v2 a){
	v2s result = {(s32)a.x, (s32)a.y};
	return result;
}
inline v2 V2(v2s a){
	v2 result = {(f32)a.x, (f32)a.y};
	return result;
}

inline v2s operator+(v2s a, v2s b){
	v2s result = {a.x + b.x, a.y + b.y};
	return result;
}

inline v2s operator-(v2s a, v2s b){
	v2s result = {a.x - b.x, a.y - b.y};
	return result;
}

inline v2s operator-(v2s a){
	v2s result = {-a.x, -a.y};
	return result;
}

inline v2s operator/(v2s a, s32 scalar){
	v2s result = {a.x/scalar, a.y/scalar};
	return result;
}
inline v2s operator*(v2s a, s32 scalar){
	v2s result = {a.x*scalar, a.y*scalar};
	return result;
}
inline v2s operator/(s32 scalar, v2s a){
	v2s result = {scalar/a.x, scalar/a.y};
	return result;
}
inline v2s operator*(s32 scalar, v2s a){
	v2s result = {a.x*scalar, a.y*scalar};
	return result;
}

inline void operator+=(v2s &a, v2s b){
	a = a + b;
}
inline void operator*=(v2s &a, f32 scalar){
	a = a * scalar;
}
inline void operator-=(v2s &a, v2s b){
	a = a - b;
}
inline void operator/=(v2s &a, s32 scalar){
	a = a / scalar;
}



//
// V4
//
struct v4{
	union {
		struct{
			f32 r;
			f32 g;
			f32 b;
			f32 a;
		};
		struct{
			f32 x;
			f32 y;
			f32 z;
			f32 w;
		};
		f32 asArray[2];
	};
};

inline v4 V4(f32 r, f32 g, f32 b, f32 a = 1.f){
	v4 result = {r, g, b, a};
	return result;
}

inline v4 operator+(v4 a, v4 b){
	v4 result = {a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a};
	return result;
}

inline v4 operator-(v4 a, v4 b){
	v4 result = {a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a};
	return result;
}

inline v4 operator-(v4 a){
	v4 result = {-a.x, -a.y, -a.z, -a.w};
	return result;
}

inline v4 Hadamard(v4 a, v4 b){
	v4 result = {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};
	return result;
}

inline v4 LerpV4(v4 a, v4 b, f32 t){
	v4 result = {Lerp(a.r, b.r, t), Lerp(a.g, b.g, t), Lerp(a.b, b.b, t), Lerp(a.a, b.a, t)};
	return result;
}


// Colors
inline v4 V4_Red    (f32 alpha = 1.f){ return V4(1.f, 0, 0, alpha); }
inline v4 V4_Green  (f32 alpha = 1.f){ return V4(0, 1.f, 0, alpha); }
inline v4 V4_Blue   (f32 alpha = 1.f){ return V4(0, 0, 1.f, alpha); }
inline v4 V4_Yellow (f32 alpha = 1.f){ return V4(1.f, 1.f, 0, alpha); }
inline v4 V4_Cyan   (f32 alpha = 1.f){ return V4(0, 1.f, 1.f, alpha); }
inline v4 V4_Magenta(f32 alpha = 1.f){ return V4(1.f, 0, 1.f, alpha); }

inline v4 V4_White(f32 alpha = 1.f){ return V4(1.f, 1.f, 1.f, alpha); }
inline v4 V4_Black(f32 alpha = 1.f){ return V4(0, 0, 0, alpha); }
inline v4 V4_Grey(f32 value = .5f, f32 alpha = 1.f){ return V4(value, value, value, alpha); }


#endif