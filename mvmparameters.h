#ifndef MVMPARAMETERS_H
#define MVMPARAMETERS_H

namespace CreateCore
{

class MVMParameters
{
public:
    MVMParameters();

    float LayerThickness() { return layerThickness; }
    void setLayerThickness( float value) { layerThickness = value; }

    float BaseSupportHeight() { return baseSupportHeight; }
    void setBaseSupportHeight(float value) { baseSupportHeight = value; }

    bool TreeSupports() { return treeSupports; }
    void setTreeSupports(bool value) { treeSupports = value; }

    double ShellingThickness() { return shellingThickness; }
    void setShellingThickness(double value) { shellingThickness = value; }

    double ShellingGridStepSize() { return shellingGridStepSize; }
    void setShellingGridStepSize(double value) { shellingGridStepSize = value; }

    double ShellingInteriorSupportSpacing() { return shellingInteriorSupportSpacing; }
    void setShellingInteriorSupportSpacing(double value) { shellingInteriorSupportSpacing = value; }

    double ShellingInteriorSupportWidth() { return shellingInteriorSupportWidth; }
    void setShellingInteriorSupportWidth(double value) { shellingInteriorSupportWidth = value; }

    int ShellingSubSampling() { return shellingSubSampling; }
    void setShellingSubSampling(double value) { shellingSubSampling = value; }

    double SupportSpace() { return supportSpace; }
    void setSupportSpace(double value) { supportSpace = value; }

    float CylRadius() { return cylRadius; }
    void setCylRadius(float value ) { cylRadius = value; }

    int ThetaDiv() { return thetaDiv; }
    void setThetaDiv(int value) { thetaDiv = value; }

    float TouchPointDiameter() { return touchPointDiameter; }
    void setTouchPointDiameter(float value) { touchPointDiameter = value; }

    int CornerDivisions() { return cornerDivisions; }
    void setCornerDivisions(int value) { cornerDivisions = value; }

private:
    float layerThickness;
    float baseSupportHeight;
    float cylRadius;
    float touchPointDiameter;

    double shellingThickness;
    double shellingGridStepSize;
    double shellingInteriorSupportSpacing;
    double shellingInteriorSupportWidth;
    int shellingSubSampling;

    double supportSpace;
    int thetaDiv;
    int cornerDivisions;

    bool treeSupports;
};

}
#endif // MVMPARAMETERS_H
