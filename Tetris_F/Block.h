#pragma once

#include "Config.h"

class Block
{
public:
	Block() = delete;
	Block(int shpaeType);
	~Block();

	void Move(int x, int y);
	void Rotate();

	const char (*GetShapeMatrix() const)[Config::BLOCK_GRID_SIZE];

public:
	int _shapeType;
	int _rotationAngle = 0;
	int _posX = 2; // mainboard에 종속적인 거리
	int _posY = 0;
};

