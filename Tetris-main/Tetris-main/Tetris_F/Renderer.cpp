#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::DrawBoard(const Board& board)
{
	for (int i = 0; i < board._row; i++)
	{
		for (int j = 0; j < board._col; j++)
		{
			ConsoleUtils::gotoxy(board._posX + 2 * j, board._posY + i);
			if (board._board[i][j] == 1)
				std::cout << "■";
			else
				std::cout << "  ";
		}
		std::cout << std::endl;
	}
	ConsoleUtils::gotoxy();
}

void Renderer::DrawBlock(const Board& board, const Block& block, int sel)
{
	for (int i = 0; i < Config::BLOCK_GRID_SIZE; i++)
	{
		for (int j = 0; j < Config::BLOCK_GRID_SIZE; j++)
		{
			if (sel == 1) // Main - 여기 block.posX 고민
				ConsoleUtils::gotoxy(board._posX + (block._posX * 2) + 2 * j, board._posY + block._posY + i);
			else if (sel == 2) // Next
				ConsoleUtils::gotoxy(board._posX + 4 + 2 * j, board._posY + 2 + i);
			
			if ((block.GetShapeMatrix())[i][j] == 1) 
			{
				std::cout << "■";
			}
		}
		std::cout << std::endl;
	}
	ConsoleUtils::gotoxy();
}

void Renderer::EraseBlock(const Board& board, const Block& block)
{
	for (int i = 0; i < Config::BLOCK_GRID_SIZE; i++)
	{
		for (int j = 0; j < Config::BLOCK_GRID_SIZE; j++)
		{ // - 여기도
			ConsoleUtils::gotoxy(board._posX + (block._posX * 2) + 2 * j, board._posY + block._posY + i);
			if ((block.GetShapeMatrix())[i][j] == 1)
			{
				std::cout << "  ";
			}
		}
		std::cout << std::endl;
	}
	ConsoleUtils::gotoxy();
}


void Renderer::ShowMessage(const std::string& message, int x, int y)
{
	ConsoleUtils::gotoxy(x, y);
	std::cout << message << std::endl;
	ConsoleUtils::gotoxy(0, 0);
}
