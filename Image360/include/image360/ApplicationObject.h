#ifndef IMAGE360_APPLICATIONOBJECT_H
#define IMAGE360_APPLICATIONOBJECT_H

/*
* Application object helps abstract out initialization of different
* renderable components. This lets the client initialize different
* components and add to the image360. For e.g. some images may
* use a cube map projection and some may use a equirectangular sphere
* projection. Other examples include existence of controllers for hmd,
* camera reticle (if controller is not present)
*/

namespace cl {
class ApplicationObject {
 public:
  virtual void initialize(Scene &scene) = 0;
  virtual void preDraw() {}
  virtual ~ApplicationObject() {}
};
}

#endif  // IMAGE360_APPLICATIONOBJECT_H