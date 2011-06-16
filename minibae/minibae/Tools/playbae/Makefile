TARGET = playbae

vpath	%.c ../../BAE_Source/Common ../../BAE_Source/Platform ../../BAE_MPEG_Source_II

INC_PATH	:= -I../../BAE_Source/Common
INC_PATH	+= -I../../BAE_Source/Platform
INC_PATH	+= -I../../BAE_MPEG_Source_II

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

			
#SRC		+= 	\
#		XMPEGFilesSun.c \
#		XMPEG_BAE_API.c \
#		mpa_c_copybytes.c \
#		mpa_conv.c \
#		mpa_dec.c \
#		mpa_dec_bits.c \
#		mpa_dec_crc.c \
#		mpa_dec_l1.c \
#		mpa_dec_l12.c \
#		mpa_dec_l2.c \
#		mpa_dec_l3.c \
#		mpa_dec_l3hf.c \
#		mpa_dec_l3u.c \
#		mpa_dec_poly.c \
#		mpa_dec_tab.c \
#		mpa_dec_vec.c \
#		mpa_version.c
			
SRC		+=	playbae.c

OBJ_DIR := obj/
OBJ := $(addprefix $(OBJ_DIR),$(addsuffix .o,$(basename ${SRC})))

#ARCH	:= -arch x86_64
ARCH	:= -arch i386
CC		:= gcc
CXX		:= g++
LD		:= g++

CFLAGS  	:= $(ARCH) -g -O3 $(INC_PATH) -D_THREAD_SAFE
CFLAGS		+= -DX_PLATFORM=X_MACINTOSH
#CFLAGS		+= -DUSE_MPEG_DECODER=1
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
	${LD} -o $@ ${LDFLAGS} ${OBJ} ${LIBS}
		
# Rules for compiling source files to object files
$(OBJ_DIR)%.o : %.cpp
	@echo @compile $<
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


