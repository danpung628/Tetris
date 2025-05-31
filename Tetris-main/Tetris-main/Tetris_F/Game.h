#pragma once

#include "Board.h"
#include "Block.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "BlackoutManager.h"
#include "ConsoleUtils.h"
#include <iostream>

struct GameLevel
{
	int _clearLines;
	int _frameRate;
	int _stickRate;
};

class Game
{
public:
	Game();
	~Game();
	
	void Run();
	void ShowMenu(int* input);
	void ShowLevelSelect(int* input);
	void GameInit();

	void MapInit();

	void GamePlay();
	void StoryPlay();


	// GamePlay
	void Stage1();
	void Stage2();
	void Stage3();
	void Stage4();

	void Story1();
	void Story2();
	void Story3();
	void Story4();
	void Ending();


	void ProcessInput();
	int PlayGameTurn(const BlackoutManager* blackout = nullptr);
	void PrintStageInfo();			// ���� �޽��� ��� 
	void EraseStageInfo();			// ���� �޼��� ����
	void RenderBoardsAndNextBlock(); //���� �� ���� �� ���
	void ClearStage();				// �������� Ŭ���� �� ����
	

public:
	Board* _mainBoard;
	Board* _nextBoard;

	Block* _currentBlock;
	Block* _nextBlock;

	Renderer* _renderer;

	InputHandler* _inputHandler;

	int _level;
	int _clearLines;
	int _frameRate;
	int _stickRate;
	
	int _stage;
	bool _isReverseControl = false;
	BlackoutManager _blackout;
};
