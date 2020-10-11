#ifndef SIMULATION_H
#define SIMULATION_H

#include "SimulationObject.h"

#include <vector>

namespace ArduinoCar_Visualization
{
	class Simulation
	{
	public:
		/**
		 * The list of objects in the simulation
		 */
		std::vector<SimulationObject> Objects;

		/**
		 * Constructor
		 */
		Simulation();

		/**
		 * Destructor
		 */
		virtual ~Simulation();

		/**
		 * Check if the simulation has completed.
		 */
		bool IsCompleted();

	private:
		/**
		 * Initializes the simulation from a environment text file.
		 */
		void Init(std::vector<std::vector<unsigned int>> environmentData,
			unsigned int environmentWidht, unsigned int environmentHeight);
	};
}

#endif // !SIMULATION_H

