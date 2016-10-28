#include "modelentity.h"

ModelEntity::ModelEntity()
{

}

void ModelEntity::GenerateAABBTree()
{
    aabbTree = new AABBTree();
    Transform3D transform;
    QElapsedTimer t;
    t.start();
    aabbTree->PopulateFromModel(modelGroup, transform);
    qDebug() << QString("AABB Tree Generation took %1 ms").arg(t.elapsed());
}
