#ifndef SUPPORT_H
#define SUPPORT_H

#include <QtCore>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "point3d.h"
#include "vector3d.h"
#include "rect3d.h"
#include "matrix3d.h"
#include "cylindercollision.h"

//using namespace CreateCore;

namespace ublas = boost::numeric::ublas;

namespace CreateCore
{

enum SupportOperationType
{
    sotElevateNoBase = 0,
    sotSingleBaseWithSupports,
    sotSingleBaseNoSupports,
    sotBasePerSupport,
    sotNone
};


struct TransformState
{
    double SupportOffsetX;
    double SupportOffsetY;
};


class MeshGeometry3D
{
public:
    QList<int> TriangleIndices;
    QList<Point3D> Positions;
    QList<Vector3D> Normals;
    QList<QPointF> TextureCoordinates;
};

class Transform3D
{
public:
    double OffsetX;
    double OffsetY;

    Transform3D::Transform3D()
    {
        OffsetX = 0;
        OffsetY = 0;
    }

    Rect3D TransformBounds(Rect3D bounds)
    {
        // TODO
        return bounds;
    }

    Transform3D CloneCurrentValue()
    {
        Transform3D t;
        t.OffsetX = this->OffsetX;
        t.OffsetY = this->OffsetY;
        return t;
    }

    Point3D Transform(Point3D p)
    {
        return p;
    }

    Vector3D Transform(Vector3D v)
    {
        return v;
    }

    static Transform3D* Identity()
    {
        return new Transform3D();
    }

    bool operator ==(Transform3D &t);
};

class Transform3DGroup : public Transform3D
{
public:
    QList<Transform3D> Children;

    void Transform(Point3D p)
    {

    }

    bool operator ==(Transform3DGroup &tg);
};

class Visual3D
{
public:
    Transform3DGroup Transform;

    bool operator ==(Visual3D & v);
};

class Model3D
{
public:
    virtual Rect3D Bounds()
    {
        // TODO
        return Rect3D();
    }

    int Content;
    Transform3D Transform;
};

class Model3DGroup : public Model3D
{
public:
    QList<Model3D*> Children;
};

class GeometryModel3D : public Model3D
{
public:
    MeshGeometry3D* Geometry;

};

class ModelVisual3D : public Visual3D
{
public:
    Transform3D* Transform;
    Model3D* Content;
};

class Vertex
{
public:
    double* Position;
    Vertex(Point3D point)
    {
        Position = new double[3] { point.x(), point.y(), point.z() };
    }
};

class ModelVisual3DObservable : public ModelVisual3D
{
public:

    GeometryModel3D* GetGeometryModel()
    {
        Model3DGroup* group = dynamic_cast<Model3DGroup*>(this->Content);
        return dynamic_cast<GeometryModel3D*>(group->Children.at(0));
    }
};

class RayHitResult
{
public:
    RayHitResult()
    {
        _height = 0;
        _distanceRayOrigin = 0;
        _vertexIndex1 = 0;
        _vertexIndex2 = 0;
        _vertexIndex3 = 0;
        _pointHit = Point3D();
        _meshHit = new MeshGeometry3D();
    }

    RayHitResult(double height)
    {
        _height = height;
        _distanceRayOrigin = 0;
        _vertexIndex1 = 0;
        _vertexIndex2 = 0;
        _vertexIndex3 = 0;
        _pointHit = Point3D();
        _meshHit = new MeshGeometry3D();
    }

    RayHitResult(MeshGeometry3D* meshHit, Point3D pointHit, double distanceToRayOrigin,
                 int vertexIndex1, int vertexIndex2, int vertexIndex3)
    {

        _height = 0;
        _distanceRayOrigin = distanceToRayOrigin;
        _vertexIndex1 = vertexIndex1;
        _vertexIndex2 = vertexIndex2;
        _vertexIndex3 = vertexIndex3;
        _pointHit = pointHit;
        _meshHit = meshHit;


    }

    double Height() { return _height; }
    void setHeight(double value) { _height = value; }

    Point3D PointHit() { return Point3D(_pointHit.x(), _pointHit.y(), _pointHit.z()); }
    double DistanceToRayOrigin() { return _distanceRayOrigin; }
    int VertexIndex1() { return _vertexIndex1; }
    int VertexIndex2() { return _vertexIndex2; }
    int VertexIndex3() { return _vertexIndex3; }
    MeshGeometry3D* MeshHit() { return _meshHit; }

private:
    double _height;
    double _distanceRayOrigin;
    int _vertexIndex1, _vertexIndex2, _vertexIndex3;
    Point3D _pointHit;
    MeshGeometry3D* _meshHit;
};

struct SupportDataSegment
{
    Point3D point;
    Cylinder cylinder;

    SupportDataSegment(Point3D p)
    {
        point = p;
    }
};

struct SupportData
{
    QList<std::tuple<QPointF, QList<RayHitResult*>>> map;
    QList<QList<SupportDataSegment>> supportDataSegments;
};

class Rotation3D
{

};

class AxisAngleRotation3D : public Rotation3D
{
public:
    AxisAngleRotation3D(Vector3D axis, double length)
    {

    }
};


class RotateTransform3D : public Transform3D
{

public:
    Matrix3D Value;
    RotateTransform3D(AxisAngleRotation3D q, Point3D p)
    {

    }
};

/* Matrix inversion routine.
    Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
 template<class T>
 bool InvertMatrix (const ublas::matrix<T>& input, ublas::matrix<T>& inverse) {
    using namespace boost::numeric::ublas;
    typedef permutation_matrix<std::size_t> pmatrix;
    // create a working copy of the input
    matrix<T> A(input);
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());
    // perform LU-factorization
    int res = lu_factorize(A,pm);
        if( res != 0 ) return false;
    // create identity matrix of "inverse"
    inverse.assign(ublas::identity_matrix<T>(A.size1()));
    // backsubstitute to get the inverse
    lu_substitute(A, pm, inverse);
    return true;
 }

 //bool operator ==(Transform3D t1, Transform3D t2);
 //bool operator ==(Transform3DGroup t1, Transform3DGroup t2);
 //bool operator ==(Visual3D v1, Visual3D v2);

}

#endif // SUPPORT_H
