#include "..\Header\Gem.h"

using namespace ArduinoCar_Core;

Gem::Gem(int id, double x, double y, char type)
{
	this->mID = id;
	this->X = x;
	this->Y = y;
	this->Type = type;
}

Gem::~Gem()
{
}

void Gem::SetPosition(double x, double y)
{
	this->X = x;
	this->Y = y;
}

void Gem::SetType(char type)
{
	this->Type = type;
}
