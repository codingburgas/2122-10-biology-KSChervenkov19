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
    shouldShowTraits = false;
    selectedTraitsMonitor = SLECTED_TRAITS_MONITOR::SPEED;

    savedSimulationInfo = false;
    catchSummaryInfo = true;
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
        catchSummaryInfo = true;
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

    if (CheckCollisionPointRec(mousePos, {1298, 241, 45, 43}) && currentState == SimulatorState::Simulation &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && additionalMenuTriggered)
    {
        shouldShowProgressBar = !shouldShowProgressBar;
    }

    camera.canRotate = !CheckCollisionPointRec(mousePos, { 1073, 165, 355, 48 }) && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    if (CheckCollisionPointRec(mousePos, {1298, 321, 45, 43}) && currentState == SimulatorState::Simulation &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && additionalMenuTriggered)
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
    entities = GuiSliderBar({1000, 282, 455, 53}, nullptr, nullptr, entities, 3, 200);
    food = GuiSliderBar({1000, 432, 455, 53}, nullptr, nullptr, food, 3, 400);
    cyclesCount = GuiSliderBar({1180, 730, 242, 51}, nullptr, TextFormat("%i", cyclesCount), cyclesCount, 3, 1000);

    if (!CheckCollisionPointRec(mousePos, {950, 0, (float)graphsContainerPos, (float)graphsContainerPos}))
    {
        UpdateCamera(&camera);
    }
}

/// Method for drawing the actual simulation.
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

        handleEntityClick();
        if (const ss::bll::simulation::Entity *selectedEntity = simulation->getEntityById(selectedEntityId);
            selectedEntity)
        {
            camera.target = {selectedEntity->m_pos.x - offset, .5f, selectedEntity->m_pos.y - offset};
            drawEntityThoughts(selectedEntity);
        }

        EndMode3D();

        drawProgressBar();

        drawAdditionalMenu();

        if (simulating)
        {
            simulation->update(GetFrameTime() * timeScale);
            animateProgress();
        }
    }
    else
    {
        if (catchSummaryInfo)
        {
            summaryInfo = getSummaryData();
            aminationProgress = 0;
            catchSummaryInfo = false;
        }

        drawSummary();

        if (CheckCollisionPointRec(mousePos, {624, 659, 267, 79}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            simulation->saveSimulationInfo({});
            currentState = SimulatorState::Setup;
        }
    }
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
    if (CheckCollisionPointRec(mousePos, {1044.0f, 37.0f, 419.0f, 65.0f}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        additionalMenuTriggered = !additionalMenuTriggered;
    }

    if (additionalMenuTriggered)
    {
        DrawTextEx(fontInter, "Timescale:", {1073, 124}, 36, 0, backgroundColors.at(!(static_cast<int>(currentTheme))));
        DrawRectangleRounded({1044.0f, 37.0f, 419.0f, 474.0f}, .2f, 1, {193, 187, 245, 53});
        timeScale = GuiSliderBar({1073, 165, 355, 48}, nullptr, nullptr, timeScale, 0.1f, 10.0f);

        DrawTextEx(fontInter, "Progressbar:", {1073, 241}, 36, 0,
                   backgroundColors.at(!(static_cast<int>(currentTheme))));
        DrawRectangleRounded({1298, 241, 45, 43}, .2f, 1, {255, 255, 255, 255});
        if (shouldShowProgressBar)
            DrawTexture(checkmark, 1300, 246, WHITE); // draw checkbox

        DrawTextEx(fontInter, "Monitor traits:", {1073, 321}, 36, 0,
                   backgroundColors.at(!(static_cast<int>(currentTheme))));
        DrawRectangleRounded({1298, 321, 45, 43}, .2f, 1, {255, 255, 255, 255});
        if (shouldShowTraits)
            DrawTexture(checkmark, 1300, 326, WHITE); // draw checkbox

        // draw switch
        if (shouldShowTraits)
            drawTraitsSwitch();
    }

    (currentTheme == ThemeTypes::LightTheme)
        ? DrawRectangleRounded({1044.0f, 37.0f, 419.0f, 65.0f}, 10.0f, 1, {101, 158, 244, 255})
        : DrawRectangleRounded({1044.0f, 37.0f, 419.0f, 65.0f}, 10.0f, 1, {158, 149, 245, 255});

    additionalMenuTriggered ? DrawTexture(dropDown_Arrow_Selected, 1390, 57, WHITE)
                            : DrawTexture(dropDown_Arrow, 1400, 48, WHITE);
}

/// Method for drawing the progress bar at the bottom of simuation
void ss::pl::simulator::Simulator::drawProgressBar()
{
    if (!shouldShowProgressBar)
        return;

    // Draw current cycle number
    DrawTextEx(fontInter, TextFormat("%i", simulation->m_currentCycle_n - 1), {aminationProgress - 11, 915}, 36, 1,
               {132, 132, 132, 255});

    // Draw background of progressbar
    DrawRectangle(0, 960, 1500, 20, WHITE);

    // Draw progressbar fill
    DrawRectangleRounded({-10, 960, aminationProgress + 10, 20}, 1, 10, Color{101, 158, 244, 255});
}

void ss::pl::simulator::Simulator::drawTraitsSwitch()
{
    if (CheckCollisionPointRec(mousePos, {1151, 403, 185, 57}))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            selectedTraitsMonitor = selectedTraitsMonitor == SLECTED_TRAITS_MONITOR::SPEED
                                        ? SLECTED_TRAITS_MONITOR::ENERGY
                                        : SLECTED_TRAITS_MONITOR::SPEED;
        }
    }

    // draw base
    DrawRectangleRounded({1152, 402, 184, 58}, 1, 10, Color{205, 208, 238, 255});

    // drawCircle
    DrawRectangleRounded(
        {static_cast<float>((selectedTraitsMonitor == SLECTED_TRAITS_MONITOR::SPEED ? 1278 : 1152)), 402, 58, 58}, 1,
        10, WHITE);

    // draw monitored trait
    DrawTextEx(fontInter, (selectedTraitsMonitor == SLECTED_TRAITS_MONITOR::SPEED ? "Speed" : "Energy"),
               {(selectedTraitsMonitor == SLECTED_TRAITS_MONITOR::SPEED ? 1167.0f : 1216.0f), 412.0f}, 36, 1,
               {132, 132, 132, 255});
}

/// Method for animating the progress bar at the bottom of simuation
/// @return current progress bar's X position
float ss::pl::simulator::Simulator::animateProgress()
{
    if (aminationProgress <=
        (static_cast<float>(simulation->m_currentCycle_n - 1) / static_cast<float>(cyclesCount)) * 1500.0f)
    {
        aminationProgress += timeScale / 2;
    }

    return aminationProgress + 10;
}

/// Method for calculating the dying animation
/// @param current float radius
/// @return new float radius
// float ss::pl::simulator::Simulator::animateDying(float currentRadius)
//{
//    if(currentRadius > 0) currentRadius -= .02f;
//    return currentRadius;
//}

/// Method for drawing every entity at the simulation field
void ss::pl::simulator::Simulator::drawEntity(const ss::bll::simulation::Entity &entity)
{
    float entityLookingDirRadian = ss::bll::utils::toRadian(entity.getFacingAngle() + 180);
    ss::types::fVec2 currentPos = entity.getPos();

    // draw entity body
    if (shouldShowTraits)
    {
        if (selectedTraitsMonitor == SLECTED_TRAITS_MONITOR::SPEED)
        {
            DrawSphere({currentPos.x - offset, .5f, currentPos.y - offset}, .5f,
                       Color{static_cast<unsigned char>(
                                 90 + entity.m_traits.speed > 1 ? (entity.m_traits.speed * 20) / 100 * 157 : 0),
                             90,
                             static_cast<unsigned char>(
                                 65 + entity.m_traits.speed < 1 ? (entity.m_traits.speed * 20) / 100 * 190 : 0),
                             255});
        }

        if (selectedTraitsMonitor == SLECTED_TRAITS_MONITOR::ENERGY)
        {
            DrawSphere({currentPos.x - offset, .5f, currentPos.y - offset}, .5f,
                       Color{0, static_cast<unsigned char>(((entity.m_currentEnergy / entity.m_energyMax) * 100) / 100 * 255), 0, 255});
        }
    }
    else
    {
        DrawSphere({currentPos.x - offset, .5f, currentPos.y - offset}, .5f,
                   entity.m_foodStage == ss::bll::simulation::EntityFoodStage::ZERO_FOOD
                       ? RED
                       : entity.m_foodStage == bll::simulation::EntityFoodStage::ONE_FOOD ? GREEN : DARKGREEN);
    }

    // draw entity fov
    DrawLine3D({currentPos.x - offset, .5f, currentPos.y - offset},
               {1.0f * cos(entityLookingDirRadian) + currentPos.x - offset, .5f,
                1.0f * sin(entityLookingDirRadian) + currentPos.y - offset},
               RED);

    // draw entity aura(only if user has demanded)
    if(shouldShowTraits)
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
    switch (entity->getBrain())
    {
    case ss::types::EntityTarget::SEARCHINGFOOD:
        DrawBillboard(camera, entitySearchingFood, {entity->getPos().x - offset, 2.0f, entity->getPos().y - offset},
                      2.0f, WHITE);
        break;

    case ss::types::EntityTarget::GOINGFOOD:
        DrawBillboard(camera, entityGoingTowardsFood, {entity->getPos().x - offset, 2.0f, entity->getPos().y - offset},
                      2.0f, WHITE);
        break;

    case ss::types::EntityTarget::GOINGHOME:
        DrawBillboard(camera, entityGoingHome, {entity->getPos().x - offset, 2.0f, entity->getPos().y - offset}, 2.0f,
                      WHITE);
        break;

    case ss::types::EntityTarget::IDLE:
        if (entity->m_foodStage == ss::bll::simulation::EntityFoodStage::TWO_FOOD)
        {
            DrawBillboard(camera, entityShouldBreed, {entity->getPos().x - offset, 2.0f, entity->getPos().y - offset},
                          2.0f, WHITE);
        }
        else
        {
            DrawBillboard(camera, entityGoingHome, {entity->getPos().x - offset, 2.0f, entity->getPos().y - offset},
                          2.0f, WHITE);
        }
        break;
    }
}

/// Method for getting the data for summary screen
/// @return data for summary screen
ss::pl::simulator::Simulator::SummaryInfo ss::pl::simulator::Simulator::getSummaryData()
{
    return SummaryInfo(simulation->getTotalAliveEntities(), simulation->getTotalDiedEntities(),
                       simulation->getAvgTraits().speed, simulation->getAvgTraits().sense);
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
    dropDown_Arrow = LoadTexture("../../assets/lightTheme/simulator/DropDown_Arrow.png");
    dropDown_Arrow_Selected = LoadTexture("../../assets/lightTheme/simulator/DropDown_Arrow_Selected.png");

    entitySearchingFood = LoadTexture(std::format("../../assets/{}/simulator/Entity_Searching_Food.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    entityGoingTowardsFood = LoadTexture(std::format("../../assets/{}/simulator/Entity_Going_Towards_Food.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    entityGoingHome = LoadTexture(std::format("../../assets/{}/simulator/Entity_Going_Home.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());
    entityShouldBreed = LoadTexture(std::format("../../assets/{}/simulator/Entity_Should_Breed.png", themePaths.at(static_cast<int>(Simulator::currentTheme))).c_str());

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
    UnloadTexture(dropDown_Arrow);
    UnloadTexture(dropDown_Arrow_Selected);
    UnloadTexture(entitySearchingFood);
    UnloadTexture(entityGoingHome);
    UnloadTexture(entityGoingTowardsFood);
    UnloadTexture(entityShouldBreed);

    UnloadFont(fontInter);
}
// clang-format on