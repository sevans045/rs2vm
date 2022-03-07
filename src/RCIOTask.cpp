#include "RCIOTask.h"
#include "RCCommon.h"
#include "RCPacket.h"

namespace rs2vm
{

RCIOTask::RCIOTask(qintptr socketDescriptor) : socketDescriptor(socketDescriptor)
{
    sock = new QTcpSocket{};
    if (!sock->setSocketDescriptor(socketDescriptor))
    {
        // emit error(sock->error());
        return;
    }

    connect(sock, &QTcpSocket::readyRead,
            this, &RCIOTask::readSocketData, Qt::DirectConnection);
    connect(sock, &QTcpSocket::disconnected,
            this, &RCIOTask::disconnected);
}

void RCIOTask::readSocketData()
{
    QByteArray data;
    data = sock->read(BUFFER_SIZE);

    QHostAddress peerAddr = sock->peerAddress();
    qWarning() << peerAddr << " data: " << data;

    if (data.size() < RCPacket::minDataSize())
    {
        qWarning() << "not enough data received";
        return;
    }

    QDataStream dataStream{data};
    // Sending from UnrealScript side in little endian.
    dataStream.setByteOrder(QDataStream::LittleEndian);

    uint32_t size;
    EPacketID packetId;
    // size = static_cast<uint32_t>(static_cast<uint8_t>(data[0]));a
    // packetId = static_cast<EPacketID>(static_cast<uint8_t>(data[1]));
    dataStream >> size;
    dataStream >> packetId;
    qWarning() << "device.size = " << dataStream.device()->size();
    qWarning() << "size = " << size;
    qWarning() << "packetId = " << packetId;

    switch (packetId)
    {
        case EPacketID::PlayerMeta:
            // handlePlayerMeta(dataStream);
            break;
        case EPacketID::PlayerStoppedSpeaking:
            // handlePlayerStoppedSpeaking(data);
            break;
        case EPacketID::PlayerSpeaking:
            // handlePlayerSpeaking(data);
            break;
        case EPacketID::PlayerSpeakingSpatialized:
            // handlePlayerSpeakingSpatialized(data);
            break;
        case EPacketID::PlayerDisconnected:
            // handlePlayerDisconnected(data);
            break;
        case EPacketID::PlayerSpawned:
            // handlePlayerSpawned(data);
            break;
        case EPacketID::PlayerDied:
            // handlePlayerDied(data);
            break;
        case EPacketID::PlayerSquadChanged:
            // handlePlayerSquadChanged(data);
            break;
        case EPacketID::PlayerTeamChanged:
            // handlePlayerTeamChanged(data);
            break;
        case EPacketID::None:
        default:
            break;
    }
}

void RCIOTask::run()
{

}

}
