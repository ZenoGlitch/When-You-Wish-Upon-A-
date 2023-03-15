#pragma once

#include "agent.h"

//class Level;

class Star : public Agent
{
public:

	Star() = default;
	Star(Level* level);

	void sense(Level* level) override;
	void decide()			 override;
	void act(Level* level)	 override;

	void draw(Level* level)	 override;

	float getEnergy()		 override;

	void setTexture(Texture &p_texture);

private:

	Texture2D *texture;

	enum Decision
	{
		respawn, idle
	} decision = idle;

	float energy = 50.0f;
	bool beingCarried = false;
	bool shouldRespawn = false;

};