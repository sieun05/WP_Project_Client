#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <winsock2.h>

#include "GameProtocol.h"
#include "PlayerManager.h"
#include "map.h"
#include "math.h"

using namespace std;

uint32_t g_myId = 0;

float g_x = 0.0f;
float g_y = 0.0f;
uint32_t g_dir = 0;

void ParseAssignPacket(char* data)
{
    char* ptr = data;

    // =========================
    // 내 ID
    // =========================
    uint32_t id;
    memcpy(&id, ptr, sizeof(id));
    ptr += sizeof(id);

    g_myId = ntohl(id);
    cout << "내 ID : " << g_myId << endl;

    // =========================
    // 플레이어 수
    // =========================
    uint32_t playerCount;
    memcpy(&playerCount, ptr, sizeof(playerCount));
    ptr += sizeof(playerCount);

    playerCount = ntohl(playerCount);
    cout << "플레이어 수 : " << playerCount << endl;

    // 🔥 기존 데이터 초기화
    PlayerManager::GetInstance().Clear();

    for (uint32_t i = 0; i < playerCount; i++)
    {
        NetPlayer np;
        memcpy(&np, ptr, sizeof(np));
        ptr += sizeof(np);

        int id = ntohl(np.id);
        int x = (int32_t)ntohl(np.x);
        int y = (int32_t)ntohl(np.y);
        int dir = ntohl(np.dir);

        // 🔥 Player 객체 생성
        Player p(id, Vec2(x, y), dir);

        PlayerManager::GetInstance().AddPlayer(p);

        cout << "Player " << id << " pos : "
            << x << "," << y << endl;

        // 내 플레이어 위치 저장
        if (id == g_myId)
        {
            g_x = x;
            g_y = y;
            g_dir = dir;
        }
    }

    // =========================
    // 맵 크기
    // =========================
    uint32_t width;
    uint32_t height;

    memcpy(&width, ptr, sizeof(width));
    ptr += sizeof(width);

    memcpy(&height, ptr, sizeof(height));
    ptr += sizeof(height);

    width = ntohl(width);
    height = ntohl(height);

    cout << "맵 크기 : " << width << " x " << height << endl;

    // 🔥 서버 맵 → TileMap으로 복사
    for (uint32_t x = 0; x < height; x++)
    {
        for (uint32_t y = 0; y < width; y++)
        {
            NetCell nc;
            memcpy(&nc, ptr, sizeof(nc));
            ptr += sizeof(nc);

            nc.type = ntohl(nc.type);
            nc.block_num = ntohl(nc.block_num);

            Cell cell{};
            cell.type = nc.type;
            cell.block_num = nc.block_num;

            g_map.SetCell(x, y, cell);
        }
    }

    cout << "맵 데이터 수신 완료" << endl;
}

void SendPlayerMove(SOCKET udpSocket, SOCKADDR_IN& serverAddr)
{
    char sendBuf[256];

    UdpPlayerMovePacket packet{};

    packet.header.playerId = htonl(g_myId);
    packet.header.type = htons((uint16_t)PacketType::PLAYER_MOVE);
    packet.dir = htonl(g_dir);

    memcpy(sendBuf, &packet, sizeof(packet));

    sendto(
        udpSocket,
        sendBuf,
        sizeof(packet),
        0,
        (SOCKADDR*)&serverAddr,
        sizeof(serverAddr)
    );
}