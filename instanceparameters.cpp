#include "instanceparameters.h"

QDomDocument ConfigFiles::getConfigFile(configFileType type)
{
    if (type == cftConfig100)
        return Config100();
    else if (type == cftConfig100VSAT)
        return Config100VSAT();
    else
        return QDomDocument();
}

QDomDocument ConfigFiles::Config100()
{
    QDomDocument doc;
    QDomElement next;
    QDomText text;
    QString num;

    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("InstanceParameters");

    //QDomAttr ns1 = doc.createAttributeNS("http://www.w3.org/2001/XMLSchema", "xmlns:xsd", "");
    //QDomAttr ns2 = doc.createAttributeNS("http://www.w3.org/2001/XMLSchema-Instance", "xmlns:xsi", "");
    root.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-Instance");

    next = doc.createElement("ContourPower");
    num = QString::number(65535);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("OffsetPower");
    num = QString::number(65535);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("InfillPower");
    num = QString::number(65535);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("EnableXHoming");
    num = QString::number(0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("EnableYHoming");
    num = QString::number(0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XHomeMaxSteps");
    num = QString::number(100000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YHomeMaxSteps");
    num = QString::number(3000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XHomeReverseSteps");
    num = QString::number(5000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YHomeReverseSteps");
    num = QString::number(1500);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XHomeOffset");
    num = QString::number(1500);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YHomeOffset");
    num = QString::number(0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XStepsPerInch");
    num = QString::number(5062);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XAcceleration");
    num = QString::number(4000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XVelocity");
    num = QString::number(1000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XDivisor");
    num = QString::number(8);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YAcceleration");
    num = QString::number(10000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YVelocity");
    num = QString::number(1000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YDivisor");
    num = QString::number(3);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YTiltSteps");
    num = QString::number(1500);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XOffset");
    num = QString::number(1000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XCompleteSteps");
    num = QString::number(9000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MotorTilt");
    num = QString::number(1);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("PreDelay");
    num = QString::number(1000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("PostDelay");
    num = QString::number(5000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("LayerThickness");
    num = QString::number(0.1);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("SliceOffset");
    num = QString::number(0.15);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("Offsets");
    num = QString::number(2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("InfillScanGap");
    num = QString::number(0.4);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("WobbleFrequency");
    num = QString::number(2.0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("WobbleAmplitude");
    num = QString::number(0.2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("InfillLineDecrement");
    num = QString::number(0.15);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("InnerKerf");
    num = QString::number(-0.1);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("OuterKerf");
    num = QString::number(-0.09);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxSpeedInfill");
    num = QString::number(3.0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxSpeedOffset");
    num = QString::number(3.0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxSpeedContour");
    num = QString::number(3.0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("SlowestDrawSpeedRatio");
    num = QString::number(0.3);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MinPostDelayRatio");
    num = QString::number(0.4);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XMotorOffset");
    num = QString::number(4000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("AdherenceReps");
    num = QString::number(2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("SpacingX");
    num = QString::number(4);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("SpacingY");
    num = QString::number(4);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("ModelElevation");
    num = QString::number(6.9);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("BaseHeight");
    num = QString::number(2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("CylRadius");
    num = QString::number(0.9);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("ConeLength");
    num = QString::number(2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("ThetaDiv");
    num = QString::number(8);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MinIntSupportLength");
    num = QString::number(200);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxIntSupportLength");
    num = QString::number(2147483647);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MinBaseSupportLength");
    num = QString::number(3);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxBaseSupportLength");
    num = QString::number(2147483647);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("TouchPointWithdrawal");
    num = QString::number(-0.8);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("CornerDivisions");
    num = QString::number(10);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    doc.appendChild(root);
    return doc;
}

QDomDocument ConfigFiles::Config100VSAT()
{
    QDomDocument doc;
    QDomElement next;
    QDomText text;
    QString num;

    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("InstanceParameters");

    //QDomAttr ns1 = doc.createAttributeNS("http://www.w3.org/2001/XMLSchema", "xmlns:xsd", "");
    //QDomAttr ns2 = doc.createAttributeNS("http://www.w3.org/2001/XMLSchema-Instance", "xmlns:xsi", "");
    root.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-Instance");

    next = doc.createElement("ContourPower");
    num = QString::number(65535);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("OffsetPower");
    num = QString::number(65535);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("InfillPower");
    num = QString::number(65535);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("EnableXHoming");
    num = QString::number(0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("EnableYHoming");
    num = QString::number(0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XHomeMaxSteps");
    num = QString::number(100000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YHomeMaxSteps");
    num = QString::number(3000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XHomeReverseSteps");
    num = QString::number(5000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YHomeReverseSteps");
    num = QString::number(1500);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XHomeOffset");
    num = QString::number(1500);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YHomeOffset");
    num = QString::number(0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XStepsPerInch");
    num = QString::number(5062);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XAcceleration");
    num = QString::number(4000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XVelocity");
    num = QString::number(500);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XDivisor");
    num = QString::number(8);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YAcceleration");
    num = QString::number(10000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YVelocity");
    num = QString::number(1000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YDivisor");
    num = QString::number(3);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("YTiltSteps");
    num = QString::number(1500);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XOffset");
    num = QString::number(1000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XCompleteSteps");
    num = QString::number(9000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MotorTilt");
    num = QString::number(1);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("PreDelay");
    num = QString::number(1000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("PostDelay");
    num = QString::number(5000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("LayerThickness");
    num = QString::number(0.1);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("SliceOffset");
    num = QString::number(0.15);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("Offsets");
    num = QString::number(2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("InfillScanGap");
    num = QString::number(0.4);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("WobbleFrequency");
    num = QString::number(2.0);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("WobbleAmplitude");
    num = QString::number(0.2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("InfillLineDecrement");
    num = QString::number(0.15);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("InnerKerf");
    num = QString::number(-0.1);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("OuterKerf");
    num = QString::number(-0.09);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxSpeedInfill");
    num = QString::number(1.2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxSpeedOffset");
    num = QString::number(0.4);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxSpeedContour");
    num = QString::number(1.2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("SlowestDrawSpeedRatio");
    num = QString::number(0.3);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MinPostDelayRatio");
    num = QString::number(0.4);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("XMotorOffset");
    num = QString::number(4000);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("AdherenceReps");
    num = QString::number(2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("SpacingX");
    num = QString::number(4);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("SpacingY");
    num = QString::number(4);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("ModelElevation");
    num = QString::number(6.9);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("BaseHeight");
    num = QString::number(2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("CylRadius");
    num = QString::number(0.9);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("ConeLength");
    num = QString::number(2);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("ThetaDiv");
    num = QString::number(8);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MinIntSupportLength");
    num = QString::number(200);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxIntSupportLength");
    num = QString::number(2147483647);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MinBaseSupportLength");
    num = QString::number(3);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("MaxBaseSupportLength");
    num = QString::number(2147483647);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("TouchPointWithdrawal");
    num = QString::number(-0.8);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    next = doc.createElement("CornerDivisions");
    num = QString::number(10);
    text = doc.createTextNode(num);
    next.appendChild(text);
    root.appendChild(next);

    doc.appendChild(root);
    return doc;
}

InstanceParameters::InstanceParameters(QObject *parent) : QObject(parent)
{

}
