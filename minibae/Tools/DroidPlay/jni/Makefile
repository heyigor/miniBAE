
all:
	@echo "Targets"
	@echo " build"
	@echo " clean"
	@echo " jni"

JAVA_CLASSES 	:= ../bin/classes
JAVA_SOURCE 	:= ../src/org/minibae
JAVA_JNI_SOURCE	:= ../../../BAE_Source/Platform/jni

jni:
	javac -d $(JAVA_CLASSES) $(JAVA_SOURCE)/*.java
	javah -d $(JAVA_JNI_SOURCE) -classpath $(JAVA_CLASSES) -jni org.minibae.Sound
	javah -d $(JAVA_JNI_SOURCE) -classpath $(JAVA_CLASSES) -jni org.minibae.Mixer

build:
	ndk-build

clean:
	ndk-build clean

