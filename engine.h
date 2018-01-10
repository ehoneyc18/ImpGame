#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "boss.h"
#include "bulletList.h"
#include "collisionStrategy.h"
#include "hud.h"
#include "bullethud.h"
#include "sound.h"

class SmartSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World bg1, bg2, bg3, lights;
  Viewport& viewport;
  SDLSound sound;

  std::vector<Drawable*> sprites;
  int currentSprite;
  Player *player;
  Boss *boss;

  CollisionStrategy *strategy;
  bool collision;

  bool makeVideo;

  const HUD& hud;
  const BulletHUD bullethud;
  bool hudToggle;
  bool bulletHUDToggle;
  bool godMode;
  float timer;
  float timer2;
  bool flag;
  bool flag2;
  bool ghostsAlive;
  bool bossAlive;

  void draw() const;
  void update(Uint32);

  void patternUp();
  void patternDown();
  void doublePattern();
  void spawnGhosts();

  void printScales() const;
  void checkForCollisions();
};
