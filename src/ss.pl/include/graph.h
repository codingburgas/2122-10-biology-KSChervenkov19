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

    inline static std::string fileName;

    virtual void Start();

    virtual void Update();

    virtual void onExit();

    virtual void loadAssets();

    virtual void deleteAssets();

    SceneManager &m_sceneManager;

  private:
    Texture2D backArrow_Texture;
    Texture2D graph_Container;
    Texture2D cycle_Next;
    Texture2D cycle_Prev;
    Vector2 mousePos;
    Vector2 graphLine;
    Vector2 graphText;
    Font fontInter;

    size_t currentCycle;
    unsigned int totalAlive;
    float currentSense;
    float currentSpeed;

    std::string populationChange;

    std::vector<ss::types::Cycle> cycleInfo;

    std::pair<float, float> getHighestSenseAndSpeed(const std::vector<ss::types::Cycle> &cycle);
   
    std::pair<float, float> maxSenseAndSpeed;

    float getHighestSense(const std::vector<ss::types::Cycle> &cycle);

    float getHighestSpeed(const std::vector<ss::types::Cycle> &cycle);

    inline float getGrowthPercentage(int lastedBef, int lastedCur);

    inline float getDecreasedPercentage(int lastedBef, int lastedCur);

    void drawGraph();
    void drawMenu();
    void checkCollision();
};
} // namespace ss::pl::graph