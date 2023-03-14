#include "starChaser.h"

#include "level.h"
#include <cassert>

//StarChaser::StarChaser(Texture *p_texture, Vector2 p_targetPosition)
//	: texture(p_texture)
//	, targetPosition(p_targetPosition)
//	, sensing(idle)
//	, decision(undecided)
//	, energy(maxEnergy)
//{
//}

void StarChaser::initialize()
{
	state = idle;
	energy = maxEnergy;
	shouldMoveToStar = true;
	shouldMoveToTrade = false;
}

void StarChaser::sense(Level *level)
{
	float posX = getPosition().x;
	float posY = getPosition().y;
	float starPosX = level->star.getPosition().x;
	float starPosy = level->star.getPosition().y;
	float tradePosX = level->tradePost.getPosition().x;
	float tradePosY = level->tradePost.getPosition().y;
	float shipPosX = level->tradePost.getPosition().x;
	float shipPosY = level->tradePost.getPosition().y;

	if (destinationReached && posX == starPosX && posY == starPosy)
	{
		shouldMoveToTrade = true;

		shouldMoveToStar = false;
		
		stepsTaken = 0;
		path.clear();
		pathFound = false;
		destinationReached = false;
	}
	else if (destinationReached && posX == tradePosX && posY == tradePosY)
	{
		shouldMoveToStar = true;

		shouldMoveToTrade = false;

		stepsTaken = 0;
		path.clear();
		pathFound = false;
		destinationReached = false;
	}
	else if (destinationReached && posX == shipPosX && posY == shipPosY)
	{
		shouldMoveToShip = true;

		shouldMoveToStar = false;
		shouldMoveToTrade = false;

		stepsTaken = 0;
		path.clear();
		pathFound = false;
		destinationReached = false;
	}

	if (level->starChaserHeldByMouse)
	{
		shouldDeactivate = true;

		shouldMoveToStar = false;
		shouldMoveToTrade = false;
		pathFound = false;
	}
	if (!level->starChaserHeldByMouse)
	{
		shouldDeactivate = false;
	}


	if (!shouldDeactivate && !shouldMoveToStar && !shouldMoveToShip && !shouldMoveToTrade)
	{
		if (posX != level->star.getPosition().x && posY != level->star.getPosition().y)
		{
			shouldMoveToStar = true;
		}
	}


	if (energy <= 0)
	{
		shouldMoveToShip = true;

		shouldMoveToTrade = false;
		shouldMoveToStar = false;
		shouldDeactivate = false;
	}
}

void StarChaser::decide()
{
	if (shouldDeactivate)
	{
		state = beingHeld;
	}
	if (shouldMoveToShip)
	{
		state = movingToShip;
	}
	if (shouldMoveToStar)
	{
		state = movingToStar;
	}
	if (shouldMoveToTrade)
	{
		state = movingToTrade;
	}
}
			
void StarChaser::act(Level *level)
{
	const float moveEnergyDrain = 1.0f;
	if (state == movingToStar && !pathFound)
	{
		path = level->pathfind(getPosition(), level->star.getPosition(), level->numberOfSteps);
		if (path.empty())
		{
			assert(false);
		}
		pathFound = true;
	}

	if (state == movingToTrade && !pathFound)
	{
		path = level->pathfind(getPosition(), level->tradePost.getPosition(), level->numberOfSteps);
		if (path.empty())
		{
			assert(false);
		}
		pathFound = true;
	}

	if (state == movingToShip && !pathFound)
	{
		path = level->pathfind(getPosition(), level->spaceShip.getPosition(), level->numberOfSteps);
		if (path.empty())
		{
			assert(false);
		}
		pathFound = true;
	}

	if (!path.empty())
	{
		if (state == movingToStar)
		{
			moveTimer -= GetFrameTime();

			if (moveTimer <= 0 && !destinationReached)
			{
				if (stepsTaken < path.size() - 1)
				{
					stepsTaken += 1;
				}
				if (stepsTaken == path.size() - 1)
				{
					destinationReached = true;
				}
				Vector2 newPos;
				newPos.x = path.at(stepsTaken).x + 5.0f;
				newPos.y = path.at(stepsTaken).y + 5.0f;
				setPosition(newPos);
				energy -= moveEnergyDrain;
				moveTimer = moveTimerReset;
			}
		}

		if (state == movingToTrade)
		{
			moveTimer -= GetFrameTime();

			if (moveTimer <= 0 && !destinationReached)
			{
				if (stepsTaken < path.size() - 1)
				{
					stepsTaken += 1;
				}
				if (stepsTaken == path.size() - 1)
				{
					destinationReached = true;
				}
				Vector2 newPos;
				newPos.x = path.at(stepsTaken).x + 5.0f;
				newPos.y = path.at(stepsTaken).y + 5.0f;
				setPosition(newPos);

				level->star.setPosition(getPosition()); // move the star with the starChaser
				
				energy -= moveEnergyDrain;
				moveTimer = moveTimerReset;
			}
		}

		if (state == movingToShip)
		{
			moveTimer -= GetFrameTime();

			if (moveTimer <= 0 && !destinationReached)
			{
				if (stepsTaken < path.size() - 1)
				{
					stepsTaken += 1;
				}
				if (stepsTaken == path.size() - 1)
				{
					destinationReached = true;
				}
				Vector2 newPos;
				newPos.x = path.at(stepsTaken).x + 5.0f;
				newPos.y = path.at(stepsTaken).y + 5.0f;
				setPosition(newPos);

				moveTimer = moveTimerReset;
			}
		}
	}
}

void StarChaser::draw(Level* level)
{
	DrawTexture(*texture, (int)getPosition().x, (int)getPosition().y, WHITE);
	DrawText(TextFormat("Energy: %i", energy), (int)getPosition().x, (int)getPosition().y - 5, 15, BLACK);
}

float StarChaser::getEnergy()
{
	return (float)energy;
}

void StarChaser::setTexture(Texture& p_texture)
{
	texture = &p_texture;
}