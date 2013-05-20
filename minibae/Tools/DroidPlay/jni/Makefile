
all:
	@echo "Targets"
	@echo " build"
	@echo " clean"
	@echo " make_jni"

build:
	ndk-build

clean:
	ndk-build clean

make_jni:
	javah -classpath ../bin/classes -jni org.minibae.Sound
	javah -classpath ../bin/classes -jni org.minibae.Mixer
	mv *.h ../../../BAE_Source/Platform/jni

 