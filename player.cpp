#include "player.h"
#include "smartSprite.h"
#include "explodingSprite.h"

Player::Player( const std::string& name) :
  MultiSprite(name),
  observers(),
  collision(false),
  alive(true),
  lives(3),
  initialVelocity(getVelocity()),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet")),
  bullets(bulletName),
  minBulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0)
{ }

Player::Player(const Player& s) :
  MultiSprite(s),
  observers(s.observers),
  collision(s.collision),
  alive(s.alive),
  lives(s.lives),
  initialVelocity(s.getVelocity()),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minBulletSpeed(s.minBulletSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastBullet(s.timeSinceLastBullet)
  { }

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::stop() {
  setVelocity(Vector2f(0,0));
}

void Player::shoot() {
  if (timeSinceLastBullet > bulletInterval) {
    Vector2f vel = getVelocity();
    float x = 0.0;
    float y = getY() + getScaledHeight()/4;
    if (vel[0] >= 0) {
      x = getX() + getScaledWidth();
      vel[0] += minBulletSpeed;
    }
    else if (vel[0] < 0) {
      x = getX() + getScaledWidth();;
      vel[0] = -vel[0];
      vel[0] += minBulletSpeed;
    }
    bullets.shoot(Vector2f(x,y), vel);
    timeSinceLastBullet = 0;
  }
}

void Player::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
}
void Player::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
}
void Player::up()    {
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
}
void Player::down()  {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    timeSinceLastBullet += ticks;
    bullets.update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
      Vector2f pos(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), Gamedata::getInstance().getXmlInt(name+"/startLoc/y"));
      setPosition(pos);
    }
    return;
  }

  advanceFrame(ticks);
  timeSinceLastBullet += ticks;
  bullets.update(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }

  stop();
}

void Player::drawBullets() {
  bullets.draw();
}
