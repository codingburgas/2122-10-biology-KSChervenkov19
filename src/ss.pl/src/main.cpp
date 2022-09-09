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
    InitWindow(screenWidth, screenHeight, "NANO Simulator");

    SetTargetFPS(60);

    SetWindowIcon(LoadImage("../../assets/logos/Logo_Square_Transparent.png"));
    Texture2D panicImage = LoadTexture("../../assets/panic.png");

    SceneManager sceneManager;

    ss::pl::mainMenu::MainMenu scene_MainMenu{"MainMenu", sceneManager};
    ss::pl::simulator::Simulator scene_Simulation{"Simulation", sceneManager};
    ss::pl::graph::Graph scene_Graph{"Graph", sceneManager};

    sceneManager.addScene(&scene_MainMenu);
    sceneManager.addScene(&scene_Simulation);
    sceneManager.addScene(&scene_Graph);

    sceneManager.setCurrentScene("MainMenu");

    if (GetMonitorWidth(0) <= screenWidth)
    {
        while (!WindowShouldClose())
        {
            BeginDrawing();

            ClearBackground(WHITE);
            DrawTexture(panicImage, 0, 0, WHITE);

            EndDrawing();
        }
    }

    else
    {
        while (!WindowShouldClose())
        {
            sceneManager.updateScene();
        }
    }

    CloseWindow();
}