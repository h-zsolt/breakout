//
// Created by c2-horosnyi on 21/01/2020.
//

#include "Block.h"

Block::~Block()
{
  delete core;
}

bool Block::setType(ASGE::Renderer* renderer, std::string filename, int hits)
{
  if (!core->initialiseSprite(renderer, filename))
  {
    return false;
  }
  core->getSprite()->width(100);
  core->getSprite()->height(40);
  life_count = hits;
  if (life_count > 0)
  {
    visible = true;
  }
  else
  {
    visible = false;
  }
}

bool Block::changeColor(ASGE::Renderer* renderer, std::string filename)
{
  if (!core->initialiseSprite(renderer, filename))
  {
    return false;
  }
  core->getSprite()->width(100);
  core->getSprite()->height(40);
  core->getSprite()->xPos(position_x);
  core->getSprite()->yPos(position_y);
}

int Block::gotHit()
{
  if (!indestructible)
  {
    life_count--;
    if (life_count <= 0)
    {
      visible = false;
    }
  }
  return life_count;
}

float Block::reportX()
{
  return position_x;
}

float Block::reportY()
{
  return position_y;
}

int Block::reportLife()
{
  return life_count;
}

ASGE::Sprite* Block::getSprite()
{
  return core->getSprite();
}

void Block::setLocation(float location_x, float location_y)
{
  position_x = location_x;
  position_y = location_y;
  core->getSprite()->xPos(position_x);
  core->getSprite()->yPos(position_y);
}

bool Block::reportVisibility()
{
  return visible;
}
