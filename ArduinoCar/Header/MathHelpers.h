#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

#include "Point2D.h"


namespace MathHelpers
{
	/**
	 * Computes the distance between two points
	 */
	double ComputeDistance(const ArduinoCar_Core::Point2D& p1, const ArduinoCar_Core::Point2D& p2);

	/**
	 * Computes the bearing between two points
	 */
	double ComputeBearing(const ArduinoCar_Core::Point2D& p1, const ArduinoCar_Core::Point2D& p2);

	/**
	 * Truncates and angle between -pi and pi
	 */
	double TruncateAngle(double angle);
}

#endif // !MATH_HELPERS_H
