#include "contourhelper.h"
#include "point3d.h"
#include "vector3d.h"

#include <cmath>

using namespace CreateCore;

ContourHelper::ContourHelper(Point3D planeOrigin, Vector3D planeNormal)
{
    // Determine the equation of the plane as
    // Ax + By + Cz + D = 0
    double l = sqrt(planeNormal.x() * planeNormal.x() + planeNormal.y() * planeNormal.y() + planeNormal.z() * planeNormal.z());
    A = planeNormal.x() / l;
    B = planeNormal.y() / l;
    C = planeNormal.z() / l;
    D = -(planeNormal.x() * planeOrigin.x() + planeNormal.y() * planeOrigin.y() + planeNormal.z() * planeOrigin.z());
}

int ContourHelper::ContourFacet(Point3D p0, Point3D p1, Point3D p2, Point3D s0, Point3D &s1)
{
    // Evaluate the equation of the plane for each vertex
    // If side < 0 then it is on the side to be retained
    // else it is to be clipped
    side[0] = A * p0.x() + B * p0.y() + C * p0.z() + D;
    side[1] = A * p1.x() + B * p1.y() + C * p1.z() + D;
    side[2] = A * p2.x() + B * p2.y() + C * p2.z() + D;

    // Are all the vertices on the same side?
    if (side[0] >= 0 && side[1] >= 0 && side[2] >= 0)
    {
        s0 = Point3D();
        s1 = Point3D();
        return -1;
    }

    if (side[0] <= 0 && side[1] <= 0 && side[2] <= 0)
    {
        s0 = Point3D();
        s1 = Point3D();
        return -2;
    }

    // Is p0 the only point on a side by itself
    if ((side[0] * side[1] < 0) && (side[0] * side[2] < 0))
    {
        s0 = Point3D(p0.x() - side[0] * (p2.x() - p0.x()) / (side[2] - side[0]),
                     p0.y() - side[0] * (p2.y() - p0.y()) / (side[2] - side[0]),
                     p0.z() - side[0] * (p2.z() - p0.z()) / (side[2] - side[0]));

        s1 = Point3D(p0.x() - side[0] * (p1.x() - p0.x()) / (side[1] - side[0]),
                     p0.y() - side[0] * (p1.y() - p0.y()) / (side[1] - side[0]),
                     p0.z() - side[0] * (p1.z() - p0.z()) / (side[1] - side[0]));

        return side[0] > 0 ? 0 : 10;
    }

    // Is p1 the only point on a side by itself
    if ((side[1] * side[0] < 0) && (side[1] * side[2] < 0))
    {
        s0 = Point3D(p1.x() - side[1] * (p2.x() - p1.x()) / (side[2] - side[1]),
                     p1.y() - side[1] * (p2.y() - p1.y()) / (side[2] - side[1]),
                     p1.z() - side[1] * (p2.z() - p1.z()) / (side[2] - side[1]));

        s1 = Point3D(p1.x() - side[1] * (p0.x() - p1.x()) / (side[0] - side[1]),
                     p1.y() - side[1] * (p0.y() - p1.y()) / (side[0] - side[1]),
                     p1.z() - side[1] * (p0.z() - p1.z()) / (side[0] - side[1]));

        return side[1] > 0 ? 1 : 11;
    }

    /* Is p2 the only point on a side by itself */
    if ((side[2] * side[0] < 0) && (side[2] * side[1] < 0))
    {
        s0 = Point3D(p2.x() - side[2] * (p0.x() - p2.x()) / (side[0] - side[2]),
                     p2.y() - side[2] * (p0.y() - p2.y()) / (side[0] - side[2]),
                     p2.z() - side[2] * (p0.z() - p2.z()) / (side[0] - side[2]));

        s1 = Point3D(p2.x() - side[2] * (p1.x() - p2.x()) / (side[1] - side[2]),
                     p2.y() - side[2] * (p1.y() - p2.y()) / (side[1] - side[2]),
                     p2.z() - side[2] * (p1.z() - p2.z()) / (side[1] - side[2]));

        return side[2] > 0 ? 2 : 12;
    }

    s0 = Point3D();
    s1 = Point3D();
    return -10;
}
