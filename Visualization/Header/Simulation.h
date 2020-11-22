#ifndef SIMULATION_H
#define SIMULATION_H

#include "ResourceManager.h"
#include "SimulationObject.h"
#include "../../ArduinoCar/Header/State.h"
#include "../../ArduinoCar/Header/SLAM.h"

#include <vector>

namespace ArduinoCar_Visualization
{
	class Simulation
	{
	public:
		static const int MAP_HORIZ_SPACING = 100.0f;		// horizontal resolution
		static const int MAP_VERTICAL_SPACING = 100.0f;		// vertical resolution


		/**
		 * The list of objects in the simulation
		 */
		std::vector<SimulationObject> Objects;



		/**
		 * Constructor
		 */
		Simulation(ResourceManager& resourceManager);

		/**
		 * Copy constructor
		 */
		Simulation(const Simulation& simulation);

		/**
		 * Destructor
		 */
		virtual ~Simulation();

		/**
		 * Check if the simulation has completed.
		 */
		bool IsCompleted();

		/**
		 * Sets up simulation based on a simulation file.
		 */
		void FromFile(const std::string& fileName);


		/**
		 * Draws the simulation.
		 */
		void Draw(SpriteRenderer& renderer);

		/**
		 * Updates the simulation
		 */
		void Update(float dt);

	private:
		/**
		 * Initializes the simulation from a environment text file.
		 */
		void Init(std::vector<std::vector<unsigned int>> environmentData,
			unsigned int environmentWidth, unsigned int environmentHeight);

		/**
		 * Generates the simulation map from the map string
		 */
		void GenerateMap(const std::string& mapString);

		/**
		 * Generates the simulation state with the read in simulation parameters
		 */
		void GenerateState(const std::string& mapString, const std::string& neededGems, const std::string& maxDistance,
			const std::string& maxSteering, const std::string& robotDistanceNoise, const std::string& robotBearingNoise);

		ResourceManager& mResourceManager;
		ArduinoCar_Core::State* mState;
		ArduinoCar_Core::SLAM mSLAM;

		unsigned int mAccumTime = 0;	// Total accumulated time (ms)
		unsigned int mActualTime = 0;	// The actual time(ms)
		unsigned int mTimeStep = 1000;	// The 'physics' update period (ms)
		unsigned int mRobotNdx = 0;		// Index of the robot in the simulation objects
	};
}

#endif // !SIMULATION_H

