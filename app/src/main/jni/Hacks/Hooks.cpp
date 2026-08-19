#pragma once

// ================================================================
//  [FIX] Đổi "../MainHeader.h" → "MainHeader.h"
//  "../" là relative path cứng — compiler không dùng -I flags
//  "MainHeader.h" → compiler search qua LOCAL_C_INCLUDES → tìm được
// ================================================================
#include "MainHeader.h"

#include <atomic>
#include <sys/mman.h>
#include <unistd.h>

// ================================================================
//  VARS NAMESPACE
// ================================================================
namespace Vars {

    namespace PlayerBalance {
        inline std::atomic<void*> instance        { nullptr };
        inline std::atomic<bool>  infiniteMoney   { false   };
        inline std::atomic<bool>  infinitePremium { false   };
        inline std::atomic<bool>  maxLevel        { false   };
        inline std::atomic<bool>  noAds           { false   };
        inline std::atomic<bool>  forceUpdateMoney{ false   };
        inline std::atomic<bool>  forceMaxLevel   { false   };
    }

    namespace FnPtrs {
        // ── Setters ──────────────────────────────────────────────
        inline void (*set_SoftMoney)(void* instance, long  value) = nullptr;
        inline void (*set_HardMoney)(void* instance, long  value) = nullptr;
        inline void (*set_Level)    (void* instance, int   value) = nullptr;
        inline void (*set_Exp)      (void* instance, int   value) = nullptr;
        inline void (*set_NoAds)    (void* instance, bool  value) = nullptr;

        // ── Originals (Dobby trampoline) ─────────────────────────
        inline void (*orig_set_SoftMoney)  (void* instance, long  value)         = nullptr;
        inline void (*old_ApplyDamage)     (void* instance, long  damage,
                                            void* from, bool isCritical,
                                            void* extra)                          = nullptr;
        inline void (*old_SetDeath)        (void* instance, bool  isDead)        = nullptr;
        inline void (*old_CharWeapon_update)(void* instance, float deltaTime)    = nullptr;
        inline void (*old_SaveLocal)       (void* instance)                      = nullptr;
    }

    namespace Combat {
        inline std::atomic<bool>  godMode        { false  };
        inline std::atomic<bool>  speedHack      { false  };
        inline std::atomic<float> speedMultiplier{ 2.0f   };
    }

    namespace Security {
        inline std::atomic<bool> antiCheat { true };
    }

} // namespace Vars

// ================================================================
//  HOOKS NAMESPACE
// ================================================================
namespace Hooks {

    // ── Legacy hook — giữ để link không lỗi ─────────────────────
    extern void (*orig_PlayerUpdate)(void* pInstance);
    void PlayerUpdate(void* pInstance);

    // ── Utility ──────────────────────────────────────────────────
    bool IsValidPtr(const void* ptr, size_t sz = sizeof(void*)) noexcept;

    // ── Watchdog ─────────────────────────────────────────────────
    void ApplyWatchdog();

    // ── Hook handlers ─────────────────────────────────────────────
    void capture_set_SoftMoney  (void* instance, long  value);
    void hook_ApplyDamage       (void* inst,     long  dmg,
                                 void* from,     bool  crit,
                                 void* extra);
    void hook_SetDeath          (void* inst, bool  isDead);
    void hook_CharWeapon_update (void* inst, float dt);
    void hook_SaveLocal         (void* inst);

    // ── Init ─────────────────────────────────────────────────────
    void InitHooks();

} // namespace Hooks
