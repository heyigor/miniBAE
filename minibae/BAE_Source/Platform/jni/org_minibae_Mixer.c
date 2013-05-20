#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

// printf
#include <android/log.h>

// for native asset manager
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

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

    if((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK)
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
	BAEMixer mixer = BAEMixer_New();
	if (mixer)
	{
	    __android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "hello mixer %p", mixer);
	}
	return (jint)mixer;
}

/*
 * Class:     org_minibae_Mixer
 * Method:    _deleteMixer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_minibae_Mixer__1deleteMixer
  (JNIEnv* env, jclass clazz, jint reference)
{
	BAEMixer mixer = (BAEMixer)reference;
	if (mixer)
	{
		BAEMixer_Delete(mixer);
	    __android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "goodbye mixer %p", mixer);
	}
}

/*
 * Class:     org_minibae_Mixer
 * Method:    _openMixer
 * Signature: (IIIIII)I
 */
JNIEXPORT jint JNICALL Java_org_minibae_Mixer__1openMixer
  (JNIEnv* env, jclass clazz, jint reference, jint sampleRate, jint terpMode, jint maxSongVoices, jint maxSoundVoices, jint mixLevel)
{
	jint status = -1;
    BAEResult    err;

	BAEMixer mixer = (BAEMixer)reference;
	if (mixer)
	{
        err = BAEMixer_Open(mixer,
                            sampleRate,
                            terpMode,
                            BAE_USE_STEREO | BAE_USE_16,
                            maxSongVoices,	// midi voices
                            maxSoundVoices, // pcm voices
                            mixLevel,
                            FALSE);
        if (err == BAE_NO_ERROR)
        {
	    	__android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "hello openMixer");
	    }
	    else
	    {
	    	__android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "failed to open mixer (%d)", err);
	    }
	}
	return status;
}

