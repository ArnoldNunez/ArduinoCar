#ifndef TURTLE_H
#define TURTLE_H

#include "SimulationObject.h"


namespace ArduinoCar_Visualization
{
	class Turtle : SimulationObject
	{
	public:
		Turtle();
		~Turtle();


		/**
		 * Draws the turtle using the specified renderer.
		 */
		virtual void Draw(SpriteRenderer& renderer);
	private:

	};


}

#endif// !TURTLE_H