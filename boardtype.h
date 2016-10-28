#ifndef BOARDTYPE_H
#define BOARDTYPE_H

#include <QObject>

class BoardType : public QObject
{
    Q_OBJECT
public:
    explicit BoardType(QObject *parent = 0);

    QString MacAddress() { return _macAddress; }
    void SetMacAddress(QString value) { _macAddress = value; }

    QString MyIPAddress(){ return _myIPAddress; }
    void SetMYIPAddress(QString value) { _myIPAddress = value; }

    quint32 FWVersion() { return _fwVersion; }
    void SetFWVersion(quint32 value) { _fwVersion = value; }

    quint32 BoardID() { return _boardID; }
    void SetBoardID(quint32 value) { _boardID = value; }

    quint32 Type() { return _boardType; }
    void SetType(quint32 value) { _boardType = value; }

    QString LocalInterference() { return _localInterference; }
    void SetLocalInterference(QString value) { _localInterference = value; }

    QString AdapterFriendlyName() { return _adapterFriendlyName; }
    void SetAdapterFriendlyName(QString value) { _adapterFriendlyName = value; }

    QString ServerVersion() { return _serverVersion; }
    void SetServerVersion(QString value) { _serverVersion = value; }

    QString Session() { return _session; }
    void SetSession(QString value) { _session = value; }

    QByteArray Auth() { return _auth; }
    void SetAuth(QByteArray value) { _auth = value; }

    QString RuntimeElapsed() { return _runtimeElapsed; }
    void SetRuntimeElapsed(QString value) { _runtimeElapsed = value; }

    quint32 Status() { return _status; }
    void SetStatus(quint32 value) { _status = value; emit StatusChanged(); }

    QString Position() { return _position; }
    void SetPosition(QString value) { _position = value; emit PositionChanged(); }
signals:
    void StatusChanged();
    void PositionChanged();

private:
    QString _macAddress;
    QString _myIPAddress;
    quint32 _fwVersion;
    quint32 _boardID;
    quint32 _boardType;
    QString _localInterference;
    QString _adapterFriendlyName;
    QString _serverVersion;
    QString _session;
    QByteArray _auth;
    QString _runtimeElapsed;
    quint32 _status;
    QString _position;
};

#endif // BOARDTYPE_H
