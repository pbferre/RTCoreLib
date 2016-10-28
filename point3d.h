// Represents a single vertex in 3-Dimensional space

#ifndef POINT3D_H
#define POINT3D_H

#include <QVector3D>
#include <QHash>

namespace CreateCore
{
class Vector3D;
class Matrix3D;

class Point3D : public QVector3D
{
public:
    Point3D();
    Point3D(double x, double y, double z);
    Point3D(const Point3D &p);

    void Offset(double offsetX, double offsetY, double offsetZ);
    int GetHashCode();

    static Point3D Add(Point3D point, Vector3D vector);
    static Point3D Subtract(Point3D point, Vector3D vector);
    static Vector3D Subtract(Point3D point1, Point3D point2);
    static void Subtract(Point3D point1, Point3D point2, Vector3D &vec);
    static Point3D Multiple(Point3D point, Matrix3D matrix);
    static bool Equals(Point3D point1, Point3D point2);
    static bool ComparePoints(Point3D p1, Point3D p2);
    static double DistanceTo(Point3D point1, Point3D point2);
    static double DistanceToSquared(Point3D point1, Point3D point2);
    static Vector3D ToVector3D(Point3D point);
    static Point3D Multiply(Point3D point, double d);
    static Point3D Multiply(Point3D point, Matrix3D matrix);
    static Point3D Sum(QList<Point3D> points);

    Vector3D ToVector3D();

    bool Equals(const Point3D &point);

    QString ToString();

    //friend Point3D operator +(Point3D point, Vector3D vector);
};


Point3D operator +(Point3D point, Vector3D vector);
Point3D operator -(Point3D point, Vector3D vector);
Vector3D operator -(Point3D point1, Point3D point2);
Point3D operator *(Point3D point, Matrix3D matrix);
bool operator <(Point3D point1, Point3D point2);
bool operator >(Point3D point1, Point3D point2);
//uint qHash(const Point3D &p) { return qHash(p.lengthSquared()); }
bool operator ==(Point3D point1, Point3D point2);
bool operator !=(Point3D point1, Point3D point2);

}

#endif // POINT3D_H
