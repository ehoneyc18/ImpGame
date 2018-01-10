#include <SDL.h>
#include "clock.h"

class HUD {
public:
  static HUD& getInstance(SDL_Renderer* rend);
  void draw() const;
  void vicDraw() const;
  void restart() const;
  HUD(const HUD&) = delete;
  HUD& operator=(const HUD&) = delete;
private:
  SDL_Renderer* renderer;
  Clock &clock;
  SDL_Rect hud;
  HUD(SDL_Renderer* r);
};
