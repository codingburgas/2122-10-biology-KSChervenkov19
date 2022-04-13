#pragma once

#include <array>
#include <format>
#include <string>

class Scene
{
  public:
    Scene(std::string sceneName) : m_sceneName(sceneName)
    {
    }

    bool isSetUp = false;

    std::string m_sceneName;

    inline static enum class ThemeTypes { LightTheme, DarkTheme };

    inline static ThemeTypes currentTheme = ThemeTypes::LightTheme;

    inline static const std::array<std::string, 2> themePaths = {"lightTheme", "darkTheme"};

    virtual void Start() = 0;

    virtual void Update() = 0;

    virtual void onExit() = 0;
};