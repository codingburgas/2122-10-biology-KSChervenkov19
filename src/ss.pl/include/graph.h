#pragma once
#include "sceneManager.h"
#include "simulation_data.h"
#include <raygui.h>
#include <raylib.h>
#include <string>
#include <vector>

namespace ss::pl::graph
{
class Graph : public Scene
{
  public:
    Graph(std::string sceneName, SceneManager &sceneManager);

    float maxSpeed;
    float maxSense;

    inline static std::string fileName;

    virtual void Start();

    virtual void Update();

    virtual void onExit();

    SceneManager &m_sceneManager;

  private:
    Texture2D backArrow_Texture;
    Texture2D graph_Container;
    Vector2 mousePos;
    Font font;

    int leftLineY;
    int leftTextY;
    int bottomLineX;
    int bottomTextX;
    int totalLasted;
    int totalDied;
    float currentSense;
    float currentSpeed;

    std::vector<ss::types::Cycle> cycleInfo;

    float getHighestSense(const std::vector<ss::types::Cycle> &cycle);

    float getHighestSpeed(const std::vector<ss::types::Cycle> &cycle);

    std::pair<float, float> getHighestSenseAndSpeed(const std::vector<ss::types::Cycle> &cycle);

    std::pair<int, int> getLastedAndDied(const ss::types::Cycle &cycle);

    void loadAssets();
    void deleteAssets();
    void drawGraph();
    void checkCollision();
};
} // namespace ss::pl::graph