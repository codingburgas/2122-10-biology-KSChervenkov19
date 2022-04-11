#pragma once

#include "raylib.h"
#include <string>

enum ANIMATION
{
    ENTER,
    HIDE,
};

class Container
{
  private:
  public:
    Container(const char *textureDest, Vector2 pos);

    Texture2D texture;
    Vector2 m_pos;
    void display();
    void triggerAnimation(ANIMATION animation);
    void unloadTexture();
};