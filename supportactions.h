#ifndef SUPPORTACTIONS_H
#define SUPPORTACTIONS_H

#include "point3d.h"
#include "rect3d.h"

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

    static Rect3D SceneRect;
    static int points;
    static QList<SliceMesh*> meshList;

    static void AddMesh(float* vec3Array, int size, float height, int entityID, float* transform);
    static void AddManualPoints(float* vec3Array, int length);
};

}

#endif // SUPPORTACTIONS_H
