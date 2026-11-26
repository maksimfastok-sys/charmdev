LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := charmdev
LOCAL_SRC_FILES := main.cpp
LOCAL_LDLIBS    := -llog -lGLESv3 -landroid -lEGL
include $(BUILD_SHARED_LIBRARY)
