#pragma once

#include "raylib.h"
#include <vector>

class Level;

enum TileType { Dirt, Grass, Rock, Count };


template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

class TileData
{
public:
	static const int size = 60;

	Color color;

	int costF(); // costG + costH
	int costG; // Distance from starting node
	int costH; // Distance from end node

	//Rectangle GetTileBox();

private:

};


class Grid
{
public:
	Grid();
	~Grid();

	static const int columns = 25, rows = 20;

	TileType map[columns][rows];

	std::vector<TileData> tileData;

	TileType GetRandomTileType();
	TileType SetTileType(TileType p_tileType);
	void SetColor(TileType p_tileType);
	TileType CycleTileTypes(TileType p_tileType);

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

	TileType &GetTile(int, int);
	std::vector<TileType> GetNeighbours(Level* level, int x, int y); // Get the neighbouring tiles
	int GetDistance(Vector2 from, Vector2 to); // Get the calculated cost from node "from" to node "to"
	Grid grid;

private:

};