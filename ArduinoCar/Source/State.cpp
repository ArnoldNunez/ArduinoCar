#include "..\Header\State.h"
#include "..\Header\MathHelpers.h"

using namespace ArduinoCar_Core;
using namespace MathHelpers;


State::State(const std::vector<std::vector<char>>& map, const std::vector<std::vector<char>>& gemChecklist)
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

State::State(const std::vector<std::vector<char>>& map, const std::vector<std::vector<char>>& gemChecklist, double maxDistance, double maxSteering)
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

State::State(const std::vector<std::vector<char>>& map, const std::vector<std::vector<char>>& gemChecklist, double maxDistance, double maxSteering, double robotDistNoise, double robotBearingNoise)
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
		robotDistNoise, robotBearingNoise);
}

State::~State()
{
}

void ArduinoCar_Core::State::GenerateMeasurements(bool noiseFlag, std::map<int, GemMeasurement>& out)
{
	for (const Gem& gemLocation : this->mGemLocsOnMap)
	{
		std::tuple<double, double> distBearing = this->mRobot.MeasureDistanceAndBearingTo(Point2D(gemLocation.X, gemLocation.Y), noiseFlag);
		out[gemLocation.GetID()] = GemMeasurement(std::get<0>(distBearing), std::get<1>(distBearing), gemLocation.Type);
	}
}
