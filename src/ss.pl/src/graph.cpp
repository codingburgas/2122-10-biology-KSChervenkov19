#include "graph.h"
#include "statistics_manager.h"
#include <algorithm>

/// Constructor for the Graph class.
///
/// @param sceneName The name of the current scene. Which here is "Graph".
/// @param sceneManager A reference to already existing SceneManager object to control the program flow.
ss::pl::graph::Graph::Graph(std::string sceneName, SceneManager &sceneManager)
    : Scene(sceneName), m_sceneManager(sceneManager)
{
}

/// Method which is called in the start of the Graph page.
void ss::pl::graph::Graph::Start() // called once, at the start of the scene
{
    loadAssets();

    cycleInfo = ss::bll::statistics::StatisticsManager::getStatisticFrom(fileName);
    maxSenseAndSpeed = getHighestSenseAndSpeed(cycleInfo);
    currentCycle = 1;
    totalAlive = cycleInfo[0].lastedEntities;

    getTraitData(currentCycle);

    autoCycle = false;
}

/// Method which is called every frame.
///
/// It updates the current scene every frame.
void ss::pl::graph::Graph::Update() // called every frame
{
    BeginDrawing();

    ClearBackground(backgroundColors.at(static_cast<int>(currentTheme)));

    drawGraph();
    drawMenu();
    drawTrait();

    EndDrawing();

    checkCollision();
    if (currentAutoState == AutoState::AutoOn)
        automateCycle();
}

/// Method which is called when we exit the program or the Graph page.
///
/// It deallocates every dynamically created object in the class' instance.
void ss::pl::graph::Graph::onExit() // called once, at the end of the scene
{
    deleteAssets();
}

/// Getter highest sense function.
///
/// Function which gets the highest sense of a generation holding in a .json file.
/// @param cycles Generation data.
/// @return Highest sense as a float.
// clang-format off
float ss::pl::graph::Graph::getHighestSense(const std::vector<ss::types::Cycle>& cycles)
{
	auto maxSenseFromCycle = [](const std::vector<ss::types::Trait>& traits)
	{
		return std::ranges::max_element(traits, [](const ss::types::Trait& lhs, const ss::types::Trait& rhs)
			{
				return lhs.sense < rhs.sense;
			})->sense;
	};

	std::vector<float> maxSenses;
	maxSenses.reserve(cycles.size());

	std::ranges::for_each(cycles, [&maxSenseFromCycle, &maxSenses](const ss::types::Cycle& cycle)
		{
			maxSenses.push_back(maxSenseFromCycle(cycle.traitsInfo));
		});

	return *std::ranges::max_element(maxSenses);
}

/// Getter highest speed function.
///
/// Function which gets the highest speed of a generation holding in a .json file.
/// @param cycles Generation data.
/// @return Highest speed as a float.
float ss::pl::graph::Graph::getHighestSpeed(const std::vector<ss::types::Cycle>& cycles)
{
	auto maxSpeedFromCycle = [](const std::vector<ss::types::Trait>& traits)
	{
		return std::ranges::max_element(traits, [](const ss::types::Trait& lhs, const ss::types::Trait& rhs)
			{
				return lhs.speed < rhs.speed;
			})->speed;
	};

	std::vector<float> maxSpeeds;
	maxSpeeds.reserve(cycles.size());

	std::ranges::for_each(cycles, [&maxSpeedFromCycle, &maxSpeeds](const ss::types::Cycle& cycle)
		{
			maxSpeeds.push_back(maxSpeedFromCycle(cycle.traitsInfo));
		});

	return *std::ranges::max_element(maxSpeeds);
}
// clang-format on

/// Function returning the highest sense and speed.
///
/// Function returning the highest sense and speed as a pair of floats.
/// @param cycles Generation data.
/// @return A pair of floats holding sense and speed.
std::pair<float, float> ss::pl::graph::Graph::getHighestSenseAndSpeed(const std::vector<ss::types::Cycle> &cycle)
{
    return {getHighestSense(cycle), getHighestSpeed(cycle)};
}

/// Method for getting the Growth percentage.
///
/// It returns the absolute value of the percentage when the current cycle' lasted entities had growth.
/// @param lastedBef It holds the lasted entities in the previous cycle.
/// @param lastedCur it holds the lasted entities in the current cycle.
/// @return The percentage when it's growing.
inline float ss::pl::graph::Graph::getGrowthPercentage(int lastedBef, int lastedCur)
{
    return std::abs(static_cast<float>(lastedCur - lastedBef) / lastedBef * 100);
}

/// Method for getting the Decreased percentage.
///
/// It returns the absolute value of the percentage when the current cycle' lasted entities had growth.
/// @param lastedBef It holds the lasted entities in the previous cycle.
/// @param lastedCur it holds the lasted entities in the current cycle.
/// @return The percentage when it's decreasing.
inline float ss::pl::graph::Graph::getDecreasedPercentage(int lastedBef, int lastedCur)
{
    return std::abs(static_cast<float>(lastedBef - lastedCur) / lastedBef * 100);
}

void ss::pl::graph::Graph::getTraitData(size_t cycle)
{
    traitData = cycleInfo[cycle - 1].traitsInfo;
}

void ss::pl::graph::Graph::drawTrait()
{
    for (auto trait : traitData)
    {
        DrawCircleV({ calculateTraitPosition(trait.sense, trait.speed) }, 10, {117, 105, 255, 255});
    }
}

Vector2 ss::pl::graph::Graph::calculateTraitPosition(float sence, float speed)
{
    speed = ((727 / maxSenseAndSpeed.second) * speed) + 198;
    sence = ((691 / maxSenseAndSpeed.first) * (maxSenseAndSpeed.first - sence)) + 118;
    return { speed, sence };
}

/// Method for drawing the Graph body.
void ss::pl::graph::Graph::drawGraph()
{
    graphLine.y = 159;
    graphLine.x = 219;
    graphText.y = 155;
    graphText.x = 840;

    currentSense = maxSenseAndSpeed.first - maxSenseAndSpeed.second / 9;
    currentSpeed = maxSenseAndSpeed.second - maxSenseAndSpeed.second / 9;

    DrawTexture(backArrow_Texture, 50, 90, WHITE);
    DrawTexture(graph_Container, 1020, 0, WHITE);
    DrawTexture(data_Container, 1025, 47, WHITE);
    if (!populationChange.empty())
        DrawTexture(data_Container, 1025, 197, WHITE);

    DrawRectangle(198, 118, 9, 691, {196, 196, 196, 255});
    DrawRectangle(198, 809, 727, 9, {196, 196, 196, 255});

    DrawTextEx(fontInter, "0", {165, 826}, 39, 0, {108, 108, 108, 255});
    DrawTextEx(fontInter, TextFormat("%.1f", maxSenseAndSpeed.first), {124, 90}, 39, 0, {108, 108, 108, 255});
    DrawTextEx(fontInter, TextFormat("%.1f", maxSenseAndSpeed.second), {900, 826}, 39, 0, {108, 108, 108, 255});
    DrawTextEx(fontInter, "speed", {478, 868}, 39, 0, BLACK);
    DrawTextEx(fontInter, std::format("Alive: {}", totalAlive).c_str(), {1060, 88}, 50, 0, BLACK);

    // if it ain't broke, don't touch it
    DrawTextEx(fontInter, populationChange.c_str(), {1060, 237}, 50, 0, BLACK);
    DrawTextPro(fontInter, "sense", {72, 537}, {0, 0}, 270, 39, 0, BLACK);

    for (int i = 0; i < 8; i++)
    {
        DrawRectangle(190, graphLine.y, 26, 9, {196, 196, 196, 255});
        DrawRectangle(graphLine.x, 800, 9, 26, {196, 196, 196, 255});

        DrawTextEx(fontInter, TextFormat("%.1f", (currentSense)), {140, static_cast<float>(graphText.y)}, 19, 0,
                   {108, 108, 108, 255});
        DrawTextEx(fontInter, TextFormat("%.1f", (currentSpeed)), {static_cast<float>(graphText.x), 844}, 19, 0,
                   {108, 108, 108, 255});

        graphLine.y += 90;
        graphLine.x += 90;
        graphText.y += 90;
        graphText.x -= 90;

        currentSense = currentSense - maxSenseAndSpeed.first / 9;
        currentSpeed = currentSpeed - maxSenseAndSpeed.second / 9;
    }
}

/// Method for drawing the menu in the graph page.
void ss::pl::graph::Graph::drawMenu()
{
    DrawTexture(cycle_Next, 1398, 836, WHITE);
    DrawTexture(cycle_Prev, 1045, 836, WHITE);
    DrawTexture(slider_Box, 1365, 747, WHITE);
    DrawTexture(auto_Button, 1161, 611, WHITE);

    DrawTextEx(fontInter, TextFormat("%i", (currentCycle)), {1238, 851}, 49, 0, BLACK);
    DrawTextEx(fontInter, "Auto", {1049, 617}, 39, 0, BLACK);
    DrawTextEx(fontInter, "Seconds:", {1049, 696}, 39, 0, BLACK);
    DrawTextEx(fontInter, TextFormat("%i", (cycleSpeed)), {1405, 757}, 33, 0, BLACK);

    cycleSpeed = GuiSliderBar({1046, 747, 320, 53}, nullptr, nullptr, cycleSpeed, 1, 21);
}

/// Method for loading all the needed assets in the graph page.
void ss::pl::graph::Graph::loadAssets()
{
    fontInter = LoadFontEx("../../assets/fonts/Inter.ttf", 95, 0, 0);

    backArrow_Texture = LoadTexture(
        std::format("../../assets/{}/graph/Back_Arrow.png", themePaths.at(static_cast<int>(Graph::currentTheme)))
            .c_str());
    graph_Container = LoadTexture(
        std::format("../../assets/{}/graph/Graph_Container.png", themePaths.at(static_cast<int>(Graph::currentTheme)))
            .c_str());
    cycle_Next = LoadTexture(
        std::format("../../assets/{}/graph/Cycle_Next.png", themePaths.at(static_cast<int>(Graph::currentTheme)))
            .c_str());
    cycle_Prev = LoadTexture(
        std::format("../../assets/{}/graph/Cycle_Prev.png", themePaths.at(static_cast<int>(Graph::currentTheme)))
            .c_str());
    slider_Box = LoadTexture(
        std::format("../../assets/{}/graph/Slider_Box.png", themePaths.at(static_cast<int>(Graph::currentTheme)))
            .c_str());
    auto_Button =
        LoadTexture(std::format("../../assets/{}/graph/{}.png", themePaths.at(static_cast<int>(Graph::currentTheme)),
                                autoPaths.at(static_cast<int>(Graph::currentAutoState)))
                        .c_str());
    data_Container = LoadTexture(
        std::format("../../assets/{}/graph/Data_Container.png", themePaths.at(static_cast<int>(Graph::currentTheme)))
            .c_str());

    GuiLoadStyle((currentTheme == ThemeTypes::LightTheme) ? "../../assets/bluish.txt.rgs"
                                                          : "../../assets/lavanda.txt.rgs");
}

/// Method for deallocating the dynamically created assets.
void ss::pl::graph::Graph::deleteAssets()
{
    UnloadTexture(backArrow_Texture);
    UnloadTexture(graph_Container);
    UnloadTexture(cycle_Next);
    UnloadTexture(cycle_Prev);
    UnloadTexture(slider_Box);
    UnloadTexture(auto_Button);
    UnloadTexture(data_Container);

    UnloadFont(fontInter);
}

/// Method for checking the collision on click.
void ss::pl::graph::Graph::checkCollision()
{
    mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, {50, 90, static_cast<float>(backArrow_Texture.width),
                                          static_cast<float>(backArrow_Texture.height)}))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (CheckCollisionPointRec(mousePos, {50, 90, static_cast<float>(backArrow_Texture.width),
                                          static_cast<float>(backArrow_Texture.height)}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_sceneManager.setCurrentScene("MainMenu");
    }

    if (CheckCollisionPointRec(
            mousePos, {1398, 836, static_cast<float>(cycle_Next.width), static_cast<float>(cycle_Next.height)}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (currentCycle != cycleInfo.size()) currentCycle++;

        getTraitData(currentCycle);

        totalAlive = cycleInfo[currentCycle - 1].lastedEntities;
        if (currentCycle != 1)
        {
            populationChange =
                (cycleInfo[currentCycle - 1].lastedEntities > cycleInfo[currentCycle - 2].lastedEntities)
                    ? std::format("Growth with: {}%",
                                  std::round(getGrowthPercentage(cycleInfo[currentCycle - 2].lastedEntities,
                                                                 cycleInfo[currentCycle - 1].lastedEntities)))
                    : std::format("Decrease with: {}%",
                                  std::round(getDecreasedPercentage(cycleInfo[currentCycle - 2].lastedEntities,
                                                                    cycleInfo[currentCycle - 1].lastedEntities)));
        }
        else
        {
            populationChange = "";
        }
    }

    if (CheckCollisionPointRec(
            mousePos, {1045, 836, static_cast<float>(cycle_Prev.width), static_cast<float>(cycle_Prev.height)}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (currentCycle - 1 != 0) currentCycle--;

        getTraitData(currentCycle);

        totalAlive = cycleInfo[currentCycle - 1].lastedEntities;
        if (currentCycle != 1)
        {
            populationChange =
                (cycleInfo[currentCycle - 1].lastedEntities > cycleInfo[currentCycle - 2].lastedEntities)
                    ? std::format("Growth with: {}%",
                                  std::round(getGrowthPercentage(cycleInfo[currentCycle - 2].lastedEntities,
                                                                 cycleInfo[currentCycle - 1].lastedEntities)))
                    : std::format("Decrease with: {}%",
                                  std::round(getDecreasedPercentage(cycleInfo[currentCycle - 2].lastedEntities,
                                                                    cycleInfo[currentCycle - 1].lastedEntities)));
        }
        else
        {
            populationChange = "";
        }
    }

    if (CheckCollisionPointRec(
            mousePos, {1161, 611, static_cast<float>(auto_Button.width), static_cast<float>(auto_Button.height)}) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        currentAutoState == AutoState::AutoOff ? currentAutoState = AutoState::AutoOn
                                               : currentAutoState = AutoState::AutoOff;
        deleteAssets();
        loadAssets();
    }
}

void ss::pl::graph::Graph::automateCycle()
{
    fElapsedTime += GetFrameTime();

    if (fElapsedTime >= cycleSpeed && currentCycle != cycleInfo.size())
    {
        currentCycle++;

        getTraitData(currentCycle);

        fElapsedTime = 0;

        totalAlive = cycleInfo[currentCycle - 1].lastedEntities;

        if (currentCycle != 1)
        {
            populationChange =
                (cycleInfo[currentCycle - 1].lastedEntities > cycleInfo[currentCycle - 2].lastedEntities)
                    ? std::format("Growth with: {}%",
                                  std::round(getGrowthPercentage(cycleInfo[currentCycle - 2].lastedEntities,
                                                                 cycleInfo[currentCycle - 1].lastedEntities)))
                    : std::format("Decrease with: {}%",
                                  std::round(getDecreasedPercentage(cycleInfo[currentCycle - 2].lastedEntities,
                                                                    cycleInfo[currentCycle - 1].lastedEntities)));
        }
        else
        {
            populationChange = "";
        }
    }
}