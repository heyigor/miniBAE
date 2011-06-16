/**********
     TITLE: Music Object Language Module - U.S. English
   VERSION: 1.1.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_l0"*/

function MO_upgradePrompt () {
	Music.promptUser ('Please Upgrade Beatnik','This page has been optimized for the features of version <B>' + Music.requiredMinVersion + ' (or higher)</B> of the Beatnik Player. The currently installed version in your browser is ' + Music.playerVersion + '.<P>If you choose to IGNORE this message, the page will continue to load normally, but may not function properly as designed by the author.','UPGRADE BEATNIK >>>','Music.installPlayer ()','IGNORE','Music.promptClose ()');
}

function MO_installPrompt () {
	Music.promptUser ('Beatnik Enhanced Content !!','This page has been optimized for the audio features of the <B>Beatnik Player</B>. It appears you do not have the Beatnik Player installed.<P>If you choose to IGNORE this message, the page will continue to load normally, except there will be no Beatnik audio.','INSTALL BEATNIK >>>','Music.installPlayer ()','IGNORE','Music.promptClose ()');
}

function MO_enableJavaPrompt () {
	Music.promptUser ('Please Enable Java','This page makes use of the interactive audio features of the Beatnik Player. Java is currently not enabled in your browser. In order for the page to function correctly with Beatnik, <B>you must have Java enabled</B>.<P>This page will continue to load normally, but some interactive audio functionality may be absent.','    OK    ','Music.promptClose ()');
}

Music.upgradePrompt = MO_upgradePrompt;
Music.installPrompt = MO_installPrompt;
Music.enableJavaPrompt = MO_enableJavaPrompt;
Music.upgradePlayerText = 'Upgrade Beatnik';
Music.getPlayerText = 'Get Beatnik';
Music.installerOptions.language = 'english';
