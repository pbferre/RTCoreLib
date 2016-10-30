#ifndef AABBTREE_H
#define AABBTREE_H

#include <limits>

#include "rect3d.h"
#include "point3d.h"
#include "triangle.h"
#include "support.h"


#include <QList>

//using namespace CreateCore;
//using namespace FullSpectrumEngineering;

namespace CreateCore
{

class LineSeg;
//class MeshGeometry3D;
//class Transform3D;
//class Model3DGroup;

class AABBNode
{
public:
    AABBNode()
    {
        _bb = CreateCore::Rect3D();
        _left = 0;
        _right = 0;
        _triangles.clear();
    }

    AABBNode(CreateCore::Rect3D bb)
    {
        _bb = bb;
        _left = 0;
        _right = 0;
        _triangles.clear();
    }

    AABBNode(CreateCore::Rect3D bb, QList<Triangle> triangles)
    {
        _bb = bb;
        _left = 0;
        _right = 0;
        _triangles = triangles;
    }

    AABBNode(CreateCore::Rect3D bb, AABBNode* left, AABBNode* right)
    {
        _bb = bb;
        _left = left;
        _right = right;
        _triangles.clear();
    }

    ~AABBNode()
    {
        if (_left)
            delete _left;
        if (_right)
            delete _right;
    }

    CreateCore::Rect3D BB() { return _bb; }
    void setBB(CreateCore::Rect3D value) { _bb = value; }

    AABBNode* Left() { return _left; }
    void setLeft(AABBNode* value) { _left = value; }

    AABBNode* Right() { return _right; }
    void setRight(AABBNode* value) { _right = value; }

    QList<Triangle> Triangles() { return _triangles; }
    void setTriangles(QList<Triangle> triangles) { _triangles = triangles; }

    int Height() { return _height; }
    void setHeight(int value) { _height = value; }

    int NumTriangles() { return _numTriangles; }
    void setNumTriangles(int value) { _numTriangles = value; }

    int NumTrianglesLeft() { return _numTrianglesLeft; }
    void setNumTrianglesLeft(int value) { _numTrianglesLeft = value; }

    int NumTrianglesRight() { return _numTrianglesRight; }
    void setNumTrianglesRight(int value) { _numTrianglesRight = value; }

private:
    CreateCore::Rect3D _bb;
    int _height;
    int _numTriangles;
    int _numTrianglesLeft;
    int _numTrianglesRight;
    AABBNode* _left;
    AABBNode* _right;
    QList<Triangle> _triangles;
};

class AABBTree
{
public:
    AABBTree()
    {
        _root = 0;
    }

    AABBNode* Root() { return _root; }
    void setRoot(AABBNode* value) { _root = value; }

    static bool IsInterior(QList<Triangle> ts, Point3D closesPoint, Point3D p);
    static double SignedDist(Rect3D bb, Point3D  p);
    static double BBVolume(Rect3D bb);
    static QList<Triangle> GetTrianglesFromVertices(QList<Point3D> vertices);
    static Rect3D GetBBFromTriangles(QList<Triangle> triangles);
    static Rect3D GetBBFromPoints(QList<Point3D> points);
    static bool Intersect(AABBNode* root1, AABBNode* root2);
    static void BuildTreeRecurse(QList<Triangle> triangles, AABBNode* node);

    void Clear();
    bool Intersect(LineSeg l);
    bool Intersect(LineSeg l, QList<Triangle> ts, QList<Point3D> ps);
    bool Intersect(LineSeg l, AABBNode* root, QList<Triangle> ts, QList<Point3D> ps);
    bool Intersect(LineSeg l, AABBNode* root);
    bool Intersect(AABBTree* tree);
    QList<Triangle> ClosestTriangles(Point3D p, double maxDist = DBL_MAX);
    Point3D ClosestPoint(Point3D p, double maxDist = DBL_MAX);
    bool IsInterior(Point3D p);
    double Distance(Point3D p, double maxDist = DBL_MAX, bool assumePositive = true, bool knownSign = false, bool quickInterior = false);
    void PopulateFromMesh(MeshGeometry3D* m, Transform3D* transform);
    void PopulateFromModel(Model3DGroup* model, Transform3D* transform);

private:
    AABBNode* _root;
};

}

#endif // AABBTREE_H
