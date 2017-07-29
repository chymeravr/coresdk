#ifndef IMAGE360_STEREOOBJECT_H
#define IMAGE360_STEREOOBJECT_H

/*
* Use StereoObject to abstract out rendering components that have different
* properties based for vieweing from different eyes. For e.g.
* Stereo Cube Map, Stereo Sphere, Stereo any rendering geometry
*/
namespace cl {
class StereoObject {
 public:
  virtual void preDrawLeft() = 0;
  virtual void preDrawRight() = 0;
  virtual ~StereoObject() {}
};
}

#endif  // IMAGE360_STEREOOBJECT_H