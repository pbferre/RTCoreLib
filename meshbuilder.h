#ifndef MESHBUILDER_H
#define MESHBUILDER_H

#include <QtCore>

#include "point3d.h"
#include "vector3d.h"
#include "rect3d.h"

//using namespace CreateCore;

namespace CreateCore
{
class MeshGeometry3D;

const QString AllCurvesShouldHaveTheSameNumberOfPoints = "All curves should have the same number of points";
const QString SourceMeshNormalsShouldNotBeNull = "Source mesh normal vectors should not be empty or null.";
const QString SourceMeshTextureCoordinatesShouldNotBeNull = "Source mesh texture coordinates should not be empty or null.";
const QString WrongNumberOfDiameters = "Wrong number of diameters.";
const QString WrongNumberOfPositions = "Wrong number of positions.";
const QString WrongNumberOfNormals = "Wrong number of normal vectors.";
const QString WrongNumberOfTextureCoordinates = "Wrong number of texture coordinates.";

enum BoxFaces
{
    bfTop = 0x1,
    bfBottom = 0x2,
    bfLeft = 0x4,
    bfRight = 0x8,
    bfFront = 0x10,
    bfBack = 0x20,
    bfAll = bfTop | bfBottom | bfLeft | bfRight | bfFront | bfBack
};

class MeshBuilder
{
public:
    MeshBuilder();
    MeshBuilder(bool generateNormals, bool generateTextureCoordinates);

//    QPointer<QList<Vector3D>> Normals() { return normals; }
//    QPointer<QList<QPointF>> TextureCoordinates() { return textureCoordinates; }
    QList<Vector3D>* Normals() { return normals; }
    QList<QPointF>* TextureCoordinates() { return textureCoordinates; }
    QList<Point3D> Positions() { return positions; }
    QList<int> TriangleIndices() { return triangleIndices; }

    bool CreateNormals();
    void CreateNormals(bool value);

    bool CreateTextureCoordinates();
    void CreateTextureCoordinates(bool value);

    static QList<QPointF> GetCircle(int thetaDiv);
    static QList<QPointF> Circle;

    void AddArrow(Point3D point1, Point3D point2, double diameter, double headLength = 10, int thetaDiv = 18);
    void AddManipulatorArrow(Point3D point1, Point3D point2, double diameter, double headLength = 10, int thetaDiv = 18);
    void AddBoundingBox(Rect3D boundingBox, double diameter);
    void AddBox(Point3D center, double xlength, double ylength, double zlength);
    void AddBox(Rect3D rectangle);
    void AddBox(Point3D center, double xlength, double ylength, double zlength, BoxFaces faces);
    void AddCone(Point3D origin, Vector3D direction, double baseRadius, double topRadius,
                 double height, bool baseCap, bool topCap, int thetaDiv);
    void AddCone(Point3D origin, Vector3D direction, double baseRadius, double topRadius,
                 double height, int thetaDiv);
    void AddCone(Point3D origin, Point3D apex, double baseRadius, bool baseCap, int thetaDiv);
    void AddCubeFace(Point3D center, Vector3D normal, Vector3D up, double dist, double width, double height);
    void AddCylinder(Point3D p1, Point3D p2, double diameter, int thetaDiv);
    void AddEdges(QList<Point3D> points, QList<int> edges, double diameter, int thetaDiv);
    void AddExtrudedGeometry(QList<QPointF> points, Vector3D xaxis, Point3D p0, Point3D p1);
    void AddLoftedGeometry(QList<QList<Point3D>> positionsList, QList<QList<Vector3D>> normalList,
                           QList<QList<QPointF>> textureCoordinateList);
    void AddNode(Point3D position, Vector3D normal, QPointF textureCoordinate);
    void AddPipe(Point3D point1, Point3D point2, double innerDiameter, double diameter, int thetaDiv);
    void AddPolygon(QList<Point3D> points);
    void AddPyramid(Point3D center, double sideLength, double height);
    void AddQuad(Point3D p0, Point3D p1, Point3D p2, Point3D p3);
    void AddQuad(Point3D p0, Point3D p1, Point3D p2, Point3D p3, QPointF uv0, QPointF uv1, QPointF uv2, QPointF uv3);
    void AddQuads(QList<Point3D> quadPositions, QList<Vector3D> quadNormals,
                  QList<QPointF> quadTextureCoordinates);
    void AddRectangularMesh(QList<Point3D> points, int columns);
    void AddRectangularMesh(QList<QList<Point3D>> points, QList<QList<QPointF>> texCoords,
                            bool closed0 = false, bool closed1 = false);
    void AddRegularIcosahedron(Point3D center, double radius, bool shareVertices);
    void AddRevolvedGeometry(QList<QPointF> points, Point3D origin, Vector3D direction, int thetaDiv);
    void AddSubdivisionSphere(Point3D center, double radius, int subdivisions);
    void AddSphere(Point3D center, double radius, int thetaDiv = 20, int phiDiv = 10);
    void AddTriangle(Point3D p0, Point3D p1, Point3D p2);
    void AddTriangle(Point3D p0, Point3D p1, Point3D p2, QPointF uv0, QPointF uv1, QPointF uv2);
    void AddTriangleFan(QList<int> vertices);
    void AddTriangleFan(QList<Point3D> fanPositions, QList<Vector3D> fanNormals,
                        QList<QPointF> fanTextureCoordinates);
    void AddTriangleStrip(QList<Point3D> stripPositions, QList<Vector3D> stripNormals,
                          QList<QPointF> stripTextureCoordinates);
    void AddPolygon(QList<int> vertexIndices);
    void AddTriangles(QList<Point3D> trianglePositions, QList<Vector3D> triangleNormals,
                      QList<QPointF> triangleTextureCoordinates);

    void AddTube(QList<Point3D> path, QList<double> values, QList<double> diameters, int thetaDiv, bool isTubeClosed);
    void AddTube(QList<Point3D> path, double diameter, int thetaDiv, bool isTubeClosed);
    void AddTube(QList<Point3D> path, QList<double> values, QList<double> diameters,
                 QList<QPointF> section, bool isTubeClosed, bool isSectionClosed);
    void AddRectangularMeshNormals(int index0, int rows, int columns);
    void AddRectangularMeshTextureCoordinates(int rows, int columns);
    void AddRectangularMeshTriangleIndices(int index0, int rows, int columns, bool isSpherical = false);
    void AddRectangularMeshTriangleIndices(int index0, int rows, int columns, bool rowsClosed, bool columnsClosed);

    void Append(MeshBuilder mesh);
    void Append(MeshGeometry3D* mesh);
    void Append(QList<Point3D> positionsToAppend, QList<int> triangleIndicesToAppend,
                QList<Vector3D> normalsToAppend, QList<QPointF> textureCoordinatesToAppend);
    void AppendTo(MeshGeometry3D target, MeshGeometry3D addition);
    void CheckPerformanceLimits();
    void Scale(double scaleX, double scaleY, double scaleZ);
    void SubdivideLinear(bool barycentric = false);
    void NoSharedVertices();
    void Subdivide4();
    void SubdivideBarycentric();

    MeshGeometry3D* ToSmoothShadedMesh(bool freeze = false);
    MeshGeometry3D* ToMesh(bool freeze = false);
    static MeshGeometry3D* GetUnitSphere(int subdivisions);

    Vector3D FindCornerNormal(Point3D p, double eps);


private:
    static QMap<int, QList<QPointF>> CircleCache;
    static QMap<int, MeshGeometry3D*> UnitSphereCache;
//    QPointer<QList<Vector3D>> normals;
//    QPointer<QList<QPointF>> textureCoordinates;
    QList<Vector3D> *normals;
    QList<QPointF> *textureCoordinates;

    QList<Point3D> positions;
    QList<int> triangleIndices;

};

}
#endif // MESHBUILDER_H
