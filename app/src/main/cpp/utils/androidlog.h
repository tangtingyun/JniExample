//
// Created by gp-0168 on 2020/11/12.
//


#ifndef JNIEXAMPLE_ANDROIDLOG_H
#define JNIEXAMPLE_ANDROIDLOG_H

#include <android/log.h>

// log标签
#define  TAG    "JNI_EXAMPLE"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

#endif //JNIEXAMPLE_ANDROIDLOG_H
