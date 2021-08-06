#include "AndroidRuntime.h"
#include "Runtime/Render/AliceGL.h"
#if ALICE_ANDROID
JAVA_API(void) JFOO(AliceRuntime, Init)(JNIEnv * env, jobject obj, jobject assetManager, jstring outerPath, jint width, jint height, jint core_version, jstring entry_script_bundle, jstring entry_script) {

}
JAVA_API(void) JFOO(AliceRuntime, OnSurfaceCreated)(JNIEnv * env, jobject obj) {

}
JAVA_API(void) JFOO(AliceRuntime, OnViewportChanged)(JNIEnv * env, jobject obj, jint width, jint height) {
	glViewport(0, 0, width, height);
}
JAVA_API(int) JFOO(AliceRuntime, Render)(JNIEnv * env, jobject obj) {
	glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	return 0;
}
JAVA_API(void) JFOO(AliceRuntime, Pause)(JNIEnv * env, jobject obj) {

}
JAVA_API(void) JFOO(AliceRuntime, Resume)(JNIEnv * env, jobject obj) {

}
JAVA_API(void) JFOO(AliceRuntime, OnReturn)(JNIEnv * env, jobject obj) {

}
JAVA_API(void) JFOO(AliceRuntime, OnShake)(JNIEnv * env, jobject obj) {

}
JAVA_API(void) JFOO(AliceRuntime, OnTouchBegin)(JNIEnv * env, jobject obj, jlong touchID, jfloat  x, jfloat  y) {

}
JAVA_API(void) JFOO(AliceRuntime, OnTouchEnd)(JNIEnv * env, jobject obj, jlong touchID, jfloat  x, jfloat  y) {

}
JAVA_API(void) JFOO(AliceRuntime, OnTouchCancel)(JNIEnv * env, jobject obj, jlong touchID, jfloat  x, jfloat  y) {

}
JAVA_API(void) JFOO(AliceRuntime, OnTouchMove)(JNIEnv * env, jobject obj, jlong touchID, jfloat  x, jfloat  y) {

}
JAVA_API(void) JFOO(AliceRuntime, OnIMEChar)(JNIEnv * env, jobject obj, jbyteArray java_string) {

}
JAVA_API(void) JFOO(AliceRuntime, OnIMECompositionString)(JNIEnv * env, jobject obj, jbyteArray java_string, jboolean isResult) {

}
JAVA_API(void) JFOO(AliceRuntime, OnEditEnd)(JNIEnv * env, jobject obj) {

}
JAVA_API(void) JFOO(AliceRuntime, OnGetInfoFromQRCode)(JNIEnv * env, jobject obj, jstring qrcode_info) {

}
JAVA_API(void) JFOO(AliceRuntime, OnGetImageFromAlbum)(JNIEnv * env, jobject obj, jstring imagepath) {
}
JAVA_API(void) JFOO(AliceRuntime, OnGetImageDataFromAlbum)(JNIEnv * env, jobject obj, jstring imagepath) {
}
JAVA_API(void) JFOO(AliceRuntime, SetDeviceInfo)(JNIEnv * env, jobject obj,
	jint core_count, jstring device_name, jstring cpu_type, jstring system_name, jint memorysize) {
}
JAVA_API(void) JFOO(AliceRuntime, SetScreenInfo)(JNIEnv * env, jobject obj,
	jint width, jint height, jfloat scale, jfloat xdpi, jfloat ydpi, jfloat dpi) {
}
#endif
