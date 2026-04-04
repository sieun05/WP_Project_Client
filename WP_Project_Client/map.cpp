#include "map.h"
#include "Direct2D_Player_Render.h"

TileMap g_map;

void TileMap::Render(int bx, int by)
{
	// 타일맵 렌더링 로직 (예: Direct2D를 사용하여 화면에 그리기)

	int start_col{ bx / CELL_SIZE };
	int start_row{ by / CELL_SIZE };
	int offset_x{ bx % CELL_SIZE };
	int offset_y{ bx % CELL_SIZE };

	int wallpaint{};
	for (int row{}; row <= SCREEN_HEIGHT_CELL_CNT; ++row) {
		for (int col{}; col <= SCREEN_WIDTH_CELL_CNT; ++col) {
			int map_col = (start_col + col);
			int map_row = (start_row + row);

			if (map_row >= MAP_HEIGHT or map_col >= MAP_WIDTH) continue;

			int x = col * CELL_SIZE - offset_x;
			int y = row * CELL_SIZE - offset_y;

			// 타일 유형에 따라 다른 비트맵을 사용하여 렌더링
			// 예: 벽, 땅, 물 등
			switch (GetCell(map_col, map_row).type) {
			case 0: //땅
			case 8:
			case 9:
			case 10:
			case 11:
				DrawGroundTile(x, y, map_col, map_row);
				break;
			case 1:
			case 2:
			case 3:
				wallpaint = 0;
				DrawWallTime(x, y, map_col, map_row, wallpaint);
				break;
			case 5:
			case 6:
			{
				int groundtile_num{};
				if (GetCell(map_col, map_row).type == 5) {
					groundtile_num = 1;
				}
				else if (GetCell(map_col, map_row).type == 6) {
					groundtile_num = 2;
				}
				DrawGround2Tile(groundtile_num, x, y, map_col, map_row);
				break;
			}//switch(cell.type) case 5, 6
			case 7:
				DrawWaterTile(x, y, map_col, map_row);
				break;
			}//switch(GetCell(map_col, map_row).type)
		}//for col
	}//for row
}

void TileMap::LoadMap(const Cell new_map[MAP_HEIGHT][MAP_WIDTH])
{
	for (int y = 0; y < MAP_HEIGHT; ++y) {
		for (int x = 0; x < MAP_WIDTH; ++x) {
			game_map[y][x] = new_map[y][x];
		}
	}
}

void TileMap::DrawGroundTile(int screenX, int screenY, int mapX, int mapY)
{
	int srcX{}, srcY{};
	int sx{}, sy{};
	D2D1_RECT_F srcRect{};
	D2D1_RECT_F destRect = D2D1::RectF((float)screenX, (float)screenY, screenX + CELL_SIZE, screenY + CELL_SIZE);

	if (not
		(mapY + 1 < MAP_HEIGHT && game_map[mapY][mapX].type == 1 or
			mapY + 1 < MAP_HEIGHT && game_map[mapY][mapX].type == 2 or
			mapY + 1 < MAP_HEIGHT && game_map[mapY][mapX].type == 3)) {
		srcX = 0, srcY = 0;
		sx = 40 * srcX, sy = 40 * srcY;
		srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
		g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);

		//상하좌우 물일때
		{
			if (mapY - 1 > 0 and game_map[mapY - 1][mapX].type == 7) {
				srcX = 0, srcY = 1;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapX + 1 < MAP_WIDTH and game_map[mapY][mapX + 1].type == 7) {
				srcX = 1, srcY = 1;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapY + 1 < MAP_HEIGHT and game_map[mapY + 1][mapX].type == 7) {
				srcX = 2, srcY = 1;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapX - 1 > 0 and game_map[mapY][mapX - 1].type == 7) {
				srcX = 3, srcY = 1;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
		}

		//상하좌우 풀일때
		{
			if (mapY - 1 > 0 and game_map[mapY - 1][mapX].type == 5) {
				srcX = 0, srcY = 2;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapX + 1 < MAP_WIDTH and game_map[mapY][mapX + 1].type == 5) {
				srcX = 1, srcY = 2;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapY + 1 < MAP_HEIGHT and game_map[mapY + 1][mapX].type == 5) {
				srcX = 2, srcY = 2;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapX - 1 > 0 and game_map[mapY][mapX - 1].type == 5) {
				srcX = 3, srcY = 2;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
		}

		//상하좌우 주황풀일때
		{
			if (mapY - 1 > 0 and game_map[mapY - 1][mapX].type == 6) {
				srcX = 0, srcY = 3;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapX + 1 < MAP_WIDTH and game_map[mapY][mapX + 1].type == 6) {
				srcX = 1, srcY = 3;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapY + 1 < MAP_HEIGHT and game_map[mapY + 1][mapX].type == 6) {
				srcX = 2, srcY = 3;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapX - 1 > 0 and game_map[mapY][mapX - 1].type == 6) {
				srcX = 3, srcY = 3;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
		}

		//(25.06.10) - 오시은
		//제작대, 요리제작대, 상자, 나무
		if (game_map[mapY][mapX].type == 9) {
			srcX = 2, srcY = 4;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		else if (game_map[mapY][mapX].type == 10) {
			srcX = 1, srcY = 4;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		else if (game_map[mapY][mapX].type == 11) {
			srcX = 0, srcY = 4;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		else if (game_map[mapY][mapX].type == 8) {
			srcX = 1, srcY = 0;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			if (mapY - 1 > 0 and game_map[mapY - 1][mapX].type == 8) {
				srcX = 2, srcY = 5;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapX + 1 < MAP_WIDTH and game_map[mapY][mapX + 1].type == 8) {
				srcX = 1, srcY = 5;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapY + 1 < MAP_HEIGHT and game_map[mapY + 1][mapX].type == 8) {
				srcX = 3, srcY = 5;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapX - 1 > 0 and game_map[mapY][mapX - 1].type == 8) {
				srcX = 0, srcY = 5;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}

			if (mapY + 1 > 0 and (game_map[mapY + 1][mapX].type == 9 or game_map[mapY + 1][mapX].type == 10)) {
				srcX = 1, srcY = 6;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapY + 1 > 0 and game_map[mapY + 1][mapX].type == 11) {
				srcX = 0, srcY = 6;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
		}
	}
}

void TileMap::DrawWallTime(int screenX, int screenY, int mapX, int mapY, int wallpaint)
{
	int srcX{}, srcY{};
	int sx{}, sy{};
	D2D1_RECT_F srcRect{};
	D2D1_RECT_F destRect{ D2D1::RectF((float)screenX, (float)screenY, screenX + CELL_SIZE, screenY + CELL_SIZE) };

	if (mapY + 1 < MAP_HEIGHT &&
		(game_map[mapY + 1][mapX].type != 1 and game_map[mapY + 1][mapX].type != 2 and game_map[mapY + 1][mapX].type != 3)) {
		if (wallpaint == 1) return;

		srcX = 0, srcY = 0; //또는 1 0
		if ((mapY + mapX) % 3 == 0) srcX++;
		sx = 40 * srcX, sy = 40 * srcY;
		srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
		g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);

		//벽 바깥쪽모서리
		{
			if (mapX - 1 > 0 and (game_map[mapY][mapX - 1].type == 0 or game_map[mapY][mapX - 1].type == 7)) {
				srcX = 2, srcY = 0;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
			if (mapX + 1 < MAP_WIDTH and (game_map[mapY][mapX + 1].type == 0 or game_map[mapY][mapX + 1].type == 7)) {
				srcX = 3, srcY = 0;
				sx = 40 * srcX, sy = 40 * srcY;
				srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
				g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			}
		}

		//벽 앞에 풀밭
		if (mapY + 1 < MAP_HEIGHT and game_map[mapY + 1][mapX].type == 5) {
			srcX = 2, srcY = 2;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}

		//벽 앞에 주황풀
		if (mapY + 1 < MAP_HEIGHT and game_map[mapY + 1][mapX].type == 6) {
			srcX = 2, srcY = 3;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}

		//(25.06.10) - 오시은
		//벽 앞에 제작대, 요리제작대, 상자
		if (mapY + 1 < MAP_HEIGHT &&
			(game_map[mapY + 1][mapX].type == 9 or game_map[mapY + 1][mapX].type == 10)) {
			srcX = 1, srcY = 4;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		else if (mapY + 1 < MAP_HEIGHT &&
			game_map[mapY + 1][mapX].type == 11) {
			srcX = 0, srcY = 4;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
	}
	else {
		if (wallpaint == 0) return;

		srcX = 3; srcY = 2;
		//TransparentBlt(hdc, screenX, screenY, CELL_SIZE, CELL_SIZE, memdc2, 40 * srcX, 40 * srcY, 40, 40, RGB(0, 0, 255));
		sx = 40 * srcX, sy = 40 * srcY;
		srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
		g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);

		/*if (mapX - 1 > 0 and game_map[mapY][mapX - 1].type == 0) {
			srcX = 3; srcY = 1;
			TransparentBlt(hdc, screenX, screenY, CELL_SIZE, CELL_SIZE, memdc2, 40 * srcX, 40 * srcY, 40, 40, RGB(0, 0, 255));
		}
		if (mapX + 1 < MAP_WIDTH and game_map[mapY][mapX + 1].type == 0) {
			srcX = 1, srcY = 1;
			TransparentBlt(hdc, screenX, screenY, CELL_SIZE, CELL_SIZE, memdc2, 40 * srcX, 40 * srcY, 40, 40, RGB(0, 0, 255));
		}*/
		if (mapY + 2 < MAP_HEIGHT and
			(game_map[mapY + 1][mapX].type == 1 or game_map[mapY + 1][mapX].type == 2 or game_map[mapY + 1][mapX].type == 3) and
			not (game_map[mapY + 2][mapX].type == 1 or game_map[mapY + 2][mapX].type == 2 or game_map[mapY + 2][mapX].type == 3)) {
			srcX = 2, srcY = 1;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}

		//(25.06.10) - 오시은
		//철벽, 금벽 반짝이
		if (game_map[mapY + 1][mapX].type == 2) {
			int ran = 1; //랜덤으로 srcX 1, 2 바뀌게 해서 반짝이 효과 수정.
			srcX = 0 + ran, srcY = 3;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			//TransparentBlt(hdc, screenX, screenY, CELL_SIZE, CELL_SIZE, memdc2, 40 * srcX, 40 * srcY, 40, 40, RGB(0, 0, 255));
		}
		else if (game_map[mapY + 1][mapX].type == 3) {
			int ran = 1; //0 or 1 수정.
			srcX = 2 + ran, srcY = 3;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
			//TransparentBlt(hdc, screenX, screenY, CELL_SIZE, CELL_SIZE, memdc2, 40 * srcX, 40 * srcY, 40, 40, RGB(0, 0, 255));
		}
	}
}

void TileMap::DrawGround2Tile(int groundtile_num, int screenX, int screenY, int mapX, int mapY)
{
		int srcX{}, srcY{};
	int sx{}, sy{};
	D2D1_RECT_F srcRect{};
	D2D1_RECT_F destRect = D2D1::RectF((float)screenX, (float)screenY, screenX + CELL_SIZE, screenY + CELL_SIZE);

	srcX = 0, srcY = 0;
	sx = 40 * srcX, sy = 40 * srcY;
	srcRect = D2D1::RectF(0, 0, 40, 40);
	g_pRenderTarget->DrawBitmap(ground[groundtile_num], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
	/*if (not (mapY + 1 < MAP_HEIGHT && game_map[mapY + 1][mapX].type == 1)) {*/
}

void TileMap::DrawWaterTile(int screenX, int screenY, int mapX, int mapY)
{
	int srcX{}, srcY{};
	int sx{}, sy{};
	D2D1_RECT_F srcRect{};
	D2D1_RECT_F destRect = D2D1::RectF((float)screenX, (float)screenY, screenX + CELL_SIZE, screenY + CELL_SIZE);

	srcX = water_sel, srcY = 0;
	sx = 40 * srcX, sy = 40 * srcY;
	srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
	g_pRenderTarget->DrawBitmap(water, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
	//앞이 땅
	{
		if (mapY - 1 > 0 and
			(game_map[mapY - 1][mapX].type != 7)) {
			srcX = water_sel, srcY = 2;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(water, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);

			srcX = 0, srcY = 1;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(water, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		if (mapX + 1 < MAP_WIDTH and game_map[mapY][mapX + 1].type == 0) {
			srcX = 1, srcY = 1;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(water, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		if (mapY + 1 < MAP_HEIGHT and game_map[mapY + 1][mapX].type == 0) {
			srcX = 2, srcY = 1;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(water, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		if (mapX - 1 > 0 and game_map[mapY][mapX - 1].type == 0) {
			srcX = 3, srcY = 1;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(water, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
	}

	//상하좌우 풀일때
	{
		if (mapY - 1 > 0 and game_map[mapY - 1][mapX].type == 5) {
			srcX = 0, srcY = 2;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		if (mapX + 1 < MAP_WIDTH and game_map[mapY][mapX + 1].type == 5) {
			srcX = 1, srcY = 2;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		if (mapY + 1 < MAP_HEIGHT and game_map[mapY + 1][mapX].type == 5) {
			srcX = 2, srcY = 2;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		if (mapX - 1 > 0 and game_map[mapY][mapX - 1].type == 5) {
			srcX = 3, srcY = 2;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
	}

	//상하좌우 주황풀일때
	{
		if (mapY - 1 > 0 and game_map[mapY - 1][mapX].type == 6) {
			srcX = 0, srcY = 3;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		if (mapX + 1 < MAP_WIDTH and game_map[mapY][mapX + 1].type == 6) {
			srcX = 1, srcY = 3;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		if (mapY + 1 < MAP_HEIGHT and game_map[mapY + 1][mapX].type == 6) {
			srcX = 2, srcY = 3;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		if (mapX - 1 > 0 and game_map[mapY][mapX - 1].type == 6) {
			srcX = 3, srcY = 3;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(ground[0], destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
	}
}

void TileMap::DrawGroundwallTile(int screenX, int screenY, int mapX, int mapY)
{
	int srcX{}, srcY{};
	int sx{}, sy{};
	D2D1_RECT_F srcRect{};
	D2D1_RECT_F destRect = D2D1::RectF((float)screenX, (float)screenY + 5, screenX + CELL_SIZE, screenY + CELL_SIZE);

	if (mapY + 1 < MAP_HEIGHT &&
		(game_map[mapY + 1][mapX].type == 1 or game_map[mapY + 1][mapX].type == 2 or game_map[mapY + 1][mapX].type == 3)) {
		srcX = 3, srcY = 2; //또는 1 0
		sx = 40 * srcX, sy = 40 * srcY;
		srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
		g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		if (mapY + 2 < MAP_HEIGHT and
			(game_map[mapY + 1][mapX].type == 1 or game_map[mapY + 1][mapX].type == 2 or game_map[mapY + 1][mapX].type == 3) and
			not (game_map[mapY + 2][mapX].type == 1 or game_map[mapY + 2][mapX].type == 2 or game_map[mapY + 2][mapX].type == 3)) {
			srcX = 2, srcY = 1;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}

		//(25.06.10) - 오시은 
		//철벽, 금벽 반짝이
		if (game_map[mapY + 1][mapX].type == 2) {
			int ran = 1; //랜덤으로 srcX 1, 2 바뀌게 해서 반짝이 효과 수정.
			srcX = 0 + ran, srcY = 3;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
		else if (game_map[mapY + 1][mapX].type == 3) {
			int ran = 1; //0 or 1 수정
			srcX = 2 + ran, srcY = 3;
			sx = 40 * srcX, sy = 40 * srcY;
			srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
			g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
		}
	}

	//(25.06.10) - 오시은
	//제작대, 요리제작대, 상자
	if (mapY + 1 < MAP_HEIGHT &&
		(game_map[mapY + 1][mapX].type == 9 or game_map[mapY + 1][mapX].type == 10)) {
		srcX = 1, srcY = 4;
		sx = 40 * srcX, sy = 40 * srcY;
		srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
		g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
	}
	else if (game_map[mapY + 1][mapX].type == 11) {
		srcX = 0, srcY = 4;
		sx = 40 * srcX, sy = 40 * srcY;
		srcRect = D2D1::RectF(sx, sy, sx + 40, sy + 40);
		g_pRenderTarget->DrawBitmap(g_WallBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, srcRect);
	}
}
