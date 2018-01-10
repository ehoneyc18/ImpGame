#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "twowayMultiSprite.h"

class SmartSprite : public TwowayMultiSprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  //virtual ~SmartSprite() { }
  using TwowayMultiSprite::update;
  virtual void update(Uint32 ticks, bool playerAlive);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }

private:
  enum MODE {NORMAL, CHASE};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
