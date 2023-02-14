#pragma once

#include "agent.h"

class StarChaser : public Agent
{
public:

	void sense(Level *level) override;
	void decide()			 override;
	void act(Level* level)	 override;

	void draw()				 override;

	float getEnergy()		 override;

private:

	float energy;


};