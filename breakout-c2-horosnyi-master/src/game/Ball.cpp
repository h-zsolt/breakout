//
// Created by c2-horosnyi on 21/01/2020.
//

#include "Ball.h"

Ball::~Ball() {}

bool Ball::init(ASGE::Renderer* renderer, std::string filename, float max_speed)
{
  if (!core->initialiseSprite(renderer, filename))
  {
    return false;
  }
  core->getSprite()->width(25);
  core->getSprite()->height(25);
  core->getSprite()->xPos(628);
  core->getSprite()->yPos(665);
  position_y = 665;
  position_x = 628;
  max_velocity = max_speed;
  return true;
}

ASGE::Sprite* Ball::getSprite()
{
  return core->getSprite();
}

void Ball::update(float time)
{
  if (state == 0)
  {
    if (heading_x)
    {
      position_x += time * velocity *
                    (1 - math_helper->absoluteFloat(((angle - 90.0f)) / 90.0f));
    }
    else
    {
      position_x -= time * velocity *
                    (1 - math_helper->absoluteFloat(((angle - 90.0f)) / 90.0f));
    }
    if (heading_y)
    {
      position_y += 0.75f * time * velocity;
    }
    else
    {
      position_y -= 0.75f * time * velocity;
    }
  }
  core->getSprite()->yPos(position_y);
  core->getSprite()->xPos(position_x);
}

void Ball::increaseSpeed(float amount, float percentage)
{
  velocity += amount;
  velocity *= percentage;
  if (velocity > max_velocity)
  {
    velocity = max_velocity;
  }
}

void Ball::changeXHeading()
{
  heading_x = !heading_x;
}

void Ball::changeYHeading()
{
  heading_y = !heading_y;
}

void Ball::changeState(int state_goal)
{
  state = state_goal;
}

float Ball::reportX()
{
  return position_x;
}

float Ball::reportY()
{
  return position_y;
}

bool Ball::reportXHeading()
{
  return heading_x;
}

bool Ball::reportYHeading()
{
  return heading_y;
}

void Ball::changeXPosition(float target)
{
  position_x = target;
  core->getSprite()->xPos(position_x);
}

void Ball::changeYPosition(float target)
{
  position_y = target;
  core->getSprite()->yPos(position_y);
}

void Ball::changeAngle(float change_to)
{
  angle = change_to;
}

int Ball::reportState()
{
  return state;
}

void Ball::reset()
{
  state = 1;
  core->getSprite()->width(25);
  core->getSprite()->height(25);
  core->getSprite()->xPos(628);
  core->getSprite()->yPos(665);
  position_y = 665;
  position_x = 628;
  angle = 0.0f;
}

void Ball::setXHeading(bool target_state)
{
  heading_x = target_state;
}

void Ball::setYHeading(bool target_state)
{
  heading_y = target_state;
}
