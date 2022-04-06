#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <vector>

const int screenWidth = 1500;
const int screenHeight = 980;

std::vector<Vector2> initNewEntities()
{
    std::vector<Vector2> entities;

    for (size_t i = 0; i < GetRandomValue(5, 20); i++)
    {
        Vector2 entity = {GetRandomValue(10.0f, screenWidth - 10.0f), GetRandomValue(10.0f, screenHeight - 10.0f)};

        entities.push_back(entity);
    }

    return entities;
}

int main()
{
    const float entityRadius = 20.0f, speed = 5000.0f;
    Vector2 entityPos = {entityRadius, entityRadius};

    InitWindow(screenWidth, screenHeight, "Simulation");

    SetTargetFPS(60);

    std::vector<Vector2> entities = initNewEntities();

    const float cubeSize = 100.0f, cubeSpeed = 100.0f;
    Vector2 cubePos = {cubeSize, 0};

    float timeScale = 1.0f;

    float fElapsedTime;

    while (!WindowShouldClose())
    {

        fElapsedTime = timeScale * GetFrameTime();

        /*if (IsKeyPressed(KEY_SPACE))
        {
            entities = initNewEntities();
        }


        BeginDrawing();

        ClearBackground(WHITE);

        for (auto entity : entities)
        {
            DrawCircle(entity.x, entity.y, entityRadius, RED);
        }

        EndDrawing();*/

        cubePos.x += cubeSpeed * fElapsedTime;

        if (cubePos.x >= screenWidth)
            cubePos = {cubeSize, cubePos.y + cubeSize};

        if (cubePos.y >= screenHeight)
            cubePos = {cubeSize, 0};

        BeginDrawing();

        timeScale = GuiSlider(Rectangle{screenWidth / 2 - 165.0f / 2.0f, screenHeight - 25.0f, 165, 20}, "TIME SCALE",
                              TextFormat("%0.1f", timeScale), timeScale, 0.5f, 100.0f);

        ClearBackground(WHITE);

        DrawRectangle(cubePos.x, cubePos.y, cubeSize, cubeSize, RED);

        EndDrawing();
    }

    CloseWindow();
}