#include "..\Header\State.h"
#include "..\Header\MathHelpers.h"

#include <iostream>
#include <random>
#include <algorithm>

using namespace ArduinoCar_Core;
using namespace MathHelpers;
using namespace std;


State::State(const std::vector<std::vector<char>>& map, const std::list<char>& gemChecklist)
{
	this->mOriginalGemChecklist = gemChecklist;
	this->mGemChecklist = gemChecklist;
	this->mMaxDistance = 1.0;
	this->mMaxSteering = M_PI / 2.0 + 0.01;
	
	int rows = map.size();
	int cols = map.front().size();

	// Process provided map
	int gemId = 0;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			// Check if uppercase ascii letter A - Z
			if (map[i][j] >= 65 && map[i][j] <= 90)
			{
				Gem gem(gemId, (double)i, (double)j, map[i][j]);
				this->mGemLocsOnMap.push_back(gem);
				++gemId;
			}
			else if (map[i][j] == '@')
			{
				this->mStartPosition = Point2D((double)i, (double)j);
			}
		}
	}

	// Initialize the robot at teh start position and at a bearing pointing due east
	this->mRobot = Robot(this->mStartPosition.X, this->mStartPosition.Y,
		0.0, this->mMaxDistance, this->mMaxSteering,
		0.05, 0.02);
}

State::State(const std::vector<std::vector<char>>& map, const std::list<char> &gemChecklist, double maxDistance, double maxSteering)
{
	this->mOriginalGemChecklist = gemChecklist;
	this->mGemChecklist = gemChecklist;
	this->mMaxDistance = maxDistance;
	this->mMaxSteering = maxSteering;

	int rows = map.size();
	int cols = map.front().size();

	// Process provided map
	int gemId = 0;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			// Check if uppercase ascii letter A - Z
			if (map[i][j] >= 65 && map[i][j] <= 90)
			{
				Gem gem(gemId, (double)i, (double)j, map[i][j]);
				this->mGemLocsOnMap.push_back(gem);
				++gemId;
			}
			else if (map[i][j] == '@')
			{
				this->mStartPosition = Point2D((double)i, (double)j);
			}
		}
	}

	// Initialize the robot at teh start position and at a bearing pointing due east
	this->mRobot = Robot(this->mStartPosition.X, this->mStartPosition.Y,
		0.0, this->mMaxDistance, this->mMaxSteering,
		0.05, 0.02);
}

State::State(const std::vector<std::vector<char>>& map, const std::list<char>& gemChecklist, double maxDistance, double maxSteering, double robotDistNoise, double robotBearingNoise)
{
	this->mOriginalGemChecklist = gemChecklist;
	this->mGemChecklist = gemChecklist;
	this->mMaxDistance = maxDistance;
	this->mMaxSteering = maxSteering;

	int rows = map.size();
	int cols = map.front().size();

	// Process provided map
	int gemId = 0;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			double coordX = (double)j;
			double coordY = -(double)i;

			// Check if uppercase ascii letter A - Z
			if (map[i][j] >= 65 && map[i][j] <= 90)
			{
				Gem gem(gemId, coordX, coordY, map[i][j]);
				this->mGemLocsOnMap.push_back(gem);
				++gemId;
			}
			else if (map[i][j] == '@')
			{
				this->mStartPosition = Point2D(coordX + 0.5, coordY - 0.5);
			}
		}
	}

	// Initialize the robot at teh start position and at a bearing pointing due east
	this->mRobot = Robot(this->mStartPosition.X, this->mStartPosition.Y,
		0.0, this->mMaxDistance, this->mMaxSteering,
		robotDistNoise, robotBearingNoise);
}

State::State(const State& state)
{
	this->mOriginalGemChecklist = state.mOriginalGemChecklist;
	this->mGemChecklist = state.mGemChecklist;
	this->mCollectedGems = state.mCollectedGems;
	this->mGemLocsOnMap = state.mGemLocsOnMap;
	this->mReportedGemLocations = state.mReportedGemLocations;
	this->mMaxDistance = state.mMaxDistance;
	this->mMaxSteering = state.mMaxSteering;
	this->mStartPosition = state.mStartPosition;
	this->mRobot = state.mRobot;
}

State::~State()
{
}




void State::GenerateMeasurements(bool noiseFlag, std::map<unsigned int, GemMeasurement>& out)
{
	for (const Gem& gemLocation : this->mGemLocsOnMap)
	{
		std::tuple<double, double> distBearing = this->mRobot.MeasureDistanceAndBearingTo(Point2D(gemLocation.X, gemLocation.Y), noiseFlag);
		out[gemLocation.GetID()] = GemMeasurement(std::get<0>(distBearing), std::get<1>(distBearing), gemLocation.Type);
	}
}

void State::UpdateAccordingTo(const Command& action, bool noiseFlag)
{
	if (action.Type == Command::Type::Move)
	{
		this->_attemptMove(action.Bearing, action.Distance, noiseFlag);
	}
	else if (action.Type == Command::Type::Extract)
	{
		this->_attemptExtraction(action.Gem, -1, -1);
	}
	else if (action.Type == Command::Type::Finished)
	{
		std::cout << "Finished all extraction" << std::endl;
	}
	else
	{
		std::cout << "Improper action format" << std::endl;
		throw;
	}
}




void State::_attemptMove(double steering, double distance, bool noiseFlag)
{
	// The robot can only move between 0 and max distance
	// The robot can only turn betwen -max steering and +max steering
	// Robot turns first before moving

	bool validDist = (distance >= 0.0 && distance <= this->mMaxDistance);
	bool validSteer = (steering >= -this->mMaxSteering && steering <= this->mMaxSteering);

	double finalSteering = steering;
	double finalDistance = distance;

	if (noiseFlag)
	{
		default_random_engine generator;
		uniform_real_distribution<double> steeringDistrib(-0.01, 0.01);
		uniform_real_distribution<double> distanceDistrib(0.99, 1.01);

		finalSteering += steeringDistrib(generator);
		finalDistance += distanceDistrib(generator);		
	}

	if (validDist && validSteer)
	{
		this->mRobot.Move(finalSteering, finalDistance);
	}
}

void ArduinoCar_Core::State::_attemptExtraction(char gemType, double currentX, double currentY)
{
	vector<Gem> gemsToRemove;

	for (const Gem& gem : this->mGemLocsOnMap)
	{
		if (gem.Type == gemType)
		{
			double distance = sqrt(pow(this->mRobot.GetX() - gem.X, 2.0) + pow(this->mRobot.GetY() - gem.Y, 2.0));
			if (distance < this->EXTRACTION_DISTANCE)
			{
				this->mCollectedGems.push_back(gem);
				this->mGemChecklist.remove(gem.Type);
				this->mReportedGemLocations[gem.Type] = Point2D(currentX, currentY);
				this->mGemLocsOnMap.remove(gem);
				return;
			}
		}
	}

	cout << "Location (" << this->mRobot.GetX() << ", " << this->mRobot.GetY() 
		<< ") does not contain a gem type <" << gemType << ">"
		<< "within the extraction distance." << endl;
} 