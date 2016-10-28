#include "quaternion3d.h"
#include "vector3d.h"
#include "constants.h"

#include <cmath>
#include <limits>

using namespace CreateCore;

Quaternion3D::Quaternion3D()
{
    QQuaternion::QQuaternion();
}

Quaternion3D::Quaternion3D(double x, double y, double z, double scalar)
{
    QQuaternion::QQuaternion(scalar, x, y, z);
}

Quaternion3D::Quaternion3D(Vector3D axisOfRotation, double angleInDegrees)
{
    angleInDegrees = fmod(angleInDegrees, 360.0f);
    double angleInRadians = angleInDegrees * (cPI / 180.0f);
    double length = axisOfRotation.length();

    Vector3D v = (axisOfRotation / length) * sin(0.5 * angleInRadians);
    setX(v.x());
    setY(v.y());
    setZ(v.z());
    setScalar(cos(0.5 * angleInRadians));
}

bool Quaternion3D::Equals(Quaternion3D q1, Quaternion3D q2)
{
    if (q1.isIdentity() && q2.isIdentity())
        return true;
    else
        return (q1.x() == q2.x()) && (q1.y() == q2.y()) && (q1.z() == q2.z()) && (q1.scalar() == q2.scalar());
}

bool Quaternion3D::Equals(Quaternion3D q)
{
    if (this->isIdentity() && q.isIdentity())
        return true;
    else
        return (this->x() == q.x()) && (this->y() == q.y()) && (this->z() == q.z()) && (this->scalar() == q.scalar());
}

int Quaternion3D::GetHashCode()
{
    return 0;
}

int Quaternion3D::GetIdentityhashCode()
{
    return 0;
}

QString Quaternion3D::ToString()
{
    return QString("X: %1\tY: %2\tZ: %3\tScalar: %4").arg(this->x()).arg(this->y()).arg(this->z()).arg(this->scalar());
}

Vector3D Quaternion3D::Axis()
{
    if (isIdentity())
        return Vector3D(0.0, 1.0, 0.0);
    else
    {
        Vector3D v(this->x(), this->y(), this->z());
        v.normalize();
        return v;
    }
}

double Quaternion3D::Angle()
{
    if (isIdentity())
        return 0;

    double msin = sqrt(this->x() * this->x() + this->y() * this->y() + this->z() * this->z());
    double mcos = this->scalar();

    if (!(msin <= DBL_MAX))
    {
        double maxcoeff = fmax(fmax(fabs(this->x()), fabs(this->y())), fabs(this->z()));
        double x = this->x() / maxcoeff;
        double y = this->y() / maxcoeff;
        double z = this->z() / maxcoeff;
        msin = sqrt(x * x + y * y + z * z);
        mcos = this->scalar() / maxcoeff;
    }

    return atan2(msin, mcos) * (360.0f / cPI);
}

void Quaternion3D::Conjugate()
{
    if (isIdentity())
        return;

    setX(this->x() * -1);
    setY(this->y() * -1);
    setZ(this->z() * -1);
}

void Quaternion3D::Invert()
{
    QQuaternion q = this->inverted();
    setX(q.x());
    setY(q.y());
    setZ(q.z());
    setScalar(q.scalar());
}

Quaternion3D Quaternion3D::Add(Quaternion3D q1, Quaternion3D q2)
{
    return q1 + q2;
}

Quaternion3D Quaternion3D::Subtract(Quaternion3D q1, Quaternion3D q2)
{
    return q1 - q2;
}

Quaternion3D Quaternion3D::Multiply(Quaternion3D q1, Quaternion3D q2)
{
    return q1 * q2;
}

void Quaternion3D::Scale(double scale)
{
    if (isIdentity())
    {
        setScalar(scale);
        return;
    }

    setX(this->x() * scale);
    setY(this->y() * scale);
    setZ(this->z() * scale);
    setScalar(this->scalar() * scale);
}

Quaternion3D Quaternion3D::Slerp(Quaternion3D from, Quaternion3D to, double t)
{
    QQuaternion q = QQuaternion::slerp(from, to, t);
    return Quaternion3D(q.x(), q.y(), q.z(), q.scalar());
}

double Quaternion3D::Max(double a, double b, double c, double d)
{
    if (b > a)
        a = b;
    if (c > a)
        a = c;
    if (d > a)
        a = d;
    return a;
}

Quaternion3D Quaternion3D::Identity()
{
    return Quaternion3D(0.0, 0.0, 0.0, 1.0);
}

Quaternion3D CreateCore::operator +(Quaternion3D left, Quaternion3D right)
{
    if (right.isIdentity())
    {
        if (left.isIdentity())
            return Quaternion3D(0.0, 0.0, 0.0, 2.0);
        else
        {
            left.setScalar(left.scalar() + 1.0f);
            return left;
        }
    }
    else if (left.isIdentity())
    {
        right.setScalar(right.scalar() + 1.0f);
        return right;
    }
    else
    {
        QQuaternion q = (QQuaternion)left + (QQuaternion)right;
        return Quaternion3D(q.x(), q.y(), q.z(), q.scalar());
    }
}

Quaternion3D CreateCore::operator -(Quaternion3D left, Quaternion3D right)
{
    if (right.isIdentity())
    {
        if (left.isIdentity())
            return Quaternion3D(0.0, 0.0, 0.0, 0.0);
        else
        {
            left.setScalar(left.scalar() - 1.0f);
            return left;
        }
    }
    else if (left.isIdentity())
    {
        return Quaternion3D(right.x() * -1, right.y() * -1, right.z() * -1, right.scalar() * -1);
    }
    else
    {
        QQuaternion q = (QQuaternion)left - (QQuaternion)right;
        return Quaternion3D(q.x(), q.y(), q.z(), q.scalar());
    }
}

Quaternion3D CreateCore::operator *(Quaternion3D left, Quaternion3D right)
{
    if (left.isIdentity())
        return right;
    if (right.isIdentity())
        return left;

    QQuaternion q = (QQuaternion)left * (QQuaternion)right;
    return Quaternion3D(q.x(), q.y(), q.z(), q.scalar());
}

