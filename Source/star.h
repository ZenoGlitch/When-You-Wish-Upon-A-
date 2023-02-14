#pragma once

#include "agent.h"

class Star : public Agent
{
public:

	Star();

	void sense(Level* level) override;
	void decide()			 override;
	void act(Level* level)	 override;

	void draw()				 override;

	float getEnergy()		 override;

private:

	Texture2D texture;


	float energy;

};