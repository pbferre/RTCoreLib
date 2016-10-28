#ifndef PRINTACTION_H
#define PRINTACTION_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>

enum PrinterConnectionType
{
    pctNone = 0,
    pctMethernet,
    pctClose
};

struct IPEndpoint
{
    QHostInfo hostInfo;
    int port;
};

class PrintAction : public QObject
{
    Q_OBJECT
public:
    explicit PrintAction(QObject *parent = 0);

    QString IPAddress() { return _ipAddress; }
    int JobID() { return _jobID; }
    QString JobPath() { return _jobPath; }
    bool Busy() { return _busy; }
    QString CurrentCommand() { return _currentCommand; }
    void setJobParameters(QString address, int id, QString path = QString());

    void Print();
    void Stop();
    void Pause();

    void Clear();
    void test();

signals:
    void finished(int JobID);
    void error(int jobID, QString errMsg);

public slots:
    void getConnectionResponse();
    void getCommandResponse();

private:
    QString _ipAddress;
    int _jobID;
    QString _jobPath;
    bool _busy;
    QString _currentCommand;

    int commandPort;
    QString commandAddress;
    QByteArray bytesToSend;

    IPEndpoint enpoint;

    void sendConnectRequest();
    void SendCommand(QString cmd);
    void UploadFile();

    QUdpSocket connectRequestSocket;
    QUdpSocket connectResponseSocket;
    QTcpSocket commandSocket;
};

#endif // PRINTACTION_H
