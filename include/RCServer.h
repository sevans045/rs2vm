#ifndef RS2VM_RCSERVER_H
#define RS2VM_RCSERVER_H

#include <cstdint>
#include <memory>

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThreadPool>

#include "RCCommon.h"
#include "RCPacket.h"

namespace rs2vm
{

/// Rising Storm 2: Vietnam dedicated server remote control.
class RCServer : public QTcpServer
{
Q_OBJECT

public:
    explicit RCServer(QObject* parent = nullptr) :
        QTcpServer(parent)
    {}

protected:
    void incomingConnection(qintptr socketDescriptor) override;

public slots:

    void startServer();

    void closeServer();
};

class RCPacketHandler : public QObject
{
Q_OBJECT

public:
    // explicit RCPacketHandler(qintptr socketDescriptor, QObject* parent = nullptr);

private:
    static inline PlayerMeta handlePlayerMeta(QDataStream& dataStream);

    static inline PlayerStoppedSpeaking handlePlayerStoppedSpeaking(QByteArray& data);

    static inline PlayerSpeaking handlePlayerSpeaking(QByteArray& data);

    static inline PlayerSpeakingSpatialized handlePlayerSpeakingSpatialized(QByteArray& data);

    static inline PlayerDisconnected handlePlayerDisconnected(QByteArray& data);

    static inline PlayerSpawned handlePlayerSpawned(QByteArray& data);

    static inline PlayerDied handlePlayerDied(QByteArray& data);

    static inline PlayerSquadChanged handlePlayerSquadChanged(QByteArray& data);

    static inline PlayerTeamChanged handlePlayerTeamChanged(QByteArray& data);

private slots:

    void readSocketData();

    void disconnected();

signals:

    void resultReady();

    void error(QTcpSocket::SocketError socketError);

private:
    QTcpSocket* sock = nullptr;
};

}

#endif // RS2VM_RCSERVER_H
