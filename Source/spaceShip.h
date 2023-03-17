#pragma once

#include "agent.h"

class SpaceShip : public Agent
{
public:

	SpaceShip() = default;

	void sense(Level* level) override;
	void decide()			 override;
	void act(Level* level)	 override;

	float getEnergy()		 override;

	void draw(Level* level)  override;

	void setTexture(Texture& p_texture);

private:
	Texture* texture;

};