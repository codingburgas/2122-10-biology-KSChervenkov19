#include "graph.h"
#include "mainMenu.h"
#include "sceneManager.h"
#include "simulator.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

const int screenWidth = 1500;
const int screenHeight = 980;

int main()
{
    InitWindow(screenWidth, screenHeight, "Simulation");

    SetTargetFPS(60);

    SceneManager sceneManager;

    ss::pl::mainMenu::MainMenu scene_MainMenu{"MainMenu", sceneManager};
    ss::pl::simulator::Simulation scene_Simulation{"Simulation", sceneManager};
    ss::pl::graph::Graph scene_Graph{"Graph", sceneManager};

    sceneManager.addScene(&scene_MainMenu);
    sceneManager.addScene(&scene_Simulation);
    sceneManager.addScene(&scene_Graph);

    sceneManager.setCurrentScene("MainMenu");

    while (!WindowShouldClose())
    {

        // fElapsedTime = timeScale * GetFrameTime();

        // cubePos.x += cubeSpeed * fElapsedTime;

        // timeScale = GuiSlider(Rectangle{ screenWidth/2 - 165.0f / 2.0f, screenHeight - 25.0f, 165, 20 }, "TIME
        // SCALE", TextFormat("%0.1f", timeScale), timeScale, 0.5f, 100.0f);
        sceneManager.updateScene();
    }

    CloseWindow();
}