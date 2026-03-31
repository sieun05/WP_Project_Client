#pragma once
#include "math.h"
#include "GameObject.h"

class Player : public GameObject {
public:
	Player() = default;
	Player(int id, Vec2 pos, int dir)
		: GameObject(pos), id(id), dir(dir) {};
	virtual ~Player() = default;

	const int& Getid() const { return id; }
	const Vec2& GetPos() const { return pos; }
	const int& GetDir() const { return dir; }

	void SetId(const int& newId) { id = newId; }
	void SetPos(const Vec2& newPos) { pos = newPos; }
	void SetDir(const int& newDir) { dir = newDir; }

	void Update(float deltaTime) override {
		// 플레이어 상태 업데이트 로직 (예: 이동, 애니메이션 등)
	}
	void Render() override {
		// 플레이어 렌더링 로직 (예: Direct2D를 사용하여 화면에 그리기)
	}

private:
	int id;
	int dir;
};

// - 구현 -
// 이동
//
// - 확장 가능 -
// 생존/사망/피해 상태
// 빠르게 걷기, 조용히 걷기
// 인벤토리, 장비
// 체력, 스태미나, 허기, 마나
// 공격력, 방어력, 이동속도, 공격속도
// 공격액션, 방어액션
// 
//