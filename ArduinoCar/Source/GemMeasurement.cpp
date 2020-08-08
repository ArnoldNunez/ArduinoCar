#include "..\Header\GemMeasurement.h"

ArduinoCar_Core::GemMeasurement::GemMeasurement()
{
	mDistance = 0.0;
	mBearing = 0.0;
	mType = 'A';
}

ArduinoCar_Core::GemMeasurement::GemMeasurement(double dist, double bearing, char type)
{
	mDistance = dist;
	mBearing = bearing;
	mType = type;
}

ArduinoCar_Core::GemMeasurement::~GemMeasurement()
{
}

double ArduinoCar_Core::GemMeasurement::GetDistance() const
{
	return mDistance;
}

double ArduinoCar_Core::GemMeasurement::GetBearing() const
{
	return mBearing;
}

char ArduinoCar_Core::GemMeasurement::GetType() const
{
	return mType;
}
