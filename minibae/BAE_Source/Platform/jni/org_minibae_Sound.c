#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

// printf
#include <android/log.h>

// for native asset manager
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "org_minibae_Sound.h"
#include "MiniBAE.h"

/*
 * Class:     org_minibae_Sound
 * Method:    _newNativeSound
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_org_minibae_Sound__1newNativeSound
  (JNIEnv* env, jobject jsound, jint mixerReference)
{
	BAEMixer mixer = (BAEMixer)mixerReference;
	BAESound sound = NULL;

	if (mixer)
	{
		__android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "hello sound %p", (void*)mixer);
		sound = BAESound_New(mixer);
	}
	return (jint)sound;
}

/*
 * Class:     org_minibae_Sound
 * Method:    _loadSound
 * Signature: (Ljava/nio/ByteBuffer;)I
 */
JNIEXPORT jint JNICALL Java_org_minibae_Sound__1loadSound__Ljava_nio_ByteBuffer_2
  (JNIEnv* env, jobject snd, jint soundReference, jobject byteBuffer)
{
	BAESound sound = (BAESound)soundReference;
	__android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "hello sound %p", (void*)soundReference);

	return 0;
}

/*
 * Class:     org_minibae_Sound
 * Method:    _loadSound
 * Signature: (Landroid/content/res/AssetManager;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_minibae_Sound__1loadSound__Landroid_content_res_AssetManager_2Ljava_lang_String_2
  (JNIEnv* env, jobject jsound, jint soundReference, jobject assetManager, jstring filename)
{
	BAESound sound = (BAESound)soundReference;
	__android_log_print(ANDROID_LOG_DEBUG, "miniBAE", "hello sound %p", (void*)soundReference);

	return 0;
}

