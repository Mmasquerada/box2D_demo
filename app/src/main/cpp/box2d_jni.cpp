/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "Box2D/Box2D.h"

#include <cstdio>
#include "CollisionModel.h"
#include "blog.h"
#include "jni.h"
#include <malloc.h>

#include <string>
#define LOG_TAG "box2d_jni"


static const char *kJniInterfacePath = "com.bilibili.esleylance.myapplication/JniInterface";

static CollisionModel *collisionModel = nullptr;

extern "C"
JNIEXPORT void
JNICALL Java_com_bilibili_esleylance_myapplication_JniInterface_createModel(
        JNIEnv *env,
        jclass clazz) {
    collisionModel = new CollisionModel();
}

extern "C"
JNIEXPORT void
JNICALL Java_com_bilibili_esleylance_myapplication_JniInterface_updateBounds(
        JNIEnv *env,
        jclass clazz,
        jint width,
        jint height,
        jint count) {
    collisionModel->updateBounds(width, height);
    slogd("bound width = %d, height = %d", width, height);
}

extern "C"
JNIEXPORT void
JNICALL Java_com_bilibili_esleylance_myapplication_JniInterface_bindBody(
        JNIEnv *env,
        jclass clazz,
        jfloat x,
        jfloat y,
        jfloat width,
        jfloat height,
        int index,
        int type) {
    if (type == 1) {
        collisionModel->createRectangleBody(x, y, width, height, index);
    } else {
        collisionModel->createBody(x, y, width, height, index);
    }
}

extern "C"
JNIEXPORT void
JNICALL Java_com_bilibili_esleylance_myapplication_JniInterface_startWorld(
        JNIEnv *env,
        jclass clazz) {
    collisionModel->startWorld();
}

extern "C"
JNIEXPORT void
JNICALL Java_com_bilibili_esleylance_myapplication_JniInterface_drawView(
        JNIEnv *env,
        jclass clazz,
        int index) {
    float *pos = collisionModel->queryPosition(index);
    jclass jniInterfaceCls = env->FindClass("com/bilibili/esleylance/myapplication/JniInterface");
    if(!jniInterfaceCls) {
        sloge("Failed to get %s class", kJniInterfacePath);
        return;
    }
    jmethodID method = env->GetStaticMethodID(jniInterfaceCls, "notifyEvent", "(IFFF)V");

    jstring str_arg = env->NewStringUTF("update position");
    env->CallStaticVoidMethod(jniInterfaceCls, method, index, pos[0], pos[1], 180.0f / 3.14f * collisionModel->queryAngle(index));
    env->DeleteLocalRef(jniInterfaceCls);
    env->DeleteLocalRef(str_arg);
}


extern "C"
JNIEXPORT void
JNICALL Java_com_bilibili_esleylance_myapplication_JniInterface_applyLinearImpulse(
        JNIEnv *env,
        jclass clazz,
        jfloat x,
        jfloat y) {
    collisionModel->applyLinearImpulse(x, y);
}
