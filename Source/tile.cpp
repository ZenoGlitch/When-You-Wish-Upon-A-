//#include "Tile.h"
//
//Tile::Tile(TileType _tileType)
//{
//	tileType = _tileType;
//
//	switch (tileType)
//	{
//	case Dirt:
//		_Energy = 0;
//		break;
//	case Grass:
//		_Energy = 10;
//		break;
//	case Rock:
//		_Energy = -1;
//		break;
//	default:
//		break;
//	}
//
//	trampeled = false;
//
//	SpreadGrass = false;
//
//	descision = Grow;
//}
//
//Tile::~Tile()
//{
//}
//
//void Tile::Update()
//{
//	if (tileType != Grass)
//		return;
//
//	switch (state)
//	{
//	case Agent::AgentState::Sensing:
//		Sense();
//		break;
//	case Agent::AgentState::Deciding:
//		Decide();
//		break;
//	case Agent::AgentState::Acting:
//		Act();
//		break;
//	default:
//		break;
//	}
//	Timer();
//}
//
//void Tile::Sense()
//{
//	//set trampled to true if any of the alive agents are trampling on this.
//	state = AgentState::Deciding;
//}
//
//void Tile::Decide()
//{
//	if (_Energy >= 100) {
//		SpreadGrass = true;
//	}
//	state = AgentState::Acting;
//}
//
//void Tile::Act()
//{
//
//	switch (descision)
//	{
//	case Grow:
//		_Energy += 0.2f;
//		break;
//	case Trampled:
//		_Energy -= 0.2f;
//		break;
//	default:
//		break;
//	}
//
//
//	switch (_ShouldSense)
//	{
//	case true:
//		state = AgentState::Sensing;
//		_ShouldSense = false;
//		break;
//	default:
//		state = AgentState::Acting;
//		break;
//	}
//}
//
//TileType Tile::GetTileType()
//{
//	return tileType;
//}
//
//void Tile::SetTileType(TileType _tileType)
//{
//	tileType = _tileType;
//
//	switch (tileType)
//	{
//	case Dirt:
//		_Energy = 0;
//		break;
//	case Grass:
//		_Energy = 10;
//		break;
//	case Rock:
//		_Energy = -1;
//		break;
//	default:
//		break;
//	}
//}
//
//Rectangle Tile::GetTileBox()
//{
//	return Rectangle(_Position.x, _Position.y, (float)GetScreenWidth() / 20, (float)GetScreenHeight() / 20);
//}
//
//void Tile::Spreading()
//{
//	_Energy /= 2;
//}
