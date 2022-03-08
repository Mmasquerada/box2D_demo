//
// Created by masque on 2022/1/12.
//

#include "android/log.h"
#ifndef LOG_TAG
#define LOG_TAG "masque"
#define slogd(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define sloge(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif
