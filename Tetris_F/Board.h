#pragma once

#include "Block.h"

class Board
{
public:
	Board() = delete;
	Board(int row, int col, int posX, int posY);
	virtual ~Board();

public:
	int _row;
	int _col;
	char** _board;
	int _posX;
	int _posY;
};

class NextBoard : public Board
{
public:
	NextBoard(int row = 6, int col = 8, int posX = 50, int posY = 3);
	~NextBoard();
};

class MainBoard : public Board
{
public:
	MainBoard(int row = 21, int col = 12, int posX = 24, int posY = 1);
	~MainBoard();

	void MainBoardInit();

	bool IsCollision(const Block& block) const;
	void MergeBlock(const Block& block);
	int ClearFullLines();
};
