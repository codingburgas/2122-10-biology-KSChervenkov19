#include "elements.h"

Container::Container(const char* textureDest, Vector2 pos) : texture(LoadTexture(textureDest)), 
	m_pos(pos)
{

}

void Container::display()
{
	DrawTexture(texture, m_pos.x, m_pos.y, WHITE);
}

void Container::unloadTexture()
{
	UnloadTexture(texture);
}