#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&);
  MultiSprite(const MultiSprite&);
  virtual ~MultiSprite();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
    return images[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

  virtual void explode();
  virtual bool explosionDone();
  virtual bool checkExplosion() const;

protected:
  std::vector<Image *> images;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  MultiSprite& operator=(const MultiSprite&);
};
#endif
