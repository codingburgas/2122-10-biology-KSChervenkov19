#pragma once

#include "sceneManager.h"
#include "simulation.h"
#include "utils.h"
#include <array>
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
    Texture2D checkmark;
    Texture2D dropDown_Arrow;
    Texture2D dropDown_Arrow_Selected;

    Texture2D entitySearchingFood;
    Texture2D entityGoingHome;
    Texture2D entityGoingTowardsFood;
    Texture2D entityShouldBreed;

    // Camera
    Camera3D camera{0};
    Camera3D entityCamera{ 0 };

    // Font
    Font fontInter;

    // Variables, used for initializing the simulation
    float graphsContainerPos;
    size_t cyclesCount;
    size_t worldSize;
    size_t entities;
    size_t food;
    // size_t currentCycle;

    float offset;
    float entityLookingDirRadian;
    float aminationProgress;
    // float entityRadius;
    // float* radiusArray;
    bool simulating;

    size_t selectedEntityId;
    ss::bll::simulation::Entity *selectedEntity;
    bool followingEntity;

    bool additionalMenuTriggered;
    float timeScale;
    bool shouldShowProgressBar;
    bool shouldShowTraits;

    enum class SLECTED_TRAITS_MONITOR
    {
        SPEED,
        ENERGY,
    } selectedTraitsMonitor;

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
    bool catchSummaryInfo;
    bool savedSimulationInfo;

    const std::array<std::string, 4> entityThoughts = {"SearchingFood", "GoingFood", "GoingHome", "Idle"};

    // Methods, used to draw scenes
    void drawSetup();
    void drawSimulation();
    void drawSummary();
    void drawAdditionalMenu();
    void drawTraitsSwitch();
    void drawProgressBar();

    // void drawEntity(const auto &entity, float radius);
    void drawEntity(const ss::bll::simulation::Entity &entity);
    void drawFood(const auto &food);
    void drawEntityThoughts(const ss::bll::simulation::Entity *entity);

    float animateProgress();
    // float animateDying(float currentRadius);

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