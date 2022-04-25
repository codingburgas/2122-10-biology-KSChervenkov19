#include "mainMenu.h"
#include "graph.h"

/// Constructor for the MainMenu class.
///
/// @param sceneName The name of the current scene. Which here is "MainMenu".
/// @param sceneManager A reference to already existing SceneManager object to control the program flow.
ss::pl::mainMenu::MainMenu::MainMenu(std::string sceneName, SceneManager &sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager)
{
}

/// Method which is called in the start of the MainMenu page.
void ss::pl::mainMenu::MainMenu::Start() // called once, at the start of the scene
{
    loadAssets();

    currentGraphPos = {-834, 54};
    statisticNames = ss::bll::statistics::StatisticsManager::getStatisticsNames();
    graphButtonPos = {525, 92};
    graphNamePos = {53, 112};
    graphCards.clear();

    currentAnimationFrameRect = { 0.0f, 0.0f, (float)themeSwitchAnimationSpritesheet.width / 12, (float)themeSwitchAnimationSpritesheet.height };

    for (std::string statistic : statisticNames)
    {
        graphCards.push_back(graphsCard{statistic, graphNamePos, graphButtonPos});
        graphNamePos.y += 189;
        graphButtonPos.y += 189;
    }
}

/// Method which is called every frame.
///
/// It updates the current scene every frame.
void ss::pl::mainMenu::MainMenu::Update() // called every frame
{
    mousePos = GetMousePosition();

    checkCollision();

    BeginDrawing();

    ClearBackground(MainMenu::backgroundColors.at(static_cast<int>(MainMenu::currentTheme)));

    drawTextures();

    animateGraphsContainer();

    if (graphsIsAnimatingIn || graphsIsOut)
    {
        displayGraphCards();
    }

    if (animationIsPlaying)
    {
        auto themeSwitchAnim = std::async(&MainMenu::playThemeAnimation, this);
        themeSwitchAnim.get();
    }
    EndDrawing();

    handleScroll();
}

/// Method which is called when we exit the program or the MainMenu page.
///
/// It deallocates every dynamically created object in the class' instance.
void ss::pl::mainMenu::MainMenu::onExit() // called once, at the end of the scene
{
    deleteAssets();

    isSetUp = false;
}

/// This method calculates the Graph container's place.
///
/// @return The possition in float.
float ss::pl::mainMenu::MainMenu::calculateGraphsContainer()
{
    float animationCalc = GetFrameTime() * graphContainerAnimationEase;

    if (graphsContainerPos >= -500 && graphsIsAnimatingIn)
        graphContainerAnimationEase *= .8983F;
    if (graphsContainerPos <= -500 && graphsIsAnimatingOut)
        graphContainerAnimationEase *= .8983F;
    if (graphContainerAnimationEase < 30)
        graphContainerAnimationEase = 30;

    return animationCalc;
}

/// Handling the scroll button.
void ss::pl::mainMenu::MainMenu::handleScroll()
{
    if (scrollOffset + GetMouseWheelMove() * 35 <= 0 &&
        std::abs(scrollOffset + GetMouseWheelMove() * 35) <= statisticNames.size() * 198 - 1000)
    {
        scrollOffset += GetMouseWheelMove() * 35;
    }
}

/// This method positions the graph container through the animation.
void ss::pl::mainMenu::MainMenu::positionGraphsContainer()
{
    currentGraphPos.x = (graphsIsAnimatingOut) ? graphsContainerPos - 53 : graphsContainerPos + 53;

    std::for_each(graphCards.begin(), graphCards.end(), [&](graphsCard &graphCard) {
        graphCard.buttonPos.x = (graphsIsAnimatingOut) ? graphsContainerPos + 425 : graphsContainerPos + 525;
        graphCard.namePos.x = (graphsIsAnimatingOut) ? graphsContainerPos + 12 : graphsContainerPos + 92;
    });
}

/// This method slows down the showing of the graph container.
void ss::pl::mainMenu::MainMenu::animateGraphsContainer()
{
    if (graphsContainerPos >= 0)
    {
        positionGraphsContainer();
        graphsIsAnimatingIn = false;
        graphContainerAnimationEase = 3000;
        graphsContainerPos = 0;
        graphsIsOut = true;
    }

    if (graphsContainerPos <= -887)
    {
        graphsIsAnimatingOut = false;
        graphContainerAnimationEase = 3000;
        graphsContainerPos = -887;
        graphsIsOut = false;
    }

    if (graphsIsAnimatingIn)
    {
        graphsContainerPos += calculateGraphsContainer();
        positionGraphsContainer();
    }

    if (graphsIsAnimatingOut)
    {
        graphsContainerPos -= calculateGraphsContainer();
        positionGraphsContainer();
    }
}

/// This method display the graph cards on click on the menu.
void ss::pl::mainMenu::MainMenu::displayGraphCards()
{
    checkGraphButtonCollisions();
    currentGraphPos.y = 54;

    for (auto statistics : statisticNames)
    {
        currentGraphPos.y -= GetMouseWheelMove();
        DrawTexture(statisticCard_Texture, currentGraphPos.x, currentGraphPos.y + scrollOffset, WHITE);
        /*DrawRectangleRounded({currentGraphPos.x, currentGraphPos.y + scrollOffset, 781, 144}, 0.54f, 20,
                             {235, 239, 247, 255});*/
        currentGraphPos.y += 189;

        for (auto graphContainer : graphCards)
        {
            DrawTexture(viewGraph_Texture, graphContainer.buttonPos.x, graphContainer.buttonPos.y + scrollOffset,
                        WHITE);
            DrawTextEx(fontInter, graphContainer.name.c_str(),
                       {graphContainer.namePos.x, graphContainer.namePos.y + scrollOffset}, 40, 1,
                       currentTheme == ThemeTypes::LightTheme ? WHITE : Color{23, 0, 71, 255});
        }
    }
}

void ss::pl::mainMenu::MainMenu::playThemeAnimation()
{
    if (!animationIsPlaying) return;

    currentAnimationFrameRect.x = (float)currentAnimationFrame * (float)themeSwitchAnimationSpritesheet.width / 12;
    DrawTextureRec(themeSwitchAnimationSpritesheet, currentAnimationFrameRect, { 0.0f, 0.0f }, WHITE);
    currentAnimationFrame++;
    if (currentAnimationFrame == 11)
    {
        currentAnimationFrame = 0;
        animationIsPlaying = false;
    }

}

/// This method checks which of the graph buttons is clicked.
void ss::pl::mainMenu::MainMenu::checkGraphButtonCollisions()
{
    std::ranges::for_each(graphCards, [this](const graphsCard &graphButton) {
        if (CheckCollisionPointRec(mousePos, Rectangle{graphButton.buttonPos.x, graphButton.buttonPos.y + scrollOffset,
                                                       static_cast<float>(this->viewGraph_Texture.width),
                                                       static_cast<float>(this->viewGraph_Texture.height)}) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            ss::pl::graph::Graph::fileName = graphButton.name;
            m_sceneManager.setCurrentScene("Graph");
            graphsIsAnimatingIn = true;
        }
    });
}


// clang-format off

/// Method for loading all the needed assets in the graph page.
void ss::pl::mainMenu::MainMenu::loadAssets()
{
    fontInter = LoadFontEx("../../assets/fonts/Inter.ttf", 90, 0, 0);

    simulatorButton_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Simulator_Button.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    logo_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Logo_Transparent.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    graphsContainer_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Graphs_Container.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    statisticCard_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Statistics_Card.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    graphsMenu_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Graphs_Button.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    themeButton_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/Theme_Button.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    viewGraph_Texture = LoadTexture(std::format("../../assets/{}/mainMenu/View_Graph_Button.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    background_Lines = LoadTexture(std::format("../../assets/{}/mainMenu/Background_Lines.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
    themeSwitchAnimationSpritesheet = LoadTexture(std::format("../../assets/{}/mainMenu/animation/switchAnimationSpritesheet.png", themePaths.at(static_cast<int>(MainMenu::currentTheme))).c_str());
}

/// Method for deallocating the dynamically created assets.
void ss::pl::mainMenu::MainMenu::deleteAssets()
{
    UnloadFont(fontInter);

    UnloadTexture(simulatorButton_Texture);
    UnloadTexture(logo_Texture);
    UnloadTexture(graphsContainer_Texture);
    UnloadTexture(statisticCard_Texture);
    UnloadTexture(graphsMenu_Texture);
    UnloadTexture(themeButton_Texture);
    UnloadTexture(viewGraph_Texture);
    UnloadTexture(background_Lines);
    UnloadTexture(themeSwitchAnimationSpritesheet);
}

/// This method draws all the needed assets in the MainMenu page.
void ss::pl::mainMenu::MainMenu::drawTextures()
{
    DrawTexture(background_Lines, 0, 0, WHITE);
    DrawTexture(simulatorButton_Texture, 509, 682, WHITE);
    DrawTexture(logo_Texture, 310, 219, WHITE);
    DrawTexture(graphsMenu_Texture, 17, 10, WHITE);
    DrawTexture(graphsContainer_Texture, graphsContainerPos, 0, WHITE);
    DrawTexture(themeButton_Texture, 1359, 15, WHITE);
}

/// This method handles when the mouse cursor is on click.
/// @return bool.
bool ss::pl::mainMenu::MainMenu::handleMouseCursor()
{
    return CheckCollisionPointRec(mousePos, { 509, 682, static_cast<float>(simulatorButton_Texture.width), static_cast<float>(simulatorButton_Texture.height) })
        || CheckCollisionPointRec(mousePos, { 17, 10, static_cast<float>(graphsMenu_Texture.width), static_cast<float>(graphsMenu_Texture.height) })
        || CheckCollisionPointRec(mousePos, { 1359, 15, static_cast<float>(themeButton_Texture.width), static_cast<float>(themeButton_Texture.height) });
}

/// This method checks for collision on every possible button.
void ss::pl::mainMenu::MainMenu::checkCollision()
{

    SetMouseCursor(handleMouseCursor() ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);

    if (CheckCollisionPointRec(mousePos, { 17, 10, static_cast<float>(graphsMenu_Texture.width), static_cast<float>(graphsMenu_Texture.height) })
        && !graphsIsAnimatingIn && !graphsIsAnimatingOut && !graphsIsOut && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        graphsIsAnimatingIn = true;
    }

    if (CheckCollisionPointRec(mousePos, { 509, 682, static_cast<float>(simulatorButton_Texture.width), static_cast<float>(simulatorButton_Texture.height) })
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !graphsIsAnimatingIn && !graphsIsAnimatingOut && !graphsIsOut)
    {
        m_sceneManager.setCurrentScene("Simulation");
    }

    if (CheckCollisionPointRec(mousePos, { 1359, 15, static_cast<float>(themeButton_Texture.width), static_cast<float>(themeButton_Texture.height) })
        && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        MainMenu::currentTheme = (MainMenu::currentTheme == ThemeTypes::LightTheme) ? ThemeTypes::DarkTheme : ThemeTypes::LightTheme;
        MainMenu::deleteAssets();
        MainMenu::loadAssets();
        animationIsPlaying = true;
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