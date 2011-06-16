/**********
     TITLE: Music Object Extension - Debug
   VERSION: 1.1.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x3"*/

function mo_debugMessage (_message,_URL,_lineNo) {
	var _debugStr = '\n**** DEBUG MESSAGE ****\n    ' + _message + ((typeof _lineNo == 'number') ? ('\n    LINE# ' + _lineNo) : '') + '\n    FILE: ' + ((typeof _URL != 'string') ? window.location.href : _URL) + '\n';
	if (Music.debugToJavaConsole && typeof java != 'undefined') java.lang.System.out.println (_debugStr);
	if (Music.debugToAlert) alert (_debugStr);
	if (Music.debugToStatus) window.defaultStatus = _debugStr;
	return true;
}

function MO_enableDebug (_windowHandle) {
	_windowHandle.onerror = mo_debugMessage;
	for (var _frameNo = 0; _frameNo < _windowHandle.frames.length; _frameNo++)
		MO_enableDebug (_windowHandle.frames [_frameNo])
	;
}

if (Music.clientName == 'Netscape' && !Music.meetsMinVersion (Music.clientVersion,'5')) MO_enableDebug (top);

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('debugMessage');

/*** Public Static Properties ***/

Music.debugToJavaConsole = true;
Music.debugToAlert = false;
Music.debugToStatus = true;
