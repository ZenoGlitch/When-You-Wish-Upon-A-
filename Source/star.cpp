#include "star.h"

Star::Star()
{
	energy = 50.0f;
	
	texture = LoadTexture("assets/star.png");
	texture.height = texture.height * 2;
	texture.width = texture.width * 2;
	
	setPosition(Vector2(500.0f, 500.0f));
}

void Star::sense(Level* level)
{

}

void Star::decide()
{

}

void Star::act(Level* level)
{

}

void Star::draw()
{
	DrawTexture(texture, (int)getPosition().x, (int)getPosition().y, WHITE );
}

float Star::getEnergy()
{
	return energy;
}
