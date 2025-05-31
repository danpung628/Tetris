#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <cstdlib>

enum class GameColor
{
    BLACK, DARK_BLUE, DARK_GREEN, DARK_SKY_BLUE, DARK_RED, DARK_VOILET,
    DARK_YELLOW, GRAY, DARK_GRAY, BLUE, GREEN, SKY_BLUE, RED, VOILET,
    YELLOW, WHITE
};

namespace ConsoleUtils
{
    void gotoxy(int x = 24, int y = 24);
    void setColor(GameColor foregroundColor, GameColor backgroundColor = GameColor::BLACK); // 배경색 기본값 지정
    void hideCursor();
    void showCursor();
    void clearScreen();
    void sleep(int milliseconds);
};
