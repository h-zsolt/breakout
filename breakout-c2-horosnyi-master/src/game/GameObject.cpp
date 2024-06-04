
#include "GameObject.h"

#include <Engine/DebugPrinter.h>

GameObject::GameObject() {}

GameObject::~GameObject()
{
  delete sprite;
}

bool GameObject::initialiseSprite(ASGE::Renderer* renderer,
                                  std::string filename)
{
  sprite = renderer->createRawSprite();

  if (!sprite->loadTexture("data/images/" + filename))
  {
    return false;
  }
  return true;
}

ASGE::Sprite* GameObject::getSprite()
{
  return sprite;
}

void GameObject::updatelocation(float change_x, float change_y)
{
  location_x += change_x;
  location_y += change_y;
  sprite->xPos(location_x);
  sprite->yPos(location_y);
}
