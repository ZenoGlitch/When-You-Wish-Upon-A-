#include "level.h"

#include <iostream>
#include <string>

void Level::Initialize()
{
    //gridManager.Initialize();
    load_textures();
    star.setTexture(star_texture);
    tradePost.setTexture(tradePost_texture);
    
    Vector2 starSpawnPos = PositionOnRandomTile();
    star.setPosition(starSpawnPos);

    Vector2 tradePostSpawnPos = PositionOnRandomTile();
    tradePost.setPosition(tradePostSpawnPos);
}

void Level::Update()
{
    remove_dead_and_add_pending_agents();

    frameCounter++;
    timef += GetFrameTime();
    delay -= GetFrameTime();

    
    if (IsMouseButtonPressed(0))
    {
        drawingCircle = true;

        int mousePosX = (int)floor(GetMousePosition().x / TileData::size);
        int mousePosY = (int)floor(GetMousePosition().y / TileData::size);

        TileType &tile = gridManager.GetTile(mousePosX, mousePosY);

        TileData tileData = gridManager.grid.tileData.at(tile);

        tile = gridManager.grid.CycleTileTypes(tile);
        //tileData.color = gridManager.grid.SetColor(tile);

        delay = 2.0f;

    }
    else drawingCircle = false;

    gridManager.Update();
}

void Level::Draw()
{
    gridManager.Draw();

    //for (auto&& grass : grass_agents)
    //{
    //    grass.draw();
    //}

    if (drawingCircle)
    {
        DrawCircle((int)GetMousePosition().x, (int)GetMousePosition().y, 10, RED);
    }

    star.draw();
    tradePost.draw();

    DrawText(TextFormat("Time: %f", timef), 10, 10, 30, BLACK);

}

void Level::Reset()
{
    // Clear all lists and vectors

    //star_agents.clear();

    all_agents.clear();
    pending_agents.clear();

    // Spawn agents
    //for (int i = 0; i < 10; i++)
    //{
    //    SpawnAgent(Sheep());
    //}
    
    //for (int i = 0; i < 1; i++)
    //{
    //    SpawnAgent("star");
    //}

}

Agent* Level::GetAgent(int id)
{
    auto agent_iterator = id_to_agent.find(id);
    if (agent_iterator != id_to_agent.end())
    {
        return agent_iterator->second;
    }
    return nullptr;
}

Agent* Level::SpawnAgent(std::string agentType)
{
    Agent* result = nullptr;

    //if (agentType == "star")
    //{
    //    Star agent(star_texture);
    //    Vector2 randPos = { (float)(rand() & GetScreenWidth()), 10 };
    //    agent.setPosition(randPos);
    //    star_agents.push_back(agent);
    //    result = &star_agents.back();
    //}
    //else if (agentType == "starChaser")
    //{
    //    StarChaser agent;
    //    Vector2 randPos = { (float)(rand() & GetScreenWidth()), (float)(rand() & GetScreenHeight()) };
    //    Tile spawnTile = gridManager.GetTile((int)(randPos.x / 96), (int)(randPos.y / 54));
    //    Vector2 spawnPos = spawnTile.position;
    //    spawnPos.x = spawnTile.position.x + spawnTile.width / 2;
    //    spawnPos.y = spawnTile.position.y + spawnTile.height / 2;
    //    agent.setPosition(spawnPos);
    //    starChaser_agents.push_back(agent);
    //    result = &starChaser_agents.back();
    //}
    //else if (agentType == "tradingPost")
    //{
    //    TradingPost agent;
    //    Vector2 randPos = { (float)(rand() & GetScreenWidth()), (float)(rand() & GetScreenHeight()) };
    //    Tile spawnTile = gridManager.GetTile((int)(randPos.x / 96), (int)(randPos.y / 54));
    //    Vector2 spawnPos = spawnTile.position;
    //    spawnPos.x = spawnTile.position.x + spawnTile.width / 2;
    //    spawnPos.y = spawnTile.position.y + spawnTile.height / 2;
    //    agent.setPosition(spawnPos);
    //    tradingPost_agents.push_back(agent);
    //    result = &tradingPost_agents.back();
    //}
    //else
    //{
    //    return nullptr;
    //}

    //Vector2 randPosition = { (float)(rand() % GetScreenWidth()), (float)(rand() % GetScreenHeight()) };
    //agent.setPosition(randPosition);
    //sheep_agents.push_back(agent);
    //result = &sheep_agents.back();

    //pending_agents.push_back(result); // Enqueue it so that it can be added to the level at the beginning of the next frame

    return result;
}

int Level::getId(Agent* agent)
{
    for (auto agents : all_agents)
    {
        if (agents == agent)
        {
            return agents->id;
        }
    }
    return -1;
}

std::vector<Vector2> Level::pathfind(Vector2 start, Vector2 end)
{
    std::vector<Vector2> result;

    result.push_back(start);
    result.push_back(end);

    return result;
}

void Level::remove_dead_and_add_pending_agents()
{
    auto agent_iterator = all_agents.begin();
    while (agent_iterator != all_agents.end())
    {
        if ((*agent_iterator)->isDead())
        {
            id_to_agent.erase((*agent_iterator)->id);
            agent_iterator = all_agents.erase(agent_iterator);
        }
        else
        {
            agent_iterator++;
        }
    }

    // This must happen _after_ we remove agents from the vector 'all_agents'.
    // @AddMoreHere

    //star_agents.remove_if([](Star& a) { return a.isDead(); });

    
    // Add all pending agents
    for (Agent* agent : pending_agents)
    {
        last_id += 1;
        agent->id = last_id;

        all_agents.push_back(agent);
        id_to_agent.insert({ agent->id, agent });
    }

    pending_agents.clear(); // Important that we clear this, otherwise we'll add the same agents every frame.
}

void Level::load_textures()
{
    star_texture = LoadTexture("assets/star2.png");

    //starChaser_texture = LoadTexture("assets/starChaser.png");
    tradePost_texture = LoadTexture("assets/tradePost.png");
}

void Level::unload_textures()
{
    UnloadTexture(star_texture);
    UnloadTexture(tradePost_texture);
    //UnloadTexture(starChaser_texture);

}

Vector2 Level::PositionOnRandomTile()
{
    Vector2 result;

    int randTileX = rand() % gridManager.grid.columns;
    int randTileY = rand() % gridManager.grid.rows;
    int offset = 5;

    result = { (float)(randTileX * TileData::size + offset), (float)(randTileY * TileData::size + offset) };

    return result;
}