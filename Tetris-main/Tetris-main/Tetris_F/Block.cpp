#include "Block.h"

Block::Block(int shapeType) : _shapeType(shapeType)
{
}

Block::~Block()
{
}

void Block::Move(int x, int y)
{
	_posX += x;
	_posY += y;
}

void Block::Rotate()
{
	_rotationAngle = (_rotationAngle + 1) % Config::NUM_ROTATIONS;
}

const char(*Block::GetShapeMatrix() const)[Config::BLOCK_GRID_SIZE]
{
	return Config::TETROMINO_SHAPES[_shapeType][_rotationAngle];
}
