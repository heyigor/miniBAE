------------------------------------------------------------------------------

	README.soundbank

	This file describes how to install a new MIDI soundbank
	for use by the Java Sound API software.

	Updated 25-Aug-99

------------------------------------------------------------------------------

The latest version of the Java Sound API allows MIDI synthesis to be
performed in software using a MIDI soundbank loaded into memory or in
hardware by passing the MIDI sequences to a MIDI sound card.

The hardware option is only available for the Windows platform.
However, the software option can use any of the soundbanks listed below.

NOTE - THE JAVA 2 RUNTIME ENVIRONMENT, STANDARD EDITION, V. 1.3 BETA
DOES NOT INCLUDE A SOFTWARE SOUNDBANK. YOU WILL NEED TO MANUALLY ADD
IT FOLLOWING THE DIRECTIONS HERE IF YOU WANT TO PLAY MIDI CONTENT WITH
YOUR J2RE INSTALLATION.

There are three soundbanks which you can install on your system;
they can be downloaded from 

 	 http://java.sun.com/products/java-media/sound

Minimal: soundbank-min.gm, 0.49 MB, sound quality = ok

	 This is the smallest soundbank you can use, with only
	 slightly less quality than the midsize soundbank. This is the
	 default soundbank shipped with Java 2 SDK, Standard Edition,
	 version 1.3 Beta.

Midsize: soundbank-mid.gm, 1.15 MB, sound quality = better than ok

	 This is the medium sized soundbank which gives you slightly
	 better quality than the minimal soundbank.  This is the
	 default soundbank shipped with Java 2 SDK, Standard Edition,
	 version 1.2, 1.2.1, 1.2.2.

Deluxe: soundbank-deluxe.gm, 5.14 MB, sound quality = best

	 A mammoth soundbank with the best quality sound samples.

Install Directions
------------------

1. If you installed the Java 2 SDK, change directories to 
   <install-dir>/jre/lib/audio *otherwise*

   If you installed the Java 2 RE, change directories to 
   <install-dir>/lib/audio*

   The <install-dir> should be where you installed your
   copy of the Java 2 SDK.

2. If you see a file here called "soundbank.gm", save
   it by renaming it to "soundbank.gm.orig"

2. Download the soundbank of your choice from the URL above
   to this directory.

3. Unzip the soundbank using the unzip utility on your system

4. Rename the downloaded soundbank to "soundbank.gm"

   NOTE - future versions of the Java Sound API software will look for
   the highest quality soundbank, so this renaming step will not be
   necessary for the future.

5. Exit all Java applications. At this point, when you start up
   a Java application or applet, it should use the new soundbank.

