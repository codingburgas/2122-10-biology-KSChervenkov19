#include "simulator.h"

Simulation::Simulation(std::string sceneName, SceneManager &sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void Simulation::Start() // called once, at the start of the scene
{
    loadAssets();
    graphsContainerPos = 950;
    cyclesCount = 1;
    worldSize = 100;
    entities = 10;
    food = 20;
    cycles = 5;

    camera = Camera3D{{10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, CAMERA_PERSPECTIVE};

    SetCameraMode(camera, CAMERA_FREE);
}

void Simulation::Update() // called every frame
{
    mousePos = GetMousePosition();

    checkInput();

    UpdateCamera(&camera);

    BeginDrawing();

    ClearBackground(backgroundColors.at(static_cast<int>(currentTheme)));

    (currentState == SimulatorState::Setup) ? drawSetup() : drawSimulation();

    EndDrawing();
}

void Simulation::onExit() // called once, at the end of the scene
{
    deleteTextures();
}

void Simulation::checkInput()
{
    if (CheckCollisionPointRec(mousePos, { 50, 90, static_cast<float>(backArrow_Texture.width),
                                          static_cast<float>(backArrow_Texture.height) }) ||
        CheckCollisionPointRec(mousePos, { 1064, 820, static_cast<float>(simulateButton_Texture.width),
                                          static_cast<float>(simulateButton_Texture.height) }))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (CheckCollisionPointRec(mousePos, { 50, 90, static_cast<float>(backArrow_Texture.width),
                                          static_cast<float>(backArrow_Texture.height) }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_sceneManager.setCurrentScene("MainMenu");
    }

    if (CheckCollisionPointRec(mousePos, { 1064, 820, static_cast<float>(simulateButton_Texture.width),
                                          static_cast<float>(simulateButton_Texture.height) }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Simulation::currentState = SimulatorState::Simulation;
    }

    if (IsKeyPressed(KEY_R))
    {
        camera.position = { 10.0f, 10.0f, 10.0f };
        camera.target = { 0.0f, 0.0f, 0.0f };
        camera.up = { 0.0f, 1.0f, 0.0f };
    }
}

void Simulation::drawSetup()
{
    DrawTexture(setupContainer_Texture, graphsContainerPos, 0, WHITE);
    DrawTexture(simulateButton_Texture, 1064, 820, WHITE);
    DrawTexture(backArrow_Texture, 57, 90, WHITE);

    DrawTextEx(fontInter, "World Size:", {1001, 86}, 32.6F, 0, backgroundColors.at(!(static_cast<int>(currentTheme))));
    DrawTextEx(fontInter, "Entities:", {1001, 236}, 32.6F, 0, backgroundColors.at(!(static_cast<int>(currentTheme))));
    DrawTextEx(fontInter, "Food:", {1001, 386}, 32.6F, 0, backgroundColors.at(!(static_cast<int>(currentTheme))));
    DrawTextEx(fontInter, "Cycles:", {1028, 730}, 39.1F, 0, backgroundColors.at(!(static_cast<int>(currentTheme))));
    DrawTextEx(fontInter, TextFormat("%i", worldSize), {1412, 186}, 25.5F, 0,
               backgroundColors.at(!(static_cast<int>(currentTheme))));
    DrawTextEx(fontInter, TextFormat("%i", entities), {1412, 336}, 25.5F, 0,
               backgroundColors.at(!(static_cast<int>(currentTheme))));
    DrawTextEx(fontInter, TextFormat("%i", food), {1412, 490}, 25.5F, 0,
               backgroundColors.at(!(static_cast<int>(currentTheme))));

    worldSize = GuiSliderBar({1000, 132, 455, 53}, nullptr, nullptr, worldSize, 50, 500);
    entities = GuiSliderBar({1000, 282, 455, 53}, nullptr, nullptr, entities, 1, 200);
    food = GuiSliderBar({1000, 432, 455, 53}, nullptr, nullptr, food, 1, 500);
    GuiValueBox({1180, 730, 242, 51}, nullptr, &cycles, 1, 200, true);
}

void Simulation::drawSimulation()
{
    BeginMode3D(camera);
    DrawGrid(10, 1.0f); /*({ 0.0f, 0.0f, 0.0f }, { 15.0f, 15.0f }, RED);*/
    EndMode3D();
}

// clang-format off
void Simulation::loadAssets()
{
    fontInter = LoadFontEx("../../assets/fonts/Inter.ttf", 96, 0, 0);
    setupContainer_Texture = LoadTexture(std::format("../../assets/{}/simulator/Setup_Container.png", themePaths.at(static_cast<int>(Simulation::currentTheme))).c_str());
    backArrow_Texture = LoadTexture(std::format("../../assets/{}/simulator/Back_Arrow.png", themePaths.at(static_cast<int>(Simulation::currentTheme))).c_str());
    simulateButton_Texture = LoadTexture(std::format("../../assets/{}/simulator/Simulate_Button.png", themePaths.at(static_cast<int>(Simulation::currentTheme))).c_str());
    GuiLoadStyle((currentTheme == ThemeTypes::LightTheme) ? "../../assets/bluish.txt.rgs" : "../../assets/lavanda.txt.rgs");
}
void Simulation::deleteTextures()
{
    UnloadTexture(setupContainer_Texture);
    UnloadTexture(simulateButton_Texture);
    UnloadTexture(backArrow_Texture);
}
// clang-format on