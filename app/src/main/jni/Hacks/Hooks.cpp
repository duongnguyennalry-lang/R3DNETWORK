//
// ThrowIO Mod — Hooks Implementation
// OFFSET CỨNG CHÍNH XÁC TỪ IL2CPP DUMP
//

#include "Hooks.hpp"
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "dobby/dobby.h"
#include "KittyMemory/KittyMemory.h"
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <atomic>

// ================================================================
//  ⭐ OFFSET CỨNG TỪ DUMP — KHÔNG ĐỘI, KHÔNG SAI ⭐
// ================================================================
namespace Offsets {
    constexpr uintptr_t set_SoftMoney      = 0x1314CFC;
    constexpr uintptr_t set_HardMoney      = 0x1314D7C;
    constexpr uintptr_t set_Level          = 0x1314DFC;
    constexpr uintptr_t set_Exp            = 0x1314E68;
    constexpr uintptr_t set_NoAds          = 0x1314C0C;
    constexpr uintptr_t ApplyDamage        = 0x12FA55C;
    constexpr uintptr_t SetDeath           = 0x12FBFA4;
    constexpr uintptr_t CharWeapon_update  = 0x12FF868;
    constexpr uintptr_t SaveLocal          = 0x1311B10;
}

// ================================================================
//  POINTER VALIDATION
// ================================================================
bool Hooks::IsValidPtr(const void* ptr, size_t sz) noexcept {
    if (!ptr) return false;
    const uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    if (addr < 0x10000ULL || addr > 0x7FFFFFFFFFFFULL) return false;
    if (addr & 0x3) return false;

    const long ps = sysconf(_SC_PAGESIZE);
    const size_t pageSize = (ps > 0) ? static_cast<size_t>(ps) : 4096;

    const uintptr_t pageStart = addr & ~(pageSize - 1);
    const uintptr_t endAddr   = addr + sz - 1;
    const uintptr_t pageEnd   = endAddr & ~(pageSize - 1);
    const size_t    numPages  = ((pageEnd - pageStart) / pageSize) + 1;

    unsigned char* vec = static_cast<unsigned char*>(alloca(numPages > 128 ? numPages : 128));
    errno = 0;
    return mincore(reinterpret_cast<void*>(pageStart), numPages * pageSize, vec) == 0;
}

// ================================================================
//  HIGH-RES TIMESTAMP
// ================================================================
static inline uint64_t NowNs() noexcept {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000ULL
         + static_cast<uint64_t>(ts.tv_nsec);
}

// ================================================================
//  WATCHDOG — 500ms cooldown, không spam mỗi frame
// ================================================================
void Hooks::ApplyWatchdog() {
    static uint64_t lastRun = 0;
    const uint64_t now = NowNs();
    if (now - lastRun < 500000000ULL) return;
    lastRun = now;

    void* inst = Vars::PlayerBalance::instance.load(std::memory_order_acquire);
    if (!inst || !IsValidPtr(inst)) return;

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
}

// ================================================================
//  HOOK HANDLERS
// ================================================================
void Hooks::capture_set_SoftMoney(void* instance, long value) {
    if (instance && IsValidPtr(instance)) {
        void* expected = nullptr;
        Vars::PlayerBalance::instance.compare_exchange_strong(
            expected, instance, std::memory_order_release);
    }
    if (Vars::FnPtrs::orig_set_SoftMoney)
        Vars::FnPtrs::orig_set_SoftMoney(instance, value);
}

void Hooks::hook_ApplyDamage(void* inst, long dmg, void* from,
                             bool crit, void* extra) {
    if (Vars::Combat::godMode.load() && inst && IsValidPtr(inst)) return;
    if (Vars::FnPtrs::old_ApplyDamage)
        Vars::FnPtrs::old_ApplyDamage(inst, dmg, from, crit, extra);
}

void Hooks::hook_SetDeath(void* inst, bool isDead) {
    if (Vars::Combat::godMode.load() && inst && IsValidPtr(inst)) isDead = false;
    if (Vars::FnPtrs::old_SetDeath)
        Vars::FnPtrs::old_SetDeath(inst, isDead);
}

void Hooks::hook_CharWeapon_update(void* inst, float dt) {
    if (Vars::Combat::speedHack.load() && inst)
        dt *= Vars::Combat::speedMultiplier.load(std::memory_order_relaxed);

    void* balInst = Vars::PlayerBalance::instance.load(std::memory_order_acquire);
    if (balInst && IsValidPtr(balInst)) {
        if (Vars::PlayerBalance::forceUpdateMoney.exchange(false)) {
            if (Vars::FnPtrs::set_SoftMoney)
                Vars::FnPtrs::set_SoftMoney(balInst, 0x7FFFFFFF);
            if (Vars::FnPtrs::set_HardMoney)
                Vars::FnPtrs::set_HardMoney(balInst, 0x7FFFFFFF);
        }
        if (Vars::PlayerBalance::forceMaxLevel.exchange(false)) {
            if (Vars::FnPtrs::set_Level)
                Vars::FnPtrs::set_Level(balInst, 99);
            if (Vars::FnPtrs::set_Exp)
                Vars::FnPtrs::set_Exp  (balInst, 0x7FFFFFFF);
        }
    }

    if (Vars::Security::antiCheat.load()) ApplyWatchdog();
    if (Vars::FnPtrs::old_CharWeapon_update)
        Vars::FnPtrs::old_CharWeapon_update(inst, dt);
}

void Hooks::hook_SaveLocal(void* inst) {
    if (Vars::Security::antiCheat.load()) ApplyWatchdog();
    if (Vars::FnPtrs::old_SaveLocal)
        Vars::FnPtrs::old_SaveLocal(inst);
    if (Vars::Security::antiCheat.load()) ApplyWatchdog();
}

// ================================================================
//  INIT HOOKS — DÙNG OFFSET CỨNG
// ================================================================
void Hooks::InitHooks() {
    LOGI(OBFUSCATE("ThrowIO: InitHooks start — hardcoded offsets"));

    // Lấy base address của libil2cpp.so
    auto il2cpp = KittyMemory::getLibraryMap(OBFUSCATE("libil2cpp.so"));
    if (!il2cpp.isValid()) {
        LOGE(OBFUSCATE("ThrowIO: FATAL — libil2cpp.so not found"));
        return;
    }
    
    // Đã cập nhật dòng này: đổi startAddress thành start
    const uintptr_t base = il2cpp.start; 
    
    LOGI(OBFUSCATE("ThrowIO: il2cpp base = 0x%lx"), base);

    // Bind function pointers: base + offset = địa chỉ tuyệt đối
    Vars::FnPtrs::set_SoftMoney =
        (void(*)(void*, long))(base + Offsets::set_SoftMoney);
    Vars::FnPtrs::set_HardMoney =
        (void(*)(void*, long))(base + Offsets::set_HardMoney);
    Vars::FnPtrs::set_Level =
        (void(*)(void*, int)) (base + Offsets::set_Level);
    Vars::FnPtrs::set_Exp =
        (void(*)(void*, int)) (base + Offsets::set_Exp);
    Vars::FnPtrs::set_NoAds =
        (void(*)(void*, bool))(base + Offsets::set_NoAds);

    LOGI(OBFUSCATE("ThrowIO: FnPtrs bound — set_SoftMoney=%p"),
         reinterpret_cast<void*>(Vars::FnPtrs::set_SoftMoney));

    // Release fence — đảm bảo pointer writes visible trước khi hook
    std::atomic_thread_fence(std::memory_order_release);

    // Cài Dobby hooks — 5 cái cho ThrowIO mod
    int installed = 0;

    DobbyHook((void*)(base + Offsets::set_SoftMoney),
              (void*)capture_set_SoftMoney,
              (void**)&Vars::FnPtrs::orig_set_SoftMoney);
    installed++;

    DobbyHook((void*)(base + Offsets::ApplyDamage),
              (void*)hook_ApplyDamage,
              (void**)&Vars::FnPtrs::old_ApplyDamage);
    installed++;

    DobbyHook((void*)(base + Offsets::SetDeath),
              (void*)hook_SetDeath,
              (void**)&Vars::FnPtrs::old_SetDeath);
    installed++;

    DobbyHook((void*)(base + Offsets::CharWeapon_update),
              (void*)hook_CharWeapon_update,
              (void**)&Vars::FnPtrs::old_CharWeapon_update);
    installed++;

    DobbyHook((void*)(base + Offsets::SaveLocal),
              (void*)hook_SaveLocal,
              (void**)&Vars::FnPtrs::old_SaveLocal);
    installed++;

    LOGI(OBFUSCATE("ThrowIO: %d hooks installed — fuck yeah"), installed);
}

// ================================================================
//  Mẫu PlayerUpdate từ R3DNETWORK gốc (giữ lại cho khớp .hpp)
//  Không dùng đến nhưng phải có để không báo lỗi link
// ================================================================
void (*Hooks::orig_PlayerUpdate)(void *pInstance) = nullptr;

void Hooks::PlayerUpdate(void *pInstance) {
    if (orig_PlayerUpdate) orig_PlayerUpdate(pInstance);
}
