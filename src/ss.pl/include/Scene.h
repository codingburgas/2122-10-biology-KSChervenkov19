#pragma once

#include <string>

class Scene
{
  public:
    Scene(std::string sceneName) : m_sceneName(sceneName)
    {
    }

    bool isSetUp = false;

    std::string m_sceneName;

    // static std::string m_theme;

    virtual void Start() = 0;

    virtual void Update() = 0;

    virtual void onExit() = 0;
};

// std::string Scene::m_theme = "lightTheme";