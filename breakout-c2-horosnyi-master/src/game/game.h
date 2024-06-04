#pragma once
#include "../math/pmvector.h"
#include "Ball.h"
#include "Block.h"
#include "Paddle.h"
#include "Vector2.h"
#include <Engine/OGLGame.h>
#include <string>

/**
 *  An OpenGL Game based on ASGE.
 */

class Breakout : public ASGE::OGLGame
{
 public:
  Breakout();
  ~Breakout() final;
  bool init() override;

 private:
  void keyHandler(ASGE::SharedEventData data);
  void clickHandler(ASGE::SharedEventData data);
  void setupResolution();
  void collisionHandler();
  void levelOne();
  void levelTwo();
  void levelThree();
  void reset();
  void changeBlockColor(int target_number, int type);
  void update(const ASGE::GameTime&) override;
  void render(const ASGE::GameTime&) override;
  void angleChanger(int segment_number, int total_segments);
  void segmentHit(float location,
                  float paddle_location,
                  float paddle_length,
                  int total_segments);
  void levelHandler();
  void makeGem(float, float);

  std::string welcome_message = "Press Enter to start the game";
  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */
  bool in_menu = true;
  bool in_game = false;
  int level_counter = 0;
  bool key_controls[3] = { false };
  Paddle* player = new Paddle;
  Block* targets[200];
  Ball* moving_object = new Ball;
  Ball* gems[10];
  bool gem_visibility[10] = { false };
  pmvector* math_collision = new pmvector;
  ASGE::Sprite* life_signs = nullptr;
  int lives = 3;
  void blockCollisionHandler();
  float collisionresults = -1; // created here for performance
  void boundingBoxCollision();

  void paddleCollision();
};