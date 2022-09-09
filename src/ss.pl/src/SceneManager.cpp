#include "sceneManager.h"

/// This method sets the current scene using a name.
///
/// @param sceneName The name of the current scene.
void SceneManager::setCurrentScene(std::string sceneName)
{
    if (currentScene != nullptr)
        currentScene->onExit();
    currentScene = getScene(sceneName);
    currentScene->Start();
}

/// This method updates the current scene.
void SceneManager::updateScene()
{
    currentScene->Update();
}

/// This method pushes a scene.
///
/// @param *scene A pointer to a new scene.
void SceneManager::addScene(Scene *scene)
{
    m_scenes.push_back(scene);
}

/// A method which gets a scene.
/// @param sceneName The scene's name.
/// @return The needed scene.
Scene *SceneManager::getScene(std::string sceneName)
{
    return (*std::ranges::find_if(m_scenes, [&sceneName](Scene *scene) { return scene->m_sceneName == sceneName; }));
}