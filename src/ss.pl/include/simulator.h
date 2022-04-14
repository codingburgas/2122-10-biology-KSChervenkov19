#pragma once

#include "sceneManager.h"
#include <raylib.h>
#include <raygui.h>

class Simulation : public Scene
{
  public:
    Simulation(std::string sceneName, SceneManager &sceneManager);

    Texture2D setupContainer_Texture;
    Texture2D backArrow_Texture;
    Texture2D simulateButton_Texture;
    Font fontInter;

    float graphsContainerPos;
    int cyclesCount;
    int worldSize;
    int entities;
    int food;
    int cycles;

    virtual void Start();

    virtual void Update();

    void onExit();

    void drawSetup();
    void drawSimulation();

    void loadAssets();
    void deleteTextures();

    enum class SimulatorState
    {
        Setup,
        Simulation
    } currentState = SimulatorState::Setup;

    SceneManager &m_sceneManager;
};