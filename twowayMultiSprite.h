#ifndef TWOWAY__MULTISPRITE__H
#define TWOWAY__MULTISPRITE__H

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include "drawable.h"

class ExplodingSprite;

class TwowayMultiSprite : public Drawable {
public:
  TwowayMultiSprite(const std::string&);
  TwowayMultiSprite(const TwowayMultiSprite&);
  TwowayMultiSprite& operator=(const TwowayMultiSprite&);
  virtual ~TwowayMultiSprite();

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

  void setImagesLeft() { images = imagesLeft; }
  void setImagesRight() { images = imagesRight; }

  virtual void explode();
  bool checkExplosion() const;
  const ExplodingSprite* getExplosion() const {return explosion;}

protected:
  std::vector<Image *> imagesRight;
  std::vector<Image *> imagesLeft;
  std::vector<Image *> images;
  ExplodingSprite* explosion;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  Vector2f makeVelocity(int, int) const;
};
#endif
