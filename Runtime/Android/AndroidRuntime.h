#include "Runtime/RuntimePrefix.h"
#ifdef ALICE_ANDROID
JAVA_API(void) JFOO(AliceRuntime, RunOnUIThread)(JNIEnv * env, jobject obj, jobject mainActivity);
JAVA_API(void) JFOO(AliceRuntime, Init)(JNIEnv * env, jobject obj, jobject assetManager, jstring outerPath, jint width, jint height, jint core_version, jstring entry_script_bundle, jstring entry_script);

JAVA_API(void) JFOO(AliceRuntime, OnSurfaceCreated)(JNIEnv * env, jobject obj);
JAVA_API(void) JFOO(AliceRuntime, OnViewportChanged)(JNIEnv * env, jobject obj, jint width, jint height);
JAVA_API(int) JFOO(AliceRuntime, Render)(JNIEnv * env, jobject obj);

JAVA_API(void) JFOO(AliceRuntime, Pause)(JNIEnv * env, jobject obj);
JAVA_API(void) JFOO(AliceRuntime, Resume)(JNIEnv * env, jobject obj);

JAVA_API(void) JFOO(AliceRuntime, OnReturn)(JNIEnv * env, jobject obj);
JAVA_API(void) JFOO(AliceRuntime, OnShake)(JNIEnv * env, jobject obj);

JAVA_API(void) JFOO(AliceRuntime, OnTouchBegin)(JNIEnv * env, jobject obj, jlong touchID, jfloat  x, jfloat  y);
JAVA_API(void) JFOO(AliceRuntime, OnTouchEnd)(JNIEnv * env, jobject obj, jlong touchID, jfloat  x, jfloat  y);
JAVA_API(void) JFOO(AliceRuntime, OnTouchCancel)(JNIEnv * env, jobject obj, jlong touchID, jfloat  x, jfloat  y);
JAVA_API(void) JFOO(AliceRuntime, OnTouchMove)(JNIEnv * env, jobject obj, jlong touchID, jfloat  x, jfloat  y);

JAVA_API(void) JFOO(AliceRuntime, OnIMEChar)(JNIEnv * env, jobject obj, jbyteArray ut8str);
JAVA_API(void) JFOO(AliceRuntime, OnIMECompositionString)(JNIEnv * env, jobject obj, jbyteArray ut8str, jboolean isResult);
JAVA_API(void) JFOO(AliceRuntime, OnEditEnd)(JNIEnv * env, jobject obj);
JAVA_API(void) JFOO(AliceRuntime, OnGetInfoFromQRCode)(JNIEnv * env, jobject obj, jstring outerPath);
JAVA_API(void) JFOO(AliceRuntime, OnGetImageFromAlbum)(JNIEnv * env, jobject obj, jstring imagepath);
JAVA_API(void) JFOO(AliceRuntime, SetDeviceInfo)(JNIEnv * env, jobject obj,
	jint core_count, jstring device_name, jstring cpu_type, jstring system_name, jint memorysize);
JAVA_API(void) JFOO(AliceRuntime, SetScreenInfo)(JNIEnv * env, jobject obj,
	jint width, jint height, jfloat scale, jfloat xdpi, jfloat ypdi, jfloat dpi);
#endif