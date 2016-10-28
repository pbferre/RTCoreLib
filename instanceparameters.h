#ifndef INSTANCEPARAMETERS_H
#define INSTANCEPARAMETERS_H

#include <QObject>
#include <QtXml>

class ConfigFiles
{
public:
    enum configFileType
    {
        cftConfig100,
        cftConfig100VSAT
    };

    static QDomDocument getConfigFile(configFileType type);
    static QDomDocument Config100();
    static QDomDocument Config100VSAT();
};

class InstanceParameters : public QObject
{
    Q_OBJECT
public:
    explicit InstanceParameters(QObject *parent = 0);

signals:

public slots:
};

#endif // INSTANCEPARAMETERS_H
