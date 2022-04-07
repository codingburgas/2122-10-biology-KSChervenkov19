#pragma once

#include <raylib.h>
#include "SceneManager.h"

class MainMenu : public Scene
{
public:

	MainMenu(std::string sceneName, SceneManager& sceneManager);

	void Start();

	void Update();

	void onExit();

	SceneManager& m_sceneManager;

};