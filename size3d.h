#ifndef SIZE3D_H
#define SIZE3D_H

#include <QString>

namespace CreateCore
{
class Point3D;
class Vector3D;

class Size3D
{
public:
    Size3D();
    Size3D(const Size3D &s);
    Size3D(double x, double y, double z);

    double SizeX() const { return _sizex; }
    double SizeY() const { return _sizey; }
    double SizeZ() const { return _sizez; }

    void setSizeX(double val) { _sizex = val; }
    void setSizeY(double val) { _sizey = val; }
    void setSizeZ(double val) { _sizez = val; }

    static Size3D Empty();

    bool IsEmpty() { return (_sizex == 0.0) && (_sizey == 0.0) && (_sizez == 0.0); }

    bool operator ==(const Size3D &s);
    bool operator !=(const Size3D &s);
    Size3D operator =(const Size3D &s);

    bool Equals(const Size3D &s);
    static bool Equals(const Size3D &s1, const Size3D &s2);

    int GetHashCode() {return 0; }
    QString ToString();

    Point3D ToPoint3D();
    Vector3D ToVector3d();

private:
    double _sizex;
    double _sizey;
    double _sizez;
};

bool operator ==(Size3D s1, Size3D s2);
bool operator !=(Size3D s1, Size3D s2);

}

#endif // SIZE3D_H
