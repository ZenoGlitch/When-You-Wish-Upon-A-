#include "level.h"

#include <iostream>
#include <string>

void Level::Initialize()
{
    //gridManager.Initialize();
    load_textures();
    set_texures();

    set_spawn_positions();
    

}

void Level::Update()
{
    remove_dead_and_add_pending_agents();

    frameCounter++;
    timef += GetFrameTime();
    delay -= GetFrameTime();

    // Cycle tile type when tile is clicked
    if (IsMouseButtonPressed(0))
    {
        drawingCircle = true;

        int mousePosX = (int)floor(GetMousePosition().x / TileData::size);
        int mousePosY = (int)floor(GetMousePosition().y / TileData::size);

        TileType &tile = gridManager.GetTile(mousePosX, mousePosY);

        //TileData tileData = gridManager.grid.tileData.at(tile);

        tile = gridManager.grid.CycleTileTypes(tile);

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
    starChaser.draw();

    //Vector2 starChaserOrigin = { starChaser.getPosition().x - 5 + TileData::size / 2, starChaser.getPosition().y - 5 + TileData::size / 2 };
    //Vector2 starOrigin = { star.getPosition().x - 5 + TileData::size / 2, star.getPosition().y - 5 + TileData::size / 2 };
    pathfind(starChaser.getPosition(), star.getPosition());


    if (IsKeyDown(KEY_B))
    {
        if (open_tiles.size() > 0)
        {
            for (int i = 0; i < open_tiles.size(); i++)
            { 
                Color transparentBlue = { 0, 121, 241, 5 };
            
                DrawRectangle((int)open_tiles.at(i).x, (int)open_tiles.at(i).y, TileData::size, TileData::size, transparentBlue);
            }    
        }

        if (closed_tiles.size() > 0)
        {
            for (int i = 0; i < closed_tiles.size(); i++)
            {
                Color transparentRed = { 230, 41, 55, 5 };

                DrawRectangle((int)closed_tiles.at(i).x, (int)closed_tiles.at(i).y, TileData::size, TileData::size, transparentRed);
            }
        }
    }


    DrawText(TextFormat("Time: %f", timef), 10, 10, 30, BLACK);

}

void Level::Reset()
{
    // Clear all lists and vectors
    open_tiles.clear();
    closed_tiles.clear();


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

    int offset = 5;
    Vector2 startTilePos = { start.x - offset, start.y - offset };
    Vector2 endTilePos = { end.x - offset, end.y - offset };

    result.push_back(startTilePos);
    result.push_back(endTilePos);

    if (open_tiles.size() == 0)
    {
        open_tiles.push_back(startTilePos);
    }


    // look at "start" surrounding 8 nodes
    int tileHalfSize = TileData::size / 2;

    Vector2 startTileOriginPos = { startTilePos.x + tileHalfSize, startTilePos.y + tileHalfSize };
    Vector2 startTileIndex = { (start.x - offset) / TileData::size, (start.y - offset) / TileData::size };

    Vector2 endTileOriginPos = { endTilePos.x + tileHalfSize, endTilePos.y + tileHalfSize };

    //std::vector<TileType> neighbours = gridManager.GetNeighbours((int)startTileIndex.x, (int)startTileIndex.y);

    //if (neighbours.size() > 0)
    //{
    //    SetNeighboursGandHCosts(neighbours, startTilePos, end, tileHalfSize);
    //}

    // find lowest F cost

    TileType currentTile = gridManager.GetTile((int)open_tiles.at(0).x / TileData::size, (int)open_tiles.at(0).y / TileData::size);
    Vector2 currentTilePos = startTilePos;
    while (open_tiles.size() > 0)
    {

        for (int i = 1; i < open_tiles.size(); i++)
        {
            if (gridManager.grid.tileData.at(i).costF() < gridManager.grid.tileData.at(currentTile).costF()
                || gridManager.grid.tileData.at(i).costF() == gridManager.grid.tileData.at(currentTile).costF()
                && gridManager.grid.tileData.at(i).costH < gridManager.grid.tileData.at(currentTile).costH)
            {
                currentTile = gridManager.GetTile((int)open_tiles.at(i).x / TileData::size, (int)open_tiles.at(i).y / TileData::size);
                currentTilePos = open_tiles.at(i);
            }
        }

        open_tiles.erase(open_tiles.begin());
        closed_tiles.push_back(Vector2(open_tiles.at(0).x, open_tiles.at(0).y));

        if (gridManager.GetTile((int)end.x / TileData::size, (int)end.y / TileData::size) == currentTile)
        {
            break;
        }

        for (auto& neighbour : gridManager.GetNeighbours(this, (int)currentTilePos.x / TileData::size, (int)currentTilePos.y / TileData::size))
        {
            Vector2 parentTilePos = {};
            if (neighbour == Rock)
            {
                continue;
            }
            //if (std::find(closed_tiles.begin(), closed_tiles.end(), neighbour_positions.at(neighbour)) != closed_tiles.end())
            //{
            //    continue;
            //}

            int newMoveCostToNeighbour = gridManager.grid.tileData.at(currentTile).costG + gridManager.GetDistance(currentTilePos, neighbour_positions.at(neighbour));
            if (newMoveCostToNeighbour < gridManager.grid.tileData.at(gridManager.GetTile((int)neighbour_positions.at(neighbour).x, (int)neighbour_positions.at(neighbour).y)).costG)
            {
                gridManager.grid.tileData.at(gridManager.GetTile((int)neighbour_positions.at(neighbour).x, (int)neighbour_positions.at(neighbour).y)).costG = newMoveCostToNeighbour;
                gridManager.grid.tileData.at(gridManager.GetTile((int)neighbour_positions.at(neighbour).x, (int)neighbour_positions.at(neighbour).y)).costH = gridManager.GetDistance(neighbour_positions.at(neighbour), endTilePos);
                parentTilePos = currentTilePos;
            }

            bool openContainsNeighbour = false;
            //if (std::find(open_tiles.begin(), open_tiles.end(), neighbour_positions.at(neighbour)) != open_tiles.end())
            //{
            //    openContainsNeighbour = true;
            //}
            if (!openContainsNeighbour)
            {
                gridManager.grid.tileData.at(gridManager.GetTile((int)neighbour_positions.at(neighbour).x, (int)neighbour_positions.at(neighbour).y)).costG = newMoveCostToNeighbour;
                gridManager.grid.tileData.at(gridManager.GetTile((int)neighbour_positions.at(neighbour).x, (int)neighbour_positions.at(neighbour).y)).costH = gridManager.GetDistance(neighbour_positions.at(neighbour), endTilePos);
                parentTilePos = currentTilePos;
            }
        }
    }

        //for (int i = 1; i < neighbours.size(); i++)
        //{
        //    int currentF = gridManager.grid.tileData.at(neighbours.at(i)).costF();
        //    int previousF = gridManager.grid.tileData.at(neighbours.at(i-1)).costF();

        //    int currentH = gridManager.grid.tileData.at(neighbours.at(i)).costH;
        //    int previousH = gridManager.grid.tileData.at(neighbours.at(i-1)).costH;

        //    TileType currentTile = gridManager.GetTile((int)neighbour_positions.at(i).x / TileData::size, 
        //                                               (int)neighbour_positions.at(i).y / TileData::size);


        //    if (currentF < previousF && currentTile != Rock) //  SOMETHING IS GOING WRONG HERE, PLEASE HELP!!!
        //    {
        //        if (open_tiles.size() > 0)
        //        {
        //            open_tiles.push_back(neighbour_positions.at(i));
        //            closed_tiles.push_back(neighbour_positions.at(i-1));
        //            open_tiles.erase(open_tiles.begin());
        //            break;
        //        }
        //    }
        //    else if (currentF == previousF && currentH < previousH  && currentTile != Rock) //  SOMETHING IS GOING WRONG HERE, PLEASE HELP!!!
        //    {
        //        if (open_tiles.size() > 0)
        //        {
        //            open_tiles.push_back(neighbour_positions.at(i));
        //            closed_tiles.push_back(neighbour_positions.at(i-1));
        //            open_tiles.erase(open_tiles.begin());
        //            break;
        //        }
        //    } 
        //    else if (currentF > previousF && currentTile != Rock)
        //    {
        //        if (open_tiles.size() > 0)
        //        {
        //            open_tiles.push_back(neighbour_positions.at(i-1));
        //            closed_tiles.push_back(neighbour_positions.at(i));
        //            open_tiles.erase(open_tiles.begin());
        //            break;
        //        }
        //    }
        //}


    DrawLine((int)startTileOriginPos.x, (int)startTileOriginPos.y, (int)endTileOriginPos.x, (int)endTileOriginPos.y, RED);

    return result;
}

void Level::SetNeighboursGandHCosts(std::vector<TileType> neighbours, Vector2 startTilePos, Vector2 end, int tileHalfSize)
{
    for (int i = -1; i <= 1 ; i++)
    {
        for (int j = -1; j <= 1 ; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }

            int checkX = (int)startTilePos.x + i;
            int checkY = (int)startTilePos.y + j;

            if (checkX >= 0 && checkX < gridManager.grid.columns && checkY >= 0 && checkY < gridManager.grid.rows)
            {
                int tileX = (int)startTilePos.x / TileData::size + i;
                int tileY = (int)startTilePos.y / TileData::size + j;
                gridManager.grid.tileData.at(gridManager.GetTile(tileX, tileY)).costG = 10;
                // figure out how to do this for all neighbours in a good way
            }
        }
    }

    gridManager.grid.tileData.at(neighbours.at(0)).costG = 14;
    gridManager.grid.tileData.at(neighbours.at(1)).costG = 10;
    gridManager.grid.tileData.at(neighbours.at(2)).costG = 14;
    gridManager.grid.tileData.at(neighbours.at(3)).costG = 10;
    gridManager.grid.tileData.at(neighbours.at(4)).costG = 10;
    gridManager.grid.tileData.at(neighbours.at(5)).costG = 14;
    gridManager.grid.tileData.at(neighbours.at(6)).costG = 10;
    gridManager.grid.tileData.at(neighbours.at(7)).costG = 14;

    Vector2 neighbour1OriginPos = { startTilePos.x - tileHalfSize ,startTilePos.y - tileHalfSize };
    Vector2 neighbour2OriginPos = { startTilePos.x                ,startTilePos.y - tileHalfSize };
    Vector2 neighbour3OriginPos = { startTilePos.x + tileHalfSize ,startTilePos.y - tileHalfSize };
    Vector2 neighbour4OriginPos = { startTilePos.x - tileHalfSize ,startTilePos.y                };
    Vector2 neighbour5OriginPos = { startTilePos.x + tileHalfSize ,startTilePos.y                };
    Vector2 neighbour6OriginPos = { startTilePos.x - tileHalfSize ,startTilePos.y + tileHalfSize };
    Vector2 neighbour7OriginPos = { startTilePos.x                ,startTilePos.y + tileHalfSize };
    Vector2 neighbour8OriginPos = { startTilePos.x + tileHalfSize ,startTilePos.y + tileHalfSize };

    

    if (neighbour_positions.size() == 0)
    {
        neighbour_positions.push_back(Vector2(neighbour1OriginPos.x - tileHalfSize, neighbour1OriginPos.y - tileHalfSize));
        neighbour_positions.push_back(Vector2(neighbour2OriginPos.x               , neighbour2OriginPos.y - tileHalfSize));
        neighbour_positions.push_back(Vector2(neighbour3OriginPos.x + tileHalfSize, neighbour3OriginPos.y - tileHalfSize));
        neighbour_positions.push_back(Vector2(neighbour4OriginPos.x - tileHalfSize, neighbour4OriginPos.y               ));
        neighbour_positions.push_back(Vector2(neighbour5OriginPos.x + tileHalfSize, neighbour5OriginPos.y               ));
        neighbour_positions.push_back(Vector2(neighbour6OriginPos.x - tileHalfSize, neighbour6OriginPos.y + tileHalfSize));
        neighbour_positions.push_back(Vector2(neighbour7OriginPos.x               , neighbour7OriginPos.y + tileHalfSize));
        neighbour_positions.push_back(Vector2(neighbour8OriginPos.x + tileHalfSize, neighbour8OriginPos.y + tileHalfSize));
    }

    

    gridManager.grid.tileData.at(neighbours.at(0)).costH = gridManager.GetDistance(neighbour1OriginPos, end);
    gridManager.grid.tileData.at(neighbours.at(1)).costH = gridManager.GetDistance(neighbour2OriginPos, end);
    gridManager.grid.tileData.at(neighbours.at(2)).costH = gridManager.GetDistance(neighbour3OriginPos, end);
    gridManager.grid.tileData.at(neighbours.at(3)).costH = gridManager.GetDistance(neighbour4OriginPos, end);
    gridManager.grid.tileData.at(neighbours.at(4)).costH = gridManager.GetDistance(neighbour5OriginPos, end);
    gridManager.grid.tileData.at(neighbours.at(5)).costH = gridManager.GetDistance(neighbour6OriginPos, end);
    gridManager.grid.tileData.at(neighbours.at(6)).costH = gridManager.GetDistance(neighbour7OriginPos, end);
    gridManager.grid.tileData.at(neighbours.at(7)).costH = gridManager.GetDistance(neighbour8OriginPos, end);
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
    tradePost_texture = LoadTexture("assets/tradePost.png");
    starChaser_texture = LoadTexture("assets/starChaser.png");
}

void Level::unload_textures()
{
    UnloadTexture(star_texture);
    UnloadTexture(tradePost_texture);
    UnloadTexture(starChaser_texture);
}

void Level::set_texures()
{
    star.setTexture(star_texture);
    tradePost.setTexture(tradePost_texture);
    starChaser.setTexture(starChaser_texture);
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

void Level::set_spawn_positions()
{
    Vector2 starSpawnPos = PositionOnRandomTile();
    star.setPosition(starSpawnPos);

    Vector2 tradePostSpawnPos = PositionOnRandomTile();
    if (tradePostSpawnPos.x == starSpawnPos.x && tradePostSpawnPos.y == starSpawnPos.y)
    {
        Vector2 tradePostSpawnPos = PositionOnRandomTile();
    }
    tradePost.setPosition(tradePostSpawnPos);


    Vector2 starChaserSpawnPos = PositionOnRandomTile();
    if (starChaserSpawnPos.x == tradePostSpawnPos.x && starChaserSpawnPos.y == tradePostSpawnPos.y)
    {
        Vector2 starChaserSpawnPos = PositionOnRandomTile();
        if (starChaserSpawnPos.x == starSpawnPos.x && starChaserSpawnPos.y == starSpawnPos.y)
        {
            Vector2 starChaserSpawnPos = PositionOnRandomTile();
        }
    }

    starChaser = StarChaser(&starChaser_texture, star.getPosition());
    starChaser.setPosition(starChaserSpawnPos);
}