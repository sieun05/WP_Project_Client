//GameProtocol.h

#pragma once
#include <cstdint>

enum class PacketType :uint16_t
{
	ASSIGN_ID = 1,
	PLAYER_STATE,
	PLAYER_MOVE,
	MAP_INIT,
	MAP_UPDATE,
};

#pragma pack(push, 1)

struct PacketHeader
{
	uint16_t type;
	uint32_t size;
};

struct NetPlayer
{
	uint32_t id;
	int32_t x;
	int32_t y;
	uint32_t dir;
};

struct NetCell
{
	int32_t type;
	int32_t block_num;
};

#pragma pack(pop)

struct UdpPacketHeader {
	uint32_t playerId;
	uint16_t type;
};

#pragma pack(push,1)

struct UdpPlayerStatePacket
{
	UdpPacketHeader header;
	uint16_t playerCount;
};

#pragma pack(pop)


#pragma pack(push,1)

struct UdpPlayerMovePacket
{
	UdpPacketHeader header;
	uint32_t dir;
};

#pragma pack(pop)

#pragma pack(push,1)

struct NetCellUpdate
{
	uint16_t x;
	uint16_t y;
	int32_t type;
	int32_t block_num;
};

struct UdpMapUpdatePacket
{
	UdpPacketHeader header;
	uint16_t updateCount;
};

#pragma pack(pop)