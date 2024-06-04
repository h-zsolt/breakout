//
// Created by c2-horosnyi on 21/01/2020.
//

#ifndef BREAKOUT_C2_HOROSNYI_MASTER_BLOCK_H
#define BREAKOUT_C2_HOROSNYI_MASTER_BLOCK_H

#include "GameObject.h"
#include <string.h>

class Block
{
 public:
  Block() = default;
  ~Block();
  bool setType(ASGE::Renderer* renderer, std::string filename, int hits);
  bool changeColor(ASGE::Renderer* renderer, std::string filename);
  void setLocation(float location_x, float location_y);
  bool reportVisibility();
  int gotHit();
  float reportX();
  float reportY();
  int reportLife();
  ASGE::Sprite* getSprite();

 private:
  GameObject* core = new GameObject;
  int life_count = 0;
  bool visible = false;
  bool indestructible = false;
  float position_x = 0.0f;
  float position_y = 0.0f;
};

#endif // BREAKOUT_C2_HOROSNYI_MASTER_BLOCK_H
