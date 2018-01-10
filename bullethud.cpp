#include <sstream>
#include "bullethud.h"
#include "gamedata.h"
#include "renderContext.h"
#include "ioMod.h"

BulletHUD::BulletHUD(const Player* p) :
  player(p),
  renderer(RenderContext::getInstance()->getRenderer()),
  bullethud( {Gamedata::getInstance().getXmlInt("bullethud/originx"), Gamedata::getInstance().getXmlInt("bullethud/originy"),
    Gamedata::getInstance().getXmlInt("bullethud/width"), Gamedata::getInstance().getXmlInt("bullethud/height")})
{}

void BulletHUD::draw() const {
  SDL_SetRenderDrawColor(renderer, 0, 150, 255, 50);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderFillRect(renderer, &bullethud);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

  SDL_Color c = {255, 0, 255, 255};

  std::stringstream strm;
  strm << "Bullet list: " << player->bulletCount();
  IOmod::getInstance().writeText(strm.str(), 431, 11, c);

  strm.str(std::string());
  strm << "Free list: " << player->freeCount();
  IOmod::getInstance().writeText(strm.str(), 431, 51, c);
}
