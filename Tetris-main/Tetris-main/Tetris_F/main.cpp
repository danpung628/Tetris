#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif // !DBG_NEW
#endif
//
#include "Game.h"


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Game game;
	game.Run();

	return 0;
}

// 지금 할 일
// - 메모리 누수 체크
// - 맵 / 블록 다음 스테이지 초기화 로직 구성
// - 프레임 빠르게 (키 입력)
// - 게임 오버 로직 구현
// - 스토리 구상
// - 스토리 UI 구상
// - 게임 플레이 스토리 UI 구상 (게임 스탯창/빈 공간 등)
// - 블록 스페이스 누르면 완전 밑에까지 가버리는 거
// - 블록 라인 클리어 이펙트 추가