#include "size3d.h"
#include "point3d.h"
#include "vector3d.h"

using namespace CreateCore;

Size3D::Size3D()
{
    _sizex = _sizey = _sizez = 0;
}

Size3D::Size3D(const Size3D &s)
{
    _sizex = s._sizex;
    _sizey = s._sizey;
    _sizez = s._sizez;
}

Size3D::Size3D(double x, double y, double z)
{
    _sizex = x;
    _sizey = y;
    _sizez = z;
}

bool Size3D::operator ==(const Size3D &s)
{
    return (_sizex == s._sizex) && (_sizey == s._sizey) && (_sizez == s._sizez);
}

bool Size3D::operator !=(const Size3D &s)
{
    return (_sizex != s._sizex) || (_sizey != s._sizey) || (_sizez != s._sizez);
}

bool Size3D::Equals(const Size3D &s1, const Size3D &s2)
{
    return (s1._sizex == s2._sizex) && (s1._sizey == s2._sizey) && (s1._sizez == s2._sizez);
}

bool Size3D::Equals(const Size3D &s)
{
    return (_sizex == s._sizex) && (_sizey == s._sizey) && (_sizez == s._sizez);
}

Size3D Size3D::operator =(const Size3D &s)
{
    _sizex = s._sizex;
    _sizey = s._sizey;
    _sizez = s._sizez;

    return *this;
}

QString Size3D::ToString()
{
    if (IsEmpty())
        return "Empty";
    else
        return QString(":sizeX %1, sizeY: %2, sizeZ: %3\n").arg(_sizex).arg(_sizey).arg(_sizez);
}

Size3D Size3D::Empty()
{
    return Size3D(0.0, 0.0, 0.0);
}

Point3D Size3D::ToPoint3D()
{
    return Point3D(_sizex, _sizey, _sizez);
}

Vector3D Size3D::ToVector3d()
{
    return Vector3D(_sizex, _sizey, _sizez);
}

bool CreateCore::operator ==(Size3D s1, Size3D s2)
{
    return (s1.SizeX() == s2.SizeX()) && (s1.SizeY() == s2.SizeY()) && (s1.SizeZ() == s2.SizeZ());
}

bool CreateCore::operator !=(Size3D s1, Size3D s2)
{
    return (s1.SizeX() != s2.SizeX()) || (s1.SizeY() != s2.SizeY()) || (s1.SizeZ() != s2.SizeZ());
}
