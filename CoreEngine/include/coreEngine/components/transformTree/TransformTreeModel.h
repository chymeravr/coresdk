#ifndef COREENGINE_TRANSFORMTREEMODEL_H
#define COREENGINE_TRANSFORMTREEMODEL_H

#include <coreEngine/components/transformTree/TransformTree.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/renderObjects/IComponent.h>

namespace cl{
	class TransformTreeModel : public TransformTree, public IRenderable, public IComponent{
	public:
		TransformTreeModel(Model *model, ILoggerFactory *loggerFactory);
		std::string getComponentType();
		virtual IRenderable *getRenderable();
		virtual bool initialize();
		virtual void draw();
		virtual void deinitialize();
	private:
		std::string componentType = "transformTree";
		Model *model = nullptr;
		std::unique_ptr<ILogger> logger;
	};
}

#endif //COREENGINE_TRANSFORMTREEMODEL_H