#include "grid.h"

#include "math.h"
#include "level.h"

//int TileData::costF() 
//{
//	return costG + costH;
//}

// GRID STUFF
#pragma region gridFunctions

Grid::~Grid(){}

Grid::Grid()
{

	for (int i = 0; i < 3; i++)
	{
		tileData.push_back(TileData());
	}


	int width = TileData::size;
	int height = TileData::size;

	for (int x = 0; x < columns; x++) 
	{
		for (int y = 0; y < rows; y++) 
		{
			map[x][y] = SetTileType(GetRandomTileType());
		}
	}
}

TileType Grid::SetTileType(TileType p_tileType)
{
	TileType tileType = p_tileType;

	switch (tileType)
	{
	case Dirt:
		tileData.at(tileType).color = BROWN;
		return tileType;
		break;
	case Grass:
		tileData.at(tileType).color = GREEN;
		return tileType;
		break;
	case Rock:
		tileData.at(tileType).color = GRAY;
		return tileType;
		break;
	default:
		return tileType;
		break;
	}


}

TileType Grid::CycleTileTypes(TileType p_tileType)
{
	TileType temp = p_tileType;

	temp = (TileType)(temp + 1);

	if (temp == Count)
	{
		temp = (TileType)0;
		SetColor(temp);
	}

	return temp;
}

void Grid::SetColor(TileType p_tileType)
{
	TileType tileType = p_tileType;

	switch (tileType)
	{
	case Dirt:
		tileData.at(tileType).color = BROWN;
		break;
	case Grass:
		tileData.at(tileType).color = GREEN;
		break;
	case Rock:
		tileData.at(tileType).color = GRAY;
		break;
	default:
		break;
	}
}

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
	//for (int x = 0; x < grid.rows; x++) 
	//{
	//	  for (int y = 0; y < grid.columns; y++) 
	//    {
	//	  	grid.map[x][y].Update();
	//	  }
	//}

	//for (int x = 0; x < grid.columns; x++)
	//{
	//	for (int y = 0; y < grid.rows; y++)
	//	{
	//		TileType tile = grid.map[x][y];
	//		if (tile == Dirt)
	//		{
	//			grid.tileData.at(tile).color = BROWN;
	//		}
	//		if (grid.map[x][y] == Grass)
	//		{
	//			grid.tileData.at(tile).color = GREEN;
	//		}
	//		if (grid.map[x][y] == Rock)
	//		{
	//			grid.tileData.at(tile).color = GRAY;
	//		}
	//	}
	//}
}

void GridManager::Draw()
{
	for (int x = 0; x < grid.columns; x++) 
	{
		for (int y = 0; y < grid.rows; y++) 
		{
			TileType tile = GetTile(x,y);

			int width  = TileData::size;
			int height = TileData::size;

			int tilePosX = x * TileData::size;
			int tilePosY = y * TileData::size;

			Color tileColor = grid.tileData.at(tile).color;


			switch (grid.map[x][y]) 
			{
			case Dirt:
				DrawRectangle(tilePosX, tilePosY, width, height, tileColor);
				break;
			case Grass:
				DrawRectangle(tilePosX, tilePosY, width, height, tileColor);
				break;
			case Rock:
				DrawRectangle(tilePosX, tilePosY, width, height, tileColor);
				break;
			default:
				DrawRectangle(tilePosX, tilePosY, width, height, tileColor);
				break;
			}
		}
	}
}


TileType &GridManager::GetTile(int x, int y)
{
	return grid.map[x][y];
}

// get neighbour by grid index
std::vector<Vector2> GridManager::GetNeighbours(/*Level *level,*/ int x, int y)
{
	std::vector<Vector2> neighbours;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}

			int checkX = x + i;
			int checkY = y + j;

			if (checkX >= 0 && checkX < grid.columns && checkY >= 0 && checkY < grid.rows)
			{
				neighbours.push_back(Vector2((float)checkX * TileData::size, (float)checkY * TileData::size));
				/*level->neighbour_positions.push_back(Vector2((float)checkX, (float)checkY));*/
			}
		}
	}

	// returns neighbours positions in pixels
	return neighbours;
}

int GridManager::GetDistance(Vector2 from, Vector2 to)
{
	int result = 0;

	int distanceX = (int)abs(from.x - to.x);
	int distanceY = (int)abs(from.y - to.y);

	//return sqrt((distanceX * distanceX) + (distanceY * distanceY));

	return distanceX + distanceY;

	//if (distanceX > distanceY)
	//{
	//	result = 14 * distanceY + 10 * (distanceX - distanceY);
	//}
	//else
	//{
	//	result = 14 * distanceX + 10 * (distanceY - distanceX);
	//}

	//return result;
}

#pragma endregion
