#include "printactioncollection.h"

PrintActionCollection::PrintActionCollection(QObject *parent) : QObject(parent)
{
    printActions.clear();
    commands.clear();

    commandTimer = new QTimer(this);
    connect(commandTimer, SIGNAL(timeout()), this, SLOT(checkCommands()));
    commandTimer->setInterval(1000);
    commandTimer->start();
}

PrintAction* PrintActionCollection::GetActionByAddress(QString address)
{
    for (int i = 0; i < printActions.count(); i++)
    {
        PrintAction* pa = printActions.at(i);
        if (pa->IPAddress() == address)
            return pa;
    }

    PrintAction* pa = new PrintAction();
    connect(pa, SIGNAL(finished(int)), this, SLOT(finished(int)));
    connect(pa, SIGNAL(error(int,QString)), this, SLOT(error(int,QString)));
    pa->setJobParameters(address, 0);
    printActions.append(pa);
}

PrintAction* PrintActionCollection::GetActionByJobID(int jobID)
{
    for (int i = 0; i < printActions.count(); i++)
    {
        PrintAction* pa = printActions.at(i);
        if (pa->JobID() == jobID)
            return pa;
    }

    return 0;
}

void PrintActionCollection::appendCommand(PrinterCommandType type, QString address, int id, QString fileName)
{
    PrinterCommand cmd(type, address, id, fileName);
    commands.append(cmd);
}

void PrintActionCollection::checkCommands()
{
    if (commands.count() > 0)
    {
        for (int i = 0; i < commands.count(); i++)
        {
            PrinterCommand cmd = commands.at(i);
            PrintAction* pa = GetActionByAddress(cmd.address);
            if (!pa->Busy())
            {
                pa->setJobParameters(cmd.address, cmd.id, cmd.fileName);
                switch(cmd.type)
                {
                case pctPrint:
                    pa->Print();
                    break;
                case pctStop:
                    pa->Stop();
                    break;
                case pctPause:
                    pa->Pause();
                    break;
                }

                commands.removeAt(i);
            }
        }
    }
}

void PrintActionCollection::finished(int jobID)
{
    PrintAction* pa = GetActionByJobID(jobID);
    if (pa)
    {
        QString ipAddress = pa->IPAddress();
        QString currentCommand = pa->CurrentCommand();
        currentCommand = currentCommand.replace('!', "");
        pa->Clear();
        QString msg = QString("%1 command successfully execute on printer at %2, Job ID %3").arg(currentCommand).arg(ipAddress).arg(jobID);
        emit printActionFinished(msg);
    }
}

void PrintActionCollection::error(int jobID, QString errMsg)
{
    PrintAction* pa = GetActionByJobID(jobID);
    if (pa)
    {
        QString ipAddress = pa->IPAddress();
        QString currentCommand = pa->CurrentCommand();
        currentCommand = currentCommand.replace('!', "");
        pa->Clear();
        QString msg = QString("Error executing %1 command on printer at %2, Job ID %3. %4").arg(currentCommand).arg(ipAddress).arg(jobID).arg(errMsg);
        emit printActionError(msg);
    }
}
