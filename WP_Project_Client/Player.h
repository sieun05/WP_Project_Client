#pragma once
#include "math.h"
#include "GameObject.h"
#include "Direct2D_Player_Render.h"

static constexpr int PLAYER_SPRITE_SIZE = 64;  // 한 프레임 크기
static constexpr int PLAYER_SPRITE_FRAMES = 4;   // 프레임 수

static constexpr int PLAYER_SIZE = 80;

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

		//25.06.08 - 오시은
		//캐릭터 크기 조정 변수 수정
		int half = PLAYER_SIZE / 2;
		int drawX = x - half;
		int drawY = y - half;

		int row = 0;
		switch (dir) {
		case DIR_DOWN:
		case DIR_DOWN_LEFT:
		case DIR_DOWN_RIGHT:
			row = 0; break;
		case DIR_LEFT:
			row = 1; break;
		case DIR_RIGHT:
			row = 2; break;
		case DIR_UP:
		case DIR_UP_LEFT:
		case DIR_UP_RIGHT:
			row = 3;
			break;
		default:
			row = 0; break;
		}

		int srcX = (p->anim % PLAYER_SPRITE_FRAMES) * PLAYER_SPRITE_SIZE;
		int srcY;

		//if (!p->isHurt)
		//	srcY = row * PLAYER_SPRITE_SIZE;
		//else
		//	
		//);

		srcY = (row + 4) * PLAYER_SPRITE_SIZE;
		D2D1_RECT_F destRect = D2D1::RectF(
			static_cast<FLOAT>(drawX),
			static_cast<FLOAT>(drawY),
			static_cast<FLOAT>(drawX + PLAYER_SIZE),
			static_cast<FLOAT>(drawY + PLAYER_SIZE)
		);

		D2D1_RECT_F srcRect = D2D1::RectF(
			static_cast<FLOAT>(srcX),
			static_cast<FLOAT>(srcY),
			static_cast<FLOAT>(srcX + PLAYER_SPRITE_SIZE),
			static_cast<FLOAT>(srcY + PLAYER_SPRITE_SIZE)
		);

		g_pRenderTarget->DrawBitmap(
			PlayerBmp,  // 알파 채널 포함된 플레이어 비트맵
			destRect,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			srcRect
		);
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