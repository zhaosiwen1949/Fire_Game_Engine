LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := PhysX
LOCAL_SRC_FILES := ../../../../../External/PhysX-3.4-3.4.2-bsd/Android/obj/local/$(TARGET_ARCH_ABI)/libPhysX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := box2d
LOCAL_SRC_FILES := ../../Box2D/obj/local/$(TARGET_ARCH_ABI)/libBox2D.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := mpg123
LOCAL_SRC_FILES := ../../mpg123/obj/local/$(TARGET_ARCH_ABI)/libmpg123.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := curl
LOCAL_SRC_FILES := ../../curl/obj/local/$(TARGET_ARCH_ABI)/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := FreeImage
LOCAL_SRC_FILES := ../../FreeImage/obj/local/$(TARGET_ARCH_ABI)/libFreeImage.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := freetype
LOCAL_SRC_FILES := ../../FreeType/obj/local/$(TARGET_ARCH_ABI)/libFreeType.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := lua
LOCAL_SRC_FILES := ../../Lua/obj/local/$(TARGET_ARCH_ABI)/libLua.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := luasocket
LOCAL_SRC_FILES := ../../luasocket/obj/local/$(TARGET_ARCH_ABI)/libluasocket.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := zip
LOCAL_SRC_FILES := ../../luazip/obj/local/$(TARGET_ARCH_ABI)/libzip.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := OpenAL
LOCAL_SRC_FILES := ../../OpenAL/obj/local/$(TARGET_ARCH_ABI)/libOpenAL.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := unity
LOCAL_SRC_FILES := ../../unity/obj/local/$(TARGET_ARCH_ABI)/libunity.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := zxing
LOCAL_SRC_FILES := ../../zxing/obj/local/arm64-v8a/libzxing.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := ../../OpenSSL1.1.1a/libcrypto.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ssl
LOCAL_SRC_FILES := ../../OpenSSL1.1.1a/libssl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := AliceRuntime
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../../../../Runtime/Android/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../../../ \
$(LOCAL_PATH)/../../../../../External/Lua/ \
$(LOCAL_PATH)/../../../../../External/protobuf/lite \
$(LOCAL_PATH)/../../../../../External/freetype2/include \
$(LOCAL_PATH)/../../../../../External/Box2D \
$(LOCAL_PATH)/../../../../../External/PhysX/include \
$(LOCAL_PATH)/../../OpenSSL1.1.1a \
$(LOCAL_PATH)/../../ffmpeg/include \
$(LOCAL_PATH)/../../luazip/jni \
$(LOCAL_PATH)/../../curl/jni/include \
$(LOCAL_PATH)/../../../../../External/zxing-cpp-master/core/src

LOCAL_STATIC_LIBRARIES := lua luasocket unity curl PhysX OpenAL freetype zip FreeImage box2d mpg123 zxing crypto ssl
LOCAL_LDLIBS    := -llog -lz -ldl -lm -lGLESv3 -landroid -latomic

$(call import-add-path,$(LOCAL_PATH))
include $(BUILD_SHARED_LIBRARY)
