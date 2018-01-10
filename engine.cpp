#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twowayMultiSprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "smartSprite.h"
#include "explodingSprite.h"

Engine::~Engine() {
  delete player;
  for (auto& i : sprites) {
    delete i;
  }
  delete strategy;
  clock.reset();
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  bg1("bg1", Gamedata::getInstance().getXmlInt("bg1/factor") ),
  bg2("bg2", Gamedata::getInstance().getXmlInt("bg2/factor") ),
  bg3("bg3", Gamedata::getInstance().getXmlInt("bg3/factor") ),
  lights("lights", Gamedata::getInstance().getXmlInt("lights/factor") ),
  viewport( Viewport::getInstance() ),
  sound(),
  sprites(),
  currentSprite(0),
  player( new Player("Bat") ),
  boss ( new Boss("Boss") ),
  strategy( new PerPixelCollisionStrategy ),
  collision(false),
  makeVideo( false ),
  hud( HUD::getInstance(rc->getRenderer())),
  bullethud(player),
  hudToggle(false),
  bulletHUDToggle(false),
  godMode(false),
  timer(-225.0),
  timer2(225.0),
  flag(true),
  flag2(false),
  ghostsAlive(false),
  bossAlive(true)
{
  int n = Gamedata::getInstance().getXmlInt("numberOfGhosts");
  sprites.reserve(n+1);

  sprites.push_back(boss);

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  bg3.draw();
  lights.draw();
  bg2.draw();
  bg1.draw();

  for (auto& i : sprites) {
    i->draw();
  }

  player->draw();
  player->drawBullets();
  boss->drawBullets();
  viewport.draw();
  if (clock.getSeconds() < 5 || hudToggle)
    hud.draw();
  if (bulletHUDToggle)
    bullethud.draw();

  std::stringstream strm;
  strm << "Lives: " << player->getLives();

  SDL_Color c = {0, 0, 255, 255};

  IOmod::getInstance().writeText(strm.str(), 550, 450, c);

  if (sprites.size() == 0) {
    hud.vicDraw();
    clock.pause();
  }
  if ((player->getLives() <= 0)) {
    hud.restart();
    clock.pause();
  }

  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  collision = false;

  std::vector<Drawable*>::iterator d = sprites.begin();

  while (d != sprites.end()) {
    if ( player->checkHit(*d)) {
      if ( dynamic_cast<Boss*>(*d) == NULL ) {
        (*d)->explode();
        ++d;
      }
      else {
        if (boss->getLives() > 0) {
          boss->die();
          ++d;
        }
        else {
          boss->setAlive(false);
          bossAlive = false;
          boss->explode();
          sound[1];
          ++d;
        }
      }
    }
    else ++d;
  }

  for ( auto d : sprites ) {
    if ( strategy->execute(*player, *d) && !(d->checkExplosion()) && !(godMode)) {
      collision = true;
    }
  }

  if (bossAlive) {
    if (boss->checkHit(player) && !(godMode))
      collision = true;
  }

  if ( collision ) {
    player->collided();
    player->setAlive(false);
    player->explode();
  }
  else {
    player->missed();
    collision = false;
  }
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();

  if (sprites.size() == 1)
    ghostsAlive = false;

  if (boss->getLives() <= 0)
    bossAlive = false;

  if (!player->checkAlive()) {
    if (player->explosionDone()) {
      godMode = true;
      player->die();
      player->setAlive(true);
      godMode = false;
    }
  }

  player->update(ticks);

  for (auto& i : sprites) {
    if(dynamic_cast<SmartSprite*>(i))
      dynamic_cast<SmartSprite*>(i)->update(ticks, player->checkAlive());
    else i->update(ticks);
  }
  bg3.update();
  lights.update();
  bg2.update();
  bg1.update();
  std::vector<Drawable*>::iterator d = sprites.begin();
  SmartSprite* doa = nullptr;
  Boss* doaB = nullptr;
  while (d != sprites.end()) {
    if ((*d)->checkExplosion()) {
      if (dynamic_cast<SmartSprite*>(*d) == NULL) {
        if (dynamic_cast<Boss*>(*d) != NULL) {
          if (boss->getLives() <= 0 && (boss->getExplosion()->chunkCount()) <= 10){
            doaB = dynamic_cast<Boss*>(*d);
            delete doaB;
            d = sprites.erase(d);
          }
          else ++d;
        }
        else ++d;
      }
      else {
        if ((dynamic_cast<SmartSprite*>(*d)->getExplosion()->chunkCount()) <= 3) {
        doa = dynamic_cast<SmartSprite*>(*d);
        player->detach(doa);
        delete doa;
        d = sprites.erase(d);
        }
        else ++d;
      }
    }
    else ++d;
  }

  if (sprites.size() == 0)
    hudToggle = true;

  if (clock.getSeconds() == 4)
    sound[2];
  else if (clock.getSeconds() >= 5 && clock.getSeconds() <= 12 && bossAlive)
  {
    patternDown();
  }
  else if (clock.getSeconds() >= 14 && clock.getSeconds() <= 21 && bossAlive)
  {
    patternUp();
    timer = -225.0;
  }
  else if (clock.getSeconds() >= 23 && clock.getSeconds() <= 30 && bossAlive)
  {
    patternDown();
    timer2 = 225.0;
  }
  else if (clock.getSeconds() >= 32 && clock.getSeconds() <= 49 && bossAlive)
  {
    patternUp();
    timer = -225.0;
  }
  else if (clock.getSeconds() == 50 && bossAlive)
  {
    timer = -225.0;
    timer2 = 225.0;
  }
  else if (clock.getSeconds() >= 51 && clock.getSeconds() <= 57 && bossAlive)
  {
    doublePattern();
  }
  else if (clock.getSeconds() == 59 && bossAlive) {
    sound[2];
    if (!ghostsAlive)
      spawnGhosts();

    timer = -225.0;
    timer2 = 225.0;
  }
  else if (clock.getSeconds() >= 64 && clock.getSeconds() <= 71 && bossAlive)
  {
    doublePattern();
  }
  else if (clock.getSeconds() == 73 && bossAlive)
  {
    sound[2];
    if (!ghostsAlive)
      spawnGhosts();
  }
  else if (clock.getSeconds() >= 78 && clock.getSeconds() <= 85 && bossAlive)
  {
    patternDown();
    timer2 = 225.0;
  }
  else if (clock.getSeconds() >= 86 && clock.getSeconds() <= 93 && bossAlive)
  {
    patternUp();
    timer = -225.0;
  }
  else if (clock.getSeconds() == 94 && bossAlive)
  {
    timer = -225.0;
    timer2 = 225.0;
  }
  else if (clock.getSeconds() >= 95 && clock.getSeconds() <= 102 && bossAlive)
  {
    doublePattern();
  }
  else if (clock.getSeconds() >= 104)
    clock.reset();


  viewport.update(); // always update viewport last
}

bool Engine::play() {
  SDL_Event event;
  //SDLSound sound;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_R]) {
          clock.reset();
          return true;
        }
        if ( keystate[SDL_SCANCODE_G]) {
          if (godMode)
            godMode = false;
          else godMode = true;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDL_SCANCODE_F1]) {
          if (bulletHUDToggle)
            bulletHUDToggle = false;
          if (hudToggle)
            hudToggle = false;
          else
            hudToggle = true;
        }
        if (keystate[SDL_SCANCODE_F2]) {
          if (hudToggle)
            hudToggle = false;

          if (bulletHUDToggle) {
            bulletHUDToggle = false;
          }
          else {
            bulletHUDToggle = true;
          }
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_SPACE] && player->checkAlive()) {
        player->shoot();
        sound[0];
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}

void Engine::patternDown() {
  if ( timer == -225.0) {
    flag = true;
  }
  else if (timer == 225.0) {
    flag = false;
  }

  if (flag) {
    boss->shoot(timer);
    timer += 3.0;
  }
  else if (!flag) {
    boss->shoot(timer);
    timer -= 3.0;
  }
}

void Engine::patternUp() {
  if ( timer2 == -225.0) {
    flag2 = true;
  }
  else if (timer2 == 225.0) {
    flag2 = false;
  }

  if (flag2) {
    boss->shoot(timer2);
    timer2 += 3.0;
  }
  else if (!flag2) {
    boss->shoot(timer2);
    timer2 -= 3.0;
  }
}

void Engine::doublePattern(){
  if ( timer == -225.0) {
    flag = true;
  }
  else if (timer == 225.0) {
    flag = false;
  }

  if ( timer2 == -225.0) {
    flag2 = true;
  }
  else if (timer2 == 225.0) {
    flag2 = false;
  }

  if (flag) {
    boss->shoot(timer);
    timer += 3.0;
  }
  else if (!flag) {
    boss->shoot(timer);
    timer -= 3.0;
  }

  if (flag2) {
    boss->shootDouble(timer2);
    timer2 += 3.0;
  }
  else if (!flag2) {
    boss->shootDouble(timer2);
    timer2 -= 3.0;
  }
}

void Engine::spawnGhosts() {
  int n = Gamedata::getInstance().getXmlInt("numberOfGhosts");
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 1; i < n+1; ++i) {
    sprites.push_back( new SmartSprite("Ghost", pos, w, h) );
    player->attach(dynamic_cast<SmartSprite*>(sprites[i]));
  }

  ghostsAlive = true;
}
