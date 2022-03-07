#include <iostream>
#include <sstream>

#include <QDataStream>
#include <QDebug>

#include "RCPacket.h"
#include "RCCommon.h"

namespace rs2vm
{

QDataStream& operator>>(QDataStream& in, EPacketID& packetId)
{
    return in >> reinterpret_cast<uint8_t&>(packetId);
}

std::ostream& operator<<(std::ostream& os, EPacketID& packetId)
{
    switch (packetId)
    {
        case EPacketID::None:
            return os << "EPacketID::None";
        case EPacketID::PlayerMeta:
            return os << "EPacketID::PlayerMeta";
        case EPacketID::PlayerStoppedSpeaking:
            return os << "EPacketID::PlayerStoppedSpeaking";
        case EPacketID::PlayerSpeaking:
            return os << "EPacketID::PlayerSpeaking";
        case EPacketID::PlayerSpeakingSpatialized:
            return os << "EPacketID::PlayerSpeakingSpatialized";
        case EPacketID::PlayerDisconnected:
            return os << "EPacketID::PlayerDisconnected";
        case EPacketID::PlayerSpawned:
            return os << "EPacketID::PlayerSpawned";
        case EPacketID::PlayerDied:
            return os << "EPacketID::PlayerDied";
        case EPacketID::PlayerSquadChanged:
            return os << "EPacketID::PlayerSquadChanged";
        case EPacketID::PlayerTeamChanged:
            return os << "EPacketID::PlayerTeamChanged";
    }
    return os << static_cast<uint32_t>(packetId);
}

QDataStream& operator<<(QDataStream& out, EPacketID& packetId)
{
    std::stringstream ss;
    ss << packetId;
    return out << ss.get();
}

QDebug operator<<(QDebug debug, EPacketID& packetId)
{
    std::stringstream ss;
    ss << packetId;
    return debug << ss.get();
}

}
