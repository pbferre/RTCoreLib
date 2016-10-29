#include "supportactions.h"
#include "mainviewmodel.h"

#include <QDebug>
#include <QtCore>

#include <cmath>

using namespace CreateCore;

Rect3D SupportActions::SceneRect = Rect3D();
QList<SliceMesh*> SupportActions::meshList = QList<SliceMesh*>();

SupportActions::SupportActions()
{

}

void SupportActions::AddMesh(float *vec3Array, int size, float height, int entityID, float *transform)
{
    qDebug() << "ADDINGAMESH";

    qDebug() << "Saved points to C:\\Users\\BrandonFSL\\Documents\\Models\\GOOPOINTS.txt";
    qDebug() << "HEIGHT: " << height << " EntityID: " << entityID << " Transform: ";

    QList<float> trans;
    for (int i = 0; i < 16; i++)
    {
        //qDebug() <<("t: " + transform[i].ToString());
        trans.append(transform[i]);
    }

    Rect3D rect(vec3Array[0], vec3Array[1], vec3Array[2], 0, 0, 0);
    QList<Point3D> m;
    int points = 0;
    for (int num = 0; num < size; num += 3)
    {
        Point3D point(vec3Array[num], vec3Array[num + 1], vec3Array[num + 2]);
        m.append(point);
        rect.Union(point);
        SceneRect.Union(point);
        points++;
    }

    qDebug() << "Added Mesh: " + m.count();
    qDebug() << QString("Mesh rect bounds: %1,%2,%3  %4,%5,%6").arg(rect.X()).arg(rect.Y()).arg(rect.Z()).arg(rect.SizeX()).arg(rect.SizeY()).arg(rect.SizeZ());
    qDebug() << QString("Scene rect bounds: %1,%2,%3  %4,%5,%6").arg(SceneRect.X()).arg(SceneRect.Y()).arg(SceneRect.Z()).arg(SceneRect.SizeX()).arg(SceneRect.SizeY()).arg(SceneRect.SizeZ());

    SliceMesh* mesh = new SliceMesh(m, mtModel, rect, entityID, trans);
    meshList.append(mesh);
}

void SupportActions::AddManualPoints(float *vec3Array, int length)
{
    qDebug() << "AddManualPoints";
    double thickness = MVM->Parameters().LayerThickness();
    MVM->setNumSlices((int)ceil((SceneRect.SizeZ()) / thickness) - (int)floor(SceneRect.Z() / thickness) + 1);
    MainViewModel::CylinderPaths.clear();
    MainViewModel::CylinderDiams.clear();

    SliceMesh* mesh = meshList.at(0);

    SupportEntityPrep sep(mesh.positions, mesh.bounds, mesh.transformM44);


    QList<Point3D> m;
    for (int num = 0; num < length; num += 3)
    {
        Point3D point(vec3Array[num], vec3Array[num + 1], vec3Array[num + 2]);
        m.append(point);
    }

    QElapsedTimer et;
    et.start();
    MainViewModel.mvm.GetSingleSupport(m[0]);

    qDebug() << "Single Support total ms within mono: " + et.elapsed();
    length = MVM->CylinderPaths.count();
}
