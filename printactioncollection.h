#ifndef PRINTACTIONCOLLECTION_H
#define PRINTACTIONCOLLECTION_H

#include <QObject>

#include "printaction.h"

enum PrinterCommandType
{
    pctPrint,
    pctStop,
    pctPause
};

struct PrinterCommand{
    PrinterCommandType type;
    QString address;
    int id;
    QString fileName;

    PrinterCommand(PrinterCommandType t, QString a, int i, QString f)
    {
        type = t;
        address = a;
        id = i;
        fileName = f;
    }
};

class PrintActionCollection : public QObject
{
    Q_OBJECT
public:
    explicit PrintActionCollection(QObject *parent = 0);

    PrintAction* GetActionByAddress(QString address);
    PrintAction* GetActionByJobID(int jobID);

    void appendCommand(PrinterCommandType type, QString address, int id, QString fileName);

signals:
    void printActionFinished(QString msg);
    void printActionError(QString msg);

public slots:
    void checkCommands();
    void finished(int jobID);
    void error(int jobID, QString errMsg);

private:
    QList<PrintAction*> printActions;
    QList<PrinterCommand> commands;

    QTimer* commandTimer;

};

#endif // PRINTACTIONCOLLECTION_H
