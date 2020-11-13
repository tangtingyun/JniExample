#include <jni.h>
#include <string>
#include <android/log.h>
#include "md5.h"

// log标签
#define  TAG    "JNI_EXAMPLE"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

static const char *EXTRA_M = "JNI_PINK";

extern "C"
JNIEXPORT jstring JNICALL
Java_com_step_jniexample_SignatureUtils_signatureParams(JNIEnv *env, jclass clazz,
                                                        jstring paramsStr) {

    const char *params = env->GetStringUTFChars(paramsStr, JNI_FALSE);

    std::string signature_str(params);
    signature_str.insert(0, EXTRA_M);

    signature_str = signature_str.substr(0, signature_str.length() - 2);

    std::string md5Sign = MD5(signature_str).toStr();


    LOGD("原始字符串 %s ", signature_str.c_str());
    LOGD("加密后字符串 %s ", md5Sign.c_str());


    env->ReleaseStringUTFChars(paramsStr, params);

    return env->NewStringUTF(signature_str.c_str());
}