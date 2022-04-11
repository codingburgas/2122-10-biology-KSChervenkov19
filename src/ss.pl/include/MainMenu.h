#pragma once

#include "SceneManager.h"
#include "elements.h"
#include <raylib.h>

class MainMenu : public Scene
{
  private:
    Vector2 mousePos;
    Texture2D simulatorButton_Texture;
    Texture2D logo_Texture;
    // Texture2D graphsContainer_Texture;
    Container *graphsContainer;
    Texture2D graphsMenu_Texture;
    bool graphsIsAnimatingIn = false;
    bool graphsIsAnimatingOut = false;
    bool graphsIsOut = false;
    float graphsContainerPos = -887;
    float drag = 3000;

    float animateGraphsContainer();

    void checkCollision();

  public:
    MainMenu(std::string sceneName, SceneManager &sceneManager);

    void Start();

    void Update();

    void onExit();

    SceneManager &m_sceneManager;

    Font font;
};