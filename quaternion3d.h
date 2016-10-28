#ifndef QUATERNION3D_H
#define QUATERNION3D_H

#include <QQuaternion>
namespace CreateCore
{

class Vector3D;

class Quaternion3D : public QQuaternion
{
public:
    Quaternion3D();
    Quaternion3D(double x, double y, double z, double scalar);
    Quaternion3D(Vector3D axisOfRotation, double angleInDegrees);

    static bool Equals(Quaternion3D q1, Quaternion3D q2);
    bool Equals(Quaternion3D q);

    int GetHashCode();
    int GetIdentityhashCode();
    QString ToString();

    static Quaternion3D Identity();
    Vector3D Axis();
    double Angle();
    void Conjugate();
    void Invert();

    static Quaternion3D Add(Quaternion3D q1, Quaternion3D q2);
    static Quaternion3D Subtract(Quaternion3D q1, Quaternion3D q2);
    static Quaternion3D Multiply(Quaternion3D q1, Quaternion3D q2);
    static Quaternion3D Slerp(Quaternion3D from, Quaternion3D to, double t);

    void Scale(double scale);

private:
    double Max(double a, double b, double c, double d);
};

//bool operator ==(Quaternion3D q1, Quaternion3D q2);
//bool operator !=(Quaternion3D q1, Quaternion3D q2);
Quaternion3D operator +(Quaternion3D q1, Quaternion3D q2);
Quaternion3D operator -(Quaternion3D q1, Quaternion3D q2);
Quaternion3D operator *(Quaternion3D q1, Quaternion3D q2);

}

#endif // QUATERNION3D_H
