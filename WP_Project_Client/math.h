#pragma once

#define CELL_SIZE 60

struct Cell {
	Cell() = default;
	Cell(int type) : type(type), block_num(0), frame(0) {}
	Cell(int type, int block_num) : type(type), block_num(block_num), frame(0) {}
	Cell(const Cell& other) = default;
	Cell& operator=(const Cell& other) = default;

	int type;
	int block_num;
	int frame;
};

struct Vec2 {
	int x, y;
}; 

enum Direction {
	DIR_DOWN = 0,       // 아래
	DIR_DOWN_RIGHT,     // 아래-오른쪽 대각선
	DIR_RIGHT,          // 오른쪽
	DIR_UP_RIGHT,       // 위-오른쪽 대각선
	DIR_UP,             // 위
	DIR_UP_LEFT,        // 위-왼쪽 대각선
	DIR_LEFT,           // 왼쪽
	DIR_DOWN_LEFT       // 아래-왼쪽 대각선
};