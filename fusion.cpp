#include <cmath>
#include <limits>

#include "point3d.h"
#include "fusion.h"
#include "clustering.h"

using namespace ModelViewer;

float JacardDistance::GetDistance(Point3D p1, Point3D p2)
{
    double x = p1.x() - p2.x();
    double y = p1.y() - p2.y();
    double z = p1.z() - p2.z();

    return sqrt(x * x + y * y + z * z);
}

float CompleteLinkage::CalculateDistance(Cluster *c1, Cluster *c2)
{
    float largestDistance = FLT_MIN;
    foreach (Point3D elementCluster1, c1->GetElements())
    {
        foreach (Point3D elementCluster2, c2->GetElements())
        {
            float distance = metric.GetDistance(elementCluster1, elementCluster2);
            if (distance > largestDistance)
                largestDistance = distance;
        }
    }

    return largestDistance;
}

float SingleLinkage::CalculateDistance(Cluster *c1, Cluster *c2)
{
    float leastDistance = FLT_MAX;
    foreach (Point3D elementCluster1, c1->GetElements())
    {
        foreach (Point3D elementCluster2, c2->GetElements())
        {
            float distance = metric.GetDistance(elementCluster1, elementCluster2);
            if (distance < leastDistance)
                leastDistance = distance;
        }
    }

    return leastDistance;
}
