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
    std::tie(maxSense, maxSpeed) = getHighestSenseAndSpeed(cycleInfo);
    totalDied = cycleInfo[0].lastedEntities -
                cycleInfo.back().lastedEntities; // This is not how you do it but im tired and can't think rn
    totalLasted = cycleInfo.back().lastedEntities;
}

void ss::pl::graph::Graph::Update() // called every frame
{
    BeginDrawing();

    ClearBackground(backgroundColors.at(static_cast<int>(currentTheme)));

    drawGraph();

    EndDrawing();

    checkCollision();
}

void ss::pl::graph::Graph::onExit() // called once, at the end of the scene
{
    deleteAssets();
}

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

std::pair<float, float> ss::pl::graph::Graph::getHighestSenseAndSpeed(const std::vector<ss::types::Cycle> &cycle)
{
    return {getHighestSense(cycle), getHighestSpeed(cycle)};
}

/*
std::pair<int, int> getLastedAndDied(const ss::types::Cycle& cycle)
{
    //Use memory for deads
}
*/

void ss::pl::graph::Graph::drawGraph()
{
    leftLineY = 159;
    leftTextY = 155;
    bottomLineX = 219;
    bottomTextX = 834;
    currentSense = maxSense - maxSense / 9;
    currentSpeed = maxSpeed - maxSpeed / 9;

    DrawTexture(backArrow_Texture, 50, 90, WHITE);
    DrawTexture(graph_Container, 1020, 0, WHITE);
    DrawRectangle(198, 118, 9, 691, {196, 196, 196, 255});
    DrawRectangle(198, 809, 727, 9, {196, 196, 196, 255});
    DrawTextEx(font, "0", {165, 826}, 39, 0, {108, 108, 108, 255});
    DrawTextEx(font, TextFormat("%.1f", maxSense), {124, 90}, 39, 0, {108, 108, 108, 255});
    DrawTextEx(font, TextFormat("%.1f", maxSpeed), {900, 826}, 39, 0, {108, 108, 108, 255});
    DrawTextEx(font, "speed", {478, 868}, 39, 0, BLACK);
    DrawTextEx(font, std::format("Lasted: {}", totalLasted).c_str(), {1055, 80}, 70, 0, BLACK);
    DrawTextEx(font, std::format("Died: {}", totalDied).c_str(), {1055, 217}, 70, 0, BLACK);
    DrawTextPro(font, "sense", {72, 537}, {0, 0}, 270, 39, 0, BLACK);
    for (int i = 0; i < 8; i++)
    {
        DrawRectangle(190, leftLineY, 26, 9, {196, 196, 196, 255});
        DrawTextEx(font, TextFormat("%.2f", (currentSense)), {140, static_cast<float>(leftTextY)}, 19, 0,
                   {108, 108, 108, 255});
        DrawTextEx(font, TextFormat("%.2f", (currentSpeed)), {static_cast<float>(bottomTextX), 844}, 19, 0,
                   {108, 108, 108, 255});
        DrawRectangle(bottomLineX, 800, 9, 26, {196, 196, 196, 255});
        leftLineY += 90;
        leftTextY += 90;
        bottomLineX += 90;
        bottomTextX -= 90;

        currentSense = currentSense - maxSense / 8;
        currentSpeed = currentSpeed - maxSpeed / 8;
    }
}

void ss::pl::graph::Graph::loadAssets()
{
    font = LoadFontEx("../../assets/fonts/Inter.ttf", 96, 0, 0);
    backArrow_Texture = LoadTexture(
        std::format("../../assets/{}/graph/Back_Arrow.png", themePaths.at(static_cast<int>(Graph::currentTheme)))
            .c_str());
    graph_Container = LoadTexture(
        std::format("../../assets/{}/graph/Graph_Container.png", themePaths.at(static_cast<int>(Graph::currentTheme)))
            .c_str());
}

void ss::pl::graph::Graph::deleteAssets()
{
    UnloadTexture(backArrow_Texture);
    UnloadTexture(graph_Container);
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
}