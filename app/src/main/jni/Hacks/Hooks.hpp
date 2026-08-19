#pragma once
//
// Created by github.com/seedhollow on 17/10/25.
// FIX: thêm inline vào cả 2 function body → không bị duplicate symbol
//

#include "Visuals.hpp"

// ── Khai báo namespace Hooks ──────────────────────────────────────────────────
namespace Hooks {
    void InitHooks();
    void ApplyWatchdog();
}

// ── Implementations của Visuals ───────────────────────────────────────────────
// FIX: PHẢI có inline khi define function body trong .hpp
// Không có inline → mỗi .cpp include file này compile 1 bản riêng → linker duplicate

inline void Visuals::Update(Draw draw, int screenWidth, int screenHeight) {
    if (Vars::PlayerData::ESPCrosshair) {
        DrawEspCrosshair(draw);
    }
}

inline void Visuals::DrawEspCrosshair(Draw draw) {
    Unity::Color crosshair_color {0, 0, 0, 255};

    if (Vars::PlayerData::CrosshairColor == 0) {
        crosshair_color = Unity::Color(255, 0, 0, 255);
    } else if (Vars::PlayerData::CrosshairColor == 1) {
        crosshair_color = Unity::Color(0, 255, 0, 255);
    } else {
        crosshair_color = Unity::Color(0, 0, 255, 255);
    }

    draw.DrawCrosshair(
        crosshair_color,
        Unity::Vector2(draw.getWidth() / 2.0f, draw.getHeight() / 2.0f),
        Vars::PlayerData::CrosshairSize * 10
    );
}
