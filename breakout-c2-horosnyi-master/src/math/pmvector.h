//
// Created by c2-horosnyi on 04/12/2019.
//

#ifndef PONG_C2_HOROSNYI_PMVECTOR_H
#define PONG_C2_HOROSNYI_PMVECTOR_H

class pmvector
{
 public:
  void setvalue(float value_x,
                float value_y,
                float value_ghost_x,
                float value_ghost_y);
  float lineCollision(bool slicer_axis_x,
                      float start_point,
                      float end_point,
                      float slicer);
  float roundCollision(bool slicer_axis_x,
                       float start_point,
                       float end_point,
                       float slicer,
                       float radius);
  bool hasHappened(float old_value, float new_value, float check_this);
  void setOld(float value_x, float value_y);
  void setNew(float value_x, float value_y);
  float absoluteFloat(float number);
  // void debug(float debug_x, float debug_y, float debug_ox, float debug_oy,
  // float debug_s, float debug_m, float debug_a);
 private:
  void
  roundCorners(float ratio, float radius, bool double_down, bool traversal_x);
  bool makeSureItExists(float centerX,
                        float centerY,
                        float pointX,
                        float pointY,
                        float max_distance);

  float old_x = 0.0f;
  float new_x = 0.0f;
  float old_y = 0.0f;
  float new_y = 0.0f;
};

#endif // PONG_C2_HOROSNYI_PMVECTOR_H
