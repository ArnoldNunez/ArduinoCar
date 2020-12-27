#include "..\Header\SimulationObject.h"

using namespace ArduinoCar_Visualization;

SimulationObject::SimulationObject()
{
}

SimulationObject::SimulationObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), Destroyed(false) 
{
}

SimulationObject::~SimulationObject()
{
}

void SimulationObject::Draw(SpriteRenderer& renderer)
{
    if (!this->Destroyed)
    {
        renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
    }
}