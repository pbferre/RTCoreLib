#include "mvmparameters.h"

using namespace CreateCore;

MVMParameters::MVMParameters()
{
    supportSpace = 5.0;
    treeSupports = true;
    cylRadius = 0.9f;
    thetaDiv = 16;
    touchPointDiameter = 0.9f;
    cornerDivisions = 10;
    layerThickness = 0.1f;

    shellingThickness = 3.0f;
    shellingGridStepSize = 1.5f;
    shellingInteriorSupportSpacing = 6.5f;
    shellingInteriorSupportWidth = 0.8;
    shellingSubSampling = 1;
}
