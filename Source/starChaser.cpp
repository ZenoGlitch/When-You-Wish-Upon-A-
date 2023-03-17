#include "starChaser.h"

#include "level.h"
#include <cassert>

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
	float shipPosX = level->spaceShip.getPosition().x;
	float shipPosY = level->spaceShip.getPosition().y;

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
		shouldRechargeEnergy = true;

		shouldMoveToStar = false;
		shouldMoveToStar = false;
		shouldMoveToTrade = false;

		stepsTaken = 0;
		path.clear();
		pathFound = false;
		destinationReached = false;
	}

	if (level->starChaserHeldByMouse || level->starHeldByMouse || level->tradePostHeldByMouse || level->spaceShipHeldByMouse)
	{
		shouldDeactivate = true;

		shouldMoveToStar = false;
		shouldMoveToTrade = false;
		shouldMoveToShip = false;
		shouldRechargeEnergy = false;

		stepsTaken = 0;
		path.clear();
		pathFound = false;
		destinationReached = false;
	}
	if (!level->starChaserHeldByMouse)
	{
		shouldDeactivate = false;
	}

	if (energy <= 0)
	{
		stepsTaken = 0;
		path.clear();
		pathFound = false;
		destinationReached = false;

		shouldMoveToShip = true;
		fullyCharged = false;

		shouldMoveToTrade = false;
		shouldMoveToStar = false;
		shouldDeactivate = false;
	}

	if (!shouldDeactivate && !shouldMoveToStar && !shouldMoveToShip && !shouldMoveToTrade && !shouldRechargeEnergy)
	{
		if (posX != level->star.getPosition().x && posY != level->star.getPosition().y)
		{
			shouldMoveToStar = true;
		}
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
	if (shouldRechargeEnergy)
	{
		state = recharging;
	}
}
			
void StarChaser::act(Level *level)
{
	const int moveEnergyDrain = 1;
	if (state == movingToStar && !pathFound && !level->starHeldByMouse)
	{
		path = level->pathfind(getPosition(), level->star.getPosition(), level->numberOfSteps);
		if (path.empty())
		{
			assert(false);
		}
		pathFound = true;
	}

	if (state == movingToTrade && !pathFound && !level->tradePostHeldByMouse)
	{
		path = level->pathfind(getPosition(), level->tradePost.getPosition(), level->numberOfSteps);
		if (path.empty())
		{
			assert(false);
		}
		pathFound = true;
	}

	if (state == movingToShip && !pathFound && !level->spaceShipHeldByMouse)
	{
		path = level->pathfind(getPosition(), level->spaceShip.getPosition(), level->numberOfSteps);
		if (path.empty())
		{
			assert(false);
		}
		pathFound = true;
	}

	if (state == recharging && !level->spaceShipHeldByMouse)
	{
		moveTimer -= GetFrameTime();
		if (energy < maxEnergy && moveTimer <= 0)
		{
			energy += 3;
			moveTimer = moveTimerReset;
			fullyCharged = false;
		}
		if (energy >= maxEnergy)
		{
			shouldRechargeEnergy = false;
			energy = maxEnergy;
			moveTimer = moveTimerReset;
			fullyCharged = true;

			stepsTaken = 0;
			path.clear();
			pathFound = false;
			destinationReached = false;

			shouldMoveToStar = true;

			shouldMoveToTrade = false;
			shouldMoveToShip = false;
			shouldDeactivate = false;
		}
	}

	if (!path.empty() && state != beingHeld)
	{
		if (state == movingToStar && !level->starHeldByMouse)
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
				if (stepsTaken <= path.size() - 1)
				{
					newPos.x = path.at(stepsTaken).x + 5.0f;
					newPos.y = path.at(stepsTaken).y + 5.0f;
					setPosition(newPos);
				}
				energy -= moveEnergyDrain;
				moveTimer = moveTimerReset;
			}
		}

		if (state == movingToTrade && !level->tradePostHeldByMouse)
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
				if (stepsTaken <= path.size() - 1)
				{
					newPos.x = path.at(stepsTaken).x + 5.0f;
					newPos.y = path.at(stepsTaken).y + 5.0f;
					setPosition(newPos);
				}

				level->star.setPosition(getPosition()); // move the star with the starChaser
				
				energy -= moveEnergyDrain;
				moveTimer = moveTimerReset;
			}
		}

		if (state == movingToShip && !level->spaceShipHeldByMouse)
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
				if (stepsTaken <= path.size() - 1)
				{
					newPos.x = path.at(stepsTaken).x + 5.0f;
					newPos.y = path.at(stepsTaken).y + 5.0f;
					setPosition(newPos);
				}

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

void StarChaser::rechargeEnergy()
{
	moveTimer -= GetFrameTime();
	if (energy < maxEnergy && moveTimer <= 0)
	{
		energy += 3;
		moveTimer = moveTimerReset;
		fullyCharged = false;
	}
	if (energy >= maxEnergy)
	{
		energy = maxEnergy;
		moveTimer = moveTimerReset;
		fullyCharged = true;
	}
}