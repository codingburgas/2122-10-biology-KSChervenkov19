#include "simulator.h"

Simulation::Simulation(std::string sceneName, SceneManager &sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void Simulation::Start() // called once, at the start of the scene
{
    setupContainer_Texture = LoadTexture(std::format("../../assets/{}/simulator/Setup_Container.png", themePaths.at(static_cast<int>(Simulation::currentTheme))).c_str());
    backArrow_Texture = LoadTexture(std::format("../../assets/{}/simulator/Back_Arrow.png", themePaths.at(static_cast<int>(Simulation::currentTheme))).c_str());
}

void Simulation::Update() // called every frame
{
    BeginDrawing();

    ClearBackground(backgroundColors.at(static_cast<int>(currentTheme)));

    DrawTexture(setupContainer_Texture, graphsContainerPos, 0, WHITE);
    DrawTexture(backArrow_Texture, 57, 53, WHITE);

    EndDrawing();

    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, { 57, 53, static_cast<float>(backArrow_Texture.width), static_cast<float>(backArrow_Texture.height) }))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (CheckCollisionPointRec(mousePos, { 57, 53, static_cast<float>(backArrow_Texture.width), static_cast<float>(backArrow_Texture.height) })
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_sceneManager.setCurrentScene("MainMenu");
    }
}

void Simulation::onExit() // called once, at the end of the scene
{
}