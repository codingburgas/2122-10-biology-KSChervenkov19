#pragma once

#include <raylib.h>
#include "SceneManager.h"

class MainMenu : public Scene
{
public:

	MainMenu(std::string sceneName);

	void Start();

	void Update();

	void onExit();

	SceneManager* sceneManager;

};