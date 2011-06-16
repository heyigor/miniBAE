Enclosed is the MiniBAE test suite:

In the Documentation folder you'll find details about MiniBAE various API's.

In the TestSuite folder you'll find demo applications for single CPU tests, and
dual CPU tests, and media.

This application is a single CPU version of MiniBAE plus our test suite:
TestSuite\TestSuite.exe

These two applications are simulation of what parts of MiniBAE run on a DSP,
and what parts run on another CPU. Run the DSP version first, then run the MCU
version. There's a communcations layer between them for control the audio.
All audio is rendered using the DSP application.
TestSuite\TestSuite_MCU.exe
TestSuite\TestSuite_DSP.exe

See:
TestSuite\testinfo.txt for details about what scripts control what.

These are simple batch files that run our TestSuite as an script for
various playback tests.
TestSuite\test01.bat
TestSuite\test02.bat
TestSuite\test03.bat
TestSuite\test04.bat
TestSuite\test05.bat
TestSuite\test06.bat
TestSuite\test07.bat

When running, it will pause in the second window that is brought up. Just
select the second window and hit return to start the test. This is required
to make sure the DSP application is running first.
