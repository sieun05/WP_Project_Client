#pragma once
#include "math.h"

class Player {
public:
	Player() = default;
	Player(int id, Vec2 pos, int dir)
		: id(id), pos(pos), dir(dir) {
	};

	int Getid() const { return id; }
	Vec2 GetPos() const { return pos; }
	int GetDir() const { return dir; }

	void SetId(int newId) { id = newId; }
	void SetPos(Vec2 newPos) { pos = newPos; }
	void SetDir(int newDir) { dir = newDir; }

	void Render() {
		// 플레이어 렌더링 로직 (예: Direct2D를 사용하여 화면에 그리기)
	}

private:
	int id;
	Vec2 pos;
	int dir;
};