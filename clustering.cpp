#include "clustering.h"
#include "fusion.h"

using namespace ModelViewer;

template <class T>
KMeans<T>::KMeans(int k, QList<T> points, double (*dist)(T, T), T (*mean)(QList<T>))
{
    _k = k;
    _points = points;
    DistanceBetween = dist;
    MeanOfPoints = mean;

    InitializeClusters();
    bool anyChanged = (_points.count() > 0);
    while (anyChanged)
    {
        UpdateMeans();
        anyChanged = UpdateClusters();
    }
}

template <class T>
void KMeans<T>::InitializeClusters()
{
    _clusters.clear();
    if (_points.isEmpty())
    {
        return;
    }
    for (int i = 0; i < _k; i++)
    {
        clusters.Add(QList<T>());
    }
    int numPerCluster = (_points.count() + (k - 1)) / k;
    for (int i = 0; i < _points.count(); i++)
    {
        _clusters[i / numPerCluster].append(_points.at(i));
    }
}

template <class T>
void KMeans<T>::UpdateMeans()
{
    _means.clear();
    for (int i = 0; i < _k; i++)
        _means.append(Mean(_clusters.at(i)));
}

template <class T>
bool KMeans<T>::UpdateClusters()
{
    bool anyChanged = false;
    QList<QList<T>> newClusters;
    for (int i = 0; i < _k; i++)
    {
        newClusters.append(QList<T>());
    }

    for (int oldC = 0; oldC < _k; oldC++)
    {
        for (int i = 0; i < _clusters.at(oldC).count(); i++)
        {
            T p = _clusters.at(oldC).at(i);
            int newC = oldC;
            double minDistance = Dist(p, _means.at(oldC));
            for (int j = 0; j < k; j++)
            {
                double d = Dist(p, _means.at(j));
                if (d < minDistance)
                {
                    newC = j;
                    minDistance = d;
                }
            }
            if (oldC != newC)
                anyChanged = true;
            newClusters.at(newC).append(p);
        }
    }

    _clusters.clear();
    for (int i = 0; i < k; i++)
    {
        if (newClusters.at(i).count() > 0)
            _clusters.append(newClusters.at(i));
    }

    _k = _clusters.count();
    return anyChanged;
}

float Cluster::CalcualteDistance(Cluster *otherCluster)
{
    return fusion->CalculateDistance(this, otherCluster);
}

ClusterPair* ClusterPairs::LowestDistancePair()
{
    ClusterPair* lowestDistancePair = 0;
    foreach (ClusterPair* pair, pairs)
        if (!lowestDistancePair || lowestDistancePair->Distance() > pair->Distance())
        {
            if (ignore == false)
                lowestDistancePair = pair;
        }
    return lowestDistancePair;
}

void ClusterPairs::RemovePairsByOldClusters(Cluster c1, Cluster c2)
{
    QList<ClusterPair*> toRemove;
    foreach (ClusterPair* pair,  pairs)
    {
        if (pair->HasCluster(c1) || pair->HasCluster(c2))
        {
            toRemove.append(pair);
        }
    }
    foreach (ClusterPair* pair, toRemove)
    {
        pairs.removeAll(pair);
    }
}

QList<Cluster> HacStart::Clusters(float maximumDistance, int minCountClustersToCreate)
{
    AbstractCriterionAlgorithm aca(maximumDistance, minCountClustersToCreate);
    return aca.Clusters(elements, fusion, metric);
}

QList<Cluster> AbstractCriterionAlgorithm::Clusters(QList<Point3D> points, Fusion* f, IDistanceMetric* m)
{
    QList<Cluster> clusters;
    ClusterPairs pairs;

    // 1. Initialize each Point3D as a cluster
    foreach (Point3D el, points)
    {
        Cluster cl(f);
        cl.AddElement(el);
        clusters.append(cl);
    }

    // 2. a) Calculate the distances of all clusters to all other clusters
    foreach (Cluster cl1, clusters)
    {
        foreach (Cluster cl2, clusters)
        {
            if (cl1 == cl2)
                continue;
            ClusterPair* pair = new ClusterPair(cl1, cl2, cl1.CalcualteDistance(&cl2));
            pairs.AddPair(pair);
        }
    }

    // 2. b) Initialize the pair with the lowest distance to each other.
    ClusterPair* lowestDistancePair = pairs.LowestDistancePair();
    if (!lowestDistancePair) return QList<Cluster>();

    // 3. Merge clusters to new clusters and recalculate distances in a loop until there are only countCluster clusters
    while (!IsFinished(clusters, lowestDistancePair))
    {
        // a) Merge: Create a new cluster and add the elements of the two old clusters

        Cluster newCluster(f);
        newCluster.AddElements(lowestDistancePair->Cluster1().GetElements());
        newCluster.AddElements(lowestDistancePair->Cluster2().GetElements());
        // b)Remove the two old clusters from clusters
        clusters.removeAll(lowestDistancePair->Cluster1());
        clusters.removeAll(lowestDistancePair->Cluster2());
        // c) Remove the two old clusters from pairs
        pairs.RemovePairsByOldClusters(lowestDistancePair->Cluster1(), lowestDistancePair->Cluster2());

        // d) Calculate the distance of the new cluster to all other clusters and save each as pair
        foreach (Cluster cluster, clusters)
        {
            ClusterPair* pair = new ClusterPair(cluster, newCluster, cluster.CalcualteDistance(&newCluster));
            pairs.AddPair(pair);
        }
        // e) Add the new cluster to clusters
        clusters.append(newCluster);

        lowestDistancePair = pairs.LowestDistancePair();
    }

    return clusters;
}

bool AbstractCriterionAlgorithm::IsFinished(QList<Cluster> currentClusters, ClusterPair *lowestDistancePair)
{
    if (!lowestDistancePair)
        return true;

    return (lowestDistancePair->Distance() > maximumDistance) || (currentClusters.count() == minCountClustersToCreate);

}
