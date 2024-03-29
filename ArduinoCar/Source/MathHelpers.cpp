#include "..\Header\MathHelpers.h"

#include <math.h>

using namespace std;

double MathHelpers::ComputeDistance(const ArduinoCar_Core::Point2D& p1, const ArduinoCar_Core::Point2D& p2)
{
	double dx = p2.X - p1.X;
	double dy = p2.Y - p1.Y;

	return sqrt(pow(dx, 2.0) + pow(dy, 2.0));
}

double MathHelpers::ComputeBearing(const ArduinoCar_Core::Point2D& p1, const ArduinoCar_Core::Point2D& p2)
{
	double dx = p2.X - p1.X;
	double dy = p2.Y - p1.Y;

	return atan2(dy, dx);
}

double MathHelpers::TruncateAngle(double angle)
{
	return Mod((angle + M_PI), (2.0 * M_PI)) - M_PI;
}

double MathHelpers::Mod(double a, double n)
{
	return a - (n * floor(a / n));
}
