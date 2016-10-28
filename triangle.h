#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "lineseg.h"
#include "point3d.h"
#include "vector3d.h"

using namespace CreateCore;

namespace CreateCore
{

class Triangle
{
public:
    explicit Triangle();
    explicit Triangle(Point3D v1, Point3D v2, Point3D v3);
    explicit Triangle(Point3D v1, Point3D v2, Point3D v3, Point3D mid, int index);

    Vector3D GetNormal();
    bool Intersect(LineSeg l);
    bool Intersect(Triangle t);
    double Distance(Point3D p, Point3D &closestPoint);
    double DistanceSquared(Point3D p, Point3D &closestPoint);
    double GetIncidentAngle(Point3D p);

    Point3D V1() { return _V1; }
    Point3D V2() { return _V2; }
    Point3D V3() { return _V3; }
    Point3D Midpoint() { return _midpoint; }
    int Index() { return _index; }

private:
    Point3D _V1, _V2, _V3, _midpoint;
    int _index;

};

}

#endif // TRIANGLE_H
