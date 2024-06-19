//
// Created by Zongcheng Chu on 6/18/24.
//

#pragma once

#include <strings.h>
#include <android/log.h>

#define TAG_INFO "INFO"
#define TAG_ERROR "ERROR"

#define LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, TAG_INFO, __VA_ARGS__)
#define LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, TAG_ERROR, __VA_ARGS__)
