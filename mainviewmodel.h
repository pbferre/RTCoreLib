#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include "point3d.h"
#include "mvmparameters.h"
#include "support.h"

namespace CreateCore
{

class ModelEntity;

class MainViewModel
{
public:
    MainViewModel();

    static QList<QList<Point3D>> CylinderPaths;
    static QList<QList<double>> CylinderDiams;
    static QList<ModelEntity*> listModelEntities;

    MVMParameters Parameters() { return parameters; }

    int NumSlices() { return numSlices; }
    void setNumSlices(int value) { numSlices = value; }

    void SupportEntityPrep(QList<Point3D> positions, Rect3D bounds, QList<float> transform, SupportOperationType sot = sotBasePerSupport);

    ModelEntity* CurrentEntity() { return currentEntity; }

    void GetSingleSupport(Point3D mouse);

private:
    MVMParameters parameters;
    ModelEntity* currentEntity;

    int numSlices;

    long GetPositionsHash(QList<Point3D> positions);
    long doubleToInt64Bits(double val);
};

static MainViewModel *MVM;

}

#endif // MAINVIEWMODEL_H
