#pragma once

#include "sceneManager.h"
#include "simulation.h"
#include "utils.h"
#include <raygui.h>
#include <raylib.h>

namespace ss::pl::simulator
{
class Simulator : public Scene
{
  private:
    // Instance of scene manager class, used to change scenes
    SceneManager &m_sceneManager;
    ss::bll::simulation::Simulation *simulation;

    // 2D variables
    Vector2 mousePos;
    Vector2 planePos;

    // Textures
    Texture2D setupContainer_Texture;
    Texture2D backArrow_Texture;
    Texture2D simulateButton_Texture;
    Texture2D summary_Container;
    Texture2D exit_Button;
    Texture2D save_Data_Button;

    // Camera
    Camera3D camera{0};

    // Font
    Font fontInter;

    // Variables, used for initializing the simulation
    float graphsContainerPos;
    int cyclesCount;
    int worldSize;
    int entities;
    int food;

    float timeScale;
    float offset;
    float entityLookingDirRadian;
    bool simulating;

    size_t selectedEntityId;

    // Variable, stores the current scene state
    enum class SimulatorState
    {
        Setup,
        Simulation
    } currentState = SimulatorState::Setup;

    // Getter variables
    struct SummaryInfo
    {
        size_t activeEntites;
        size_t diedEntities;
        float averageSpeed;
        float averageSense;
    } summaryInfo;

    // Methods, used to draw scenes
    void drawSetup();
    void drawSimulation();
    void drawSummary();

    void drawEntity(const auto &entity);
    void drawFood(const auto &food);

    // Getter methods
    SummaryInfo getSummaryData();

    // Method which checks the mouse/keyboard input
    void checkInput();
    void handleEntityClick();

    // Method that resets the camera position, rotation, look
    void resetCamera();

  public:
    Simulator(std::string sceneName, SceneManager &sceneManager);

    virtual void Start();

    virtual void Update();

    virtual void onExit();

    virtual void loadAssets();

    virtual void deleteAssets();
};
} // namespace ss::pl::simulator