#include <cmath>
#include <tuple>

#include <QStack>

#include "aabbtree.h"

#include "point3d.h"

using namespace ModelViewer;
using namespace CreateCore;
using namespace FullSpectrumEngineering;

bool AABBTree::IsInterior(QList<Triangle> ts, Point3D closestPoint, Point3D p)
{
    if (ts.count() == 0)
        return true;

    Vector3D normal;
    if (ts.count() == 1)
        normal = ts[0].GetNormal();
    else if (ts.count() < 3)
        foreach (Triangle t, ts)
        {
            Vector3D tNormal = t.GetNormal();
            tNormal.normalize();
            normal += tNormal;
        }
    else
        foreach (Triangle t, ts)
        {
            double alpha = t.GetIncidentAngle(closestPoint);
            Vector3D tNormal = t.GetNormal();
            tNormal.normalize();
            normal += alpha * tNormal;
        }

    double dot = Vector3D::DotProduct(normal, p - closestPoint);
    return dot < 0;
}

//returns the signed distance between a point and a bb.
//uses the supnorm distance metric. negative if point is interior.
//Note: euclideanDist / sqrt(3) <= supnormDist <= euclideanDist.
//Also note: in the interior (and sometimes in the exterior) supnormDist==euclideanDist.
double AABBTree::SignedDist(Rect3D bb, Point3D p)
{
    double dist = bb.X() - p.x();
    dist = fmax(dist, bb.Y() - p.y());
    dist = fmax(dist, bb.Z() - p.z());
    dist = fmax(dist, p.x() - (bb.X() + bb.SizeX()));
    dist = fmax(dist, p.y() - (bb.Y() + bb.SizeY()));
    dist = fmax(dist, p.z() - (bb.Z() + bb.SizeZ()));

    return dist;
}

double AABBTree::BBVolume(Rect3D bb)
{
    return bb.SizeX() * bb.SizeY() * bb.SizeZ();
}

QList<Triangle> AABBTree::GetTrianglesFromVertices(QList<Point3D> vertices)
{
    QList<Triangle> triangles;
    for (int i = 0; i < vertices.count() / 3; i++)
    {
        Triangle t(vertices.at(3 * i), vertices.at(3 * i + 1), vertices.at(3 * i + 2));
        triangles.append(t);
    }

    return triangles;
}

Rect3D AABBTree::GetBBFromTriangles(QList<Triangle> triangles)
{
    double minx, miny, minz;
    minx = miny = minz = DBL_MAX;
    double maxx, maxy, maxz;
    maxx = maxy = maxz = DBL_MIN;
    foreach (Triangle t, triangles)
    {
        minx = fmin(fmin(fmin(minx, t.V1().x()), t.V2().x()), t.V3().x());
        miny = fmin(fmin(fmin(miny, t.V1().y()), t.V2().y()), t.V3().y());
        minz = fmin(fmin(fmin(minz, t.V1().z()), t.V2().z()), t.V3().z());
        maxx = fmax(fmax(fmax(maxx, t.V1().x()), t.V2().x()), t.V3().x());
        maxy = fmax(fmax(fmax(maxy, t.V1().y()), t.V2().y()), t.V3().y());
        maxz = fmax(fmax(fmax(maxz, t.V1().z()), t.V2().z()), t.V3().z());
    }

    return Rect3D(minx, miny, minz, maxx - minx, maxy - miny, maxz - minz);
}

Rect3D AABBTree::GetBBFromPoints(QList<Point3D> points)
{
    double minx, miny, minz;
    minx = miny = minz = DBL_MAX;
    double maxx, maxy, maxz;
    maxx = maxy = maxz = DBL_MIN;
    foreach (Point3D p, points)
    {
        minx = fmin(minx, p.x());
        miny = fmin(miny, p.y());
        minz = fmin(minz, p.z());
        maxx = fmax(maxx, p.x());
        maxy = fmax(maxy, p.y());
        maxz = fmax(maxz, p.z());
    }
    return Rect3D(minx, miny, minz, maxx - minx, maxy - miny, maxz - minz);
}

bool AABBTree::Intersect(AABBNode *root1, AABBNode *root2)
{
    QStack<std::tuple<AABBNode*, AABBNode*>> workStack;
    workStack.push(std::make_tuple(root1, root2));

    int totalTestedTriangles = 0;
    while (workStack.count() > 0)
    {
        std::tuple<AABBNode*, AABBNode*> level = workStack.pop();
        AABBNode* n1 = std::get<0>(level);
        AABBNode* n2 = std::get<1>(level);

        if (n1->BB().IntersectsWith(n2->BB()))
        {
            if (n1->Height() > 1 && n2->Height() > 1)
            {
                // branch into the BB with the larger volume
                if (BBVolume(n1->BB()) > BBVolume(n2->BB()))
                {
                    workStack.push(std::make_tuple(n1->Left(), n2));
                    workStack.push(std::make_tuple(n1->Right(), n2));
                }
                else
                {
                    workStack.push(std::make_tuple(n1, n2->Left()));
                    workStack.push(std::make_tuple(n1, n2->Right()));
                }
            }
            else if (n1->Height() > 1)
            {
                workStack.push(std::make_tuple(n1->Left(), n2));
                workStack.push(std::make_tuple(n1->Right(), n2));
            }
            else if (n2->Height() > 1)
            {
                workStack.push(std::make_tuple(n1, n2->Left()));
                workStack.push(std::make_tuple(n1, n2->Right()));
            }
            else
            {
                foreach (Triangle t1, n1->Triangles())
                {
                    foreach (Triangle t2, n2->Triangles())
                    {
                        totalTestedTriangles++;
                        if (t1.Intersect(t2))
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

void AABBTree::BuildTreeRecurse(QList<Triangle> triangles, AABBNode *node)
{
    Rect3D bb = GetBBFromTriangles(triangles);
    QList<Triangle> leftTriangles;
    QList<Triangle> rightTriangles;

    node->setNumTriangles(triangles.count());

    if (bb.SizeX() >= bb.SizeY() && bb.SizeX() >= bb.SizeZ())
    {
        double splitSum = 0;
        for (int i = 0; i < triangles.count(); i++)
        {
            Triangle t = triangles.at(i);
            splitSum += t.Midpoint().x();
            //splitSum += triangles.at(i).Midpoint().x();
        }

        double splitVal = splitSum / triangles.count();
        for (int i = 0; i < triangles.count(); i++)
        {
            Triangle t = triangles.at(i);
            if (t.Midpoint().x() < splitVal)
                leftTriangles.append(triangles.at(i));
            else
                rightTriangles.append(triangles.at(i));
        }
    }
    else if (bb.SizeY() >= bb.SizeX() && bb.SizeY() >= bb.SizeZ())
    {
        double splitSum = 0;
        for (int i = 0; i < triangles.count(); i++)
        {
            Triangle t = triangles.at(i);
            splitSum += t.Midpoint().y();
        }

        double splitVal = splitSum / triangles.count();
        for (int i = 0; i < triangles.count(); i++)
        {
            Triangle t = triangles.at(i);
            if (t.Midpoint().y() < splitVal)
                leftTriangles.append(triangles.at(i));
            else
                rightTriangles.append(triangles.at(i));
        }
    }
    else //SizeZ is biggest
    {
        double splitSum = 0;
        for (int i = 0; i < triangles.count(); i++)
        {
            Triangle t = triangles.at(i);
            splitSum += t.Midpoint().z();
        }

        double splitVal = splitSum / triangles.count();
        for (int i = 0; i < triangles.count(); i++)
        {
            Triangle t = triangles.at(i);
            if (t.Midpoint().z() < splitVal)
                leftTriangles.append(triangles.at(i));
            else
                rightTriangles.append(triangles.at(i));
        }
    }

    triangles.clear();

    node->BB() = bb;
    if (leftTriangles.isEmpty())
    {//leaf node
        node->setTriangles(rightTriangles);
        node->setHeight(1);
        node->setNumTrianglesLeft(0);
        node->setNumTrianglesRight(0);
    }
    else if (rightTriangles.isEmpty())
    {//leaf node
        node->setTriangles(leftTriangles);
        node->setHeight(1);
        node->setNumTrianglesLeft(0);
        node->setNumTrianglesRight(0);
    }
    else
    {//split and recurse
        node->setNumTrianglesLeft(leftTriangles.count());
        node->setNumTrianglesRight(rightTriangles.count());
        node->setLeft(new AABBNode());
        BuildTreeRecurse(leftTriangles, node->Left());
        node->setRight(new AABBNode());
        BuildTreeRecurse(rightTriangles, node->Right());
        node->setHeight(fmax(node->Right()->Height(), node->Left()->Height()) + 1);
    }
}

void AABBTree::Clear()
{
    if (_root)
        delete _root;

    _root = 0;
}

bool AABBTree::Intersect(LineSeg l)
{
    return Intersect(l, _root);
}

bool AABBTree::Intersect(LineSeg l, QList<Triangle> ts, QList<Point3D> ps)
{
    return Intersect(l, _root, ts, ps);
}

bool AABBTree::IsInterior(Point3D p)
{
    //minimize risk of geometric alignment by shooting off at strange angle
    Point3D exit(_root->BB().X() - 2.5, _root->BB().Y() - 3.02, _root->BB().Z() - 5.003);
    LineSeg ray(p, exit);
    QList<Triangle> triangles;
    QList<Point3D> points;
    Intersect(ray, triangles, points);
    int entrances = 0;
    int exits = 0;
    for (int i = 0; i < triangles.count(); i++)
    {
        Triangle t = triangles.at(i);
        Vector3D normal = t.GetNormal();
        Vector3D dir = p - points.at(i);
        if (Vector3D::DotProduct(normal, dir) > 0)
            exits++;
        else
            entrances++;
    }
    return (entrances > exits);
}

QList<Triangle> AABBTree::ClosestTriangles(Point3D p, double maxDist)
{
    double epsilon = 1e-9;
    QStack<AABBNode*> workStack;
    QList<Triangle> closestTriangles;
    Point3D closestPoint;
    workStack.push(_root);

    //depth first tree parsing. Updates maxDist to skip more bounding boxes as better points are found.
    while (workStack.count() > 0)
    {
        AABBNode* node = workStack.pop();
        if (node->Triangles().isEmpty() == false)
            foreach (Triangle t, node->Triangles())
            {
                Point3D closestP;
                double d = t.Distance(p, closestP);
                //totalTested++;

                //if multiple triangles tie for closest, it means the nearest point is an
                //edge or a vertex. Store all these triangles to test if point is interior.
                if (d - maxDist < -epsilon)
                {
                    maxDist = d;
                    closestTriangles.clear();
                    closestTriangles.append(t);
                    closestPoint = closestP;
                }
                else if (d - maxDist < epsilon)
                {
                    Vector3D v = closestP - closestPoint;
                    if (v.lengthSquared() < epsilon)
                        closestTriangles.append(t);
                }
                //else
                //    Debugger.Break();
            }
        else
        {
            //distance to each bounding box. Distance is underestimated here to eliminate square roots.
            double leftDist = SignedDist(node->Left()->BB(), p);
            double rightDist = SignedDist(node->Right()->BB(), p);
            if (leftDist < maxDist)
            {
                if (rightDist < maxDist)
                {
                    //add the closer of the two nodes second so it gets popped next.
                    if (leftDist < rightDist)
                    {
                        workStack.push(node->Right());
                        workStack.push(node->Left());
                    }
                    else
                    {
                        workStack.push(node->Left());
                        workStack.push(node->Right());
                    }
                }
                else
                    workStack.push(node->Left());
            }
            else
                if (rightDist < maxDist)
                    workStack.push(node->Right());
        }
    }

    return closestTriangles;
}

Point3D AABBTree::ClosestPoint(Point3D p, double maxDist)
{
    double epsilon = 1e-9;
    QStack<AABBNode*> workStack;
    Point3D closestPoint;
    workStack.push(_root);

    //depth first tree parsing. Updates maxDist to skip more bounding boxes as better points are found.
    while (workStack.count() > 0)
    {
        AABBNode* node = workStack.pop();
        if (node->Triangles().isEmpty() == false)
            foreach (Triangle t, node->Triangles())
            {
                Point3D closestP;
                double d = t.Distance(p, closestP);

                //if multiple triangles tie for closest, it means the nearest point is an
                //edge or a vertex. Store all these triangles to test if point is interior.
                if (d - maxDist < -epsilon)
                {
                    maxDist = d;
                    closestPoint = closestP;
                }
            }
        else
        {
            //distance to each bounding box. Distance is underestimated here to eliminate square roots.
            double leftDist = SignedDist(node->Left()->BB(), p);
            double rightDist = SignedDist(node->Right()->BB(), p);
            if (leftDist < maxDist)
            {
                if (rightDist < maxDist)
                {
                    //add the closer of the two nodes second so it gets popped next.
                    if (leftDist < rightDist)
                    {
                        workStack.push(node->Right());
                        workStack.push(node->Left());
                    }
                    else
                    {
                        workStack.push(node->Left());
                        workStack.push(node->Right());
                    }
                }
                else
                    workStack.push(node->Left());
            }
            else
                if (rightDist < maxDist)
                    workStack.push(node->Right());
        }
    }
    return closestPoint;
}

double AABBTree::Distance(Point3D p, double maxDist, bool assumePositive, bool knownSign, bool quickInterior)
{
    double epsilon = 1e-9;
    QStack<AABBNode*> workStack;
    QList<Triangle> closestTriangles;
    Point3D closestPoint;
    workStack.push(_root);

    //depth first tree parsing. Updates maxDist to skip more bounding boxes as better points are found.
    while (workStack.count() > 0)
    {
        AABBNode* node = workStack.pop();
        if (node->Triangles().isEmpty() == false)
            foreach (Triangle t, node->Triangles())
            {
                Point3D closestP;
                double d = t.Distance(p, closestP);

                //if multiple triangles tie for closest, it means the nearest point is an
                //edge or a vertex. Store all these triangles to test if point is interior.
                if (d - maxDist < -epsilon)
                {
                    maxDist = d;
                    closestTriangles.clear();
                    closestTriangles.append(t);
                    closestPoint = closestP;
                }
                else if (d - maxDist < epsilon)
                {
                    Vector3D v = closestP - closestPoint;
                    if (v.lengthSquared() < epsilon)
                        closestTriangles.append(t);
                }
            }
        else
        {
            //distance to each bounding box. Distance is underestimated here to eliminate square roots.
            double leftDist = SignedDist(node->Left()->BB(), p);
            double rightDist = SignedDist(node->Right()->BB(), p);
            if (leftDist < maxDist)
            {
                if (rightDist < maxDist)
                {
                    //add the closer of the two nodes second so it gets popped next.
                    if (leftDist < rightDist)
                    {
                        workStack.push(node->Right());
                        workStack.push(node->Left());
                    }
                    else
                    {
                        workStack.push(node->Left());
                        workStack.push(node->Right());
                    }
                }
                else
                    workStack.push(node->Left());
            }
            else
                if (rightDist < maxDist)
                    workStack.push(node->Right());
        }
    }

    //make dist negative if p is an interior point.
    if (closestTriangles.count() == 0 || knownSign)
    {
        if (!assumePositive)
            maxDist *= -1;
    }
    else
    {
        bool interior;
        if (quickInterior)
            interior = IsInterior(closestTriangles, closestPoint, p);
        else
            interior = IsInterior(p);
        if (interior)
            maxDist *= -1;
    }

    return maxDist;
}

bool AABBTree::Intersect(LineSeg l, AABBNode *root, QList<Triangle> ts, QList<Point3D> ps)
{
    QStack<AABBNode*> workStack;
    workStack.push(root);
    int numIntersections = 0;

    while (workStack.count() > 0)
    {
        AABBNode* n = workStack.pop();

        if (l.Intersect(n->BB()))
        {
            if (n->Triangles().count() > 0)
            {
                for (int i = 0; i < n->Triangles().count(); i++)
                {
                    Point3D p;
                    if (l.Intersect(n->Triangles().at(i), p))
                    {
                        ts.append(n->Triangles().at(i));
                        ps.append(p);
                        numIntersections++;
                    }
                }
            }
            else
            {
                workStack.push(n->Left());
                workStack.push(n->Right());
            }
        }
    }

    return numIntersections > 0;
}

bool AABBTree::Intersect(LineSeg l, AABBNode *root)
{
    QStack<AABBNode*> workStack;
    workStack.push(root);

    while (workStack.count() > 0)
    {
        AABBNode* n = workStack.pop();

        if (l.Intersect(n->BB()))
        {
            if (n->Triangles().count() > 0)
            {
                for (int i = 0; i < n->Triangles().count(); i++)
                {
                    Point3D p;
                    if (l.Intersect(n->Triangles().at(i), p))
                    {
                        return true;
                    }
                }
            }
            else
            {
                workStack.push(n->Left());
                workStack.push(n->Right());
            }
        }
    }
    return false;
}

bool AABBTree::Intersect(AABBTree tree)
{
    return Intersect(_root, tree.Root());
}

void AABBTree::PopulateFromMesh(MeshGeometry3D m, Transform3D* transform)
{
    QList<Triangle> triangles;
    QList<Point3D> pos = m.Positions;
    QList<int> tri = m.TriangleIndices;

    for (int i = 0; i < pos.count(); i++)
        tri.append(i);

    //fill triangles from model data
    for (int i = 0; i < tri.count() / 3 ; i++)
    {
        QList<Point3D> vertices;
        double minx, miny, minz;
        minx = miny = minz = DBL_MAX;
        double maxx, maxy, maxz;
        maxx = maxy = maxz = DBL_MIN;
        for (int j = 0; j < 3; j++)
        {
            int n = 3 * i + j;
            Point3D p = pos [tri [n]];
            if (transform)
                p = transform->Transform(p);
            vertices.append(p);
            minx = fmin(minx, p.x());
            miny = fmin(miny, p.y());
            minz = fmin(minz, p.z());
            maxx = fmax(maxx, p.x());
            maxy = fmax(maxy, p.y());
            maxz = fmax(maxz, p.z());
        }
        Point3D midpoint(0.5 * (minx + maxx), 0.5 * (miny + maxy), 0.5 * (minz + maxz));
        triangles.append(Triangle(vertices[0], vertices[1], vertices[2], midpoint, i));
    }

    _root = new AABBNode();
    BuildTreeRecurse(triangles, _root);
}

void AABBTree::PopulateFromModel(Model3DGroup model, Transform3D transform)
{
    Model3D m = model.Children.at(0);
    Model3D* mptr = &m;
    GeometryModel3D*gmptr = dynamic_cast<GeometryModel3D*>(mptr);
    GeometryModel3D gm = *gmptr;
    MeshGeometry3D mesh = gm.Geometry;
    Transform3D* t = &transform;
    PopulateFromMesh(mesh, t);
}
