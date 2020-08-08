#include "..\Header\MathHelpers.h"

#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;

double MathHelpers::ComputeDistance(const ArduinoCar_Core::Point2D& p1, const ArduinoCar_Core::Point2D& p2)
{
	double dx = p1.X - p2.X;
	double dy = p1.Y - p2.Y;

	return sqrt(pow(dx, 2.0) + pow(dy, 2.0));
}

double MathHelpers::ComputeBearing(const ArduinoCar_Core::Point2D& p1, const ArduinoCar_Core::Point2D& p2)
{
	double dx = p1.X - p2.X;
	double dy = p1.Y - p2.Y;

	return atan2(dy, dx);
}

double MathHelpers::TruncateAngle(double angle)
{
	return fmod((angle + M_PI), (2.0 * M_PI)) - M_PI;
}
