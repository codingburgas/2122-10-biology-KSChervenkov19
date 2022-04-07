#pragma once

#include <raylib.h>
#include "SceneManager.h"


class Simulation : public Scene
{
public:

	Simulation(std::string sceneName);

	virtual void Start();

	virtual void Update();

	void onExit();

	SceneManager* sceneManager;
};