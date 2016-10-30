#include "support.h"

using namespace CreateCore;

//bool operator ==(Transform3D t1, Transform3D t2)
//{
//    return (t1.OffsetX == t2.OffsetX) && (t1.OffsetY == t2.OffsetY);
//}

//bool operator ==(Transform3DGroup t1, Transform3DGroup t2)
//{
//    return (t1.Children == t2.Children) && (t1.OffsetX == t2.OffsetX) && (t1.OffsetY == t2.OffsetY);
//}

//bool operator ==(Visual3D v1, Visual3D v2)
//{
//    return v1.Transform == v2.Transform;
//}

bool Transform3D::operator ==(Transform3D &t)
{
    return (this->OffsetX == t.OffsetX) && (this->OffsetY == t.OffsetY);
}

bool Transform3DGroup::operator ==(Transform3DGroup &tg)
{
    return (this->Children == tg.Children) && (this->OffsetX == tg.OffsetX) && (this->OffsetY == tg.OffsetY);
}

bool Visual3D::operator ==(Visual3D & v)
{
    return this->Transform == v.Transform;
}
