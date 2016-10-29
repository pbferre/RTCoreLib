#include <algorithm>

#include "astar.h"

using namespace Tests;
//using namespace ModelViewer;

Point3D PathFinder::AStar_LastUnimpededZVoxel = Point3D();
QList<Surr> PathFinder::Surrounding = PathFinder::InitializeSurrounding();
QList<Surr> PathFinder::SurroundingEntryPoints = PathFinder::InitializeEntrySurrs();

BRWorld2::BRWorld2(int lWorldX, int lWorldY, int lWorldZ, double zeroX, double zeroY, double zeroZ, double stepSize)
{
    _zeroX = zeroX;
    _zeroY = zeroY;
    _zeroZ = zeroZ;
    _stepSize = stepSize;
    _lWorldX = (int)(lWorldX / stepSize);
    _lWorldY = (int)(lWorldY / stepSize);
    _lWorldZ = (int)(lWorldZ / stepSize);
}

bool BRWorld2::Get(double x, double y, double z)
{
    int xi = (int)((x - _zeroX) / _stepSize + 0.5);
    int yi = (int)((y - _zeroY) / _stepSize + 0.5);
    int zi = (int)((z - _zeroZ) / _stepSize + 0.5);
    if (xi < 0 || xi >= _lWorldX || yi < 0 || yi >= _lWorldY || zi < 0 || zi >= _lWorldZ)
        return true;

    int idx = zi * _lWorldX * _lWorldY + yi * _lWorldX + xi;

    return brWorld.contains(idx);
}

void BRWorld2::Set(double x, double y, double z)
{
    int xi = (int)((x - _zeroX) / _stepSize + 0.5);
    int yi = (int)((y - _zeroY) / _stepSize + 0.5);
    int zi = (int)((z - _zeroZ) / _stepSize + 0.5);
    if (xi < 0 || xi >= _lWorldX || yi < 0 || yi >= _lWorldY || zi < 0 || zi >= _lWorldZ)
        return;

    int idx = zi * _lWorldX * _lWorldY + yi * _lWorldX + xi;

    brWorld.insert(idx);
}

void BRWorld2::Unset(double x, double y, double z)
{
    int xi = (int)((x - _zeroX) / _stepSize + 0.5);
    int yi = (int)((y - _zeroY) / _stepSize + 0.5);
    int zi = (int)((z - _zeroZ) / _stepSize + 0.5);
    if (xi < 0 || xi >= _lWorldX || yi < 0 || yi >= _lWorldY || zi < 0 || zi >= _lWorldZ)
        return;

    int idx = zi * _lWorldX * _lWorldY + yi * _lWorldX + xi;

    brWorld.remove(idx);
}

int World::PositionIsFree(Point3D position)
{
    // outside bounds of distance transform is not an issue, we just won't have mesh distance.
    if (position.x() < 0 || position.y() < 0 || position.z() < 0)
        return 0;
    if (position.z() >= _distance.SizeX() || position.y() >= _distance.SizeY() || position.x() >= _distance.SizeZ())
        return 0;


    double d = GetDTValue(position);

    if (d < _surfaceDistance)
        return 0;
    else return -1;
}

bool SearchNode::Compare(SearchNode* a,  SearchNode* b)
{
    if (a->Cost() < b->Cost())
        return true;
    if (b->Cost() < a->Cost())
        return false;

    if (a->PathCost() < b->PathCost())
        return true;
    if (b->PathCost() < a->PathCost())
        return false;

    return Point3D::ComparePoints(a->Position(), b->Position());
}

void MinHeap::Add(SearchNode *item)
{
    theList.append(item);
    std::sort(theList.begin(), theList.end(), &SearchNode::Compare);
}

SearchNode* MinHeap::ExtractFirst()
{
    if (theList.isEmpty())
        return 0;

    SearchNode* item = theList.at(0);
    theList.removeFirst();
    return item;
}

double Surr::Euclidean(Point3D p)
{
    return sqrt(1 * p.x() * p.x() + 1 * p.y() * p.y() + 1 * p.z() * p.z());
}

double Surr::Sideways(Point3D p)
{
    return sqrt(1.0 * p.x() * p.x() + 0.5 * p.y() * p.y() + 0.5 * p.z() * p.z());
}

double Surr::Downward(Point3D p)
{
    return sqrt(0.5 * p.x() * p.x() + 1.0 * p.y() * p.y() + 1.0 * p.z() * p.z());
}

double Surr::DSquared(Point3D p)
{
    return 1 * p.x() * p.x() + 1 * p.y() * p.y() + 1 * p.z() * p.z();
}

double Surr::Manhattan(Point3D p)
{
    return fabs(1 * p.x()) + fabs(1 * p.y()) + fabs(1 * p.z());
}

double Surr::SupNorm(Point3D p)
{
    return fmax(fmax(fabs(1 * p.x()), fabs(1 * p.y())), fabs(1 * p.z()));
}

bool Tests::operator ==(Surr s1, Surr s2)
{
    return (s1.Point() == s2.Point()) && (s1.Cost() == s2.Cost());
}

SearchNode* PathFinder::FindPath(World world, Point3D start, Point3D end, DistanceTransform* dt, bool unimpededZShortcut, bool adjustableEndpoint)
{
    return FindPathReversed(world, start, end, dt, unimpededZShortcut, adjustableEndpoint);
}

double PathFinder::EndDistance(Point3D p, Point3D end, bool zOnly)
{
    if (zOnly)
        return fabs(p.x() - end.x());
    else
        return Point3D::DistanceTo(p, end);
}

double PathFinder::EndDistanceSquared(Point3D p, Point3D end, bool zOnly)
{
    if (zOnly)
        return (p.x() - end.x()) * (p.x() - end.x());
    else
        return Point3D::DistanceToSquared(p, end);
}

QList<Surr> PathFinder::InitializeSurrounding()
{
    QList<Surr> s;
    s.append(Surr(-1, 2, 2));
    s.append(Surr(-1, 2, 1));
    s.append(Surr(-1, 2, 0));
    s.append(Surr(-1, 2, -1));
    s.append(Surr(-1, 2, -2));

    s.append(Surr(-1, 1, 2));
    s.append(Surr(-1, 1, 1));
    s.append(Surr(-1, 1, 0));
    s.append(Surr(-1, 1, -1));
    s.append(Surr(-1, 1, -2));

    s.append(Surr(-1, 0, 2));
    s.append(Surr(-1, 0, 1));
    s.append(Surr(-1, 0, 0));
    s.append(Surr(-1, 0, -1));
    s.append(Surr(-1, 0, -2));

    s.append(Surr(-1, -1, 2));
    s.append(Surr(-1, -1, 1));
    s.append(Surr(-1, -1, 0));
    s.append(Surr(-1, -1, -1));
    s.append(Surr(-1, -1, -2));

    s.append(Surr(-1, -2, 2));
    s.append(Surr(-1, -2, 1));
    s.append(Surr(-1, -2, 0));
    s.append(Surr(-1, -2, -1));
    s.append(Surr(-1, -2, -2));

    for (int i = 0; i < s.count(); i++)
    {
        Surr surr = s.at(i);
        Point3D p = surr.Point();
        Vector3D v = p.ToVector3D();
        v.normalize();
        v *= StepSize;
        s[i] = Surr(v.ToPoint3D(), &Surr::Euclidean);
    }
    return s;
}

QList<Surr> PathFinder::InitializeEntrySurrs()
{
    QList<Surr> s;
    s.append(Surr(-1, 3, 3));
    s.append(Surr(-1, 3, 0));
    s.append(Surr(-1, 3, -3));

    s.append(Surr(-1, 1, 1));
    s.append(Surr(-1, 1, 0));
    s.append(Surr(-1, 1, -1));

    s.append(Surr(-1, 0, 1));
    s.append(Surr(-1, 0, 3));
    s.append(Surr(-1, 0, 0));
    s.append(Surr(-1, 0, -1));
    s.append(Surr(-1, 0, -3));

    s.append(Surr(-1, -1, 1));
    s.append(Surr(-1, -1, 0));
    s.append(Surr(-1, -1, -1));

    s.append(Surr(-1, -3, 3));
    s.append(Surr(-1, -3, 0));
    s.append(Surr(-1, -3, -3));

    for (int i = 0; i < s.count(); i++)
    {
        Surr surr = s.at(i);
        Point3D p = surr.Point();
        Vector3D v = p.ToVector3D();
        v.normalize();
        v *= EntryStepSize;
        s[i] = Surr(v.ToPoint3D(), &Surr::Downward);
    }
    return s;
}

SearchNode* PathFinder::FindPathReversed(World world, Point3D start, Point3D end, DistanceTransform* dt, bool UnimpededZShortcut, bool adjustableEndpoint)
{
    // rapidly find a starting position if start param is not a suitable voxel
    if (world.PositionIsFree(start) != 0)
    {
        bool foundPath = FindNearestStart(world, start, end, dt);
        if (!foundPath)
            return 0;
        //debug_Mark(start, ModelEntity.internalMaterial, shell);
    }
    if (world.PositionIsFree(end) != 0)
    {
        if (adjustableEndpoint)
        {
            // walk towards start position until a free position is found.
            // this can lead to supports drawn through objects
            bool foundPath = false;
            Vector3D dir = (start - end);
            dir.normalize();
            while (foundPath == false)
            {
                end += dir;
                //end = new Point3D(end.X - 1, end.Y, end.Z);
                foundPath = world.PositionIsFree(end) == 0;

                //debug_Mark(end, ModelEntity.BlackMaterial, shell);
            }
        }
        else
            return 0;
    }

    SearchNode* startNode = new SearchNode(start, 0, 0, 0);

    MinHeap openList;
    openList.Add(startNode);

    // Enlarge voxel set to allow for path options outside distance transform
    auto worldBuffer = 20;

    auto lWorldX = world.Distance().SizeZ() + worldBuffer;
    auto lWorldY = world.Distance().SizeY() + worldBuffer;
    auto lWorldZ = world.Distance().SizeX() + worldBuffer;
    double zeroX = start.x() - floor(start.x());
    double zeroY = start.y() - floor(start.y());
    double zeroZ = start.z() - floor(start.z());

    BRWorld2 brWorld(lWorldX, lWorldY, lWorldZ, zeroX, zeroY, zeroZ, StepSize/3);

    brWorld.Set(start.x() + worldBuffer / 2, start.y() + worldBuffer / 2, start.z() + worldBuffer / 2);

    int pathCount = 0;
    int count = 0;

    // Find path
    while (openList.HasNext() && count < 1e5)
    {

        SearchNode* current = openList.ExtractFirst();

        if (EndDistanceSquared(current->Position(), end, UnimpededZShortcut) <= 2)
        {
            if (UnimpededZShortcut)
            {
                AStar_LastUnimpededZVoxel = current->Position();
                return current;
            }
            return new SearchNode(end, current->PathCost() + 1, current->Cost() + 1, current);
        }

        for (int i = 0; i < Surrounding.count(); i++)
        {
            count++;
            Vector3D p = (Vector3D)Surrounding[i].Point().ToVector3D();
            p.normalize();
            Surr surr = Surrounding.at(i);

            Point3D tmp = Point3D::Add(current->Position(), surr.Point().ToVector3D());
            if (tmp.x() < end.x() && EndDistanceSquared(tmp, end, UnimpededZShortcut) > 2)
                continue;

            try
            {

                if (world.PositionIsFree(tmp) == 0 &&
                        brWorld.Get(tmp.x() + worldBuffer / 2, tmp.y() + worldBuffer / 2, tmp.z() + worldBuffer / 2) == false)
                {

                    brWorld.Set(tmp.x() + worldBuffer / 2, tmp.y() + worldBuffer / 2, tmp.z() + worldBuffer / 2);
                    double pathCost = current->PathCost() + surr.Cost();

                    //double cost = pathCost + s.append((Point3D)(end - tmp)).Cost;
                    double cost;
                    if (UnimpededZShortcut)
                        cost = 0.85 * pathCost + EndDistance(tmp, end, UnimpededZShortcut);
                    else
                        cost = 0.75 * (pathCost - surr.Cost()) + 0.75 * surr.Cost() + Surr::Euclidean((end - tmp).ToPoint3D());
                    SearchNode* node = new SearchNode(tmp, cost, pathCost, current);
                    openList.Add(node);
                }
            }
            catch (...)
            {
                throw;
            }
        }
        pathCount++;
    }
    return 0; //no path found
}

bool PathFinder::FindNearestStart(World world, Point3D start, Point3D end, DistanceTransform* dt)
{
    bool foundPath = false;
    QList<PointAndDT> startingPositions;
    startingPositions.append(PointAndDT(start, world));
    QList<PointAndDT>  testedIndices;
    QList<Surr> verifiedOpenVoxels;
    int minIterations = 25;
    int maxIterations = 500;
    int iterations = 0;
    while (startingPositions.count() > 0 && (iterations < minIterations || (testedIndices.count() < maxIterations && !foundPath)))
    {
        PointAndDT node = startingPositions.first();
        startingPositions.removeFirst();
        testedIndices.append(node);
        std::sort(testedIndices.begin(), testedIndices.end(), &PointAndDT::LessThan);
        for (int i = 0; i < SurroundingEntryPoints.count(); i++)
        {
            Surr surr = SurroundingEntryPoints.at(i);
            Point3D pt = surr.Point();
            Vector3D shift = pt.ToVector3D();
            Point3D p = node.Point() + shift;
            PointAndDT tmp(p, world);
            if (tmp.DT() < world.SurfaceDistance())
                tmp.SetDT(world.SurfaceDistance() + 0.5 * Point3D::DistanceTo(p, start) + 0.4 * Point3D::DistanceTo(p, end));
            else
                tmp.SetDT(0.5 * Point3D::DistanceTo(p, start) + 0.4 * Point3D::DistanceTo(p, end));

            if (!testedIndices.contains(tmp) && !startingPositions.contains(tmp))
            {
                startingPositions.append(tmp);
                std::sort(startingPositions.begin(), startingPositions.end(), &PointAndDT::LessThan);
            }

            if (world.PositionIsFree(tmp.Point()) == 0)
            {
                Surr temp((tmp.Point() - start).ToPoint3D(), &Surr::Downward);
                //var temp = s.append(Surr((Point3D)(tmp.point - start), Surr.Downward);
                temp.SetPoint(tmp.Point());
                temp.SetCost(temp.Cost() + 0.5 * Surr::Euclidean((end - tmp.Point()).ToPoint3D()));
                verifiedOpenVoxels.append(temp);
                std::sort(verifiedOpenVoxels.begin(), verifiedOpenVoxels.end(), &Surr::LessThan);
                foundPath = true;
            }
            else
            {
                //debug_Mark(tmp.point, ModelEntity.sliceMaterial, shell);
            }
        }
        iterations++;
    }
    if (verifiedOpenVoxels.count() > 0)
        start = verifiedOpenVoxels.first().Point();
    return foundPath;
}

bool Tests::operator ==(PointAndDT a, PointAndDT b)
{
    return (a.Point() == b.Point()) && (a.DT() == b.DT());
}

