#include "modelentity.h"
#include "supportgenerator.h"
#include "mainviewmodel.h"

using namespace CreateCore;

ModelEntity::ModelEntity()
{

}

void ModelEntity::GenerateAABBTree()
{
    aabbTree = new AABBTree();
    Transform3D* transform = new Transform3D();
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

void ModelEntity::GenerateSupportsPrep(MVMParameters params, SupportOperationType sot, QList<Point3D> positions, Rect3D bounds)
{
    _positions = positions;
    _bounds = bounds;

    // positions used for cylinder collision and discretization
    modelSupports = new SupportGenerator(params, positions, bounds, TransformState(), sot);
}

void ModelEntity::GetSingleSupportEntity(Point3D mouse)
{
    modelSupports->SingleSupportRay(this->modelVisual3D, mouse);
}
