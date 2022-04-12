#include "simulator.h"

Simulation::Simulation(std::string sceneName, SceneManager &sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void Simulation::Start() // called once, at the start of the scene
{
}

void Simulation::Update() // called every frame
{
    BeginDrawing();

    ClearBackground({4, 12, 32, 255});

    DrawRectangle(500, 390, 250, 100, RED);

    DrawText("SIMULATION", 1500 / 2, 980 / 3, 15, BLACK);

    EndDrawing();

    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, {500, 390, 250, 100}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_sceneManager.setCurrentScene("MainMenu");
    }
}

void Simulation::onExit() // called once, at the end of the scene
{
}