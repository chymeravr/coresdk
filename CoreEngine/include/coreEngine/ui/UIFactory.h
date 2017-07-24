#ifndef COREENGINE_UIFACTORY_H
#define COREENGINE_UIFACTORY_H

#include <coreEngine/factory/IUniformFadeColorFactory.h>
#include <coreEngine/ui/CharacterElement.h>
#include <coreEngine/ui/FadePlanarBackground.h>
#include <coreEngine/ui/FontStore.h>
#include <coreEngine/ui/ITextMaterialFactory.h>
#include <coreEngine/ui/PlanarBackground.h>
#include <coreEngine/ui/Reticle.h>
#include <coreEngine/ui/TextElement.h>

namespace cl {
class UIFactory {
 public:
  UIFactory(ILoggerFactory *loggerFactory,
            std::unique_ptr<IModelFactory> modelFactory,
            std::unique_ptr<IUniformColorFactory> uniformColorFactory,
            std::unique_ptr<IUniformFadeColorFactory> uniformFadeColorFactory,
            std::unique_ptr<ITransformTreeFactory> transformTreeFactory,
            std::unique_ptr<ITextMaterialFactory> textMaterialFactory) {
    assert(loggerFactory != nullptr);
    assert(modelFactory != nullptr);
    assert(uniformColorFactory != nullptr);
    assert(uniformFadeColorFactory != nullptr);
    assert(transformTreeFactory != nullptr);
    assert(textMaterialFactory != nullptr);
    this->loggerFactory = loggerFactory;
    this->modelFactory = std::move(modelFactory);
    this->uniformColorFactory = std::move(uniformColorFactory);
    this->uniformFadeColorFactory = std::move(uniformFadeColorFactory);
    this->transformTreeFactory = std::move(transformTreeFactory);
    this->textMaterialFactory = std::move(textMaterialFactory);
  }
  std::unique_ptr<PlanarBackground> createPlanarBackground(
      std::string id, Scene *scene, CL_Vec4 color, CL_Vec3 &localPosition,
      CL_Vec3 &localRotation, float width, float height) {
    return std::unique_ptr<PlanarBackground>(new PlanarBackground(
        id, modelFactory.get(), uniformColorFactory.get(), scene, color,
        transformTreeFactory.get(), localPosition, localRotation, width, height,
        loggerFactory));
  }

  std::unique_ptr<FadePlanarBackground> createFadePlanarBackground(
      std::string id, Scene *scene, CL_Vec4 color, CL_Vec3 &localPosition,
      CL_Vec3 &localRotation, float width, float height) {
    return std::unique_ptr<FadePlanarBackground>(new FadePlanarBackground(
        id, modelFactory.get(), uniformFadeColorFactory.get(), scene, color,
        transformTreeFactory.get(), localPosition, localRotation, width, height,
        loggerFactory));
  }

  std::unique_ptr<CharacterElement> createCharacterElement(
      std::string id, CL_Vec2 origin, CL_GLfloat height, CL_GLfloat width,
      MaterialText *material, Scene *scene) {
    return std::unique_ptr<CharacterElement>(new CharacterElement(
        id, origin, height, width, material, modelFactory.get(), scene,
        transformTreeFactory.get()));
  }
  std::unique_ptr<TextElement> createTextElement(
      std::string id, FontStore *fontStore, TextStyle *textStyle,
      std::string text, CL_Vec3 &localPosition, CL_Vec3 &localRotation,
      Scene *scene) {
    return std::unique_ptr<TextElement>(new TextElement(
        id, fontStore, textStyle, text, transformTreeFactory.get(),
        localPosition, localRotation, this, scene));
  }
  std::unique_ptr<FontStore> createFontStore(Scene *scene,
                                             std::string fontName) {
    return std::unique_ptr<FontStore>(new FontStore(
        scene, fontName, textMaterialFactory.get(), loggerFactory));
  }
  std::unique_ptr<Reticle> createReticle(std::string id, Scene *scene,
                                         TransformTree *parentTransform,
                                         CL_Vec4 &color) {
    return std::unique_ptr<Reticle>(
        new Reticle(uniformColorFactory.get(), transformTreeFactory.get(),
                    parentTransform, modelFactory.get(), id, scene, color));
  }

  // std::unique_ptr<FadeScreen> createFadeScreen(std::string id, Scene *scene,
  // TransformTree *parentTransform, CL_Vec4 &color){
  // 	return std::unique_ptr<Reticle>(new Reticle(uniformColorFactory.get(),
  // transformTreeFactory.get(), parentTransform, modelFactory.get(), id, scene,
  // color));
  // }

  ITextMaterialFactory *getTextMaterialFactory() {
    return textMaterialFactory.get();
  }
  IUniformColorFactory *getUniformColorFactory() {
    return uniformColorFactory.get();
  }

 private:
  ILoggerFactory *loggerFactory = nullptr;
  std::unique_ptr<IModelFactory> modelFactory = nullptr;
  std::unique_ptr<IUniformColorFactory> uniformColorFactory = nullptr;
  std::unique_ptr<IUniformFadeColorFactory> uniformFadeColorFactory = nullptr;
  std::unique_ptr<ITransformTreeFactory> transformTreeFactory = nullptr;
  std::unique_ptr<ITextMaterialFactory> textMaterialFactory = nullptr;
};
}

#endif  // COREENGINE_UIFACTORY_H