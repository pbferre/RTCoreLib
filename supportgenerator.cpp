#include "supportgenerator.h"
#include "meshbuilder.h"
#include "matrix3d.h"
#include "mvmparameters.h"
#include "aabbtree.h"
#include "rcexceptions.h"
#include "mainviewmodel.h"
#include "modelentity.h"

#include <cmath>
#include <limits>
#include <algorithm>

//using namespace FullSpectrumEngineering;
using namespace CreateCore;

double SupportGenerator::IntersectionSafetyDistance = 0.4;

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
    supportXSpacing = p.SupportSpace();
    supportYSpacing = p.SupportSpace();
    supportCylRad = p.CylRadius();
    circularThetaDiv = p.ThetaDiv();
    touchPointDiameter = p.TouchPointDiameter();
    touchPointLength = touchPointDiameter / 2;
    if (touchPointDiameter > 2 * supportCylRad)
        touchPointWithdrawal = -coneSupportLength;
    else
        touchPointWithdrawal = -coneSupportLength * touchPointDiameter / (2 * supportCylRad);

    cornerComplexity = p.CornerDivisions();
}

void SupportGenerator::DoHitTest(Visual3D* hitModel, QList<QPointF> list, double Z)
{
    ModelEntity* m = 0;
    foreach (ModelEntity* me, MVM->listModelEntities)
    {
        if (me->SupportModelVisual3D() == hitModel || m->ModelVisual3D() == hitModel)
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
    foreach(QPointF point, list)
    {
        QList<RayHitResult*> rhrList;
        RayHitResult* rhr = new RayHitResult(Z);
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
            MeshGeometry3D* mesh = dynamic_cast<MeshGeometry3D*>(m->ModelGroup()->Children.at(0));
            QList<RayHitResult*> results = hitMap.value(p);
            for (int i = 0; i < trianglesOfIntersection.count(); i++)
            {
                Triangle t = trianglesOfIntersection.at(i);
                int ind = t.Index();
                //int ind = trianglesOfIntersection.at(i).Index();
                RayHitResult* result = new RayHitResult(mesh, pointsOfIntersection.at(i), Point3D::DistanceTo(pointsOfIntersection.at(i), a), 3 * ind, 3 * ind + 1, 3 * ind + 2);
                result->setHeight(pointsOfIntersection.at(i).z());

                //add to list while keeping sorted by intersection height
                int insertionLocation = -1;
                if (results.count() > 1)
                {
                    for (int ii = 1; ii < results.count(); ii++)
                    {
                        RayHitResult* rhr = results.at(ii);
                        if (result->Height() < rhr->Height())
                            //if (result.Height() < results.at(ii).Height())
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
        QList<RayHitResult*> list = hitMap.value(p);
        if (list.isEmpty())
            hitMap.remove(p);
    }
}

void SupportGenerator::SingleSupportRay(ModelVisual3DObservable* hitModel, Point3D p)
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

            QList<RayHitResult*> meshHits = data.map.value(key);
            int floor = 0;
            int ceiling = meshHits.count();

            Point3D origin = Point3D();
            if (projectedMouse != origin)
            {
                double d = DBL_MAX;
                int hitIndex = 0;
                int altSurface;

                foreach (RayHitResult* res, meshHits)
                {
                    double k = fabs(fabs(mouse.z()) - fabs(res->Height()));
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
                    if (hitIndex + 1 == meshHits.count()) // if top hit, adjust down
                    {
                        hitIndex = meshHits.count() - 2;
                        altSurface = meshHits.count() - 3;
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
                for (int i = 0; i < meshHits.count(); i++)
                {
                    RayHitResult* rhr = meshHits.at(i);
                    //if (segment.point.z() == meshHits.at(i).Height())
                    if (segment.point.z() == rhr->Height())
                    {
                        if (i > 0 && i < meshHits.count())
                            index = i - 1;
                    }
                }
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

                RayHitResult* rhr;
                rhr = meshHits.at(meshHitTopIndex);
                double topHitZ = rhr->Height();
                rhr = meshHits.at(bIndex);
                double bottomHitZ = rhr->Height();

                MeshGeometry3D* supportMesh;
                //rhr = meshHits.at(meshHitTopIndex);
                if (meshHitTopIndex == bIndex)
                {
                    supportMesh = GetSingleSupport(hit2D, meshHits.at(meshHitTopIndex), cyl, hitModel, 0, forceBuild);
                }
                else
                {
                    supportMesh = GetSingleSupport(hit2D, meshHits.at(meshHitTopIndex), cyl, hitModel, meshHits.at(bIndex), forceBuild);
                }

                segment.cylinder = cyl;
                if (supportMesh->Positions.isEmpty())
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

Vector3D SupportGenerator::GetNegatedNormal(RayHitResult *hit)
{
    int v1 = hit->VertexIndex1();
    int v2 = hit->VertexIndex2();
    int v3 = hit->VertexIndex3();
    Point3D pt0 = positions.at(v1);
    Point3D pt1 = positions.at(v2);
    Point3D pt2 = positions.at(v3);

    Vector3D hitNormal = Vector3D::CrossProduct(pt1 - pt0, pt2 - pt0);
    hitNormal.Negate();
    hitNormal.normalize();

    if (fabs(hitNormal.z() - 1) <= 0)
    {
        hitNormal.setX(hitNormal.x() + 1e-12);
        //hitNormal.x() += 1e-12;
        hitNormal.normalize();
    }
    return hitNormal;
}

MeshGeometry3D* SupportGenerator::GetSingleSupport(QPointF center, RayHitResult* meshHit, Cylinder &cylinder,
                                                   Visual3D* hitModel, RayHitResult* meshHitB, bool forceBuild)
{
    cylinder.Clear();

    ModelEntity* me = GetModelEntityFromHitModel(hitModel);
    if (!me)
        return 0;

    if (!MVM->Parameters().TreeSupports())
        return 0;

    Point3D displacedCylBottom;
    QList<double> diameters_T;
    QList<Point3D> path_T;
    double topHit = meshHit->PointHit().z();
    Vector3D hitNormalT = GetNegatedNormal(meshHit);
    Point3D topOrigin(center.x(), center.y(), topHit);

    QList<Point3D> pointList = me->DistTransform()->FindPath(topOrigin);
    if (pointList.count() > 0)
    {
        std::reverse(pointList.begin(), pointList.end());
        path_T.clear();

        path_T.append(Point3D(topOrigin + Vector3D::Multiply(hitNormalT, touchPointLength)));
        double A = touchPointDiameter;
        double X = 0.5 * A + supportCylRad;
        double l1 = X / 2 + IntersectionSafetyDistance;
        path_T.append(Point3D(topOrigin + Vector3D::Multiply(hitNormalT, l1)));
        path_T.append(pointList);

        diameters_T.clear();
        for (int i = 0; i < path_T.count(); i++)
            diameters_T.append(2 * supportCylRad);

        double l2 = l1 * touchPointLength;
        diameters_T[1] = X;
        diameters_T[0] = X + (A - X) / l1 * l2;

        displacedCylBottom = Point3D (path_T.last().x(), path_T.last().y(), MVM->Parameters().BaseSupportHeight());
        if (supportOperationType == sotBasePerSupport || supportOperationType == sotNone || supportOperationType == sotElevateNoBase)
        {
            if (path_T.last() != displacedCylBottom)
            {
                path_T.append(displacedCylBottom);
                diameters_T.append(supportCylRad * 2);
            }

            path_T.append(displacedCylBottom);
            path_T.append(Point3D(displacedCylBottom.x(), displacedCylBottom.y(), 0));

            diameters_T.append(supportCylRad * 8);
            diameters_T.append(supportCylRad * 6);
        }
        else
        {
            path_T.append(displacedCylBottom);
            diameters_T.append(supportCylRad * 2);
        }
    }
    else
    {
        Point3D displacedCylTop;

        double bottomHit = MVM->Parameters().BaseSupportHeight();

        if (meshHitB && meshHitB->MeshHit())
        {
            bottomHit = meshHitB->PointHit().z();
        }
        double cylBottomZ = bottomHit;

        Vector3D hitNormalB;
        Point3D bottomOrigin (center.x(), center.y(), bottomHit);
        QList<double> diameters_B;
        QList<Point3D> path_B;
        if (meshHitB && meshHitB->MeshHit())
        {
            hitNormalB = GetNegatedNormal(meshHitB);
            bottomOrigin = bottomOrigin - Vector3D::Multiply (hitNormalB, touchPointWithdrawal);
            ConstructCorner (bottomOrigin, hitNormalB, displacedCylBottom, path_B, diameters_B, Vector3D());
            cylBottomZ = displacedCylBottom.z();
        }

        // adjust for desired contact with mesh
        Point3D adjustedTopOrigin = topOrigin - Vector3D::Multiply (hitNormalT, touchPointWithdrawal);

        // If top normal is facing down, then don't construct support
        //if (hitNormalT.Z < 0) return null;

        // process top corner, get top of column as out param (base of top corner)
        ConstructCorner (adjustedTopOrigin, hitNormalT, displacedCylTop, path_T, diameters_T, Vector3D());

        Cylinder cyl(displacedCylTop.x(), displacedCylTop.y(), supportCylRad, cylBottomZ, displacedCylTop.z());

        int angleGuess = 0;
        bool breakNow = false;
        while (!breakNow) {
            //return null;
            if (angleGuess > 0) {
                angleGuess += 10;
                angleGuess *= -1;
            } else {
                angleGuess -= 10;
                angleGuess *= -1;
            }

            QList<Point3D> supportPath = path_T.mid(1, path_T.count() - 1);
            QList<double> supportDiameters = diameters_T.mid (1, path_T.count() - 1);
            if (meshHitB && meshHitB->MeshHit())
            {
                for (int i = path_B.count() - 1; i > 0; i--)
                {
                    supportPath.append(path_B.at(i));
                    supportDiameters.append(diameters_B.at(i));
                }
            }
            else
            {
                supportPath.append(Point3D (displacedCylTop.x(), displacedCylTop.y(), cylBottomZ));
                supportDiameters.append(supportCylRad * 2);
            }

            if (!TestSupportModelIntersection (supportPath, supportDiameters, me))
                break;

            if (fabs(angleGuess) > 150) {
                if (forceBuild) {
                    angleGuess = 0;
                    breakNow = true;
                }
                else
                {
                    qDebug() << "No good elbow angle. ABORT";
                    return 0;
                }
            }

            // Rotate normal and construct cone/corner
            AxisAngleRotation3D axisAngle (hitNormalT.FindAnyPerpindicular(), angleGuess);
            RotateTransform3D rotate (axisAngle, topOrigin);
            Vector3D vector = rotate.Transform(hitNormalT);
            adjustedTopOrigin = topOrigin - Vector3D::Multiply (vector, touchPointWithdrawal);
            ConstructCorner (adjustedTopOrigin, vector, displacedCylTop, path_T, diameters_T, Vector3D());
        }

        cylinder = cyl;

        // Add bottom cone for auto generation (mesh-mesh internal supports)
        if (bottomHit != MVM->Parameters().BaseSupportHeight() && meshHitB && meshHitB->MeshHit())
        {
            for (int i = path_B.count(); i > 0; i--)
            {
                path_T.append(path_B.at(i - 1));
                diameters_T.append(diameters_B.at(i - 1));
            }
        }
        else
        {
            // Mesh-Base or Mesh to Z-plane
            displacedCylBottom = Point3D (displacedCylTop.x(), displacedCylTop.y(), bottomHit);


            if (supportOperationType == sotBasePerSupport || supportOperationType == sotNone || supportOperationType == sotElevateNoBase)
            {
                path_T.append(displacedCylBottom);
                path_T.append(displacedCylBottom);
                path_T.append(Point3D (displacedCylBottom.x(), displacedCylBottom.y(), 0));

                diameters_T.append(supportCylRad * 2);
                diameters_T.append(supportCylRad * 8);
                diameters_T.append(supportCylRad * 6);
                //diameters_T.Add(0);
            }
            else
            {
                path_T.append(displacedCylBottom);
                diameters_T.append(supportCylRad * 2);
            }

        }
    }

    if (path_T.count() > 0)
    {
        int end = path_T.count();
        if (meshHitB)
            end -= 1;
        QList<Point3D> testPath = path_T.mid(2, end - 3);
        QList<double> testDiam = diameters_T.mid(2, end - 3);
        if (TestSupportModelIntersection (testPath, testDiam, me))
        {
            if (!forceBuild)
            {
                qDebug() << "Found path intersects model. ABORT";
                return new MeshGeometry3D();
            }

        }
        else
        {
            //Debugger.Break();
        }

        MVM->CylinderPaths.append(path_T);
        MVM->CylinderDiams.append(diameters_T);

        MeshBuilder* meshBuilder = new MeshBuilder (true, true);
        BuildSupport (path_T, diameters_T, MeshBuilder::GetCircle(circularThetaDiv), meshBuilder);

        MeshGeometry3D* outMesh = meshBuilder->ToMesh(false);
        //outMesh.TextureCoordinates[0] = textureZero;
        return outMesh;
    }
    else
    {
        qDebug() << "Pathfinding failed.";
        return new MeshGeometry3D ();
    }
}

void SupportGenerator::ConstructCorner(Point3D hitOrigin, Vector3D hitNormal, Point3D &displacedCylinderLimit,
                                       QList<Point3D> &path, QList<double> &diameters, Vector3D zPlane)
{
    path.clear();
    diameters.clear();

    // multiply normal by cone length in direction of the hit normal and subtract from orignal top hit point
    // this gets new point from where to originate cone in direction of hit point
    Point3D offsetOrigin = hitOrigin - Vector3D::Multiply(hitNormal, coneSupportLength);

    // shortcut : temp meshbuilder to get aggregate points of this cone alone
    MeshBuilder* mb = new MeshBuilder(true, true);


    // Add top cone to find ConeLimitZ
    mb->AddCone(offsetOrigin, hitNormal, supportCylRad, supportConeRad, coneSupportLength, false, false, circularThetaDiv);
    path.append(hitOrigin);
    path.append(offsetOrigin);

    //Vector3D zPlane;
    Point3D ConeLimitZ;
    //find if cone is support bottom or top
    if (hitNormal.z() > 0)
    {
        if (zPlane == Vector3D())
            zPlane = Vector3D(0, 0, 1);
        // Get top Z bounds of rendered Cone. This will visually 'hinge' with support cylinder
        ConeLimitZ = mb->Positions().at(0);
        foreach(Point3D pt, mb->Positions())
        {
            if (pt.z() < ConeLimitZ.z())
                ConeLimitZ = pt;
        }
    }
    else
    {
        if (zPlane == Vector3D())
            zPlane = Vector3D(0, 0, -1);
        // Get bottom Z bounds of rendered Cone. This will visually 'hinge' with support cylinder
        ConeLimitZ = mb->Positions().at(0);
        foreach(Point3D pt, mb->Positions())
        {
            if (pt.z() > ConeLimitZ.z())
                ConeLimitZ = pt;
        }
    }


    // get axis for rotation (along hinge path)
    Vector3D axis = Vector3D::CrossProduct(hitNormal, zPlane);

    // angle between normal and z-plane (top/bottom of cylinder is considered Z-plane)
    double loftAngle = Vector3D::AngleBetween(hitNormal, zPlane);

    //Debug.Assert(loftAngle < 90);

    double segAngle = loftAngle / cornerComplexity;

    // center point of cone base
    Point3D cConeBase = offsetOrigin;

    // starting value
    Point3D position = offsetOrigin;

    for (double i = segAngle; i < loftAngle; i += segAngle)
    {
        RotateTransform3D rt (AxisAngleRotation3D(axis, i), ConeLimitZ);
        Matrix3D m = rt.Value;

        // trasnform
        position = m.Transform(cConeBase);
        path.append(position);
    }

    // Set cylinder top/bottom to this new value
    displacedCylinderLimit = position;

    //add diameter of cone tip
    diameters.append(supportConeRad);
    //add diameters of column points
    for (int i = 0; i < path.count() - 1; i++)
    {
        diameters.append(supportCylRad * 2);
    }
}

bool SupportGenerator::TestSupportModelIntersection(QList<Point3D> origSupportPath, QList<double> origDiameters, ModelEntity* modelEntity)
{
    QList<Point3D> supportPath(origSupportPath);
    QList<double> diameters(origDiameters);
    MeshBuilder* supportMeshBuilder = new MeshBuilder(true, true);
    QList<double> expandedDiameters;
    for (int i = 0; i < diameters.count(); i++)
        expandedDiameters.append(diameters.at(i) + 2 * IntersectionSafetyDistance);
    BuildSupport(supportPath, expandedDiameters, MeshBuilder::GetCircle(circularThetaDiv), supportMeshBuilder);
    MeshGeometry3D* supportMesh = supportMeshBuilder->ToMesh(true);
    AABBTree supportAABB;
    supportAABB.PopulateFromMesh(supportMesh, Transform3D::Identity());

    bool result = supportAABB.Intersect(modelEntity->Tree());
    return result;
}

void SupportGenerator::BuildSupport(QList<Point3D> path, QList<double> diameters, QList<QPointF> section, MeshBuilder* meshBuilder)
{
    if (diameters.isEmpty())
    {
        throw new InvalidOperationException("cannot be zero diameters");
    }
    if (path.count() < 2 || section.count() < 2)
    {
        return;
    }

    if (diameters.count() == path.count())
    {
        if (diameters.at(0) != 0)
        {
            path.insert(0, path.at(0));
            diameters.insert(0, 0);
        }
        if (diameters.last() != 0)
        {
            path.append(path.last());
            diameters.append(0);
        }
    }

    int index0 = meshBuilder->Positions().count();

    int next = 1;
    while (next < path.count() && path.at(0) == path.at(next))
        next++;

    if (next == path.count())
        return; //null path (repeated points)

    Vector3D forward = path.at(next) - path.at(0);
    Vector3D up = forward.FindAnyPerpindicular();

    int diametersCount = diameters.count();

    for (int i = 0; i < path.count(); i++)
    {
        double r = diameters.at(i % diametersCount) / 2;

        if (i + 1 < path.count() && path.at(i) != path.at(i + 1))
            forward = path.at(i + 1) - path.at(i);
        Vector3D right = Vector3D::CrossProduct(up, forward);
        Vector3D up = Vector3D::CrossProduct(forward, right);
        up.normalize();
        right.normalize();
        Vector3D u = right;
        Vector3D v = up;
        for (int j = 0; j < section.count(); j++)
        {
            Vector3D w = (section.at(j).x() * u * r) + (section.at(j).y() * v * r);
            Point3D q = path.at(i) + w;
            meshBuilder->Positions().append(q);
            if (!meshBuilder->Normals()->isEmpty())
            {
                w.normalize();
                meshBuilder->Normals()->append(w);
            }

            if (!meshBuilder->TextureCoordinates()->isEmpty())
            {
                meshBuilder->TextureCoordinates()->append(QPointF());
            }

        }
    }

    meshBuilder->AddRectangularMeshTriangleIndices(index0, path.count(), section.count(), true, false);
}
