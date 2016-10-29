#include "meshbuilder.h"
#include "point3d.h"
#include "vector3d.h"
#include "rect3d.h"
#include "support.h"
#include "constants.h"
#include "rcexceptions.h"

using namespace CreateCore;

QList<QPointF> MeshBuilder::Circle = QList<QPointF>();
QMap<int, QList<QPointF> > MeshBuilder::CircleCache = QMap<int, QList<QPointF>>();
QMap<int, MeshGeometry3D*> MeshBuilder::UnitSphereCache = QMap<int, MeshGeometry3D*>();

MeshBuilder::MeshBuilder()
{
    MeshBuilder::MeshBuilder(true, true);
}

MeshBuilder::MeshBuilder(bool generateNormals, bool generateTextureCoordinates)
{
    positions.clear();
    triangleIndices.clear();

    if (generateNormals)
        normals = new QList<Vector3D>;
    else
        normals = 0;

    if (generateTextureCoordinates)
        textureCoordinates = new QList<QPointF>;
    else
        textureCoordinates = 0;
}

bool MeshBuilder::CreateNormals()
{
    //return !(normals.isNull());
    if (normals)
        return true;
    else
        return false;
}

void MeshBuilder::CreateNormals(bool value)
{
    //if (value && normals.isNull())
    if (value && normals == 0)
        normals = new QList<Vector3D>;

    if (!value)
        normals->clear();
}

bool MeshBuilder::CreateTextureCoordinates()
{
    //return !(textureCoordinates.isNull());
    if (textureCoordinates)
        return true;
    else
        return false;
}

void MeshBuilder::CreateTextureCoordinates(bool value)
{
    //if (value && textureCoordinates.isNull())
    if (value && textureCoordinates == 0)
        textureCoordinates = new QList<QPointF>;

    if (!value)
        textureCoordinates->clear();
}

QList<QPointF> MeshBuilder::GetCirle(int thetaDiv)
{
    QList<QPointF> circle;
    if(CircleCache.contains(thetaDiv))
        circle = CircleCache.value(thetaDiv);
    else
    {
        for (int i = 0; i < thetaDiv; i++)
        {
            double theta = cPI * 2 * ((double)i / (thetaDiv - 1));
            circle.append(QPointF(cos(theta), -sin(theta)));
        }
        CircleCache.insert(thetaDiv, circle);
    }

    return circle;
}

void MeshBuilder::AddArrow(Point3D point1, Point3D point2, double diameter, double headLength, int thetaDiv)
{
    Vector3D dir = point2 - point1;
    double length = dir.length();
    double r = diameter / 2;

    QList<QPointF> pc;
    pc.append(QPointF(0,0));
    pc.append(QPointF(0, r));
    pc.append(QPointF(length - (diameter * headLength), r));
    pc.append(QPointF(length - (diameter * headLength), r * 5));
    pc.append(QPointF(length, 0));

    AddRevolvedGeometry(pc, point1, dir, thetaDiv);
}

void MeshBuilder::AddManipulatorArrow(Point3D point1, Point3D point2, double diameter, double headLength, int thetaDiv)
{
    Vector3D dir = point2 - point1;
    double length = dir.length();
    double r = diameter / 2;

    QList<QPointF> pc;
    pc.append(QPointF(length - (diameter * headLength), 0));
    pc.append(QPointF(length - (diameter * headLength), r));
    pc.append(QPointF(length - (diameter * headLength), r * 5));
    pc.append(QPointF(length, 0));

    AddRevolvedGeometry(pc, point1, dir, thetaDiv);
}

void MeshBuilder::AddBoundingBox(Rect3D boundingBox, double diameter)
{
    Point3D p0(boundingBox.X(), boundingBox.Y(), boundingBox.Z());
    Point3D p1(boundingBox.X(), boundingBox.Y() + boundingBox.SizeY(), boundingBox.Z());
    Point3D p2(boundingBox.X() + boundingBox.SizeX(), boundingBox.Y() + boundingBox.SizeY(), boundingBox.Z());
    Point3D p3(boundingBox.X() + boundingBox.SizeX(), boundingBox.Y(), boundingBox.Z());
    Point3D p4(boundingBox.X(), boundingBox.Y(), boundingBox.Z() + boundingBox.SizeZ());
    Point3D p5(boundingBox.X(), boundingBox.Y() + boundingBox.SizeY(), boundingBox.Z() + boundingBox.SizeZ());
    Point3D p6(boundingBox.X() + boundingBox.SizeX(), boundingBox.Y() + boundingBox.SizeY(), boundingBox.Z() + boundingBox.SizeZ());
    Point3D p7(boundingBox.X() + boundingBox.SizeX(), boundingBox.Y(), boundingBox.Z() + boundingBox.SizeZ());

    AddCylinder(p0, p1, diameter, 10);
    AddCylinder(p1, p2, diameter, 10);
    AddCylinder(p2, p3, diameter, 10);
    AddCylinder(p3, p0, diameter, 10);

    AddCylinder(p4, p5, diameter, 10);
    AddCylinder(p5, p6, diameter, 10);
    AddCylinder(p6, p7, diameter, 10);
    AddCylinder(p7, p4, diameter, 10);

    AddCylinder(p0, p4, diameter, 10);
    AddCylinder(p1, p5, diameter, 10);
    AddCylinder(p2, p6, diameter, 10);
    AddCylinder(p3, p7, diameter, 10);
}

void MeshBuilder::AddBox(Point3D center, double xlength, double ylength, double zlength)
{
    AddBox(center, xlength, ylength, zlength, bfAll);
}

void MeshBuilder::AddBox(Rect3D rectangle)
{
    AddBox(Point3D(rectangle.X() + (rectangle.SizeX() * 0.5), rectangle.Y() + (rectangle.SizeY() * 0.5), rectangle.Z() + (rectangle.SizeZ() * 0.5)),
           rectangle.SizeX(), rectangle.SizeY(), rectangle.SizeZ(), bfAll);
}

void MeshBuilder::AddBox(Point3D center, double xlength, double ylength, double zlength, BoxFaces faces)
{
    if ((faces & bfFront) == bfFront)
    {
        AddCubeFace(center, Vector3D(1, 0, 0), Vector3D(0, 0, 1), xlength, ylength, zlength);
    }

    if ((faces & bfBack) == bfBack)
    {
        AddCubeFace(center, Vector3D(-1, 0, 0), Vector3D(0, 0, 1), xlength, ylength, zlength);
    }

    if ((faces & bfLeft) == bfLeft)
    {
        AddCubeFace(center, Vector3D(0, -1, 0), Vector3D(0, 0, 1), ylength, xlength, zlength);
    }

    if ((faces & bfRight) == bfRight)
    {
        AddCubeFace(center, Vector3D(0, 1, 0), Vector3D(0, 0, 1), ylength, xlength, zlength);
    }

    if ((faces & bfTop) == bfTop)
    {
        AddCubeFace(center, Vector3D(0, 0, 1), Vector3D(0, 1, 0), zlength, xlength, ylength);
    }

    if ((faces & bfBottom) == bfBottom)
    {
        AddCubeFace(center, Vector3D(0, 0, -1), Vector3D(0, 1, 0), zlength, xlength, ylength);
    }
}

void MeshBuilder::AddCone(Point3D origin, Vector3D direction, double baseRadius, double topRadius,
                          double height, bool baseCap, bool topCap, int thetaDiv)
{
    QList<QPointF> pc;
    if (baseCap)
    {
        pc.append(QPointF(0, 0));
    }

    pc.append(QPointF(0, baseRadius));
    pc.append(QPointF(height, topRadius));
    if (topCap)
    {
        pc.append(QPointF(height, 0));
    }

    AddRevolvedGeometry(pc, origin, direction, thetaDiv);
}

void MeshBuilder::AddCone(Point3D origin, Vector3D direction, double baseRadius,
                          double topRadius, double height, int thetaDiv)
{
    QList<QPointF> pc;
    pc.append(QPointF(0, baseRadius));
    pc.append(QPointF(height, topRadius));

    AddRevolvedGeometry(pc, origin, direction, thetaDiv);
}

void MeshBuilder::AddCone(Point3D origin, Point3D apex, double baseRadius, bool baseCap, int thetaDiv)
{
    Vector3D dir = apex - origin;
    AddCone(origin, dir, baseRadius, 0, dir.length(), baseCap, false, thetaDiv);
}

void MeshBuilder::AddCubeFace(Point3D center, Vector3D normal, Vector3D up, double dist, double width, double height)
{
    Vector3D right = Vector3D::CrossProduct(normal, up);
    Vector3D n = normal * dist / 2;
    up *= height / 2;
    right *= width / 2;
    Point3D p1 = center + n - up - right;
    Point3D p2 = center + n - up + right;
    Point3D p3 = center + n + up + right;
    Point3D p4 = center + n + up - right;

    int i0 = positions.count();
    positions.append(p1);
    positions.append(p2);
    positions.append(p3);
    positions.append(p4);
    //if (!normals.isNull())
    if (normals)
    {
        normals->append(normal);
        normals->append(normal);
        normals->append(normal);
        normals->append(normal);
    }

    //if (!textureCoordinates.isNull())
    if (textureCoordinates)
    {
        textureCoordinates->append(QPointF(1, 1));
        textureCoordinates->append(QPointF(0, 1));
        textureCoordinates->append(QPointF(0, 0));
        textureCoordinates->append(QPointF(1, 0));
    }

    triangleIndices.append(i0 + 2);
    triangleIndices.append(i0 + 1);
    triangleIndices.append(i0 + 0);
    triangleIndices.append(i0 + 0);
    triangleIndices.append(i0 + 3);
    triangleIndices.append(i0 + 2);
}

void MeshBuilder::AddCylinder(Point3D p1, Point3D p2, double diameter, int thetaDiv)
{
    Vector3D n = p2 - p1;
    double l = n.length();
    n.normalize();
    AddCone(p1, n, diameter / 2, diameter / 2, l, false, false, thetaDiv);
}

void MeshBuilder::AddEdges(QList<Point3D> points, QList<int> edges, double diameter, int thetaDiv)
{
    for (int i = 0; i < edges.count() - 1; i += 2)
    {
        AddCylinder(points.at(edges.at(i)), points.at(edges.at(i + 1)), diameter, thetaDiv);
    }
}

void MeshBuilder::AddExtrudedGeometry(QList<QPointF> points, Vector3D xaxis, Point3D p0, Point3D p1)
{
    Vector3D ydirection = Vector3D::CrossProduct(xaxis, p1 - p0);
    ydirection.normalize();
    xaxis.normalize();

    int index0 = positions.count();
    int np = 2 * points.count();
    foreach (QPointF p ,points)
    {
        Vector3D v = (xaxis * p.x()) + (ydirection * p.y());
        positions.append(p0 + v);
        positions.append(p1 + v);
        v.normalize();
        //if (!normals.isNull())
        if (normals)
        {
            normals->append(v);
            normals->append(v);
        }

        //if (!textureCoordinates.isNull())
        if (textureCoordinates)
        {
            textureCoordinates->append(QPointF(0, 0));
            textureCoordinates->append(QPointF(1, 0));
        }

        int i1 = index0 + 1;
        int i2 = (index0 + 2) % np;
        int i3 = ((index0 + 2) % np) + 1;

        triangleIndices.append(i1);
        triangleIndices.append(i2);
        triangleIndices.append(index0);

        triangleIndices.append(i1);
        triangleIndices.append(i3);
        triangleIndices.append(i2);
    }
}

void MeshBuilder::AddLoftedGeometry(QList<QList<Point3D> > positionsList, QList<QList<Vector3D> > normalList,
                                    QList<QList<QPointF> > textureCoordinateList)
{
    int index0 = positions.count();
    int n = -1;
    for (int i = 0; i < positionsList.count(); i++)
    {
        QList<Point3D> pc = positionsList.at(i);

        // check that all curves have same number of points
        if (n == -1)
        {
            n = pc.count();
        }

        if (pc.count() != n)
        {
            throw new InvalidOperationException(AllCurvesShouldHaveTheSameNumberOfPoints);
        }

        // add the points
        foreach (Point3D p, pc)
        {
            positions.append(p);
        }

        // add normals
        //if (!normals.isNull() && !normalList.isEmpty())
        if (normals && !normalList.isEmpty())
        {
            QList<Vector3D> nc = normalList.at(i);
            foreach (Vector3D normal, nc)
            {
                normals->append(normal);
            }
        }

        // add texcoords
        //if (!textureCoordinates.isNull() && !textureCoordinateList.isEmpty())
        if (textureCoordinates && !textureCoordinateList.isEmpty())
        {
            QList<QPointF> tc = textureCoordinateList.at(i);
            foreach (QPointF t, tc)
            {
                textureCoordinates->append(t);
            }
        }
    }

    for (int i = 0; i + 1 < positionsList.count(); i++)
    {
        for (int j = 0; j + 1 < n; j++)
        {
            int i0 = index0 + (i * n) + j;
            int i1 = i0 + n;
            int i2 = i1 + 1;
            int i3 = i0 + 1;
            triangleIndices.append(i0);
            triangleIndices.append(i1);
            triangleIndices.append(i2);

            triangleIndices.append(i2);
            triangleIndices.append(i3);
            triangleIndices.append(i0);
        }
    }
}

void MeshBuilder::AddNode(Point3D position, Vector3D normal, QPointF textureCoordinate)
{
    positions.append(position);

    //if (!normals.isNull())
    if (normals)
    {
        normals->append(normal);
    }

    //if (!textureCoordinates.isNull())
    if (textureCoordinates)
    {
        textureCoordinates->append(textureCoordinate);
    }
}

void MeshBuilder::AddPipe(Point3D point1, Point3D point2, double innerDiameter, double diameter, int thetaDiv)
{
    Vector3D dir = point2 - point1;

    double height = dir.length();
    dir.normalize();

    QList<QPointF> pc;
    pc.append(QPointF(0, innerDiameter / 2));
    pc.append(QPointF(0, diameter / 2));
    pc.append(QPointF(height, diameter / 2));
    pc.append(QPointF(height, innerDiameter / 2));

    if (innerDiameter > 0)
    {
        // Add the inner surface
        pc.append(QPointF(0, innerDiameter / 2));
    }

    AddRevolvedGeometry(pc, point1, dir, thetaDiv);
}

void MeshBuilder::AddPolygon(QList<Point3D> points)
{
    switch (points.count())
    {
    case 3:
        AddTriangle(points.at(0), points.at(1), points.at(2));
        break;
    case 4:
        AddQuad(points.at(0), points.at(1), points.at(2), points.at(3));
        break;
    default:
        AddTriangleFan(points, QList<Vector3D>(), QList<QPointF>());
        break;
    }
}

void MeshBuilder::AddPyramid(Point3D center, double sideLength, double height)
{
    Point3D p1(center.x() - (sideLength * 0.5), center.y() - (sideLength * 0.5), center.z());
    Point3D p2(center.x() + (sideLength * 0.5), center.y() - (sideLength * 0.5), center.z());
    Point3D p3(center.x() + (sideLength * 0.5), center.y() + (sideLength * 0.5), center.z());
    Point3D p4(center.x() - (sideLength * 0.5), center.y() + (sideLength * 0.5), center.z());
    Point3D p5(center.x(), center.x(), center.z() + height);
    AddTriangle(p1, p2, p5);
    AddTriangle(p2, p3, p5);
    AddTriangle(p3, p4, p5);
    AddTriangle(p4, p1, p5);
}

void MeshBuilder::AddQuad(Point3D p0, Point3D p1, Point3D p2, Point3D p3)
{
    QPointF uv0(0, 0);
    QPointF uv1(1, 0);
    QPointF uv2(1, 1);
    QPointF uv3(0, 1);
    AddQuad(p0, p1, p2, p3, uv0, uv1, uv2, uv3);
}

void MeshBuilder::AddQuad(Point3D p0, Point3D p1, Point3D p2, Point3D p3, QPointF uv0, QPointF uv1, QPointF uv2, QPointF uv3)
{
    int i0 = positions.count();

    positions.append(p0);
    positions.append(p1);
    positions.append(p2);
    positions.append(p3);

    //if (!textureCoordinates.isNull())
    if (textureCoordinates)
    {
        textureCoordinates->append(uv0);
        textureCoordinates->append(uv1);
        textureCoordinates->append(uv2);
        textureCoordinates->append(uv3);
    }

    //if (!normals.isNull())
    if (normals)
    {
        Vector3D w = Vector3D::CrossProduct(p3 - p0, p1 - p0);
        w.normalize();
        normals->append(w);
        normals->append(w);
        normals->append(w);
        normals->append(w);
    }

    triangleIndices.append(i0 + 0);
    triangleIndices.append(i0 + 1);
    triangleIndices.append(i0 + 2);

    triangleIndices.append(i0 + 2);
    triangleIndices.append(i0 + 3);
    triangleIndices.append(i0 + 0);
}

void MeshBuilder::AddQuads(QList<Point3D> quadPositions, QList<Vector3D> quadNormals, QList<QPointF> quadTextureCoordinates)
{
    if (quadPositions.isEmpty())
    {
        throw new ArgumentNullException("quadPositions");
    }

    //if (!normals.isNull() && quadNormals.isEmpty())
    if (normals && quadNormals.isEmpty())
    {
        throw new ArgumentNullException("quadNormals");
    }

    //if (!textureCoordinates.isNull() && quadTextureCoordinates.isEmpty())
    if (textureCoordinates && quadTextureCoordinates.isEmpty())
    {
        throw new ArgumentNullException("quadTextureCoordinates");
    }

    if (quadNormals.count() != quadPositions.count())
    {
        throw new InvalidOperationException(WrongNumberOfNormals);
    }

    if (quadTextureCoordinates.count() != quadPositions.count())
    {
        throw new InvalidOperationException(WrongNumberOfTextureCoordinates);
    }

    //Debug.Assert(quadPositions.Count > 0 && quadPositions.Count % 4 == 0, "Wrong number of positions.");

    int index0 = positions.count();
    foreach (Point3D p, quadPositions)
    {
        positions.append(p);
    }

    //if (!textureCoordinates.isNull() && !quadTextureCoordinates.isEmpty())
    if (textureCoordinates && !quadTextureCoordinates.isEmpty())
    {
        foreach (QPointF tc, quadTextureCoordinates)
        {
            textureCoordinates->append(tc);
        }
    }

    //if (!normals.isNull() && !quadNormals.isEmpty())
    if (normals && !quadNormals.isEmpty())
    {
        foreach (Vector3D n, quadNormals)
        {
            normals->append(n);
        }
    }

    int indexEnd = positions.count();
    for (int i = index0; i + 3 < indexEnd; i++)
    {
        triangleIndices.append(i);
        triangleIndices.append(i + 1);
        triangleIndices.append(i + 2);

        triangleIndices.append(i + 2);
        triangleIndices.append(i + 3);
        triangleIndices.append(i);
    }
}

void MeshBuilder::AddRectangularMesh(QList<Point3D> points, int columns)
{
    if (points.isEmpty())
    {
        throw new ArgumentNullException("points");
    }

    int index0 = positions.count();

    foreach (Point3D pt, points)
    {
        positions.append(pt);
    }

    int rows = points.count() / columns;

    AddRectangularMeshTriangleIndices(index0, rows, columns);
    //if (!normals.isNull())
    if (normals)
    {
        AddRectangularMeshNormals(index0, rows, columns);
    }

    //if (!textureCoordinates.isNull())
    if (textureCoordinates)
    {
        AddRectangularMeshTextureCoordinates(rows, columns);
    }
}

void MeshBuilder::AddRectangularMesh(QList<QList<Point3D> > points, QList<QList<QPointF> > texCoords,
                                     bool closed0, bool closed1)
{
    if (points.isEmpty())
    {
        throw new ArgumentNullException("points");
    }

    int rows = points.count();
    int columns = points.at(0).count();
    int index0 = positions.count();
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            positions.append(points.at(i).at(j));
        }
    }

    AddRectangularMeshTriangleIndices(index0, rows, columns, closed0, closed1);

    //if(!normals.isNull())
    if (normals)
    {
        AddRectangularMeshNormals(index0, rows, columns);
    }

    //if (!textureCoordinates.isNull())
    if (textureCoordinates)
    {
        if (!texCoords.isEmpty())
        {
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < columns; j++)
                {
                    textureCoordinates->append(texCoords.at(i).at(j));
                }
            }
        }
        else
        {
            AddRectangularMeshTextureCoordinates(rows, columns);
        }
    }
}

void MeshBuilder::AddRegularIcosahedron(Point3D center, double radius, bool shareVertices)
{
    double a = sqrt(2.0 / (5.0 + sqrt(5.0)));
    double b = sqrt(2.0 / (5.0 - sqrt(5.0)));

    //int icosahedronIndices[] =
    QList<int> icosahedronIndices =
    {
        1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4, 1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2, 3, 10, 7,
        10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0, 10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
    };

    QList<Vector3D> icosahedronVertices =
    {
        Vector3D(-a, 0, b), Vector3D(a, 0, b), Vector3D(-a, 0, -b), Vector3D(a, 0, -b),
        Vector3D(0, b, a), Vector3D(0, b, -a), Vector3D(0, -b, a), Vector3D(0, -b, -a),
        Vector3D(b, a, 0), Vector3D(-b, a, 0), Vector3D(b, -a, 0), Vector3D(-b, -a, 0)
    };

    if (shareVertices)
    {
        int index0 = positions.count();
        foreach (Vector3D v, icosahedronVertices)
        {
            positions.append(center + (v * radius));
        }

        foreach (int i, icosahedronIndices)
        {
            triangleIndices.append(index0 + i);
        }
    }
    else
    {
        int sz = sizeof(icosahedronIndices) / sizeof(int);
        for (int i = 0; i + 2 < sz; i += 3)
        {
            AddTriangle(center + (icosahedronVertices[icosahedronIndices[i]] * radius),
                        center + (icosahedronVertices[icosahedronIndices[i + 1]] * radius),
                        center + (icosahedronVertices[icosahedronIndices[i + 2]] * radius));
        }
    }
}

void MeshBuilder::AddRevolvedGeometry(QList<QPointF> points, Point3D origin, Vector3D direction, int thetaDiv)
{
    direction.normalize();

    // Find two unit vectors orthogonal to the specified direction
    Vector3D u = direction.FindAnyPerpindicular();
    Vector3D v = Vector3D::CrossProduct(direction, u);

    u.normalize();
    v.normalize();

    QList<QPointF> circle;
    if (!CircleCache.contains(thetaDiv))
    {
        circle.clear();
        for (int i = 0; i < thetaDiv; i++)
        {
            double theta = cPI * 2 * ((double)i / (thetaDiv - 1));
            circle.append(QPointF(cos(theta), -sin(theta)));
        }
        CircleCache.insert(thetaDiv, circle);
    }

    Circle = circle;
    int index0 = positions.count();
    int n = points.count();

    int totalNodes = (points.count() - 1) * 2 * thetaDiv;
    int rowNodes = (points.count() - 1) * 2;

    Point3D last;

    for (int i = 0; i < thetaDiv; i++)
    {
        Vector3D w = (v * circle[i].x()) + (u * circle[i].y());
        for (int j = 0; j + 1 < n; j++)
        {
            QPointF p1 = points.at(j);
            QPointF p2 = points.at(j + 1);

            // Add segment
            Point3D q1 = origin + (direction * p1.x()) + (w * p1.y());
            Point3D q2 = origin + (direction * p2.x()) + (w * p2.y());

            if (thetaDiv == 60)
            {
                int seg = (thetaDiv / 4);
                int s = i % seg;
                if ((s > seg - 3 || s < 2))
                {
                    p2.setY(p2.y() - 0.9);

                    q1 = origin + (direction * p1.x()) + (w * p1.y());
                    q2 = origin + (direction * p2.x()) + (w * p2.y());

                    last = q1;
                }
                else if ((s > seg - 6 || s < 5))
                {
                    q1 = origin + (direction * p1.x()) + (w * p1.y());
                    q2 = origin + (direction * p2.x()) + (w * p2.y());

                    last = q1;
                }
                else
                {
                    q1 = Point3D(); //origin + (direction * p1.X) + (w * p1.Y);
                    q2 = Point3D(); //origin + (direction * p2.X) + (w * p2.Y);
                }
            }


            // todo:should not add segment if q1==q2 (corner point)
            // const double eps = 1e-6;
            // if (Point3D.Subtract(q1, q2).LengthSquared < eps)
            // continue;
            double tx = points.at(j + 1).x() - points.at(j).x();
            double ty = points.at(j + 1).y() - points.at(j).y();

            Vector3D normal = (-direction * ty) + (w * tx);
            normal.normalize();

            positions.append(q1);
            positions.append(q2);

            //if (!normals.isNull())
            if (normals)
            {
                normals->append(normal);
                normals->append(normal);
            }

            //if (!textureCoordinates.isNull())
            if (textureCoordinates)
            {
                textureCoordinates->append(QPointF((double)i / (thetaDiv - 1), (double)j / (n - 1)));
                textureCoordinates->append(QPointF((double)i / (thetaDiv - 1), (double)(j + 1) / (n - 1)));
            }

            int i0 = index0 + (i * rowNodes) + (j * 2);
            int i1 = i0 + 1;
            int i2 = index0 + ((((i + 1) * rowNodes) + (j * 2)) % totalNodes);
            int i3 = i2 + 1;

            triangleIndices.append(i1);
            triangleIndices.append(i0);
            triangleIndices.append(i2);

            triangleIndices.append(i1);
            triangleIndices.append(i2);
            triangleIndices.append(i3);
        }
    }
}

void MeshBuilder::AddSubdivisionSphere(Point3D center, double radius, int subdivisions)
{
    int p0 = positions.count();
    Append(GetUnitSphere(subdivisions));
    int p1 = positions.count();
    for (int i = p0; i < p1; i++)
    {
        positions[i] = center + (radius * Point3D::ToVector3D(positions.at(i)));
    }
}

void MeshBuilder::AddSphere(Point3D center, double radius, int thetaDiv, int phiDiv)
{
    //int index0 = positions->count();
    double dt = 2 * cPI / thetaDiv;
    double dp = cPI / phiDiv;

    for (int pi = 0; pi <= phiDiv; pi++)
    {
        double phi = pi * dp;

        for (int ti = 0; ti <= thetaDiv; ti++)
        {
            // we want to start the mesh on the x axis
            double theta = ti * dt;

            // Spherical coordinates
            // http://mathworld.wolfram.com/SphericalCoordinates.html
            double x = cos(theta) * sin(phi);
            double y = sin(theta) * sin(phi);
            double z = cos(phi);

            Point3D p(center.x() + (radius * x), center.y() + (radius * y), center.z() + (radius * z));
            positions.append(p);

            //if (!normals.isNull())
            if (normals)
            {
                Vector3D n(x, y, z);
                normals->append(n);
            }

            //if (!textureCoordinates.isNull())
            if (textureCoordinates)
            {
                QPointF uv(theta / (2 * cPI), phi / cPI);
                textureCoordinates->append(uv);
            }
        }
    }
}

void MeshBuilder::AddTriangle(Point3D p0, Point3D p1, Point3D p2)
{
    QPointF uv0(0, 0);
    QPointF uv1(1, 0);
    QPointF uv2(0, 1);
    AddTriangle(p0, p1, p2, uv0, uv1, uv2);
}

void MeshBuilder::AddTriangle(Point3D p0, Point3D p1, Point3D p2, QPointF uv0, QPointF uv1, QPointF uv2)
{
    int i0 = positions.count();

    positions.append(p0);
    positions.append(p1);
    positions.append(p2);

    //if (!textureCoordinates.isNull())
    if (textureCoordinates)
    {
        textureCoordinates->append(uv0);
        textureCoordinates->append(uv1);
        textureCoordinates->append(uv2);
    }

    //if (!normals.isNull())
    if (normals)
    {
        Vector3D w = Vector3D::CrossProduct(p1 - p0, p2 - p0);
        w.normalize();
        normals->append(w);
        normals->append(w);
        normals->append(w);
    }

    triangleIndices.append(i0 + 0);
    triangleIndices.append(i0 + 1);
    triangleIndices.append(i0 + 2);
}

void MeshBuilder::AddTriangleFan(QList<int> vertices)
{
    for (int i = 0; i + 2 < vertices.count(); i++)
    {
        triangleIndices.append(vertices.at(0));
        triangleIndices.append(vertices.at(i + 1));
        triangleIndices.append(vertices.at(i + 2));
    }
}

void MeshBuilder::AddTriangleFan(QList<Point3D> fanPositions, QList<Vector3D> fanNormals,
                                 QList<QPointF> fanTextureCoordinates)
{
    if (fanPositions.isEmpty())
    {
        throw new ArgumentNullException("fanPositions");
    }

    //if (!normals.isNull() && fanNormals.isEmpty())
    if (normals && fanNormals.isEmpty())
    {
        throw new ArgumentNullException("fanNormals");
    }

    //if (!textureCoordinates.isNull() && fanTextureCoordinates.isEmpty())
    if (textureCoordinates && fanTextureCoordinates.isEmpty())
    {
        throw new ArgumentNullException("fanTextureCoordinates");
    }

    int index0 = positions.count();
    foreach (Point3D p, fanPositions)
    {
        positions.append(p);
    }

    //if (!textureCoordinates.isNull() && !fanTextureCoordinates.isEmpty())
    if (textureCoordinates && !fanTextureCoordinates.isEmpty())
    {
        foreach (QPointF tc, fanTextureCoordinates)
        {
            textureCoordinates->append(tc);
        }
    }

    //if (!normals.isNull() && !fanNormals.isEmpty())
    if (normals && !fanNormals.isEmpty())
    {
        foreach (Vector3D n, fanNormals)
        {
            normals->append(n);
        }
    }

    int indexEnd = positions.count();
    for (int i = index0; i + 2 < indexEnd; i++)
    {
        triangleIndices.append(index0);
        triangleIndices.append(i + 1);
        triangleIndices.append(i + 2);
    }
}

void MeshBuilder::AddTriangleStrip(QList<Point3D> stripPositions, QList<Vector3D> stripNormals,
                                   QList<QPointF> stripTextureCoordinates)
{
    if (stripPositions.isEmpty())
    {
        throw new ArgumentNullException("stripPositions");
    }

    //if (!normals.isNull() && stripNormals.isEmpty())
    if (normals && stripNormals.isEmpty())
    {
        throw new ArgumentNullException("stripNormals");
    }

    //if (!textureCoordinates.isNull() && stripTextureCoordinates.isEmpty())
    if (textureCoordinates && stripTextureCoordinates.isEmpty())
    {
        throw new ArgumentNullException("stripTextureCoordinates");
    }

    if (stripNormals.count() != stripPositions.count())
    {
        throw new InvalidOperationException(WrongNumberOfNormals);
    }

    if (stripTextureCoordinates.count() != stripPositions.count())
    {
        throw new InvalidOperationException(WrongNumberOfTextureCoordinates);
    }

    int index0 = positions.count();
    for (int i = 0; i < stripPositions.count(); i++)
    {
        positions.append(stripPositions[i]);
        //if (!normals.isNull() && !stripNormals.isEmpty())
        if (normals && !stripNormals.isEmpty())
        {
            normals->append(stripNormals[i]);
        }

        //if (!textureCoordinates.isNull() && !stripTextureCoordinates.isEmpty())
        if (textureCoordinates && !stripTextureCoordinates.isEmpty())
        {
            textureCoordinates->append(stripTextureCoordinates[i]);
        }
    }

    int indexEnd = positions.count();
    for (int i = index0; i + 2 < indexEnd; i += 2)
    {
        triangleIndices.append(i);
        triangleIndices.append(i + 1);
        triangleIndices.append(i + 2);

        if (i + 3 < indexEnd)
        {
            triangleIndices.append(i + 1);
            triangleIndices.append(i + 3);
            triangleIndices.append(i + 2);
        }
    }
}

void MeshBuilder::AddPolygon(QList<int> vertexIndices)
{
    int n = vertexIndices.count();
    for (int i = 0; i + 2 < n; i++)
    {
        triangleIndices.append(vertexIndices[0]);
        triangleIndices.append(vertexIndices[i + 1]);
        triangleIndices.append(vertexIndices[i + 2]);
    }
}

void MeshBuilder::AddTriangles(QList<Point3D> trianglePositions, QList<Vector3D> triangleNormals,
                               QList<QPointF> triangleTextureCoordinates)
{
    if (trianglePositions.isEmpty())
    {
        throw new ArgumentNullException("trianglePositions");
    }

    //if (!normals.isNull() && triangleNormals.isEmpty())
    if (normals && triangleNormals.isEmpty())
    {
        throw new ArgumentNullException("triangleNormals");
    }

    //if (!textureCoordinates.isNull() && triangleTextureCoordinates.isEmpty())
    if (textureCoordinates && triangleTextureCoordinates.isEmpty())
    {
        throw new ArgumentNullException("triangleTextureCoordinates");
    }

    if (trianglePositions.count() % 3 != 0)
    {
        throw new InvalidOperationException(WrongNumberOfPositions);
    }

    if (triangleNormals.count() != trianglePositions.count())
    {
        throw new InvalidOperationException(WrongNumberOfNormals);
    }

    if (triangleTextureCoordinates.count() != trianglePositions.count())
    {
        throw new InvalidOperationException(WrongNumberOfTextureCoordinates);
    }

    int index0 = positions.count();
    foreach (Point3D p, trianglePositions)
    {
        positions.append(p);
    }

    //if (!textureCoordinates.isNull() && !triangleTextureCoordinates.isEmpty())
    if (textureCoordinates && !triangleTextureCoordinates.isEmpty())
    {
        foreach (QPointF tc, triangleTextureCoordinates)
        {
            textureCoordinates->append(tc);
        }
    }

    //if (!normals.isNull() && !triangleNormals.isEmpty())
    if (normals && !triangleNormals.isEmpty())
    {
        foreach (Vector3D n, triangleNormals)
        {
            normals->append(n);
        }
    }

    int indexEnd = positions.count();
    for (int i = index0; i < indexEnd; i++)
    {
        triangleIndices.append(i);
    }
}

void MeshBuilder::AddTube(QList<Point3D> path, QList<double> values, QList<double> diameters, int thetaDiv, bool isTubeClosed)
{
    QList<QPointF> circle = GetCirle(thetaDiv);
    AddTube(path, values, diameters, circle, isTubeClosed, true);
}

void MeshBuilder::AddTube(QList<Point3D> path, double diameter, int thetaDiv, bool isTubeClosed)
{
    QList<double> d;
    d.append(diameter);
    AddTube(path, QList<double>(), d, thetaDiv, isTubeClosed);
}

void MeshBuilder::AddTube(QList<Point3D> path, QList<double> values, QList<double> diameters,
                          QList<QPointF> section, bool isTubeClosed, bool isSectionClosed)
{
    if (values.isEmpty())
    {
        throw new InvalidOperationException(WrongNumberOfTextureCoordinates);
    }

    if (diameters.isEmpty())
    {
        throw new InvalidOperationException(WrongNumberOfDiameters);
    }

    int index0 = positions.count();
    int pathLength = path.count();
    int sectionLength = section.count();
    if (pathLength < 2 || sectionLength < 2)
    {
        return;
    }

    Vector3D vec = path.at(1) - path.at(0);
    Vector3D up = vec.FindAnyPerpindicular();

    int diametersCount = diameters.count();
    int valuesCount = values.count();

    for (int i = 0; i < pathLength; i++)
    {
        double r = !diameters.isEmpty() ? diameters[i % diametersCount] / 2 : 1;
        int i0 = i > 0 ? i - 1 : i;
        int i1 = i + 1 < pathLength ? i + 1 : i;

        Vector3D forward = path[i1] - path[i0];
        Vector3D right = Vector3D::CrossProduct(up, forward);
        up = Vector3D::CrossProduct(forward, right);
        up.normalize();
        right.normalize();
        Vector3D u = right;
        Vector3D v = up;
        for (int j = 0; j < sectionLength; j++)
        {
            Vector3D w = (section.at(j).x() * u * r) + (section.at(j).y() * v * r);
            Point3D q = path[i] + w;
            positions.append(q);
            //if (!normals.isNull())
            if (normals)
            {
                w.normalize();
                normals->append(w);
            }

            //if (!textureCoordinates.isNull())
            if (textureCoordinates)
            {
                textureCoordinates->append(!values.isEmpty() ? QPointF(values[i % valuesCount], (double)j / (sectionLength - 1)): QPointF());
            }
        }
    }

    AddRectangularMeshTriangleIndices(index0, pathLength, sectionLength, isSectionClosed, isTubeClosed);
}

void MeshBuilder::Append(MeshBuilder mesh)
{
    Append(mesh.Positions(), mesh.TriangleIndices(), *mesh.Normals(), *mesh.TextureCoordinates());
}

void MeshBuilder::Append(MeshGeometry3D* mesh)
{
    Append(mesh->Positions, mesh->TriangleIndices, mesh->Normals, mesh->TextureCoordinates);     // mesh.Normals, mesh.TextureCoordinates);
}

void MeshBuilder::Append(QList<Point3D> positionsToAppend, QList<int> triangleIndicesToAppend,
                         QList<Vector3D> normalsToAppend, QList<QPointF> textureCoordinatesToAppend)
{
    if (positionsToAppend.isEmpty())
    {
        throw new ArgumentNullException("positionsToAppend");
    }

    //if (!normals.isNull() && normalsToAppend.isEmpty())
    if (normals && normalsToAppend.isEmpty())
    {
        throw new InvalidOperationException(SourceMeshNormalsShouldNotBeNull);
    }

    //if (!textureCoordinates.isNull() && textureCoordinatesToAppend.isEmpty())
    if (textureCoordinates && textureCoordinatesToAppend.isEmpty())
    {
        throw new InvalidOperationException(SourceMeshTextureCoordinatesShouldNotBeNull);
    }

    if (normalsToAppend.count() != positionsToAppend.count())
    {
        throw new InvalidOperationException(WrongNumberOfNormals);
    }

    if (textureCoordinatesToAppend.count() != positionsToAppend.count())
    {
        throw new InvalidOperationException(WrongNumberOfTextureCoordinates);
    }

    int index0 = positions.count();
    foreach (Point3D p, positionsToAppend)
    {
        positions.append(p);
    }

    //if (!normals.isNull() && !normalsToAppend.isEmpty())
    if (normals && !normalsToAppend.isEmpty())
    {
        foreach (Vector3D n, normalsToAppend)
        {
            normals->append(n);
        }
    }

    //if (!textureCoordinates.isNull() && !textureCoordinatesToAppend.isEmpty())
    if (textureCoordinates && !textureCoordinatesToAppend.isEmpty())
    {
        foreach (QPointF t, textureCoordinatesToAppend)
        {
            textureCoordinates->append(t);
        }
    }

    foreach (int i,triangleIndicesToAppend)
    {
        triangleIndices.append(index0 + i);
    }
}

void MeshBuilder::AppendTo(MeshGeometry3D target, MeshGeometry3D addition)
{
    QList<Point3D> positionsToAppend = addition.Positions;
    QList<int> triangleIndicesToAppend = addition.TriangleIndices;
    QList<Vector3D> normalsToAppend = addition.Normals;
    QList<QPointF> textureCoordinatesToAppend = addition.TextureCoordinates;

    if (positionsToAppend.isEmpty())
    {
        throw new ArgumentNullException("positionsToAppend");
    }

    if (!target.Normals.isEmpty() && normalsToAppend.isEmpty())
    {
        throw new InvalidOperationException(SourceMeshNormalsShouldNotBeNull);
    }

    if (!target.TextureCoordinates.isDetached() && textureCoordinatesToAppend.isEmpty())
    {
        throw new InvalidOperationException(SourceMeshTextureCoordinatesShouldNotBeNull);
    }

    if (!textureCoordinatesToAppend.isEmpty() && textureCoordinatesToAppend.count() != positionsToAppend.count())
    {
        throw new InvalidOperationException(WrongNumberOfTextureCoordinates);
    }

    int index0 = target.Positions.count();
    foreach (Point3D p, positionsToAppend)
    {
        target.Positions.append(p);
    }

    if (!target.TextureCoordinates.isEmpty() && !textureCoordinatesToAppend.isEmpty())
    {
        foreach (QPointF t, textureCoordinatesToAppend)
        {
            target.TextureCoordinates.append(t);
        }
    }

    foreach (int i, triangleIndicesToAppend)
    {
        target.TriangleIndices.append(index0 + i);
    }
}

void MeshBuilder::CheckPerformanceLimits()
{
    QString s;
    if (positions.count() > 20000)
    {
        //Trace.WriteLine(string.Format("Too many positions ({0}).", positions.Count));
        s = QString("Too many positions: %1").arg(positions.count());
        qDebug(s.toUtf8().data());
    }

    if (triangleIndices.count() > 60002)
    {
        //Trace.WriteLine(string.Format("Too many triangle indices ({0}).", triangleIndices.Count));
        s = QString("Too many triangle indices: %1").arg(triangleIndices.count());
        qDebug(s.toUtf8().data());
    }
}

void MeshBuilder::Scale(double scaleX, double scaleY, double scaleZ)
{
    for (int i = 0; i < positions.count(); i++)
    {
        positions[i] = Point3D(positions.at(i).x() * scaleX, positions.at(i).y() * scaleY, positions.at(i).z() * scaleZ);
    }

    //if (!normals.isNull())
    if (normals)
    {
        for (int i = 0; i < normals->count(); i++)
        {
            normals->operator [](i) = Vector3D(normals->at(i).x() * scaleX, normals->at(i).y() * scaleY, normals->at(i).z() * scaleZ);
            normals->operator [](i).normalize();
        }
    }
}

void MeshBuilder::SubdivideLinear(bool barycentric)
{
    if (barycentric)
    {
        SubdivideBarycentric();
    }
    else
    {
        Subdivide4();
    }
}

MeshGeometry3D* MeshBuilder::ToSmoothShadedMesh(bool freeze)
{
    //if (!normals.isNull() && positions.count() != normals->count())
    if (normals && positions.count() != normals->count())
    {
        throw new InvalidOperationException(WrongNumberOfNormals);
    }

    //if (!textureCoordinates.isNull() && positions.count() != textureCoordinates->count())
    if (textureCoordinates && positions.count() != textureCoordinates->count())
    {
        throw new InvalidOperationException(WrongNumberOfTextureCoordinates);
    }

    if (positions.count() > 1000)
    {
        QMap<Point3D, QList<int>> gg;

        for (int h = 0; h < positions.count(); h++)
        {
            Point3D p = positions.at(h);
            if (gg.contains(p))
            {
                QList<int> l = gg.value(p);
                l.append(h);
            }
            else
            {
                QList<int> l;
                l.append(h);
                gg.insert(p, l);
            }
        }

        QList<Point3D> points3d;
        QList<QPointF> points;
        QList<Vector3D> vecs;

        foreach(Point3D p, gg.keys())
        {
            QList<int> l = gg.value(p);
            points3d.append(p);
            //if (!textureCoordinates.isNull())
            if (textureCoordinates)
                points.append(textureCoordinates->at(0));

            foreach(int i, l)
            {
                triangleIndices[i] = points3d.indexOf(p);
            }
        }

        for (int h = 0; h < triangleIndices.count(); h += 3)
        {
            int i1 = triangleIndices.at(h);
            int i2 = triangleIndices.at(h + 1);
            int i3 = triangleIndices.at(h + 2);
            Point3D p0 = points3d[i1];
            Point3D p1 = points3d[i2];
            Point3D p2 = points3d[i3];


            Vector3D w = Vector3D::CrossProduct(p1 - p0, p2 - p0);
            w.normalize();
            vecs.append(w);
            vecs.append(w);
            vecs.append(w);
        }

        positions = points3d;
        textureCoordinates = &points;
        normals = &vecs;
    }

    MeshGeometry3D* mg = new MeshGeometry3D();
    mg->Positions = positions;
    mg->TriangleIndices = triangleIndices;
    mg->Normals = *normals;
    mg->TextureCoordinates = *textureCoordinates;

    return mg;
}

MeshGeometry3D* MeshBuilder::ToMesh(bool freeze)
{
    //if (!normals.isNull() && positions.count() != normals->count())
    if (normals && positions.count() != normals->count())
    {
        throw new InvalidOperationException(WrongNumberOfNormals);
    }

    //if (!textureCoordinates//.isNull() && positions.count() != textureCoordinates->count())
    if (textureCoordinates && positions.count() != textureCoordinates->count())
    {
        throw new InvalidOperationException(WrongNumberOfTextureCoordinates);
    }

    MeshGeometry3D* mg = new MeshGeometry3D();
    mg->Positions = positions;
    mg->TriangleIndices = triangleIndices;
    mg->Normals = *normals;
    mg->TextureCoordinates = *textureCoordinates;

    return mg;
}

MeshGeometry3D* MeshBuilder::GetUnitSphere(int subdivisions)
{
    if (UnitSphereCache.contains(subdivisions))
    {
        return UnitSphereCache.value(subdivisions);
    }

    MeshBuilder mb(false, false);
    mb.AddRegularIcosahedron(Point3D(), 1, false);
    for (int i = 0; i < subdivisions; i++)
    {
        mb.SubdivideLinear();
    }

    for (int i = 0; i < mb.Positions().count(); i++)
    {
        Vector3D v = Point3D::ToVector3D(mb.Positions().at(i));
        v.normalize();
        mb.Positions()[i] = v.ToPoint3D();
    }

    MeshGeometry3D* mesh = mb.ToMesh();
    UnitSphereCache[subdivisions] = mesh;
    return mesh;
}

void MeshBuilder::AddRectangularMeshNormals(int index0, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        int i1 = i + 1;
        if (i1 == rows)
        {
            i1--;
        }

        int i0 = i1 - 1;
        for (int j = 0; j < columns; j++)
        {
            int j1 = j + 1;
            if (j1 == columns)
            {
                j1--;
            }

            int j0 = j1 - 1;
            Vector3D u = Point3D::Subtract(positions.at(index0 + (i1 * columns) + j0), positions.at(index0 + (i0 * columns) + j0));
            Vector3D v = Point3D::Subtract(positions.at(index0 + (i0 * columns) + j1), positions.at(index0 + (i0 * columns) + j0));
            Vector3D normal = Vector3D::CrossProduct(u, v);
            normal.normalize();
            normals->append(normal);
        }
    }
}

void MeshBuilder::AddRectangularMeshTextureCoordinates(int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        double v = (double)i / (rows - 1);
        for (int j = 0; j < columns; j++)
        {
            double u = (double)j / (columns - 1);
            textureCoordinates->append(QPointF(u, v));
        }
    }
}

void MeshBuilder::AddRectangularMeshTriangleIndices(int index0, int rows, int columns, bool isSpherical)
{
    for (int i = 0; i < rows - 1; i++)
    {
        for (int j = 0; j < columns - 1; j++)
        {
            int ij = (i * columns) + j;
            if (!isSpherical || i > 0)
            {
                triangleIndices.append(index0 + ij);
                triangleIndices.append(index0 + ij + 1 + columns);
                triangleIndices.append(index0 + ij + 1);
            }

            if (!isSpherical || i < rows - 2)
            {
                triangleIndices.append(index0 + ij + 1 + columns);
                triangleIndices.append(index0 + ij);
                triangleIndices.append(index0 + ij + columns);
            }
        }
    }
}

void MeshBuilder::AddRectangularMeshTriangleIndices(int index0, int rows, int columns, bool rowsClosed, bool columnsClosed)
{
    int m2 = rows - 1;
    int n2 = columns - 1;
    if (columnsClosed)
    {
        m2++;
    }

    if (rowsClosed)
    {
        n2++;
    }

    for (int i = 0; i < m2; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            int i00 = index0 + (i * columns) + j;
            int i01 = index0 + (i * columns) + ((j + 1) % columns);
            int i10 = index0 + (((i + 1) % rows) * columns) + j;
            int i11 = index0 + (((i + 1) % rows) * columns) + ((j + 1) % columns);
            triangleIndices.append(i00);
            triangleIndices.append(i11);
            triangleIndices.append(i01);

            triangleIndices.append(i11);
            triangleIndices.append(i00);
            triangleIndices.append(i10);
        }
    }
}

Vector3D MeshBuilder::FindCornerNormal(Point3D p, double eps)
{
    Vector3D sum;
    int count = 0;
    QList<Vector3D> addedNormals;
    for (int i = 0; i < triangleIndices.count(); i += 3)
    {
        int i0 = i;
        int i1 = i + 1;
        int i2 = i + 2;
        Point3D p0 = positions.at(triangleIndices.at(i0));
        Point3D p1 = positions.at(triangleIndices.at(i1));
        Point3D p2 = positions.at(triangleIndices.at(i2));

        // check if any of the vertices are on the corner
        double d0 = (p - p0).lengthSquared();
        double d1 = (p - p1).lengthSquared();
        double d2 = (p - p2).lengthSquared();
        double mind = fmin(d0, fmin(d1, d2));
        if (mind > eps)
        {
            continue;
        }

        // calculate the triangle normal and check if this face is already added
        Vector3D normal = Vector3D::CrossProduct(p1 - p0, p2 - p0);
        normal.normalize();

        // todo: need to use the epsilon value to compare the normals?
        if (addedNormals.contains(normal))
        {
            continue;
        }

        count++;
        sum += normal;
        addedNormals.append(normal);
    }

    if (count == 0)
    {
        return Vector3D();
    }

    return sum * (1.0 / count);
}

void MeshBuilder::NoSharedVertices()
{
    QList<Point3D> points3d;
    QList<int> intlist;
    QList<Vector3D> vecs;
    QList<QPointF> points;


    for (int i = 0; i < triangleIndices.count(); i += 3)
    {
        int i0 = i;
        int i1 = i + 1;
        int i2 = i + 2;
        int index0 = triangleIndices.at(i0);
        int index1 = triangleIndices.at(i1);
        int index2 = triangleIndices.at(i2);
        Point3D p0 = positions.at(index0);
        Point3D p1 = positions.at(index1);
        Point3D p2 = positions.at(index2);
        points3d.append(p0);
        points3d.append(p1);
        points3d.append(p2);
        intlist.append(i0);
        intlist.append(i1);
        intlist.append(i2);
        //if (!normals.isNull())
        if (normals)
        {
            vecs.append(normals[index0]);
            vecs.append(normals[index1]);
            vecs.append(normals[index2]);
        }

        //if (!textureCoordinates.isNull())
        if (textureCoordinates)
        {
            points.append(textureCoordinates[index0]);
            points.append(textureCoordinates[index1]);
            points.append(textureCoordinates[index2]);
        }
    }

    positions = points3d;
    triangleIndices = intlist;
    normals = &vecs;
    textureCoordinates = &points;
}


void MeshBuilder::Subdivide4()
{
    // Each triangle is divided into four subtriangles, adding new vertices in the middle of each edge.
    int ip = positions.count();
    int ntri = triangleIndices.count();
    for (int i = 0; i < ntri; i += 3)
    {
        int i0 = triangleIndices.at(i);
        int i1 = triangleIndices.at(i + 1);
        int i2 = triangleIndices.at(i + 2);
        Point3D p0 = positions.at(i0);
        Point3D p1 = positions.at(i1);
        Point3D p2 = positions.at(i2);
        Vector3D v01 = p1 - p0;
        Vector3D v12 = p2 - p1;
        Vector3D v20 = p0 - p2;
        Point3D p01 = p0 + (v01 * 0.5);
        Point3D p12 = p1 + (v12 * 0.5);
        Point3D p20 = p2 + (v20 * 0.5);

        int i01 = ip++;
        int i12 = ip++;
        int i20 = ip++;

        positions.append(p01);
        positions.append(p12);
        positions.append(p20);

        //if (!normals.isNull())
        if (normals)
        {
            Vector3D n = normals->at(i0);
            normals->append(n);
            normals->append(n);
            normals->append(n);
        }

        //if (!textureCoordinates.isNull())
        if (textureCoordinates)
        {
            QPointF uv0 = textureCoordinates->at(i0);
            QPointF uv1 = textureCoordinates->at(i0 + 1);
            QPointF uv2 = textureCoordinates->at(i0 + 2);
            QPointF t01 = uv1 - uv0;
            QPointF t12 = uv2 - uv1;
            QPointF t20 = uv0 - uv2;
            QPointF u01 = uv0 + (t01 * 0.5);
            QPointF u12 = uv1 + (t12 * 0.5);
            QPointF u20 = uv2 + (t20 * 0.5);
            textureCoordinates->append(u01);
            textureCoordinates->append(u12);
            textureCoordinates->append(u20);
        }

        // TriangleIndices[i ] = i0;
        triangleIndices.operator [](i + 1) = i01;
        triangleIndices.operator [](i + 2) = i20;

        triangleIndices.append(i01);
        triangleIndices.append(i1);
        triangleIndices.append(i12);

        triangleIndices.append(i12);
        triangleIndices.append(i2);
        triangleIndices.append(i20);

        triangleIndices.append(i01);
        triangleIndices.append(i12);
        triangleIndices.append(i20);
    }
}

void MeshBuilder::SubdivideBarycentric()
{
    // The BCS of a triangle S divides it into six triangles; each part has one vertex v2 at the
    // barycenter of S, another one v1 at the midpoint of some side, and the last one v0 at one
    // of the original vertices.
    int im = positions.count();
    int ntri = triangleIndices.count();
    for (int i = 0; i < ntri; i += 3)
    {
        int i0 = triangleIndices.at(i);
        int i1 = triangleIndices.at(i + 1);
        int i2 = triangleIndices.at(i + 2);
        Point3D p0 = positions.at(i0);
        Point3D p1 = positions.at(i1);
        Point3D p2 = positions.at(i2);
        Vector3D v01 = p1 - p0;
        Vector3D v12 = p2 - p1;
        Vector3D v20 = p0 - p2;
        Point3D p01 = p0 + (v01 * 0.5);
        Point3D p12 = p1 + (v12 * 0.5);
        Point3D p20 = p2 + (v20 * 0.5);
        Point3D m((p0.x() + p1.x() + p2.x()) / 3, (p0.y() + p1.y() + p2.y()) / 3, (p0.z() + p1.z() + p2.z()) / 3);

        int i01 = im + 1;
        int i12 = im + 2;
        int i20 = im + 3;

        positions.append(m);
        positions.append(p01);
        positions.append(p12);
        positions.append(p20);

        //if (!normals.isNull())
        if (normals)
        {
            Vector3D n = normals->at(i0);
            normals->append(n);
            normals->append(n);
            normals->append(n);
            normals->append(n);
        }

        //if (!textureCoordinates.isNull())
        if (textureCoordinates)
        {
            QPointF uv0 = textureCoordinates->at(i0);
            QPointF uv1 = textureCoordinates->at(i0 + 1);
            QPointF uv2 = textureCoordinates->at(i0 + 2);
            QPointF t01 = uv1 - uv0;
            QPointF t12 = uv2 - uv1;
            QPointF t20 = uv0 - uv2;
            QPointF u01 = uv0 + (t01 * 0.5);
            QPointF u12 = uv1 + (t12 * 0.5);
            QPointF u20 = uv2 + (t20 * 0.5);
            QPointF uvm ((uv0.x() + uv1.x()) * 0.5, (uv0.y() + uv1.y()) * 0.5);
            textureCoordinates->append(uvm);
            textureCoordinates->append(u01);
            textureCoordinates->append(u12);
            textureCoordinates->append(u20);
        }

        // TriangleIndices[i ] = i0;
        triangleIndices[i + 1] = i01;
        triangleIndices[i + 2] = im;

        triangleIndices.append(i01);
        triangleIndices.append(i1);
        triangleIndices.append(im);

        triangleIndices.append(i1);
        triangleIndices.append(i12);
        triangleIndices.append(im);

        triangleIndices.append(i12);
        triangleIndices.append(i2);
        triangleIndices.append(im);

        triangleIndices.append(i2);
        triangleIndices.append(i20);
        triangleIndices.append(im);

        triangleIndices.append(i20);
        triangleIndices.append(i0);
        triangleIndices.append(im);

        im += 4;
    }
}
