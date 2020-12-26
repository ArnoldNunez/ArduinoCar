#include "../Header/Command.h"

using namespace ArduinoCar_Core;
using namespace std;

Command::Command()
{
	Type = Command::Type::Move;
	Distance = 0;
	Bearing = 0;
	Gem = ' ';
	GemX = 0;
	GemY = 0;
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

Command::~Command()
{

}

