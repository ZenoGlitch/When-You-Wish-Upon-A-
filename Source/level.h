#pragma once

#include "raylib.h"

#include "grid.h"
#include "agent.h"
#include "star.h"
#include "starChaser.h"
#include "tradingPost.h"
#include "spaceShip.h"


#include <list>
#include <unordered_map>
#include <vector>

class Level
{
public: // Functions

	Level() = default;

	void Initialize();
	void Update();
	void Draw();

	void Reset();

	Agent *GetAgent(int id);

		  
	Agent *SpawnAgent(std::string agentType);

	// @AddMoreHere

	int getId(Agent *agent);

	void unload_textures();

	std::vector<Vector2> pathfind(Vector2 start, Vector2 end, int maxSteps);
	std::vector<Vector2> retracePath(Vector2 startTile, Vector2 endTile, int index);

	Vector2 PositionOnRandomTile();
	bool isDestinationValid(Vector2 p_destination_position);

private: // Functions

	void input();

	void remove_dead_and_add_pending_agents();
	// Remember, if you add more lists (see @AddMoreHere), edit this function so that dead agents are removed correctly without leaking memory

	void load_textures();
	void set_texures();

	void set_spawn_positions();

	/*void SetNeighboursGandHCosts(std::vector<TileType> neighbours, Vector2 startTilePos, Vector2 end, int tileHalfSize);*/
	/*void SetNeighbourPositions(std::vector<TileType> neighbours);*/


public: // Containers and variables

	int numberOfSteps = 1;

	int last_id = 0;

	struct pathfindingNode 
	{
		int costF(); // costG + costH
		int costG; // Distance from starting node
		int costH; // Distance from end node

		Vector2 position;
		Vector2 origin();
		pathfindingNode *getNode(Vector2 position);

		Vector2 parentPosition;
	};

	std::vector<pathfindingNode> open_tiles;
	std::vector<pathfindingNode> closed_tiles;
	std::vector<Vector2> neighbour_positions;

	//NOTE(Filippo): Using a list here is not the best idea, ideally you should store agents in some other data structure that keeps them close to each other while being pointer-stable.

	//std::list<Star> star_agents;
	std::list<StarChaser> starChaser_agents;
	//std::list<TradingPost> tradingPost_agents;
	
	// @AddMoreHere

	std::unordered_map<int, Agent*> id_to_agent;
	std::vector<Agent*> all_agents;

	std::vector<Agent*> pending_agents; // Agents that will be added at the beginning of the next frame

	GridManager gridManager;

	Star star;
	TradingPost tradePost;
	StarChaser starChaser;
	SpaceShip spaceShip;

	Texture star_texture;
	Texture tradePost_texture;
	Texture starChaser_texture;
	Texture spaceShip_texture;

	bool starChaserHeldByMouse = false;
	bool starHeldByMouse = false;
	bool spaceShipHeldByMouse = false;
	bool tradePostHeldByMouse = false;

	int starsCollected = 0;


private: // Containers and variables

	int frameCounter = 0;

	float timef = 0;

	bool drawingCircle = false;
	bool drawingOpenAndClosedTiles = false;
	bool drawControlScheme = false;

	float delay = 2.0f;

	const int tileSize = TileData::size;

};