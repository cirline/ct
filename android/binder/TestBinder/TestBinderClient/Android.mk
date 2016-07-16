#
# Copyright (C) 2011 iPanel Inc.
#


LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := eng

LOCAL_SRC_FILES:= \
    ITestBinderService.cpp \
    

LOCAL_SHARED_LIBRARIES := \
	libui libcutils libutils libbinder liblog

LOCAL_MODULE:= libtestBinderClient

LOCAL_C_INCLUDES := \
	$(TOP)/hardware/libhardware/include/ \
	$(TOP)/frameworks/base/include/ \
	$(TOP)/frameworks/tv/include/

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
