# ================================================================
#  lglLayout/jni/Android.mk — FULL FIXED
#  Fix: Thêm đường dẫn jni root vào LOCAL_C_INCLUDES
#       Hooks.cpp dùng "../MainHeader.h" → cần app/src/main/jni/
# ================================================================

FLAVOR_LOCAL_PATH := $(call my-dir)

# ── Trỏ về jni root chính ───────────────────────────────────────
# FLAVOR_LOCAL_PATH = lglLayout/jni/
# JNI_ROOT          = app/src/main/jni/   ← chỗ MainHeader.h nằm
JNI_ROOT := $(FLAVOR_LOCAL_PATH)/../../app/src/main/jni

# ── Include paths ────────────────────────────────────────────────
# [FIX] Thêm JNI_ROOT + các subfolder
LOCAL_C_INCLUDES += \
    $(FLAVOR_LOCAL_PATH) \
    $(FLAVOR_LOCAL_PATH)/Menu \
    $(JNI_ROOT) \
    $(JNI_ROOT)/Hacks \
    $(JNI_ROOT)/Includes \
    $(JNI_ROOT)/Menu \
    $(JNI_ROOT)/KittyMemory

# ── [FIX BACKUP] -I flag trực tiếp phòng trường hợp make scope issue
LOCAL_CPPFLAGS += -I$(JNI_ROOT) -I$(JNI_ROOT)/Hacks

# ── Source files từ flavor ───────────────────────────────────────
LOCAL_SRC_FILES += \
    $(FLAVOR_LOCAL_PATH)/Menu/Setup.cpp \
