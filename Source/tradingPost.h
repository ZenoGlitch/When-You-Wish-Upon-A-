#pragma once

#include "agent.h"

class TradingPost : public Agent
{
public:
	TradingPost() = default;
	TradingPost(Level *level);

	void sense(Level* level) override;
	void decide()			 override;
	void act(Level* level)	 override;

	void draw(Level *level)	 override;

	float getEnergy()		 override;
	void setTexture(Texture& p_texture);

private:

	float energy;
	Texture *texture;
};