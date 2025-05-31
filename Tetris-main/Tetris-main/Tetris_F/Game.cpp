#include "Game.h"
#include <chrono>

GameLevel gameLevel[3] =
{
	{4, 500, 0},
	{20, 250, 0},
	{30, 100, 0}
};

Game::Game()
	: _mainBoard(new MainBoard()),
	_nextBoard(new NextBoard()),
	_renderer(new Renderer())
{
	
}

Game::~Game()
{
	delete _mainBoard;
	delete _nextBoard;
	delete _renderer;
}

void Game::Run()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	while (true)
	{
		int input;
		ShowMenu(&input);
		if (input == 1)
		{
			ShowLevelSelect(&input);
			_level = input;
			GameInit(); // -���⼭ �� �ʱ�ȭ
			while (_stage < 5)
			{
				ConsoleUtils::showCursor();
				StoryPlay();
				ConsoleUtils::hideCursor();
				GamePlay();
			}
			Ending();
		}
		else if (input == 2)
			return;
	}
}

void Game::ShowMenu(int* input)
{
	std::cout << "1. ���� ����" << std::endl;
	std::cout << "2. ���� ����" << std::endl;
	std::cin >> *input;
	ConsoleUtils::clearScreen();
}

void Game::ShowLevelSelect(int* input)
{
	std::cout << "1. 1�г�" << std::endl;
	std::cout << "2. 2�г�" << std::endl;
	std::cout << "3. 3�г�" << std::endl;
	std::cin >> *input;
	ConsoleUtils::clearScreen();
}

void Game::GameInit()
{
	_clearLines = gameLevel[_level - 1]._clearLines;
	_frameRate = gameLevel[_level - 1]._frameRate;
	_stickRate = gameLevel[_level - 1]._stickRate;
	_stage = 4; // �ϴ� stage 3 ���� �����ϰ� ����

	// ��� ����
	if (_currentBlock == nullptr)
		//_currentBlock = new Block(rand()%7);
		_currentBlock = new Block(0);
	if (_nextBlock == nullptr)
		//_nextBlock = new Block(rand() % 7);
		_nextBlock = new Block(0);
}

void Game::MapInit()
{
	((MainBoard*)_mainBoard)->MainBoardInit();

	if (_currentBlock == nullptr)
		/*_currentBlock = new Block(rand() % 7);*/
		_currentBlock = new Block(0);
	if (_nextBlock == nullptr)
		//_nextBlock = new Block(rand() % 7);
		_nextBlock = new Block(0);
}

void Game::GamePlay()
{
	switch (_stage)
	{
	case 1:
		Stage1();
		break;
	case 2:
		Stage2();
		break;
	case 3:
		Stage3();
		break;
	case 4:
		Stage4();
		break;
	}
}

void Game::StoryPlay()
{
	switch (_stage)
	{
	case 1:
		Story1();
		break;
	case 2:
		Story2();
		break;
	case 3:
		Story3();
		break;
	case 4:
		Story4();
		break;
	}
}

void Game::Stage1()
{
	MapInit();

	std::cout << "STAGE: " << _stage << std::endl;
	std::cout << "LEVEL: " << _level << std::endl;
	std::cout << "GOALS: " <<_clearLines << std::endl;

	_renderer->DrawBoard(*_mainBoard);
	_renderer->DrawBoard(*_nextBoard);

	_renderer->DrawBlock(*_nextBoard, *_nextBlock, 2);

	while (true)	// �̺κе� �Լ��� ���� ���� ������
	{
		_renderer->DrawBlock(*_mainBoard, *_currentBlock, 1);
		ConsoleUtils::sleep(_frameRate); // _frameRate
		_renderer->EraseBlock(*_mainBoard, *_currentBlock);
		ProcessInput();
		_currentBlock->Move(0, 1);
		_renderer->DrawBlock(*_mainBoard, *_currentBlock, 1);

		if (((MainBoard*)_mainBoard)->IsCollision(*_currentBlock)) // ���� ����ȯ ����
		{
			_currentBlock->Move(0, -1);

			// ���� ���� ���� ���� �̱��� (�� �� �����غ����� �� ����)
			
			((MainBoard*)_mainBoard)->MergeBlock(*_currentBlock);
			int hits = ((MainBoard*)_mainBoard)->ClearFullLines(); // �̰� ���� ���� ���� ����
			_clearLines -= hits;
			if (hits > 0)
			{
				if (_clearLines <= 0)
				{
					_renderer->DrawBoard(*_mainBoard);
					_clearLines = 0;
					ConsoleUtils::gotoxy(0, 0);
					std::cout << "STAGE: " << _stage << std::endl;
					std::cout << "LEVEL: " << _level << std::endl;
					std::cout << "GOALS: " << _clearLines << std::endl;
					std::cout << "Stage clear" << std::endl;
					ConsoleUtils::sleep(3000);
					ConsoleUtils::clearScreen();
					_stage++;
					// �޸� �� �����ϰ� �������� �̰� �ϴ� ���� ��� TODO
					delete _currentBlock;
					_currentBlock = nullptr;
					delete _nextBlock;
					_nextBlock = nullptr;
					return;
				}
				else
				{
					ConsoleUtils::gotoxy(0, 0);
					std::cout << "STAGE: " << _stage << std::endl;
					std::cout << "LEVEL: " << _level << std::endl;
					std::cout << "GOALS: " << _clearLines << std::endl;
				}
			}

			delete _currentBlock;
			_currentBlock = nullptr;
			if (_currentBlock == nullptr)
				_currentBlock = _nextBlock;
			_nextBlock = new Block(rand() % 7);

			_renderer->DrawBoard(*_mainBoard);
			_renderer->DrawBoard(*_nextBoard);

			_renderer->DrawBlock(*_nextBoard, *_nextBlock, 2);
		}
	}
}

void Game::Stage2()
{
	MapInit();

	PrintStageInfo();
	RenderBoardsAndNextBlock();
	auto start = std::chrono::steady_clock::now();
	bool up = false;
	while (_clearLines > 0) {
		int hits = PlayGameTurn();
		auto now = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
		ConsoleUtils::gotoxy(10, 1);
		std::cout << "���� �ð�: " << ((9999 - elapsed)%10+1) << "��      ";
		if (((9999 - elapsed) % 10 + 1) != 10) {
			up = true;
		}
		if (((9999 - elapsed) % 10 + 1)==10&&up==true) {
 			((MainBoard*)_mainBoard)->AddFullLines();
			_renderer->DrawBoard((*_mainBoard));
			up = false;
		}
		PrintStageInfo();
	}
	ClearStage();
}

void Game::Stage3()	// ���̵� ���� �߰���� clearline �� ����, 
{
	MapInit();

	PrintStageInfo();
	RenderBoardsAndNextBlock();

	bool midtermShown = false;
	int halfGoal = gameLevel[_level - 1]._clearLines / 2;	// clearlines�� ���� ���ϸ� �߰���� �ǽ�

	while (_clearLines > 0)
	{
		int hits = PlayGameTurn();

		if (!midtermShown && _clearLines <= halfGoal)
		{
			midtermShown = true;
			EraseStageInfo();
			_renderer->ShowMessage("�߰���� �ǽ�! 30�� ���� �� ���� ���켼��!", 10, 0);

			auto start = std::chrono::steady_clock::now();
			bool midtermSuccess = false;
			int linesBefore = _clearLines;

			while (true)
			{
				auto now = std::chrono::steady_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

				// �ǽð� ���� �ð� ���
				ConsoleUtils::gotoxy(10, 1);
				std::cout << "���� �ð�: " << (30 - elapsed) << "��      ";

				if (elapsed >= 30) break;

				PlayGameTurn();

				if (_clearLines < linesBefore) {
					midtermSuccess = true;
					break;
				}
			}

			if (midtermSuccess) {
				_renderer->ShowMessage("�߰���� ���! �������� �����մϴ�.              ", 10, 0);
				_clearLines += 1;
			}
			else {
				_renderer->ShowMessage("�߰���� ����! �������� �����մϴ�.              ", 10, 0);
				_clearLines += 1;
			}
			
			ConsoleUtils::sleep(3000);      
			_renderer->ShowMessage("                                               ", 10, 0);
			_renderer->ShowMessage("                ", 10, 1);
		}

		PrintStageInfo();
	}

	ClearStage();
}

void Game::Stage4() {
	_isReverseControl = true; //  ����Ű ���� Ȱ��ȭ

	MapInit();
	PrintStageInfo();
	RenderBoardsAndNextBlock();

	bool blackout60Started = false;// 60% Ŭ���� ���� ���� ���� ����
	bool blackout30Started = false;
	int line60 = gameLevel[_level - 1]._clearLines * 0.6;
	int line30 = gameLevel[_level - 1]._clearLines * 0.3;

	while (_clearLines > 0) {
		
		//  Ŭ���� ������ 60% ������ �� ù ���� ����
		if (!blackout60Started && _clearLines <= line60) {
			_renderer->ShowMessage("������ �����ɴϴ�...", 10, 0);
			_blackout.Start();
			blackout60Started = true;
		}

		//  Ŭ���� ������ 30% ������ �� �� ��° ���� ����
		if (!blackout30Started && _clearLines <= line30) {
			_renderer->ShowMessage("������ �����ɴϴ�...", 10, 0);
			_blackout.Start();
			blackout30Started = true;
		}

		_blackout.Update(); // ���� ���� ����

		//  ��� ����߸��� ���� (���� ���� ����)
		int hits = PlayGameTurn(&_blackout);

		PrintStageInfo();
	}

	_isReverseControl = false; // ���� ���� ����
	ClearStage();
}



void Game::Story1()
{
	std::cout << "Story1" << std::endl;
	int input;
	std::cout << "> ";
	std::cin >> input;
	ConsoleUtils::clearScreen();
}

void Game::Story2()
{
	std::cout << "Story2" << std::endl;
	int input;
	std::cout << "> ";
	std::cin >> input;
	ConsoleUtils::clearScreen();
}

void Game::Story3()
{
	std::cout << "Story3" << std::endl;
	int input;
	std::cout << "> ";
	std::cin >> input;
	ConsoleUtils::clearScreen();
}

void Game::Story4()
{
	std::cout << "Story4" << std::endl;
	int input;
	std::cout << "> ";
	std::cin >> input;
	ConsoleUtils::clearScreen();
}

void Game::Ending()
{
	std::cout << "Ending" << std::endl;
	int input;
	std::cout << "> ";
	std::cin >> input;
	ConsoleUtils::clearScreen();
}

void Game::ProcessInput()
{
	int key = _inputHandler->GetKeyPressed();
	//Ű ���� ���� 
	if (_isReverseControl) {
		if (key == Config::KEY_LEFT) key = Config::KEY_RIGHT;
		else if (key == Config::KEY_RIGHT) key = Config::KEY_LEFT;
		
	}

	switch (key) {
	case Config::KEY_LEFT:
		_currentBlock->Move(-1, 0);
		if (((MainBoard*)_mainBoard)->IsCollision(*_currentBlock))
			_currentBlock->Move(1, 0);
		break;
	case Config::KEY_RIGHT:
		_currentBlock->Move(1, 0);
		if (((MainBoard*)_mainBoard)->IsCollision(*_currentBlock))
			_currentBlock->Move(-1, 0);
		break;
	case Config::KEY_DOWN:
		_currentBlock->Move(0, 1);
		if (((MainBoard*)_mainBoard)->IsCollision(*_currentBlock))
			_currentBlock->Move(0, -1);
		break;
	case Config::KEY_UP:
		_currentBlock->Rotate();
		break;
	case Config::KEY_SPACE:
		while (true)
		{
			_currentBlock->Move(0, 1);
			if (((MainBoard*)_mainBoard)->IsCollision(*_currentBlock))
			{
				_currentBlock->Move(0, -1);
				return;
			}
		}
		break;
	case Config::KEY_ESC: // ���� ���� / �޴� �̵� ���� ����
		// �̱���
		break;
	}
}


int Game::PlayGameTurn(const BlackoutManager* blackout)
{
	_renderer->DrawBlock(*_mainBoard, *_currentBlock, 1);
	ConsoleUtils::sleep(_frameRate);
	_renderer->EraseBlock(*_mainBoard, *_currentBlock);

	ProcessInput();
	_currentBlock->Move(0, 1);
	_renderer->DrawBoard(*_mainBoard, _currentBlock, &_blackout);

	if (((MainBoard*)_mainBoard)->IsCollision(*_currentBlock))
	{
		_currentBlock->Move(0, -1);
		((MainBoard*)_mainBoard)->MergeBlock(*_currentBlock);
		int hits = ((MainBoard*)_mainBoard)->ClearFullLines();
		_clearLines -= hits;

		delete _currentBlock;
		_currentBlock = nullptr;

		_currentBlock = _nextBlock;
		//_nextBlock = new Block(rand() % 7);
		_nextBlock = new Block(0);

		_renderer->DrawBoard(*_mainBoard, _currentBlock, &_blackout);
		_renderer->DrawBoard(*_nextBoard);
		_renderer->DrawBlock(*_nextBoard, *_nextBlock, 2);

		return hits; // �̹� �Ͽ� �� �� �����ߴ���
	}

	return 0;
}

void Game::PrintStageInfo()
{
	ConsoleUtils::gotoxy(0, 5);
	std::cout << "STAGE: " << _stage << std::endl;
	std::cout << "LEVEL: " << _level << std::endl;
	std::cout << "GOALS: " << _clearLines << std::endl;
}

void Game::EraseStageInfo()
{
	ConsoleUtils::gotoxy(0, 5);
	std::cout << "           " << std::endl;
	std::cout << "           " << std::endl;
	std::cout << "           " << std::endl;
}


void Game::RenderBoardsAndNextBlock()
{
	_renderer->DrawBoard(*_mainBoard);
	_renderer->DrawBoard(*_nextBoard);
	_renderer->DrawBlock(*_nextBoard, *_nextBlock, 2);
}

void Game::ClearStage()
{
	std::cout << "Stage Clear!" << std::endl;
	ConsoleUtils::sleep(2000);
	ConsoleUtils::clearScreen();
	_stage++;

	delete _currentBlock;
	_currentBlock = nullptr;

	delete _nextBlock;
	_nextBlock = nullptr;
}