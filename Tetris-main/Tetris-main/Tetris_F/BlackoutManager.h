#pragma once
#include <chrono>

class BlackoutManager {
private:
    bool _active = false;
    std::chrono::steady_clock::time_point _startTime;

public:
    void Start();          // ���� ����
    void Update();         // ���� ����
    bool IsActive() const; // ���� ����������
};