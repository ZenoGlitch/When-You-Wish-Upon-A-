#pragma once

#include "agent.h"

#include <vector>

class StarChaser : public Agent
{
public:

	StarChaser() = default;
	StarChaser(Texture *p_texture, Vector2 p_targetPosition);

	void sense(Level *level) override;
	void decide()			 override;
	void act(Level *level)	 override;

	void draw()				 override;

	float getEnergy()		 override;

	void setTexture(Texture& p_texture);

	std::vector<Vector2> path;

	bool destinationReached = false;

private:

	enum Sense
	{
		lookingForStar, isCarrying, idle
	} sensing = idle;

	enum Decide
	{
		movingToStar, movingToTrade, undecided
	} decision = undecided;


	Vector2 targetPosition;

	Texture *texture;

	float energy;

	const float moveTimerReset = 1.0f;
	float moveTimer = moveTimerReset;

};