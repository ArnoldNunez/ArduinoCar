#ifndef SLAM_H
#define SLAM_H

#include "Matrix.h"
#include "Point2D.h"
#include "GemMeasurement.h"

#include <string>
#include <map>
#include <list>

namespace ArduinoCar_Core
{
	/**
	 * Basic SLAM Module
	 */
	class SLAM
	{
	public:

		/**
		 * Default Constructor
		 */
		SLAM();

		/**
		 * Copy Constructor
		 */
		SLAM(const SLAM&) = delete;

		/**
		 * Default Destructor
		 */
		virtual ~SLAM();

		/**
		 * Processes a new series of measurements.
		 * \param measurements The collection of measurements (map) in the format
		 * {landmark id: {distance: 0.0, bearing: 0.0, type: 'B'}, ... }
		 * \return Point2D The currently believed robot position
		 */
		Point2D ProcessMeasurements(const std::map<unsigned int, GemMeasurement>& measurements);

		/**
		 * Processes a new movement
		 * \param steering Amount to turn
		 * \param distance Distance to move
		 * \param motionNoise Movement noise
		 * \return Point2D The currently believed robot position
		 */
		Point2D ProcessMovement(double steering, double distance, double motionNoise = 0.01);

		/**
		 * Gets the computed estimation matrix
		 * \return Matrix The mu matrix containing the estimated positions for landmarks.
		 */
		Matrix GetEstimateMatrix() const { return this->mMu; };

		/**
		 * Gets the landmark index mapping
		 */
		std::map<unsigned int, int> GetLandmarkNdxMap() const { return this->mLandmarkNdxMap; };

	private:

		/// SLAM matrices
		Matrix mOmega;
		Matrix mXi;
		Matrix mMu;

		/// Tuneable SLAM parameters
		double mInitLocConstr;
		double mMotionSigma;
		double mMeasurementSigma;

		/// Runtime Data
		int mDim;
		std::map<unsigned int, int> mLandmarkNdxMap;
		double mEstimatedHeading;
		Point2D mEstimatedPos;
		Point2D mPrevEstimatedPos;
		Point2D mRobotPos;
	};
}


#endif // !SLAM_H