#include "level.h"

#include <iostream>
#include <string>
#include <cassert>

bool operator == (Vector2 a, Vector2 b) 
{
    return a.x == b.x && a.y == b.y;
}

bool operator != (Vector2 a, Vector2 b)
{
    if ((a.x != b.x && a.y != b.y) || (a.x == b.x && a.y != b.y) || (a.x != b.x && a.y == b.y))
    {
        return true;
    }
    else return false;
}

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
   /* std::vector<Vector2> path = pathfind(starChaser.getPosition(), star.getPosition());*/

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

    //pathfind(starChaser.getPosition(), star.getPosition());

    if (IsKeyDown(KEY_B))
    {
        if (open_tiles.size() > 0)
        {
            for (int i = 0; i < open_tiles.size(); i++)
            { 
                Color transparentBlue = { 0, 121, 241, 100 };
                Vector2 pos = open_tiles.at(i).position;
                DrawRectangle((int)pos.x, (int)pos.y, TileData::size, TileData::size, transparentBlue);
            }    
        }

        if (closed_tiles.size() > 0)
        {
            for (int i = 0; i < closed_tiles.size(); i++)
            {
                Color transparentRed = { 230, 41, 55, 100 };

                Vector2 pos = closed_tiles.at(i).position;
                DrawRectangle((int)pos.x, (int)pos.y, TileData::size, TileData::size, transparentRed);
            }
        }
    }



    for (int i = 0; i < closed_tiles.size(); i++)
    {
        DrawText(TextFormat("F: %i", closed_tiles.at(i).costF()), closed_tiles.at(i).position.x, closed_tiles.at(i).position.y, 15, BLACK);
        //DrawText(TextFormat("G: %i", closed_tiles.at(i).costG), closed_tiles.at(i).position.x, closed_tiles.at(i).position.y + 15, 15, RED);
        DrawText(TextFormat("H: %i", closed_tiles.at(i).costH), closed_tiles.at(i).position.x, closed_tiles.at(i).position.y + 30, 15, DARKBLUE);

        DrawLine(closed_tiles.at(i).position.x + TileData::size / 2, closed_tiles.at(i).position.y + TileData::size / 2, closed_tiles.at(i).parentPosition.x + TileData::size / 2, closed_tiles.at(i).parentPosition.y + TileData::size / 2, YELLOW);
    }

    for (int i = 0; i < open_tiles.size(); i++)
    {
        DrawText(TextFormat("F: %i", open_tiles.at(i).costF()), open_tiles.at(i).position.x, open_tiles.at(i).position.y, 15, BLACK);
        //DrawText(TextFormat("G: %i", open_tiles.at(i).costG),   open_tiles.at(i).position.x, open_tiles.at(i).position.y + 15, 15, RED);
        DrawText(TextFormat("H: %i", open_tiles.at(i).costH),   open_tiles.at(i).position.x, open_tiles.at(i).position.y + 30, 15, DARKBLUE);

        DrawLine(open_tiles.at(i).position.x + TileData::size / 2, open_tiles.at(i).position.y + TileData::size / 2, open_tiles.at(i).parentPosition.x + TileData::size / 2, open_tiles.at(i).parentPosition.y + TileData::size / 2, YELLOW);
    }

    std::vector<Vector2> path = pathfind(starChaser.getPosition(), star.getPosition());

    for (int i = 0; i < path.size() - 1; i++)
    {
        DrawLine((int)path.at(i).x + TileData::size / 2, (int)path.at(i).y + TileData::size / 2, (int)path.at(i + 1).x + TileData::size / 2, (int)path.at(i + 1).y + TileData::size / 2, RED);
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
    open_tiles.clear();
    closed_tiles.clear();

    std::vector<Vector2> result;

    int offset = 5;
    Vector2 startTilePos = { start.x - offset, start.y - offset };
    Vector2 endTilePos = { end.x - offset, end.y - offset };

    int tileSize = TileData::size;
    int tileHalfSize = TileData::size / 2;

    Vector2 startTileOriginPos = { startTilePos.x + tileHalfSize, startTilePos.y + tileHalfSize };
    Vector2 startTileIndex = { (start.x - offset) / tileSize, (start.y - offset) / tileSize };

    Vector2 endTileOriginPos = { endTilePos.x + tileHalfSize, endTilePos.y + tileHalfSize };

    result.push_back(startTilePos);
    result.push_back(endTilePos);

    if (open_tiles.size() == 0)
    {
        pathfindingNode startNode = {};
        startNode.position = startTilePos;
        open_tiles.push_back(startNode);
    }

    int index = -1;
    
    while (open_tiles.size() > 0)
    {
        bool found = false;
        index = -1;
        for (int i = 0; i < open_tiles.size(); i++)
        {
            if (index == -1 || open_tiles.at(i).costF() <  open_tiles.at(index).costF() 
                 || (open_tiles.at(i).costF() == open_tiles.at(index).costF()
                && open_tiles.at(i).costH   <  open_tiles.at(index).costH))
            {
                
                index = i;
                found = true;
            }
        }

        //for (auto& open : open_tiles)
        //{
        //    for (auto& n : gridManager.GetNeighbours(open.position.x, open.position.y))
        //    {

        //        if (index == -1 || 
        //            open.costF() < open_tiles.at(index).costF() || 
        //            (open.costF() == open_tiles.at(index).costF() && open.costH < open_tiles.at(index).costH))
        //        {
        //            index += 1;
        //            found = true;
        //        }

        //    }
        //}

        if (!found)
        {
            break;
        }

        if (open_tiles.at(index).position == endTilePos)
        {
            closed_tiles.push_back(open_tiles.at(index));
            open_tiles.erase(open_tiles.begin() + index);
            break;
        }

        for (auto n : gridManager.GetNeighbours((int)open_tiles.at(index).position.x / tileSize, (int)open_tiles.at(index).position.y / tileSize))
        {
            Vector2 parentTilePos = {};

            if (gridManager.GetTile((int)n.x / tileSize, (int)n.y / tileSize) == Rock)
            {
                continue;
            }

            if (n == open_tiles.at(index).position)
            {
                continue;
            }

            if (std::find_if(closed_tiles.begin(), closed_tiles.end(), [n](auto& node) {return node.position == n; }) != closed_tiles.end()
                || std::find_if(open_tiles.begin(), open_tiles.end(), [n](auto& node) {return node.position == n; }) != open_tiles.end())
            {
                continue;
            }

            int newMoveCostToNeighbour = open_tiles.at(index).costG + gridManager.GetDistance(open_tiles.at(index).position, Vector2( n.x + tileHalfSize, n.y + tileHalfSize));
       
            pathfindingNode neighbour = {};
            neighbour.position = n;
            neighbour.costG = newMoveCostToNeighbour;
            neighbour.costH = gridManager.GetDistance(Vector2(n.x + tileHalfSize, n.y + tileHalfSize), endTileOriginPos);
            neighbour.parentPosition = open_tiles.at(index).position;
            open_tiles.push_back(neighbour);

        }

        if (index >= 0)
        {
            closed_tiles.push_back(open_tiles.at(index));
            open_tiles.erase(open_tiles.begin() + index);
        }

    }
    
    result = retracePath(startTilePos, endTilePos, index);

    return result;
}

std::vector<Vector2> Level::retracePath(Vector2 startTile, Vector2 endTile, int index)
{
    std::vector<Vector2> path;

    pathfindingNode* currentNode = &closed_tiles.back();

    

    while (true)
    {
        path.push_back(currentNode->position);

        if (currentNode->position == startTile)
        {
            break;
        }

        pathfindingNode* parentNode = nullptr;

        for (auto& closed : closed_tiles)
        {
            if (currentNode->parentPosition == closed.position)
            {
                parentNode = &closed;
                break;
            }
        }

        if (parentNode == nullptr)
        {
            assert(false);
        }
        
        currentNode = parentNode;
        
    }

    std::reverse(path.begin(), path.end());

    return path;

    //pathfindingNode currentNode = {};
    //if (open_tiles.size() > 0)
    //{
    //    currentNode.position = open_tiles.at(open_tiles.size()-1).position;
    //    //while (currentNode.position != startNodePos)
    //    //{
    //    //    path.push_back(currentNode.position);
    //    //    currentNode.position = currentNode.parentPosition;
    //    //}
    //    for (int i = open_tiles.size() - 1; i > 0 ; i--)
    //    {
    //        path.push_back(currentNode.position);
    //        currentNode.position = open_tiles.at(i).parentPosition;
    //    }
    //}
    ////path.reverse();
    ////std::list<Vector2>::iterator it;
    ////for (it == path.begin(); it != path.end(); it++)
    ////{
    ////    DrawLine(it->x + TileData::size / 2, it->y + TileData::size / 2,  );
    ////}
    //
    //std::vector<Vector2> reversedPath;
    //for (int i = (int)path.size() - 1; i > 0; i--)
    //{
    //    reversedPath.push_back(path.at(i));
    //}
    //for (int i = 0; i < reversedPath.size(); i++)
    //{
    //    if (i + 1 != reversedPath.size())
    //    {
    //        DrawLine((int)reversedPath.at(i).x, (int)reversedPath.at(i).y, (int)reversedPath.at(i + 1).x, (int)reversedPath.at(i + 1).y, RED);
    //    }
    //}

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

int Level::pathfindingNode::costF()
{
    return costG + costH;
}