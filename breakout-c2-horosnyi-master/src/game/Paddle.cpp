//
// Created by c2-horosnyi on 21/01/2020.
//

#include "Paddle.h"

Paddle::~Paddle()
{
  delete core;
}

ASGE::Sprite* Paddle::getSprite()
{
  return core->getSprite();
}

bool Paddle::init(ASGE::Renderer* renderer,
                  std::string filename,
                  float max_speed)
{
  if (!core->initialiseSprite(renderer, filename))
  {
    return false;
  }
  core->getSprite()->width(120);
  core->getSprite()->height(20);
  core->getSprite()->xPos(580);
  position_x = 580;
  core->getSprite()->yPos(690);
  max_velocity = max_speed;
  return true;
}

void Paddle::update(bool left, bool right, float time)
{
  time_passed = time;
  if (left)
  {
    velocity -= time * 800;
    if (velocity > 0)
    {
      velocity -= time * 1200;
    }
  }
  if (right)
  {
    velocity += time * 800;
    if (velocity < 0)
    {
      velocity += time * 1200;
    }
  }
  if ((left && right) || (!left && !right))
  {
    if (velocity > 0)
    {
      velocity -= time * 1600;
      if (velocity < 0)
      {
        velocity = 0;
      }
    }
    if (velocity < 0)
    {
      velocity += time * 1600;
      if (velocity > 0)
      {
        velocity = 0;
      }
    }
  }
  if (velocity > max_velocity)
  {
    velocity = max_velocity;
  }
  if (velocity < -max_velocity)
  {
    velocity = -max_velocity;
  }
  position_x += velocity * time;
  boundaries();
  core->getSprite()->xPos(position_x);
}

void Paddle::boundaries()
{
  if (position_x > 1160)
  {
    position_x = 1160;
    velocity = 0;
  }
  if (position_x < 0)
  {
    position_x = 0;
    velocity = 0;
  }
}

float Paddle::reportX()
{
  return position_x;
}

float Paddle::reportMovement()
{
  return velocity * time_passed;
}

void Paddle::reset()
{
  state = 0;
  core->getSprite()->width(120);
  core->getSprite()->height(20);
  core->getSprite()->xPos(580);
  position_x = 580;
  core->getSprite()->yPos(690);
  velocity = 0.0f;
}
