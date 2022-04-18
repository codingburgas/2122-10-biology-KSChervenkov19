#pragma once

#include "sceneManager.h"
#include <raygui.h>
#include <raylib.h>

class Simulation : public Scene
{
  private:
    // Instance of scene manager class, used to change scenes
    SceneManager &m_sceneManager;

    // 2D variables
    Vector2 mousePos;
    Vector2 planePos;

    // Textures
    Texture2D setupContainer_Texture;
    Texture2D backArrow_Texture;
    Texture2D simulateButton_Texture;

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
    int cycles;

    // Variable, stores the current scene state
    enum class SimulatorState
    {
        Setup,
        Simulation
    } currentState = SimulatorState::Setup;

    // Methods, used to draw scenes
    void drawSetup();
    void drawSimulation();

    // Method which checks the mouse/keyboard input
    void checkInput();

    // Method that resets the camera position, rotation, look
    void resetCamera();

    // Methods, used to load/unload assets or textures
    void loadAssets();
    void deleteTextures();

  public:
    Simulation(std::string sceneName, SceneManager &sceneManager);

    virtual void Start();

    virtual void Update();

    virtual void onExit();
};