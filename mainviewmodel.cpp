#include "mainviewmodel.h"
#include "modelentity.h"

using namespace CreateCore;

QList<QList<Point3D>> MainViewModel::CylinderPaths = QList<QList<Point3D>>();
QList<QList<double>> MainViewModel::CylinderDiams = QList<QList<double>>();
QList<ModelEntity*> MainViewModel::listModelEntities = QList<ModelEntity*>();
MainViewModel* MainViewModel::MVM = 0;

bool littleEndian()
{
    short int word = 0x0001;
    char *byte = (char *) &word;
    return(byte[0] ? true : false);
}

MainViewModel::MainViewModel()
{
    if (!MVM)
        MVM = this;;
}

long MainViewModel::doubleToInt64Bits(double val)
{
    QByteArray b1;
    b1.setNum(val, 'e', 16);

    char c[sizeof(double)];
    memcpy(c, &val, sizeof(double));
    QByteArray b2(c, sizeof(double));
    if (littleEndian())
    {
        std::reverse(b2.begin(), b2.end());
    }
    b2=b2.toHex();
    return b2.toLong(0, 16);
}

long MainViewModel::GetPositionsHash(QList<Point3D> positions)
{
    int hash = 17;
    for (int i = 0; i < positions.count(); i++)
    {
        long x = doubleToInt64Bits(positions.at(i).x());
        long y = doubleToInt64Bits(positions.at(i).y());
        long z = doubleToInt64Bits(positions.at(i).z());
        long r = 17;
        r = 37 * r + (int)(x ^ (x >> 32));
        r = 37 * r + (int)(y ^ (y >> 32));
        r = 37 * r + (int)(z ^ (z >> 32));
        r += hash;
        hash = (r << 5) - r;
    }
    return hash;
}

void MainViewModel::SupportEntityPrep(QList<Point3D> positions, Rect3D bounds, QList<float> transform, SupportOperationType sot)
{
    qDebug() << "SUPPORTENTITYPREP+++++++++++++++";
    try
    {
        QList<Point3D> d(positions);
        long hash = GetPositionsHash(d);

        qDebug() << "hash: " << hash;
        if (!listModelEntities.isEmpty())
        {
            foreach (ModelEntity* me, listModelEntities)
            {
                if (me->PositionsHash == hash)
                {
                    currentEntity = me;
                    currentEntity->GenerateSupportsPrep(parameters, sot, positions, bounds);
                    return;
                }
            }
        }

        ModelEntity* modelEntity = new ModelEntity();
        modelEntity->PositionsHash = hash;
        listModelEntities.append(modelEntity);

        Model3DGroup* mg = new Model3DGroup();
        GeometryModel3D* gm3d = new GeometryModel3D();
        MeshGeometry3D* mg3d = new MeshGeometry3D();
        mg3d->Positions = positions;
        gm3d->Geometry = mg3d;
        mg->Children.append(gm3d);
        modelEntity->setModelGroup(mg);

        ModelVisual3DObservable* mv = new ModelVisual3DObservable();
        mv->Content = modelEntity->ModelGroup()->Children.first();
        modelEntity->setModelVisual3D(mv);

        modelEntity->GenerateAABBTree();
        modelEntity->GenerateDistanceTransform();
        modelEntity->GenerateSupportsPrep(MVM->Parameters(), sot, positions, bounds);
        currentEntity = modelEntity;

    }
    catch (...)
    {

    }
}

void MainViewModel::GetSingleSupport(Point3D mouse)
{
    currentEntity->GetSingleSupportEntity(mouse);
}
