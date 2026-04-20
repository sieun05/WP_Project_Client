#include "PlayerManager.h"

// 싱글톤
PlayerManager& PlayerManager::GetInstance()
{
    static PlayerManager instance;
    return instance;
}

// 플레이어 추가
void PlayerManager::AddPlayer(const Player& p)
{
    players.push_back(p);
}

// 전체 삭제
void PlayerManager::Clear()
{
    players.clear();
}

// 전체 접근
std::vector<Player>& PlayerManager::GetPlayers()
{
    return players;
}