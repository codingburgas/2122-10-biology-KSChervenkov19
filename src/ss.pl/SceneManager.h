#pragma once

#include <iostream>
#include <vector>
#include "Scene.h"

class SceneManager
{

public:

	void setCurrentScene(std::string sceneName);

	void updateScene();

	void addScene(Scene* scene);

private:

	Scene* getScene(std::string sceneName);

	Scene* currentScene = nullptr;

	std::vector<Scene*> m_scenes;

};