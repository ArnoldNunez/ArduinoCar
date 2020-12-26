#ifndef COMMAND_H
#define COMMAND_H

namespace ArduinoCar_Core
{
	/**
	 * Class for representing a robot command.
	 */
	class Command
	{
	public:
		/**
		 * The types of commands
		 */
		enum Type { Move, Extract, Finished };

		/**
		 * The type of the command.
		 */
		Command::Type Type;

		/**
		 * Distance to move
		 */
		double Distance;

		/**
		 * Amount to rotate (rads)
		 */
		double Bearing;

		/**
		 * The gem to extract
		 */
		char Gem;

		/**
		 * Estimated Gem x position
		 */
		double GemX;

		/**
		 * Estimated Gem y position
		 */
		double GemY;



		/**
		 * Constructor
		 */
		Command();

		/**
		 * Destructor
		 */
		~Command();

		/**
		 * Copy constructor.
		 */
		Command(const Command& command);
	};
}

#endif // !COMMAND_H
