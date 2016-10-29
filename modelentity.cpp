#include "modelentity.h"

ModelEntity::ModelEntity()
{

}

void ModelEntity::GenerateAABBTree()
{
    aabbTree = new AABBTree();
    Transform3D transform;
    QElapsedTimer t;
    t.start();
    aabbTree->PopulateFromModel(modelGroup, transform);
    qDebug() << QString("AABB Tree Generation took %1 ms").arg(t.elapsed());
}

void ModelEntity::GenerateDistanceTransform()
{
    double expansionDistance = MVM->Parameters().ShellingThickness();     //MainViewModel .Instance.ShellingThickness;
    double gridStepSize = MVM->Parameters().ShellingGridStepSize();      //MainViewModel.Instance.ShellingGridStepSize;
    double supportSpacing = MVM->Parameters().ShellingInteriorSupportSpacing();        //MainViewModel.Instance.ShellingInteriorSupportSpacing;
    double supportWidth = MVM->Parameters().ShellingInteriorSupportWidth();      //MainViewModel.Instance.ShellingInteriorSupportWidth;
    int subsampling = MVM->Parameters().ShellingSubSampling();      //(int)MainViewModel.Instance.ShellingSubsampling;
    double maxDist = expansionDistance + gridStepSize;
    distanceTransform = new DistanceTransform(aabbTree, expansionDistance, gridStepSize, maxDist, false);
}
