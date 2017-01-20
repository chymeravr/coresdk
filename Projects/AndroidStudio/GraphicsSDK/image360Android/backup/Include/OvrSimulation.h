//
// Created by robin_chimera on 1/7/2017.
//

#ifndef GRAPHICSSDK_OVRSIMULATION_H
#define GRAPHICSSDK_OVRSIMULATION_H

#include <VrApi_Types.h>

namespace cl {

    class OvrSimulation {
        ovrVector3f currentRotation;
    public:
        void clear();
        void advance(double predictedDisplayTime);
    };
}
#endif //GRAPHICSSDK_OVRSIMULATION_H
