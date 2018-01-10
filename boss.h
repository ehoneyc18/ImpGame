#ifndef BOSS__H
#define BOSS__H

#include <string>
#include <list>
#include <cmath>
#include "bulletList.h"
#include "multisprite.h"

class Boss : public MultiSprite {
public:
  Boss(const std::string&);
  Boss(const Boss&);
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }

  void shoot(float var);
  void shootDouble(float var);
  unsigned int bulletCount() const { return bullets.bulletCount(); }
  unsigned int freeCount() const { return bullets.freeCount(); }
  bool checkHit(const Drawable* d) { return bullets.hit(d); }
  bool checkAlive() const {return alive;}
  void setAlive(const bool set) { alive = set; }
  int getLives() const { return lives; }
  void die() { lives--; }

  Boss& operator=(const Boss&) = delete;

  void drawBullets();

  const ExplodingSprite* getExplosion() const {return explosion;}

private:
  bool collision;
  bool alive;
  int lives;
  Vector2f initialVelocity;
  std::string bulletName;
  BulletList bullets;
  BulletList bullets2;
  float minBulletSpeed;
  float bulletInterval;
  float timeSinceLastBullet;
  float timeSinceLastBullet2;
};
#endif
