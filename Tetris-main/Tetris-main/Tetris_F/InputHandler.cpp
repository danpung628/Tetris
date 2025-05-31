#include "InputHandler.h"
#include "Config.h"
#include <conio.h> // _getch()
#include <iostream>

int InputHandler::GetKeyPressed() const
{
    if (_kbhit())
    {
        int key = _getch();

        if (key == Config::EXT_KEY)
        {
            key = _getch();

            while (_kbhit()) {
                _getch();
            }
        }

        std::cout << "눌린 키 코드: " << key << std::endl;
        return key;

    }
    return 0; // 키 입력 없음
}
