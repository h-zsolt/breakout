#include <string>

#include "time.h"
#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

#include "game.h"

enum
{
  EASY_BLOCK_LIFE = 1,
  MEDIUM_BLOCK_LIFE = 2,
  HARD_BLOCK_LIFE = 3,
  UNBREAKABLE_BLOCK_LIFE = 9999,
  MAXSPEED = 400
};

/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */

Breakout::Breakout()
{
  game_name = "ASGE Game";
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
Breakout::~Breakout()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));

  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));
  delete player;
  delete moving_object;
  for (int i = 0; i < 200; i++)
  {
    delete targets[i];
  }
  delete life_signs;
  delete math_collision;
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool Breakout::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }

  toggleFPS();
  srand(time(NULL));
  renderer->setClearColour(ASGE::COLOURS::BLACK);

  // input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &Breakout::keyHandler, this);

  mouse_callback_id =
    inputs->addCallbackFnc(ASGE::E_MOUSE_CLICK, &Breakout::clickHandler, this);

  player->init(renderer.get(), "paddleBlu.png", MAXSPEED);
  for (int i = 0; i < 200; i++)
  {
    targets[i] = new Block;
    targets[i]->setType(renderer.get(), "element_red_square.png", 0);
  }
  for (int i = 0; i < 10; i++)
  {
    gems[i] = new Ball;
    gems[i]->init(
      renderer.get(), "element_yellow_diamond_glossy.png", MAXSPEED / 4);
    gems[i]->getSprite()->width(30);
    gems[i]->getSprite()->height(30);
  }
  moving_object->init(renderer.get(), "ballBlue.png", MAXSPEED * 2);
  life_signs = renderer->createRawSprite();

  if (!life_signs->loadTexture("data/images/element_red_diamond_glossy.png"))
  {
    return false;
  }
  life_signs->height(30);
  life_signs->width(30);
  life_signs->yPos(10);
  return true;
}

/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void Breakout::setupResolution()
{
  // how will you calculate the game's resolution?
  // will it scale correctly in full screen? what AR will you use?
  // how will the game be framed in native 16:9 resolutions?
  // here are some arbitrary values for you to adjust as you see fit
  // https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/
  // Scaling_and_MultiResolution_in_2D_Games.php

  // 720p is a pretty modest starting point, consider 1080p
  game_width = 1280;
  game_height = 720;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void Breakout::keyHandler(ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }
  if (in_game)
  {
    if ((key->key == ASGE::KEYS::KEY_A || key->key == ASGE::KEYS::KEY_LEFT) &&
        key->action == ASGE::KEYS::KEY_PRESSED)
    {
      key_controls[0] = true;
    }
    if ((key->key == ASGE::KEYS::KEY_D || key->key == ASGE::KEYS::KEY_RIGHT) &&
        key->action == ASGE::KEYS::KEY_PRESSED)
    {
      key_controls[1] = true;
    }
    if ((key->key == ASGE::KEYS::KEY_A || key->key == ASGE::KEYS::KEY_LEFT) &&
        key->action == ASGE::KEYS::KEY_RELEASED)
    {
      key_controls[0] = false;
    }
    if ((key->key == ASGE::KEYS::KEY_D || key->key == ASGE::KEYS::KEY_RIGHT) &&
        key->action == ASGE::KEYS::KEY_RELEASED)
    {
      key_controls[1] = false;
    }
    if ((key->key == ASGE::KEYS::KEY_SPACE ||
         key->key == ASGE::KEYS::KEY_ENTER) &&
        key->action == ASGE::KEYS::KEY_PRESSED)
    {
      key_controls[2] = true;
    }
    if ((key->key == ASGE::KEYS::KEY_SPACE ||
         key->key == ASGE::KEYS::KEY_ENTER) &&
        key->action == ASGE::KEYS::KEY_RELEASED)
    {
      key_controls[2] = false;
    }
    if (key->key == ASGE::KEYS::KEY_W &&
        key->action == ASGE::KEYS::KEY_RELEASED)
    {
      levelHandler();
    }
  }
  if (in_menu)
  {
    if (key->key == ASGE::KEYS::KEY_ENTER)
    {
      lives = 3;
      in_menu = false;
      in_game = true;
      level_counter = 1;
      levelOne();
    }
  }
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void Breakout::clickHandler(ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());

  double x_pos = click->xpos;
  double y_pos = click->ypos;

  ASGE::DebugPrinter{} << "x_pos: " << x_pos << std::endl;
  ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void Breakout::update(const ASGE::GameTime& game_time)
{
  float dt_sec = game_time.delta.count() / 1000.0;
  // make sure you use delta time in any movement calculations!

  if (in_game)
  {
    player->update(key_controls[0], key_controls[1], dt_sec);
    if (key_controls[2])
    {
      if (moving_object->reportState() == 1)
      {
        moving_object->changeState(0);
      }
      /*switch(player.getState())
      {
          case 1:
              break;
          default:
              break;
      }*/
    }
    if (moving_object->reportState() == 0)
    {
      math_collision->setOld(moving_object->reportX() + 12,
                             moving_object->reportY() + 12);
      moving_object->update(dt_sec);
      math_collision->setNew(moving_object->reportX() + 12,
                             moving_object->reportY() + 12);
      collisionHandler();
    }
    if (moving_object->reportState() == 1)
    {
      moving_object->changeXPosition(moving_object->reportX() +
                                     player->reportMovement());
    }
    for (int i = 0; i < 10; i++)
    {
      if (gem_visibility[i])
      {
        gems[i]->update(dt_sec);
        if (gems[i]->reportY() >= 660 && gems[i]->reportY() < 710 &&
            gems[i]->reportX() > player->reportX() - 30 &&
            gems[i]->reportX() < player->reportX() + 120)
        {
          lives++;
          gem_visibility[i] = false;
        }
        if (gems[i]->reportY() >= 720)
        {
          gem_visibility[i] = false;
        }
      }
    }
  }
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void Breakout::render(const ASGE::GameTime&)
{
  renderer->setFont(0);

  if (in_menu)
  {
    renderer->renderText(welcome_message,
                         (game_width / 2.0f) - (welcome_message.length() * 5),
                         (game_height / 2) - 5,
                         ASGE::COLOURS::WHITE);
  }
  if (in_game)
  {
    renderer->renderSprite(*player->getSprite());
    for (int i = 0; i < 200; i++)
    {
      if (targets[i]->reportVisibility())
      {
        renderer->renderSprite(*targets[i]->getSprite());
      }
    }
    renderer->renderSprite(*moving_object->getSprite());
    for (int i = 0; i < lives; i++)
    {
      life_signs->xPos(1230 - (i * 40));
      renderer->renderSprite(*life_signs);
    }
    for (int i = 0; i < 10; i++)
    {
      if (gem_visibility[i])
      {
        renderer->renderSprite(*gems[i]->getSprite());
      }
    }
  }
}

void Breakout::collisionHandler()
{
  boundingBoxCollision();
  paddleCollision();
  blockCollisionHandler();
}

void Breakout::paddleCollision()
{
  if (moving_object->reportY() > 690 - 25)
  {
    collisionresults = math_collision->roundCollision(
      true, player->reportX(), player->reportX() + 120, 690, 12);
    if (collisionresults != -1 && moving_object->reportYHeading())
    {
      moving_object->changeYHeading();
      segmentHit(collisionresults, player->reportX(), 120, 5);
    }
    collisionresults =
      math_collision->roundCollision(false, 690, 710, player->reportX(), 12);
    if (collisionresults != -1 && moving_object->reportYHeading())
    {
      moving_object->changeYHeading();
      angleChanger(0, 5);
    }
    collisionresults = math_collision->roundCollision(
      false, 690, 710, player->reportX() + 120, 12);
    if (collisionresults != -1 && moving_object->reportYHeading())
    {
      moving_object->changeYHeading();
      angleChanger(5, 5);
    }
  }
}

void Breakout::boundingBoxCollision()
{
  if (moving_object->reportY() <= 0)
  {
    collisionresults = math_collision->roundCollision(true, 0, 1280, 0, 12);
    if (collisionresults != -1 && !moving_object->reportYHeading())
    {
      moving_object->changeYHeading();
      moving_object->changeYPosition(-moving_object->reportY());
    }
  }
  if (moving_object->reportX() <= 0)
  {
    collisionresults = math_collision->roundCollision(false, 0, 720, 0, 12);
    if (collisionresults != -1 && !moving_object->reportXHeading())
    {
      moving_object->changeXHeading();
      moving_object->changeXPosition(-moving_object->reportX());
    }
  }
  if (moving_object->reportX() >= 1280 - 25)
  {
    collisionresults = math_collision->roundCollision(false, 0, 720, 1280, 12);
    if (collisionresults != -1 && moving_object->reportXHeading())
    {
      moving_object->changeXHeading();
      moving_object->changeXPosition(1280 - 25 -
                                     (1280 - 25 - moving_object->reportX()));
    }
  }
  if (moving_object->reportY() >= 720)
  {
    reset();
    lives--;
    if (lives == 0)
    {
      in_menu = true;
      in_game = false;
    }
  }
}

void Breakout::blockCollisionHandler()
{
  bool onlyonce = true;
  bool wincondition = true;
  for (int i = 0; i < 200 && onlyonce; i++)
  {
    if (targets[i]->reportVisibility()/* &&
            math_collision->absoluteFloat(moving_object->reportX() - targets[i]->reportX()) < 150 &&
            math_collision->absoluteFloat(moving_object->reportY() - targets[i]->reportY()) < 80*/)
    {
      wincondition = false;
      collisionresults =
        math_collision->roundCollision(false,
                                       targets[i]->reportY(),
                                       targets[i]->reportY() + 40,
                                       targets[i]->reportX(),
                                       12);
      if (collisionresults != -1 && moving_object->reportXHeading() && onlyonce)
      {
        moving_object->changeXHeading();
        onlyonce = false;
        changeBlockColor(i, targets[i]->gotHit());
        if (targets[i]->reportLife() == 0)
        {
          if (rand() % 5 == 0)
          {
            makeGem(targets[i]->reportX(), collisionresults);
          }
        }
        ASGE::DebugPrinter{} << "LEFT BLOCK HIT AT: " << std::endl;
        ASGE::DebugPrinter{} << "x_pos: " << targets[i]->reportX() << std::endl;
        ASGE::DebugPrinter{} << "y_pos: " << collisionresults << std::endl;
      }
      collisionresults =
        math_collision->roundCollision(false,
                                       targets[i]->reportY(),
                                       targets[i]->reportY() + 40,
                                       targets[i]->reportX() + 100,
                                       12);
      if (collisionresults != -1 && !moving_object->reportXHeading() &&
          onlyonce)
      {
        moving_object->changeXHeading();
        onlyonce = false;
        changeBlockColor(i, targets[i]->gotHit());
        if (targets[i]->reportLife() == 0)
        {
          if (rand() % 5 == 0)
          {
            makeGem(targets[i]->reportX(), collisionresults);
          }
        }
        ASGE::DebugPrinter{} << "RIGHT BLOCK HIT AT: " << std::endl;
        ASGE::DebugPrinter{} << "x_pos: " << targets[i]->reportX() + 100
                             << std::endl;
        ASGE::DebugPrinter{} << "y_pos: " << collisionresults << std::endl;
      }
      collisionresults =
        math_collision->roundCollision(true,
                                       targets[i]->reportX(),
                                       targets[i]->reportX() + 100,
                                       targets[i]->reportY(),
                                       12);
      if (collisionresults != -1 && moving_object->reportYHeading() && onlyonce)
      {
        moving_object->changeYHeading();
        onlyonce = false;
        changeBlockColor(i, targets[i]->gotHit());
        if (targets[i]->reportLife() == 0)
        {
          if (rand() % 5 == 0)
          {
            makeGem(collisionresults, targets[i]->reportY());
          }
        }
        ASGE::DebugPrinter{} << "UPPER BLOCK HIT AT: " << std::endl;
        ASGE::DebugPrinter{} << "x_pos: " << collisionresults << std::endl;
        ASGE::DebugPrinter{} << "y_pos: " << targets[i]->reportY() << std::endl;
      }
      collisionresults =
        math_collision->roundCollision(true,
                                       targets[i]->reportX(),
                                       targets[i]->reportX() + 100,
                                       targets[i]->reportY() + 40,
                                       12);
      if (collisionresults != -1 && !moving_object->reportYHeading() &&
          onlyonce)
      {
        moving_object->changeYHeading();
        onlyonce = false;
        changeBlockColor(i, targets[i]->gotHit());
        if (targets[i]->reportLife() == 0)
        {
          if (rand() % 5 == 0)
          {
            makeGem(collisionresults, targets[i]->reportY());
          }
        }
        ASGE::DebugPrinter{} << "LOWER BLOCK HIT AT: " << std::endl;
        ASGE::DebugPrinter{} << "x_pos: " << collisionresults << std::endl;
        ASGE::DebugPrinter{} << "y_pos: " << targets[i]->reportY() + 40
                             << std::endl;
      }
    }
  }
  if (wincondition)
  {
    levelHandler();
  }
}

void Breakout::levelOne()
{
  for (int i = 0; i < 12; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      targets[i * 8 + j]->setType(
        renderer.get(), "element_red_rectangle.png", 1);
      targets[i * 8 + j]->setLocation(40 + i * 100, 80 + j * 40);
    }
  }
}

void Breakout::angleChanger(int segment_number, int total_segments)
{
  int mid_segment = total_segments / 2;
  int angular_segments = (total_segments - 1) / 2;
  float angle_per_segment = 60.0f / angular_segments;
  ASGE::DebugPrinter{} << "PADDLE HIT: " << std::endl;
  ASGE::DebugPrinter{} << "segment: " << segment_number << std::endl;
  ASGE::DebugPrinter{} << "angle-per-segment: " << angle_per_segment
                       << std::endl;
  if (total_segments % 2 == 0)
  {
    if (segment_number > mid_segment)
    {
      if (!moving_object->reportXHeading())
      {
        moving_object->changeXHeading();
      }
      moving_object->changeAngle(
        angle_per_segment *
        math_collision->absoluteFloat(mid_segment - segment_number));
    }
    if (segment_number == mid_segment || segment_number == mid_segment - 1)
    {
      moving_object->changeAngle(0);
    }
    if (segment_number < mid_segment - 1)
    {
      if (moving_object->reportXHeading())
      {
        moving_object->changeXHeading();
      }
      moving_object->changeAngle(
        angle_per_segment *
        math_collision->absoluteFloat(mid_segment - segment_number - 1));
    }
  }
  else
  {
    if (segment_number == mid_segment)
    {
      moving_object->changeAngle(0);
    }
    if (segment_number > mid_segment)
    {
      if (!moving_object->reportXHeading())
      {
        moving_object->changeXHeading();
      }
      moving_object->changeAngle(
        angle_per_segment *
        math_collision->absoluteFloat(mid_segment - segment_number));
    }
    if (segment_number < mid_segment)
    {
      if (moving_object->reportXHeading())
      {
        moving_object->changeXHeading();
      }
      moving_object->changeAngle(
        angle_per_segment *
        math_collision->absoluteFloat(mid_segment - segment_number));
    }
  }
}

void Breakout::segmentHit(float location,
                          float paddle_location,
                          float paddle_length,
                          int total_segments)
{
  float relative_location =
    math_collision->absoluteFloat(location - paddle_location);
  float segment_length = paddle_length / total_segments;
  int division_result = relative_location / segment_length;
  angleChanger(division_result, total_segments);
}

void Breakout::levelTwo()
{
  for (int i = 0; i < 12; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (j < 4)
      {
        targets[i * 8 + j]->setType(
          renderer.get(), "element_blue_rectangle.png", 2);
      }
      else
      {
        targets[i * 8 + j]->setType(
          renderer.get(), "element_red_rectangle.png", 1);
      }
      targets[i * 8 + j]->setLocation(40 + i * 100, 80 + j * 40);
    }
  }
}

void Breakout::levelThree()
{
  for (int i = 0; i < 12; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if ((i + j) % 2 == 0)
      {
        targets[i * 8 + j]->setType(
          renderer.get(), "element_green_rectangle.png", 3);
        targets[i * 8 + j]->setLocation(40 + i * 100, 80 + j * 40);
      }
      else
      {
        targets[i * 8 + j]->setType(
          renderer.get(), "element_red_rectangle.png", 0);
        targets[i * 8 + j]->setLocation(40 + i * 100, 80 + j * 40);
      }
    }
  }
}

void Breakout::reset()
{
  for (int i = 0; i < 10; i++)
  {
    gem_visibility[i] = false;
  }
  moving_object->reset();
  player->reset();
}

void Breakout::changeBlockColor(int target_number, int type)
{
  switch (type)
  {
    case 1:
      targets[target_number]->changeColor(renderer.get(),
                                          "element_red_rectangle.png");
      break;
    case 2:
      targets[target_number]->changeColor(renderer.get(),
                                          "element_blue_rectangle.png");
      break;
    default:
      break;
  }
}

void Breakout::levelHandler()
{
  level_counter++;
  reset();
  lives = 3;
  switch (level_counter)
  {
    case 2:
      levelTwo();
      break;
    case 3:
      levelThree();
      break;
    default:
      levelThree();
      break;
  }
}

void Breakout::makeGem(float location_x, float location_y)
{
  bool only_once = true;
  for (int i = 0; i < 10; i++)
  {
    if (only_once && !gem_visibility[i])
    {
      only_once = false;
      gems[i]->changeXPosition(location_x);
      gems[i]->changeYPosition(location_y);
      gems[i]->setYHeading(true);
      gem_visibility[i] = true;
      gems[i]->changeState(0);
    }
  }
}
