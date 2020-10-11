#ifndef TURTLE_H
#define TURTLE_H

#include "SimulationObject.h"


namespace ArduinoCar_Visualization
{
	class Turtle : SimulationObject
	{
	public:
		/**
		 * Constructor
		 */
		Turtle();

		/**
		 * Destructor
		 */
		virtual ~Turtle();


		/**
		 * Draws the turtle using the specified renderer.
		 */
		virtual void Draw(SpriteRenderer& renderer);
	};


}

#endif// !TURTLE_H