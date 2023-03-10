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
    starChaser.initialize();
}

void Level::Update()
{
    remove_dead_and_add_pending_agents();

    frameCounter++;
    timef += GetFrameTime();
    delay -= GetFrameTime();

    if (IsKeyDown(KEY_TAB))
    {
        drawControlScheme = true;
    }
    else drawControlScheme = false;

    // Cycle tile type when tile is clicked
    if (IsMouseButtonPressed(0) && !starChaserHeldByMouse)
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

    if (IsKeyPressed(KEY_ONE))
    {
        closed_tiles.clear();
        open_tiles.clear();
        starChaser.path.clear();

        starChaserHeldByMouse = true;
    }

    if (IsKeyPressed(KEY_TWO))
    {
        closed_tiles.clear();
        open_tiles.clear();
        starChaser.path.clear();

        starHeldByMouse = true;
    }

    if (starChaserHeldByMouse)
    {
        starChaser.setPosition(GetMousePosition());
        if (IsMouseButtonPressed(0))
        {
            int tilePosX = (int)floor(GetMousePosition().x / TileData::size);
            int tilePosY = (int)floor(GetMousePosition().y / TileData::size);
            int releasePosX = tilePosX * TileData::size + 5;
            int releasePosY = tilePosY * TileData::size + 5;
            Vector2 releasePos = { releasePosX, releasePosY };
            starChaser.setPosition(releasePos);
            starChaserHeldByMouse = false;
        }
    }

    if (starHeldByMouse)
    {
        star.setPosition(GetMousePosition());
        if (IsMouseButtonPressed(0))
        {
            int tilePosX = (int)floor(GetMousePosition().x / TileData::size);
            int tilePosY = (int)floor(GetMousePosition().y / TileData::size);
            int releasePosX = tilePosX * TileData::size + 5;
            int releasePosY = tilePosY * TileData::size + 5;
            Vector2 releasePos = { releasePosX, releasePosY };
            star.setPosition(releasePos);
            starHeldByMouse = false;
        }
    }

    // Debug controls
    //if (IsKeyPressed(KEY_C) && numberOfSteps >= 0)
    //{
    //    numberOfSteps -= 1;
    //}
    //if (IsKeyPressed(KEY_V))
    //{
    //    numberOfSteps += 1;
    //}
    if (IsKeyPressed(KEY_B))
    {
        drawingOpenAndClosedTiles = true;
    }
    if (IsKeyReleased(KEY_B))
    {
        drawingOpenAndClosedTiles = false;
    }

    starChaser.sense(this);
    starChaser.decide();
    starChaser.act(this);

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

    tradePost.draw(this);
    star.draw(this);
    starChaser.draw(this);

    //Vector2 starChaserOrigin = { starChaser.getPosition().x - 5 + TileData::size / 2, starChaser.getPosition().y - 5 + TileData::size / 2 };
    //Vector2 starOrigin = { star.getPosition().x - 5 + TileData::size / 2, star.getPosition().y - 5 + TileData::size / 2 };

    //pathfind(starChaser.getPosition(), star.getPosition());

    if (drawingOpenAndClosedTiles)
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


    // Draw F, G and H costs for each closed tile and draw line from current node to it's parent
    for (int i = 0; i < closed_tiles.size(); i++) // Draw the F, G and H costs for each closed tile
    {
        //DrawText(TextFormat("F: %i", closed_tiles.at(i).costF()), closed_tiles.at(i).position.x, closed_tiles.at(i).position.y, 15, BLACK);
        ////DrawText(TextFormat("G: %i", closed_tiles.at(i).costG), closed_tiles.at(i).position.x, closed_tiles.at(i).position.y + 15, 15, RED);
        //DrawText(TextFormat("H: %i", closed_tiles.at(i).costH), closed_tiles.at(i).position.x, closed_tiles.at(i).position.y + 30, 15, DARKBLUE);
        DrawLine(closed_tiles.at(i).position.x + TileData::size / 2, closed_tiles.at(i).position.y + TileData::size / 2, closed_tiles.at(i).parentPosition.x + TileData::size / 2, closed_tiles.at(i).parentPosition.y + TileData::size / 2, YELLOW);
    }

    //Draw F, G and H costs for each open tile and draw line from current node to it's parent
    for (int i = 0; i < open_tiles.size(); i++)
    {
        //DrawText(TextFormat("F: %i", open_tiles.at(i).costF()), open_tiles.at(i).position.x, open_tiles.at(i).position.y, 15, BLACK);
        ////DrawText(TextFormat("G: %i", open_tiles.at(i).costG),   open_tiles.at(i).position.x, open_tiles.at(i).position.y + 15, 15, RED);
        //DrawText(TextFormat("H: %i", open_tiles.at(i).costH),   open_tiles.at(i).position.x, open_tiles.at(i).position.y + 30, 15, DARKBLUE);
        DrawLine(open_tiles.at(i).position.x + TileData::size / 2, open_tiles.at(i).position.y + TileData::size / 2, open_tiles.at(i).parentPosition.x + TileData::size / 2, open_tiles.at(i).parentPosition.y + TileData::size / 2, YELLOW);
    }

    if (starChaser.path.size() > 0)
    {
        for (int i = 0; i < starChaser.path.size() - 1; i++) // Draw the resulting path selected by A*
        {
            DrawLine((int)starChaser.path.at(i).x + TileData::size / 2,
                     (int)starChaser.path.at(i).y + TileData::size / 2,
                     (int)starChaser.path.at(i + 1).x + TileData::size / 2,
                     (int)starChaser.path.at(i + 1).y + TileData::size / 2, RED);
        }
    }

    // testing
    DrawText(TextFormat("Open tiles:   %i", (int)open_tiles.size()), 10, GetScreenHeight() - 50, 30, BLACK); // Draw number of open tiles on screen
    DrawText(TextFormat("Closed tiles:   %i", (int)closed_tiles.size()), 10, GetScreenHeight() - 80, 30, BLACK); // Draw number of closed tiles on screen


    DrawText(TextFormat("Time: %f", timef), 10, 10, 30, BLACK);

    if (drawControlScheme)
    {
        int centerScreenX = GetScreenWidth() / 2;
        int centerScreenY = GetScreenHeight() / 2;

        int fontSize = 30;

        DrawRectangle(0, centerScreenY - 150, 850, 350, LIGHTGRAY);
        DrawText(TextFormat("Pick up Star Chaser: 1"), 50, centerScreenY -100, fontSize, BLACK);
        DrawText(TextFormat("Pick up Star: 2"), 50, centerScreenY - 50, fontSize, BLACK );
        DrawText(TextFormat("Pick up Trading Post : 3"), 50, centerScreenY, fontSize, BLACK );
        DrawText(TextFormat("Pick up Spaceship : 4"), 50, centerScreenY + 50, fontSize, BLACK );
        DrawText(TextFormat("Place held actor / Change tile type : Left Click"), 50, centerScreenY + 100, fontSize, BLACK );
    }

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
    if (agentType == "StarChaser")
    {
        StarChaser agent;
        Vector2 randPos = { (float)(rand() & GetScreenWidth()), (float)(rand() & GetScreenHeight()) };
        TileType spawnTileType = gridManager.GetTile((int)(randPos.x / TileData::size), (int)(randPos.y / TileData::size));
        Vector2 spawnPos;
        spawnPos.x = randPos.x + TileData::size / 2;
        spawnPos.y = randPos.y + TileData::size / 2;
        agent.setPosition(spawnPos);
        starChaser_agents.push_back(agent);
        result = &starChaser_agents.back();
    }
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

std::vector<Vector2> Level::pathfind(Vector2 start, Vector2 end, int maxSteps)
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
        //// Uncomment code below to add the ability to step through the A* pathfinding for debugging
        //if (maxSteps <= 0)
        //{
        //    break;
        //}
        //maxSteps -= 1;

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

            if (std::find_if(closed_tiles.begin(), closed_tiles.end(), [n](auto& node) {return node.position == n; }) != closed_tiles.end())
            {
                continue;
            }
            else if (std::find_if(open_tiles.begin(), open_tiles.end(), [n](auto& node) {return node.position == n; }) != open_tiles.end())
            {
                pathfindingNode *neighbourNode = nullptr;
                for (auto open : open_tiles)
                {
                    if (open.position == n)
                    {
                        neighbourNode = &open;
                        break;
                    }
                }
                /*int newG = open_tiles.at(index).costG + gridManager.GetDistance(open_tiles.at(index).position, Vector2(n.x, n.y));*/
                int newG = open_tiles.at(index).costG + gridManager.GetDistance(open_tiles.at(index).origin(), Vector2(n.x + tileHalfSize, n.y + tileHalfSize));
                if (neighbourNode != nullptr && newG < neighbourNode->costG)
                {
                    neighbourNode->costG = newG;
                    neighbourNode->costF();
                              
                    /*neighbourNode->parentPosition = open_tiles.at(index).position;*/
                    neighbourNode->parentPosition = open_tiles.at(index).position;
                }
                else
                {
                    continue;
                }
            }

            int newMoveCostToNeighbour = open_tiles.at(index).costG + gridManager.GetDistance(open_tiles.at(index).origin(), Vector2(n.x + tileHalfSize, n.y + tileHalfSize));
            
            if (/*index == -1 &&*/ newMoveCostToNeighbour < open_tiles.at(index).costG)
            {
                continue;
            }
            
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

}

bool Level::isDestinationValid(Vector2 p_destination_position)
{
    std::vector<Vector2> destNeighbours = gridManager.GetNeighbours(p_destination_position.x / TileData::size, p_destination_position.y / TileData::size);

    int amonuntOfRockNeighbours = 0;
    bool destTileValidAsDestination = true;
    for (auto sN : destNeighbours)
    {

        TileType currentlyLookedAtTile = gridManager.GetTile(sN.x / TileData::size, sN.y / TileData::size);
        if (currentlyLookedAtTile == Rock)
        {
            amonuntOfRockNeighbours += 1;
        }
    }
    if (amonuntOfRockNeighbours >= destNeighbours.size())
    {
        destTileValidAsDestination = false;
    }

    return destTileValidAsDestination;
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

    while (true)
    {
        if (gridManager.GetTile(result.x / TileData::size, result.y / TileData::size) == Rock)
        {
            int randTileX = rand() % gridManager.grid.columns;
            int randTileY = rand() % gridManager.grid.rows;
            int offset = 5;

            result = { (float)(randTileX * TileData::size + offset), (float)(randTileY * TileData::size + offset) };
        }
        else
        {
            break;
        }
    }

    return result;
}

void Level::set_spawn_positions()
{
    // Set STAR start position
    bool starStartTtileIsValid = true;
    Vector2 starSpawnPos = PositionOnRandomTile();
    //if (gridManager.GetTile(starSpawnPos.x / TileData::size, starSpawnPos.y / TileData::size) == Rock)
    //{
    //    false;
    //}
    //if (!starStartTtileIsValid)
    //{
    //    starSpawnPos = PositionOnRandomTile();
    //}
    //else
    //{
    //    star.setPosition(starSpawnPos);
    //}
    star.setPosition(starSpawnPos);


    // Set TRADING POST start position
    bool tradePostStartTileIsValid = true;
    Vector2 tradePostSpawnPos = PositionOnRandomTile();
    if (tradePostSpawnPos.x == starSpawnPos.x && tradePostSpawnPos.y == starSpawnPos.y 
        /*|| gridManager.GetTile(tradePostSpawnPos.x / TileData::size, tradePostSpawnPos.y / TileData::size) == Rock*/)
    {
        tradePostStartTileIsValid = false;
    }
    if (!tradePostStartTileIsValid)
    {
        Vector2 tradePostSpawnPos = PositionOnRandomTile();
    }
    else
    {
        tradePost.setPosition(tradePostSpawnPos);
    }
    //tradePost.setPosition(tradePostSpawnPos);


    // Set STARCHASER start position
    bool starChaserStartTileIsValid = true;
    Vector2 starChaserSpawnPos = PositionOnRandomTile();
    if (starChaserSpawnPos.x == tradePostSpawnPos.x && starChaserSpawnPos.y == tradePostSpawnPos.y
        || starChaserSpawnPos.x == starSpawnPos.x && starChaserSpawnPos.y == starSpawnPos.y
        /*|| gridManager.GetTile(starChaserSpawnPos.x / TileData::size, starChaserSpawnPos.y / TileData::size) == Rock*/)
    {
        starChaserStartTileIsValid = false;
    }
    if (!starChaserStartTileIsValid)
    {
        starChaserSpawnPos = PositionOnRandomTile();
    }
    else
    {
        starChaser.setPosition(starChaserSpawnPos);
    }

    //starChaser = StarChaser(&starChaser_texture, star.getPosition());
}

Level::pathfindingNode *Level::pathfindingNode::getNode(Vector2 p_position)
{
    if (position == p_position)
    {
        return this;
    }
    else
        return nullptr;
}

int Level::pathfindingNode::costF()
{
    return costG + costH;
}

Vector2 Level::pathfindingNode::origin()
{
    return Vector2(position.x + TileData::size / 2, position.y + TileData::size / 2);
}

