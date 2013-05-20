
all:
	@echo "Targets"
	@echo " build"
	@echo " clean"
	@echo " jni"

JAVA_CLASSES 		:= ../bin/classes
JAVA_BOOT_CLASSES	:= $(ANDROID_SDK)/platforms/android-17/android.jar
JAVA_SOURCE 		:= ../src/org/minibae
JAVA_JNI_SOURCE		:= ../../../BAE_Source/Platform/jni

jni:
	javac -bootclasspath $(JAVA_BOOT_CLASSES) -d $(JAVA_CLASSES) $(JAVA_SOURCE)/*.java
	javah -d $(JAVA_JNI_SOURCE) -bootclasspath $(JAVA_BOOT_CLASSES) -classpath $(JAVA_CLASSES) -jni org.minibae.Sound
	javah -d $(JAVA_JNI_SOURCE) -bootclasspath $(JAVA_BOOT_CLASSES) -classpath $(JAVA_CLASSES) -jni org.minibae.Mixer

build:
	ndk-build

clean:
	ndk-build clean

