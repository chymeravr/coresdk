#include <coreEngine/components/gazeDetector/GazeDetector.h>
#include <assert.h>
#include <iostream>

namespace cl{
	GazeDetector::GazeDetector(std::string &idOfObjectComponentAttachedTo,
		TransformTree *shooterTransform,
		TransformTree *gazeObjectTransform,
		EventGazeListener *gazeListener,
		GazeDetectorContainer *gazeDetectorContainer){
		
		this->id = idOfObjectComponentAttachedTo;
		
		assert(shooterTransform != nullptr);
		this->shooterTransform = shooterTransform;

		assert(gazeObjectTransform != nullptr);
		this->gazeObjectTransform = gazeObjectTransform;

		assert(gazeListener != nullptr);
		this->gazeListener = gazeListener;

		assert(gazeDetectorContainer != nullptr);
		this->gazeDetectorContainer = gazeDetectorContainer;
	}

	std::string GazeDetector::getComponentType(){
		return this->componentType;
	}

	IRenderable *GazeDetector::getRenderable(){
		return this;
	}

	bool GazeDetector::initialize(){
		gazeDetectorContainer->totalGazeDetectors++;
		gazeDetectorContainer->gazeDetectors[id] = this;
		return true;
	}

	void GazeDetector::draw(){
		isGaze = this->getIsGazed();
		if (isGaze){
			distanceFromShooter = this->getDistanceFromShooter();
		}

		gazeDetectorContainer->gazeDetectorsScannedInCurrentLoop++;

		if (gazeDetectorContainer->gazeDetectorsScannedInCurrentLoop == gazeDetectorContainer->totalGazeDetectors){//this is last gazeDetector
			dispatchEvents();
			resetContainer();
		}
	}

	void GazeDetector::deinitialize(){
		gazeDetectorContainer->totalGazeDetectors--;
		gazeDetectorContainer->gazeDetectors.erase(id);
	}

	void GazeDetector::dispatchEvents(){
		GazeDetector *winner = nullptr;
		float distance = -1.0f;
		for (auto it = gazeDetectorContainer->gazeDetectors.begin(); it != gazeDetectorContainer->gazeDetectors.end(); it++){
			GazeDetector *current = (*it).second;
			if (current->isGaze){
				if (distance == -1.0f){
					winner = current;
					distance = current->distanceFromShooter;
				}
				else{
					if (current->distanceFromShooter < distance){
						winner = current;
						distance = current->distanceFromShooter;
					}
				}
			}
		}

		if (gazeDetectorContainer->currentTarget == nullptr){
			if (winner == nullptr){
			}
			else{
				winner->gazeListener->onGazeStarted();
				winner->gazeListener->onGaze();
			}
		}
		else if (winner == nullptr){
			gazeDetectorContainer->currentTarget->gazeListener->onGazeEnded();
		}
		else{
			if (gazeDetectorContainer->currentTarget->id == winner->id){
				gazeDetectorContainer->currentTarget->gazeListener->onGaze();
			}
			else{
				gazeDetectorContainer->currentTarget->gazeListener->onGazeEnded();
				winner->gazeListener->onGazeStarted();
				winner->gazeListener->onGaze();
			}
		}

		gazeDetectorContainer->currentTarget = winner;
	}

	void GazeDetector::resetContainer(){
		gazeDetectorContainer->gazeDetectorsScannedInCurrentLoop = 0;
	}
}