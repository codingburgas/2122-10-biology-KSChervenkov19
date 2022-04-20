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
  private:
    // Textures
    Texture2D backArrow_Texture;
    Texture2D graph_Container;
    Texture2D cycle_Next;
    Texture2D cycle_Prev;
    Texture2D slider_Box;
    Texture2D auto_Button;
    Texture2D data_Container;

    // Vector2 variables
    Vector2 mousePos;
    Vector2 graphLine;
    Vector2 graphText;

    // Custom font
    Font fontInter;

    // Variables, regarding each cycle and it's data
    size_t currentCycle;
    unsigned int totalAlive;
    float currentSense;
    float currentSpeed;
    bool autoCycle;
    int cycleSpeed;
    float fElapsedTime;

    std::string populationChange;

    std::vector<ss::types::Cycle> cycleInfo;

    // Pairs, used to pair cycle data
    std::pair<float, float> getHighestSenseAndSpeed(const std::vector<ss::types::Cycle> &cycle);

    std::pair<float, float> maxSenseAndSpeed;

    // Methods for drawing textures
    void drawGraph();
    void drawMenu();

    // Methods, checking mouse collisions
    void checkCollision();

    // Mathods, automating cycle
    void automateCycle();

    enum class AutoState
    {
        AutoOn,
        AutoOff
    } currentAutoState = AutoState::AutoOff;

    const std::array<std::string, 2> autoPaths = {"Auto_Button_Checked", "Auto_Button_Unchecked"};

  public:
    Graph(std::string sceneName, SceneManager &sceneManager);

    // Inline static variable, used to save opened graph file
    inline static std::string fileName;

    // Derived methods from class Scene
    virtual void Start();

    virtual void Update();

    virtual void onExit();

    virtual void loadAssets();

    virtual void deleteAssets();

    SceneManager &m_sceneManager;

    // Methods, used to calculate cycle data
    static float getHighestSense(const std::vector<ss::types::Cycle> &cycle);

    static float getHighestSpeed(const std::vector<ss::types::Cycle> &cycle);

    static inline float getGrowthPercentage(int lastedBef, int lastedCur);

    static inline float getDecreasedPercentage(int lastedBef, int lastedCur);
};
} // namespace ss::pl::graph