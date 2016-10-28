#include <cmath>
#include <cfloat>

#include "lineseg.h"
//#include "rect3d.h"
//#include "vector3d.h"
//#include "point3d.h"
#include "triangle.h"

using namespace ModelViewer;
using namespace CreateCore;

const double SMALL_NUM = 0.00000001;

LineSeg::LineSeg(Point3D p1, Point3D p2)
{
  _p1 = p1;
  _p2 = p2;
}

bool LineSeg::Contains(Point3D p)
{
    Vector3D v1 = p - _p1;
    Vector3D v2 = _p2 - _p1;
    QVector3D cp = Point3D::crossProduct(v1, v2);

    if (cp.lengthSquared() != 0)
        return false;

    if (Point3D::dotProduct(v1, v2) < 0)
        return false;

    if (v1.lengthSquared() > v2.lengthSquared())
        return false;

    return true;
}

Point3D LineSeg::Intersection(Rect3D box)
{
    return Point3D();
}

bool LineSeg::FastIntersect(Rect3D bb)
{
    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    Point3D Dir(_p2.x() - _p1.x(), _p2.y() - _p1.y(), _p2.z() - _p1.z());
    tmin = (bb.X() - _p1.x()) / Dir.x();
    tmax = (bb.X() + bb.SizeX() - _p1.x()) / Dir.x();
    if (Dir.x() < 0)
    {//swap tmin, tmax
        double temp = tmin; tmin = tmax; tmax = temp;
    }

    tymin = (bb.Y() - _p1.y()) / Dir.y();
    tymax = (bb.Y() + bb.SizeY() - _p1.y()) / Dir.y();
    if (Dir.y() < 0)
    {//swap tymin, tymax
        double temp = tymin; tymin = tymax; tymax = temp;
    }

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bb.Z() - _p1.z()) / Dir.z();
    tzmax = (bb.Z() + bb.SizeZ() - _p1.z()) / Dir.z();
    if (Dir.z() < 0)
    {//swap tzmin, tzmax
        double temp = tzmin; tzmin = tzmax; tzmax = temp;
    }

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;
    return ((tmin <= 1) && (tmax >= 0));
}

bool LineSeg::Intersect(Rect3D box)
{
    if (box.Contains(_p1) || box.Contains(_p2))
        return true;

    if ((fmin(_p1.x(), _p2.x()) < box.X()) || (fmin(_p1.y(), _p2.y()) < box.Y()) || (fmin(_p1.z(), _p2.z()) < box.Z()))
        return false;

    if ((fmax(_p1.x(), _p2.x()) > box.X()) || (fmax(_p1.y(), _p2.y()) > box.Y()) || (fmax(_p1.z(), _p2.z()) > box.Z()))
        return false;

    return FastIntersect(box);
}

bool LineSeg::Intersect(Triangle t, Point3D p)
{
    Vector3D dir(_p2.x() - _p1.x(), _p2.y() - _p1.y(), _p2.z() - _p1.z());
    double d;
    bool lineIntersect = ComputeLineTriangleIntersection(true, _p1, dir, t.V1(), t.V2(), t.V3(), p, d);
    if (lineIntersect && d >= 0 && d <= 1)
        return true;
    else
        return false;

    return false;
}

bool LineSeg::Intersect(Triangle t)
{
    Vector3D dir = _p2 - _p1;
    Point3D P;
    bool lineIntersect = intersect3D_RayTriangle(_p1, dir, t.V1(), t.V2(), t.V3(), P);
    if (lineIntersect)
    {
        double dot = Vector3D::DotProduct(P - _p1, dir);
        if (dot > 0 && dot < dir.lengthSquared())
            return true;
    }
    return false;
}

bool LineSeg::ComputeLineTriangleIntersection(bool FaceBoth, Point3D &origin, Vector3D &direction, Point3D &v0,
                                              Point3D &v1, Point3D &v2, Point3D &hitCoord, double &dist)
{
    Vector3D e1;
    Point3D::Subtract(v1, v0, e1);

    Vector3D e2;
    Point3D::Subtract(v2, v0, e2);

    Vector3D r;
    Vector3D::CrossProduct(direction, e2, r);

    double a = Vector3D::DotProduct(e1, r);

    Vector3D s;
    if (a > 0 && FaceBoth)
    {
        Point3D::Subtract(origin, v0, s);
    }
    else if (a < 0 && FaceBoth)
    {
        Point3D::Subtract(v0, origin, s);
        a = -a;
    }
    else
    {
        hitCoord = Point3D();
        dist = 0;
        return false;
    }

    double u = Vector3D::DotProduct(s, r);
    if ((u < 0) || (a < u))
    {
        hitCoord = Point3D();
        dist = 0;
        return false;
    }

    Vector3D q;
    Vector3D::CrossProduct(s, e1, q);

    double v = Vector3D::DotProduct(direction, q);
    if ((v < 0) || (a < (u + v)))
    {
        hitCoord = Point3D();
        dist = 0;
        return false;
    }

    double t = Vector3D::DotProduct(e2, q);
    double f = 1 / a;

    t = t * f;
    u = u * f;
    v = v * f;
    double hitX = v0.x() + u * (v1.x() - v0.x()) + v * (v2.x() - v0.x());
    double hitY = v0.y() + u * (v1.y() - v0.y()) + v * (v2.y() - v0.y());
    double hitZ = v0.z() + u * (v1.z() - v0.z()) + v * (v2.z() - v0.z());
    hitCoord = Point3D(hitX, hitY, hitZ);
    dist = t;

    return true;
}

bool LineSeg::intersect3D_RayTriangle(Point3D &origin, Vector3D &direction, Point3D &v0, Point3D &v1, Point3D &v2, Point3D &P)
{
    Vector3D u, v, n;              // triangle vectors
    Vector3D dir, w0, w;           // ray vectors
    double r, a, b;              // params to calc ray-plane intersect
    P = Point3D();

    // get triangle edge vectors and plane normal
    u = v1 - v0;
    v = v2 - v0;
    n = Vector3D::CrossProduct(u, v);              // cross product
    if (n == Vector3D::Empty())             // triangle is degenerate
        return false;                  // do not deal with this case

    dir = direction;              // ray direction vector
    w0 = origin - v0;
    a = Vector3D::DotProduct(n, w0) * -1;
    b = Vector3D::DotProduct(n, dir);
    if (fabs(b) < SMALL_NUM)
    {     // ray is  parallel to triangle plane
        if (a == 0)                 // ray lies in triangle plane
            return true;
        else
            return false;              // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                    // ray goes away from triangle
        return false;                   // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    P = origin + r * dir;            // intersect point of ray and plane

    // is I inside T?
    double uu, uv, vv, wu, wv, D;
    uu = Vector3D::DotProduct(u, u);
    uv = Vector3D::DotProduct(u, v);
    vv = Vector3D::DotProduct(v, v);
    w = P - v0;
    wu = Vector3D::DotProduct(w, u);
    wv = Vector3D::DotProduct(w, v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    double s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // I is outside T
        return false;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return false;

    return true;
}
