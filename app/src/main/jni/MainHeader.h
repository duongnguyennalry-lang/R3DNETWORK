// ================================================================
//  MainHeader.h — AXIOM DEVELOPMENT
//  app/src/main/jni/MainHeader.h
//  ThrowIO Mod — Hardcoded Offset Edition
// ================================================================
#pragma once

// ── System headers ──────────────────────────────────────────────
#include <jni.h>
#include <android/log.h>
#include <unistd.h>
#include <dlfcn.h>
#include <pthread.h>
#include <sys/mman.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>           // clock_gettime cho ApplyWatchdog

// ── STL ─────────────────────────────────────────────────────────
#include <string>
#include <atomic>
#include <cstdint>
#include <algorithm>       // std::clamp, std::swap

// ── Dobby — Hook framework (BẮT BUỘC) ──────────────────────────
#include "dobby/dobby.h"

// ── KittyMemory — thao tác bộ nhớ (nếu cần) ────────────────────
#include "KittyMemory/KittyMemory.hpp"

// ── Project headers (BẮT BUỘC) ─────────────────────────────────
#include "Hacks/Vars.h"
#include "Hacks/Hooks.hpp"

// ── Log Macros ──────────────────────────────────────────────────
#define LOG_TAG "ThrowIO_Axiom"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

// ── Dobby Helper macro ─────────────────────────────────────────
// Dùng: DHK(target_addr, hook_func, orig_func_ptr)
#define DHK(target, hook, orig) \
    DobbyHook((void*)(target), (void*)(hook), (void**)&(orig))

// ── AddPointer helper ──────────────────────────────────────────
// Gán địa chỉ vào con trỏ hàm một cách an toàn
template<typename T>
static inline void AddPointer(T& fnPtr, uintptr_t offset) {
    fnPtr = reinterpret_cast<T>(offset);
}

// ── Valid pointer checker ──────────────────────────────────────
static inline bool IsValidPtr(const void* ptr, size_t sz = 1) noexcept {
    if (!ptr) return false;
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    if (addr < 0x10000 || addr > 0x7FFFFFFFFFFFULL) return false;
    if ((addr & 3) != 0) return false; // 4-byte alignment
    return true;
}

// ── GL globals (dùng cho kích thước màn hình nếu cần) ──────────
inline int glWidth  = 0;
inline int glHeight = 0;

// ── Globals — khai báo extern (định nghĩa trong Main.cpp) ───────
extern uintptr_t    address;          // base address của libil2cpp.so
extern const char*  targetLibName;    // "libil2cpp.so"

// ── Lib helper functions ───────────────────────────────────────
extern bool        isLibraryLoaded(const char* libName);
extern uintptr_t   findLibrary(const char* libName);

// ── Menu screen function pointers ──────────────────────────────
namespace Menu {
    inline int (*Screen_get_height)() = nullptr;
    inline int (*Screen_get_width)()  = nullptr;
}
