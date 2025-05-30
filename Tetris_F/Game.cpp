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
			GameInit(); // -여기서 다 초기화
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
	std::cout << "1. 게임 시작" << std::endl;
	std::cout << "2. 게임 종료" << std::endl;
	std::cin >> *input;
	ConsoleUtils::clearScreen();
}

void Game::ShowLevelSelect(int* input)
{
	std::cout << "1. 1학년" << std::endl;
	std::cout << "2. 2학년" << std::endl;
	std::cout << "3. 3학년" << std::endl;
	std::cin >> *input;
	ConsoleUtils::clearScreen();
}

void Game::GameInit()
{
	_clearLines = gameLevel[_level - 1]._clearLines;
	_frameRate = gameLevel[_level - 1]._frameRate;
	_stickRate = gameLevel[_level - 1]._stickRate;
	_stage = 3; // 일단 stage 3 부터 시작하게 변경

	// 블록 관련
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

	while (true)	// 이부분도 함수로 빼면 좋지 않을까
	{
		_renderer->DrawBlock(*_mainBoard, *_currentBlock, 1);
		ConsoleUtils::sleep(_frameRate); // _frameRate
		_renderer->EraseBlock(*_mainBoard, *_currentBlock);
		ProcessInput();
		_currentBlock->Move(0, 1);
		_renderer->DrawBlock(*_mainBoard, *_currentBlock, 1);

		if (((MainBoard*)_mainBoard)->IsCollision(*_currentBlock)) // 강제 형변환 문제
		{
			_currentBlock->Move(0, -1);

			// 게임 오버 로직 아직 미구현 (좀 더 생각해봐야할 것 같음)
			
			((MainBoard*)_mainBoard)->MergeBlock(*_currentBlock);
			int hits = ((MainBoard*)_mainBoard)->ClearFullLines(); // 이거 내부 로직 조금 엉망
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
					// 메모리 다 해제하고 나가야함 이거 일단 조금 고민 TODO
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
	std::cout << "Stage2" << std::endl;
	int input;
	std::cout << "> ";
	std::cin >> input;
	_stage++;
	ConsoleUtils::clearScreen();
}

void Game::Stage3()	// 난이도 별로 중간고사 clearline 수 변경, 
{
	MapInit();

	PrintStageInfo();
	RenderBoardsAndNextBlock();

	bool midtermShown = false;
	int halfGoal = gameLevel[_level - 1]._clearLines / 2;	// clearlines의 절반 이하면 중간고사 실시

	while (_clearLines > 0)
	{
		int hits = PlayGameTurn();

		if (!midtermShown && _clearLines <= halfGoal)
		{
			midtermShown = true;
			EraseStageInfo();
			_renderer->ShowMessage("중간고사 실시! 30초 동안 한 줄을 지우세요!", 10, 0);

			auto start = std::chrono::steady_clock::now();
			bool midtermSuccess = false;
			int linesBefore = _clearLines;

			while (true)
			{
				auto now = std::chrono::steady_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

				// 실시간 남은 시간 출력
				ConsoleUtils::gotoxy(10, 1);
				std::cout << "남은 시간: " << (30 - elapsed) << "초      ";

				if (elapsed >= 30) break;

				PlayGameTurn();

				if (_clearLines < linesBefore) {
					midtermSuccess = true;
					break;
				}
			}

			if (midtermSuccess) {
				_renderer->ShowMessage("중간고사 통과! 수업으로 복귀합니다.              ", 10, 0);
				_clearLines += 1;
			}
			else {
				_renderer->ShowMessage("중간고사 실패! 수업으로 복귀합니다.              ", 10, 0);
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


void Game::Stage4()
{
	std::cout << "Stage4" << std::endl;
	int input;
	std::cout << "> ";
	std::cin >> input;
	_stage++;
	ConsoleUtils::clearScreen();
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
	case Config::KEY_ESC: // 게임 퍼즈 / 메뉴 이동 구현 예정
		// 미구현
		break;
	}
}


int Game::PlayGameTurn()
{
	_renderer->DrawBlock(*_mainBoard, *_currentBlock, 1);
	ConsoleUtils::sleep(_frameRate);
	_renderer->EraseBlock(*_mainBoard, *_currentBlock);

	ProcessInput();
	_currentBlock->Move(0, 1);
	_renderer->DrawBlock(*_mainBoard, *_currentBlock, 1);

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

		_renderer->DrawBoard(*_mainBoard);
		_renderer->DrawBoard(*_nextBoard);
		_renderer->DrawBlock(*_nextBoard, *_nextBlock, 2);

		return hits; // 이번 턴에 몇 줄 제거했는지
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