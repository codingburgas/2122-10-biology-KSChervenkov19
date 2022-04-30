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
    simulating = true;

    additionalMenuTriggered = false;
    shouldShowProgressBar = true;
    shouldShowTraits = true;
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
        CheckCollisionPointRec(mousePos, {1020, 820, static_cast<float>(simulateButton_Texture.width),
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
        const types::SimulationInfo simInfo = {worldSize, cyclesCount, food, entities, {2.0f, 1.0f}};
        simulation = new ss::bll::simulation::Simulation(simInfo);
        offset = static_cast<float>(worldSize) / 2.0f;
        camera.canRotate = true;
        resetCamera();
    }

    if (CheckCollisionPointRec(
            mousePos, {992, 243, static_cast<float>(exit_Button.width), static_cast<float>(exit_Button.height)}) &&
        currentState == SimulatorState::Simulation && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (simulation->isSimulationDone)
        {
            resetCamera();
            currentState = SimulatorState::Setup;
        }
    }

    if (CheckCollisionPointRec(mousePos, {606, 673, static_cast<float>(save_Data_Button.width),
                                          static_cast<float>(save_Data_Button.height)}) &&
        currentState == SimulatorState::Simulation && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (simulation->isSimulationDone)
        {
            resetCamera();
            simulation->saveSimulationInfo("test");
            currentState = SimulatorState::Setup;
        }
    }

    if (CheckCollisionPointRec(mousePos, {1298, 241, 45, 43}) &&
        currentState == SimulatorState::Simulation && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && additionalMenuTriggered)
    {
        shouldShowProgressBar = !shouldShowProgressBar;
    }

    if (CheckCollisionPointRec(mousePos, {1298, 321, 45, 43}) &&
        currentState == SimulatorState::Simulation && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && additionalMenuTriggered)
    {
        shouldShowTraits = !shouldShowTraits;
    }

    if (IsKeyPressed(KEY_R))
    {
        resetCamera();
        selectedEntityId = NULL;
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        simulating = !simulating;
    }
}

/// Method for handling the camera and collision for clickig on entity
void ss::pl::simulator::Simulator::handleEntityClick()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Ray ray = GetMouseRay(GetMousePosition(), camera);

        RayCollision rayCollision;

        for (const auto &entity : simulation->getActiveEntities(simulation->m_entities, simulation->m_entitiesEndIt))
        {
            rayCollision =
                GetRayCollisionSphere(ray, {entity.getPos().x - offset, 0.5f, entity.getPos().y - offset}, .5f);
            if (rayCollision.hit)
            {
                selectedEntityId = entity.m_id;
            }
        }
    }
}

/// Method for reseting the camera to its initial position.
void ss::pl::simulator::Simulator::resetCamera()
{
    camera.position = {10.0f, 10.0f, 10.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
}

/// Method for drawing the setUp page before the actual simulation.
void ss::pl::simulator::Simulator::drawSetup()
{
    BeginMode3D(camera);
    DrawPlane({planePos.x - 8.0f, planePos.y - 5.0f, 0.0f}, {(float)worldSize, (float)worldSize}, WHITE);
    EndMode3D();

    DrawTexture(setupContainer_Texture, graphsContainerPos, 0, WHITE);
    DrawTexture(simulateButton_Texture, 1020, 820, WHITE);
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

    worldSize = GuiSliderBar({1000, 132, 455, 53}, nullptr, nullptr, worldSize, 6, 200);
    entities = GuiSliderBar({1000, 282, 455, 53}, nullptr, nullptr, entities, 1, 200);
    food = GuiSliderBar({1000, 432, 455, 53}, nullptr, nullptr, food, 2, 400);
    cyclesCount = GuiSliderBar({1180, 730, 242, 51}, nullptr, TextFormat("%i", cyclesCount), cyclesCount, 1, 200);

    if (!CheckCollisionPointRec(mousePos, {950, 0, (float)graphsContainerPos, (float)graphsContainerPos}))
    {
        UpdateCamera(&camera);
    }
}

/// Method for drawing the actual simulation.
void ss::pl::simulator::Simulator::drawSimulation()
{
    static bool flag = true;
    if (!simulation->isSimulationDone)
    {
        UpdateCamera(&camera);
        BeginMode3D(camera);
        DrawPlane({0.0f, 0.0f, 0.0f}, {(float)worldSize, (float)worldSize}, WHITE);
        DrawGrid(worldSize, 1.0f);
        animateProgress();

        int index = 0;
        if (currentCycle != simulation->m_currentCycle_n)
        {
            delete[]radiusArray;
            radiusArray = new float[simulation->getActiveEntities(simulation->m_entities, simulation->m_entitiesEndIt).size()];
            currentCycle = simulation->m_currentCycle_n;
        }
        for (const auto &entity : simulation->getActiveEntities(simulation->m_entities, simulation->m_entitiesEndIt))
        {
            if (entity.m_isDoneWithCycle && entity.m_foodStage == ss::bll::simulation::EntityFoodStage::ZERO_FOOD)
            {
                radiusArray[index] = animateDying(radiusArray[index]);
            }
            else
            {
                radiusArray[index] = .5f;
            }

            drawEntity(entity, radiusArray[index]);
            index++;
            // Debugging
            // std::cout << entity.m_cycleBornAt << ' ';
        }
        // std::cout << '\n';

        for (const auto &food : simulation->getFoods())
        {
            drawFood(food);
        }

        // PLEASE FIX THIS
        // IT THROWS

        EndMode3D();

        drawProgressBar();

        drawAdditionalMenu();

        handleEntityClick();
        if (const ss::bll::simulation::Entity *selectedEntity = simulation->getEntityById(selectedEntityId);
            selectedEntity)
        {
            camera.target = {selectedEntity->m_pos.x - offset, .5f, selectedEntity->m_pos.y - offset};
            drawEntityThoughts(selectedEntity);
        }

        if (simulating)
            simulation->update(GetFrameTime() * timeScale);
    }
    else
    {
        if (flag)
        {
            summaryInfo = getSummaryData();
            aminationProgress = 0;
            simulation->saveSimulationInfo({});
            flag = false;
        }

        drawSummary();
    }

    // The funny. Do not touch
    // bll::simulation::Cycle::distributeEntities(simulation->m_entities, simulation->m_simInfo.worldSize);
    // bll::simulation::Cycle::randomizeFoodPositions(simulation->m_foods, simulation->m_simInfo.worldSize);
}

/// Method for drawing summary screen at the end of the simulation
void ss::pl::simulator::Simulator::drawSummary()
{
    DrawTexture(summary_Container, 437, 201, WHITE);
    DrawTexture(exit_Button, 992, 243, WHITE);
    DrawTexture(save_Data_Button, 606, 673, WHITE);

    DrawTextEx(fontInter, "SUMMARY", {595, 255}, 70, 0,
               Simulator::backgroundColors.at(!static_cast<int>(Simulator::currentTheme)));
    DrawTextEx(fontInter, TextFormat("Total alive entities: %i", summaryInfo.activeEntites), {566, 369}, 32, 0,
               Simulator::backgroundColors.at(!static_cast<int>(Simulator::currentTheme)));
    DrawTextEx(fontInter, TextFormat("Total died entities: %i", summaryInfo.diedEntities), {566, 439}, 32, 0,
               Simulator::backgroundColors.at(!static_cast<int>(Simulator::currentTheme)));
    DrawTextEx(fontInter, TextFormat("Average entity speed: %.1f", summaryInfo.averageSpeed), {566, 508}, 32, 0,
               Simulator::backgroundColors.at(!static_cast<int>(Simulator::currentTheme)));
    DrawTextEx(fontInter, TextFormat("Average entity sense: %.1f", summaryInfo.averageSense), {566, 578}, 32, 0,
               Simulator::backgroundColors.at(!static_cast<int>(Simulator::currentTheme)));
}

/// Method for drawing controls menu at simulation runtime
void ss::pl::simulator::Simulator::drawAdditionalMenu()
{
    if (CheckCollisionPointRec(mousePos, {1350.0f, 59.0f, 78.0f, 22.0f}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        additionalMenuTriggered = !additionalMenuTriggered;
    }

    if (additionalMenuTriggered)
    {
        DrawTextEx(fontInter, "Timescale:", {1073, 124}, 36, 0,
                   backgroundColors.at(!(static_cast<int>(currentTheme))));
        DrawRectangleRounded({1044.0f, 37.0f, 419.0f, 474.0f}, .2f, 1, {193, 187, 245, 53});
        timeScale = GuiSliderBar({1073, 165, 355, 48}, nullptr, nullptr, timeScale, 0.1f, 10.0f);

        DrawTextEx(fontInter, "Progressbar:", {1073, 241}, 36, 0,
                   backgroundColors.at(!(static_cast<int>(currentTheme))));
        DrawRectangleRounded({1298, 241, 45, 43}, .2f, 1, {255, 255, 255, 255});
        if (shouldShowProgressBar) DrawTexture(checkmark, 1300, 246, WHITE);
        // draw checkbox

        DrawTextEx(fontInter, "Monitor traits:", {1073, 321}, 36, 0,
                   backgroundColors.at(!(static_cast<int>(currentTheme))));
        DrawRectangleRounded({1298, 321, 45, 43}, .2f, 1, {255, 255, 255, 255});
        if (shouldShowTraits) DrawTexture(checkmark, 1300, 326, WHITE);
    }

    (currentTheme == ThemeTypes::LightTheme)? 
    DrawRectangleRounded({1044.0f, 37.0f, 419.0f, 65.0f}, 10.0f, 1, {101, 158, 244, 255}):
    DrawRectangleRounded({1044.0f, 37.0f, 419.0f, 65.0f}, 10.0f, 1, {158, 149, 245, 255});

    DrawRectangleRounded({1350.0f, 59.0f, 78.0f, 22.0f}, 10.0f, 1, WHITE);
}

/// Method for drawing the progress bar at the bottom of simuation
void ss::pl::simulator::Simulator::drawProgressBar()
{
    if (!shouldShowProgressBar)
        return;

    //std::cout << (((simulation->m_currentCycle_n - 1 / cyclesCount) * 10) / 100) << '\n';
    // Draw current cycle number
    DrawTextEx(fontInter, TextFormat("%i", simulation->m_currentCycle_n - 1), { 659, 915 }, 36, 1, { 132, 132, 132, 255 });

    // Draw background of progressbar
    DrawRectangle(0, 960, 1500, 20, WHITE);

    // Draw progressbar fill
    DrawRectangleRounded({-10, 960, aminationProgress + 10, 20}, 1, 10, Color{101, 158, 244, 255});
}

/// Method for animating the progress bar at the bottom of simuation
/// @return current progress bar's X position
float ss::pl::simulator::Simulator::animateProgress()
{
    if (aminationProgress <= (static_cast<float>(simulation->m_currentCycle_n - 1) / static_cast<float>(cyclesCount)) * 1500.0f)
    {
        aminationProgress += timeScale / 2;
    }

    return aminationProgress + 10;
}

/// Method for calculating the dying animation 
/// @param current float radius
/// @return new float radius
float ss::pl::simulator::Simulator::animateDying(float currentRadius)
{
    if(currentRadius > 0) currentRadius -= .02f;
    return currentRadius;
}

/// Method for drawing every entity at the simulation field
void ss::pl::simulator::Simulator::drawEntity(const auto &entity, float radius)
{
    float entityLookingDirRadian = ss::bll::utils::toRadian(entity.getFacingAngle() + 180);
    ss::types::fVec2 currentPos = entity.getPos();

    // draw entity body
    DrawSphere({currentPos.x - offset, .5f, currentPos.y - offset}, radius,
               entity.m_foodStage == ss::bll::simulation::EntityFoodStage::ZERO_FOOD
                   ? RED
                   : entity.m_foodStage == bll::simulation::EntityFoodStage::ONE_FOOD ? GREEN : DARKGREEN);

    // draw entity fov
    DrawLine3D({currentPos.x - offset, .5f, currentPos.y - offset},
               {1.0f * cos(entityLookingDirRadian) + currentPos.x - offset, .5f,
                1.0f * sin(entityLookingDirRadian) + currentPos.y - offset},
               RED);

    // draw entity aura(only if user has demanded)
    DrawCircle3D({currentPos.x - offset, .1f, currentPos.y - offset}, entity.m_traits.sense, {1.0f, 0.0f, 0.0f}, 90.0f,
                 {255, 0, 0, 100});
}

/// Method for drawing every food at the simulation field
void ss::pl::simulator::Simulator::drawFood(const auto &food)
{
    if (!food.isEaten)
    {
        DrawSphere({food.pos.x - offset, 0.1f, food.pos.y - offset}, 0.3f, GREEN);
    }
}

/// Method for drawing entity's movement target when you click on it
void ss::pl::simulator::Simulator::drawEntityThoughts(const ss::bll::simulation::Entity *entity)
{
    //std::cout << entityThoughts.at(static_cast<size_t>(entity->getBrain()));
}

/// Method for getting the data for summary screen
/// @return data for summary screen
ss::pl::simulator::Simulator::SummaryInfo ss::pl::simulator::Simulator::getSummaryData()
{
    return SummaryInfo(simulation->getActiveEntities(simulation->m_entities, simulation->m_entitiesEndIt).size(),
                       simulation->m_entities.size() -
                           simulation->getActiveEntities(simulation->m_entities, simulation->m_entitiesEndIt).size(),
                       69, 69);
}

// clang-format off

/// Method for loading all the needed assets in the Simulation page.
void ss::pl::simulator::Simulator::loadAssets()
{
    fontInter = LoadFontEx("../../assets/fonts/Inter.ttf", 96, 0, 0);

    setupContainer_Texture = LoadTexture(std::format("../../assets/{}/simulator/Setup_Container.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    backArrow_Texture = LoadTexture(std::format("../../assets/{}/simulator/Back_Arrow.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    simulateButton_Texture = LoadTexture(std::format("../../assets/{}/simulator/Simulate_Button.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    summary_Container = LoadTexture(std::format("../../assets/{}/simulator/Summary_Container.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    exit_Button = LoadTexture(std::format("../../assets/{}/simulator/Exit_Button.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    save_Data_Button = LoadTexture(std::format("../../assets/{}/simulator/Save_Data_Button.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    checkmark = LoadTexture("../../assets/lightTheme/simulator/Checkmark.png");

    GuiLoadStyle((currentTheme == ThemeTypes::LightTheme) ? "../../assets/bluish.txt.rgs" : "../../assets/lavanda.txt.rgs");
}

/// Method for deallocating the dynamically created assets.
void ss::pl::simulator::Simulator::deleteAssets()
{
    UnloadTexture(setupContainer_Texture);
    UnloadTexture(simulateButton_Texture);
    UnloadTexture(backArrow_Texture);
    UnloadTexture(summary_Container);
    UnloadTexture(exit_Button);
    UnloadTexture(save_Data_Button);
    UnloadTexture(checkmark);

    UnloadFont(fontInter);
}
// clang-format on