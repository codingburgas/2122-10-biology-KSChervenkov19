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
    cyclesCount = 5;
    worldSize = 10;
    entities = 10;
    food = 20;

    timeScale = 1.0f;

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

    if (CheckCollisionPointRec(mousePos, {522, 25, 455, 48}))
    {
        camera.canRotate = false;
    }
    else
    {
        camera.canRotate = true;
    }

    if (CheckCollisionPointRec(mousePos, {1064, 820, static_cast<float>(simulateButton_Texture.width),
                                          static_cast<float>(simulateButton_Texture.height)}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Simulator::currentState = SimulatorState::Simulation;
        const types::SimulationInfo simInfo = {static_cast<size_t>(worldSize),
                                               static_cast<size_t>(cyclesCount),
                                               static_cast<size_t>(food),
                                               static_cast<size_t>(entities),
                                               {2.0f, 1.0f}};
        simulation = new ss::bll::simulation::Simulation(simInfo);
        offset = static_cast<float>(worldSize) / 2.0f;
        camera.canRotate = true;
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

    worldSize = GuiSliderBar({1000, 132, 455, 53}, nullptr, nullptr, worldSize, 20, 200);
    entities = GuiSliderBar({1000, 282, 455, 53}, nullptr, nullptr, entities, 10, 100);
    food = GuiSliderBar({1000, 432, 455, 53}, nullptr, nullptr, food, 0, 200);
    cyclesCount = GuiSliderBar({1180, 730, 242, 51}, nullptr, TextFormat("%i", cyclesCount), cyclesCount, 1, 200);

    if (!CheckCollisionPointRec(mousePos, {950, 0, (float)graphsContainerPos, (float)graphsContainerPos}))
    {
        UpdateCamera(&camera);
    }
}

/// This method draws the actual simulation.
void ss::pl::simulator::Simulator::drawSimulation()
{

    if (!simulation->isSimulationDone)
    {
        UpdateCamera(&camera);

        BeginMode3D(camera);
            DrawPlane({0.0f, 0.0f, 0.0f}, {(float)worldSize, (float)worldSize}, WHITE);
            DrawGrid(worldSize, 1.0f);

            for (const auto &entity : simulation->getActiveEntities(simulation->m_entities, simulation->m_entitiesEndIt))
            {
                drawEntity(entity);
            }

            for (const auto &food : simulation->getFoods())
            {
                drawFood(food);
            }

            timeScale = GuiSliderBar({522, 25, 455, 48}, "Timescale:", TextFormat("%.2f", timeScale), timeScale, 0.1f, 10.0f);

            simulation->update(GetFrameTime() * timeScale);
        EndMode3D();
    }
    else
    {
        DrawText("SIMULATION DONE", 500, 500, 100, BLACK);
        //change scene
    }





    // The funny. Do not touch
    // bll::simulation::Cycle::distributeEntities(simulation->m_entities, simulation->m_simInfo.worldSize);
    // bll::simulation::Cycle::randomizeFoodPositions(simulation->m_foods, simulation->m_simInfo.worldSize);
}

void ss::pl::simulator::Simulator::drawEntity(const auto& entity)
{
    float entityLookingDirRadian = ss::bll::utils::toRadian(entity.getFacingAngle() + 180);
    ss::types::fVec2 currentPos = entity.getPos();

    //draw entity body
    DrawSphere({ currentPos.x - offset, .5f, currentPos.y - offset }, .5f, entity.m_foodStage == ss::bll::simulation::EntityFoodStage::ZERO_FOOD ? RED : entity.m_foodStage == bll::simulation::EntityFoodStage::ONE_FOOD ? GREEN : DARKGREEN);

    //draw entity fov
    DrawLine3D({ currentPos.x - offset, .5f, currentPos.y - offset },
        { 1.0f * cos(entityLookingDirRadian) + currentPos.x - offset, .5f,
         1.0f * sin(entityLookingDirRadian) + currentPos.y - offset },
        RED);

    //draw entity aura(only if user has demanded)
    DrawCircle3D({ currentPos.x - offset, .1f, currentPos.y - offset }, entity.m_traits.sense, { 1.0f, 0.0f, 0.0f },
        90.0f, { 255, 0, 0, 100 });

}

void ss::pl::simulator::Simulator::drawFood(const auto& food)
{
    if (!food.isEaten)
    {
        DrawSphere({ food.pos.x - offset, 0.1f, food.pos.y - offset }, 0.3f, GREEN);
    }
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