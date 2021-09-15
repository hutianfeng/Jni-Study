#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
/**
 * 它是有格式要求的，由Java_包名类名函数名组成，定义的时候一定要按照这个格式来，
 * 其中，MainActivity是我声明native方法的类的类名。
 * @param env
 * @return
 */
Java_com_peak_jni_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}