#include "starChaser.h"

#include "level.h"

StarChaser::StarChaser(Texture *p_texture, Vector2 p_targetPosition)
	: texture(p_texture)
	, targetPosition(p_targetPosition)
	, sensing(idle)
	, decision(undecided)
{
	energy = 50.0f;
}

void StarChaser::sense(Level *level)
{
	
}

void StarChaser::decide()
{

}

void StarChaser::act(Level *level)
{

}

//void StarChaser::aStar(Level *level)
//{
//
//}

void StarChaser::draw()
{
	DrawTexture(*texture, (int)getPosition().x, (int)getPosition().y, WHITE);
}

float StarChaser::getEnergy()
{
	return energy;
}

void StarChaser::setTexture(Texture& p_texture)
{
	texture = &p_texture;
}