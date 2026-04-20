#include "Network.h"
#include "GameProtocol.h"
#include "socket_f.h"
#include "PlayerManager.h"

bool NetworkInit()
{
    WSADATA wsdata;

    if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0)
        return false;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("192.168.0.63");

    if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) != 0)
        return false;

    u_long mode = 1;
    ioctlsocket(clientSocket, FIONBIO, &mode);

    // UDP
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    u_long udpmode = 1;
    ioctlsocket(udpSocket, FIONBIO, &udpmode);

    udpServer.sin_family = AF_INET;
    udpServer.sin_port = htons(UDP_PORT);
    udpServer.sin_addr.s_addr = inet_addr("192.168.0.63");

    return true;
}

void NetworkUpdate()
{
    // TCP 처리
    int recvLen = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (recvLen > 0)
    {
        PacketHeader header{};
        memcpy(&header, buffer, sizeof(header));

        header.type = ntohs(header.type);
        header.size = ntohl(header.size);

        char* payload = buffer + sizeof(PacketHeader);

        switch ((PacketType)header.type)
        {
        case PacketType::ASSIGN_ID:
            ParseAssignPacket(payload);
            SendPlayerMove(udpSocket, udpServer);
            break;
        }
    }

    // UDP 처리
    SOCKADDR_IN fromAddr{};
    int addrLen = sizeof(fromAddr);

    int udprecvLen = recvfrom(
        udpSocket,
        buffer,
        sizeof(buffer),
        0,
        (SOCKADDR*)&fromAddr,
        &addrLen
    );

    if (udprecvLen > 0)
    {
        UdpPacketHeader header{};
        memcpy(&header, buffer, sizeof(header));

        uint16_t type = ntohs(header.type);

        char* ptr = buffer + sizeof(UdpPacketHeader);

        if ((PacketType)type == PacketType::PLAYER_STATE)
        {
            uint16_t count;
            memcpy(&count, ptr, sizeof(count));
            ptr += sizeof(count);

            count = ntohs(count);

            // 🔥 여기 수정해야 함 (중요)
            PlayerManager::GetInstance().Clear();

            for (int i = 0; i < count; i++)
            {
                NetPlayer np{};
                memcpy(&np, ptr, sizeof(np));
                ptr += sizeof(np);

                int id = ntohl(np.id);
                float x = (int32_t)ntohl(np.x) / 100.0f;
                float y = (int32_t)ntohl(np.y) / 100.0f;
                int dir = ntohl(np.dir);

                Player p(id, Vec2(x, y), dir);
                PlayerManager::GetInstance().AddPlayer(p);

                if (id == g_myId)
                {
                    g_x = x;
                    g_y = y;
                    g_dir = dir;
                }
            }
        }
    }
}

void NetworkRelease()
{
    closesocket(clientSocket);
    closesocket(udpSocket);

    WSACleanup();
}
