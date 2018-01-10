#include <iostream>
#include <cmath>
#include "ioMod.h"
#include "bulletList.h"
#include "renderContext.h"
#include "collisionStrategy.h"

BulletList::~BulletList() { delete strategy; }

BulletList::BulletList(const std::string& n) :
  name(n),
  myVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),Gamedata::getInstance().getXmlInt(name+"/speedY")),
  bulletImg(RenderContext::getInstance()->getImage(name)),
  bulletList(),
  freeList(),
  strategy(new PerPixelCollisionStrategy)
{}

BulletList::BulletList(const BulletList& bl) :
  name(bl.name),
  myVelocity(bl.myVelocity),
  bulletImg(bl.bulletImg),
  bulletList(bl.bulletList),
  freeList(bl.freeList),
  strategy(bl.strategy)
{}

void BulletList::shoot(const Vector2f& pos, const Vector2f& objVel) {
  if (freeList.empty()) {
    Bullet b(name, pos, objVel);
    bulletList.push_back(b);
  }
  else {
    Bullet b = freeList.front();
    freeList.pop_front();
    b.reset();
    b.setVelocity(objVel);
    b.setPosition(pos);
    bulletList.push_back(b);
  }
}

bool BulletList::hit(const Drawable* obj) {
  for (auto& bullet : bulletList) {
    if (strategy->execute(bullet, *obj) && !obj->checkExplosion()) {
      bullet.setHit();
      return true;
    }
  }
  return false;
}

void BulletList::draw() const {
  for (const auto& bullet : bulletList ) {
    bullet.draw();
  }
}

void BulletList::update(Uint32 ticks) {
  std::list<Bullet>::iterator it = bulletList.begin();
  while (it != bulletList.end()) {
    it->update(ticks);
    if (it->goneTooFar()) {
      freeList.push_back(*it);
      it = bulletList.erase(it);
    }
    else ++it;
  }
}
