TARGET = playbae

VPATH = ../../BAE_Source/Common ../../BAE_Source/Platform ../../BAE_MPEG_Source_II
VPATH += ../../BAE_MPEG_Source_II/mp3dec/src

INC_PATH	:= -I../../BAE_Source/Common
INC_PATH	+= -I../../BAE_Source/Platform
INC_PATH	+= -I../../BAE_MPEG_Source_II
INC_PATH	+= -I../../BAE_MPEG_Source_II/mp3dec/include -I../../BAE_MPEG_Source_II/mp3dec/src

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
			g72x.c

SRC			+= \
			pvmp3_normalize.cpp \
		 	pvmp3_alias_reduction.cpp \
		 	pvmp3_crc.cpp \
		 	pvmp3_decode_header.cpp \
		 	pvmp3_decode_huff_cw.cpp \
		 	pvmp3_getbits.cpp \
		 	pvmp3_dequantize_sample.cpp \
		 	pvmp3_framedecoder.cpp \
		 	pvmp3_get_main_data_size.cpp \
		 	pvmp3_get_side_info.cpp \
		 	pvmp3_get_scale_factors.cpp \
		 	pvmp3_mpeg2_get_scale_data.cpp \
		 	pvmp3_mpeg2_get_scale_factors.cpp \
		 	pvmp3_mpeg2_stereo_proc.cpp \
		 	pvmp3_huffman_decoding.cpp \
		 	pvmp3_huffman_parsing.cpp \
		 	pvmp3_tables.cpp \
		 	pvmp3_imdct_synth.cpp \
		 	pvmp3_mdct_6.cpp \
		 	pvmp3_dct_6.cpp \
		 	pvmp3_poly_phase_synthesis.cpp \
		 	pvmp3_equalizer.cpp \
		 	pvmp3_seek_synch.cpp \
		 	pvmp3_stereo_proc.cpp \
		 	pvmp3_reorder.cpp

ifeq ($(TARGET_ARCH),arm)
SRC += \
	asm/pvmp3_polyphase_filter_window_gcc.s \
 	asm/pvmp3_mdct_18_gcc.s \
 	asm/pvmp3_dct_9_gcc.s \
	asm/pvmp3_dct_16_gcc.s
else
SRC += \
 	pvmp3_polyphase_filter_window.cpp \
 	pvmp3_mdct_18.cpp \
 	pvmp3_dct_9.cpp \
 	pvmp3_dct_16.cpp
endif

SRC			+= \
			XMPEGFiles.c \
			XMPEG_PACKET_BAE_API.cpp
			
SRC		+=	playbae.c

OBJ_DIR := obj/
OBJ := $(addprefix $(OBJ_DIR),$(addsuffix .o,$(basename ${SRC})))

CC     := clang
CXX    := clang++
LD     := clang++

CFLAGS  	:= $(ARCH) -g -O3 $(INC_PATH) -D_THREAD_SAFE
CFLAGS		+= -DX_PLATFORM=X_MACINTOSH
CFLAGS		+= -DUSE_MPEG_DECODER=1
CFLAGS		+= -D_DEBUG=1
#CFLAGS		+= -DREVERB_USED=REVERB_DISABLED
CXXFLAGS 	:= $(CFLAGS)
LDFLAGS		:= $(ARCH) -all_load -prebind


LIBS	= 	-L/opt/local/lib \
			-lpthread \
			-Wl,-framework,CoreFoundation \
			-Wl,-framework,CoreAudio \
			-Wl,-framework,AudioToolbox

all: $(TARGET)

$(TARGET): $(OBJ)
	${LD} -o $@ ${LDFLAGS} ${OBJ} ${LIBS}
		
# Rules for compiling source files to object files
$(OBJ_DIR)%.o : %.cpp
	@echo @compile++ $<
	@mkdir -p $(OBJ_DIR)
	@${CXX} -c ${CXXFLAGS} $< -o $@

$(OBJ_DIR)%.o : %.c
	@echo @compile $<
	@mkdir -p $(OBJ_DIR)
	@${CC} -c ${CFLAGS} $< -o $@

install: all
	cp $(TARGET) ../../bin/.

lib_install:

CONTENT := ../../Content
BANKS := ../../banks

# requires mp3 codec
#TEST_RMF    := LT_RMF_MPEG.rmf
#TEST_RMF	:= little_boss.rmf
#TEST_RMF	:= Ajarna.rmf
#TEST_RMF	:= health_and_safety.rmf

# standard
#TEST_RMF	:= renegado-full.rmf
#TEST_RMF	:= the-future-is.rmf
#TEST_RMF    := jetset-full.rmf
#TEST_RMF	:= "Hide 'n Seek.rmf"
#TEST_RMF	:= polyzoot-full.rmf
TEST_RMF	:= chop-squad-full.rmf

test1: $(TARGET)
	./$(TARGET) -p $(BANKS)/patches/patches.hsb -m $(CONTENT)/rmf/kartv.mid -mr 44100

test2: $(TARGET)
	./$(TARGET) -p $(BANKS)/patches/patches.hsb -m $(CONTENT)/midi/synco.mid -mr 44100

test3: $(TARGET)
	./$(TARGET) -p $(BANKS)/patches/patches.hsb -r $(CONTENT)/rmf/$(TEST_RMF) -mr 44100

test4: $(TARGET)
	./$(TARGET) -p $(BANKS)/patches/patches.hsb -w $(CONTENT)/wav/sexy_coffee_pot.wav -mr 44100

test5: $(TARGET)
	./$(TARGET) -p $(BANKS)/patches/patches.hsb -sw $(CONTENT)/wav/sexy_coffee_pot.wav -mr 44100

clean:
	@rm -f $(TARGET)
	@rm -rdf $(OBJ_DIR)
	@rm -rdf build
	@echo Cleaned!


