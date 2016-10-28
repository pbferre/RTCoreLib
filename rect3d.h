#ifndef RECT3D_H
#define RECT3D_H

#include <QString>

#include "size3d.h"
#include "point3d.h"
#include "vector3d.h"

namespace CreateCore
{

//class Point3D;
//class Vector3D;

class Rect3D
{
public:
    Rect3D();
    Rect3D(Point3D p, Size3D s);
    Rect3D(const Rect3D &r);
    Rect3D(double x, double y, double z, double h, double w, double d);
    Rect3D(Point3D p1, Point3D p2);
    Rect3D(Point3D p, Vector3D v);

    static Rect3D Empty();

    Point3D Location();
    void setLocation(const Point3D &p) {_origin = p;}

    Size3D Size();
    void setSize(const Size3D &s) {_size = s; }

    double SizeX() const {return _size.SizeX(); }
    double SizeY() const {return _size.SizeY(); }
    double SizeZ() const {return _size.SizeZ(); }
    void setSizeX(double value) { _size.setSizeX(value); }
    void setSizeY(double value) { _size.setSizeY(value); }
    void setSizeZ(double value) { _size.setSizeZ(value); }

    double X() const { return _origin.x(); }
    double Y() const { return _origin.y(); }
    double Z() const { return _origin.z(); }
    void setX(double value) { _origin.setX(value); }
    void setY(double value) { _origin.setY(value); }
    void setZ(double value) { _origin.setY(value); }

    bool Contains(Point3D p);
    bool Contains(Rect3D r);
    bool IsEmpty() { return _size.IsEmpty(); }
    bool IntersectsWith(Rect3D r);

    void Intersect(Rect3D r);
    static Rect3D Intersect(Rect3D r1, Rect3D r2);

    void Union(Rect3D r);
    static Rect3D Union(Rect3D r1, Rect3D r2);
    void Union(Point3D p);
    static Rect3D Union(Rect3D r, Point3D v);

    void Offset(double x, double y, double z);
    void Offset(Vector3D v);
    static Rect3D Offset(Rect3D r, Vector3D v);
    static Rect3D Offset(Rect3D r, double x, double y, double z);

    bool operator ==(const Rect3D &r);
    bool operator !=(const Rect3D &r);

    static bool Equals(Rect3D r1, Rect3D r2);
    bool Equals(Rect3D r);

    int GetHashCode();
    QString ToString();

private:
    Point3D _origin;
    Size3D _size;

    static Rect3D Infinite();

    void setEmpty();

};

}

#endif // RECT3D_H
