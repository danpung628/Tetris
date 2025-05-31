#pragma once
#include <chrono>

class BlackoutManager {
private:
    bool _active = false;
    std::chrono::steady_clock::time_point _startTime;

public:
    void Start();          // 암전 시작
    void Update();         // 상태 갱신
    bool IsActive() const; // 현재 암전중인지
};