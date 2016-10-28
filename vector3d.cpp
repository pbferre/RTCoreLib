#define USE_MATH_DEFINES

#include <math.h>

#include "vector3d.h"
#include "point3d.h"
#include "size3d.h"
#include "constants.h"
#include "matrix3d.h"

#include <cmath>

using namespace CreateCore;

Vector3D::Vector3D()
{
    QVector3D::QVector3D();
}

Vector3D::Vector3D(double x, double y, double z)
{
    QVector3D::QVector3D(x, y, z);
}

Vector3D::Vector3D(const Vector3D &v)
{
    setX(v.x());
    setY(v.y());
    setZ(v.z());
}

bool Vector3D::Equals(Vector3D v1, Vector3D v2)
{
    return (v1.x() == v2.x()) && (v1.y() == v2.y()) && (v1.z() == v2.z());
}

bool Vector3D::Equals(const Vector3D vec)
{
    return (this->x() == vec.x()) && (this->y() == vec.y()) && (this->z() == vec.z());
}

int Vector3D::GetHashCode()
{
    return 0;
}

double Vector3D::AngleBetween(Vector3D v1, Vector3D v2)
{
    v1.normalize();
    v2.normalize();

    double ratio = QVector3D::dotProduct(v1, v2);
    double theta;
    if (ratio < 0)
    {
        Vector3D tmpvec = -v1 - v2;
        theta = cPI - 2.0 * asin(tmpvec.length() / 2.0);
    }
    else
    {
        Vector3D tmpvec = v1 - v2;
        theta = 2.0 * asin(tmpvec.length() / 2.0);
    }

    return theta;
}

QString Vector3D::ToString()
{
     return QString("X: %1\tY: %2\tZ: %3").arg(this->x()).arg(this->y()).arg(this->z());
}

void Vector3D::Negate()
{
    setX(this->x() * -1);
    setY(this->y() * -1);
    setZ(this->z() * -1);
}

Vector3D Vector3D::Add(Vector3D v1, Vector3D v2)
{
    Vector3D v(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
    return v;
}

Vector3D Vector3D::Subtract(Vector3D v1, Vector3D v2)
{
    Vector3D v(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
    return v;
}

Vector3D Vector3D::Add(Vector3D vec, Point3D point)
{
    Vector3D v(vec.x() + point.x(), vec.y() + point.y(), vec.z() + point.z());
    return v;
}

Vector3D Vector3D::Subtract(Vector3D vec, Point3D point)
{
    Vector3D v(vec.x() - point.x(), vec.y() - point.y(), vec.z() - point.z());
    return v;
}

Vector3D Vector3D::Multiply(Vector3D vec, double scalar)
{
    Vector3D v(vec.x() * scalar, vec.y() * scalar, vec.z() * scalar);
    return v;
}

Vector3D Vector3D::Multiply(double scalar, Vector3D vec)
{
    Vector3D v(vec.x() * scalar, vec.y() * scalar, vec.z() * scalar);
    return v;
}

Vector3D Vector3D::Multiply(Vector3D vec, Matrix3D matrix)
{
    return matrix.Transform(vec);
}

Vector3D Vector3D::Divide(Vector3D vec, double scalar)
{
    return vec * (1.0 / scalar);
}

double Vector3D::DotProduct(Vector3D v1, Vector3D v2)
{
    return QVector3D::dotProduct(v1, v2);
}

Vector3D Vector3D::CrossProduct(Vector3D v1, Vector3D v2)
{
    QVector3D qvec = QVector3D::crossProduct(v1, v2);
    Vector3D v(qvec.x(), qvec.y(), qvec.z());
    return v;
}

void Vector3D::CrossProduct(Vector3D v1, Vector3D v2, Vector3D &result)
{
    Vector3D vec = CrossProduct(v1, v2);
    result = vec;
}

Vector3D Vector3D::FindAnyPerpindicular()
{
    QVector3D v = this->normalized();
    QVector3D u = QVector3D::crossProduct(QVector3D(0.0f, 1.0f, 0.0f), v);
    if (u.lengthSquared() < 1e-3)
        u = QVector3D::crossProduct(QVector3D(1.0f, 0.0f, 0.0f), v);

    return Vector3D(u.x(), u.y(), u.z());
}

Point3D Vector3D::ToPoint3D()
{
    Point3D p(this->x(), this->y(), this->z());
    return p;
}

Size3D Vector3D::ToSize3D()
{
    Size3D s(this->x(), this->y(), this->z());
    return s;
}

bool CreateCore::operator ==(Vector3D v1, Vector3D v2)
{
    return (v1.x() == v2.x()) && (v1.y() == v2.y()) && (v1.z() == v2.z());
}

bool CreateCore::operator !=(Vector3D v1, Vector3D v2)
{
    return (v1.x() != v2.x()) || (v1.y() != v2.y()) || (v1.z() != v2.z());
}

Vector3D CreateCore::operator -(Vector3D vec)
{
    Vector3D v(vec.x() * -1, vec.y() * -1, vec.z() * -1);
    return v;
}

Vector3D CreateCore::operator +(Vector3D v1, Vector3D v2)
{
    Vector3D v(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
    return v;
}

Vector3D CreateCore::operator -(Vector3D v1, Vector3D v2)
{
    Vector3D v(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
    return v;
}

Point3D CreateCore::operator +(Vector3D vec, Point3D point)
{
    Point3D p(vec.x() + point.x(), vec.y() + point.y(), vec.z() + point.z());
    return p;
}

Point3D CreateCore::operator -(Vector3D vec, Point3D point)
{
    Point3D p(vec.x() - point.x(), vec.y() - point.y(), vec.z() - point.z());
    return p;
}

Vector3D CreateCore::operator *(Vector3D vec, double scalar)
{
    Vector3D v(vec.x() * scalar, vec.y() * scalar, vec.z() * scalar);
    return v;
}

Vector3D CreateCore::operator *(double scalar, Vector3D vec)
{
    Vector3D v(vec.x() * scalar, vec.y() * scalar, vec.z() * scalar);
    return v;
}

Vector3D CreateCore::operator /(Vector3D vec, double scalar)
{
    Vector3D v(vec.x() / scalar, vec.y() / scalar, vec.z() / scalar);
    return v;
}

Vector3D CreateCore::operator *(Vector3D vec, Matrix3D matrix)
{
    return matrix.Transform(vec);
}
