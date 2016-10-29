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

private:
    MVMParameters parameters;

    int numSlices;
};

MainViewModel *MVM;

}

#endif // MAINVIEWMODEL_H
