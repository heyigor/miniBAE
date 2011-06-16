/**********
     TITLE: Music Object Patch - Pre 3.2.0
   VERSION: 1.1.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.2.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_p2"*/

function mo_debug (_message,_URL,_lineNo) {
	var _debugStr = '\n**** DEBUG MESSAGE ****\n    ' + _message + ((typeof _lineNo == 'number') ? ('\n    LINE# ' + _lineNo) : '') + '\n    FILE: ' + ((typeof _URL != 'string') ? window.location.href : _URL) + '\n';
	if (Music.debugToJavaConsole && typeof java != 'undefined') java.lang.System.out.println (_debugStr);
	if (Music.debugToAlert) alert (_debugStr);
	if (Music.debugToStatus) window.defaultStatus = _debugStr;
	return true;
}

function MO_enableDebug (_windowHandle) {
	_windowHandle.onerror = mo_debug;
	for (var _frameNo = 0; _frameNo < _windowHandle.frames.length; _frameNo++)
		MO_enableDebug (_windowHandle.frames [_frameNo])
	;
}

MO_enableDebug (top);

if (typeof Music != 'undefined') {
	/*** Public Static Properties ***/
	Music.debugToJavaConsole = true;
	Music.debugToAlert = false;
	Music.debugToStatus = true;
}

/*** Public Instance Methods ***/

mo_addInstanceMethods ('engageAudio','getReverbType','setGlobalMute','setReverbType');
