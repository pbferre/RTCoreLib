#ifndef CYLINDERCOLLISION_H
#define CYLINDERCOLLISION_H

#include "point3d.h"
#include "rect3d.h"

#include <QtCore>

#include <tuple>

typedef std::tuple<int, int> Segment3DIndex;

//using namespace CreateCore;

namespace CreateCore
{

struct Cylinder
{
    double _x;
    double _y;
    double _r;
    double _zBot;
    double _zTop;

    Cylinder(double x, double y, double r, double zBot, double zTop)
    {
        _x = x;
        _y = y;
        _r = r;
        _zBot = zBot;
        _zTop = zTop;
    }

    void Clear()
    {
        _x = 0;
        _y = 0;
        _r = 0;
        _zBot = 0;
        _zTop = 0;
    }
};

class Segment3D
{
public:
    Segment3D(Point3D p0, Point3D p1)
    {
        if (_p1.z() >= _p0.z())
        {
            _p0 = p0;
            _p1 = p1;
        }
        else
        {
            _p0 = p1;
            _p1 = p0;
        }
    }

    Point3D P0() { return _p0; }
    Point3D P1() { return _p1; }

private:
    Point3D _p0;
    Point3D _p1;
};

class Segment3DIndexComparer
{
public:
    static bool Equals(Segment3DIndex s0, Segment3DIndex s1)
    {
        if ((std::get<0>(s0) == std::get<0>(s1)) && (std::get<1>(s0) == std::get<1>(s1)))
            return true;
        else if ((std::get<0>(s0) == std::get<1>(s1)) && (std::get<1>(s1) == std::get<0>(s1)))
            return true;
        else
            return false;
    }

    static int GetHashCode(Segment3DIndex s)
    {
        return std::get<0>(s) * 16777619 * std::get<1>(s);
    }
};

class CylinderCollision
{
public:
    CylinderCollision(QList<Point3D> positions, Rect3D bounds, double res = 0.5);

    void BresenhamLine(long x0, long y0, long x1, long y1);
    void AddCurrentIndex(int i, int j);
    bool CollidesWithMesh(Segment3D cylinder, double radius);
    bool CollidesWithSegment3D(int i0, int i1, Segment3D cylinder, double radius);
    bool CollidesWithMesh(Cylinder cyl);
    bool CollidesWithTriangle(int i0, int i1, Cylinder cyl);

    static QList<double> LastFailure;
    static bool ClipEdge(Segment3D edge, double minZ, double maxZ);
    static double DistanceSqBetweenSegments(Segment3D s0, Segment3D s1);


private:
    QList<Point3D> _positions;
    QList<Segment3DIndex>** _point3DIndices;

    double _resolutionX;
    double _resolutionY;

    double _meshXOffset;
    double _meshYOffset;

    int _meshXLimit;
    int _meshYLimit;

    int _p0Index;
    int _p1Index;
    long _div;

    bool Seg3DIndexOrder(Segment3DIndex s1, Segment3DIndex s2);
};

}

#endif // CYLINDERCOLLISION_H
