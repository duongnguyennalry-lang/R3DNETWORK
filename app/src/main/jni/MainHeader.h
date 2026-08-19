// ================================================================
//  MainHeader.h — AXIOM DEVELOPMENT
//  app/src/main/jni/MainHeader.h
// ================================================================
#pragma once

// ── System ──────────────────────────────────────────────────────
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
#include <ctime>

// ── STL ─────────────────────────────────────────────────────────
#include <string>
#include <atomic>
#include <cstdint>
#include <algorithm>

// ── EGL / GL ────────────────────────────────────────────────────
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

// ── Dobby ───────────────────────────────────────────────────────
#include "dobby/dobby.h"

// ── KittyMemory ─────────────────────────────────────────────────
#include "KittyMemory/KittyMemory.hpp"

// ── BNM — BẬT LẠI, Main.cpp cần ────────────────────────────────
#include "ByNameModding/BNM.hpp"

// ── Project headers ─────────────────────────────────────────────
#include "Hacks/Vars.h"
#include "Hacks/Hooks.hpp"

// ── Log Macros ──────────────────────────────────────────────────
#define LOG_TAG "ThrowIO_Axiom"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

// ── Dobby Helper ────────────────────────────────────────────────
#define DHK(target, hook, orig) \
    DobbyHook((void*)(target), (void*)(hook), (void**)&(orig))

// ── BNM Macros — define thẳng, không ifdef ──────────────────────
#define getClass(name, ns)    BNM::LoadClass(name, ns)
#define getOffset(cls, name)  (uintptr_t)cls.GetMethod(name).GetOffset()
#define AttachIl2Cpp()        BNM::Loading::TryLoadByJNI()
#define DetachIl2Cpp()        BNM::UnloadBNM()
#define OBFBNM(ns, cls, m, a) \
    BNM::LoadClass(cls, ns).GetMethod(m, a).GetOffset()

// ── AddPointer ──────────────────────────────────────────────────
template<typename T>
static inline void AddPointer(T& fnPtr, uintptr_t offset) {
    fnPtr = reinterpret_cast<T>(offset);
}

// ── GL Globals ──────────────────────────────────────────────────
inline int glWidth  = 0;
inline int glHeight = 0;

// ── Extern globals ──────────────────────────────────────────────
extern uintptr_t    address;
extern const char*  targetLibName;

extern jclass    UnityPlayer_cls;
extern jfieldID  UnityPlayer_CurrentActivity_fid;
extern void*   (*old_RegisterNatives)(JNIEnv*, jclass,
                    const JNINativeMethod*, jint);
extern void      hook_RegisterNatives(JNIEnv*, jclass,
                    const JNINativeMethod*, jint);

extern bool        isLibraryLoaded(const char* libName);
extern uintptr_t   findLibrary(const char* libName);

// ── Menu ────────────────────────────────────────────────────────
namespace Menu {
    inline int (*Screen_get_height)() = nullptr;
    inline int (*Screen_get_width)()  = nullptr;
}
