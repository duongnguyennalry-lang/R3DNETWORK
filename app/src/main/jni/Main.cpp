#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <numbers>

#include "Includes/CrashLogger.h"
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "dobby/dobby.h"
#include "Menu/FeatureModule.hpp"
#include "UnityResolve/UnityResolve.hpp"
#include "Includes/Utils.hpp"
#include "Includes/RemapTools.h"
#include "KittyMemory/MemoryPatch.h"
#include "Includes/ObscuredTypes.hpp"
#include "Includes/ESPManager.h"
#include "Menu/JNILoader.hpp"
#include "Menu/Setup.hpp"
#include "Hacks/Hooks.hpp"

#define IL2CPP_MODULE OBFUSCATE("libil2cpp.so")

// ================================================================
//  DEBUG HELPER — ghi file trực tiếp, không qua logcat
// ================================================================
static FILE* g_dbg = nullptr;

static void dbg_open() {
    system("mkdir -p /sdcard/ThrowIO_Crash");
    g_dbg = fopen("/sdcard/ThrowIO_Crash/early.txt", "w");
}

static void dbg(const char* msg) {
    if (!g_dbg) return;
    fprintf(g_dbg, "%s\n", msg);
    fflush(g_dbg);
}

static void dbg_close() {
    if (!g_dbg) return;
    fclose(g_dbg);
    g_dbg = nullptr;
}

// ================================================================
//  HACK THREAD
// ================================================================
void *hack_thread(void *) {
    FILE* f = fopen("/sdcard/ThrowIO_Crash/early.txt", "a");
    if (f) { fprintf(f, "STEP 3: hack_thread started\n"); fflush(f); }

    LOGI(OBFUSCATE("pthread created"));

    if (f) { fprintf(f, "STEP 4: new ESPManager...\n"); fflush(f); }
    espManager = new ESPManager();
    if (f) { fprintf(f, "STEP 5: ESPManager ok\n"); fflush(f); }

    // Chờ libil2cpp load
    if (f) { fprintf(f, "STEP 6: waiting for il2cpp...\n"); fflush(f); }
    do {
        sleep(1);
    } while (!KittyMemory::getLibraryMap(IL2CPP_MODULE).isValid());

    if (f) { fprintf(f, "STEP 7: il2cpp loaded\n"); fflush(f); }
    LOGI(OBFUSCATE("%s has been loaded"), (const char *) IL2CPP_MODULE);
    LOGI(OBFUSCATE("Trying to hook in il2cpp now..."));

    sleep(5);
    if (f) { fprintf(f, "STEP 8: after sleep 5s\n"); fflush(f); }

    auto p_handler = dlopen(IL2CPP_MODULE, RTLD_NOW);
    if (!p_handler) {
        if (f) {
            fprintf(f, "FATAL: dlopen fail: %s\n", dlerror());
            fflush(f); fclose(f);
        }
        LOGE(OBFUSCATE("Failed to dlopen %s: %s"),
             (const char *) IL2CPP_MODULE, dlerror());
        return NULL;
    }
    if (f) { fprintf(f, "STEP 9: dlopen ok\n"); fflush(f); }

    if (f) { fprintf(f, "STEP 10: UnityResolve::Init...\n"); fflush(f); }
    UnityResolve::Init(p_handler);
    if (f) { fprintf(f, "STEP 11: UnityResolve::Init ok\n"); fflush(f); }

    LOGI(OBFUSCATE("Starting hooks"));
    if (f) { fprintf(f, "STEP 12: Hooks::InitHooks...\n"); fflush(f); }
    Hooks::InitHooks();
    if (f) { fprintf(f, "STEP 13: InitHooks ok — DONE\n"); fflush(f); fclose(f); }

    return NULL;
}

// ================================================================
//  ENTRY POINT
// ================================================================
__attribute__((constructor))
void init() {
    // Bước 1 — ghi file ngay lập tức trước mọi thứ
    system("mkdir -p /sdcard/ThrowIO_Crash");
    FILE* f = fopen("/sdcard/ThrowIO_Crash/early.txt", "w");
    if (f) { fprintf(f, "STEP 1: init() entered\n"); fflush(f); }

    InitCrashLogger();
    if (f) { fprintf(f, "STEP 2: InitCrashLogger ok — launching thread\n"); fflush(f); fclose(f); }

    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);

    RemapTools::RemapLibrary(OBFUSCATE("libLoader.so"));
}

// ================================================================
//  JNI_OnLoad
// ================================================================
extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    if (JNILoader::RegisterAll(env) != JNI_OK)
        return JNI_ERR;

    if (RegisterMenu(env) != 0)
        return JNI_ERR;

    return JNI_VERSION_1_6;
}
