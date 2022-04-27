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

    SetWindowIcon(LoadImage("../../assets/logos/Logo_Square_Transparent.png"));

    SceneManager sceneManager;

    ss::pl::mainMenu::MainMenu scene_MainMenu{"MainMenu", sceneManager};
    ss::pl::simulator::Simulator scene_Simulation{"Simulation", sceneManager};
    ss::pl::graph::Graph scene_Graph{"Graph", sceneManager};

    sceneManager.addScene(&scene_MainMenu);
    sceneManager.addScene(&scene_Simulation);
    sceneManager.addScene(&scene_Graph);

    sceneManager.setCurrentScene("MainMenu");

    while (!WindowShouldClose())
    {
        sceneManager.updateScene();
    }

    CloseWindow();
}