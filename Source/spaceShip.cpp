#include "spaceShip.h"

#include "level.h"

void SpaceShip::sense(Level* level)
{
}

void SpaceShip::decide()
{
}

void SpaceShip::act(Level* level)
{
}

void SpaceShip::draw(Level* level)
{
	DrawTexture(*texture, (int)getPosition().x, (int)getPosition().y, WHITE);
}

void SpaceShip::setTexture(Texture& p_texture)
{
	texture = &p_texture;
}

float SpaceShip::getEnergy()
{
	return 0.0f;
}
