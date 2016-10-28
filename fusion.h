#ifndef FUSION_H
#define FUSION_H

#include <cmath>
#include <limits>

#include "point3d.h"

using namespace CreateCore;

namespace ModelViewer
{
class Cluster;

class IDistanceMetric
{
public:
    virtual float GetDistance(Point3D p1, Point3D p2);
};

class JacardDistance : public IDistanceMetric
{
public:
    float GetDistance(Point3D p1, Point3D p2);
};

class Fusion
{
public:
    virtual void SetDistanceMetric(IDistanceMetric value) { metric = value; }
    virtual float CalculateDistance(Cluster* c1, Cluster* c2);

protected:
    IDistanceMetric metric;
};

class CompleteLinkage : public Fusion
{
public:
    float CalculateDistance(Cluster *c1, Cluster *c2);
};

class SingleLinkage : public Fusion
{
    float CalculateDistance(Cluster *c1, Cluster *c2);
};

}

#endif // FUSION_H
