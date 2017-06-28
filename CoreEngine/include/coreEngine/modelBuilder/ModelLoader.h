#ifndef COREENGINE_MODELLOADER
#define COREENGINE_MODELLOADER

#include <conf/GLTypes.h>
#include <coreEngine/conf/MathType.h>
#include <coreEngine/renderObjects/Model.h>
#include <stdlib.h>
#include <vector>

namespace cl {
class ModelLoader {
 public:
  static void load_obj(std::string filename, Model *model);
 };
}
#endif  // COREENGINE_MODELLOADER