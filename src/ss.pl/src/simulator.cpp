#include "simulator.h"

/// Constructor for the Simulation class.
///
/// @param sceneName The name of the current scene. Which here is "Simulation".
/// @param sceneManager A reference to already existing SceneManager object to control the program flow.
ss::pl::simulator::Simulator::Simulator(std::string sceneName, SceneManager &sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager)
{
}

/// Method which is called in the start of the Simulation page.
void ss::pl::simulator::Simulator::Start() // called once, at the start of the scene
{
    loadAssets();

    graphsContainerPos = 950;
    cyclesCount = 1;
    worldSize = 10;
    entities = 10;
    food = 20;
    cycles = 5;

    camera = Camera3D{{10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, CAMERA_PERSPECTIVE};
    camera.canRotate = false;
    SetCameraMode(camera, CAMERA_FREE);

    planePos = {0.0f, 0.0f};
}

/// Method which is called every frame.
///
/// It updates the current scene every frame.
void ss::pl::simulator::Simulator::Update() // called every frame
{
    mousePos = GetMousePosition();

    checkInput();

    BeginDrawing();

    ClearBackground(backgroundColors.at(static_cast<int>(currentTheme)));

    (currentState == SimulatorState::Setup) ? drawSetup() : drawSimulation();

    EndDrawing();
}

/// Method which is called when we exit the program or the Simulation page.
///
/// It deallocates every dynamically created object in the class' instance.
void ss::pl::simulator::Simulator::onExit() // called once, at the end of the scene
{
    deleteAssets();
}

/// This method creates checks for the mouse input during the actual simulation.
void ss::pl::simulator::Simulator::checkInput()
{
    if (CheckCollisionPointRec(mousePos, {50, 90, static_cast<float>(backArrow_Texture.width),
                                          static_cast<float>(backArrow_Texture.height)}) ||
        CheckCollisionPointRec(mousePos, {1064, 820, static_cast<float>(simulateButton_Texture.width),
                                          static_cast<float>(simulateButton_Texture.height)}))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (CheckCollisionPointRec(mousePos, {50, 90, static_cast<float>(backArrow_Texture.width),
                                          static_cast<float>(backArrow_Texture.height)}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_sceneManager.setCurrentScene("MainMenu");
    }

    if (CheckCollisionPointRec(mousePos, {1064, 820, static_cast<float>(simulateButton_Texture.width),
                                          static_cast<float>(simulateButton_Texture.height)}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Simulator::currentState = SimulatorState::Simulation;
        ss::types::SimulationInfo simInfo = { worldSize, cyclesCount, food, entities, 1.0f, 1.0f };
        simulation = new ss::bll::simulation::Simulation(simInfo);
;       camera.canRotate = true;
        resetCamera();
    }

    if (IsKeyPressed(KEY_R))
    {
        resetCamera();
    }
}

/// This method resets the camera to its initial position.
void ss::pl::simulator::Simulator::resetCamera()
{
    camera.position = {10.0f, 10.0f, 10.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
}

/// This method draws the setUp page before the actual simulation.
void ss::pl::simulator::Simulator::drawSetup()
{
    BeginMode3D(camera);
    DrawPlane({planePos.x - 8.0f, planePos.y - 5.0f, 0.0f}, {(float)worldSize, (float)worldSize}, WHITE);
    EndMode3D();

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

    worldSize = GuiSliderBar({1000, 132, 455, 53}, nullptr, nullptr, worldSize, 1, 50);
    entities = GuiSliderBar({1000, 282, 455, 53}, nullptr, nullptr, entities, 1, 200);
    food = GuiSliderBar({1000, 432, 455, 53}, nullptr, nullptr, food, 1, 500);
    GuiValueBox({1180, 730, 242, 51}, nullptr, &cycles, 1, 200, true);

    if (!CheckCollisionPointRec(mousePos, {950, 0, (float)graphsContainerPos, (float)graphsContainerPos}))
    {
        UpdateCamera(&camera);
    }
}

/// This method draws the actual simulation.
void ss::pl::simulator::Simulator::drawSimulation()
{
    UpdateCamera(&camera);

    BeginMode3D(camera);
    DrawGrid(worldSize, 1.0f);
    DrawPlane({0.0f, 0.0f, 0.0f}, {(float)worldSize, (float)worldSize}, WHITE);

    const float offset  = (float)worldSize / 2.0f;

    for (const auto& entity : simulation->getActiveEntities(simulation->m_entities, simulation->m_entitiesEndIt))
    {
        float entityLookingDirRadian = ss::bll::utils::toRadian(entity.getFacingAngle() + 180);
        ss::types::fVec2 currentPos = entity.getPos();

        DrawSphere({ currentPos.x - offset, .5f, currentPos.y - offset}, .5f, RED);
        DrawLine3D({ currentPos.x - offset, .5f, currentPos.y - offset}, {1.0f * cos(entityLookingDirRadian) + currentPos.x - offset, .5f, 1.0f * sin(entityLookingDirRadian) + currentPos.y - offset}, RED);
        std::cout << entity.getPos().x << ' ' << entity.getPos().y  << ' ' << entity.getFacingAngle() << "   ";
    }

    std::cout << '\n';


    EndMode3D();
}

// clang-format off

/// Method for loading all the needed assets in the Simulation page.
void ss::pl::simulator::Simulator::loadAssets()
{
    fontInter = LoadFontEx("../../assets/fonts/Inter.ttf", 96, 0, 0);

    setupContainer_Texture = LoadTexture(std::format("../../assets/{}/simulator/Setup_Container.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    backArrow_Texture = LoadTexture(std::format("../../assets/{}/simulator/Back_Arrow.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    simulateButton_Texture = LoadTexture(std::format("../../assets/{}/simulator/Simulate_Button.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());

    GuiLoadStyle((currentTheme == ThemeTypes::LightTheme) ? "../../assets/bluish.txt.rgs" : "../../assets/lavanda.txt.rgs");
}

/// Method for deallocating the dynamically created assets.
void ss::pl::simulator::Simulator::deleteAssets()
{
    UnloadTexture(setupContainer_Texture);
    UnloadTexture(simulateButton_Texture);
    UnloadTexture(backArrow_Texture);

    UnloadFont(fontInter);
}
// clang-format on