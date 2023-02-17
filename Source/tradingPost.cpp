#include "tradingPost.h"

#include "level.h"

TradingPost::TradingPost(Level *level)
	: texture(&level->tradePost_texture)
{
	energy = 50.0f;
	setPosition(Vector2(500.0f, 500.0f));
}

void TradingPost::sense(Level* level)
{

}

void TradingPost::decide()
{

}

void TradingPost::act(Level* level)
{

}

void TradingPost::draw()
{
	DrawTexture(*texture, (int)getPosition().x, (int)getPosition().y, WHITE);
}

float TradingPost::getEnergy()
{
	return energy;
}

void TradingPost::setTexture(Texture& p_texture)
{
	texture = &p_texture;
}
