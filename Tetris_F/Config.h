#pragma once
//#include "Point.h"

namespace Config
{
    const int EXT_KEY = 0xE0;     // 확장 키 prefix
    const int KEY_UP = 0x48;      // 위 방향키
    const int KEY_LEFT = 0x4B;    // 왼쪽 방향키
    const int KEY_RIGHT = 0x4D;   // 오른쪽 방향키
    const int KEY_DOWN = 0x50;    // 아래 방향키
    const int KEY_SPACE = 0x20;   // 스페이스바
    const int KEY_ESC = 0x1B;     // ESC 키 (예: 종료/일시정지)

    const int NUM_SHAPES = 7;         // 총 블록 모양 개수
    const int NUM_ROTATIONS = 4;      // 각 모양의 최대 회전 수
    const int BLOCK_GRID_SIZE = 4;    // 각 블록을 표현하는 그리드 크기 (4x4)

    extern const char TETROMINO_SHAPES[NUM_SHAPES][NUM_ROTATIONS][BLOCK_GRID_SIZE][BLOCK_GRID_SIZE];
}