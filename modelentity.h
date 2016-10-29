#ifndef MODELENTITY_H
#define MODELENTITY_H

//#include "supportgenerator.h"
#include "aabbtree.h"
#include "distancetransform.h"
#include "support.h"
#include "mvmparameters.h"

class SupportGenerator;

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

    Model3DGroup* ModelGroup() { return modelGroup; }
    void setModelGroup(Model3DGroup* value) { modelGroup = value; }

    AABBTree* Tree() { return aabbTree; }
    void GenerateAABBTree();

    DistanceTransform* DistTransform() { return distanceTransform; }
    void GenerateDistanceTransform();

    long PositionsHash;
    void GenerateSupportsPrep(MVMParameters params, SupportOperationType sot, QList<Point3D> positions, Rect3D bounds);

    SupportGenerator* modelSupports;

    void GetSingleSupportEntity(Point3D mouse);

private:
    ModelVisual3DObservable* supportModelVisual3D;
    ModelVisual3DObservable* modelVisual3D;
    Model3DGroup* modelGroup;

    AABBTree* aabbTree;
    DistanceTransform* distanceTransform;

    QList<Point3D> _positions;
    Rect3D _bounds;
};

}

#endif // MODELENTITY_H
