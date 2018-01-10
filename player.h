#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <list>
#include <cmath>
#include "bulletList.h"
#include "multisprite.h"

class SmartSprite;

class Player : public MultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);
  void attach( SmartSprite* o ) { observers.push_back(o); }
  void detach( SmartSprite* o );

  void collided() { collision = true; }
  void missed() { collision = false; }

  void shoot();
  unsigned int bulletCount() const { return bullets.bulletCount(); }
  unsigned int freeCount() const { return bullets.freeCount(); }
  bool checkHit(const Drawable* d) { return bullets.hit(d); }
  bool checkAlive() const {return alive;}
  void setAlive(const bool set) { alive = set; }
  int getLives() const { return lives; }
  void die() { lives--; }

  Player& operator=(const Player&) = delete;

  void right();
  void left();
  void up();
  void down();
  void stop();

  void drawBullets();

protected:
  std::list<SmartSprite*> observers;

private:
  bool collision;
  bool alive;
  int lives;
  Vector2f initialVelocity;
  std::string bulletName;
  BulletList bullets;
  float minBulletSpeed;
  float bulletInterval;
  float timeSinceLastBullet;
};
#endif
