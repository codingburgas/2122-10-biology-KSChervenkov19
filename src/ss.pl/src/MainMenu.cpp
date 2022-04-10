#include "MainMenu.h"

MainMenu::MainMenu(std::string sceneName, SceneManager &sceneManager) : Scene(sceneName), m_sceneManager(sceneManager)
{
}

void MainMenu::Start() // called once, at the start of the scene
{
    font = LoadFontEx("../../assets/OpenSans.ttf", 96, 0, 0);
    buttonTexture = LoadTexture("../../assets/button.png");
    logoTexture = LoadTexture("../../assets/Logo_Transparent_Light.png");
}

void MainMenu::Update() // called every frame
{
    BeginDrawing();
    ClearBackground({ 4, 12, 32, 255 });
    //ClearBackground(WHITE);

    DrawTexture(buttonTexture, 525, 736, WHITE);
    DrawTexture(logoTexture, 310, 219, WHITE);
    EndDrawing();

    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, { 525, 736, static_cast<float>(buttonTexture.width), static_cast<float>(buttonTexture.height) }))
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
    UnloadFont(font);
    UnloadTexture(buttonTexture);
    UnloadTexture(logoTexture);
}