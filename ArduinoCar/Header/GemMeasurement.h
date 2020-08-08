#ifndef GEM_MEASUREMENT_H
#define GEM_MEASUREMENT_H

namespace ArduinoCar_Core
{
	class GemMeasurement
	{
	public:
		/**
		 * Default Constructor
		 */
		GemMeasurement();

		/**
		 * Constructor
		 * \param dist The distance to the gem
		 * \param bearing The bearing to the gem
		 * \param type The type of the gem
		 */
		GemMeasurement(double dist, double bearing, char type);

		/**
		 * Copy Constructor (disabled)
		 */
		GemMeasurement(const GemMeasurement&) = delete;

		/**
		 * Destructor
		 */
		virtual ~GemMeasurement();

		
		/**
		 * Getter for the distance to the gem.
		 */
		double GetDistance() const;

		/**
		 * Getter for the bearing to the gem.
		 */
		double GetBearing() const;

		/**
		 * Getter for the tye of the gem.
		 */
		char GetType() const;

	private:
		double mDistance;	/// Distance away the gem is
		double mBearing;	/// The bearing to the gem
		char mType;			/// The type of the gem
	};

}


#endif // !GEM_MEASUREMENT_H
