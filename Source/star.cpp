#include "star.h"

#include "level.h"


Star::Star(Level *level)
	: texture(&level->star_texture)
{
	setPosition(Vector2(500.0f, -70.0f));
}

void Star::sense(Level* level)
{
	// is there already stars spawned?
	//if (level->star_agents.size() < 3)
	//{
	//	areThereEnoughStars = true;
	//}
	//else areThereEnoughStars = false;

}

void Star::decide()
{
	//if (areThereEnoughStars)
	//{
	//	decision = shouldSpawn;
	//}
	//else decision = idle;
}

void Star::act(Level* level)
{
	//if (decision == shouldSpawn)
	//{
	//	level->SpawnAgent("star");
	//}
}

void Star::draw()
{
	DrawTexture(*texture, (int)getPosition().x, (int)getPosition().y, WHITE );
}

float Star::getEnergy()
{
	return energy;
}

void Star::setTexture(Texture &p_texture)
{
	texture = &p_texture;
}
