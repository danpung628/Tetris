#include "BlackoutManager.h"

void BlackoutManager::Start() {
    _active = true;
    _startTime = std::chrono::steady_clock::now();
}

void BlackoutManager::Update() {
    if (!_active) return;
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - _startTime).count();
    if (elapsed >= 20) {
        _active = false;
    }
}

bool BlackoutManager::IsActive() const {
    return _active;
}