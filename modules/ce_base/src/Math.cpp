//- Standard Library -
#define _USE_MATH_DEFINES
#include <cmath>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Math.h>

using namespace std;

namespace ce
{
	float g_pi = (float)M_PI;
	float g_2pi = g_pi * 2;
	float g_degToRad = g_pi / 180.f;
	float g_radToDeg = 1.f / g_degToRad;
	float DegreesToRadians()
	{
		return g_degToRad;
	}
	float RadiansToDegrees()
	{
		return g_radToDeg;
	}
	float AngleDifferenceDegrees(float a, float b)
	{
		return BaseAngleDegrees(a - b);
	}
	float BaseAngleDegrees(float a)
	{
		if(a < -180.f)
			a += 360.f;
		else if(a > 180.f)
			a -= 360.f;
		return a;
	}
	float AngleDifferenceRadians(float a, float b)
	{
		return BaseAngleRadians(a - b);
	}
	float BaseAngleRadians(float a)
	{
		if(a < -g_pi)
			a += g_2pi;
		else if(a >(float)M_PI)
			a -= g_2pi;
		return a;
	}
}
