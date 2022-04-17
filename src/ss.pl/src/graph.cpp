#include "graph.h"
#include "statistics_manager.h"
#include <algorithm>

ss::pl::graph::Graph::Graph(std::string sceneName, SceneManager &sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void ss::pl::graph::Graph::Start() // called once, at the start of the scene
{
    loadAssets();
    cycleInfo = ss::bll::statistics::StatisticsManager::getStatisticFrom(fileName);
    maxSenseAndSpeed = getHighestSenseAndSpeed(cycleInfo);
    currentCycle = 1;
    totalAlive = cycleInfo[0].lastedEntities;
}
void ss::pl::graph::Graph::Update() // called every frame
{
    BeginDrawing();

    ClearBackground(backgroundColors.at(static_cast<int>(currentTheme)));

    drawGraph();
    drawMenu();

    EndDrawing();

    checkCollision();
}

void ss::pl::graph::Graph::onExit() // called once, at the end of the scene
{
    deleteAssets();
}

/// Getter highest sense function.
/// 
/// Function which gets the highest sense of a generation holding in a .json file.
/// @param cycles Generation data.
/// @return Highest sense as a float.
float ss::pl::graph::Graph::getHighestSense(const std::vector<ss::types::Cycle> &cycles)
{
    auto maxSenseFromCycle = [](const std::vector<ss::types::Trait> &traits) {
        return std::ranges::max_element(traits, [](const ss::types::Trait &lhs,
                                                   const ss::types::Trait &rhs) { return lhs.sense < rhs.sense; })
            ->sense;
    };

    std::vector<float> maxSenses;
    maxSenses.reserve(cycles.size());

    std::ranges::for_each(cycles, [&maxSenseFromCycle, &maxSenses](const ss::types::Cycle &cycle) {
        maxSenses.push_back(maxSenseFromCycle(cycle.traitsInfo));
    });

    return *std::ranges::max_element(maxSenses);
}

/// Getter highest speed function.
/// 
/// Function which gets the highest speed of a generation holding in a .json file.
/// @param cycles Generation data.
/// @return Highest speed as a float.
float ss::pl::graph::Graph::getHighestSpeed(const std::vector<ss::types::Cycle> &cycles)
{
    auto maxSpeedFromCycle = [](const std::vector<ss::types::Trait> &traits) {
        return std::ranges::max_element(traits, [](const ss::types::Trait &lhs,
                                                   const ss::types::Trait &rhs) { return lhs.speed < rhs.speed; })
            ->speed;
    };

    std::vector<float> maxSpeeds;
    maxSpeeds.reserve(cycles.size());

    std::ranges::for_each(cycles, [&maxSpeedFromCycle, &maxSpeeds](const ss::types::Cycle &cycle) {
        maxSpeeds.push_back(maxSpeedFromCycle(cycle.traitsInfo));
    });

    return *std::ranges::max_element(maxSpeeds);
}


/// Function returning the highest sense and speed.
/// 
/// Function returning the highest sense and speed as a pair of floats.
/// @param cycles Generation data.
/// @return A pair of floats holding sense and speed.
std::pair<float, float> ss::pl::graph::Graph::getHighestSenseAndSpeed(const std::vector<ss::types::Cycle> &cycle)
{
    return {getHighestSense(cycle), getHighestSpeed(cycle)};
}

/// Function returning lasted and died entities.
/// 
/// Function returning lasted and died entities of each cycle. The died entities are calculated using the previous cycle.
/// @param cycle A cycle of a generation.
/// @return A pair of ints holding lasted and died.
std::pair<int, int> ss::pl::graph::Graph::getLastedAndDied(const ss::types::Cycle& cycle) // !WARNING! THE FUNCTION IS UNSAFE!
{
    return { cycle.lastedEntities, (&cycle - sizeof(ss::types::Cycle))->lastedEntities - cycle.lastedEntities };
}

void ss::pl::graph::Graph::drawGraph()
{
    leftLineY = 159;
    leftTextY = 155;
    bottomLineX = 219;
    bottomTextX = 840;
    currentSense = maxSenseAndSpeed.first - maxSenseAndSpeed.second / 9;
    currentSpeed = maxSenseAndSpeed.second - maxSenseAndSpeed.second / 9;

    DrawTexture(backArrow_Texture, 50, 90, WHITE);
    DrawTexture(graph_Container, 1020, 0, WHITE);
    DrawRectangle(198, 118, 9, 691, {196, 196, 196, 255});
    DrawRectangle(198, 809, 727, 9, {196, 196, 196, 255});
    DrawTextEx(font, "0", {165, 826}, 39, 0, {108, 108, 108, 255});
    DrawTextEx(font, TextFormat("%.1f", maxSenseAndSpeed.first), {124, 90}, 39, 0, {108, 108, 108, 255});
    DrawTextEx(font, TextFormat("%.1f", maxSenseAndSpeed.second), {900, 826}, 39, 0, {108, 108, 108, 255});
    DrawTextEx(font, "speed", {478, 868}, 39, 0, BLACK);
    DrawTextEx(font, std::format("Alive: {}", totalAlive).c_str(), {1055, 80}, 70, 0, BLACK);
    // if it ain't broke, don't touch it
        DrawTextEx(font, populationChange.c_str(), { 1055, 217 }, 50, 0, BLACK);
    DrawTextPro(font, "sense", {72, 537}, {0, 0}, 270, 39, 0, BLACK);
    for (int i = 0; i < 8; i++)
    {
        DrawRectangle(190, leftLineY, 26, 9, {196, 196, 196, 255});
        DrawTextEx(font, TextFormat("%.1f", (currentSense)), {140, static_cast<float>(leftTextY)}, 19, 0,
                   {108, 108, 108, 255});
        DrawTextEx(font, TextFormat("%.1f", (currentSpeed)), {static_cast<float>(bottomTextX), 844}, 19, 0,
                   {108, 108, 108, 255});
        DrawRectangle(bottomLineX, 800, 9, 26, {196, 196, 196, 255});
        leftLineY += 90;
        leftTextY += 90;
        bottomLineX += 90;
        bottomTextX -= 90;

        currentSense = currentSense - maxSenseAndSpeed.first / 9;
        currentSpeed = currentSpeed - maxSenseAndSpeed.second / 9;
    }
}

void ss::pl::graph::Graph::drawMenu()
{
    DrawTexture(cycle_Next, 1350, 774, WHITE);
    DrawTexture(cycle_Prev, 1087, 774, WHITE);
    DrawTextEx(font, TextFormat("%i", (currentCycle)), { 1238, 788 }, 49, 0, BLACK);
}

void ss::pl::graph::Graph::loadAssets()
{
    font = LoadFontEx("../../assets/fonts/Inter.ttf", 95, 0, 0);
    backArrow_Texture = LoadTexture(std::format("../../assets/{}/graph/Back_Arrow.png", themePaths.at(static_cast<int>(Graph::currentTheme))).c_str());
    graph_Container = LoadTexture(std::format("../../assets/{}/graph/Graph_Container.png", themePaths.at(static_cast<int>(Graph::currentTheme))).c_str());
    cycle_Next = LoadTexture(std::format("../../assets/{}/graph/Cycle_Next.png", themePaths.at(static_cast<int>(Graph::currentTheme))).c_str());
    cycle_Prev = LoadTexture(std::format("../../assets/{}/graph/Cycle_Prev.png", themePaths.at(static_cast<int>(Graph::currentTheme))).c_str());
}

void ss::pl::graph::Graph::deleteAssets()
{
    UnloadTexture(backArrow_Texture);
    UnloadTexture(graph_Container);
    UnloadTexture(cycle_Next);
    UnloadTexture(cycle_Prev);
    UnloadFont(font);
}

void ss::pl::graph::Graph::checkCollision()
{
    mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, {50, 90, static_cast<float>(backArrow_Texture.width),
                                          static_cast<float>(backArrow_Texture.height)}))
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

    if (CheckCollisionPointRec(mousePos, { 1350, 774, static_cast<float>(cycle_Next.width),
                                          static_cast<float>(cycle_Next.height) }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if(currentCycle != cycleInfo.size()) currentCycle++;

        totalAlive = cycleInfo[currentCycle - 1].lastedEntities;
        if (currentCycle != 1)
        {
            float testing = std::abs(static_cast<float> ((cycleInfo[currentCycle - 1].lastedEntities - cycleInfo[currentCycle - 2].lastedEntities)) / cycleInfo[currentCycle - 2].lastedEntities) * 100;
            float testing2 = std::abs(static_cast<float> ((cycleInfo[currentCycle - 2].lastedEntities - cycleInfo[currentCycle - 1].lastedEntities)) / cycleInfo[currentCycle - 2].lastedEntities) * 100;

            populationChange = (cycleInfo[currentCycle - 1].lastedEntities > cycleInfo[currentCycle - 2].lastedEntities) ?
                std::format("Growth with: {} %", std::round(testing)):
                std::format("Decrease with: {} %", std::round(testing2));
        }
        else
        {
            populationChange = "";
        }
    }

    if (CheckCollisionPointRec(mousePos, { 1087, 774, static_cast<float>(cycle_Prev.width),
                                          static_cast<float>(cycle_Prev.height) }) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (currentCycle - 1 != 0) currentCycle--;

        totalAlive = cycleInfo[currentCycle - 1].lastedEntities;
        if (currentCycle != 1)
        {
            float testing = std::abs(static_cast<float> ((cycleInfo[currentCycle - 1].lastedEntities - cycleInfo[currentCycle - 2].lastedEntities)) / cycleInfo[currentCycle - 2].lastedEntities) * 100;
            float testing2 = std::abs(static_cast<float> ((cycleInfo[currentCycle - 2].lastedEntities - cycleInfo[currentCycle - 1].lastedEntities)) / cycleInfo[currentCycle - 2].lastedEntities) * 100;

            populationChange = (cycleInfo[currentCycle - 1].lastedEntities > cycleInfo[currentCycle - 2].lastedEntities) ?
                std::format("Growth with: {} %", std::round(testing)):
                std::format("Decrease with: {} %", std::round(testing2));
        }
        else
        {
            populationChange = "";
        }
    }
}