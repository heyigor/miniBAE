ACTIVITY=com.igorlabs/com.igorlabs.Start
TARGET=2

first: 
	@echo "TARGETS:"
	@echo "   build"
	@echo "   super_clean"
	@echo "   native"

build: native all
	@echo done!

native:
	ndk-build

super_clean: clean
	@ndk-build clean
	@rm -Rf out
	@rm -Rf obj
	@rm -Rf bin

include YMCA.mk
