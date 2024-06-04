//
// Created by c2-horosnyi on 21/01/2020.
//

#ifndef BREAKOUT_C2_HOROSNYI_MASTER_PADDLE_H
#define BREAKOUT_C2_HOROSNYI_MASTER_PADDLE_H

#include "GameObject.h"

class Paddle
{
 public:
  Paddle() = default;
  ~Paddle();
  bool init(ASGE::Renderer* renderer, std::string filename, float max_speed);
  ASGE::Sprite* getSprite();
  void update(bool left, bool right, float time);
  float reportX();
  float reportMovement();
  void reset();

 private:
  GameObject* core = new GameObject;
  float position_x = 0.0f;
  float position_y = 0.0f;
  float time_passed = 0.0f;
  int state = 0;
  float velocity = 0;
  float max_velocity = 50;
  void boundaries();
};

#endif // BREAKOUT_C2_HOROSNYI_MASTER_PADDLE_H
