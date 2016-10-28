#include "supportgenerator.h"

#include <cmath>
#include <limits>

//using namespace FullSpectrumEngineering;
using namespace CreateCore;

SupportGenerator::SupportGenerator()
{

}

SupportGenerator::SupportGenerator(MVMParameters params, QList<Point3D> list, Rect3D rect, TransformState state, SupportOperationType sot)
{
    supportOperationType = sot;
    transformState = state;
    ImportParams(params);
    bounds = rect;
    positions = list;
    offsetX = (bounds.Location().x() * -1) - bounds.SizeX() / 2.0;
    offsetY = (bounds.Location().y() * -1) - bounds.SizeY() / 2.0;
    offsetX = round(offsetX * 1000000) / 1000000;
    offsetY = round(offsetY * 1000000) / 1000000;
    transformState.SupportOffsetX = offsetX;
    transformState.SupportOffsetY = offsetY;
    supportXLim = (int)(bounds.SizeX() / supportXSpacing + 2);
    supportYLim = (int)(bounds.SizeY() / supportYSpacing + 2);

    int numSlices = (int)ceil((bounds.Z() + bounds.SizeZ()) / layerThickness) + 1;
    supportSlicePoints.clear();
    for (int i = 0; i < numSlices; i++)
    {
        QList<QList<Point3D>> l;
        supportSlicePoints.append(l);
    }
}

void SupportGenerator::ImportParams(MVMParameters p)
{
    baseSupportHeight = p.BaseSupportHeight();
    layerThickness = p.LayerThickness();
}

void SupportGenerator::DoHitTest(Visual3D* hitModel, QList<QPointF> list, double Z)
{
    ModelEntity* m = 0;
    foreach (ModelEntity* me, MVM->listModelEntities)
    {
        if (m->supportModelVisual3D == hitModel || m->modelVisual3D == hitModel)
        {
            m = me;
            break;
        }
    }
    if (!m)
        return;

    if (!m->Tree())
    {
        qDebug() << "modelEntity.aabbTree is NULL";
        m->GenerateAABBTree();
    }

    hitMap.clear();
    foreach(QPointF p, list)
    {
        QList<RayHitResult> rhrList;
        RayHitResult rhr(Z);
        rhrList.append(rhr);

        if (!hitMap.contains(point))
        {
            hitMap.insert(point, rhrList);
        }
    }

    foreach(QPointF p, list)
    {
        QList<Triangle> trianglesOfIntersection;
        QList<Point3D> pointsOfIntersection;
        double zStart = m->Tree()->Root()->BB().Z();
        double zEnd = zStart + m->Tree()->Root()->BB().SizeZ();
        Point3D a(p.x(), p.y(), zStart);
        Point3D b(p.x(), p.y(), zEnd);
        bool intersects = m->Tree()->Intersect(LineSeg(a, b), trianglesOfIntersection, pointsOfIntersection);
        if (intersects)
        {
            MeshGeometry3D mesh = (MeshGeometry3D)m->modelGroup.Children.at(0);
            QList<RayHitResult> results = hitMap.value(p);
            for (int i = 0; i < trianglesOfIntersection.count(); i++)
            {
                int ind = trianglesOfIntersection.at(i).Index();
                RayHitResult result(mesh, pointsOfIntersection.at(i), Point3D::DistanceTo(pointsOfIntersection.at(i), a), 3 * ind, 3 * ind + 1, 3 * ind + 2);
                result.setHeight(pointsOfIntersection.at(i).z());

                //add to list while keeping sorted by intersection height
                int insertionLocation = -1;
                if (results.count() > 1)
                {
                    for (int ii = 1; ii < results.count(); ii++)
                    {
                        if (result.Height() < results.at(ii).Height())
                        {
                            insertionLocation = ii;
                            break;
                        }
                    }
                }
                if (insertionLocation > 0)
                    results.insert(insertionLocation, result);
                else
                    results.append(result);
            }
        }
    }

    foreach(QPointF p, hitMap.keys())
    {
        QList<RayHitResult> list = hitMap.value(p);
        if (list.isEmpty())
            hitMap.remove(p);
    }
}

void SupportGenerator::SingleSupportRay(ModelVisual3DObservable hitModel, Point3D p)
{
    hitList.clear();
    hitList.append(QPointF(p.x(), p.y()));
    mouse = p;
    DoHitTest(hitModel, hitList, baseSupportHeight);
    qDebug() << "--------------------drawing at  : " << mouse.x() << " " << mouse.y() << " " << mouse.z();
    if (hitMap.count() > 0)
    {
        SupportData data;
        data.map = hitMap;
        QList<SupportDataSegment> list;
        list.append(SupportDataSegment(p));
        data.supportDataSegments.append(list);
        BuildBeams(data, p, true);
    }
}

void SupportGenerator::BuildBeams(SupportData data, Point3D projectedMouse, bool forceBuild)
{
    for (int i = 0; i < data.supportDataSegments.count(); i++)
    {
        QList<SupportDataSegment> group = data.supportDataSegments.at(i);

        for (int j = 0; j < group.count(); j++)
        {
            SupportDataSegment segment = group.at(j);
            QPointF key(segment.point.x(), segment.point.y());

            if (!data.map.contains(key))
            {
                qDebug() << "Problem retrieving results for point at X:" << key.x() << " Y:" << key.y();
                continue;
            }

            QList<RayHitResult> meshHits = data.map.value(key);
            int floor = 0;
            int ceiling = meshHits.count();

            Point3D origin();
            if (projectedMouse != origin)
            {
                double d = DBL_MAX;
                int hitIndex = 0;
                int altSurface;

                foreach (RayHitResult res, meshHits)
                {
                    double k = fabs(fabs(mouse.z()) - fabs(res.Height()));
                    if (k < d)
                    {
                        d = k;
                        hitIndex = meshHits.indexOf(res);
                    }
                }

                if (hitIndex == 0)
                {
                    altSurface = hitIndex + 1;
                }
                else
                {
                    if (hitIndex % 2 == 1) // if odd, connecting hit is --
                    {
                        altSurface = hitIndex - 1;
                    }
                    else // if even, connecting hit is ++
                    {
                        altSurface = hitIndex + 1;
                    }
                    if (hitIndex + 1 == meshHits.Count) // if top hit, adjust down
                    {
                        hitIndex = meshHits.Count - 2;
                        altSurface = meshHits.Count - 3;
                    }
                }

                if (hitIndex < altSurface)
                {
                    floor = hitIndex;
                    ceiling = altSurface;
                }
                else
                {
                    floor = altSurface;
                    floor = hitIndex;
                }

                Q_ASSERT(ceiling <= meshHits.count() && floor > 0);

                Top = ceiling;
                Bottom = floor;
            }
            else
            {
                int index = 0;
                for (int i = 0; i < meshHits.Count; i++)
                    if (segment.point.z() == meshHits.at(i).Height())
                        if (i > 0 && i < meshHits.count())
                            index = i - 1;
                floor = index;
                ceiling = index + 2;
            }

            for (int bIndex = floor; bIndex < ceiling; bIndex += 2)
            {
                Cylinder cyl;
                double x = key.x();
                double y = key.y();
                QPointF hit2D(x, y);

                int meshHitTopIndex = meshHits.count() - 1 >= bIndex + 1 ? bIndex + 1 : bIndex;
                if (projectedMouse != origin)
                {
                    meshHitTopIndex = ceiling;
                }

                double topHitZ = meshHits.at(meshHitTopIndex).Height();
                double bottomHitZ = meshHits.at(bIndex).Height();

                MeshGeometry3D supportMesh;
                if (meshHitTopIndex == bIndex)
                {
                    supportMesh = GetSingleSupport(hit2D, meshHits.at(meshHitTopIndex), cyl, hitModel, 0, forceBuild);
                }
                else
                {
                    supportMesh = GetSingleSupport(hit2D, meshHits.at(meshHitTopIndex), cyl, hitModel, meshHits.at(bIndex), forceBuild);
                }

                segment.cylinder = cyl;
                if (supportMesh.Positions.isEmpty())
                {
                    group.removeAt(j);
                }
            }
        }
    }
}

ModelEntity* SupportGenerator::GetModelEntityFromHitModel(Visual3D* hitModel)
{
    foreach(ModelEntity* me, MVM->listModelEntities)
    {
        if (me->SupportModelVisual3D() == hitModel || me->ModelVisual3D() == hitModel)
        {
            return me;
        }
    }

    return 0;
}

MeshGeometry3D* SupportGenerator::GetSingleSupport(QPointF center, RayHitResult meshHit, Cylinder &cyl,
                                                  Visual3D hitModel, RayHitResult meshHitB, bool forceBuild)
{
    cyl.Clear();

    ModelEntity* me = GetModelEntityFromHitModel(hitModel);
    if (!me)
        return 0;

    if (!MVM->Parameters().TreeSupports())
        return 0;



}
