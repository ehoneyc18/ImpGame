#include "hud.h"
#include "gamedata.h"
#include "ioMod.h"
#include <SDL.h>
#include <iostream>
#include <sstream>

HUD::HUD(SDL_Renderer* r) :
  renderer(r),
  clock(Clock::getInstance()),
  hud({Gamedata::getInstance().getXmlInt("hud/originx"), Gamedata::getInstance().getXmlInt("hud/originy"),
    Gamedata::getInstance().getXmlInt("hud/width"), Gamedata::getInstance().getXmlInt("hud/height")})
{}

HUD& HUD::getInstance(SDL_Renderer* rend) {
  static HUD instance(rend);
  return instance;
}

void HUD::draw() const {
  std::stringstream strm;
  strm << "FPS: " << clock.getFps();

  SDL_Color c = {0, 0, 255, 255};

  IOmod::getInstance().writeText(strm.str(), 30, 10, c);

  strm.str(std::string());
  c = {0, 255, 0, 255};

  strm << "ehoneyc";

  IOmod::getInstance().writeText(strm.str(), 30, 430, c);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &hud);

  std::string s1 = Gamedata::getInstance().getXmlStr("hud/line1");

  std::stringstream stream;
  stream << s1;

  SDL_Color c2 = {255, 0, 255, 255};
  IOmod::getInstance().writeText(stream.str(), 431, 11, c2);

  std::string s2 = Gamedata::getInstance().getXmlStr("hud/line2");

  stream.str(std::string());
  stream << s2;

  IOmod::getInstance().writeText(stream.str(), 431, 41, c2);

  std::string s3 = Gamedata::getInstance().getXmlStr("hud/line3");

  stream.str(std::string());
  stream << s3;

  IOmod::getInstance().writeText(stream.str(), 431, 71, c2);

  std::string s4 = Gamedata::getInstance().getXmlStr("hud/line4");

  stream.str(std::string());
  stream << s4;

  IOmod::getInstance().writeText(stream.str(), 431, 101, c2);
}

void HUD::vicDraw() const {
  SDL_Rect victory = {293, 215, 90, 40};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &victory);

  std::string s1 = "You win!";

  std::stringstream stream;
  stream << s1;

  SDL_Color c2 = {255, 0, 255, 255};
  IOmod::getInstance().writeText(stream.str(), 310, 225, c2);
}

void HUD::restart() const {
  SDL_Rect victory = {265, 215, 180, 40};

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &victory);

  std::string s1 = "Game over! R to restart";

  std::stringstream stream;
  stream << s1;

  SDL_Color c2 = {255, 0, 255, 255};
  IOmod::getInstance().writeText(stream.str(), 272, 225, c2);
}
