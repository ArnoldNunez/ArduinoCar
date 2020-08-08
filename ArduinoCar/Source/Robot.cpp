#include "..\Header\Robot.h"
#include "..\Header\MathHelpers.h"


#include <random>
#include <algorithm>

using namespace std;
using namespace MathHelpers;


ArduinoCar_Core::Robot::Robot()
{
	mX = 0.0;
	mY = 0.0;
	mBearing = 0.0;
	mMaxDistance = 1.0;
	mMaxSteering = 3.14 / 2;
	mDistNoise = 0.05;
	mBearingNoise = 0.02;

	mSteeringNoise = 0.0;
	mDistanceNoise = 0.0;
	mMeasurementNoise = 0.0;
}

ArduinoCar_Core::Robot::~Robot()
{
}

ArduinoCar_Core::Robot::Robot(double x, double y, double bearing, double maxDistance, double maxSteering, double distNoise, double bearingNoise)
{
	mX = x;
	mY = y;
	mBearing = 0.0;
	mMaxDistance = 1.0;
	mMaxSteering = 3.14 / 2;
	mDistNoise = 0.05;
	mBearingNoise = 0.02;
	mSteeringNoise = 0.0;
	mDistanceNoise = 0.0;
	mMeasurementNoise = 0.0;
}



void ArduinoCar_Core::Robot::SetNoise(double steering_noise, double distance_noise, double measurement_noise)
{
	mSteeringNoise = steering_noise;
	mDistanceNoise = distance_noise;
	mMeasurementNoise = measurement_noise;
}

void ArduinoCar_Core::Robot::Move(double steering, double distance, bool noise)
{
	if (noise)
	{
		default_random_engine generator;
		uniform_real_distribution<double> steeringDistrib(-0.01, 0.01);
		uniform_real_distribution<double> distanceDistrib(0.99, 1.01);

		steering += steeringDistrib(generator);
		distance += distanceDistrib(generator);
	}

	steering = max(-mMaxSteering, steering);
	steering = min(mMaxSteering, steering);
	distance = max(0.0, distance);
	distance = min(mMaxDistance, distance);

	mBearing = MathHelpers::TruncateAngle(mBearing + steering);
	mX += distance * cos(mBearing);
	mY += distance * sin(mBearing);
}

std::tuple<double, double> ArduinoCar_Core::Robot::MeasureDistanceAndBearingTo(const Point2D& point, bool noise)
{
	double distNoisePercent = mDistanceNoise;
	double bearingNoisePercent = mBearingNoise;
	Point2D currentPos(mX, mY);

	double distanceToPoint = MathHelpers::ComputeDistance(currentPos, point);
	double bearingToPoint = MathHelpers::ComputeBearing(currentPos, point);

	double distanceNoise = 0.0;
	double bearingNoise = 0.0;
	if (noise)
	{
		double distanceSigma = distNoisePercent * distanceToPoint;
		double bearingSigma = bearingNoisePercent * distanceToPoint;

		default_random_engine generator;
		normal_distribution<double> distribution(0.0, distanceSigma);
		distanceNoise = distribution(generator);
		bearingNoise = distribution(generator);
	}

	double measuredDistance = distanceToPoint + distanceNoise;
	double measuredBearing = MathHelpers::TruncateAngle(bearingToPoint - mBearing + bearingNoise);

	return std::tuple<double, double>(measuredDistance, measuredBearing);
}

ArduinoCar_Core::Point2D ArduinoCar_Core::Robot::FindNextPoint(double steering, double distance)
{
	steering = max(-mMaxSteering, steering);
	steering = min(mMaxSteering, steering);
	distance = max(0.0, distance);
	distance = min(mMaxDistance, distance);

	double bearing = MathHelpers::TruncateAngle(mBearing + steering);
	double x = mX + (distance * cos(bearing));
	double y = mY + (distance * sin(bearing));

	return Point2D(x, y);
}


std::ostream& ArduinoCar_Core::operator<<(std::ostream& strm, const Robot& r)
{
	// TODO: insert return statement here
	return strm << "Robot --- X: " << r.mX << " Y: " << r.mY << endl;
}
