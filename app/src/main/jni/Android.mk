LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := dobby/libraries/$(TARGET_ARCH_ABI)/libdobby.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/dobby/
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := GameHelper

LOCAL_CFLAGS    := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS  := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++20
LOCAL_CPPFLAGS  += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_LDFLAGS   += -Wl,--gc-sections,--strip-all,-llog
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2
LOCAL_ARM_MODE  := arm

# ── [FIX] Include paths ──────────────────────────────────────────
# Dùng $(LAYOUT) trực tiếp — tránh my-dir bị sai trong $(eval include)
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Hacks
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Includes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Menu
LOCAL_C_INCLUDES += $(LOCAL_PATH)/KittyMemory
# [KEY FIX] Thêm lglLayout/jni vào include path qua biến LAYOUT
ifdef LAYOUT
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../$(LAYOUT)/jni
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../$(LAYOUT)/jni/Menu
endif

LOCAL_STATIC_LIBRARIES := libdobby

LOCAL_SRC_FILES := $(LOCAL_PATH)/Main.cpp \
    $(LOCAL_PATH)/Includes/Utils.cpp \
    $(LOCAL_PATH)/Includes/Draw.cpp \
    $(LOCAL_PATH)/Includes/ObscuredTypes.cpp \
    $(LOCAL_PATH)/Menu/JNILoader.cpp \
    $(LOCAL_PATH)/Menu/FeatureModule.cpp \
    $(LOCAL_PATH)/Menu/Menu.cpp \
    $(LOCAL_PATH)/KittyMemory/KittyMemory.cpp \
    $(LOCAL_PATH)/KittyMemory/MemoryPatch.cpp \
    $(LOCAL_PATH)/KittyMemory/MemoryBackup.cpp \
    $(LOCAL_PATH)/KittyMemory/KittyUtils.cpp \
    $(LOCAL_PATH)/Hacks/Hooks.cpp \
    $(LOCAL_PATH)/Hacks/Visuals.cpp \

FLAVOR_DIMENSION_VARS := LAYOUT

$(foreach FLAVOR_DIMENSION,$(FLAVOR_DIMENSION_VARS),\
    $(eval FLAVOR := $($(FLAVOR_DIMENSION))) \
    $(if $(FLAVOR),\
        $(info Including JNI for $(FLAVOR_DIMENSION)=$(FLAVOR))\
        $(if $(wildcard $(LOCAL_PATH)/../../$(FLAVOR)/jni/Android.mk),\
            $(eval include $(LOCAL_PATH)/../../$(FLAVOR)/jni/Android.mk),\
            $(warning Missing Android.mk for flavor: $(FLAVOR))\
        ),\
        $(warning No value set for dimension $(FLAVOR_DIMENSION))\
    )\
)

include $(BUILD_SHARED_LIBRARY)


# Loader
include $(CLEAR_VARS)

LOCAL_MODULE    := Loader
LOCAL_CFLAGS    := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_LDLIBS    := -llog -landroid

LOCAL_SRC_FILES := Loader/Loader.cpp

include $(BUILD_SHARED_LIBRARY)
