#include "mainMenu.h"
#include "graph.h"

MainMenu::MainMenu(std::string sceneName, SceneManager &sceneManager) : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void MainMenu::Start() // called once, at the start of the scene
{
    loadTextures();
    currentGraphPos = {-834, 54};
    statisticNames = ss::bll::statistics::StatisticsManager::getStatisticsNames();
    graphButtonPos = { 525, 92 };
    graphNamePos = { 53, 112 };
    for(std::string statistic : statisticNames)
    {
        graphCards.push_back(graphsCard{ statistic, graphNamePos, graphButtonPos });
        graphNamePos.y += 189;
        graphButtonPos.y += 189;
    }
}

void MainMenu::Update() // called every frame
{
    mousePos = GetMousePosition();

    checkCollision();

    checkGraphButtonCollisions();

    BeginDrawing();

    ClearBackground(MainMenu::backgroundColors.at(static_cast<int>(MainMenu::currentTheme)));

    DrawTexture(simulatorButton_Texture, 525, 736, WHITE);
    DrawTexture(logo_Texture, 310, 219, WHITE);
    DrawTexture(graphsMenu_Texture, 57, 53, WHITE);
    DrawTexture(graphsContainer_Texture, graphsContainerPos, 0, WHITE);
    DrawTexture(themeButton_Texture, 1386, 41, WHITE);

    animateGraphsContainer();
    if (graphsIsAnimatingIn || graphsIsOut)
    {
        displayGraphCards();
    }

    EndDrawing();

    if (offset + GetMouseWheelMove() * 35 <= 0 &&
        std::abs(offset + GetMouseWheelMove() * 35) <= statisticNames.size() * 198 - 1000)
    {
        offset += GetMouseWheelMove() * 35;
    }

}

void MainMenu::onExit() // called once, at the end of the scene
{
    isSetUp = false;
    deleteTextures();
}

float MainMenu::calculateGraphsContainer()
{
    float animationCalc = GetFrameTime() * drag;
    if (graphsContainerPos >= -500 && graphsIsAnimatingIn) drag *= .8983F;
    if (graphsContainerPos <= -500 && graphsIsAnimatingOut) drag *= .8983F;
    if (drag < 30) drag = 30;
    return animationCalc;
}

void MainMenu::animateGraphsContainer()
{

    if (graphsIsAnimatingIn)
    {
        graphsContainerPos += calculateGraphsContainer();
        currentGraphPos.x = graphsContainerPos + 53;
        std::for_each(graphCards.begin(), graphCards.end(), [&](graphsCard& graphCard) 
        {
                graphCard.buttonPos.x = graphsContainerPos + 525;
                graphCard.namePos.x = graphsContainerPos + 92;
        });
    }
    if (graphsIsAnimatingOut)
    {
        graphsContainerPos -= calculateGraphsContainer();
        currentGraphPos.x = graphsContainerPos - 53;
        std::for_each(graphCards.begin(), graphCards.end(), [&](graphsCard& graphCard)
        {
            graphCard.buttonPos.x = graphsContainerPos + 425;
            graphCard.namePos.x = graphsContainerPos + 12;
        });
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
        graphsIsOut = false;
    }
}

void MainMenu::displayGraphCards()
{
    currentGraphPos.y = 54;

    for (auto statistics : statisticNames)
    {
        currentGraphPos.y -= GetMouseWheelMove();
        DrawRectangleRounded({ currentGraphPos.x, currentGraphPos.y + offset, 781, 144 }, 0.54f, 20, {235, 239, 247, 255});
        currentGraphPos.y += 189;

        for (auto graphContainer : graphCards)
        {
            DrawTexture(viewGraph_Texture, graphContainer.buttonPos.x, graphContainer.buttonPos.y + offset, WHITE);
            DrawTextEx(font, graphContainer.name.c_str(), { graphContainer.namePos.x, graphContainer.namePos.y + offset }, 40, 1, BLACK);
        }
    }
}

void MainMenu::checkGraphButtonCollisions()
{
    std::ranges::for_each(graphCards, [this](const graphsCard& graphButton)
        {
            if (CheckCollisionPointRec(mousePos, Rectangle{ graphButton.buttonPos.x, graphButton.buttonPos.y + offset, static_cast<float>(this->viewGraph_Texture.width), static_cast<float>(this->viewGraph_Texture.height) }) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                //std::cout << "Displaying graphs for " << graphButton.name << std::endl;
                ss::pl::graph::Graph::fileName = graphButton.name;
                m_sceneManager.setCurrentScene("Graph");
            }
        });
}

void MainMenu::deleteTextures()
{
    UnloadFont(font);
    UnloadTexture(simulatorButton_Texture);
    UnloadTexture(logo_Texture);
    UnloadTexture(graphsMenu_Texture);
    UnloadTexture(graphsContainer_Texture);
}

// clang-format off
void MainMenu::loadTextures()
{
    font = LoadFontEx("../../assets/fonts/Inter.ttf", 96, 0, 0);
    simulatorButton_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Simulator_Button.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    logo_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Logo_Transparent.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    graphsContainer_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Graphs_Container.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    graphsMenu_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Graphs_Button.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    themeButton_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Theme_Button.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    viewGraph_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/View_Graph_Button.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());

}

auto MainMenu::collisionCoordiantes()
{
    return CheckCollisionPointRec(mousePos, { 525, 736, static_cast<float>(simulatorButton_Texture.width), static_cast<float>(simulatorButton_Texture.height) })
        || CheckCollisionPointRec(mousePos, { 57, 53, static_cast<float>(graphsMenu_Texture.width), static_cast<float>(graphsMenu_Texture.height) })
        || CheckCollisionPointRec(mousePos, { 1386, 41, static_cast<float>(themeButton_Texture.width), static_cast<float>(themeButton_Texture.height) });
}

void MainMenu::checkCollision()
{
    if (collisionCoordiantes())
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (CheckCollisionPointRec(mousePos, { 57, 53, static_cast<float>(graphsMenu_Texture.width), static_cast<float>(graphsMenu_Texture.height) })
        && !graphsIsAnimatingIn && !graphsIsAnimatingOut && !graphsIsOut && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        graphsIsAnimatingIn = true;
    }

    if (CheckCollisionPointRec(mousePos, { 525, 736, static_cast<float>(simulatorButton_Texture.width), static_cast<float>(simulatorButton_Texture.height) })
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !graphsIsAnimatingIn && !graphsIsAnimatingOut && !graphsIsOut)
    {
        m_sceneManager.setCurrentScene("Simulation");
    }

    if (CheckCollisionPointRec(mousePos, { 1386, 41, static_cast<float>(themeButton_Texture.width), static_cast<float>(themeButton_Texture.height) })
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        MainMenu::currentTheme = (MainMenu::currentTheme == ThemeTypes::LightTheme) ? ThemeTypes::DarkTheme : ThemeTypes::LightTheme;
        MainMenu::deleteTextures();
        MainMenu::loadTextures();
    }


    if (!CheckCollisionPointRec(mousePos, { 0, 0, static_cast<float>(graphsContainer_Texture.width), static_cast<float>(graphsContainer_Texture.height) })
        && !CheckCollisionPointRec(mousePos, { 1386, 41, static_cast<float>(themeButton_Texture.width), static_cast<float>(themeButton_Texture.height) })
        && graphsIsOut && !graphsIsAnimatingIn && !graphsIsAnimatingOut)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            graphsIsAnimatingOut = true;
        }
    }

}
// clang-format on