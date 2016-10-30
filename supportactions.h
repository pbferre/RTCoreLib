#ifndef SUPPORTACTIONS_H
#define SUPPORTACTIONS_H

#include "point3d.h"
#include "rect3d.h"
#include "mainviewmodel.h"

namespace CreateCore
{

enum MeshType
{
    mtModel = 0,
    mtBase,
    mtGridPlane,
    mtSupport,
    mtNone
};

struct SliceMesh
{
    SliceMesh(QList<Point3D> pos, MeshType mt, Rect3D r, int eID, QList<float> t)
        {
            positions = pos;
            meshtype = mt;
            bounds = r;
            entityID = eID;
            transformM44 = t;
        }

        QList<Point3D> positions;
        MeshType meshtype;
        Rect3D bounds;
        int entityID;
        QList<float> transformM44;
};

class SupportActions
{
public:
    SupportActions();

    Rect3D SceneRect;
    QList<SliceMesh*> meshList;

    void AddMesh(float* vec3Array, int size, float height, int entityID, float* transform);
    void AddManualPoints(float* vec3Array, int length);

    MainViewModel* MVM;

private:
    int slicesCompleted;
    int points;
};

}

#endif // SUPPORTACTIONS_H
