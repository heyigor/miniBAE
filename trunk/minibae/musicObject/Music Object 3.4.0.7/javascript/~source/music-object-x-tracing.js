/**********
     TITLE: Music Object Extension - Tracing
   VERSION: 1.0.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_xh"*/

function MO_openConsole () {
	if (typeof Music.MO_consoleWindow == 'undefined') {
		Music.MO_consoleWindow = Music.openWindow ('','consoleWindow',990,320,false,'scrollbars=yes');
		if (typeof Music.MO_consoleWindow.alreadyOpen != 'boolean') {
			Music.MO_consoleWindow.alreadyOpen = true;
			Music.MO_consoleWindow.document.open ('text/plain');
		}
	}
}

function MO_tracingMessage (_message) {
	if (Music.tracingEnabled) {
		if (!Music.MO_tracingStarted) {
			Music.MO_tracingStarted = true;
			if (!Music.MO_traceToJavaConsole) MO_openConsole ();
			MO_tracingMessage ('\nSTARTING TRACE SESSION:\n=======================');
		}
		if (Music.MO_traceToJavaConsole) {
			java.lang.System.out.println (_message);
		} else {
			Music.MO_consoleWindow.document.writeln (_message);
			Music.MO_consoleWindow.scrollBy (0,100);
		}
	}
}

function MO_escapeStr (_sourceStr,_enclosingQuote) {
	if (typeof _enclosingQuote != 'string') _enclosingQuote = '';
	var _result = '';
	for (var _charNo = 0; _charNo < _sourceStr.length; _charNo++) {
		var _currentChar = _sourceStr.charAt (_charNo);
		if (((_currentChar == "'" || _currentChar == '"') && (_currentChar == _enclosingQuote || _enclosingQuote == '')) || _currentChar == '\\') _result += '\\';
		_result += _currentChar;
	}
	return _enclosingQuote + _result + _enclosingQuote;
}

function MO_methodTracer () {
	var
		_argsStr = '',
		_owner = (methodType == 'STATIC') ? Music : this,
		_ownerName = (methodType == 'STATIC') ? 'Music' : this.objectName
	;
	_owner.MO_tracingNestLevel++;
	if (Music.tracingEnabled && _owner.MO_tracingNestLevel == 1) {
		var _argsDisplayStr = '';
		for (var _argNo = 0; _argNo < arguments.length; _argNo++) {
			var _arg = arguments [_argNo];
			if (typeof _arg == 'string') {
				_arg = MO_escapeStr (_arg,"'");
			} else if (typeof _arg == 'function') {
				_arg = _arg.toString ();
				_arg = _arg.substring (_arg.indexOf (' ') + 1,_arg.indexOf ('('));
			}
			_argsDisplayStr += ((_argsDisplayStr != '') ? ',' : '') + _arg;
		}
		MO_tracingMessage (methodType + ' METHOD: ' + _ownerName + '.' + methodName + ' (' + _argsDisplayStr + ');' + ((typeof result != 'undefined') ? ('   RETURNED...   TYPE: ' + typeof result + '   VALUE: ' + result) : ''));
	}
	for (var _argNo = 0; _argNo < arguments.length; _argNo++)
		_argsStr += ((_argsStr != '') ? ',' : '') + 'arguments [' + _argNo + ']'
	;
	var _result = eval (_ownerName + '.method_' + methodName + ' (' + _argsStr + ')');
	_owner.MO_tracingNestLevel--;
	return _result;
}

function MO_createMethodTracer (_methodType,_methodName) {
	Music [_methodType + 'MethodTracer_' + _methodName] = new Function ('var methodName = \'' + _methodName + '\'; var methodType = \'' + _methodType.toUpperCase () + '\';\n' + Music.MO_methodTracer);
}

function mo_setMethodTracing (_method,_tracingOn) {
	var
		_isInstance = Music.isInstance (this),
		_owner = _isInstance ? this : Music,
		_methodType = _isInstance ? 'instance' : 'static'
	;
	if (typeof _tracingOn != 'boolean' || _tracingOn) {
		if (typeof Music [_methodType + 'MethodTracer_' + _method] == 'undefined') MO_createMethodTracer (_methodType,_method);
		if (typeof _owner ['method_' + _method] == 'undefined') _owner ['method_' + _method] = _owner [_method];
		_owner [_method] = Music [_methodType + 'MethodTracer_' + _method];
	} else {
		Music.wireMethod (_owner,_method);
	}
}

function mo_enableTracing (_tracingOn) {
	var
		_instance = Music.isInstance (this),
		_owner = _instance ? this : Music,
		_methods = Music ['public' + (_instance ? 'Instance' : 'Static') + 'Methods']
	;
	for (var _methodNo = 0; _methodNo < _methods.length; _methodNo++)
		_owner.setMethodTracing (_methods [_methodNo],_tracingOn)
	;
}

function MO_tracingInstanceExtender (_this) {
	_this.MO_tracingNestLevel = 0;
	if (Music.tracingEnabled) _this.enableTracing ();
}

Music.addInstanceExtender (MO_tracingInstanceExtender);

/*** Special Static Methods for Tracing (neither truly public nor truly private) ***/

Music.enableTracing = mo_enableTracing;
Music.setMethodTracing = mo_setMethodTracing;

/*** Private Instance Methods ***/

Music.addPrivateInstanceMethods ('enableTracing','setMethodTracing');

/*** Public Static Properties ***/

Music.preferJavaConsole = true;
Music.tracingEnabled = true;

/*** Private Static Properties ***/

Music.MO_tracingNestLevel = 0;
Music.MO_tracingStarted = false;
Music.MO_traceToJavaConsole = typeof java != 'undefined' && Music.preferJavaConsole;
Music.MO_methodTracer = MO_methodTracer.toString ();
Music.MO_methodTracer = Music.MO_methodTracer.substring (Music.MO_methodTracer.indexOf ('{') + 1,Music.MO_methodTracer.lastIndexOf ('}'));

/*** Initialization ***/

if (Music.tracingEnabled) Music.enableTracing ();
