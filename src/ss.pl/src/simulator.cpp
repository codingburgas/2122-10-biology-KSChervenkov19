#include "simulator.h"

Simulation::Simulation(std::string sceneName, SceneManager &sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void Simulation::Start() // called once, at the start of the scene
{
    loadTextures();
    graphsContainerPos = 950;
    cyclesCount = 1;
    worldSize = 100;
    entities = 10;
    food = 20;
}

void Simulation::Update() // called every frame
{
    BeginDrawing();

    ClearBackground(backgroundColors.at(static_cast<int>(currentTheme)));

    (currentState == SimulatorState::Setup) ? drawSetup() : drawSimulation();

    EndDrawing();

    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, {57, 53, static_cast<float>(backArrow_Texture.width),
                                          static_cast<float>(backArrow_Texture.height)}))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (CheckCollisionPointRec(mousePos, {57, 53, static_cast<float>(backArrow_Texture.width),
                                          static_cast<float>(backArrow_Texture.height)}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_sceneManager.setCurrentScene("MainMenu");
    }

    if (CheckCollisionPointRec(mousePos, {1079, 873, static_cast<float>(simulateButton_Texture.width),
                                          static_cast<float>(simulateButton_Texture.height)}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Simulation::currentState = SimulatorState::Simulation;
    }
}

void Simulation::onExit() // called once, at the end of the scene
{
    deleteTextures();
}

void Simulation::drawSetup()
{
    DrawTexture(setupContainer_Texture, graphsContainerPos, 0, WHITE);
    DrawTexture(simulateButton_Texture, 1064, 820, WHITE);
    DrawTexture(backArrow_Texture, 57, 53, WHITE);
    DrawTextEx(fontInter, "World Size:", { 1001 , 86 }, 32.6F, 0, {5, 11, 30, 255});
    DrawTextEx(fontInter, "Entities:", { 1001 , 236 }, 32.6F, 0, { 5, 11, 30, 255 });
    DrawTextEx(fontInter, "Food:", { 1001 , 386 }, 32.6F, 0, { 5, 11, 30, 255 });
    DrawTextEx(fontInter, "Cycles:", { 1028 , 730 }, 39.1F, 0, { 5, 11, 30, 255 });
    worldSize = GuiSliderBar({ 1000, 132, 455, 53 }, nullptr, TextFormat("%i", worldSize), worldSize, 50, 500);
    entities = GuiSliderBar({ 1000, 282, 455, 53 }, nullptr, TextFormat("%i", entities), entities, 1, 200);
    food = GuiSliderBar({ 1000, 432, 455, 53 }, nullptr, TextFormat("%i", food), food, 1, 500);
    GuiSpinner({ 1180, 730, 242, 51 }, nullptr, &cyclesCount, 1, 1000, true);
}

void Simulation::drawSimulation()
{
    std::cout << "in simulating";
}

// clang-format off
void Simulation::loadTextures()
{
    fontInter = LoadFontEx("../../assets/fonts/Inter.ttf", 96, 0, 0);
    setupContainer_Texture = LoadTexture(std::format("../../assets/{}/simulator/Setup_Container.png", themePaths.at(static_cast<int>(Simulation::currentTheme))).c_str());
    backArrow_Texture = LoadTexture(std::format("../../assets/{}/simulator/Back_Arrow.png", themePaths.at(static_cast<int>(Simulation::currentTheme))).c_str());
    simulateButton_Texture = LoadTexture(std::format("../../assets/{}/simulator/Simulate_Button.png", themePaths.at(static_cast<int>(Simulation::currentTheme))).c_str());
}
void Simulation::deleteTextures()
{
    UnloadTexture(setupContainer_Texture);
    UnloadTexture(simulateButton_Texture);
    UnloadTexture(backArrow_Texture);
}
// clang-format on