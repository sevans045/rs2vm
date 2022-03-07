#include <iostream>
#include <cstdint>
#include <sstream>

#include <QDataStream>
#include <QtEndian>

#include "RCServer.h"
#include "RCIOTask.h"

namespace rs2vm
{

void RCServer::incomingConnection(qintptr socketDescriptor)
{
    // new RCPacketHandler(socketDescriptor, this);
    auto* task = new RCIOTask{socketDescriptor};
    QThreadPool::globalInstance()->start(task);
}

void RCServer::startServer()
{
    listen(QHostAddress("localhost"), 10053);
}

void RCServer::closeServer()
{
    close();
}

// TODO: don't return anything and handle the data right away.
PlayerMeta RCPacketHandler::handlePlayerMeta(QDataStream& dataStream)
{
    if (dataStream.device()->size() < PlayerMeta::minDataSize())
    {
        qWarning() << "not enough data for PlayerMeta";
        return {};
    }

    PlayerMeta pm;
    uint32_t netIdA;
    uint32_t netIdB;

    dataStream >> pm.team;
    dataStream >> pm.squad;
    dataStream >> pm.playerId;
    dataStream >> netIdA;
    dataStream >> netIdB;
    // (B << 32) | A
    pm.uniqueNetId = (static_cast<uint64_t>(netIdB) << 32) | netIdA;
    // dataStream >> pm.uniqueNetId;
//	pm.team = static_cast<ETeam>(static_cast<uint8_t>(data[2]));
//	pm.squad = static_cast<uint8_t>(data[3]);
//	pm.playerId = qFromBigEndian(
//		static_cast<uint32_t>(data[4])		  |
//		static_cast<uint32_t>(data[5]) << 8   |
//		static_cast<uint32_t>(data[6]) << 16  |
//		static_cast<uint32_t>(data[7]) << 24);
//	pm.uniqueNetId = qFromBigEndian(
//		(static_cast<uint64_t>(data[8])        |
//		 static_cast<uint64_t>(data[9])  << 8  |
//		 static_cast<uint64_t>(data[10]) << 16 |
//		 static_cast<uint64_t>(data[11]) << 24
//		 ) << 32 |
//		(static_cast<uint64_t>(data[12])       |
//		 static_cast<uint64_t>(data[13]) << 8  |
//		 static_cast<uint64_t>(data[14]) << 16 |
//		 static_cast<uint64_t>(data[15]) << 24
//		 ));

    qWarning() << "team          " << static_cast<uint8_t>(pm.team);
    qWarning() << "squad         " << pm.squad;
    qWarning() << "playerId      " << pm.playerId;
    qWarning() << "uniqueNetId   " << pm.uniqueNetId;

    // TODO: Do murmur specific things with PlayerMeta data...
    //		 emit result?

    return pm;
}

PlayerStoppedSpeaking RCPacketHandler::handlePlayerStoppedSpeaking(QByteArray& data)
{
    return {};
}

PlayerSpeaking RCPacketHandler::handlePlayerSpeaking(QByteArray& data)
{
    return {};
}

PlayerSpeakingSpatialized RCPacketHandler::handlePlayerSpeakingSpatialized(QByteArray& data)
{
    return {};
}

PlayerDisconnected RCPacketHandler::handlePlayerDisconnected(QByteArray& data)
{
    return {};
}

PlayerSpawned RCPacketHandler::handlePlayerSpawned(QByteArray& data)
{
    return {};
}

PlayerDied RCPacketHandler::handlePlayerDied(QByteArray& data)
{
    return {};
}

PlayerSquadChanged RCPacketHandler::handlePlayerSquadChanged(QByteArray& data)
{
    return {};
}

PlayerTeamChanged RCPacketHandler::handlePlayerTeamChanged(QByteArray& data)
{
    return {};
}

void RCPacketHandler::disconnected()
{

}

uint32_t PlayerMeta::minDataSize()
{
    return RCPacket::minDataSize()
           + sizeof(uint8_t)
           + sizeof(uint8_t)
           + sizeof(uint32_t)
           + sizeof(uint64_t);
}

}
