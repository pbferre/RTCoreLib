#ifndef SUPPORTGENERATOR_H
#define SUPPORTGENERATOR_H

#include "rect3d.h"
#include "point3d.h"
#include "vector3d.h"
#include "mainviewmodel.h"
#include "support.h"

namespace CreateCore
{

class SupportGenerator
{
public:
    SupportGenerator();
    SupportGenerator(MVMParameters params, QList<Point3D> list, Rect3D rect, TransformState state,
                     SupportOperationType sot = SingleBaseWithSupports);

    void SingleSupportRay(ModelVisual3DObservable hitModel, Point3D mouse);

    static ModelEntity* GetModelEntityFromHitModel(Visual3D* hitModel);
    static int Top;
    static int Bottom;

private:
     QList<QPointF> hitList;
     QMap<QPointF, QList<RayHitResult>> hitMap;

     ModelVisual3DObservable hitModel;
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


     void DoHitTest(Visual3D* hitModel, QList<QPointF> list, double Z);
     void ImportParams(MVMParameters p);
     void BuildBeams(SupportData data, Point3D projectedMouse = Point3D(), bool forceBuild = false);

     MeshGeometry3D* GetSingleSupport(QPointF center, RayHitResult meshHit, Cylinder &cyl,
                                     Visual3D hitModel, RayHitResult meshHitB, bool forceBuild = false);
};

}

#endif // SUPPORTGENERATOR_H
