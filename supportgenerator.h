#ifndef SUPPORTGENERATOR_H
#define SUPPORTGENERATOR_H

#include <tuple>

#include "rect3d.h"
#include "point3d.h"
#include "vector3d.h"
#include "support.h"
#include "meshbuilder.h"
#include "mvmparameters.h"

namespace CreateCore
{
class ModelEntity;

class SupportGenerator
{
public:
    SupportGenerator();
    SupportGenerator(MVMParameters params, QList<Point3D> list, Rect3D rect, TransformState state,
                     SupportOperationType sot = sotSingleBaseWithSupports);

    void SingleSupportRay(ModelVisual3DObservable* hitModel, Point3D mouse);

    static ModelEntity* GetModelEntityFromHitModel(Visual3D* hitModel);
    static int Top;
    static int Bottom;
    static double IntersectionSafetyDistance;

private:
     QList<QPointF> hitList;
     //QHash<QPointF, QList<RayHitResult*>> hitMap;
     QList<std::tuple<QPointF, QList<RayHitResult*>>> hitMap;

     ModelVisual3DObservable* hitModel;
     SupportOperationType supportOperationType;
     TransformState transformState;
     Rect3D bounds;
     QList<Point3D> positions;
     QList<QList<QList<Point3D>>> supportSlicePoints;
     Point3D mouse;

     double baseSupportHeight;
     double layerThickness;
     double offsetX, offsetY;
     int supportXLim, supportYLim;
     double supportXSpacing, supportYSpacing;
     double supportCylRad;
     double touchPointDiameter;
     double touchPointLength;
     double touchPointWithdrawal;
     double coneSupportLength;
     double supportConeRad;
     int circularThetaDiv;
     double cornerComplexity;

     static void BuildSupport(QList<Point3D> path, QList<double> diameters, QList<QPointF> section, MeshBuilder* meshBuilder);

     void DoHitTest(Visual3D* hitModel, QList<QPointF> list, double Z);
     void ImportParams(MVMParameters p);
     void BuildBeams(SupportData data, Point3D projectedMouse = Point3D(), bool forceBuild = false);
     void ConstructCorner(Point3D hitOrigin, Vector3D hitNormal, Point3D &displacedCylinderLimit,
                          QList<Point3D> &path, QList<double> &diameters, Vector3D zPlane);
     bool TestSupportModelIntersection(QList<Point3D> origSupportPath, QList<double> origDiameters, ModelEntity* modelEntity);

     MeshGeometry3D* GetSingleSupport(QPointF center, RayHitResult* meshHit, Cylinder &cyl,
                                     Visual3D* hitModel, RayHitResult* meshHitB, bool forceBuild = false);

     Vector3D GetNegatedNormal(RayHitResult* hit);

};

}

#endif // SUPPORTGENERATOR_H
