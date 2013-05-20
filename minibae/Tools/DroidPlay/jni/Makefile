
all:
	@echo "Targets"
	@echo " build"
	@echo " clean"
	@echo " jni"

build:
	ndk-build

clean:
	ndk-build clean

jni:
	javac -d ../bin/classes ../src/org/minibae/*.java
	javah -classpath ../bin/classes -jni org.minibae.Sound
	javah -classpath ../bin/classes -jni org.minibae.Mixer
	mv *.h ../../../BAE_Source/Platform/jni

 