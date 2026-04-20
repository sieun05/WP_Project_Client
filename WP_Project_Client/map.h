#pragma once
#include "math.h"

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

#define MAP_PIXEL_WIDTH MAP_WIDTH * CELL_SIZE
#define MAP_PIXEL_HEIGHT MAP_HEIGHT * CELL_SIZE

#define SCREEN_WIDTH_CELL_CNT 32
#define SCREEN_HEIGHT_CELL_CNT 18

class TileMap 
{
public:
	void Update(float deltaTime);
	void Render(int bx, int by);
	
	Cell& GetCell(int x, int y) { return game_map[y][x]; }
	void SetCell(int x, int y, const Cell& cell) { game_map[y][x] = cell; }
	void LoadMap(const Cell new_map[MAP_HEIGHT][MAP_WIDTH]);
	int& GetWaterSel() { return water_sel; }
	void SetWaterSel(int new_water_sel) { water_sel = new_water_sel; }

	void DrawGroundTile(int x, int y, int map_col, int map_row);
	void DrawWallTile(int x, int y, int map_col, int map_row, int wallpaint);
	void DrawGround2Tile(int groundtile_num, int x, int y, int map_col, int map_row);
	void DrawWaterTile(int x, int y, int map_col, int map_row);

	void DrawGroundwallTile(int x, int y, int map_col, int map_row);

private:
	Cell game_map[MAP_HEIGHT][MAP_WIDTH]{};
	int water_sel{};
};

extern TileMap g_map;