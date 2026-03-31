#pragma once
#include "math.h"

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

#define MAP_PIXEL_WIDTH MAP_WIDTH * CELL_SIZE
#define MAP_PIXEL_HEIGHT MAP_HEIGHT * CELL_SIZE

class TileMap 
{
public:
	void Render() {
		// 타일맵 렌더링 로직 (예: Direct2D를 사용하여 화면에 그리기)
	}
	Cell& GetCell(int x, int y) { return game_map[y][x]; }
	void SetCell(int x, int y, const Cell& cell) { game_map[y][x] = cell; }
	void LoadMap(const Cell new_map[MAP_HEIGHT][MAP_WIDTH]) {
		for (int y = 0; y < MAP_HEIGHT; ++y) {
			for (int x = 0; x < MAP_WIDTH; ++x) {
				game_map[y][x] = new_map[y][x];
			}
		}
	}

private:
	Cell game_map[MAP_HEIGHT][MAP_WIDTH]{};
};

TileMap map;