#include "graph.h"
#include "statistics_manager.h"

ss::pl::graph::Graph::Graph(std::string sceneName, SceneManager& sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void ss::pl::graph::Graph::Start() // called once, at the start of the scene
{
    cycleInfo = ss::bll::statistics::StatisticsManager::getStatisticFrom(fileName);
    std::cout << cycleInfo[0].lastedEntities - cycleInfo[1].lastedEntities;
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