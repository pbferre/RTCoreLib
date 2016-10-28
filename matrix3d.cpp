#include "matrix3d.h"
#include"quaternion3d.h"
#include "point3d.h"
#include "vector3d.h"

#include <QVector4D>

using namespace CreateCore;

Matrix3D::Matrix3D()
{
    QMatrix4x4::QMatrix4x4();
}

Matrix3D::Matrix3D(double m11, double m12, double m13, double m14,
                   double m21, double m22, double m23, double m24,
                   double m31, double m32, double m33, double m34,
                   double m41, double m42, double m43, double m44)
{
    QMatrix4x4::QMatrix4x4(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
}

Matrix3D::Matrix3D(QMatrix4x4 m)
{
    for (int i = 0; i < 4; i++)
    {
        this->setRow(i, m.row(i));
    }
}

Matrix3D Matrix3D::Identity()
{
    return Matrix3D();
}

void Matrix3D::Prepend(Matrix3D m)
{
    Matrix3D thismatrix = CopyThis();
    thismatrix = m * thismatrix;
    for (int i = 0; i < 4; i++)
    {
        this->setRow(i, thismatrix.row(i));
    }
}

void Matrix3D::Append(Matrix3D m)
{
    Matrix3D thismatrix = CopyThis();
    thismatrix *= m;
    for (int i = 0; i < 4; i++)
    {
        this->setRow(i, thismatrix.row(i));
    }
}

Matrix3D Matrix3D::Multiply(Matrix3D matrix1, Matrix3D matrix2)
{
    QMatrix4x4 m = matrix1 * matrix2;
    return Matrix3D(m);
}

Point3D Matrix3D::Transform(Point3D point)
{
    MultiplyPoint(point);
    return point;
}

void Matrix3D::Transform(QList<Point3D> points)
{
    foreach(Point3D point, points)
    {
        MultiplyPoint(point);
    }
}

Vector3D Matrix3D::Transform(Vector3D vec)
{
    MultiplyVector(vec);
    return vec;
}

void Matrix3D::Transform(QList<Vector3D> vecs)
{
    foreach(Vector3D vec, vecs)
    {
        MultiplyVector(vec);
    }
}

void Matrix3D::MultiplyPoint(Point3D &point)
{
    if (isIdentity())
        return;

    double x = point.x();
    double y = point.y();
    double z = point.z();

    point.setX(x * M11() + y * M21() * z * M31() + OffsetX());
    point.setY(x * M12() + y * M22() * z * M32() + OffsetY());
    point.setZ(x * M13() + y * M23() * z * M33() + OffsetZ());

    if (!isAffine())
    {
        double w = x * M14() + y * M24() + z * M34() + M44();

        point.setX(point.x() / w);
        point.setY(point.y() / w);
        point.setZ(point.z() / w);
    }
}

void Matrix3D::MultiplyVector(Vector3D &vec)
{
    if (isIdentity())
        return;

    double x = vec.x();
    double y = vec.y();
    double z = vec.z();

    vec.setX(x * M11() + y * M21() + z * M31());
    vec.setY(x * M12() + y * M22() + z * M32());
    vec.setZ(x * M13() + y * M23() + z * M33());
}

bool Matrix3D::Equals(const Matrix3D &value)
{
   return CopyThis() == value;
}

int Matrix3D::GetHashCode()
{
    return 0;
}

QString Matrix3D::ToString()
{
    QString result = QString("1: %1\t2: %2\t3: %3\t4: %4\n").arg(this->M11()).arg(this->M12()).arg(this->M13()).arg(this->M14());
    result.append(QString("5: %1\t6: %2\t7: %3\t8: %4\n").arg(this->M21()).arg(this->M22()).arg(this->M23()).arg(this->M24()));
    result.append(QString("9: %1\t10: %2\t11: %3\t12: %4\n").arg(this->M31()).arg(this->M32()).arg(this->M33()).arg(this->M34()));
    result.append(QString("13: %1\t14: %2\t15: %3\t16: %4\n").arg(this->M41()).arg(this->M42()).arg(this->M43()).arg(this->M44()));

    return result;

}

Matrix3D Matrix3D::CopyThis()
{
    Matrix3D result;
    for (int i = 0; i < 4; i++)
    {
        result.setRow(i, this->row(i));
    }

    return result;
}

//void Matrix3D::Rotate(Quaternion3D q)
//{
//    Point3D center;
//    this *= CreateRotationMatrix(q, center);
//}

//void Matrix3D::RotatePrepend(Quaternion3D q)
//{
//    Point3D center;
//    this = CreateRotationMatrix(q, center) * this;
//}

//void Matrix3D::RotateAt(Quaternion3D q, Point3D p)
//{
//    this *= CreateRotationMatrix(q, p);
//}

//void Matrix3D::RotateAtPrepend(Quaternion3D q, Point3D p)
//{
//    this = CreateRotationMatrix(q, p) * this;
//}

//void Matrix3D::Scale(Vector3D scale)
//{
//    if (isIdentity())
//        SetScaleMatrix(scale);
//    else
//    {
//        for (int i = 0; i < 4; i++)
//        {
//            QVector4D v4 = this->row(i);
//            v4.setX(v4.x() * scale.x());
//            v4.setY(v4.y() * scale.y());
//            v4.setY(v4.z() * scale.z());
//            this->setRow(i, v4);
//        }
//    }
//}

//void Matrix3D::ScalePrepend(Vector3D scale)
//{
//    if (isIdentity())
//        SetScaleMatrix(scale);
//    else
//    {
//        QVector4D row;
//        row = this->row(0);
//        row *= scale.x();
//        this->setRow(0, row);

//        row = this->row(1);
//        row *= scale.y();
//        this->setRow(1, row);

//        row = this->row(2);
//        row *= scale.z();
//        this->setRow(2, row);
//    }
//}

//void Matrix3D::Translate(Vector3D offset)
//{
//    if (isIdentity())
//        SetTranslationMatrix(offset);
//    else
//    {
//        for (int i = 0; i < 4; i++)
//        {
//            QVector4D row = this->row(i);
//            row.setX(row.w() * offset.x());
//            row.setY(row.w() * offset.y());
//            row.setZ(row.w() * offset.z());
//            this->setRow(i, row);
//        }
//    }
//}

