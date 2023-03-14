#pragma once

#include "agent.h"

#include <vector>

class StarChaser : public Agent
{
public:

	StarChaser() = default;
	//StarChaser(Texture *p_texture, Vector2 p_targetPosition);

	void initialize();
	void sense(Level *level) override;
	void decide()			 override;
	void act(Level *level)	 override;

	void draw(Level * level) override;

	float getEnergy()		 override;

	void setTexture(Texture& p_texture);

	std::vector<Vector2> path;

	bool destinationReached = false;

private:

	enum State
	{
		energyLow, 
		movingToStar, 
		movingToTrade, 
		movingToShip, 
		beingHeld, 
		idle,

	} state = idle;

	Vector2 targetPosition;

	Texture *texture;

	const int maxEnergy = 15;
	int energy;

	int stepsTaken = 0;

	const float moveTimerReset = 1.0f;
	float moveTimer = moveTimerReset;
	bool pathFound = false;

	bool shouldMoveToStar = true;
	bool shouldMoveToTrade = false;
	bool shouldMoveToShip = false;

	bool shouldDeactivate = false;


};