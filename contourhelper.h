#ifndef CONTOURHELPER_H
#define CONTOURHELPER_H

namespace CreateCore
{
class Point3D;
class Vector3D;

class ContourHelper
{
public:
    ContourHelper(Point3D planeOrigin, Vector3D planeNormal);

    int ContourFacet(Point3D p0, Point3D p1, Point3D p2, Point3D s0, Point3D &s1);

private:
    double A, B, C, D;
    double side[3];
};

}
#endif // CONTOURHELPER_H
