#ifndef ASTAR_H
#define ASTAR_H

#include <QtCore>

#include "point3d.h"
#include "distancetransform.h"

using namespace CreateCore;
using namespace ModelViewer;

namespace Tests
{

class World;

class BRWorld2
{
public:
    BRWorld2(int lWorldX, int lWorldY, int lWorldZ, double zeroX, double zeroY, double zeroZ, double stepSize = 1.0);
    bool Get(double x, double y, double z);
    void Set(double x, double y, double z);
    void Unset(double x, double y, double z);

private:
    QSet<int> brWorld;
    double _zeroX;
    double _zeroY;
    double _zeroZ;
    int _lWorldX;
    int _lWorldY;
    int _lWorldZ;
    double _stepSize;
};

class SearchNode
{
public:
    SearchNode(Point3D position, double cost, double pathCost, SearchNode* next)
    {
        _position = position;
        _cost = cost;
        _pathCost = pathCost;
        _next = next;
    }

    SearchNode* Next() { return _next; }
    SearchNode* NextListElem();

    Point3D Position() { return _position; }
    double Cost() { return _cost; }
    double PathCost() { return _pathCost; }


    static bool Compare(SearchNode* a,  SearchNode* b);

private:
    Point3D _position;
    double _cost;
    double _pathCost;
    SearchNode* _next;
    SearchNode* _nextListElem;
};

class MinHeap
{
public:
    bool HasNext() { return theList.count() > 0; }
    void Add(SearchNode* item);
    SearchNode* ExtractFirst();

private:
    QList<SearchNode*> theList;
};

class Surr
{
public:
    double (*CostFunc)(Point3D p);

    Surr(double x, double y, double z) {init(Point3D(x, y, z), 0); }
    Surr(Point3D p) { init(p, 0); }
    Surr(Point3D p, double (*cf)(Point3D)) { init(p, cf); }

    //norms
    static double Euclidean(Point3D p);

    // lower cost to moving horizontally
    static double Sideways(Point3D p);

    // lower cost to moving downward
    static double Downward(Point3D p);
    static double DSquared(Point3D p);
    static double Manhattan(Point3D p);
    static double SupNorm(Point3D p);

    Point3D Point() { return _point; }
    double Cost() { return _cost; }

    void SetPoint(Point3D value) { _point = value; }
    void SetCost(double value) { _cost = value; }

    static bool LessThan(Surr s1, Surr s2)
    {
        if (s1.Cost() < s2.Cost())
            return true;
        if (s2.Cost() < s1.Cost())
            return false;

        return Point3D::ComparePoints(s1.Point(), s2.Point());
    }

private:
    Point3D _point;
    double _cost;

    void init(Point3D p, double (*cf)(Point3D p))
    {
        _point = p;
        _cost = cf(p);
    }
};

bool operator ==(Surr s1, Surr s2);

class PathFinder
{
public:
    static SearchNode* FindPath(World world, Point3D start, Point3D end, DistanceTransform* dt, bool unimpededZShortcut = true, bool adjustableEndpoint = false);
    static double EndDistance(Point3D p, Point3D end, bool zOnly);
    static double EndDistanceSquared(Point3D p, Point3D end, bool zOnly);
    static SearchNode* FindPathReversed(World world, Point3D start, Point3D end, DistanceTransform* dt, bool UnimpededZShortcut, bool adjustableEndpoint = false);
    static bool FindNearestStart(World world, Point3D start, Point3D end, DistanceTransform* dt);

    static Point3D AStar_LastUnimpededZVoxel;

    static double StepSize;
    static QList<Surr> Surrounding;
    static QList<Surr> InitializeSurrounding();

    static double EntryStepSize;
    static QList<Surr> SurroundingEntryPoints;
    static QList<Surr> InitializeEntrySurrs();
};

class World
{
public:
    World() { _surfaceDistance = -2.0; }

    double GetDTValue(Point3D a, Point3D b) { return _distance.Eval(a.x() + b.x(), a.y() + b.y(), a.z() + b.z()); }
    double GetDTValue(Point3D position) { return _distance.Eval(position.x(), position.y(), position.z()); }

    int PositionIsFree(Point3D position);

    DistanceTransform Distance() { return _distance; }

    double SurfaceDistance() { return _surfaceDistance; }

    void SetDistance(DistanceTransform dt) { _distance = dt; }

private:
    double _surfaceDistance;
    DistanceTransform _distance;
};

class PointAndDT
{
public:
    PointAndDT(Point3D point, double dt) { _point = point; _dt = dt; }
    PointAndDT(Point3D point, World world) { _point = point; _dt = world.GetDTValue(point); }

    static bool LessThan(PointAndDT a, PointAndDT b)
    {
        if (a._dt < b._dt)
            return true;
        if (b._dt < a._dt)
            return false;

        return Point3D::ComparePoints(a.Point(), b.Point());
    }

    Point3D Point() {return _point; }
    double DT() {return _dt; }

    void SetDT(double value) { _dt = value; }

private:
    Point3D _point;
    double _dt;
};

bool operator ==(PointAndDT a, PointAndDT b);

}
#endif // ASTAR_H
