#ifndef DISTANCETRANSFORM_H
#define DISTANCETRANSFORM_H

#include <QtCore>

#include "aabbtree.h"
#include "rect3d.h"
#include "point3d.h"

using namespace CreateCore;

namespace ModelViewer
{

class DistanceTransform
{
public:
    DistanceTransform();
    DistanceTransform(AABBTree aabbTree, double expansionDistance,
                      double gridStepSize, double maxDist, bool quickInterior = false);

    void SetGridSizes(double expansionDistance, double gridStepSize);
    void GenerateDistanceTransform(bool quickInterior = false);
    double Eval(double zi, double yi, double xi);
    QList<Point3D> FindPath(Point3D start, Point3D end, bool adjustableEndpoint = false);
    void SmoothPath(QList<Point3D> &path);
    Point3D AStar_GetFirstUnimpededPoint(double modelDistance, Point3D start);
    double Get(int x, int y, int z);
    void Set(int x, int y, int z, double value);

    static double CubicInterp(QList<double> V, double frac);
    static QList<double> Polyfit(double* x, double* y, int szx, int degree, int szy);
    static double stepSize;

    int SizeX() { return _sizeX; }
    int SizeY() { return _sizeY; }
    int SizeZ() { return _sizeZ; }

    double StartX() { return _startX; }
    double StartY() { return _startY; }
    double StartZ() { return _startZ; }

private:
    int _sizeX, _sizeY, _sizeZ;
    double _startX, _startY, _startZ;
    double _gridStepSize;
    double _maxDist;
    double *** distanceTransform;
    AABBTree _aabbTree;
};

}

#endif // DISTANCETRANSFORM_H
