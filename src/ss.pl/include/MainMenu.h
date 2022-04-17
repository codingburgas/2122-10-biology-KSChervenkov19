#pragma once

#include "SceneManager.h"
#include "statistics_manager.h"
#include <algorithm>
#include <ranges>
#include <raylib.h>

class MainMenu : public Scene
{
  private:
    Vector2 mousePos;
    Vector2 currentGraphPos;
    Texture2D simulatorButton_Texture;
    Texture2D logo_Texture;
    Texture2D graphsContainer_Texture;
    Texture2D graphsMenu_Texture;
    Texture2D themeButton_Texture;
    Texture2D viewGraph_Texture;

    int offset = 0;

    struct graphsCard
    {
        std::string name;
        Vector2 namePos;
        Vector2 buttonPos; // button position
    };
    Vector2 graphNamePos;
    Vector2 graphButtonPos;

    std::vector<std::string> statisticNames;
    std::vector<graphsCard> graphCards;

    bool graphsIsAnimatingIn = false;
    bool graphsIsAnimatingOut = false;
    bool graphsIsOut = false;

    float graphsContainerPos = -887;
    float drag = 3000;

    float calculateGraphsContainer();

    void checkCollision();

    void animateGraphsContainer();

    void displayGraphCards();

    auto collisionCoordiantes();

    void checkGraphButtonCollisions();

    void loadTextures();

    void deleteTextures();

  public:
    MainMenu(std::string sceneName, SceneManager &sceneManager);

    void Start();

    void Update();

    void onExit();

    SceneManager &m_sceneManager;

    Font font;
};