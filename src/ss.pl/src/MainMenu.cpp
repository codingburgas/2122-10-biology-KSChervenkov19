#include "mainMenu.h"

MainMenu::MainMenu(std::string sceneName, SceneManager &sceneManager) : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void MainMenu::Start() // called once, at the start of the scene
{
    font = LoadFontEx("../../assets/OpenSans.ttf", 96, 0, 0);
    simulatorButton_Texture = LoadTexture("../../assets/assets_MainMenu/Simulator_Button.png");
    logo_Texture = LoadTexture("../../assets/Logo_Transparent_Light.png");
    graphsContainer = new Container("../../assets/assets_MainMenu/Graphs_Container.png", {0, 0});
    graphsMenu_Texture = LoadTexture("../../assets/assets_MainMenu/Graphs_Button.png");
    themeButton_Texture = LoadTexture("../../assets/assets_MainMenu/Theme_Button.png");
    // MainMenu::m_theme = "darkTheme";
    // std::cout << MainMenu::m_theme;
}

void MainMenu::Update() // called every frame
{
    BeginDrawing();

    ClearBackground({4, 12, 32, 255});

    DrawTexture(simulatorButton_Texture, 525, 736, WHITE);
    DrawTexture(logo_Texture, 310, 219, WHITE);
    // DrawTexture(graphsContainer_Texture, graphsContainerPos, 0, WHITE);
    // graphsContainer->display();
    DrawTexture(graphsMenu_Texture, 57, 53, WHITE);
    DrawTexture(themeButton_Texture, 1386, 41, WHITE);

    EndDrawing();

    checkCollision();

    animateGraphsContainer();
}

void MainMenu::onExit() // called once, at the end of the scene
{
    isSetUp = false;
    UnloadFont(font);
    UnloadTexture(simulatorButton_Texture);
    UnloadTexture(logo_Texture);
    /*UnloadTexture(graphsContainer_Texture);*/
    UnloadTexture(graphsMenu_Texture);
    graphsContainer->unloadTexture();
    delete graphsContainer;
}

float MainMenu::calculateGraphsContainer()
{
    float animationCalc = GetFrameTime() * drag;
    drag *= .9441F;
    return animationCalc;
}

void MainMenu::animateGraphsContainer()
{
    /*
    if (graphsIsAnimatingIn)
    {
        graphsContainerPos += calculateGraphsContainer();
    }
    if (graphsIsAnimatingOut)
    {
        graphsContainerPos -= calculateGraphsContainer();
    }
    if (graphsContainerPos >= 0)
    {
        graphsIsAnimatingIn = false;
        drag = 3000;
        graphsContainerPos = 0;
        graphsIsOut = true;
    }
    if (graphsContainerPos <= -887)
    {
        graphsIsAnimatingOut = false;
        drag = 3000;
        graphsContainerPos = -887;
    }
    */
}

// clang-format off
auto MainMenu::collisionCoordiantes()
{
    return CheckCollisionPointRec(mousePos, { 525, 736, static_cast<float>(simulatorButton_Texture.width), static_cast<float>(simulatorButton_Texture.height) })
        || CheckCollisionPointRec(mousePos, { 57, 53, static_cast<float>(graphsMenu_Texture.width), static_cast<float>(graphsMenu_Texture.height) })
        || CheckCollisionPointRec(mousePos, { 1386, 41, static_cast<float>(themeButton_Texture.width), static_cast<float>(themeButton_Texture.height) });
}

void MainMenu::checkCollision()
{
    mousePos = GetMousePosition();

    if (collisionCoordiantes())
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (CheckCollisionPointRec(mousePos, { 57, 53, static_cast<float>(graphsMenu_Texture.width), static_cast<float>(graphsMenu_Texture.height) })
        && !graphsIsAnimatingIn && !graphsIsAnimatingOut && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        graphsIsAnimatingIn = true;
    }

    if (CheckCollisionPointRec(mousePos, { 525, 736, static_cast<float>(simulatorButton_Texture.width), static_cast<float>(simulatorButton_Texture.height) }) 
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_sceneManager.setCurrentScene("Simulation");
    }

    if (CheckCollisionPointRec(mousePos, { 1386, 41, static_cast<float>(themeButton_Texture.width), static_cast<float>(themeButton_Texture.height) })
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        std::cout << "Theme Switched!";
    }

    /*
    if (!CheckCollisionPointRec(mousePos, { 0, 0, static_cast<float>(graphsContainer_Texture.width), static_cast<float>(graphsContainer_Texture.height) })
        && graphsIsOut && !graphsIsAnimatingIn && !graphsIsAnimatingOut)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            graphsIsAnimatingOut = true;
        }
    }
    */
}
// clang-format on