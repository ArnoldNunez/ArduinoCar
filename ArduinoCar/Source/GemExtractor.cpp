#include "..\Header\GemExtractor.h"

#include <algorithm>
#include <iostream>

using namespace std;
using namespace ArduinoCar_Core;

ArduinoCar_Core::GemExtractor::GemExtractor(double maxDistance, double maxSteering) 
	: mMaxDistance(maxDistance), mMaxSteering(maxSteering)
{
	mMinExtractDist = 0.05;
}

ArduinoCar_Core::GemExtractor::~GemExtractor()
{
}

std::string ArduinoCar_Core::GemExtractor::NextMove(const std::list<char>& neededGems, const std::map<unsigned int, GemMeasurement>& measurements)
{
	if (neededGems.size() == 0)
	{
		return "finished";
	}

	// Determine next gem to extract

	unsigned int measId = -1;
	map<unsigned int, GemMeasurement>::const_iterator it;
	for (it = measurements.begin(); it != measurements.end(); it++)
	{
		if (it->second.GetType() == neededGems.front())
		{
			measId = it->first;
		}
	}

	// Limit distance and steering values based on robot capability

	double distance = min(this->mMaxDistance, measurements.at(measId).GetDistance());
	double steering = 0.0;

	if (measurements.at(measId).GetBearing() < -this->mMaxSteering)
	{
		steering = -this->mMaxSteering;
	}
	else if (measurements.at(measId).GetBearing() > this->mMaxSteering)
	{
		steering = this->mMaxSteering;
	}
	else
	{
		steering = measurements.at(measId).GetBearing();
	}

	string action = "";
	Point2D robotPos(-1.0, -1.0);
	if (measurements.at(measId).GetDistance() < this->mMinExtractDist)
	{
		double estGemX = this->mSLAM.GetEstimateMatrix()(this->mSLAM.GetLandmarkNdxMap()[measId], 0);
		double estGemY = this->mSLAM.GetEstimateMatrix()(this->mSLAM.GetLandmarkNdxMap()[measId] + 1, 0);
		action = "extract " + neededGems.front() + ' ' + to_string(estGemX) + ' ' + to_string(estGemY);
	}
	else
	{
		// Estimate robot position using Online GraphSLAM
		this->mSLAM.ProcessMeasurements(measurements);
		robotPos = this->mSLAM.ProcessMovement(steering, distance);
		action = "move " + to_string(steering) + " " + to_string(distance);
	}

	cout << "ROBOT POS: (" << robotPos.X << ", " << robotPos.Y << ")" << endl;
	cout << "Action: " << action << endl;

	return action;
}