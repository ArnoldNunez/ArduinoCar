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
		enum Type { Move, Extract };

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

	Command::Command()
	{
		Type = Command::Type::Move;
		Distance = 0;
		Bearing = 0;
		Gem = ' ';
		GemX = 0;
		GemY = 0;
	}

	Command::~Command()
	{
	}

	Command::Command(const Command& command)
	{
		Type = command.Type;
		Distance = command.Distance;
		Bearing = command.Bearing;
		Gem = command.Gem;
		GemX = command.GemX;
		GemY = command.GemY;
	}
}

#endif // !COMMAND_H
