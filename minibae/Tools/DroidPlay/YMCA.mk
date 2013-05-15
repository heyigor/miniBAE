#______________________________________________________________________________________
#                             _ _   _____   ___   ___
#                            | v | |     | |  _| | _ |
#                             |_|  |_|_|_| |___| |_|_|
#______________________________________________________________________________________
#                                                                           version 1.0
# YMCA (You Make Cool Apps)
# Description: Makefile for building Android apps using Ant
# Author: Andrea Stagi (@4ndreaSt4gi)
# Source: https://github.com/4ndreaSt4gi/YMCA
# License: MIT
#
# Copyright (C) 2012 Andrea Stagi <stagi.andrea@gmail.com>.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#______________________________________________________________________________________
#
# Here is a complete list of configuration parameters:
#
# ANDROID_SDK    The path where the Android SDK is placed on your system.
#
# TARGET         The Android target. For a complete list, type Makefile targets.
#
# DEVICE         The device where you want to run your app on. Set to ALL for uploading
#                your app on all devices attached.
#
# ACTIVITY       The main activity you want to run.
#
# This makefile also defines the following goals for use on the command line
# when you run make:
#
# all            This is the default goal if no one is specified. It builds the apk.
#
# clean          Deletes files created during the build.
#
# upload         Uploads the last debug apk on an attached Android DEVICE.
#                Set DEVICE to a specified target, ALL for every device attached.
#
# upload-release Uploads the last signed apk on an attached Android DEVICE.
#                Set DEVICE to a specified target, ALL for every device attached.
#
# sign           Generate a signed package.
#
# restartadb     Restarts adb. Sometimes needed.
#
# devices        Shows the list of all available DEVICEs.
#
# targets        Shows the list of all available targets.
#
# log            Shows the log of a specified DEVICE. Set DEVICE to a specified target.
#______________________________________________________________________________________

ifeq ($(DEVICE) , ALL)
	ALL_DEVS=$(shell $(ANDROID_SDK)/platform-tools/adb devices | grep '[A-Z0-9][A-Z0-9][A-Z0-9]*' | tr '\t[a-z]' ' ' | tr '\n' ' ')
else
    ifdef DEVICE
		DEVICE_CMD= -s $(DEVICE)
    endif
endif

all:
    ifdef TARGET
		@echo "\nUpdating the project..."
		@$(ANDROID_SDK)/tools/android update project --path ./ --target $(TARGET)
    else
        ifeq ($(wildcard build.xml),) 
			$(error "\nERROR! Use make TARGET=N. For a complete list, type 'Makefile targets'.")
        endif
    endif

	@echo "\nBuilding debug apk..."
	@ant debug

upload:
    ifeq ($(DEVICE) , ALL)
		@echo "\nUploading on all the devices attached..."
		$(foreach dev, $(ALL_DEVS),$(ANDROID_SDK)/platform-tools/adb -s $(dev) install -r ./bin/*debug.apk;)
		@echo "\nLaunching the main activity on all the devices attached..."
		$(foreach dev, $(ALL_DEVS),$(ANDROID_SDK)/platform-tools/adb  -s $(dev) shell am start -n $(ACTIVITY);)
    else
		@echo "\nUploading on device..."
		@$(ANDROID_SDK)/platform-tools/adb $(DEVICE_CMD) install -r ./bin/*debug.apk
		@echo "\nLaunching the main activity..."
		@$(ANDROID_SDK)/platform-tools/adb $(DEVICE_CMD) shell am start -n $(ACTIVITY)
    endif

upload-release:
    ifeq ($(DEVICE) , ALL)
		@echo "\nUploading on all the devices attached..."
		$(foreach dev, $(ALL_DEVS),$(ANDROID_SDK)/platform-tools/adb -s $(dev) install -r ./bin/*release.apk;)
		@echo "\nLaunching the main activity on all the devices attached..."
		$(foreach dev, $(ALL_DEVS),$(ANDROID_SDK)/platform-tools/adb  -s $(dev) shell am start -n $(ACTIVITY);)
    else
		@echo "\nUploading on device..."
		@$(ANDROID_SDK)/platform-tools/adb $(DEVICE_CMD) install -r ./bin/*release.apk
		@echo "\nLaunching the main activity..."
		@$(ANDROID_SDK)/platform-tools/adb $(DEVICE_CMD) shell am start -n $(ACTIVITY)
    endif

clean:
	@echo "\nCleaning the project..."
	ant clean

sign:
	@echo "\nGenerating a signed apk..."
    ifneq ($(wildcard ant.properties),)
	@cat ant.properties > .ant.propertiestmp
    ifneq ($(wildcard build.properties),)
	@cat build.properties > .build.propertiestmp
    endif
	@echo "" >> ant.properties
	@echo "" >> build.properties
	@echo "key.store=$(KEY_STORE)" >> ant.properties
	@echo "key.alias=$(KEY_ALIAS)" >> ant.properties
	@echo "key.store=$(KEY_STORE)" >> build.properties
	@echo "key.alias=$(KEY_ALIAS)" >> build.properties
    ifdef KEY_STORE_PASSWORD
        ifdef KEY_ALIAS_PASSWORD
		    @echo "key.alias.password=$(KEY_ALIAS_PASSWORD)" >> ant.properties
		    @echo "key.store.password=$(KEY_STORE_PASSWORD)" >> ant.properties
		    @echo "key.alias.password=$(KEY_ALIAS_PASSWORD)" >> build.properties
		    @echo "key.store.password=$(KEY_STORE_PASSWORD)" >> build.properties
        endif
    endif
	ant release
    endif
	@cat .ant.propertiestmp > ant.properties
	@cat .build.propertiestmp > build.properties
	@rm .ant.propertiestmp
	@rm .build.propertiestmp

restartadb:
	@echo "\nRestarting adb..."
	sudo $(ANDROID_SDK)/platform-tools/adb kill-server
	sudo $(ANDROID_SDK)/platform-tools/adb start-server

devices:
	@$(ANDROID_SDK)/platform-tools/adb devices

targets:
	@$(ANDROID_SDK)/tools/android list

log:
	@$(ANDROID_SDK)/platform-tools/adb $(DEVICE_CMD) logcat
