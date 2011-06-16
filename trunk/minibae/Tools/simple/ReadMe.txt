This document describes in detail the "simple" project.

This project contains a limited version of BAE that can only output
mono 16 no reverb data. It can play RMF files. The example code it
currently set to output at 8 kHz at 10 ms quantized sample generation.
Also the BAE mixer is limited to 8 voices. So any voices beyond that
are clipped.

When executed each test application will create data files. All data
is stored in intel format.

It is a Visual Studio 5 with Service Pack 3 installed.

To build:

You'll need DirectSound 5 at least. Configure your include and library
folders within the "Options..." menu.

Do a "Batch Build" and build all projects.

You'll end up with three ".exe" files which to place in the same folder with the
files patches.hsb, squarewave.rmf, and filteredsquarewave.rmf.

When you double click them they will play a 4 second tone and exit.

********************
filterloop.exe

plays:
filteredsquarewave.rmf

This test generates:
PV_Generate16outputMono.da2 (See GenOutputSimple.c)
PV_ServeInterp2FilterPartialBuffer16.da2 (See GenSynthFiltersSimple.c)

********************
normalloop.exe

plays:
squarewave.rmf

This test generates:
PV_Generate16outputMono.da1 (See GenOutputSimple.c)
PV_ServeInterp2FullBuffer16.da1 (See GenSynthInterp2Simple.c)
PV_ServeInterp2PartialBuffer16.da1 (See GenSynthInterp2Simple.c)

The "filterloop.exe, and normalloop.exe" examples write to various files in a 
microsecond time domain. Each area of the code captured contains a time frame 
and all variables that are required for that function to work. Both input data 
and output data are captured.

Details of file data written by file function:
GenOutputSimple.c
Data is in the same time domain for both tests. But results are different
because one data is filtered, and the other is not.

function PV_Generate16outputMono
Format:
" BEGIN- "
" Time: XXXX "	// time in microseconds
input
long word		// quality should be 8
long word		// MusicGlobals->Four_Loop this is the total number of samples
				// to be built this pass divided by 4.
data			// internal mixed down sample data to be converted to PCM format
				// will be MusicGlobals->Four_Loop * 4 in size.
output
data			// processed samples in PCM format
				// will be MusicGlobals->Four_Loop * 4 in size.
" -END "


GenSynthInterp2Simple.c

function PV_ServeInterp2FullBuffer16
Format:
"BEGIN-"
"Time: XXXX"	// time in microseconds
input
long word		// this_voice->lastAmplitudeL; last amplitude used in loop
long word		// this_voice->NoteVolume; volume level that has been calculated
word			// this_voice->NoteVolumeEnvelope; LFO, envelope volume
long word		// this_voice->NotePitch; 16.16 fixed (1.0 is natural pitch, 2.0 is twice as fast)
long word		// this_voice->NoteWave; 20.12 fixed (sample position to read from)
long word		// MusicGlobals->Four_Loop this is the total number of samples
				// to be built this pass divided by 4.
long word		// this_voice->voiceMode; 
long word		// this_voice->NotePtr; pointer to source data. Assume as offset 0.
long word		// this_voice->NotePtrEnd, pointer to end of source data. Assume as offset end.
long word		// this_voice->NoteLoopPtr, pointer within NotePtr:NotePtrEnd. Assume as offset to loop start
long word		// this_voice->NoteLoopEnd, pointer within NotePtr:NotePtrEnd. Assume as offset to loop end

data			// source sample data starting NotePtr to NotePtrEnd.
				// will be NotePtrEnd - NotePtr bytes in size.
data			// mix buffer will be MusicGlobals->Four_Loop * 4 in size (bytes)

output
long word		// this_voice->NoteWave; 20.12 fixed (sample position to read from)
long word		// this_voice->lastAmplitudeL; last amplitude used in loop
data			// mix buffer will be MusicGlobals->Four_Loop * 4 in size (bytes)
"-END"

function PV_ServeInterp2FilterPartialBuffer16
"BEGIN-
"Time: XXXX"	// time in microseconds
input
data			// filter array (256 bytes)
long word		// this_voice->Z1value; filter value
long word		// this_voice->zIndex; index into filter array
long word		// this_voice->LPF_resonance; filter resonance amount
long word		// this_voice->LPF_frequency; freq amount
long word		// this_voice->previous_zFrequency; last freq amount
long word		// this_voice->lastAmplitudeL; last amplitude used in loop
long word		// this_voice->NoteVolume; volume level that has been calculated
word			// this_voice->NoteVolumeEnvelope; LFO, envelope volume
long word		// this_voice->NotePitch; 16.16 fixed (1.0 is natural pitch, 2.0 is twice as fast)
long word		// this_voice->NoteWave; 20.12 fixed (sample position to read from)
long word		// MusicGlobals->Four_Loop this is the total number of samples
				// to be built this pass divided by 4.
long word		// this_voice->voiceMode; 
long word		// this_voice->NotePtr; pointer to source data. Assume as offset 0.
long word		// this_voice->NotePtrEnd, pointer to end of source data. Assume as offset end.
long word		// this_voice->NoteLoopPtr, pointer within NotePtr:NotePtrEnd. Assume as offset to loop start
long word		// this_voice->NoteLoopEnd, pointer within NotePtr:NotePtrEnd. Assume as offset to loop end
data			// source sample data starting NotePtr to NotePtrEnd.
				// will be NotePtrEnd - NotePtr bytes in size.
data			// mix buffer will be MusicGlobals->Four_Loop * 4 in size (bytes)

output
long word		// this_voice->NoteWave; 20.12 fixed (sample position to read from)
long word		// this_voice->lastAmplitudeL; last amplitude used in loop
long word		// this_voice->Z1value; filter value
long word		// this_voice->zIndex; index into filter array
data			// mix buffer will be MusicGlobals->Four_Loop * 4 in size (bytes)
"-END"

To understand the general flow use the time index values to piece together
a rough view.

Overall it looks like:

For normalloop.exe
Many calls to PV_ServeInterp2FullBuffer16, then PV_Generate16outputMono.
Then prior to ending it will be PV_ServeInterp2PartialBuffer16, then 
PV_Generate16outputMono.

For filterloop.exe
Many calls to PV_ServeInterp2FilterPartialBuffer16, then PV_Generate16outputMono.

********************
simple.exe

plays:
normalwithoutsamples.rmf

It does not record or generate any files.

That's it.


