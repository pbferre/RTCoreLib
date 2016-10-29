#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include "point3d.h"
#include "mvmparameters.h"
#include "modelentity.h"

namespace CreateCore
{

enum SupportOperationType
{
    sotElevateNoBase = 0,
    sotSingleBaseWithSupports,
    sotSingleBaseNoSupports,
    sotBasePerSupport,
    sotNone
};

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

private:
    MVMParameters parameters;
    ModelEntity* currentEntity;

    int numSlices;

    int GetPositionsHash(QList<Point3D> positions);
    long doubleToInt64Bits(double val);
};

MainViewModel *MVM;

}

#endif // MAINVIEWMODEL_H
