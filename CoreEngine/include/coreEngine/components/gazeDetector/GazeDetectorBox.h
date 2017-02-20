#ifndef COREENGINE_GAZEDETECTERBOX_H
#define COREENGINE_GAZEDETECTERBOX_H

#include <coreEngine/components/gazeDetector/GazeDetector.h>

namespace cl{
	class GazeDetectorBox : public GazeDetector{
	public:
		/*
		* @args idOfObjectComponentAttachedTo: sceneId (unique) of the object this component is attached to
		* @args shooterTransform: transformTree of the object which is used as shooting source (camera or gun or something)
		* @args gazeObjectTransform: transformTree of the object this component is attached to
		* @args gazeListener:
		* @args gazeDetectorContainer: This objects contain information about other gaze detectors this gaze detector might require
		* @args lengthX: length along X-axis of the bounding box in local coordinates
		* @args lengthY: length along Y-axis of the bounding box in local coordinates
		* @args lengthZ: length along Z-axis of the bounding box in local coordinates
		*/
		GazeDetectorBox(std::string &idOfObjectComponentAttachedTo,
			TransformTree *shooterTransform,
			TransformTree *gazeObjectTransform,
			EventGazeListener *gazeListener,
			GazeDetectorContainer *gazeDetectorContainer,
			CL_Vec3 &shooterLocalOrigin,
			CL_Vec3 &shooterLocalLookAt,
			float lengthX,
			float lengthY,
			float lengthZ);
		//Implementing GazeDetector interface
		bool getIsGazed();
		float getDistanceFromShooter();
	private:
		bool intersectionWithPlane(CL_Vec3 &P0, CL_Vec3 &P1, CL_Vec3 &P3, CL_Vec3 &L0, CL_Vec3 &L1);
		float dotProduct(CL_Vec3 &v1, CL_Vec3 &v2);
	private:
		float lengthX;
		float lengthY;
		float lengthZ;
		CL_Vec3 shooterLocalOrigin;
		CL_Vec3 shooterLocalLookAt;
	};
}

#endif //COREENGINE_GAZEDETECTERBOX_H