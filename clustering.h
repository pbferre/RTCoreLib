#ifndef CLUSTERING_H
#define CLUSTERING_H

#include <QtCore>

#include "point3d.h"
#include "fusion.h"

using namespace CreateCore;

namespace ModelViewer
{
class Fusion;

template <class T>
class KMeans
{
public:
    //KMeans(int k, QList<T> points, DistanceBetween dist, MeanOfPoints mean);
    KMeans(int k, QList<T> points, double(*dist)(T, T), T(*mean)(QList<T>));

    //DistanceBetween Dist;
    //MeanOfPoints Mean;

    void InitializeClusters();
    void UpdateMeans();
    bool UpdateClusters();

private:
    int _k;
    QList<T> _points;
    QList<QList<T>> _clusters;
    QList<T> _means;
    double (*DistanceBetween)(T p1, T p2);
    T (*MeanOfPoints)(QList<T> pList);
};

class Cluster
{
public:
    Cluster()
    {
        fusion = 0;
        elements.clear();
    }

    Cluster(Fusion* f) { fusion = f; }
    void AddElement(Point3D p) { elements.append(p); }
    void AddElements(QList<Point3D> points) { elements.append(points); }
    QList<Point3D> GetElements() const { return elements; }
    float CalcualteDistance(Cluster* otherCluster);

    bool operator ==(const Cluster &cluster) { return this->GetElements() == cluster.GetElements(); }

private:
    QList<Point3D> elements;
    Fusion* fusion;
};

class ClusterPair
{
public:
    ClusterPair(Cluster c1, Cluster c2, float dist)
    {
        cluster1 = c1;
        cluster2 = c2;
        distance = dist;
    }

    Cluster Cluster1() { return cluster1; }
    Cluster Cluster2() { return cluster2; }
    float Distance() { return distance; }

    bool HasCluster(Cluster c) { return (cluster1 == c) || (cluster2 == c); }

private:
    Cluster cluster1, cluster2;
    float distance;
};

class ClusterPairs
{
public:
    ClusterPair* LowestDistancePair();
    int Count() { return pairs.count(); }
    void AddPair(ClusterPair* pair) { pairs.append(pair); }
    void RemovePairsByOldClusters(Cluster c1, Cluster c2);

private:
    QList<ClusterPair*> pairs;
    bool ignore;
};

class HacStart
{
public:
    HacStart(QList<Point3D> elementList)
    {
        elements.clear();
        SetElements(elementList);
        fusion = new CompleteLinkage();
        metric = new JacardDistance();
    }

    HacStart(QList<Point3D> elementList, Fusion* f, IDistanceMetric* m)
    {
        elements.clear();
        SetElements(elementList);
        fusion = f;
        metric = m;
    }

    void SetElements(QList<Point3D> elementList) {elements.append(elementList); }

    QList<Cluster> Clusters(float maximumDistance, int minCountClustersToCreate);

private:
    QList<Point3D> elements;
    ClusterPairs pairs;
    Fusion* fusion;
    IDistanceMetric* metric;
};

class AbstractCriterionAlgorithm
{
public:
    AbstractCriterionAlgorithm(float dist, int count)
    {
        maximumDistance = dist;
        minCountClustersToCreate = count;
    }

    QList<Cluster> Clusters(QList<Point3D> points, Fusion* f, IDistanceMetric* m);
    bool IsFinished(QList<Cluster> clusters, ClusterPair* lowestDistancePair);

private:
    float maximumDistance;
    int minCountClustersToCreate;
};

}

#endif // CLUSTERING_H
