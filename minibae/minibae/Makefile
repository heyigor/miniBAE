TARGET = libMiniBAE.a

vpath	%.c BAE_Source/Common BAE_Source/Platform 
vpath	%.c banks/patches
vpath	%.c BAE_MPEG_Source_II

INC_PATH	:= -IBAE_Source/Common
INC_PATH	+= -IBAE_Source/Platform
INC_PATH	+= -IBAE_MPEG_Source_II
INC_PATH	+= -Ibanks/patches

SRC		:=	BAE_API_MacOSX.c

SRC		+= \
			DriverTools.c \
			GenAudioStreams.c \
			GenCache.c \
			GenChorus.c \
			GenFiltersReverbU3232.c \
			GenInterp2ReverbU3232.c \
			GenOutput.c \
			GenPatch.c \
			GenReverb.c \
			GenReverbNew.c \
			GenSample.c \
			GenSeq.c \
			GenSeqTools.c \
			GenSetup.c \
			GenSong.c \
			GenSoundFiles.c \
			GenSynth.c \
			GenSynthFiltersSimple.c \
			GenSynthFiltersU3232.c \
			GenSynthInterp2Simple.c \
			GenSynthInterp2U3232.c \
			MiniBAE.c \
			NewNewLZSS.c \
			SampleTools.c \
			X_API.c \
			X_Decompress.c \
			X_IMA.c \
			g711.c \
			g721.c \
			g723_24.c \
			g723_40.c \
			g72x.c \
			BAEPatches.c

#SRC		+= 	\
#			XMPEGFilesSun.c \
#			XMPEG_BAE_API.c \
#			mpa_c_copybytes.c \
#			mpa_conv.c \
#			mpa_dec.c \
#			mpa_dec_bits.c \
#			mpa_dec_crc.c \
#			mpa_dec_l1.c \
#			mpa_dec_l12.c \
#			mpa_dec_l2.c \
#			mpa_dec_l3.c \
#			mpa_dec_l3hf.c \
#			mpa_dec_l3u.c \
#			mpa_dec_poly.c \
#			mpa_dec_tab.c \
#			mpa_dec_vec.c \
#			mpa_version.c

OBJ_DIR := obj/
OBJ := $(addprefix $(OBJ_DIR),$(addsuffix .o,$(basename ${SRC})))

#ARCH	:= -arch x86_64
ARCH	:= -arch i386
CC		:= gcc
CXX		:= g++
LD		:= g++
AR      := ar

CFLAGS  	:= $(ARCH) -g -O0 $(INC_PATH) -D_THREAD_SAFE
CFLAGS		+= -DX_PLATFORM=X_MACINTOSH
CFLAGS		+= -D_DEBUG=1
#CFLAGS		+= -DREVERB_USED=REVERB_DISABLED
CXXFLAGS 	:= $(CFLAGS)
LDFLAGS		:= $(ARCH) -all_load -static-libgcc -prebind


LIBS	= 	-L/opt/local/lib \
			-lpthread \
			-Wl,-framework,CoreFoundation \
			-Wl,-framework,CoreAudio \
			-Wl,-framework,AudioToolbox

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) rcs $(TARGET) $(OBJ)
		
# Rules for compiling source files to object files
$(OBJ_DIR)%.o : %.cpp
	@echo @compile $<
	@mkdir -p $(OBJ_DIR)
	@${CXX} -c ${CXXFLAGS} $< -o $@

$(OBJ_DIR)%.o : %.c
	@echo @compile $<
	@mkdir -p $(OBJ_DIR)
	@${CC} -c ${CFLAGS} $< -o $@

clean:
	@rm -f $(TARGET)
	@rm -rdf $(OBJ_DIR)
	@rm -rdf build
	@echo Cleaned!


