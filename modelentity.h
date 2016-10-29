#ifndef MODELENTITY_H
#define MODELENTITY_H

#include "supportgenerator.h"
#include "aabbtree.h"
#include "distancetransform.h"

namespace CreateCore
{

class ModelEntity
{
public:
    ModelEntity();

    ModelVisual3DObservable* SupportModelVisual3D() { return supportModelVisual3D; }
    void setSupportModelVisual3D(ModelVisual3DObservable* value) { supportModelVisual3D = value; }

    ModelVisual3DObservable* ModelVisual3D() { return modelVisual3D; }
    void setModelVisual3D(ModelVisual3DObservable* value) { modelVisual3D = value; }

    AABBTree* Tree() { return aabbTree; }
    void GenerateAABBTree();

    DistanceTransform* DistTransform() { return distanceTransform; }
    void GenerateDistanceTransform();

private:
    ModelVisual3DObservable* supportModelVisual3D;
    ModelVisual3DObservable* modelVisual3D;
    Model3DGroup modelGroup;

    AABBTree* aabbTree;
    DistanceTransform* distanceTransform;
};

}

#endif // MODELENTITY_H
