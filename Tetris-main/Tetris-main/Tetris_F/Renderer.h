#pragma once

#include "Board.h"
#include "Block.h"
#include "BlackoutManager.h"
#include "ConsoleUtils.h"
#include <iostream>
#include <vector>


class Renderer
{
public:
	Renderer();
	~Renderer();

	//인자 추가 -> 블록 위치 + 암전 상태를 함께 전달
	void DrawBoard(const Board& board, const Block* block = nullptr, const BlackoutManager* blackout = nullptr);

	void DrawBlock(const Board& board, const Block& block, int sel);
	void EraseBlock(const Board& board, const Block& block);

	void ShowMessage(const std::string& message, int x = 0, int y = 0);
private:
	bool Row_Blackout(int row, const Block* block, const BlackoutManager* blackout) const;
};

