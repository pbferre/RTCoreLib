#include <cmath>

#include "triangle.h"
//#include "vector3d.h"

//using namespace ModelViewer;
using namespace CreateCore;

Triangle::Triangle()
{
    _V1 = Point3D();
    _V2 = Point3D();
    _V3 = Point3D();
    _midpoint = Point3D();
    _index = 0;
}

Triangle::Triangle(Point3D v1, Point3D v2, Point3D v3)
{
    _V1 = v1;
    _V2 = v2;
    _V3 = v3;
}

Triangle::Triangle(Point3D v1, Point3D v2, Point3D v3, Point3D mid, int index)
{
    _V1 = v1;
    _V2 = v2;
    _V3 = v3;
    _midpoint = mid;
    _index = index;
}

double Triangle::Distance(Point3D p, Point3D &closestPoint)
{
    return sqrt(DistanceSquared(p, closestPoint));
}

double Triangle::DistanceSquared(Point3D p, Point3D &closestPoint)
{
    Vector3D diff = _V1 - p;
    Vector3D edge0 = _V2 - _V1;
    Vector3D edge1 = _V3 - _V1;
    double a00 = edge0.lengthSquared();
    double a01 = Point3D::dotProduct(edge0, edge1);
    double a11 = edge1.lengthSquared();
    double b0 = Point3D::dotProduct(diff, edge0);
    double b1 = Point3D::dotProduct(diff, edge1);
    double c = diff.lengthSquared();
    double det = fabs(a00 * a11 - a01 * a01);
    double s = a01 * b1 - a11 * b0;
    double t = a01 * b0 - a00 * b1;
    double sqrDistance;

    if (s + t <= det)
    {
        if (s < 0)
        {
            if (t < 0)  // region 4
            {
                if (b0 < 0)
                {
                    t = 0;
                    if (-b0 >= a00)
                    {
                        s = 1;
                        sqrDistance = a00 + (2) * b0 + c;
                    }
                    else
                    {
                        s = -b0 / a00;
                        sqrDistance = b0 * s + c;
                    }
                }
                else
                {
                    s = 0;
                    if (b1 >= 0)
                    {
                        t = 0;
                        sqrDistance = c;
                    }
                    else if (-b1 >= a11)
                    {
                        t = 1;
                        sqrDistance = a11 + (2) * b1 + c;
                    }
                    else
                    {
                        t = -b1 / a11;
                        sqrDistance = b1 * t + c;
                    }
                }
            }
            else  // region 3
            {
                s = 0;
                if (b1 >= 0)
                {
                    t = 0;
                    sqrDistance = c;
                }
                else if (-b1 >= a11)
                {
                    t = 1;
                    sqrDistance = a11 + (2) * b1 + c;
                }
                else
                {
                    t = -b1 / a11;
                    sqrDistance = b1 * t + c;
                }
            }
        }
        else if (t < 0)  // region 5
        {
            t = 0;
            if (b0 >= 0)
            {
                s = 0;
                sqrDistance = c;
            }
            else if (-b0 >= a00)
            {
                s = 1;
                sqrDistance = a00 + (2) * b0 + c;
            }
            else
            {
                s = -b0 / a00;
                sqrDistance = b0 * s + c;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            double invDet = (1) / det;
            s *= invDet;
            t *= invDet;
            sqrDistance = s * (a00 * s + a01 * t + (2) * b0) +
                    t * (a01 * s + a11 * t + (2) * b1) + c;
        }
    }
    else
    {
        double tmp0, tmp1, numer, denom;

        if (s < 0)  // region 2
        {
            tmp0 = a01 + b0;
            tmp1 = a11 + b1;
            if (tmp1 > tmp0)
            {
                numer = tmp1 - tmp0;
                denom = a00 - (2) * a01 + a11;
                if (numer >= denom)
                {
                    s = 1;
                    t = 0;
                    sqrDistance = a00 + (2) * b0 + c;
                }
                else
                {
                    s = numer / denom;
                    t = 1 - s;
                    sqrDistance = s * (a00 * s + a01 * t + (2) * b0) +
                            t * (a01 * s + a11 * t + (2) * b1) + c;
                }
            }
            else
            {
                s = 0;
                if (tmp1 <= 0)
                {
                    t = 1;
                    sqrDistance = a11 + (2) * b1 + c;
                }
                else if (b1 >= 0)
                {
                    t = 0;
                    sqrDistance = c;
                }
                else
                {
                    t = -b1 / a11;
                    sqrDistance = b1 * t + c;
                }
            }
        }
        else if (t < 0)  // region 6
        {
            tmp0 = a01 + b1;
            tmp1 = a00 + b0;
            if (tmp1 > tmp0)
            {
                numer = tmp1 - tmp0;
                denom = a00 - (2) * a01 + a11;
                if (numer >= denom)
                {
                    t = 1;
                    s = 0;
                    sqrDistance = a11 + (2) * b1 + c;
                }
                else
                {
                    t = numer / denom;
                    s = 1 - t;
                    sqrDistance = s * (a00 * s + a01 * t + (2) * b0) +
                            t * (a01 * s + a11 * t + (2) * b1) + c;
                }
            }
            else
            {
                t = 0;
                if (tmp1 <= 0)
                {
                    s = 1;
                    sqrDistance = a00 + (2) * b0 + c;
                }
                else if (b0 >= 0)
                {
                    s = 0;
                    sqrDistance = c;
                }
                else
                {
                    s = -b0 / a00;
                    sqrDistance = b0 * s + c;
                }
            }
        }
        else  // region 1
        {
            numer = a11 + b1 - a01 - b0;
            if (numer <= 0)
            {
                s = 0;
                t = 1;
                sqrDistance = a11 + (2) * b1 + c;
            }
            else
            {
                denom = a00 - (2) * a01 + a11;
                if (numer >= denom)
                {
                    s = 1;
                    t = 0;
                    sqrDistance = a00 + (2) * b0 + c;
                }
                else
                {
                    s = numer / denom;
                    t = 1 - s;
                    sqrDistance = s * (a00 * s + a01 * t + (2) * b0) +
                            t * (a01 * s + a11 * t + (2) * b1) + c;
                }
            }
        }
    }

    // Account for numerical round-off error.
    if (sqrDistance < 0)
    {
        sqrDistance = 0;
    }

    closestPoint = _V1 + s * edge0 + t * edge1;
    return sqrDistance;
}

double Triangle::GetIncidentAngle(Point3D p)
{
    Vector3D v1, v2, v3;
    v1 = p - _V1;
    v2 = p - _V2;
    v3 = p - _V3;
    double angleBetween = 0;
    if (v1.lengthSquared() < v2.lengthSquared() && v1.lengthSquared() < v3.lengthSquared())
        angleBetween = Vector3D::AngleBetween(v2, v3);
    else if (v2.lengthSquared() < v1.lengthSquared() && v2.lengthSquared() < v3.lengthSquared())
        angleBetween = Vector3D::AngleBetween(v1, v3);
    else
        angleBetween = Vector3D::AngleBetween(v1, v2);

    return angleBetween;
}

Vector3D Triangle::GetNormal()
{
    Vector3D u = _V2 - _V1;
    Vector3D v = _V3 - _V1;
    return Vector3D::CrossProduct(u, v);
}

bool Triangle::Intersect(LineSeg l)
{
    Triangle  t(_V1, _V2, _V3);
    return l.Intersect(t);
}

bool Triangle::Intersect(Triangle t)
{
    LineSeg l (t.V1(), t.V2());
    if (Intersect(l))
        return true;
    l = LineSeg(t.V2(), t.V3());
    if (Intersect(l))
        return true;
    l = LineSeg(t.V3(), t.V1());
    if (Intersect(l))
        return true;

    l = LineSeg(_V1, _V2);
    if (t.Intersect(l))
        return true;
    l = LineSeg(_V2, _V3);
    if (t.Intersect(l))
        return true;
    l = LineSeg(_V3, _V1);
    if (t.Intersect(l))
        return true;

    return false;
}
