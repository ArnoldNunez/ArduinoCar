#ifndef SIMULATION_OBJECT_H
#define SIMULATION_OBJECT_H

#include <glad\glad.h>
#include <glm\glm.hpp>

#include "Texture2D.h"
#include "SpriteRenderer.h"

namespace ArduinoCar_Visualization
{
	/**
	 * Any object of our simulation will be a derivative of this class.
	 */
	class SimulationObject
	{
	public:
		glm::vec2 Position;	// The position of the object
		glm::vec2 Size;		// The size of the object
		glm::vec2 Velocity;	// The velocity of the object
		glm::vec3 Color;	// The color of the object

		float Rotation;		// The rotation of the object

		/// Flag indicating whether this simulation object has been removed
		bool Destroyed;

		/// The sprite associated with this object
		Texture2D Sprite;

		/**
		 * Constructor
		 */
		SimulationObject();

		/**
		 * Constructor
		 */
		SimulationObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

		/**
		 * Destructor
		 */
		~SimulationObject();

		/**
		 * Draws the sprite associated with this object using the specified renderer.
		 */
		virtual void Draw(SpriteRenderer& renderer);
	};


}


#endif // !SIMULATION_OBJECT_H
