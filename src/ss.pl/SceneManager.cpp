#include "SceneManager.h"

void SceneManager::setCurrentScene(std::string sceneName)
{
	if (currentScene != nullptr) currentScene->onExit();
	currentScene = getScene(sceneName);
	currentScene->Start();
}

void SceneManager::updateScene()
{
	currentScene->Update();
}

void SceneManager::addScene(Scene* scene)
{
	m_scenes.push_back(scene);
}

Scene* SceneManager::getScene(std::string sceneName)
{
	return (*std::ranges::find_if(m_scenes, [&sceneName](Scene* scene) {return scene->m_sceneName == sceneName; }));
}