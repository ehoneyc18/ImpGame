#include "boss.h"
#include "explodingSprite.h"

Boss::Boss( const std::string& name) :
  MultiSprite(name),
  collision(false),
  alive(true),
  lives(145),
  initialVelocity(getVelocity()),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet")),
  bullets(bulletName),
  bullets2(bulletName),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0),
  timeSinceLastBullet2(0)
{ setScale(3); }

Boss::Boss(const Boss& s) :
  MultiSprite(s),
  collision(s.collision),
  alive(s.alive),
  lives(s.lives),
  initialVelocity(s.getVelocity()),
  bulletName(s.bulletName),
  bullets(s.bullets),
  bullets2(s.bullets2),
  minBulletSpeed(s.minBulletSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet),
  timeSinceLastBullet2(s.timeSinceLastBullet2)
  { }

  void Boss::shoot(float var) {
    if (timeSinceLastBullet > bulletInterval) {
      Vector2f vel(-120.0, var);
      float x = getX() - 10.0;
      float y = getY() + getScaledHeight()/2;

      bullets.shoot(Vector2f(x,y), vel);
      timeSinceLastBullet = 0;
    }
  }

  void Boss::shootDouble(float var) {
    if (timeSinceLastBullet2 > bulletInterval) {
      Vector2f vel(-120.0, var);
      float x = getX() - 10.0;
      float y = getY() + getScaledHeight()/2;

      bullets2.shoot(Vector2f(x,y), vel);
      timeSinceLastBullet2 = 0;
    }
  }


void Boss::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    timeSinceLastBullet += ticks;
    timeSinceLastBullet2 += ticks;
    bullets.update(ticks);
    bullets2.update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  else {
    advanceFrame(ticks);
    timeSinceLastBullet += ticks;
    timeSinceLastBullet2 += ticks;
    bullets.update(ticks);
    bullets2.update(ticks);
  }

}

void Boss::drawBullets() {
  if (alive) {
    bullets.draw();
    bullets2.draw();
  }
}
