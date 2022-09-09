#pragma once

#include "Scene.h"
#include <iostream>
#include <vector>

class SceneManager
{

  public:
    void setCurrentScene(std::string sceneName);

    void updateScene();

    void addScene(Scene *scene);

  private:
    Scene *getScene(std::string sceneName);

    Scene *currentScene = nullptr;

    std::vector<Scene *> m_scenes;
};