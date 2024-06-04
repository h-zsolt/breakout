//
// Created by c2-horosnyi on 11/02/2020.
//

#ifndef BREAKOUT_C2_HOROSNYI_MASTER_GEMS_H
#define BREAKOUT_C2_HOROSNYI_MASTER_GEMS_H

#include "../math/pmvector.h"
#include "GameObject.h"

class Gems
{
 public:
  Gems() = default;
  ~Gems();
  bool init(ASGE::Renderer* renderer, std::string filename, float max_speed);
  ASGE::Sprite* getSprite();
  void update(float time);
  void increaseSpeed(float amount, float percentage);
  void changeXHeading();
  void changeYHeading();
  bool reportXHeading();
  bool reportYHeading();
  void changeState(int state_goal);
  int reportState();
  float reportRadius();
  void changeAngle(float change_to);
  void changeXPosition(float target);
  void changeYPosition(float target);
  float reportX();
  float reportY();
  void reset();

 private:
  GameObject* core = new GameObject;
  pmvector* math_helper = new pmvector;
  float position_x = 0.0f;
  float position_y = 0.0f;
  bool heading_x = false;
  bool heading_y = false;
  int state = 1;
  float radius = 12.0f;
  float size = 25.0f;
  float velocity = 400;
  float max_velocity = 800;
  float angle = 0.0f;
};

#endif // BREAKOUT_C2_HOROSNYI_MASTER_GEMS_H
