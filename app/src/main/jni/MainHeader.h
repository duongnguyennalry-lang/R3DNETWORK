// ================================================================
//  MainHeader.h — AXIOM DEVELOPMENT
//  app/src/main/jni/MainHeader.h
//  Central include cho toàn bộ ThrowIO mod
// ================================================================
#pragma once

// ── System ──────────────────────────────────────────────────────
#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <unistd.h>
#include <dlfcn.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

// ── STL ─────────────────────────────────────────────────────────
#include <string>
#include <atomic>
#include <cstdint>
#include <algorithm>
#include <functional>

// ── EGL / GL ────────────────────────────────────────────────────
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

// ── ImGui ───────────────────────────────────────────────────────
#include "Includes/imgui/imgui.h"
#include "Includes/imgui/imgui_internal.h"
#include "Includes/imgui/backends/imgui_impl_android.h"
#include "Includes/imgui/backends/imgui_impl_opengl3.h"

// ── BNM (ByNameModding) ─────────────────────────────────────────
#include "ByNameModding/BNM.hpp"

// ── Dobby Hook ──────────────────────────────────────────────────
#include "dobby/dobby.h"

// ── KittyMemory ─────────────────────────────────────────────────
#include "KittyMemory/KittyMemory.hpp"

// ── Menu ────────────────────────────────────────────────────────
#include "Menu/JNILoader.h"
#include "Menu/FeatureModule.h"

// ── Includes ────────────────────────────────────────────────────
#include "Includes/Utils.h"
#include "Includes/Obfuscate.h"

// ── Log Macros ──────────────────────────────────────────────────
#define LOG_TAG "ThrowIO_Axiom"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

// ── BNM Helpers ─────────────────────────────────────────────────
#define getClass(name, ns)   BNM::LoadClass(name, ns)
#define getOffset(cls, name) (uintptr_t)cls.GetMethod(name).GetOffset()
#define AttachIl2Cpp()       BNM::Loading::TryLoadByJNI()
#define DetachIl2Cpp()       BNM::UnloadBNM()
#define OBFBNM(ns, cls, m, a) \
    BNM::LoadClass(cls, ns).GetMethod(m, a).GetOffset()

// ── Dobby Helper ────────────────────────────────────────────────
#define DHK(target, hook, orig) \
    DobbyHook((void*)(target), (void*)(hook), (void**)&(orig))

// ── AddPointer ──────────────────────────────────────────────────
template<typename T>
static inline void AddPointer(T& fnPtr, uintptr_t offset) {
    fnPtr = reinterpret_cast<T>(offset);
}

// ── GL Globals ──────────────────────────────────────────────────
inline int glWidth  = 0;
inline int glHeight = 0;

// ── ImGui Setup ─────────────────────────────────────────────────
inline void SetupImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAndroid_Init(nullptr);
    ImGui_ImplOpenGL3_Init("#version 300 es");
}

// ── Globals (extern) ────────────────────────────────────────────
extern uintptr_t address;
extern const char* targetLibName;

// ── Unity JNI refs ──────────────────────────────────────────────
extern jclass    UnityPlayer_cls;
extern jfieldID  UnityPlayer_CurrentActivity_fid;
extern void*   (*old_RegisterNatives)(JNIEnv*, jclass, const JNINativeMethod*, jint);
extern void    hook_RegisterNatives(JNIEnv*, jclass, const JNINativeMethod*, jint);

// ── Lib check helpers ───────────────────────────────────────────
extern bool        isLibraryLoaded(const char* libName);
extern uintptr_t   findLibrary(const char* libName);

// ── Menu globals ────────────────────────────────────────────────
namespace Menu {
    inline int  (*Screen_get_height)() = nullptr;
    inline int  (*Screen_get_width)()  = nullptr;
}
