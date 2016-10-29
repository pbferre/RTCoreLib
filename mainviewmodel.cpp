#include "mainviewmodel.h"

using namespace CreateCore;

MainViewModel::MainViewModel()
{
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

int MainViewModel::GetPositionsHash(QList<Point3D> positions)
{
    int hash = 17;
    for (int i = 0; i < positions.Count; i++)
    {
        long x = doubleToInt64Bits(positions.at(i).x());
        long y = doubleToInt64Bits(positions.at(i).y());
        long z = doubleToInt64Bits(positions.at(i).z());
        int r = 17;
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
        int hash = GetPositionsHash(d);

        qDebug() << "hash: " << hash;
        if (!listModelEntities.isEmpty())
        {
            foreach (ModelEntity* me, listModelEntities)
            {
                if (me->PositionsHash == hash)
                {
                    currentEntity = me;
                    currentEntity
                    return;
                }
            }

            var existing = ListModelEntities.Where(o => o.PositionsHash == hash);
            if (existing.Any())
            {
                Console.WriteLine("existing match: " + hash);
                CurrentEntity = existing.First();
                CurrentEntity.GenerateSupportsPrep(Instance, supportOperationType, positions, bounds);
                return;
            }
        }

        ModelEntity modelEntity = new ModelEntity();
        modelEntity.PositionsHash = hash;

        if (ListModelEntities != null)
        {
            ListModelEntities.Add(modelEntity);
        }
        else
        {
            ListModelEntities = new List<ModelEntity>() { modelEntity };
        }

        modelEntity.Model = new Model3DGroup();
        modelEntity.Model.Children = new List<Model3D>();
        modelEntity.Model.Children.Add(new GeometryModel3D() { Geometry = new MeshGeometry3D() { Positions = d, } });

        modelEntity.ModelVisual3D = new ModelVisual3DObservable();
        modelEntity.ModelVisual3D.Content = modelEntity.Model.Children.First();

        CurrentEntity = modelEntity;
        modelEntity.GenerateAABBTree();
        modelEntity.GenerateDistanceTansform();
        modelEntity.GenerateSupportsPrep(Instance, supportOperationType, positions, bounds);
    }
    catch (...)
    {

    }
}
