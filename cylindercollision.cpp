#include "cylindercollision.h"

#include <cmath>
#include <algorithm>
#include <functional>

using namespace CreateCore;
//using namespace ModelViewer;

static const double EPS = 1e-6;
static const long multiplier = 1000000;

QList<double> CylinderCollision::LastFailure = QList<double>();

bool CylinderCollision::Seg3DIndexOrder(Segment3DIndex s1, Segment3DIndex s2)
{
    Point3D p1, p2;
    int index;

    index = std::get<0>(s1);
    if (index >= 0 && index < _positions.count())
        p1 = _positions.at(index);
    else
        return true;

    index = std::get<0>(s2);
    if (index >= 0 && index < _positions.count())
        p2 = _positions.at(index);
    else
        return true;

    return p1.z() < p2.z();
}

CylinderCollision::CylinderCollision(QList<Point3D> positions, Rect3D bounds, double res)
{
    // Initialize grid of triangle data
    _resolutionX = res;
    _resolutionY = res;

    _meshXOffset = bounds.Location().x();
    _meshYOffset = bounds.Location().y();

    _meshXLimit = ceil(bounds.SizeX() / 2.0 / _resolutionX);
    _meshXLimit = 2 * _meshXLimit + 1;
    _meshYLimit = ceil(bounds.SizeY() / 2.0 / _resolutionY);
    _meshYLimit = 2 * _meshYLimit + 1;


    //Point3DIndices = new List<Segment3DIndex>[meshXLimit, meshYLimit];
    _point3DIndices = new QList<Segment3DIndex>*[_meshXLimit];
    for (int i = 0; i < _meshXLimit; i++)
    {
        _point3DIndices[i] = new QList<Segment3DIndex>[_meshYLimit];
    }

    // Add all triangles into grid
    _div = round(res * multiplier);
    for (int p = 0; p < _positions.count(); p += 3)
    {
        for (int v = 0; v < 3; v++)
        {
            _p0Index = p + v;
            _p1Index = p + (v + 1) % 3;

            // Make sure p0 is lower than p1
            if (_positions.at(_p0Index).z() > _positions.at(_p1Index).z())
                std::swap(_p0Index, _p1Index);

            long x0 = round((_positions.at(_p0Index).x() - _meshXOffset) * multiplier);
            long y0 = round((_positions.at(_p0Index).y() - _meshYOffset) * multiplier);
            long x1 = round((_positions.at(_p1Index).x() - _meshXOffset) * multiplier);
            long y1 = round((_positions.at(_p1Index).y() - _meshYOffset) * multiplier);

            BresenhamLine(x0, y0, x1, y1);
        }
    }

    for (int i = 0; i < _meshXLimit; i++)
    {
        for (int j = 0; j < _meshYLimit; j++)
        {
            QList<Segment3DIndex> list = _point3DIndices[i][j];
            if (list.isEmpty())
                continue;

            using namespace std::placeholders;
            std::sort(list.begin(), list.end(), std::bind(&CylinderCollision::Seg3DIndexOrder, this, _1, _2));
        }
    }
}

void CylinderCollision::BresenhamLine(long x0, long y0, long x1, long y1)
{
    bool steep = abs(y1 - y0) > abs(x1 - x0);

    // Make sure Abs(slope) is less than 1
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    // Make line is in LHP
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    long dX = (x1 - x0) * _div;
    long dY = abs(y1 - y0) * _div;
    long err = dX / 2;
    long yinc = y0 < y1 ? 1 : -1;
    long ystep = yinc * _div;
    long y = y0;

    for (long x = x0; x <= x1; x += _div)
    {
        if (steep)
        {
            // plot(y / _div, x / _div)
            AddCurrentIndex((int)(y / _div), (int)(x / _div));
        }
        else
        {
            // plot(x / _div, y / _div)
            AddCurrentIndex((int)(x / _div), (int)(y / _div));
        }

        err = err - dY;
        if (err < 0)
        {
            y += ystep;
            err += dX;

            if (steep)
            {
                // plot(y / _div, x / _div)
                // plot((y - ystep) / _div, (x + _div) / _div)
                AddCurrentIndex((int)(y / _div), (int)(x / _div));
                AddCurrentIndex((int)((y - ystep) / _div), (int)((x + _div) / _div));
            }
            else
            {
                // plot(x / _div, y / _div)
                // plot((x + _div) / _div, (y - ystep) / _div)
                AddCurrentIndex((int)(x / _div), (int)(y / _div));
                AddCurrentIndex((int)((x + _div) / _div), (int)((y - ystep) / _div));
            }
        }
    }
}

void CylinderCollision::AddCurrentIndex(int i, int j)
{
    if (i < 0 || i >= _meshXLimit)
        return;
    if (j < 0 || j >= _meshYLimit)
        return;

    QList<Segment3DIndex> list = _point3DIndices[i][j];


    list.append(Segment3DIndex(_p0Index, _p1Index));
}

bool CylinderCollision::CollidesWithMesh(Segment3D cylinder, double radius)
{
    double minx = fmin(cylinder.P0().x(), cylinder.P1().x()) - radius;
    double maxx = fmax(cylinder.P0().x(), cylinder.P1().x()) + radius;
    double miny = fmin(cylinder.P0().y(), cylinder.P1().y()) - radius;
    double maxy = fmax(cylinder.P0().y(), cylinder.P1().y()) + radius;

    int mini = floor((minx - _meshXOffset) / _resolutionX);
    if (mini < 0) \
        mini = 0;

    int maxi = ceil((maxx - _meshXOffset) / _resolutionX);
    if (maxi >= _meshXLimit)
        maxi = _meshXLimit - 1;

    int minj = floor((miny - _meshYOffset) / _resolutionY);
    if (minj < 0)
        minj = 0;

    int maxj = ceil((maxy - _meshYOffset) / _resolutionY);
    if (maxj >= _meshYLimit)
        maxj = _meshYLimit - 1;

    //var indexHash = new HashSet<Segment3DIndex>(new Segment3DIndexComparer());
    QList<Segment3DIndex> indexHash;
    for (int i = mini; i <= maxi; i++)
    {
        for (int j = minj; j <= maxj; j++)
        {
            QList<Segment3DIndex> indexList = _point3DIndices[i][j];
            if (indexList.isEmpty())
                continue;

            for (int p = 0; p < indexList.count(); p++)
            {
                // Skip if top if cylinder is lower than lower point in segment
                Segment3DIndex currentIndex = indexList.at(p);
                if (cylinder.P1().z() < _positions.at(std::get<0>(currentIndex)).z())
                    break;

                // Determine if edge was checked before
                if (indexHash.contains(currentIndex))
                    break;
                else
                    indexHash.append(currentIndex);

                if (CollidesWithSegment3D(std::get<0>(currentIndex), std::get<1>(currentIndex), cylinder, radius))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool CylinderCollision::CollidesWithSegment3D(int i0, int i1, Segment3D cylinder, double radius)
{
    double cylMinZ = cylinder.P0().z();
    double cylMaxZ = cylinder.P1().z();

    double rSq = radius * radius;
    Point3D p0 = _positions.at(i0);
    Point3D p1 = _positions.at(i1);
    Segment3D seg(p0, p1);
    if (ClipEdge(seg, cylMinZ, cylMaxZ))
        if (DistanceSqBetweenSegments(seg, cylinder) < rSq)
            return true;

    return false;
}

bool CylinderCollision::CollidesWithMesh(Cylinder cyl)
{
    double minx = cyl._x - cyl._r;
    double maxx = cyl._x + cyl._r;
    double miny = cyl._y - cyl._r;
    double maxy = cyl._y + cyl._r;

    int mini = floor((minx - _meshXOffset) / _resolutionX);
    if (mini < 0)
        mini = 0;

    int maxi = ceil((maxx - _meshXOffset) / _resolutionX);
    if (maxi >= _meshXLimit)
        maxi = _meshXLimit - 1;

    int minj = floor((miny - _meshYOffset) / _resolutionY);
    if (minj < 0)
        minj = 0;

    int maxj = ceil((maxy - _meshYOffset) / _resolutionY);
    if (maxj >= _meshYLimit)
        maxj = _meshYLimit - 1;

    //var indexHash = new HashSet<Segment3DIndex>(new Segment3DIndexComparer());
    QList<Segment3DIndex> indexHash;
    for (int i = mini; i <= maxi; i++)
    {
        for (int j = minj; j <= maxj; j++)
        {
            QList<Segment3DIndex> indexList = _point3DIndices[i][j];
            if (indexList.isEmpty())
                continue;

            for (int p = 0; p < indexList.count(); p++)
            {
                // Skip if top if cylinder is lower than lower point in segment
                Segment3DIndex currentIndex = indexList.at(p);
                if (cyl._zTop < _positions.at(std::get<0>(currentIndex)).z()) break;

                // Determine if edge was checked before
                if (indexHash.contains(currentIndex))
                    break;
                else
                    indexHash.append(currentIndex);

                if (CollidesWithTriangle(std::get<0>(currentIndex), std::get<1>(currentIndex), cyl))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool CylinderCollision::CollidesWithTriangle(int i0, int i1, Cylinder cyl)
{
    double cylMinZ = cyl._zBot;
    double cylMaxZ = cyl._zTop;
    Segment3D cylinder(Point3D(cyl._x, cyl._y, cyl._zBot), Point3D(cyl._x, cyl._y, cyl._zTop));

    // Check cylinder intercept with edges
    double rSq = cyl._r * cyl._r;
    Point3D p0 = _positions.at(i0);
    Point3D p1 = _positions.at(i1);
    Segment3D seg(p0, p1);
    if (ClipEdge(seg, cylMinZ, cylMaxZ))
    {
        double t = DistanceSqBetweenSegments(seg, cylinder);
        if (t < rSq)
        {
            LastFailure = QList<double> { (double)i0, (double)i1, t, rSq };
            return true;
        }
    }
    return false;
}

bool CylinderCollision::ClipEdge(Segment3D edge, double minZ, double maxZ)
{
    if (edge.P1().z() < minZ || edge.P0().z() > maxZ) return false;

    if (fabs(edge.P0().z() - edge.P1().z()) > EPS)
    {
        if (edge.P0().z() < minZ)
        {
            double t = (minZ - edge.P0().z()) / (edge.P1().z() - edge.P0().z());
            edge.P0() = edge.P0() + (edge.P1() - edge.P0()) * t;
        }
        if (edge.P1().z() > maxZ)
        {
            double t = (maxZ - edge.P0().z()) / (edge.P1().z() - edge.P0().z());
            edge.P0() = edge.P0() + (edge.P1() - edge.P0()) * t;
        }
    }

    return true;
}

double CylinderCollision::DistanceSqBetweenSegments(Segment3D s0, Segment3D s1)
{
    Vector3D s0Direction = s0.P1() - s0.P0();
    Vector3D s1Direction = s1.P1() - s1.P0();
    Vector3D v = s0.P0() - s1.P0();
    double a = s0Direction.lengthSquared();
    double b = Vector3D::DotProduct(s0Direction, s1Direction);
    double c = s1Direction.lengthSquared();
    double d = Vector3D::DotProduct(s0Direction, v);
    double e = Vector3D::DotProduct(s1Direction, v);
    double determinant = a * c - b * b;

    double sNumerator = 0, sDenominator = determinant;
    double tNumerator = 0, tDenominator = determinant;
    if (determinant < EPS)
    {
        // Near singular matrix (lines parallel or colinear)
        sNumerator = 0.0;
        sDenominator = 1.0;
        tNumerator = e;
        tDenominator = c;
    }
    else
    {
        // Matrix not near singular, solve the 2 equations
        sNumerator = (b * e - c * d);
        tNumerator = (a * e - b * d);

        // Check if solution is in segment s0
        if (sNumerator < 0.0)
        {
            // If s < 0, set s = 0 to keep in segment
            sNumerator = 0.0;
            tNumerator = e;
            tDenominator = c;
        }
        else if (sNumerator > sDenominator)
        {
            // If s > 1, set s = 1 to keep in segment
            sNumerator = sDenominator;
            tNumerator = e + b;
            tDenominator = c;
        }
    }

    // Check if solution is in segment s1
    if (tNumerator < 0.0)
    {
        // If t < 0, set t = 0 to keep in segment
        tNumerator = 0.0;

        // Recompute s for new t
        if (-d < 0.0)
            sNumerator = 0.0;
        else if (-d > a)
            sNumerator = sDenominator;
        else
        {
            sNumerator = -d;
            sDenominator = a;
        }
    }
    else if (tNumerator > tDenominator)
    {
        // If t > 1, set t = 1 to keep in segment
        tNumerator = tDenominator;

        // Recompute s for new t
        if ((-d + b) < 0.0)
            sNumerator = 0;
        else if ((-d + b) > a)
            sNumerator = sDenominator;
        else
        {
            sNumerator = (-d + b);
            sDenominator = a;
        }
    }

    double s = sNumerator / sDenominator;
    double t = tNumerator / tDenominator;
    Vector3D dP = v + (s * s0Direction) - (t * s1Direction);

    return dP.lengthSquared();
}
