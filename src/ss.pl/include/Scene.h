#pragma once

#include <array>
#include <format>
#include <raylib.h>
#include <string>

class Scene
{
  public:
    Scene(std::string sceneName) : m_sceneName(sceneName)
    {
    }

    bool isSetUp = false;

    std::string m_sceneName;

    enum class ThemeTypes { LightTheme, DarkTheme };

    inline static ThemeTypes currentTheme = ThemeTypes::LightTheme;

    inline static const std::array<std::string, 2> themePaths = {"lightTheme", "darkTheme"};

    inline static const std::array<Color, 2> backgroundColors{{{235, 239, 247, 255}, {4, 12, 32, 255}}};

    virtual void Start() = 0;

    virtual void Update() = 0;

    virtual void onExit() = 0;

    virtual void loadAssets() = 0;

    virtual void deleteAssets() = 0;
};