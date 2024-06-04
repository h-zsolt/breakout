
#ifndef BREAKOUT_GAMEOBJECT_H
#define BREAKOUT_GAMEOBJECT_H

#include <Engine/Renderer.h>
#include <Engine/Sprite.h>

class GameObject
{
 public:
  GameObject();
  ~GameObject();
  ASGE::Sprite* getSprite();
  void updatelocation(float change_x, float change_y);
  bool initialiseSprite(ASGE::Renderer* renderer, std::string filename);

 private:
  ASGE::Sprite* sprite = nullptr;
  float location_x = 0.0f;
  float location_y = 0.0f;
};

#endif // BREAKOUT_GAMEOBJECT_H
