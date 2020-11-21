#include "..\Header\Simulation.h"
#include "..\Header\StringHelpers.h"

#include <iostream>
#include <fstream>
#include <map>

using namespace ArduinoCar_Visualization;
using namespace std;


ArduinoCar_Visualization::Simulation::Simulation(ResourceManager& resourceManager) : mResourceManager(resourceManager)
{
}


Simulation::~Simulation()
{
}

bool ArduinoCar_Visualization::Simulation::IsCompleted()
{
	return false;
}

void Simulation::FromFile(const std::string& fileName)
{
	string text;
	vector<string> lines;
	map<string, string> keyVal;

	ifstream file("Simulations/" + fileName);

	if (file)
	{
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);

		char* buffer = new char[length];
		memset(buffer, 0, length);

		file.read(buffer, length);

		file.close();

		text.assign(buffer, length);

		delete[] buffer;
	}
	else
	{
		// file failed
		std::cout << "ERROR::SIMULATION: Failed to read simulation file: " << fileName << std::endl;
		return;
	}
	

	// Process file contents
	lines = StringHelpers::Split(text, "\n");
	for (size_t i = 0; i < lines.size(); i++)
	{
		vector<string> fieldVal = StringHelpers::Split(lines[i], "=");

		if (fieldVal[0].find("area map") != string::npos)
		{
			keyVal["area_map"] = fieldVal[1];
		}
		else if (fieldVal[0].find("needed gems") != string::npos)
		{
			keyVal["needed_gems"] = fieldVal[1];
		}
		else if (fieldVal[0].find("horizon distance") != string::npos)
		{
			keyVal["horizontal_distance"] = fieldVal[1];
		}
		else if (fieldVal[0].find("max steering") != string::npos)
		{
			keyVal["max_steering"] = fieldVal[1];
		}
		else if (fieldVal[0].find("robot distance noise") != string::npos)
		{
			keyVal["robot_distance_noise"] = fieldVal[1];
		}
		else if (fieldVal[0].find("robot bearing noise") != string::npos)
		{
			keyVal["robot_bearing_noise"] = fieldVal[1];
		}
		else
		{
			cout << "ERROR::SIMULATION: Failed to read simulation file keyvalue in file: " 
				<< fileName << " Field: " << fieldVal[0] << endl;
		}
	}


	GenerateMap(keyVal["area_map"]);

	// TODO: Generate State based on simulation parameters
	//GenerateState(...)

	return;
}

void ArduinoCar_Visualization::Simulation::Draw(SpriteRenderer& renderer)
{
	for (size_t i = 0; i < this->Objects.size(); i++)
	{
		this->Objects[i].Draw(renderer);
	}
}

void ArduinoCar_Visualization::Simulation::Update(float dt)
{
	mAccumTime += dt;

	while (mAccumTime >= mTimeStep)
	{
		mActualTime += mTimeStep;
		mAccumTime -= mTimeStep;

		// Update Robot and Gems

	}
}

void ArduinoCar_Visualization::Simulation::GenerateMap(const std::string& mapString)
{	
	float mapW = 0;
	float mapH = 0;

	float posX = 0;
	float posY = 0;

	for (size_t i = 0; i < mapString.length(); i++)
	{
		if (mapString[i] == ',')
		{
			mapW = posX - 1;
			posY += Simulation::MAP_VERTICAL_SPACING;
			posX = 0;
		}
		else if (mapString[i] == '.')
		{
			posX += Simulation::MAP_HORIZ_SPACING;
		}
		else if (mapString[i] == '@')
		{
			glm::vec2 loc(posX, posY);
			glm::vec2 size(40.0f, 40.0f);

			SimulationObject rbt(loc, size, mResourceManager.GetTexture("turtle"));
			this->Objects.push_back(rbt);

			posX += Simulation::MAP_HORIZ_SPACING;
		}
		else if (mapString[i] >= 65 && mapString[i] <= 122)
		{
			glm::vec2 loc(posX, posY);
			glm::vec2 size(30.0f, 30.0f);
			glm::vec3 color(245.0f, 245.0f, 0.0f);

			SimulationObject gem(loc, size, mResourceManager.GetTexture("gem"));
			gem.Color = color;
			this->Objects.push_back(gem);

			posX += Simulation::MAP_HORIZ_SPACING;
		}
	}

	mapH = posY;

	//glm::vec2 loc(0.0f, 0.0f);
	//glm::vec2 size(mapW, mapH);
	//SimulationObject ground(loc, size, mResourceManager.GetTexture("face"));
	//this->Objects.push_back(ground);
}
