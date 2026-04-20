#pragma once
#include <vector>
#include "Player.h"

class PlayerManager
{
public:
    static PlayerManager& GetInstance();

    void AddPlayer(const Player& p);
    void Clear();

    std::vector<Player>& GetPlayers();

private:
    PlayerManager() = default;
    ~PlayerManager() = default;

private:
    std::vector<Player> players;
};