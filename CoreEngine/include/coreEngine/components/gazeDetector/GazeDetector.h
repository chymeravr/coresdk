#ifndef COREENGINE_GAZEDETECTOR_H
#define COREENGINE_GAZEDETECTOR_H

#include <coreEngine/renderObjects/IComponent.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/components/transformTree/TransformTree.h>
#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/components/gazeDetector/GazeDetectorContainer.h>

namespace cl{
	class GazeDetector : public IComponent, public IRenderable{
	public:
		/*
		* @args idOfObjectComponentAttachedTo: sceneId (unique) of the object this component is attached to
		* @args shooterTransform: transformTree of the object which is used as shooting source (camera or gun or something)
		* @args gazeObjectTransform: transformTree of the object this component is attached to
		* @args gazeListener:
		* @args gazeDetectorContainer: This objects contain information about other gaze detectors this gaze detector might require
		*/
		GazeDetector(std::string &idOfObjectComponentAttachedTo, 
			TransformTree *shooterTransform,
			TransformTree *gazeObjectTransform,
			EventGazeListener *gazeListener,
			GazeDetectorContainer *gazeDetectorContainer);
		//IComponent interface implementation
		std::string getComponentType();
		IRenderable *getRenderable();

		//IRenderable interface implementation
		bool initialize();
		void draw();
		void deinitialize();

		//Interface to be implemented by deriving classes
		virtual bool getIsGazed() = 0;
		virtual float getDistanceFromShooter() = 0;

	private:
		void dispatchEvents();
		void resetContainer();
	protected:
		std::string componentType = "gazeDetector";
		std::string id = "";
		TransformTree *shooterTransform = nullptr;
		TransformTree *gazeObjectTransform = nullptr;
		EventGazeListener *gazeListener = nullptr;
		GazeDetectorContainer *gazeDetectorContainer = nullptr;

		bool isGaze = false;
		float distanceFromShooter = 0.0f;
	};
}

#endif //COREENGINE_GAZEDETECTOR_H