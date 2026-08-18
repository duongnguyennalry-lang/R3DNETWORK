//
// ThrowIO Mod — Variables Storage
// Menu thread ghi vào đây, hook thread đọc từ đây
//

#pragma once

#include <atomic>
#include <cstdint>

namespace Vars {

    // ── TIỀN TỆ + TIẾN TRÌNH ──────────────────────────────────
    struct PlayerBalance_t {
        inline static std::atomic<void*>  instance{nullptr};
        inline static std::atomic<bool>   infiniteMoney{false};
        inline static std::atomic<bool>   infinitePremium{false};
        inline static std::atomic<bool>   maxLevel{false};
        inline static std::atomic<bool>   noAds{false};
        inline static std::atomic<bool>   forceUpdateMoney{false};
        inline static std::atomic<bool>   forceMaxLevel{false};
    };

    // ── CHIẾN ĐẤU ──────────────────────────────────────────────
    struct Combat_t {
        inline static std::atomic<bool>   godMode{false};
        inline static std::atomic<bool>   speedHack{false};
        inline static std::atomic<float>  speedMultiplier{2.0f};
    };

    // ── BẢO MẬT ────────────────────────────────────────────────
    struct Security_t {
        inline static std::atomic<bool>   antiCheat{true};
    };

    // ── ESP (giữ lại từ gốc nếu sau này cần) ───────────────────
    struct PlayerData_t {
        inline static std::atomic<bool>   godMode{false};
        inline static std::atomic<bool>   ESP{false};
        inline static std::atomic<bool>   ESPCrosshair{false};
        inline static std::atomic<int>    CrosshairSize{1};
        inline static std::atomic<int>    CrosshairColor{0};
    };

    // ── FUNCTION POINTERS ──────────────────────────────────────
    struct FnPtrs_t {
        inline static void (*set_SoftMoney)(void*, long) = nullptr;
        inline static void (*set_HardMoney)(void*, long) = nullptr;
        inline static void (*set_Level)    (void*, int)  = nullptr;
        inline static void (*set_Exp)      (void*, int)  = nullptr;
        inline static void (*set_NoAds)    (void*, bool) = nullptr;

        inline static void (*old_ApplyDamage)      (void*, long, void*, bool, void*) = nullptr;
        inline static void (*old_SetDeath)         (void*, bool)  = nullptr;
        inline static void (*old_CharWeapon_update)(void*, float) = nullptr;
        inline static void (*old_SaveLocal)        (void*)        = nullptr;
        inline static void (*orig_set_SoftMoney)   (void*, long)  = nullptr;
    };

    // ── ALIAS NGẮN GỌN ────────────────────────────────────────
    using PlayerBalance = PlayerBalance_t;
    using Combat        = Combat_t;
    using Security      = Security_t;
    using PlayerData    = PlayerData_t;
    using FnPtrs        = FnPtrs_t;

} // namespace Vars
