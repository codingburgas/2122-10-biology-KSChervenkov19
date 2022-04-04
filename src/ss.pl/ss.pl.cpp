#include "raylib.h"
#include <vector>

const int screenWidth = 800;
const int screenHeight = 450;

std::vector<Vector2> initNewEntities()
{
    std::vector<Vector2> entities;

    for (size_t i = 0; i < GetRandomValue(5, 20); i++)
    {
        Vector2 entity = { GetRandomValue(10.0f, screenWidth - 10.0f), GetRandomValue(10.0f, screenHeight - 10.0f) };

        entities.push_back(entity);
    }

    return entities;
}

int main()
{
    const float entityRadius = 20.0f, speed = 5000.0f;
    Vector2 entityPos = { entityRadius, entityRadius };

    InitWindow(screenWidth, screenHeight, "raylib [models] example - heightmap loading and drawing");

    SetTargetFPS(60);

    std::vector<Vector2> entities = initNewEntities();

    while (!WindowShouldClose())
    {

        if (IsKeyPressed(KEY_SPACE))
            entities = initNewEntities();

        BeginDrawing();

        ClearBackground(WHITE);

        for (auto entity : entities)
        {
            DrawCircle(entity.x, entity.y, entityRadius, RED);
        }

        EndDrawing();
    }

    CloseWindow();
}