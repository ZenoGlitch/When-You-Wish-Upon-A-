#include "raylib.h"
#include "level.h"
#include <ctime>

// IMPORTANT:
// Feel free to not use the Level, and redo everything on your own. This is just some boilerplate code to get you started.

// If you want, I suggest manually copying my code from scratch. That should help with understanding how it works a bit better.

int main(void)
{    
    //const int screenWidth = 1920;
    //const int screenHeight = 1080;
    const int screenWidth = 1500;
    const int screenHeight = 1200;

    SetConfigFlags(/*ConfigFlags::FLAG_WINDOW_RESIZABLE |*/ ConfigFlags::FLAG_VSYNC_HINT | ConfigFlags::FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "When you wish upon A*");

    // SetTargetFPS(60); // Uncomment this if you want to use fixed framerate (I suggest using vsync)
     //SetExitKey(0);    // Uncomment this to disable ESC as exit key

    srand((unsigned int)time(0));

    Level level;

    level.Initialize();
    level.Reset();

    while (!WindowShouldClose())
    {
        // Update
        level.Update();

        // Draw
        BeginDrawing();

        ClearBackground(WHITE);
        
        level.Draw();

        EndDrawing();
    }

    level.unload_textures();
    CloseWindow();

    return 0;
}