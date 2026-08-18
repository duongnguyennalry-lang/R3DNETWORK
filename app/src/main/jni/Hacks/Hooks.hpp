//
// Created by rosetta on 05/07/2024.
// Modified by AXIOM for ThrowIO Mod
//

#ifndef HOOKS_H
#define HOOKS_H

#include "Vars.h"
#include "UnityResolve/UnityResolve.hpp"
#include "Includes/ObscuredTypes.hpp"
#include "Includes/obfuscate.h"
#include <cstddef>

class Hooks {
public:
    static void InitHooks();

    // ── Mẫu gốc R3DNETWORK (giữ lại nếu cần) ────────────────
    static void (*orig_PlayerUpdate)(void *pInstance);
    static void PlayerUpdate(void *pInstance);

    // ── THROWIO MOD HOOKS ──────────────────────────────────
    // Capture instance + hooks
    static void capture_set_SoftMoney(void* instance, long value);
    static void hook_ApplyDamage     (void* inst, long dmg, void* from, bool crit, void* extra);
    static void hook_SetDeath        (void* inst, bool isDead);
    static void hook_CharWeapon_update(void* inst, float dt);
    static void hook_SaveLocal       (void* inst);

    // Utilities
    static void ApplyWatchdog();
    static bool IsValidPtr(const void* ptr, size_t sz = 1) noexcept;
};

#endif // HOOKS_H
