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
	//sensing = idle;
	//decision = undecided;

	state = idle;
	energy = maxEnergy;
	shouldMoveToStar = true;
	//shouldCarryStar = false;
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

	if (destinationReached && posX == starPosX && posY == starPosy)
	{
		/*sensing = isCarrying;*/
		/*state = isCarrying;*/
		//shouldCarryStar = true;
		shouldMoveToTrade = true;
		shouldMoveToStar = false;
		
		stepsTaken = 0;
		path.clear();
		pathFound = false;
		destinationReached = false;
	}
	else if (destinationReached && posX == tradePosX && posY == tradePosY)
	{
		/*sensing = idle;*/
		/*state = idle;*/
		shouldMoveToStar = true;
		//shouldCarryStar = false;
		shouldMoveToTrade = false;

		stepsTaken = 0;
		path.clear();
		pathFound = false;
		destinationReached = false;
	}

	if (level->starChaserHeldByMouse)
	{
		/*sensing = beingHeld;*/
		/*state = beingHeld;*/
		shouldDeactivate = true;

		//shouldCarryStar = false;
		shouldMoveToStar = false;
		shouldMoveToTrade = false;
		pathFound = false;
	}
	if (!level->starChaserHeldByMouse)
	{
		/*sensing = idle;*/
		/*state = idle;*/
		shouldDeactivate = false;
	}

	//if (/*sensing == idle*/ state == idle && posX != starPosX && posY != starPosy)
	//{
	//	/*sensing = lookingForStar;*/
	//	/*state = lookingForStar;*/

	//}

	if (/*!shouldCarryStar &&*/ !shouldDeactivate && !shouldMoveToStar && !shouldMoveToShip && !shouldMoveToTrade)
	{
		if (posX != level->star.getPosition().x && posY != level->star.getPosition().y)
		{
			shouldMoveToStar = true;
		}
	}


	if (energy <= 0)
	{
		/*sensing = energyLow;*/
		shouldMoveToShip = true;
		//shouldCarryStar = false;
		shouldMoveToTrade = false;
		shouldMoveToStar = false;
		shouldDeactivate = false;

	}


	//if (decision == movingToStar)
	//{

	//}

	//if (level->isDestinationValid(level->star.getPosition()))
	//{
	//	std::vector<Vector2> path = level->pathfind(getPosition(), level->star.getPosition(), level->numberOfSteps);

	//	for (int i = 0; i < path.size() - 1; i++) // Draw the resulting path selected by A*
	//	{
	//		DrawLine((int)path.at(i).x + TileData::size / 2, (int)path.at(i).y + TileData::size / 2, (int)path.at(i + 1).x + TileData::size / 2, (int)path.at(i + 1).y + TileData::size / 2, RED);
	//		
	//	}
	//}
}

void StarChaser::decide()
{
	//if (/*sensing == idle*/ state == idle|| sensing == beingHeld)
	//{
	//	/*decision = undecided;*/
	//	state = undecided; // hmm
	//}
	//if (sensing == lookingForStar)
	//{
	//	decision = movingToStar;
	//}
	//if (sensing == isCarrying)
	//{
	//	decision = movingToTrade;
	//}
	//if (sensing == energyLow)
	//{
	//	decision = movingToShip;
	//}

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
	//if (shouldCarryStar)
	//{
	//	state = isCarrying;
	//}
	if (shouldMoveToTrade)
	{
		state = movingToTrade;
	}
}


			
void StarChaser::act(Level *level)
{
	const float moveEnergyDrain = 1.0f;
	if (/*decision == movingToStar*/ state == movingToStar && !pathFound)
	{
		path = level->pathfind(getPosition(), level->star.getPosition(), level->numberOfSteps);
		if (path.empty())
		{
			assert(false);
		}
		pathFound = true;
	}

	if (/*sensing == isCarrying*/ state == movingToTrade && !pathFound)
	{
		path = level->pathfind(getPosition(), level->tradePost.getPosition(), level->numberOfSteps);
		if (path.empty())
		{
			assert(false);
		}
		pathFound = true;
	}

	//if (path.size() <= 0)
	//{

	//}

	if (!path.empty())
	{
		/*if (decision == movingToStar)
		{*/

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
				//if (/*sensing == isCarrying*/ state == isCarrying)
				//{
				//	level->star.setPosition(getPosition());
				//}
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

				level->star.setPosition(getPosition());
				
				energy -= moveEnergyDrain;
				moveTimer = moveTimerReset;
			}
		}
		/*}*/
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