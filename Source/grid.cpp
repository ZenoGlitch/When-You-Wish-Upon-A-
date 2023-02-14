#include "grid.h"

#include "math.h"

// TILE STUFF
#pragma region tileFunctions

Tile::Tile(TileType p_tileType)
{
	tileType = p_tileType;
	SetTileType(tileType);

	blocked = false;

	decision = Grow;
}

void Tile::Update()
{

}

void Tile::Sense()
{

}

void Tile::Decide()
{

}

void Tile::Act()
{

}

void Tile::SetPosition(float p_pos_x, float p_pos_y)
{
	position.x = p_pos_x;
	position.y = p_pos_y;
}

void Tile::SetTileType(TileType p_tileType)
{
	tileType = p_tileType;

	switch (tileType)
	{
	case Dirt:
		color = BROWN;
		break;
	case Grass:
		color = GREEN;
		break;
	case Rock:
		color = GRAY;
		break;
	default:
		break;
	}
}

TileType Tile::GetTileType()
{
	return tileType;
}

TileType Tile::CycleTileTypes(TileType p_tileType)
{
	TileType temp = p_tileType;

	if (temp == Dirt)
	{
		//tileType = Grass;
		return Grass;
	}
	if (temp == Grass)
	{
		//tileType = Rock;
		return Rock;
	}
	if (temp == Rock)
	{
		//tileType = Dirt;
		return Dirt;
	}
}

Rectangle Tile::GetTileBox()
{
	return Rectangle(position.x, position.y, (float)GetScreenWidth() / 20, (float)GetScreenHeight() / 20);
}


#pragma endregion


// GRID STUFF
#pragma region gridFunctions

Grid::~Grid(){}

Grid::Grid()
{
	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < columns; y++) {
			map[x][y].SetTileType(GetRandomTileType());

			map[x][y].SetPosition((float)(x * rows), (float)(y * columns));
		}
	}
}

//void Grid::SetTileType(TileType p_tileType)
//{
//	TileType tileType = p_tileType;
//
//	switch (tileType)
//	{
//	case Dirt:
//		g_color = BROWN;
//		break;
//	case Grass:
//		g_color = GREEN;
//		break;
//	case Rock:
//		g_color = GRAY;
//		break;
//	default:
//		break;
//	}
//}

TileType Grid::GetRandomTileType()
{

	int x = GetRandomValue(0, 100);

	if (x > 90)
	{
		return Rock;
	}
	if (x > 80)
	{
		return Grass;
	}
	else
	{
		return Dirt;
	}

}



#pragma endregion


// GRIDMANAGER STUFF
#pragma region grigManagerFunctions

GridManager* GridManager::Instance = nullptr;

GridManager::~GridManager(){}

GridManager* GridManager::GetInstance()
{
	if (!Instance)
	{
		Instance = new GridManager;
	}
	return Instance;
}

void GridManager::Update()
{
	//for (int x = 0; x < grid.rows; x++) {
	//	for (int y = 0; y < grid.columns; y++) {
	//		grid.map[x][y].Update();


	//	}
	//}
}

void GridManager::Draw()
{
	int tileWidth = GetScreenWidth() / grid.rows;
	int tileHeight = GetScreenHeight() / grid.columns;

	for (int x = 0; x < grid.rows; x++) {
		for (int y = 0; y < grid.columns; y++) {
			
			int tilePosX = GetScreenWidth() / grid.rows * x;	
			int tilePosY = GetScreenHeight() / grid.columns * y;
			switch (grid.map[x][y].GetTileType()) {
			case Dirt:
				DrawRectangle(tilePosX, tilePosY, tileWidth, tileHeight, grid.map[x][y].color);
				break;
			case Grass:
				DrawRectangle(tilePosX, tilePosY, tileWidth, tileHeight, grid.map[x][y].color);
				break;
			case Rock:
				DrawRectangle(tilePosX, tilePosY, tileWidth, tileHeight, grid.map[x][y].color);
				break;
			default:
				DrawRectangle(tilePosX, tilePosY, tileWidth, tileHeight, grid.map[x][y].color);
				break;
			}
		}
	}
}


Tile GridManager::GetTile(int x, int y)
{
	//int xPos = (int)floor(x / 40);
	//int yPos = (int)floor(y / 40);

	return grid.map[x][y];
}

#pragma endregion
