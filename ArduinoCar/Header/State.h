#ifndef STATE_H
#define STATE_H

#include "Point2D.h"
#include "MathHelpers.h"
#include "Gem.h"
#include "Robot.h"
#include "GemMeasurement.h"

#include <vector>
#include <map>

namespace ArduinoCar_Core
{
	/**
	 * Class for the current state of the simulation
	 */
	class State
	{
	public:
		/**
		 * Constructor
		 */
		State(const std::vector<std::vector<char>> &map,
			const std::vector<std::vector<char>> &gemChecklist);

		/**
		 * Constructor
		 */
		State(const std::vector<std::vector<char>>& map,
			const std::vector<std::vector<char>>& gemChecklist,
			double maxDistance,
			double maxSteering);

		/**
		 * Constructor
		 */
		State(const std::vector<std::vector<char>>& map,
			const std::vector<std::vector<char>>& gemChecklist,
			double maxDistance,
			double maxSteering,
			double robotDistNoise,
			double robotBearingNoise);

		/**
		 * Destructor
		 */
		virtual ~State();

		/**
		 * Generates measurements from the robot to the gems on the map.
		 * The resulting measurements are place dinto the input list.
		 * Generates measurements with noise if flag set.
		 */
		void GenerateMeasurements(bool noiseFlag, std::map<int, GemMeasurement>& out);

	private:
		std::vector<std::vector<char>> mOriginalGemChecklist;
		std::vector<std::vector<char>> mGemChecklist;
		std::vector<std::vector<char>> mCollectedGems;
		std::vector<Gem> mGemLocsOnMap;	// TODO
		std::vector<ArduinoCar_Core::Point2D> mReportedGemLocations;

		double mMaxDistance;
		double mMaxSteering;
		ArduinoCar_Core::Point2D mStartPosition;
		Robot mRobot;
	};
}

#endif // !STATE_H
