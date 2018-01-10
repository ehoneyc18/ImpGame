#include "gamedata.h"
#include "renderContext.h"
#include "player.h"

class BulletHUD {
public:
  BulletHUD(const Player* p);
  void draw() const;
  BulletHUD(const BulletHUD&) = delete;
  BulletHUD& operator=(const BulletHUD&) = delete;
private:
  const Player* player;
  SDL_Renderer* renderer;
  SDL_Rect bullethud;
};
