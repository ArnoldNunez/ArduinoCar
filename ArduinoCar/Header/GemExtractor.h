#ifndef GEM_EXTRACTOR_H
#define GEM_EXTRACTOR_H

#include "Matrix.h"
#include "Point2D.h"
#include "GemMeasurement.h"
#include "SLAM.h"
#include "Command.h"

#include <string>
#include <map>
#include <list>

namespace ArduinoCar_Core
{
	/**
	 * Uses the SLAM module to extract gems
	 */
	class GemExtractor
	{
	public:

		/**
		 * Constructor
		 */
		GemExtractor(double maxDistance, double maxSteering);

		/**
		 * Copy constructor
		 */
		GemExtractor(const GemExtractor&) = delete;

		/**
		 * Destructor
		 */
		~GemExtractor();

		/**
		 * Determines the next move to make to grab the needed gems.
		 */
		ArduinoCar_Core::Command NextMove(const std::list<char>& neededGems, const std::map<unsigned int, GemMeasurement>& measurements);

		/**
		 * Gets the max distance allowed for robot movement in one step
		 */
		double GetMaxDistance() const { return mMaxDistance; }

		/**
		 * Gets the max steering angle allowed for robot movement in one step
		 */
		double GetMaxSteering() const { return mMaxSteering; }

	private:
		ArduinoCar_Core::SLAM mSLAM;
		double mMaxDistance;
		double mMaxSteering;
		double mMinExtractDist;
	};
}

#endif // !GEM_EXTRACTOR_H