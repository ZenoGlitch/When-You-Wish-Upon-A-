#include "star.h"

#include "level.h"


Star::Star(Level *level)
	: texture(&level->star_texture)
{
	setPosition(Vector2(500.0f, -70.0f));
}

void Star::sense(Level* level)
{
	float posX = getPosition().x;
	float posY = getPosition().y;
	float tradePosX = level->tradePost.getPosition().x;
	float tradePosY = level->tradePost.getPosition().y;

	if (posX == tradePosX && posY == tradePosY)
	{
		shouldRespawn = true;
	}
}

void Star::decide()
{
	if (shouldRespawn)
	{
		decision = respawn;
	}
}

void Star::act(Level* level)
{
	if (decision == respawn)
	{
		setPosition(level->PositionOnRandomTile());
		level->starsCollected += 1;
		shouldRespawn = false;
		decision = idle;
	}
}

void Star::draw(Level* level)
{
	if (texture != nullptr)
	{
		DrawTexture(*texture, (int)getPosition().x, (int)getPosition().y, WHITE );
	}
}

float Star::getEnergy()
{
	return energy;
}

void Star::setTexture(Texture &p_texture)
{
	texture = &p_texture;
}
