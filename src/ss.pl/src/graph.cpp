#include "graph.h"
#include "statistics_manager.h"
#include <algorithm>

ss::pl::graph::Graph::Graph(std::string sceneName, SceneManager& sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager), maxSpeed(10)
{

}

void ss::pl::graph::Graph::Start() // called once, at the start of the scene
{
    cycleInfo = ss::bll::statistics::StatisticsManager::getStatisticFrom(fileName);
}

void ss::pl::graph::Graph::Update() // called every frame
{
    BeginDrawing();

    ClearBackground(backgroundColors.at(static_cast<int>(currentTheme)));

    EndDrawing();
}

void ss::pl::graph::Graph::onExit() // called once, at the end of the scene
{

}

float ss::pl::graph::Graph::getHighestSense(const std::vector<ss::types::Cycle>& cycles)
{
    auto maxSenseFromCycle = [](const std::vector<ss::types::Trait>& traits)
    {
        return std::ranges::max_element(traits, [](const ss::types::Trait& lhs, const ss::types::Trait& rhs)
            {
                return lhs.sense < rhs.sense;
            })->sense;
    };

    std::vector<float> maxSenses;
    maxSenses.reserve(cycles.size());

    std::ranges::for_each(cycles, [&maxSenseFromCycle, &maxSenses](const ss::types::Cycle& cycle)
        {
            maxSenses.push_back(maxSenseFromCycle(cycle.traitsInfo));
        });

    return *std::ranges::max_element(maxSenses);
}

float ss::pl::graph::Graph::getHighestSpeed(const std::vector<ss::types::Cycle>& cycles)
{
    auto maxSpeedFromCycle = [](const std::vector<ss::types::Trait>& traits)
    {
        return std::ranges::max_element(traits, [](const ss::types::Trait& lhs, const ss::types::Trait& rhs)
            {
                return lhs.speed < rhs.speed;
            })->speed;
    };

    std::vector<float> maxSpeeds;
    maxSpeeds.reserve(cycles.size());

    std::ranges::for_each(cycles, [&maxSpeedFromCycle, &maxSpeeds](const ss::types::Cycle& cycle)
        {
            maxSpeeds.push_back(maxSpeedFromCycle(cycle.traitsInfo));
        });

    return *std::ranges::max_element(maxSpeeds);
}

std::pair<float, float> ss::pl::graph::Graph::getHighestSenseAndSpeed(const std::vector<ss::types::Cycle>& cycle)
{
    return { getHighestSense(cycle), getHighestSpeed(cycle) };
}

std::pair<int, int> getLastedAndDied(const ss::types::Cycle& cycle)
{
    //Use memory for deads
}