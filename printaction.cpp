#include "printaction.h"

PrintAction::PrintAction(QObject *parent) : QObject(parent)
{
    _ipAddress = "";
    _jobID = 0;
    _jobPath = "";
    _busy = false;

    _currentCommand.clear();
    bytesToSend.clear();

    bool b = connectResponseSocket.bind(QHostAddress::LocalHost, 25);
    if (b)
        connect(&connectResponseSocket, SIGNAL(readyRead()), this, SLOT(getConnectionResponse()));
}

void PrintAction::sendConnectRequest()
{
    if (_ipAddress.isEmpty())
        emit error(_jobID, "Empty IP Address for Connect Request");

    QByteArray connectBytes;
    connectBytes.append(0xFF);
    connectBytes.append(0x04);
    connectBytes.append(0x02);
    connectBytes.append(0xFF);

    connect(&connectRequestSocket, SIGNAL(readyRead()), this, SLOT(getConnectionResponse()));
    connectRequestSocket.writeDatagram(connectBytes, QHostAddress(_ipAddress), 23);
}

void PrintAction::getConnectionResponse()
{
    while (connectRequestSocket.hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(connectRequestSocket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        connectRequestSocket.readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        qDebug() << datagram;

        QString s(datagram);
        if (s.contains(':'))
        {
            QStringList l = s.split(':');
            commandAddress = l.at(0);
            commandPort = l.at(1).toInt();
        }

        SendCommand(_currentCommand);
    }
}

void PrintAction::test()
{
    _ipAddress = "192.168.1.158";
    QFile f("C:\\Source\\RetinaCreate\\build2\\retinacreate\\Layout1.rc2");
    if (!f.exists())
    {
        emit error(_jobID, QString("File %1 does not exist").arg(f.fileName()));
        return;
    }
    _jobPath = f.fileName();
    Print();
}

void PrintAction::SendCommand(QString cmd)
{
    if (commandAddress.isEmpty() || commandPort == 0)
    {
        emit error(_jobID, "Invalid IP Address and/or port");
        return;
    }

    commandSocket.connectToHost(commandAddress, commandPort);
    if (commandSocket.waitForConnected(5000))
        qDebug() << "Connected";
    else
    {
        emit error(_jobID, QString("Unable to connect to %1 on port %2").arg(commandAddress).arg(commandPort));
        return;
    }

    QObject::disconnect(&commandSocket, 0, 0, 0);
    QObject::connect(&commandSocket, SIGNAL(readyRead()), this, SLOT(getCommandResponse()));
    commandSocket.write(cmd.toLatin1());
}

void PrintAction::getCommandResponse()
{
    QByteArray b = commandSocket.readAll();
    qDebug() << b;

    QString msg(b);
    if (msg == "UPLOAD OK")
        UploadFile();
    else if (msg == "OK" || msg == "UPLOAD SUCCESS")
        emit finished(_jobID);
    else
        emit error(_jobID, msg);
}

void PrintAction::Print()
{
    _busy = true;
    QString remotePath = "C:\\Temp\\";
    QFile f(_jobPath);
    if (!f.exists())
    {
        emit error(_jobID, QString("File %1 does not exits").arg(_jobPath));
        return;
    }
    QFileInfo finfo(f);
    remotePath += finfo.fileName();
    if (!f.open(QIODevice::ReadOnly))
    {
        emit error(_jobID, QString("Unable to open file %1").arg(_jobPath));
        return;
    }
    bytesToSend = f.readAll();
    f.close();

    _currentCommand = QString("UPLOAD!|%1|%2").arg(remotePath).arg(bytesToSend.length());
    sendConnectRequest();
}

void PrintAction::Stop()
{
    _busy = true;
    _currentCommand = "STOP!";
    sendConnectRequest();
}

void PrintAction::Pause()
{
    _busy = true;
    _currentCommand = "PAUSE!";
    sendConnectRequest();
}

void PrintAction::setJobParameters(QString address, int id, QString path)
{
    _ipAddress = address;
    _jobID = id;
    _jobPath = path;
}

void PrintAction::Clear()
{
    _currentCommand.clear();
    bytesToSend.clear();
    _jobID = 0;
    _jobPath = "";
    _busy = false;
}

void PrintAction::UploadFile()
{
    if (commandSocket.state() != QAbstractSocket::ConnectedState)
    {
        emit error(_jobID, QString("Disconnected from %1, unable to upload").arg(_ipAddress));
        return;
    }

    commandSocket.write(bytesToSend);
}
