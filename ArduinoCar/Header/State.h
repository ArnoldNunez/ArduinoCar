#ifndef STATE_H
#define STATE_H

#include "Point2D.h"
#include "MathHelpers.h"
#include "Gem.h"
#include "Robot.h"
#include "GemMeasurement.h"
#include "Command.h"

#include <vector>
#include <list>
#include <map>

namespace ArduinoCar_Core
{
	/**
	 * Class for the current state of the simulation
	 */
	class State
	{
	public:
		const double EXTRACTION_DISTANCE = 0.25;


		/**
		 * Constructor
		 */
		State(const std::vector<std::vector<char>> &map,
			const std::list<char> &gemChecklist);

		/**
		 * Constructor
		 */
		State(const std::vector<std::vector<char>>& map,
			const std::list<char>& gemChecklist,
			double maxDistance,
			double maxSteering);

		/**
		 * Constructor
		 */
		State(const std::vector<std::vector<char>>& map,
			const std::list<char>& gemChecklist,
			double maxDistance,
			double maxSteering,
			double robotDistNoise,
			double robotBearingNoise);

		/**
		 * Copy constructor
		 */
		State(const State& state);

		/**
		 * Destructor
		 */
		virtual ~State();

		/**
		 * Generates measurements from the robot to the gems on the map.
		 * The resulting measurements are place dinto the input list.
		 * Generates measurements with noise if flag set.
		 */
		void GenerateMeasurements(bool noiseFlag, std::map<unsigned int, GemMeasurement>& out);

		/**
		 * Updates the state acording to the given action
		 */
		void UpdateAccordingTo(const Command& action, bool noiseFlag);

		/**
		 * Gets the gem checklist.
		 */
		std::list<char> GetGemChecklist() const { return mGemChecklist; }

		/**
		 * Gets the robot.
		 */
		const Robot& GetRobot() const { return mRobot; }

		/**
		 * Gets the gem locations on the map
		 */
		const std::list<Gem> GetGemMapLocs() const { return mGemLocsOnMap; }

		/**
		 * Gets the list of collected gems
		 */
		const std::list<Gem> GetCollectedGems() const { return mCollectedGems; }


	private:
		std::list<char> mOriginalGemChecklist;			// List of gem types to be picked up
		std::list<char> mGemChecklist;					// Copy of original gem checklist
		std::list<Gem> mCollectedGems;					// List of collected gems
		std::list<Gem> mGemLocsOnMap;					// List of gems and their x,y locations on map
		std::map<char, Point2D> mReportedGemLocations;	// Map between gem type and its reported pickup location

		double mMaxDistance;
		double mMaxSteering;
		Point2D mStartPosition;
		Robot mRobot;


		/**
		 * Attempts to perform a move action if that action is valid
		 */
		void _attemptMove(double steering, double distance, bool noiseFlag);

		/**
		 * Attempt to extract a gem from the current x,y location.
		 */
		void _attemptExtraction(char gemType, double currentX, double currentY);
	};
}

#endif // !STATE_H
