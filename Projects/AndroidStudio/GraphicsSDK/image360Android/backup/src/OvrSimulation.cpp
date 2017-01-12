//
// Created by robin_chimera on 1/7/2017.
//

#include <OvrSimulation.h>

namespace cl{
        /**
     *
     * OvrSimulation class methods
     *
     */
        void OvrSimulation::clear() {
            this->currentRotation.x = 0.0f;
            this->currentRotation.y = 0.0f;
            this->currentRotation.z = 0.0f;
        }

        void OvrSimulation::advance(double predictedDisplayTime) {
            this->currentRotation.x = (float) (predictedDisplayTime);
            this->currentRotation.y = (float) (predictedDisplayTime);
            this->currentRotation.z = (float) (predictedDisplayTime);
        }
};