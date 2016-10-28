#include "point3d.h"
#include "vector3d.h"
#include "matrix3d.h"

#include <QList>

using namespace CreateCore;

Point3D::Point3D()
{
    QVector3D::QVector3D();
}

Point3D::Point3D(double x, double y, double z)
{
    setX(x);
    setY(y);
    setZ(z);
}

Point3D::Point3D(const Point3D &p)
{
    setX(p.x());
    setY(p.y());
    setZ(p.z());
}

void Point3D::Offset(double offsetX, double offsetY, double offsetZ)
{
    setX(this->x() + offsetX);
    setY(this->y() + offsetY);
    setZ(this->z() + offsetZ);
}

Point3D Point3D::Add(Point3D point, Vector3D vector)
{
    Point3D p(point.x() + vector.x(), point.y() + vector.y(), point.z() + vector.z());
    return p;
}

Point3D Point3D::Subtract(Point3D point, Vector3D vector)
{
    Point3D p(point.x() - vector.x(), point.y() - vector.y(), point.z() - vector.z());
    return p;
}

Vector3D Point3D::Subtract(Point3D point1, Point3D point2)
{
    Vector3D v(point1.x() - point2.x(), point1.y() - point2.y(), point1.z() - point2.z());
    return v;
}

void Point3D::Subtract(Point3D point1, Point3D point2, Vector3D &vec)
{
    vec.setX(point1.x() - point2.x());
    vec.setY(point1.y() - point2.y());
    vec.setZ(point1.z() - point2.z());
}

Point3D Point3D::Multiply(Point3D point, Matrix3D matrix)
{
    return matrix.Transform(point);
}

Point3D Point3D::Multiply(Point3D point, double d)
{
    Point3D p(point.x() * d, point.y() * d, point.z() * d);
    return p;
}

Point3D Point3D::Sum(QList<Point3D> points)
{
    double x = 0.0f;
    double y = 0.0f;
    double z = 0.0f;
    foreach (Point3D point, points)
    {
        x += point.x();
        y += point.y();
        z += point.z();
    }

    Point3D p(x, y, z);
    return p;
}

bool Point3D::Equals(Point3D point1, Point3D point2)
{
    return (point1.x() == point2.x()) && (point1.y() == point2.y()) && (point1.z() == point2.z());
}

bool Point3D::ComparePoints(Point3D a, Point3D b)
{
    if (a.x() < b.x())
        return true;
    else if (b.x() < a.x())
        return false;

    if (a.y() < b.y())
        return true;
    else if (b.y() < a.y())
        return false;

    if (a.z() < b.z())
        return true;
    else if (b.z() < a.z())
        return false;

    return false;
}

bool Point3D::Equals(const Point3D &point)
{
    return (this->x() == point.x()) && (this->y() == point.y()) && (this->z() == point.z());
}

QString Point3D::ToString()
{
    return QString("X: %1\tY: %2\tZ: %3").arg(this->x()).arg(this->y()).arg(this->z());
}

double Point3D::DistanceTo(Point3D point1, Point3D point2)
{
    Vector3D v = point1 - point2;
    return v.length();
}

double Point3D::DistanceToSquared(Point3D point1, Point3D point2)
{
    Vector3D v = point1 - point2;
    return v.lengthSquared();
}

int Point3D::GetHashCode()
{
    return 0;
}

Vector3D Point3D::ToVector3D(Point3D point)
{
    Vector3D v(point.x(), point.y(), point.z());
    return v;
}

Vector3D Point3D::ToVector3D()
{
    Vector3D v(this->x(), this->y(), this->z());
    return v;
}

Point3D CreateCore::operator +(Point3D point, Vector3D vec)
{
    Point3D p(point.x() + vec.x(), point.y() + vec.y(), point.z() + vec.z());
    return p;
}

Point3D CreateCore::operator -(Point3D point, Vector3D vec)
{
    Point3D p(point.x() - vec.x(), point.y() - vec.y(), point.z() - vec.z());
    return p;
}

Vector3D CreateCore::operator -(Point3D point1, Point3D point2)
{
    Vector3D v(point1.x() - point2.x(), point1.y() - point2.y(), point1.z() - point2.z());
    return v;
}

Point3D CreateCore::operator *(Point3D point, Matrix3D matrix)
{
    return matrix.Transform(point);
}

bool CreateCore::operator <(Point3D point1, Point3D point2)
{
    if (point1.x() != point2.x())
        return point1.x() < point2.x();
    if (point1.y() != point2.y())
        return point1.y() < point2.y();
    if (point1.z() != point2.z())
        return point1.z() < point2.z();

    return false;
}

bool CreateCore::operator >(Point3D point1, Point3D point2)
{
    if (point1.x() != point2.x())
        return point1.x() > point2.x();
    if (point1.y() != point2.y())
        return point1.y() > point2.y();
    if (point1.z() != point2.z())
        return point1.z() > point2.z();

    return false;
}

bool CreateCore::operator ==(Point3D point1, Point3D point2)
{
    return (point1.x() == point2.x()) && (point1.y() == point2.y()) && (point1.z() == point2.z());
}

bool CreateCore::operator !=(Point3D point1, Point3D point2)
{
    return (point1.x() != point2.x()) || (point1.y() != point2.y()) || (point1.z() != point2.z());
}
