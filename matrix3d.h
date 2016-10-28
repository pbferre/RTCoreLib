#ifndef MATRIX3D_H
#define MATRIX3D_H

#include <QMatrix4x4>

namespace CreateCore
{
class Quaternion3D;
class Point3D;
class Vector3D;

class Matrix3D : QMatrix4x4
{
public:
    Matrix3D();
    Matrix3D(double m11, double m12, double m13, double m14,
             double m21, double m22, double m23, double m24,
             double m31, double m32, double m33, double m34,
             double m41, double m42, double m43, double m44);
    Matrix3D(QMatrix4x4 m);

    static Matrix3D Identity();
    void Prepend(Matrix3D m);
    void Append(Matrix3D m);

    //void Rotate(Quaternion3D q);
//    void RotatePrepend(Quaternion3D q);
//    void RotateAt(Quaternion3D q, Point3D p);
//    void RotateAtPrepend(Quaternion3D q, Point3D p);

    //void Scale(Vector3D v);
    //void ScalePrepend(Vector3D v);
    //void ScaleAt(Vector3D v, Point3D p);
    //void ScaleAtPrepend(Vector3D v, Point3D p);

    //void Translate(Vector3D offset);
    //void TranslatePrepend(Vector3D offset);

    Matrix3D Multiply(Matrix3D matrix1, Matrix3D matrix2);

    Point3D Transform(Point3D point);
    void Transform(QList<Point3D> points);
    Vector3D Transform(Vector3D vec);
    void Transform(QList<Vector3D> vecs);

    //void Invert();

    static bool Equals(Matrix3D m1, Matrix3D m2) {return m1 == m2;}
    bool Equals(const Matrix3D &value);

    int GetHashCode();
    QString ToString();

    // Individual Data Members
    double M11() { return row(0).x();}
    double M12() { return row(0).y();}
    double M13() { return row(0).z();}
    double M14() { return row(0).w();}
    //
    double M21() { return row(1).x();}
    double M22() { return row(1).y();}
    double M23() { return row(1).z();}
    double M24() { return row(1).w();}
    //
    double M31() { return row(2).x();}
    double M32() { return row(2).y();}
    double M33() { return row(2).z();}
    double M34() { return row(2).w();}
    //
    double M41() { return row(3).x();}
    double M42() { return row(3).y();}
    double M43() { return row(3).z();}
    double M44() { return row(3).w();}
    //
    double OffsetX() { return row(3).x();}
    double OffsetY() { return row(3).y();}
    double OffsetZ() { return row(3).z();}

private:
    //void SetScaleMatrix(Vector3D scale);
    //void SetScaleMatrix(Vector3D scale, Point3D center);
    //void SetTranslationMatrix(Vector3D offset);
    void MultiplyPoint(Point3D &point);
    void MultiplyVector(Vector3D &vec);

    //static Matrix3D CreateRotationMatrix(Quaternion3D quat, Point3D center);

    //double GetNormalizedAfflineDeterminant();
    //bool NormalizedAfflineInvert();
    //bool InvertCore();

    Matrix3D CopyThis();

};

}

#endif // MATRIX3D_H
