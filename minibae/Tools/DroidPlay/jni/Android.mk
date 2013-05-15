# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)/../../../BAE_Source

include $(CLEAR_VARS)


LOCAL_MODULE    := miniBAE
LOCAL_SRC_FILES	:= \
			Common/DriverTools.c \
			Common/GenAudioStreams.c \
			Common/GenCache.c \
			Common/GenChorus.c \
			Common/GenFiltersReverbU3232.c \
			Common/GenInterp2ReverbU3232.c \
			Common/GenOutput.c \
			Common/GenPatch.c \
			Common/GenReverb.c \
			Common/GenReverbNew.c \
			Common/GenSample.c \
			Common/GenSeq.c \
			Common/GenSeqTools.c \
			Common/GenSetup.c \
			Common/GenSong.c \
			Common/GenSoundFiles.c \
			Common/GenSynth.c \
			Common/GenSynthFiltersSimple.c \
			Common/GenSynthFiltersU3232.c \
			Common/GenSynthInterp2Simple.c \
			Common/GenSynthInterp2U3232.c \
			Common/MiniBAE.c \
			Common/NewNewLZSS.c \
			Common/SampleTools.c \
			Common/X_API.c \
			Common/X_Decompress.c \
			Common/X_IMA.c \
			Common/g711.c \
			Common/g721.c \
			Common/g723_24.c \
			Common/g723_40.c \
			Common/g72x.c

LOCAL_SRC_FILES += \
			Platform/BAE_API_Android.c

LOCAL_C_INCLUDES	:= $(LOCAL_PATH)/Common
LOCAL_C_INCLUDES	+= $(LOCAL_PATH)/Platform
LOCAL_C_INCLUDES	+= $(LOCAL_PATH)/../BAE_MPEG_Source_II

LOCAL_CFLAGS := -DX_PLATFORM=X_ANDROID

LOCAL_ARM_MODE := arm

# for native audio
LOCAL_LDLIBS    += -lOpenSLES
# for logging
LOCAL_LDLIBS    += -llog
# for native asset manager
LOCAL_LDLIBS    += -landroid


include $(BUILD_SHARED_LIBRARY)

