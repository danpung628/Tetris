#include "Board.h"

Board::Board(int row, int col, int posX, int posY) 
	: _row(row), _col(col), _posX(posX), _posY(posY)
{
	_board = new char* [_row];
	for (int i = 0; i < _row; i++)
		_board[i] = new char[_col];
}

Board::~Board()
{
	for (int i = 0; i < _row; i++)
		delete[] _board[i];
	delete[] _board;
}

NextBoard::NextBoard(int row, int col, int posX, int posY) 
	: Board(row, col, posX, posY)
{
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _col; j++)
		{
			if (i == 0 || i == _row - 1 || j == 0 || j == _col - 1)
				_board[i][j] = 1;
			else
				_board[i][j] = 0;
		}
	}
}

NextBoard::~NextBoard()
{
}

MainBoard::MainBoard(int row, int col, int posX, int posY) 
	: Board(row, col, posX, posY)
{

}

MainBoard::~MainBoard()
{
}

void MainBoard::MainBoardInit()
{
	for (int i = 0; i < _row; i++)
	{
		for (int j = 0; j < _col; j++)
		{
			if (i == _row - 1 || j == 0 || j == _col - 1)
				_board[i][j] = 1;
			else
				_board[i][j] = 0;
		}
	}
}

bool MainBoard::IsCollision(const Block& block) const
{
	const char (*shapeMatrix)[Config::BLOCK_GRID_SIZE] = block.GetShapeMatrix();

	int block_dat;

	for (int r = 0; r < Config::BLOCK_GRID_SIZE; ++r)
	{
		for (int c = 0; c < Config::BLOCK_GRID_SIZE; ++c)
		{
			if (shapeMatrix[r][c] == 1)
			{
				int boardX = block._posX + c;
				int boardY = block._posY + r;

				if (boardX == 0 || boardX == _col)
					block_dat = 1;
				else
					block_dat = _board[block._posY + r][block._posX + c];

				if ((block_dat == 1)) // && shapeMatrix[r][c] == 1
					return true;
			}
		}
	}
	return false;
}

void MainBoard::MergeBlock(const Block& block)
{
	const char (*shapeMatrix)[Config::BLOCK_GRID_SIZE] = block.GetShapeMatrix();

	for (int r = 0; r < Config::BLOCK_GRID_SIZE; ++r)
	{
		for (int c = 0; c < Config::BLOCK_GRID_SIZE; ++c)
		{
			if (shapeMatrix[r][c] == 1)
			{
				_board[r + block._posY][c + block._posX] |= shapeMatrix[r][c];
			}
		}
	}
}

int MainBoard::ClearFullLines()
{
	int linesCleared = 0;
	// 바닥에서부터 위로 검사 (맨 윗줄은 검사 안함, 바닥벽 전까지)
	for (int r = _row - 2; r >= 0; --r)
	{
		bool lineFull = true;
		for (int c = 1; c < _col - 1; ++c)
		{ // 좌우 벽 제외
			if (_board[r][c] == 0)
			{ // 하나라도 빈 칸이 있으면
				lineFull = false;
				break;
			}
		}

		if (lineFull)
		{
			linesCleared++;

			for (int k = r; k > 0; --k)
			{ // k는 현재 줄부터 윗 줄까지
				for (int c = 1; c < _col - 1; ++c)
				{
					_board[k][c] = _board[k - 1][c]; // 윗 줄을 현재 줄로 복사
				}
			}

			for (int c = 1; c < _col - 1; ++c) {
				_board[0][c] = 0;
			}
			r++;
		}
	}
	return linesCleared;
}
