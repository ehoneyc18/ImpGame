#ifndef BULLETLIST__H
#define BULLETLIST__H

#include <list>
#include <vector>
#include "bullet.h"
#include "image.h"

class CollisionStrategy;

class BulletList {
public:
  BulletList(const std::string&);
  BulletList(const BulletList&);
  ~BulletList();
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount() const { return freeList.size(); }

  bool shooting() const { return !bulletList.empty(); }
  bool hit(const Drawable*);

  BulletList& operator=(const BulletList&) = delete;
private:
  std::string name;
  Vector2f myVelocity;
  Image* bulletImg;
  std::list<Bullet> bulletList;
  std::list<Bullet> freeList;
  CollisionStrategy* strategy;
};
#endif
