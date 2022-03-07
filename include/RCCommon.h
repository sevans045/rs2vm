#ifndef RS2VM_RCCOMMON_H
#define RS2VM_RCCOMMON_H

namespace rs2vm
{

constexpr int BUFFER_SIZE = 255;

enum class EPacketID
{
    None = 0,
    PlayerMeta = 1,
    PlayerStoppedSpeaking = 2,
    PlayerSpeaking = 3,
    PlayerSpeakingSpatialized = 4,
    PlayerDisconnected = 5,
    PlayerSpawned = 6,
    PlayerDied = 7,
    PlayerSquadChanged = 8,
    PlayerTeamChanged = 9,
};

enum class ETeam
{
    Axis = 0,
    Allies = 1,
    None = 255,
};

QDataStream& operator>>(QDataStream& in, EPacketID& packetId);
QDebug operator<<(QDebug debug, EPacketID& packetId);
std::ostream& operator<<(std::ostream& os, EPacketID& packetId);
QDataStream& operator<<(QDataStream& out, EPacketID& packetId);

}

#endif //RS2VM_RCCOMMON_H
