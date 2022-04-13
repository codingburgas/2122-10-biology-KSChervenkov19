#pragma once

#include "sceneManager.h"
#include <raylib.h>

class Simulation : public Scene
{
  public:
    Simulation(std::string sceneName, SceneManager &sceneManager);

    Texture2D setupContainer_Texture;
    Texture2D backArrow_Texture;
    Texture2D simulateButton_Texture;

    float graphsContainerPos = 950;

    virtual void Start();

    virtual void Update();

    void onExit();

    void drawSetup();
    void drawSimulation();

    enum class SimulatorState { Setup, Simulation } currentState = SimulatorState::Setup;

    //SimulatorState currentState = SimulatorState::Setup;

    SceneManager &m_sceneManager;
};