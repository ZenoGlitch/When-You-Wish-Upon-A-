#include "level.h"

#include <iostream>
#include <string>

void Level::Initialize()
{
    //gridManager.Initialize();
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

        int mousePosX = (int)floor(GetMousePosition().x / 96);
        int mousePosY = (int)floor(GetMousePosition().y / 54);

        Tile tile = gridManager.GetTile(mousePosX, mousePosY);

        int tilePosX = (int)(tile.position.x);
        int tilePosY = (int)(tile.position.y);

        //Grid activeGrid = gridManager.grid;
        TileType localTileType = gridManager.grid.map[mousePosX][mousePosY].GetTileType();

        TileType cycledTileType = gridManager.grid.map[mousePosX][mousePosY].CycleTileTypes(localTileType);
        //activeGrid.map[mousePosX][mousePosY].SetTileType(activeGrid.map[mousePosX][mousePosY].CycleTileTypes(tileType));
        gridManager.grid.map[mousePosX][mousePosY].SetTileType(cycledTileType);
        delay = 2.0f;

    }
    else drawingCircle = false;

    gridManager.Update();
    //for (auto& sheep : sheep_agents)
    //{
    //    // TODO: This piece of code needs to be changed to make sure that sense, decide, act, happen at different frequencies.
    //    if (frameCounter >= 3)
    //    {
    //        sheep.sense(this);
    //    }
    //    sheep.decide();
    //    sheep.act(this);
    //}
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

    DrawText(TextFormat("Time: %f", timef), 10, 10, 30, BLACK);

}

void Level::Reset()
{
    // Clear all lists and vectors
    //grass_agents.clear();

    all_agents.clear();
    pending_agents.clear();

    // Spawn agents
    //for (int i = 0; i < 10; i++)
    //{
    //    SpawnAgent(Sheep());
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

Agent* Level::SpawnAgent()
{
    Agent* result = nullptr;

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

    //sheep_agents.remove_if([](Sheep& a) { return a.isDead(); });


    
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
