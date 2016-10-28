#ifndef MVMPARAMETERS_H
#define MVMPARAMETERS_H


class MVMParameters
{
public:
    MVMParameters();

    float LayerThickness() { return layerThickness; }
    void setLayerThickness( float value) { layerThickness = value; }

    float BaseSupportHeight() { return baseSupportHeight; }
    void setBaseSupportHeight(float value) { baseSupportHeight = value; }

    bool TreeSupports() { return treeSupports; }
    void setTreeSupports() { treeSupports = value; }

private:
    float layerThickness;
    float baseSupportHeight;

    bool treeSupports;
};

#endif // MVMPARAMETERS_H
