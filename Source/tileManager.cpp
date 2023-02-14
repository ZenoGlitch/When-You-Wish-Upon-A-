//#include "tileManager.h"
//
//GridManager* GridManager::Instance = nullptr;
//
//GridManager::GridManager()
//{
//}
//
//GridManager* GridManager::GetInstance()
//{
//	if (!Instance)
//		Instance = new GridManager;
//	return Instance;
//}
//
//GridManager::~GridManager()
//{
//}
//
//void GridManager::Update()
//{
//	for (int x = 0; x < grid.rows; x++) {
//		for (int y = 0; y < grid.yTiles; y++) {
//			grid.map[x][y].Update();
//
//			if (grid.map[x][y].SpreadGrass) 
//			{
//				SpreadGrass(x,y);
//				grid.map[x][y].Spreading();
//				grid.map[x][y].SpreadGrass = false;
//			}
//		}
//	}
//}
//
//void GridManager::Draw()
//{
//	for (int x = 0; x < grid.rows; x++) {
//		for (int y = 0; y < grid.yTiles; y++) {
//			switch (grid.map[x][y].GetTileType()) {
//			case Dirt:
//				DrawRectangle(GetScreenWidth() / grid.rows * x, GetScreenHeight() / grid.yTiles * y, GetScreenWidth() / grid.rows, GetScreenHeight() / grid.yTiles, BROWN);
//				break;
//			case Grass:
//				DrawRectangle(GetScreenWidth() / grid.rows * x, GetScreenHeight() / grid.yTiles * y, GetScreenWidth() / grid.rows, GetScreenHeight() / grid.yTiles, GREEN);
//				break;
//			case Rock:
//				DrawRectangle(GetScreenWidth() / grid.rows * x, GetScreenHeight() / grid.yTiles * y, GetScreenWidth() / grid.rows, GetScreenHeight() / grid.yTiles, GRAY);
//				break;
//			default:
//				DrawRectangle(GetScreenWidth() / grid.rows * x, GetScreenHeight() / grid.yTiles * y, GetScreenWidth() / grid.rows, GetScreenHeight() / grid.yTiles, BROWN);
//				break;
//			}
//		}
//	}
//}
//
//Tile GridManager::GetTile(int x, int y)
//{
//	int xPos = (int)floor(x / 40);
//	int yPos = (int)floor(y / 40);
//
//
//	return grid.map[xPos][yPos];
//}
//
//void GridManager::SpreadGrass(int x, int y)
//{
//	for (int i = 0; i < 4; i++) {
//		char dir = "udlr"[rand() % 4];
//
//		if (dir == 'u') {
//			if (grid.map[x][y + 1].GetTileType() == Dirt) 
//			{
//				grid.map[x][y+1].SetTileType(Grass);
//				break;
//			}
//		}
//		if (dir == 'd') {
//			if (grid.map[x][y - 1].GetTileType() == Dirt) 
//			{
//				grid.map[x][y-1].SetTileType(Grass);
//				break;
//			}
//		}
//		if (dir == 'l') {
//			if (grid.map[x - 1][y].GetTileType() == Dirt) 
//			{
//				grid.map[x - 1][y].SetTileType(Grass);
//				break;
//			}
//		}
//		if (dir == 'r') {
//		if (grid.map[x + 1][y].GetTileType() == Dirt) {
//				grid.map[x +1][y].SetTileType(Grass);
//				break;
//			}
//		}
//	}
//
//
//}
