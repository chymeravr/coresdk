#ifndef COREENGINE_CUBEBUILDER_H
#define COREENGINE_CUBEBUILDER_H

#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/modifier/ModelModifier.h>

namespace cl{
	class CubeBuilder{
	public:
		CubeBuilder(ModelModifier *modelModifier);
		/**
		* Builds a cube whose faces have normals inwards. Order of faces in the data are Front, Left, Back, Right, Top, Bottom.
		*/
		virtual void buildInwardCube(Model *model);
		/**
		* Builds a cube whose faces have normals outwards. Order of faces in the data are Front, Left, Back, Right, Top, Bottom.
		*/
		virtual void buildOutwardCube(Model *model);

	private:
		ModelModifier *modelModifier;
	};
}

#endif //COREENGINE_CUBEBUILDER_H