#include "MainMenu.h"


MainMenu::MainMenu(std::string sceneName, SceneManager& sceneManager) : Scene(sceneName), m_sceneManager(sceneManager)
{

}

void MainMenu::Start() // called once, at the start of the scene
{

}

void MainMenu::Update() // called every frame
{
	BeginDrawing();
	ClearBackground(WHITE);

	DrawRectangle(500, 390, 250, 100, BLUE);

	DrawText("MAIN MENU", 1500 / 2, 980 / 3, 15, BLACK);

	EndDrawing();

	Vector2 mousePos = GetMousePosition();

	if (CheckCollisionPointRec(mousePos, { 500, 390, 250, 100 }) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		m_sceneManager.setCurrentScene("Simulation");
	}

}

void MainMenu::onExit() // called once, at the end of the scene
{
	isSetUp = false;
}