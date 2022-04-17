#pragma once

#include "SceneManager.h"
#include "statistics_manager.h"
#include <algorithm>
#include <ranges>
#include <raylib.h>

class MainMenu : public Scene
{
  private:
    //instance of the scene manager
    SceneManager& m_sceneManager;

    //Vector2 variables
    Vector2 mousePos;
    Vector2 currentGraphPos;
    int scrollOffset = 0;

    //Textures
    Texture2D simulatorButton_Texture;
    Texture2D logo_Texture;
    Texture2D graphsContainer_Texture;
    Texture2D graphsMenu_Texture;
    Texture2D themeButton_Texture;
    Texture2D viewGraph_Texture;

    //Custom font
    Font font;

    //Variables, regarding the graph containers
    struct graphsCard
    {
        std::string name;
        Vector2 namePos;
        Vector2 buttonPos;
    };

    Vector2 graphNamePos;
    Vector2 graphButtonPos;

    std::vector<std::string> statisticNames;
    std::vector<graphsCard> graphCards;

    //Variables, used to handle the graph container animation
    bool graphsIsAnimatingIn = false;
    bool graphsIsAnimatingOut = false;
    bool graphsIsOut = false;

    float graphsContainerPos = -887;
    float graphContainerAnimationEase = 3000;

    //Methods, handling different things
    void handleScroll();
    bool handleMouseCursor();

    //Methods, checking mouse collisions
    void checkCollision();
    void checkGraphButtonCollisions();

    //Methods, used to handle the graphs container
    float calculateGraphsContainer();
    void animateGraphsContainer();
    void displayGraphCards();

    //Methods for setup and cleanup of variables
    void loadTextures();
    void deleteTextures();

  public:
    MainMenu(std::string sceneName, SceneManager &sceneManager);

    //Derived methods from class Scene
    virtual void Start();

    virtual void Update();

    virtual void onExit();
};