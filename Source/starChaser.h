#pragma once

#include "agent.h"

class StarChaser : public Agent
{
public:

	StarChaser();

	void sense(Level *level) override;
	void decide()			 override;
	void act(Level* level)	 override;

	void draw()				 override;

	float getEnergy()		 override;

private:

	float energy;


};