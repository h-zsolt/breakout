//
// Created by c2-horosnyi on 04/12/2019.
//

#include "pmvector.h"

float pmvector::lineCollision(bool slicer_axis_x,
                              float start_point,
                              float end_point,
                              float slicer)
{
  if (slicer_axis_x)
  {
    if (new_y - old_y != 0)
    {
      float ratio = (new_x - old_x) / (new_y - old_y);
      float arbitrary = old_y * ratio - old_x;
      float meeting = ratio * slicer - arbitrary;
      if (meeting >= start_point && meeting <= end_point &&
          hasHappened(old_y, new_y, slicer))
      {
        return meeting;
      }
    }
  }
  else
  {
    if (new_x - old_x != 0)
    {
      float ratio = (new_y - old_y) / (new_x - old_x);
      float arbitrary = old_x * ratio - old_y;
      float meeting = ratio * slicer - arbitrary;
      if (meeting >= start_point && meeting <= end_point &&
          hasHappened(old_x, new_x, slicer))
      {
        return meeting;
      }
    }
  }
  return -1;
}

float pmvector::roundCollision(bool slicer_axis_x,
                               float start_point,
                               float end_point,
                               float slicer,
                               float radius)
{
  if (slicer_axis_x)
  {
    if (new_y - old_y != 0)
    {
      float ratio = (new_x - old_x) / (new_y - old_y);
      float arbitrary = 0;
      float meeting = 0;
      // path collision
      roundCorners(ratio, radius, false, !slicer_axis_x);
      arbitrary = old_y * ratio - old_x;
      meeting = ratio * slicer - arbitrary;
      if (meeting >= start_point && meeting <= end_point &&
          hasHappened(old_y, new_y, slicer) &&
          makeSureItExists(new_x, new_y, meeting, arbitrary, radius))
      {
        return meeting;
      }

      roundCorners(ratio, radius, true, !slicer_axis_x);
      arbitrary = old_y * ratio - old_x;
      meeting = ratio * slicer - arbitrary;
      if (meeting >= start_point && meeting <= end_point &&
          hasHappened(old_y, new_y, slicer) &&
          makeSureItExists(new_x, new_y, meeting, arbitrary, radius))
      {
        return meeting;
      }
      // reset to centerpoint
      roundCorners(ratio, radius, false, !slicer_axis_x);
      // check ball collision
      arbitrary = slicer;
      float intersection = 1 - (absoluteFloat(arbitrary - new_y) / radius);
      meeting = new_x + (intersection * radius);
      if (meeting >= start_point && meeting <= end_point &&
          makeSureItExists(new_x, new_y, meeting, arbitrary, radius))
      {
        return meeting;
      }
      meeting = new_x - (intersection * radius);
      if (meeting >= start_point && meeting <= end_point &&
          makeSureItExists(new_x, new_y, meeting, arbitrary, radius))
      {
        return meeting;
      }
    }
  }
  else
  {
    if (new_x - old_x != 0)
    {
      float ratio = (new_y - old_y) / (new_x - old_x);
      float arbitrary = 0;
      float meeting = 0;

      roundCorners(ratio, radius, false, !slicer_axis_x);
      arbitrary = old_x * ratio - old_y;
      meeting = ratio * slicer - arbitrary;
      if (meeting >= start_point && meeting <= end_point &&
          hasHappened(old_x, new_x, slicer) &&
          makeSureItExists(new_x, new_y, arbitrary, meeting, radius))
      {
        return meeting;
      }

      roundCorners(ratio, radius, true, !slicer_axis_x);
      arbitrary = old_x * ratio - old_y;
      meeting = ratio * slicer - arbitrary;
      if (meeting >= start_point && meeting <= end_point &&
          hasHappened(old_x, new_x, slicer) &&
          makeSureItExists(new_x, new_y, arbitrary, meeting, radius))
      {
        return meeting;
      }

      roundCorners(ratio, radius, false, !slicer_axis_x);

      arbitrary = slicer;
      float intersection = 1 - (absoluteFloat(arbitrary - new_x) / radius);
      meeting = new_y + (intersection * radius);
      if (meeting >= start_point && meeting <= end_point &&
          makeSureItExists(new_x, new_y, arbitrary, meeting, radius))
      {
        return meeting;
      }
      meeting = new_y - (intersection * radius);
      if (meeting >= start_point && meeting <= end_point &&
          makeSureItExists(new_x, new_y, arbitrary, meeting, radius))
      {
        return meeting;
      }
    }
  }
  return -1;
}

void pmvector::setvalue(float value_x,
                        float value_y,
                        float value_ghost_x,
                        float value_ghost_y)
{
  new_x = value_x;
  new_y = value_y;
  old_x = value_ghost_x;
  old_y = value_ghost_y;
}

bool pmvector::hasHappened(float old_value, float new_value, float check_this)
{
  if (old_value < new_value)
  {
    if (check_this >= old_value && check_this <= new_value)
    {
      return true;
    }
  }
  else
  {
    if (check_this <= old_value && check_this >= new_value)
    {
      return true;
    }
  }
  return false;
}

void pmvector::roundCorners(float ratio,
                            float radius,
                            bool double_down,
                            bool traversal_x)
{
  if (double_down)
  {
    if (traversal_x)
    {
      old_x -= radius * ratio * 2;
      old_y -= radius * (1 - ratio) * 2;
      new_x -= radius * ratio * 2;
      new_y -= radius * (1 - ratio) * 2;
    }
    else
    {
      old_y -= radius * ratio * 2;
      old_x -= radius * (1 - ratio) * 2;
      new_y -= radius * ratio * 2;
      new_x -= radius * (1 - ratio) * 2;
    }
  }
  else
  {
    if (traversal_x) // X is more predominant
    {
      old_x += radius * ratio;
      old_y += radius * (1 - ratio);
      new_x += radius * ratio;
      new_y += radius * (1 - ratio);
    }
    else
    {
      old_y += radius * ratio;
      old_x += radius * (1 - ratio);
      new_y += radius * ratio;
      new_x += radius * (1 - ratio);
    }
  }
}

void pmvector::setOld(float value_x, float value_y)
{
  old_x = value_x;
  old_y = value_y;
}

void pmvector::setNew(float value_x, float value_y)
{
  new_x = value_x;
  new_y = value_y;
}

float pmvector::absoluteFloat(float number)
{
  if (number < 0)
  {
    number *= -1;
  }
  return number;
}

bool pmvector::makeSureItExists(
  float centerX, float centerY, float pointX, float pointY, float max_distance)
{
  if (absoluteFloat(centerX - pointX) + absoluteFloat(centerY - pointY) >
      max_distance)
  {
    return false;
  }
  else
  {
    return true;
  }
}

/*void pmvector::debug(float debug_x, float debug_y, float debug_ox, float
debug_oy, float debug_s, float debug_m, float debug_a)
{
    //nothing to see here
}*/
