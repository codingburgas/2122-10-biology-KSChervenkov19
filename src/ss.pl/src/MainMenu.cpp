#include "MainMenu.h"

MainMenu::MainMenu(std::string sceneName, SceneManager &sceneManager) : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void MainMenu::Start() // called once, at the start of the scene
{
    font = LoadFontEx("../../assets/OpenSans.ttf", 96, 0, 0);
}

void MainMenu::Update() // called every frame
{
    BeginDrawing();
    ClearBackground(WHITE);

    // DrawRectangle(560, 250, 380, 80, BLUE);

    DrawTextEx(font, "Simulation", { 560, 250 }, 80, 10, BLACK);
    DrawTextEx(font, "Graphs", { 628, 450 }, 80, 10, BLACK);
    DrawTextEx(font, "Exit", { 685, 650 }, 80, 10, BLACK);

    EndDrawing();

    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, { 560, 250, 380, 80 }))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            m_sceneManager.setCurrentScene("Simulation");
        }
        SetMouseCursor(4);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void MainMenu::onExit() // called once, at the end of the scene
{
    isSetUp = false;
}