#ifndef RS2VM_RCPACKET_H
#define RS2VM_RCPACKET_H

#include <cstdint>

#include "RCCommon.h"

namespace rs2vm
{

class RCPacket
{
public:
    RCPacket() : id(EPacketID::None)
    {};

    virtual ~RCPacket() = default;

    inline static uint32_t minDataSize()
    { return HEADER_SIZE; };

public:
    static constexpr uint8_t HEADER_SIZE = sizeof(uint8_t) * 2;
    EPacketID id;
};

class PlayerMeta : public RCPacket
{
public:
    inline static uint32_t minDataSize();

public:
    ETeam team;
    uint8_t squad;
    uint32_t playerId;
    uint64_t uniqueNetId;
};

class PlayerStoppedSpeaking : public RCPacket
{
};

class PlayerSpeaking : public RCPacket
{
};

class PlayerSpeakingSpatialized : public RCPacket
{
};

class PlayerDisconnected : public RCPacket
{
};

class PlayerSpawned : public RCPacket
{
};

class PlayerDied : public RCPacket
{
};

class PlayerSquadChanged : public RCPacket
{
};

class PlayerTeamChanged : public RCPacket
{
};

}

#endif //RS2VM_RCPACKET_H
