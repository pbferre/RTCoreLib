#ifndef LINESEG_H
#define LINESEG_H

#include "point3d.h"
#include "rect3d.h"
#include "vector3d.h"

using namespace CreateCore;

namespace ModelViewer
{
class Triangle;

class LineSeg
{
public:
    LineSeg(Point3D p1, Point3D p2);

    Point3D p1() {return _p1;}
    Point3D p2() {return _p2;}

    bool Contains(Point3D point);
    bool Intersect(Rect3D box);
    bool Intersect(Triangle t, Point3D p);
    bool Intersect(Triangle t);

private:
    Point3D _p1, _p2;
    Point3D Intersection(Rect3D box);
    bool FastIntersect(Rect3D bb);

    static bool ComputeLineTriangleIntersection(bool FaceBoth, Point3D &origin, Vector3D &direction, Point3D &v0,
                                                Point3D &v1, Point3D &v2, Point3D &hitCoord, double &dist);

    static bool intersect3D_RayTriangle(Point3D &origin, Vector3D &direction, Point3D &v0 ,
                                        Point3D &v1, Point3D &v2, Point3D &P);
};

}

#endif // LINESEG_H
