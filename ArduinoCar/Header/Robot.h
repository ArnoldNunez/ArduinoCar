#ifndef ROBOT_H
#define ROBOT_H

#include "Point2D.h"

#include <vector>
#include <ostream>
#include <tuple>


namespace ArduinoCar_Core
{
	/**
	 * Class representing the arduino robot
	 */
	class Robot
	{
	public:
		/**
		 * Default constructor
		 */
		Robot();

		/**
		 * Constructor
		 * \param x Coordinate x
		 * \param y Coordinate y
		 * \param bearing The bearing of the robot
		 * \param maxDistance The max distance the robot can travel in one timestep
		 * \param distNoise The travel distance noise
		 * \param bearingNoise The amount of turning noise
		 */
		Robot(double x, double y, double bearing, double maxDistance, double maxSteering, double distNoise, double bearingNoise);

		/**
		 * Default destructor
		 */
		virtual ~Robot();

		/**
		 * Default Copy Constructor
		 */
		Robot(const Robot&) = delete;



		/**
		 * Sets the noise values on the robot
		 */
		void SetNoise(double steering_noise, double distance_noise, double measurement_noise);

		/**
		 * Attempts to move robot by dx, dy. If outside world boundary,
		 * then move does nothing and instead returns failure.
		 */
		void Move(double steering, double distance, bool noise = false);

		/**
		 * Measures the distance and bearing to a specific point. Can compute using noise
		 * if noise flag set to true.
		 * \param point The point we want to measure distance to
		 * \param noise Flag indicating whether the calculation should include measurement noise.
		 */
		std::tuple<double, double> MeasureDistanceAndBearingTo(const Point2D& point, bool noise = false);

		/**
		 * Find the next point without updating the robot's position
		 * \param steering The amount to turn
		 * \param distance The amount to travel
		 */
		Point2D FindNextPoint(double steering, double distance);


		double GetX() const { return this->mX; }
		double GetY() const { return this->mY; }
		double GetBearing() const { return this->mBearing; }

	private:
		double mX;
		double mY;
		double mBearing;
		double mMaxDistance;
		double mMaxSteering;
		double mBearingNoise;
		double mSteeringNoise;
		double mDistanceNoise;
		double mMeasurementNoise;


		/**
		 * To String override
		 */
		friend std::ostream& operator<<(std::ostream& strm, const Robot& r);
	};
}

#endif