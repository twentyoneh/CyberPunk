
#include "raylib.h"
#include "./character/RoleStats.h"
#include <iostream>

int main(void)
{
    srand(time(nullptr));

    // создание персожажа и вывод его статов
    AbilityScores scores = RoleStats::roll(RoleStats::Role::Rockerboy, rand() % 10 + 1);
    std::cout << RoleStats::roleName(RoleStats::Role::Rockerboy) << std::endl;
    scores.print();
    //--------------------------------------------------------------------------------------


    std::cin >> scores.INT_;
    // // Initialization
    // //--------------------------------------------------------------------------------------
    // const int screenWidth = 800;
    // const int screenHeight = 450;

    // SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    // InitWindow(screenWidth, screenHeight, "CR");

    // Vector2 mousePos = GetMousePosition();

    // SetTargetFPS(60);
    // //--------------------------------------------------------------------------------------

    // // Main game loop
    // while (!WindowShouldClose())    // Detect window close button or ESC key
    // {
    //     // Update
    //     //----------------------------------------------------------------------------------
    //     mousePos = GetMousePosition();

    //     //----------------------------------------------------------------------------------

    //     // Draw
    //     //----------------------------------------------------------------------------------
    //     BeginDrawing();

    //         ClearBackground(RAYWHITE);

            
    //         DrawText(TextFormat("mousePos: %.2fx%.2f", mousePos.x, mousePos.y), 50, 210, 20, GRAY);


    //     EndDrawing();
    //     //----------------------------------------------------------------------------------
    // }

    // // De-Initialization
    // //--------------------------------------------------------------------------------------

    // // TODO: Unload all loaded resources at this point

    // CloseWindow();        // Close window and OpenGL context
    // //--------------------------------------------------------------------------------------

    return 0;
}