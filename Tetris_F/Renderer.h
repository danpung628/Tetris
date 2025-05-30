#pragma once

#include "Board.h"
#include "Block.h"

#include "ConsoleUtils.h"
#include <iostream>


class Renderer
{
public:
	Renderer();
	~Renderer();

	void DrawBoard(const Board& board);

	void DrawBlock(const Board& board, const Block& block, int sel);
	void EraseBlock(const Board& board, const Block& block);

	void ShowMessage(const std::string& message, int x = 0, int y = 0);
};

