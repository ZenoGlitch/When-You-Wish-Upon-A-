#pragma once

#include "agent.h"

class StarChaser : public Agent
{
public:

	StarChaser() = default;
	StarChaser(Texture *p_texture, Vector2 p_targetPosition);

	void sense(Level *level) override;
	void decide()						 override;
	void act(Level *level)				 override;

	//void aStar(Level *level);

	void draw()				 override;

	float getEnergy()		 override;
	void setTexture(Texture& p_texture);

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


};