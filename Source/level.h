#pragma once

#include "raylib.h"

#include "grid.h"
#include "agent.h"
#include "star.h"
#include "starChaser.h"
#include "tradingPost.h"


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

public: // containers and variables

	int last_id = 0;

	//NOTE(Filippo): Using a list here is not the best idea, ideally you should store agents in some other data structure that keeps them close to each other while being pointer-stable.

	//std::list<Star> star_agents;
	//std::list<StarChaser> starChaser_agents;
	//std::list<TradingPost> tradingPost_agents;
	
	// @AddMoreHere

	std::unordered_map<int, Agent*> id_to_agent;
	std::vector<Agent*> all_agents;

	std::vector<Agent*> pending_agents; // Agents that will be added at the beginning of the next frame

	GridManager gridManager;

	Texture star_texture;
	Texture tradePost_texture;
	Texture starChaser_texture;

private: // functions

	std::vector<Vector2> pathfind(Vector2 start, Vector2 end);

	void remove_dead_and_add_pending_agents();
	// Remember, if you add more lists (see @AddMoreHere), edit this function so that dead agents are removed correctly without leaking memory

	void load_textures();

	Vector2 PositionOnRandomTile();

private: // Containers and variables

	int frameCounter = 0;

	float timef = 0;

	bool drawingCircle = false;

	Star star;
	TradingPost tradePost;

	float delay = 2.0f;

};