#pragma once

#include "raylib.h"

class Level;

enum TileType { Dirt = 0, Grass = 1, Rock = 2 };

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

class Tile
{
public:
	Tile(TileType p_tileType = Dirt);
	~Tile() = default;

	void Update();

	void Sense();
	void Decide();
	void Act();

	void SetPosition(float p_pos_x, float p_pos_y);

	void SetTileType(TileType p_tileType);
	TileType GetTileType();
	TileType CycleTileTypes(TileType p_tileType);

	bool blocked;

	Rectangle GetTileBox();

	Vector2 position;
	Color color;

	TileType tileType;

private:
	enum GrassDecision { Grow, Trampled } decision;

};


class Grid
{
public:
	Grid();
	~Grid();

	static const int rows = 20, columns = 20;

	Tile map[rows][columns];

	//void SetTileType(TileType p_tileType);
	TileType GetRandomTileType();

	//Color g_color;
private:


};

class GridManager
{
public:
	GridManager() = default;
	~GridManager();

	static GridManager* Instance;
	static GridManager* GetInstance();

	void Update();
	void Draw();

	Tile GetTile(int, int);

	Grid grid;

private:

};