//
// ThrowIO Mod — Hook Implementations
// Chỉ include Vars.h, KHÔNG khai báo lại gì từ Vars
//
#include "Vars.h"
#include "MainHeader.h"

#include <sys/mman.h>
#include <unistd.h>

namespace Hooks {

    // ── Legacy — giữ để link không lỗi ───────────────────────────
    void (*orig_PlayerUpdate)(void* pInstance) = nullptr;

    // ================================================================
    //  UTILITY: pointer sanity check
    //  FIX: sz có default value → gọi 1 hoặc 2 argument đều được
    // ================================================================
    bool IsValidPtr(const void* ptr, size_t sz = sizeof(void*)) noexcept {
        if (!ptr) return false;
        auto addr = reinterpret_cast<uintptr_t>(ptr);
        return addr > 0x1000UL && addr < 0x0001000000000000UL;
    }

    // ================================================================
    //  WATCHDOG — áp dụng tất cả toggle đang bật
    //  Gọi mỗi frame + mỗi SaveLocal
    // ================================================================
    void ApplyWatchdog() {
        void* inst = Vars::PlayerBalance::instance.load(std::memory_order_acquire);
        if (!IsValidPtr(inst)) return;

        if (Vars::PlayerBalance::infiniteMoney.load() && Vars::FnPtrs::set_SoftMoney)
            Vars::FnPtrs::set_SoftMoney(inst, 0x7FFFFFFF);

        if (Vars::PlayerBalance::infinitePremium.load() && Vars::FnPtrs::set_HardMoney)
            Vars::FnPtrs::set_HardMoney(inst, 0x7FFFFFFF);

        if (Vars::PlayerBalance::maxLevel.load()) {
            if (Vars::FnPtrs::set_Level) Vars::FnPtrs::set_Level(inst, 99);
            if (Vars::FnPtrs::set_Exp)   Vars::FnPtrs::set_Exp  (inst, 0x7FFFFFFF);
        }

        if (Vars::PlayerBalance::noAds.load() && Vars::FnPtrs::set_NoAds)
            Vars::FnPtrs::set_NoAds(inst, true);

        // One-shot force buttons từ menu
        if (Vars::PlayerBalance::forceUpdateMoney.exchange(false)) {
            if (Vars::FnPtrs::set_SoftMoney) Vars::FnPtrs::set_SoftMoney(inst, 0x7FFFFFFF);
            if (Vars::FnPtrs::set_HardMoney) Vars::FnPtrs::set_HardMoney(inst, 0x7FFFFFFF);
        }

        if (Vars::PlayerBalance::forceMaxLevel.exchange(false)) {
            if (Vars::FnPtrs::set_Level) Vars::FnPtrs::set_Level(inst, 99);
            if (Vars::FnPtrs::set_Exp)   Vars::FnPtrs::set_Exp  (inst, 0x7FFFFFFF);
        }
    }

    // ================================================================
    //  HOOK: capture_set_SoftMoney
    //  Bắt instance lần đầu tiên PlayerBalance gọi set_SoftMoney
    // ================================================================
    void capture_set_SoftMoney(void* instance, long value) {
        if (IsValidPtr(instance)) {
            void* expected = nullptr;
            Vars::PlayerBalance::instance.compare_exchange_strong(
                expected, instance,
                std::memory_order_release,
                std::memory_order_relaxed
            );
        }
        if (Vars::FnPtrs::orig_set_SoftMoney)
            Vars::FnPtrs::orig_set_SoftMoney(instance, value);
    }

    // ================================================================
    //  HOOK: ApplyDamage — God Mode
    // ================================================================
    void hook_ApplyDamage(void* inst, long dmg, void* from, bool crit, void* extra) {
        if (Vars::Combat::godMode.load() && IsValidPtr(inst)) return;
        if (Vars::FnPtrs::old_ApplyDamage)
            Vars::FnPtrs::old_ApplyDamage(inst, dmg, from, crit, extra);
    }

    // ================================================================
    //  HOOK: SetDeath — Không cho chết
    // ================================================================
    void hook_SetDeath(void* inst, bool isDead) {
        if (Vars::Combat::godMode.load() && IsValidPtr(inst))
            isDead = false;
        if (Vars::FnPtrs::old_SetDeath)
            Vars::FnPtrs::old_SetDeath(inst, isDead);
    }

    // ================================================================
    //  HOOK: CharWeapon::update — Speed Hack
    // ================================================================
    void hook_CharWeapon_update(void* inst, float dt) {
        if (Vars::Combat::speedHack.load() && IsValidPtr(inst))
            dt *= Vars::Combat::speedMultiplier.load(std::memory_order_relaxed);
        if (Vars::FnPtrs::old_CharWeapon_update)
            Vars::FnPtrs::old_CharWeapon_update(inst, dt);
    }

    // ================================================================
    //  HOOK: SaveLocal — Anti-Cheat sandwich
    // ================================================================
    void hook_SaveLocal(void* inst) {
        if (Vars::Security::antiCheat.load()) ApplyWatchdog();
        if (Vars::FnPtrs::old_SaveLocal)      Vars::FnPtrs::old_SaveLocal(inst);
        if (Vars::Security::antiCheat.load()) ApplyWatchdog();
    }

    // ================================================================
    //  LEGACY: PlayerUpdate
    // ================================================================
    void PlayerUpdate(void* pInstance) {
        if (orig_PlayerUpdate) orig_PlayerUpdate(pInstance);
    }

    // ================================================================
    //  InitHooks — placeholder, hooks thực gắn trong hack_thread
    // ================================================================
    void InitHooks() {
        LOGI(OBFUSCATE("ThrowIO: Hooks namespace ready"));
    }

} // namespace Hooks
