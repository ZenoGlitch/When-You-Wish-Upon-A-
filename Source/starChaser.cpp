#include "starChaser.h"

#include "level.h"

StarChaser::StarChaser(Texture *p_texture, Vector2 p_targetPosition)
	: texture(p_texture)
	, targetPosition(p_targetPosition)
	, sensing(idle)
	, decision(undecided)
{
	energy = 50.0f;
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
		sensing = isCarrying;
		path.clear();
	}
	else if (destinationReached && posX == tradePosX && posY == tradePosY)
	{
		sensing = idle;
		path.clear();
	}

	if (level->starChaserHeldByMouse)
	{
		sensing = idle;
	}

	if (sensing == idle && posX != starPosX && posY != starPosy)
	{
		sensing = lookingForStar;
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
	if (sensing == lookingForStar)
	{
		decision = movingToStar;
		
	}

	if (sensing == isCarrying)
	{
		decision = movingToTrade;
	}

}

	bool pathFound = false;
			int it = 0;
void StarChaser::act(Level *level)
{

	if (/*sensing == lookingForStar &&*/ !pathFound)
	{
		path = level->pathfind(getPosition(), level->star.getPosition(), level->numberOfSteps);
		pathFound = true;
	}


	//if (path.size() <= 0)
	//{

	//}

	if (path.size() > 0)
	{
		/*if (decision == movingToStar)
		{*/
			moveTimer -= GetFrameTime();

			if (moveTimer <= 0)
			{
				if (it < path.size() - 1)
				{
					it += 1;
				}
				if (it == path.size() - 1)
				{
					destinationReached = true;
				}
				setPosition(path.at(it));
				moveTimer = moveTimerReset;
			}
		/*}*/
	}
}

void StarChaser::draw()
{
	DrawTexture(*texture, (int)getPosition().x, (int)getPosition().y, WHITE);
}

float StarChaser::getEnergy()
{
	return energy;
}

void StarChaser::setTexture(Texture& p_texture)
{
	texture = &p_texture;
}