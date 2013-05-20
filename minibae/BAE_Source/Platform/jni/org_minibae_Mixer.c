#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>

#include "org_minibae_Mixer.h"
#include "MiniBAE.h"

//http://developer.android.com/training/articles/perf-jni.html

static JavaVM* gJavaVM = NULL;


jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;

	// cache java VM
	gJavaVM = vm;

    __android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "JNI_OnLoad called");

    if((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        __android_log_print(ANDROID_LOG_ERROR, "miniBAE", "Failed to get the environment using GetEnv()");
        return -1;
    }

    // Get jclass with env->FindClass.
    // Register methods with env->RegisterNatives.

    return JNI_VERSION_1_6;
}

/*
 * Class:     org_minibae_Mixer
 * Method:    _newMixer
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_minibae_Mixer__1newMixer
  (JNIEnv* env, jclass clazz)
{
    __android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "hello newMixer");
	return 0;
}

/*
 * Class:     org_minibae_Mixer
 * Method:    _deleteMixer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_minibae_Mixer__1deleteMixer
  (JNIEnv* env, jclass clazz, jint reference)
{
}

/*
 * Class:     org_minibae_Mixer
 * Method:    _openMixer
 * Signature: (IIIIII)I
 */
JNIEXPORT jint JNICALL Java_org_minibae_Mixer__1openMixer
  (JNIEnv* env, jclass clazz, jint sampleRate, jint terpMode, jint modifiers, jint maxSongVoices, jint maxSoundVoices, jint mixLevel)
{
    __android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "hello openMixer");
	return 0;
}

