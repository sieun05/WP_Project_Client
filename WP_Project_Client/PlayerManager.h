#pragma once
#include <unordered_map>
#include "Player.h"

class PlayerManager 
{
public:
	void AddofUpdatePlayer(uint32_t id, int x, int y) {
		auto it = players.find(id);
		if (it != players.end()) {
			it->second.SetPos({ x, y });
		}
		else {
			players.emplace(id, Player(id, { x, y }, 0));
		}
	};
	void RemovePlayer(uint32_t id) {
		players.erase(id);
	};

	Player* GetPlayer(uint32_t id) {
		auto it = players.find(id);
		if (it != players.end()) {
			return &it->second;
		}
		return nullptr;
	}
	void Update(float deltaTime) {
		for (auto& pair : players) {
			pair.second.Update(deltaTime);
		}
	}
	void Render() {
		for (auto& pair : players) {
			pair.second.Render();
		}
	}

private:
	std::unordered_map<uint32_t, Player> players;
};

PlayerManager playerManager;