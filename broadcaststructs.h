#ifndef BROADCASTSTRUCTS_H
#define BROADCASTSTRUCTS_H

#include <QtCore>

namespace CreateCore_Network
{

struct BroadcastStruct
{
    char TagStatus;
    char pucBuffer;
    char CMD_DISCOVER_TARGET;
    char ucBoardType;
    char ucBoardID;
    unsigned int ulCLientIPAddr;
    unsigned int ulVersion;

    QByteArray pucMACArray;
    QByteArray pcAppTitle;
    QByteArray auth;

    BroadcastStruct()
    {
        TagStatus = 0;
        pucBuffer = 0;
        CMD_DISCOVER_TARGET = 0;
        ucBoardID = 0;
        ucBoardType = 0;
        ulCLientIPAddr = 0;
        ulVersion = 0;

        pucMACArray.resize(6);
        pcAppTitle.resize(64);
        auth.resize(256);
    }
};

}
#endif // BROADCASTSTRUCTS_H
