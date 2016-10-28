// Represents a vertex and a direction in 3-Dimensional Space

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <QVector3D>
#include <QHash>

namespace CreateCore
{
class Point3D;
class Size3D;
class Matrix3D;

class Vector3D : public QVector3D
{
public:
    Vector3D();
    Vector3D(double x, double y, double z);
    Vector3D(const Vector3D &v);

    static bool Equals(Vector3D v1, Vector3D v2);
    bool Equals(const Vector3D vec);

    static double AngleBetween(Vector3D v1, Vector3D v2);
    static Vector3D Add(Vector3D v1, Vector3D v2);
    static Vector3D Add(Vector3D vec, Point3D point);
    static Vector3D Subtract(Vector3D v1, Vector3D v2);
    static Vector3D Subtract(Vector3D vec, Point3D point);
    static Vector3D Multiply(Vector3D vec, double scalar);
    static Vector3D Multiply(double scalar, Vector3D vec);
    static Vector3D Multiply(Vector3D vec, Matrix3D matrix);
    static Vector3D Divide(Vector3D vec, double scalar);
    static bool IsUndefined() {return false;}
    static Vector3D Empty() { return Vector3D(0.0, 0.0, 0.0); }

    Point3D ToPoint3D();
    Size3D ToSize3D();

    static Vector3D CrossProduct(Vector3D v1, Vector3D v2);
    static void CrossProduct(Vector3D v1, Vector3D v2, Vector3D &result);
    static double DotProduct(Vector3D v1, Vector3D v2);

    Vector3D FindAnyPerpindicular();
    void Negate();

    int GetHashCode();
    QString ToString();
};

bool operator ==(Vector3D v1, Vector3D v2);
bool operator !=(Vector3D v1, Vector3D v2);
Vector3D operator *(double scalar, Vector3D vec);
Vector3D operator *(Vector3D vec, double scalar);
Vector3D operator *(Vector3D vec, Matrix3D matrix);
Vector3D operator /(Vector3D vec, double scalar);
Vector3D operator +(Vector3D v1, Vector3D v2);
Point3D operator +(Vector3D v, Point3D p);
Vector3D operator -(Vector3D vec);
Vector3D operator -(Vector3D v1, Vector3D v2);
Point3D operator -(Vector3D v, Point3D p);
//uint qHash(const Vector3D &v) { return qHash(v.lengthSquared()); }

}

#endif // VECTOR3D_H
