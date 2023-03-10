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

	//enum Sense
	//{
	//	energyLow, lookingForStar, isCarrying, idle, beingHeld
	//} sensing = idle;

	//enum Decide
	//{
	//	movingToStar, movingToTrade, movingToShip, undecided
	//} decision = undecided;

	enum State
	{
		energyLow, 
		lookingForStar, 
		isCarrying, 
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

	const float moveTimerReset = 1.0f;
	float moveTimer = moveTimerReset;
	bool pathFound = false;

};