#pragma once

#include "agent.h"

class SpaceShip : public Agent
{
public:

	SpaceShip() = default;
	//StarChaser(Texture *p_texture, Vector2 p_targetPosition);

	void initialize();
	void sense(Level* level) override;
	void decide()			 override;
	void act(Level* level)	 override;

	float getEnergy()		 override;

	void draw(Level* level)  override;

	void setTexture(Texture& p_texture);

private:
	Texture* texture;

};