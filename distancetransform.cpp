#include "distancetransform.h"
#include "astar.h"

#include <cmath>
#include <limits>

using namespace CreateCore;
using namespace ModelViewer;
using namespace Tests;

namespace ublas = boost::numeric::ublas;

double DistanceTransform::stepSize = 0.75;
double PathFinder::StepSize = 0.75;
double PathFinder::EntryStepSize = 0.5;

DistanceTransform::DistanceTransform()
{
    distanceTransform = 0;
}

DistanceTransform::DistanceTransform(AABBTree* aabbTree, double expansionDistance,
                                     double gridStepSize, double maxDist, bool quickInterior)
{
    _aabbTree = aabbTree;
    _gridStepSize = gridStepSize;
    SetGridSizes(expansionDistance, gridStepSize);
    _maxDist = maxDist;
    GenerateDistanceTransform(quickInterior);
}

void DistanceTransform::SetGridSizes(double expansionDistance, double gridStepSize)
{
    Rect3D bb = _aabbTree->Root()->BB();
    _startX = bb.X() - expansionDistance - gridStepSize / 2;
    _startY = bb.Y() - expansionDistance - gridStepSize / 2;
    _startZ = bb.Z() - expansionDistance - gridStepSize / 2;
    _sizeZ = fmax(0, ceil((bb.SizeX() + 2 * expansionDistance) / gridStepSize) + 2);
    _sizeY = fmax(0, ceil((bb.SizeY() + 2 * expansionDistance) / gridStepSize) + 2);
    _sizeX = fmax(0, ceil((bb.SizeX() + 2 * expansionDistance) / gridStepSize) + 2);
}

double DistanceTransform::Get(int x, int y, int z)
{
    return distanceTransform[x][y][z];
}

void DistanceTransform::Set(int x, int y, int z, double value)
{
    distanceTransform[x][y][z] = value;
}

void DistanceTransform::GenerateDistanceTransform(bool quickInterior)
{
    //distanceTransform = new double[sizeZ, sizeY, sizeX];
    distanceTransform = new double**[_sizeZ];
    for (int i = 0; i < _sizeZ; i++)
    {
        distanceTransform[i] = new double*[_sizeY];
        for (int j = 0; j < _sizeY; j++ )
        {
            distanceTransform[i][j] = new double[_sizeX];
        }
    }

    double previousD = -_maxDist;
    for (int i = 0; i < _sizeZ; i++)
    {
        double z = _startZ + i * _gridStepSize;
        for (int j = 0; j < _sizeY; j++)
        {
            bool lastExterior = true;
            bool knownSign = false;
            double y = _startY + j * _gridStepSize;
            for (int k = 0; k < _sizeX; k++)
            {
                double x = _startX + k * _gridStepSize;
                Point3D p(x, y, z);
                double d = _aabbTree->Distance(p, _maxDist, lastExterior, knownSign, quickInterior);
                previousD = d;
                lastExterior = d >= 0;
                knownSign = (fabs(d) > _gridStepSize);
                Set(i, j, k, -d);
            }
            lastExterior = true;
            knownSign = false;
        }
    }
}

double DistanceTransform::CubicInterp(QList<double> V, double frac)
{
    double A = (V.at(3) - V.at(2)) - (V.at(0) - V.at(1));
    double B = (V.at(0) - V.at(1)) - A;
    double C = V.at(2) - V.at(0);
    double D = V.at(1);

    return D + frac * (C + frac * (B + frac * A));
}

double DistanceTransform::Eval(double zi, double yi, double xi)
{
    Q_ASSERT(fmin(fmin(_sizeX, _sizeY), _sizeZ) > 3);
    int minXi = (int)fmin(fmax(0, floor(xi) - 1), _sizeX - 4);
    int minYi = (int)fmin(fmax(0, floor(yi) - 1), _sizeY - 4);
    int minZi = (int)fmin(fmax(0, floor(zi) - 1), _sizeZ - 4);
    double fracX = xi - minXi - 1;
    double fracY = yi - minYi - 1;
    double fracZ = zi - minZi - 1;

    QList<double> zVec;
    for (int k = 0; k < 4; k++)
    {
        QList<double> yVec;
        for (int j = 0; j < 4; j++)
        {
            QList<double> xVec;
            for (int i = 0; i < 4; i++)
            {
                xVec.append(Get(minZi + k, minYi + j, minXi + i));
            }
            yVec.append(CubicInterp(xVec, fracX));
        }
        zVec.append(CubicInterp(yVec, fracY));
    }
    double interpolatedValue = CubicInterp(zVec, fracZ);
    return interpolatedValue;
}

QList<Point3D> DistanceTransform::FindPath(Point3D start)
{
    World world;
    world.SetDistance(*this);

    double x1 = (start.x() - _startX) / _gridStepSize;
    double y1 = (start.y() - _startY) / _gridStepSize;
    double z1 = (start.z() - _startZ) / _gridStepSize;
    double pn[3] = { z1, y1, x1 };


    double pe[3];
    double z2 = (2 - _startZ) / _gridStepSize;
    pe[0] = z2;
    pe[1] = y1;
    pe[2] = x1;

    Point3D vStart(pn[0], pn[1], pn[2]);
    Point3D vEnd(pe[0], pe[1], pe[2]);

    bool buildPlaneShortCut = false;    //(end == 0);
    SearchNode* node = PathFinder::FindPath(world, vStart, vEnd, this, buildPlaneShortCut, adjustableEndpoint);


    QList<Point3D> path;
    while (node)// && node.next != null)
    {
        double z = _startZ + node->Position().x() * _gridStepSize;
        double y = _startY + node->Position().y() * _gridStepSize;
        double x = _startX + node->Position().z() * _gridStepSize;
        Point3D a(x, y, z);
        path.append(a);
        node = node->Next();
    }
    SmoothPath(path);
    return path;
}

QList<Point3D> DistanceTransform::FindPath(Point3D start, Point3D end, bool adjustableEndpoint)
{
    //if(world==null)
    World world;
    world.SetDistance(*this);

    double x1 = (start.x() - _startX) / _gridStepSize;
    double y1 = (start.y() - _startY) / _gridStepSize;
    double z1 = (start.z() - _startZ) / _gridStepSize;
    double pn[3] = { z1, y1, x1 };


    double pe[3];
    Point3D e = end;
    double x2 = (e.x() - _startX) / _gridStepSize;
    double y2 = (e.y() - _startY) / _gridStepSize;
    double z2 = (e.z() - _startZ) / _gridStepSize;
    pe[0] = z2;
    pe[1] = y2;
    pe[2] = x2;

    Point3D vStart(pn[0], pn[1], pn[2]);
    Point3D vEnd(pe[0], pe[1], pe[2]);

    bool buildPlaneShortCut = false;    //(end == 0);
    SearchNode* node = PathFinder::FindPath(world, vStart, vEnd, this, buildPlaneShortCut, adjustableEndpoint);


    QList<Point3D> path;
    while (node)// && node.next != null)
    {
        double z = _startZ + node->Position().x() * _gridStepSize;
        double y = _startY + node->Position().y() * _gridStepSize;
        double x = _startX + node->Position().z() * _gridStepSize;
        Point3D a(x, y, z);
        path.append(a);
        node = node->Next();
    }
    SmoothPath(path);
    return path;
}

void DistanceTransform::SmoothPath(QList<Point3D> &path)
{
    if (path.count() < 5)
        return;

    QList<Point3D> smoothed;
    int radius = 7;

    for (int i = 0; i < path.count(); i++)
    {
        int leftD = i - 1;//skip first first and last
        int rightD = path.count() - 1 - i;
        int r = radius;
        if (leftD < r) r = leftD;
        if (rightD < r) r = rightD;
        if (r < 0) r = 0;
        if (r == 1)
        {
            Vector3D mean(0, 0, 0);
            for (int j = -r; j <= r; j++)
            {
                Point3D pt = path.at(i + j);
                mean = mean + pt.ToVector3D();
            }

            int sz = 2 * r + 1;
            Vector3D v = mean / (sz);
            smoothed.append(v.ToPoint3D());

            double* xs = new double[sz];
            QList<double*> ys;
            for (int t = 0; t < 3; t++)
                ys.append(new double[sz]);

            for (int j = -r; j <= r; j++)
            {
                xs[j + r] = j;
                ys.at(0)[j + r] = path.at(i + j).x();
                ys.at(1)[j + r] = path.at(i + j).y();
                ys.at(2)[j + r] = path.at(i + j).z();
            }
            QList<double> temp = Polyfit(xs, ys.at(0), sz, 3, sz);
            double x = temp.isEmpty() ? 0 : temp.at(0);
            temp = Polyfit(xs, ys.at(1), sz, 3, sz);
            double y = temp.isEmpty() ? 0 : temp.at(0);
            temp = Polyfit(xs, ys.at(2), sz, 3, sz);
            double z = temp.isEmpty() ? 0 : temp.at(0);
            smoothed.append(Point3D(x, y, z));
        }
        else
        {
            smoothed.append(path.at(i));
        }
    }

    path = smoothed;
}

Point3D DistanceTransform::AStar_GetFirstUnimpededPoint(double modelDistance, Point3D start)
{
    int nx = _sizeZ;
    int ny = _sizeY;
    int nz = _sizeX;
    double x1 = (start.x() - _startX) / _gridStepSize;
    double y1 = (start.y() - _startY) / _gridStepSize;
    double z1 = (start.z() - _startZ) / _gridStepSize;
    double pn[3] = { z1, y1, x1 };
    Point3D vStart(pn[0], pn[1], pn[2]);
    Point3D vEnd(0, pn[1], pn[2]);

    // make a new dt that looks for completely open vertical columns
    double** dt2d; //= new double[ny, nz];
    dt2d = new double*[ny];
    for (int i = 0; i < ny; i++)
    {
        dt2d[ny] = new double[nz];
    }


    for (int yi = 0; yi < ny; yi++)
    {
        for (int zi = 0; zi < nz; zi++)
        {
            double max = DBL_MIN;
            for (int xi = 0; xi < z1 - 1; xi++)
            {
                if (xi > 0 && xi < nx - 1)
                {
                    //deal with bad dt's with strings of eronious positive values
                    double temp = DBL_MAX;
                    for (int i = xi - 1; i < xi + 2; i++)
                    {
                        temp = fmin(temp, distanceTransform[i][yi][zi]);
                    }
                    max = fmax(max, temp);
                }
                else
                    max = fmax(max, distanceTransform[xi][yi][zi]);
            }
            dt2d[yi][zi] = max;
        }
    }

    Point3D centerP(DBL_MIN, DBL_MIN, DBL_MIN);
    double nearest = DBL_MAX;
    for (int yi = 0; yi < ny; yi++)
    {
        double y = yi * _gridStepSize + _startY;
        for (int zi = 0; zi < nz; zi++)
        {
            double x = zi * _gridStepSize + _startX;
            if (dt2d[yi][zi] < modelDistance)
            {
                double d = sqrt((x - start.x()) * (x - start.x()) + (y - start.y()) * (y - start.y()));
                if (d < nearest)
                {
                    nearest = d;
                    centerP = Point3D(x, y, start.z() - _gridStepSize);
                }
            }
        }
    }
    return centerP;
}

QList<double> DistanceTransform::Polyfit(double *x, double *y, int sz, int degree, int szy)
{
    try
    {
        ublas::matrix<double> m(sz, degree + 1);
        for (int i = 0; i < sz; i++)
        {
            for (int j = 0; j <= degree; j++)
            {
                m(i, j) = pow(x[i], j);
            }
        }

        ublas::vector<double> v(szy);
        for (int i = 0; i < szy; i++)
        {
            v[i] = y[i];
        }

        ublas::matrix<double> transpose(m);
        ublas::trans(transpose);
        ublas::matrix<double> square = ublas::prod(m, transpose);

        ublas::matrix<double> inverse(square.size1(), square.size2());
        InvertMatrix(square, inverse);
        ublas::matrix<double> p = ublas::prod(inverse, transpose);
        ublas::vector<double> res = ublas::prod(p, v);

        QList<double> result;
        for (int i = 0; i < res.size(); i++)
            result.append(res[i]);

        return result;

    }
    catch(...)
    {
        return QList<double>();
    }
}
