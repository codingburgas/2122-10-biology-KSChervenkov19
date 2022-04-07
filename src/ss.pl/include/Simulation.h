#pragma once

#include <raylib.h>
#include "SceneManager.h"


class Simulation : public Scene
{
public:

	Simulation(std::string sceneName, SceneManager& sceneManager);

	virtual void Start();

	virtual void Update();

	void onExit();

	SceneManager& m_sceneManager;
};