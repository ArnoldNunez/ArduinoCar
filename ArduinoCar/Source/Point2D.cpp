#include "..\Header\Point2D.h"

ArduinoCar_Core::Point2D::Point2D()
{
	X = 0.0;
	Y = 0.0;
}

ArduinoCar_Core::Point2D::Point2D(double x, double y)
{
	X = x;
	Y = y;
}

ArduinoCar_Core::Point2D::~Point2D()
{
}

ArduinoCar_Core::Point2D::Point2D(const Point2D& pt)
{
	X = pt.X;
	Y = pt.Y;
}
