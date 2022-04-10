#pragma once

#include "SceneManager.h"
#include <raylib.h>

class MainMenu : public Scene
{
  private:
    Texture2D buttonTexture;
    Texture2D logoTexture;

  public:
    MainMenu(std::string sceneName, SceneManager &sceneManager);

    void Start();

    void Update();

    void onExit();

    SceneManager &m_sceneManager;

    Font font;
};