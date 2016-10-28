#include <cmath>
#include <cfloat>

#include "rect3d.h"

using namespace CreateCore;

Rect3D::Rect3D()
{
    _origin = Point3D();
    _size = Size3D();
}

Rect3D::Rect3D(Point3D p, Size3D s)
{
    _origin = p;
    _size = s;

    if(s.SizeX() < 0)
    {
        _origin.setX(_origin.x() - s.SizeX());
        _size.setSizeX(fabs(s.SizeX()));
    }

    if(s.SizeY() < 0)
    {
        _origin.setY(_origin.y() - s.SizeY());
        _size.setSizeY(fabs(s.SizeY()));
    }

    if(s.SizeZ() < 0)
    {
        _origin.setZ(_origin.z() - s.SizeZ());
        _size.setSizeZ(fabs(s.SizeZ()));
    }

}

Rect3D::Rect3D(const Rect3D &r)
{
    _origin.setX(r._origin.x());
    _origin.setY(r._origin.y());
    _origin.setZ(r._origin.z());
    setSizeX(r.SizeX());
    setSizeY(r.SizeY());
    setSizeZ(r.SizeZ());
}

Rect3D::Rect3D(double x, double y, double z, double h, double w, double d)
{
    _origin.setX(x);
    _origin.setY(y);
    _origin.setZ(z);
    _size.setSizeX(h);
    _size.setSizeY(w);
    _size.setSizeZ(d);
}

Rect3D::Rect3D(Point3D p1, Point3D p2)
{
    //Set the origin point to the minumum of the x, y, z coordinates of the two vectors
    _origin.setX(fmin(p1.x(), p2.x()));
    _origin.setY(fmin(p1.y(), p2.y()));
    _origin.setZ(fmin(p1.z(), p2.z()));

    _size.setSizeX(fmax(p1.x(), p2.x()) - _origin.x());
    _size.setSizeY(fmax(p1.y(), p2.y()) - _origin.y());
    _size.setSizeZ(fmax(p1.z(), p2.z()) - _origin.z());
}

Rect3D::Rect3D(Point3D p, Vector3D v)
{
    Rect3D(p, p + v);
}

Rect3D Rect3D::Empty()
{
    Point3D p;  // Creates a null vector at origin
    Size3D s;     // creates an empty size

    return Rect3D(p, s);
}

Point3D Rect3D::Location()
{
    return Point3D(_origin.x(), _origin.y(), _origin.z());
}

Size3D Rect3D::Size()
{
    return Size3D(_size.SizeX(), _size.SizeY(), _size.SizeZ());
}

bool Rect3D::Contains(Point3D p)
{
    if (IsEmpty())
        return false;
    else
        return ((p.x() >= _origin.x()) && (p.x() <= _origin.x() + _size.SizeX())) &&
                ((p.y() >= _origin.y()) && (p.y() <= _origin.y() + _size.SizeY())) &&
                ((p.z() >= _origin.z()) && (p.z() <= _origin.z() + _size.SizeZ()));
}

bool Rect3D::Contains(Rect3D r)
{
    if (IsEmpty() || r.IsEmpty())
        return false;
    else
    {
        return (_origin.x() <= r.X()) &&
                (_origin.y() <= r.Y()) &&
                (_origin.z() <= r.Z()) &&
                (_origin.x() + _size.SizeX() >= r.X() + r.SizeX()) &&
                (_origin.y() + _size.SizeY() >= r.Y() + r.SizeY()) &&
                (_origin.z() + _size.SizeZ() >= r.Z() + r.SizeZ());
    }
}

bool Rect3D::IntersectsWith(Rect3D r)
{
    if (IsEmpty() || r.IsEmpty())
        return false;
    else
    {
        return (r.X() <= (_origin.x() + _size.SizeX())) &&
                ((r.X() + r.SizeX()) >= _origin.x()) &&
                (r.Y() <= (_origin.y() + _size.SizeY())) &&
                ((r.Y() + r.SizeY()) >= _origin.y()) &&
                (r.Z() <= (_origin.z() + _size.SizeZ())) &&
                ((r.Z() + r.SizeZ()) >= _origin.z());
    }
}

void Rect3D::Intersect(Rect3D r)
{
    if (IsEmpty() || r.IsEmpty() || !this->IntersectsWith(r))
    {
        setEmpty();
    }
    else
    {
        double x = fmax(_origin.x(), r.X());
        double y = fmax(_origin.y(), r.Y());
        double z = fmax(_origin.z(), r.Z());
        _size.setSizeX(fmin(_origin.x() + _size.SizeX(), r.X() + r.SizeX()) - x);
        _size.setSizeY(fmin(_origin.y() + _size.SizeY(), r.Y() + r.SizeY()) - y);
        _size.setSizeZ(fmin(_origin.z() + _size.SizeZ(), r.Z() + r.SizeZ()) - z);

        _origin.setX(x);
        _origin.setY(y);
        _origin.setZ(z);
    }
}

Rect3D Rect3D::Intersect(Rect3D r1, Rect3D r2)
{
    r1.Intersect(r2);
    return r1;
}

void Rect3D::Union(Rect3D r)
{
    if (IsEmpty())
    {
        _origin.setX(r.X());
        _origin.setY(r.Y());
        _origin.setZ(r.Z());
        _size.setSizeX(r.SizeX());
        _size.setSizeY(r.SizeY());
        _size.setSizeZ(r.SizeZ());
    }
    else if (!r.IsEmpty())
    {
        double x = fmin(_origin.x(), r.X());
        double y = fmin(_origin.y(), r.Y());
        double z = fmin(_origin.z(), r.Z());
        _size.setSizeX(fmax(_origin.x() + _size.SizeX(), r.X() + r.SizeX()) - x);
        _size.setSizeY(fmax(_origin.y() + _size.SizeY(), r.Y() + r.SizeY()) - y);
        _size.setSizeZ(fmax(_origin.z() + _size.SizeZ(), r.Z() + r.SizeZ()) - z);

        _origin.setX(x);
        _origin.setY(y);
        _origin.setZ(z);
    }
}

Rect3D Rect3D::Union(Rect3D r1, Rect3D r2)
{
    r1.Union(r2);
    return r1;
}

void Rect3D::Union(Point3D p)
{
    Rect3D r(p, p);
    Union(r);
}

Rect3D Rect3D::Union(Rect3D r, Point3D p)
{
    Rect3D r2(p, p);
    r.Union(r2);
    return r;
}

void Rect3D::Offset(double x, double y, double z)
{
    _origin.setX(_origin.x() + x);
    _origin.setY(_origin.y() + y);
    _origin.setZ(_origin.z() + z);
}

void Rect3D::Offset(Vector3D v)
{
    Offset(v.x(), v.y(), v.z());
}

Rect3D Rect3D::Offset(Rect3D r, Vector3D v)
{
    r.Offset(v.x(), v.y(), v.z());
    return r;
}

Rect3D Rect3D::Offset(Rect3D r, double x, double y, double z)
{
    r.Offset(x, y, z);
    return r;
}

Rect3D Rect3D::Infinite()
{
    double negmax = -1 * FLT_MAX;
    double infsize = FLT_MAX;
    Rect3D r(negmax, negmax, negmax, infsize, infsize, infsize);
    return r;
}

int Rect3D::GetHashCode()
{
    return 0;
}

bool Rect3D::operator ==(const Rect3D &r)
{
    return (_origin.x() == r.X()) &&
            (_origin.y() == r.Y()) &&
            (_origin.z() == r.Z()) &&
            (_size.SizeX() == r.SizeX()) &&
            (_size.SizeY() == r.SizeY()) &&
            (_size.SizeZ() == r.SizeY());
}

bool Rect3D::operator !=(const Rect3D &r)
{
    return (_origin.x() != r.X()) ||
            (_origin.y() != r.Y()) ||
            (_origin.z() != r.Z()) ||
            (_size.SizeX() != r.SizeX()) ||
            (_size.SizeY() != r.SizeY()) ||
            (_size.SizeZ() != r.SizeY());
}

bool Rect3D::Equals(Rect3D r1, Rect3D r2)
{
    return (r1 == r2);
}

bool Rect3D::Equals(Rect3D r)
{
    return (_origin.x() == r.X()) &&
            (_origin.y() == r.Y()) &&
            (_origin.z() == r.Z()) &&
            (_size.SizeX() == r.SizeX()) &&
            (_size.SizeY() == r.SizeY()) &&
            (_size.SizeZ() == r.SizeY());
}

QString Rect3D::ToString()
{
    return QString("Origin: x = %1, y = %2, z = %3\nSize: x = %4, y = %5, z = %6").arg(_origin.x()).arg(_origin.y()).arg(_origin.z()).arg(_size.SizeX()).arg(_size.SizeY()).arg(_size.SizeZ());
}

void Rect3D::setEmpty()
{
    _origin.setX(0.0);;
    _origin.setY(0.0);
    _origin.setZ(0.0);
    _size.setSizeX(0.0);
    _size.setSizeY(0.0);
    _size.setSizeZ(0.0);
}
