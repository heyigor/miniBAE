/**********
     TITLE: Music Object
   VERSION: 3.4.0.7
      TYPE: Object Definition
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

/*ScruncherSettings Mappings=",MO"*/

/*Scruncher:Leave Next Comment*/
/**********
     TITLE: Music Object Component - Basic
   VERSION: 3.4.0.7
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

window.onerror = null;

function MO_isInstance (_object) {return typeof _object._playerRef != MO_undefined}
function MO_instanceOrStatic (_object) {return MO_isInstance (_object) ? _object : Music}

function MO_indexOf (_sourceStr,_searchStr,_startPos) {
	var _result = _sourceStr.indexOf (_searchStr,_startPos);
	return (_result != -1) ? _result : _sourceStr.length;
}

function MO_doNothing () {}
function MO_returnAsIs (_param) {return _param}
function MO_returnFalse () {return false}
function MO_timeInMilliseconds () {return (new Date ()).getTime ()}

function MO_pathFromTop () {
	var
		_result = '',
		_windowHandle = window
	;
	while (_windowHandle.parent != _windowHandle && _windowHandle.parent) {
		for (var _frameNo = 1; _frameNo <= _windowHandle.parent.frames.length; _frameNo++) {
			if (_windowHandle == _windowHandle.parent.frames [_frameNo - 1])
				_result = 'frames[' + (_frameNo - 1) + '].' + _result
			;
		}
		_windowHandle = _windowHandle.parent;
	}
	return 'top.' + _result;
}

function mo_on () {return this.ready && MO_Music.enabled && !this.locked}

function MO_addEventHandler (_windowHandle,_objectID,_eventType,_handler) {
	var _storedHandlerStr = '';
	if (typeof _objectID [_eventType] == 'function') {
		if (typeof _windowHandle.MO_storedHandlers == MO_undefined) _windowHandle.MO_storedHandlers = 0;
		var _storedHandler = 'mo_storedEventHandler' + ++_windowHandle.MO_storedHandlers;
		_windowHandle [_storedHandler] = _objectID [_eventType];
		_storedHandlerStr = '; return ' + _storedHandler + ' (event)';
	}
	_objectID [_eventType] = new _windowHandle.Function ('event',_handler + _storedHandlerStr);
}

function mo_play (_param1,_param2) {
	var _this = this;
	if (_this.on ()) {
		_this.endVolumeFade ();
		if (typeof _param2 == 'string') {
			if (_param2 != '' && _param2.indexOf ('.') == -1 && _param2.indexOf ('groovoid://') != 0) _param2 = 'groovoid://' + _param2;
			if (typeof _param1 == 'string') _param1 = (_param1 == 'auto') ? (_param2.indexOf ('groovoid://Background-') == 0) : (_param1 == 'yes');
			_this._playerRef.play (_param1,_param2);
		} else if (typeof _param1 == 'boolean' || typeof _param1 == 'number') {
			_this._playerRef.play (_param1);
		} else if (typeof _param1 == 'string') {
			_this.play ('auto',_param1);
		} else {
			if (!_this.isPlaying () || MO_Music._playerIsAtLeast220) _this._playerRef.play ();
		}
	}
}

function mo_stop (_fade) {
	var _this = this;
	if (_this.on ()) {
		if (typeof _fade == MO_undefined || _fade == 0 || !MO_Music._canFade || _this.isPaused ()) {
			_this.endVolumeFade ();
			_this._playerRef.stop ();
		} else {
			_this.fadeVolume (null,0,_fade,_this.objectName + '.stop ()');
		}
	}
}

function mo_pause (_fade) {
	var _this = this;
	if (_this.on ()) {
		_this.endVolumeFade ();
		if (_this.isPaused ()) {
			_this._playerRef.pause ();
			if (typeof _fade != MO_undefined && MO_Music._canFade) _this.fadeVolume (0,100,_fade);
		} else {
			(typeof _fade == MO_undefined || !MO_Music._canFade) ? _this._playerRef.pause () : _this.fadeVolume (null,0,_fade,_this.objectName + '.pause ()');
		}
	}
}

function mo_enable () {
	var
		_this = this,
		_playerName = _this._playerName,
		_document = document
	;
	if (_this._playerRef == null) _this._playerRef = (typeof window [_playerName] == 'object') ? window [_playerName] : ((typeof _document [_playerName] == 'object') ? _document [_playerName] : ((typeof _document.embeds != MO_undefined && typeof _document.embeds [_playerName] == 'object') ? _document.embeds [_playerName] : ((typeof _document [_playerName + 'Layer'] != MO_undefined && typeof _document [_playerName + 'Layer'].document.embeds [0] == 'object') ? _document [_playerName + 'Layer'].document.embeds [0] : _this._playerRef)));
	if (_this._playerRef != null && (!MO_Music._isMacIE || typeof _this._playerRef.scriptable != MO_undefined)) {
		if (MO_Music.playerVersion == '') MO_Music._setPlayerVersion (_this._playerRef.getVersion ());
		MO_Music._hasCallbackIssue ? _this._playerRef.enableCallbacks (true) : _this.execOnReady ();
	} else {
		setTimeout (_this.objectName + '.enable ()',500);
	}
}

function mo_execOnReady () {
	var _this = this;
	if (_this.ready) {
		if (MO_Music.playerType == 'ActiveX') _this._playerRef.receivedReady (true);
	} else {
		_this.ready = _this._playerRef != null;
		if (_this.ready) {
			_this.updateMetaEventsEnabled ();
			if (MO_Music.global == null) MO_Music.global = _this._playerRef;
			if (MO_Music.playerType == 'ActiveX') _this._playerRef.receivedReady (true);
			if (_this == MO_Music._instanceToHide) {
				var _layerName = _this._playerName + 'Layer';
				if (typeof document [_layerName] != MO_undefined) {
					document [_layerName].visibility = false;
				} else if (typeof document.getElementById != MO_undefined && document.getElementById (_layerName) != null) {
					document.getElementById (_layerName).style.visibility = 'hidden';
				}
			}
			if (_this._delayAutostart) {
				_this._playerRef.setAutostart (true);
				_this.play ();
			}
			_this.execCallback ('onReady');
		} else {
			_this.enable ();
		}
	}
	if (_this.ready) _this.execCallback ('onLoad');
}

function mo_execOnPlay () {
	this.setVolume (this.VOLUME);
	this.updateAudibleState ();
	this.execCallback ('onPlay');
}

function mo_getOutputVolume () {
	var
		_this = this,
		_lastOutputVolume = _this._outputVolume
	;
	_this._outputVolume = _this.VOLUME;
	for (var _volScalar in _this.volumeScalars) _this._outputVolume *= _this.volumeScalars [_volScalar] / 100;
	_this._outputVolume = Math.round (_this._outputVolume);
	if (_this._outputVolume != _lastOutputVolume) _this.execCallback ('onOutputVolumeChange');
	return _this._outputVolume;
}

function mo_setVolume (_param1,_param2) {
	var _this = this;
	if (typeof _param2 != MO_undefined) {
		if (_param2 < 0) delete _this.volumeScalars [_param1]; else _this.volumeScalars [_param1] = _param2;
	} else if (typeof _param1 != MO_undefined) {
		_this.VOLUME = _param1;
	}
	_this.getOutputVolume ();
	if (_this.on ()) _this._playerRef.setVolume (_this._outputVolume);
}

function mo_getVolume (_volumeScalarName) {
	return (typeof _volumeScalarName == 'string') ? ((typeof this.volumeScalars [_volumeScalarName] != MO_undefined) ? this.volumeScalars [_volumeScalarName] : null) : this.VOLUME;
}

function MO_layoutAttributes (_this) {
	return MO_tagAttr (_this,'WIDTH') + MO_tagAttr (_this,'HEIGHT') + MO_tagAttr (_this,'HSPACE') + MO_tagAttr (_this,'VSPACE') + MO_tagAttr (_this,'ALIGN');
}

function MO_parseAttributes (_attrList,_this) {
	var
		_endFound = false,
		_attrStartPos = 0
	;
	while (!_endFound && _attrStartPos < _attrList.length) {
		var _attrFound = false;
		while (!_attrFound && _attrStartPos < _attrList.length) {
			_attrFound = _attrList.charAt (_attrStartPos) != ' ';
			if (!_attrFound) _attrStartPos++;
		}
		if (_attrFound) {
			var
				_equalPos = MO_indexOf (_attrList,'=',_attrStartPos),
				_spacePos = MO_indexOf (_attrList,' ',_attrStartPos),
				_closePos = MO_indexOf (_attrList,'>',_attrStartPos),
				_attrNameEndPos = Math.min (Math.min (_spacePos,_equalPos),_closePos),
				_hasValue = _attrNameEndPos != _spacePos && _attrNameEndPos != _closePos,
				_attrName = _attrList.substring (_attrStartPos,_attrNameEndPos).toUpperCase (),
				_attrValueEndPos = _attrNameEndPos,
				_attrValue = ''
			;
			_endFound = _closePos == _attrNameEndPos;
			if (_hasValue) {
				var
					_attrValuePos = _attrNameEndPos + 1,
					_quoteChar = _attrList.charAt (_attrValuePos)
				;
				if (_quoteChar == '"' || _quoteChar == "'") {
					_attrValueEndPos = _attrValuePos;
					var _attrValueEndFound = false;
					while (!_attrValueEndFound && _attrValueEndPos < _attrList.length - 1) {
						_attrValueEndPos = MO_indexOf (_attrList,_quoteChar,_attrValueEndPos + 1);
						_attrValueEndFound = true;
						var
							_checkingEscape = true,
							_charPos = _attrValueEndPos
						;
						while (_checkingEscape) {
							_charPos--;
							_checkingEscape = _attrList.charAt (_charPos) == '\\';
							if (_checkingEscape) _attrValueEndFound = !_attrValueEndFound;
						}
					}
					_attrValue = eval (_attrList.substring (_attrValuePos,_attrValueEndPos + 1));
				} else {
					_attrValueEndPos = Math.min (MO_indexOf (_attrList,' ',_attrValuePos),MO_indexOf (_attrList,'>',_attrValuePos));
					_attrValue = _attrList.substring (_attrValuePos,_attrValueEndPos);
				}
			}
			_attrStartPos = _attrValueEndPos + 1;
			_this [_attrName] = _attrValue;
		}
	}
}

function MO_tagAttr (_this,_attrName) {
	var _attrValue = _this [_attrName];
	return (_attrValue != null) ? (' ' + _attrName + (((_attrValue + '') != '') ? ('="' + _attrValue + '"') : '')) : '';
}

function mo_killReferences () {
	var _instances = MO_Music.instances;
	for (var _instanceNo = _instances.length - 1; _instanceNo >= 0; _instanceNo--) {
		_instances [_instanceNo].ready = false;
		_instances [_instanceNo]._playerRef = null;
	}
	MO_Music.global = null;
}

function mo_magicEmbed (_attrList) {
	var
		_Music = MO_Music,
		_isInstance = MO_isInstance (this),
		_this = _isInstance ? this : _Music,
		_embedDefaults = _Music.embedDefaults,
		_attrNames = _Music.embedProperties
	;
	if (typeof _Music.playerCompatible != 'boolean') mo_isPlayerCompatible ();
	if (typeof _this._playerName != 'string' || !_isInstance) {
		for (var _attrNo = 0; _attrNo < _attrNames.length; _attrNo++) {
			_this [_attrNames [_attrNo]] = (typeof _embedDefaults [_attrNames [_attrNo]] != MO_undefined) ? _embedDefaults [_attrNames [_attrNo]] : null;
		}
		MO_parseAttributes (_attrList,_this);
		if (_this.HIDDEN == '') _this.HIDDEN = 'TRUE';
		_this._hidden = _this.HIDDEN != null && _this.HIDDEN.toUpperCase () == 'TRUE';
		_this._hideInLayer = _this._hidden && _this.WIDTH <= '2' && _this.HEIGHT <= '2' && _Music.clientPlatform != 'WebTV';
		_this.BGCOLOR = document.bgColor;
		_this.MUSICOBJECTVERSION = _Music.version;
		var
			_panelW = _this.WIDTH - 0,
			_panelH = _this.HEIGHT - 0,
			_showAnyway = false,
			_embedOutput = ''
		;
		if (_Music.enabled && _Music.clientSupported && _Music.hasPlayer && (_Music.playerCompatible || !_Music.silentIfInadequate)) {
			if (_Music._showAtLeastOneInstance && !_Music._someInstancesShown && _this._hidden) {
				_this.HIDDEN = 'FALSE';
				if (_this._hideInLayer) {
					if (_isInstance) _Music._instanceToHide = _this;
					_showAnyway = true;
					_this.WIDTH = _this.HEIGHT = 2;
				}
			}
			_Music._someInstancesShown = true;
			if (_isInstance) {
				_this._playerName = _this.objectName + 'Player';
				var _prefix = _this.objectName + '.execOn';
				_this.ONREADY = _prefix + 'Ready()';
				_this.ONPLAY = _prefix + 'Play()';
				_this.ONPAUSE = _prefix + 'Pause()';
				_this.ONSTOP = _prefix + 'Stop()';
				_this.ONMETAEVENT = _prefix + 'MetaEvent()';
				_this.ONAUDIOENGAGEDCHANGE = 'Music.execCallback (\'onAudioEngagedChange\')';
				_this.METAEVENTS = 'FALSE';
				if (_Music._hasCallbackIssue) {
					_this.CALLBACKS = 'FALSE';
					_this._delayAutostart = _this.AUTOSTART.toUpperCase () == 'TRUE';
					_this.AUTOSTART = 'FALSE';
					if (!_Music._bodyEventsAdded) {
						_Music.windowOnloadStr = '';
						_Music._bodyEventsAdded = true;
						if (_Music.playerType == 'ActiveX') MO_addEventHandler (window,window,'onunload','mo_killReferences ()');
						MO_addEventHandler (window,window,'onload','eval (Music.windowOnloadStr)');
					}
					_Music.windowOnloadStr += _this.objectName + '.enable ();';
				}
				if (MO_Music._isMacIE) {
					_this.ID = _this.objectName + _this.DOCUMENTID;
					_this.WINDOWREF = MO_pathFromTop ();
				}
			}
			if (_Music.playerType == 'Plug-in') {
				_embedOutput += '<EMBED TYPE="' + (_Music._playerIsAtLeast210 ? 'application/beatnik' : 'audio/rmf') + '" PLUGINSPAGE="http://www.beatnik.com/to/?player"' + (_isInstance ? (' NAME="' + _this._playerName + '"') : '');
				for (var _attrNo = 0; _attrNo < _attrNames.length; _attrNo++)
					_embedOutput += MO_tagAttr (_this,_attrNames [_attrNo])
				;
				_embedOutput += '>';
				if (_isInstance && typeof MO_requireJava != MO_undefined) MO_requireJava ();
			} else if (_Music.playerType == 'ActiveX') {
				if (_isInstance) {
					var _callbacks = new Array ('OnReady','OnPlay','OnPause','OnStop','OnMetaEvent','OnAudioEngagedChange');
					for (var _callbackNo = 0; _callbackNo < _callbacks.length; _callbackNo++) {
						var
							_callbackParams = '(' + (_callbacks [_callbackNo] == 'OnMetaEvent' ? 'eventType,eventValue' : '') + ')',
							_callbackHandler = _this [_callbacks [_callbackNo].toUpperCase ()]
						;
						_embedOutput += '<SCRIPT LANGUAGE=JavaScript FOR="' + _this._playerName + '" EVENT="' + _callbacks [_callbackNo] + _callbackParams + '">' + _callbackHandler.substring (0,_callbackHandler.indexOf ('(')) + _callbackParams + '</SCRIPT>\n';
					}
				}
				_embedOutput += '<OBJECT' + (_isInstance ? (' ID="' + _this._playerName + '"') : '') + MO_layoutAttributes (_this) + ' CLASSID="CLSID:B384F118-18EE-11D1-95C8-00A024330339" CODEBASE="http://download.beatnik.com/beatnik-player/beatnik-player.cab">\n';
				for (var _attrNo = 0; _attrNo < _attrNames.length; _attrNo++) {
					if (_this [_attrNames [_attrNo]] != null)
						_embedOutput += '<PARAM NAME="' + _attrNames [_attrNo] + '" VALUE="' + _this [_attrNames [_attrNo]] + '">\n'
					;
				}
				_embedOutput += '</OBJECT>';
			}
			if (_this._hideInLayer) {
				if (_Music.playerType == 'Plug-in' && typeof Layer != MO_undefined) {
					_embedOutput = '<LAYER' + (_isInstance ? (' NAME="' + _this._playerName + 'Layer"') : '') + ' LEFT=0 TOP=0' + (!_showAnyway ? ' VISIBILITY=HIDDEN' : '') + '>' + _embedOutput + '</LAYER>';
				} else {
					_embedOutput = '<SPAN ' + (_isInstance ? (' ID="' + _this._playerName + 'Layer"') : '') + 'STYLE="position:absolute;left:0;top:0;' + (!_showAnyway ? 'visibility:hidden;' : '') + '">' + _embedOutput + '</SPAN>';
				}
			}
		} else {
			if (typeof MO_playerSubstitute != MO_undefined) _embedOutput = MO_playerSubstitute (_this);
		}
		document.write (_embedOutput);
		_this.VOLUME = _this.VOLUME - 0;
	}
}

function MO_wireMethod (_this,_method) {
	_this [_method] = (typeof window ['mo_' + _method] != 'function') ? MO_doNothing : window ['mo_' + _method];
}

function MO_appendToArray (_arrayToAppendTo,_arrayToAppend) {
	for (var _elementNo = 0; _elementNo < _arrayToAppend.length; _elementNo++) {
		_arrayToAppendTo [_arrayToAppendTo.length] = _arrayToAppend [_elementNo];
	}
}

function MO_addPublicStaticMethods () {
	var
		_args = MO_addPublicStaticMethods.arguments,
		_publicStaticMethods = MO_Music.publicStaticMethods
	;
	for (var _argNo = 0; _argNo < _args.length; _argNo++)
		MO_wireMethod (MO_Music,_publicStaticMethods [_publicStaticMethods.length] = _args [_argNo])
	;
}

function MO_addPublicInstanceMethods () {
	MO_appendToArray (MO_Music.publicInstanceMethods,MO_addPublicInstanceMethods.arguments);
}

function MO_addPrivateInstanceMethods () {
	MO_appendToArray (MO_Music.privateInstanceMethods,MO_addPrivateInstanceMethods.arguments);
}

/*** Music Object Constructor ***/

function Music (_objectName) {
	var
		_this = this,
		_methods
	;
	/*** Instance Properties ***/
	_this._delayAutostart = _this.ready = _this.locked = false;
	_this.objectName = _this.alias = (typeof _objectName == 'string') ? _objectName : ('mo_GIN' + MO_Music._generatedInstanceNames++);
	_this.VOLUME = _this._outputVolume = 100;
	_this.volumeScalars = new Array ();
	_this._playerRef = null;

	/*** Initialisation ***/
	_methods = MO_Music.publicInstanceMethods;
	for (var _methodNo = 0; _methodNo < _methods.length; _methodNo++)
		MO_wireMethod (_this,_methods [_methodNo])
	;
	_methods = MO_Music.privateInstanceMethods;
	for (var _methodNo = 0; _methodNo < _methods.length; _methodNo++)
		MO_wireMethod (_this,_methods [_methodNo])
	;

	window [_this.objectName] = MO_Music.instances [MO_Music.instances.length] = _this;
	_this.extend ();
}

MO_Music = Music;

/*** Placeholder Compatibility Methods ***/

MO_Music.retrieveVersion = MO_returnAsIs;
MO_Music.hasMinVersion = MO_returnFalse;
MO_Music._setPlayerVersion = MO_doNothing;

/*** Placeholder Compatibility Properties ***/

MO_Music.playerVersion = '';

/*** Constants ***/

MO_navigator = navigator;
MO_undefined = 'undefined';
MO_MSIE = 'Microsoft Internet Explorer';

/*** Public Static Properties (read only) ***/

MO_Music.clientSupported = MO_Music.hasPlayer = MO_Music.playerCompatible = true;
MO_Music.playerType = (MO_navigator.appName == MO_MSIE && MO_navigator.platform.indexOf ('Win') != -1) ? 'ActiveX' : 'Plug-in';

/*** Public Static Properties ***/

MO_Music.enabled = true;
MO_Music.version = '3.4.0.7';
MO_Music.instances = new Array ();
MO_Music.publicInstanceMethods = new Array ();
MO_Music.privateInstanceMethods = new Array ();
MO_Music.publicStaticMethods = new Array ();
MO_Music.embedProperties = new Array ('SRC','WIDTH','HEIGHT','AUTOSTART','LOOP','VOLUME','FILEVOLUMESCALING','ALIGN','HSPACE','VSPACE','BGCOLOR','HIDDEN','DISPLAY','MODE','GROOVOID','ONREADY','ONPLAY','ONPAUSE','ONSTOP','ONMETAEVENT','CALLBACKS','METAEVENTS','MUSICOBJECTVERSION','AUDIODEVICEPRIORITY','DOCUMENTID','ID','WINDOWREF','AUDIOENGAGED','ONAUDIOENGAGEDCHANGE');
var MO_embedDefaults = MO_Music.embedDefaults = new Array ();
MO_embedDefaults.AUTOSTART = MO_embedDefaults.FILEVOLUMESCALING = MO_embedDefaults.AUDIOENGAGED = 'TRUE';
MO_embedDefaults.DOCUMENTID = MO_timeInMilliseconds () + Math.round (Math.random () * 1000);
MO_embedDefaults.WIDTH = '144';
MO_embedDefaults.HEIGHT = '60';
MO_embedDefaults.HSPACE = MO_embedDefaults.VSPACE = '0';
MO_embedDefaults.VOLUME = '100';
MO_embedDefaults.AUDIODEVICEPRIORITY = '1';

/*** Private Static Properties ***/

MO_Music._playerIsAtLeast210 = MO_Music._playerIsAtLeast220 = MO_Music._bodyEventsAdded = MO_Music._canFade = MO_Music._someInstancesShown = false;
MO_Music._generatedInstanceNames = 0;
MO_Music._hasCallbackIssue = MO_navigator.userAgent.indexOf ('WebTV') == -1;
MO_Music._instanceToHide = null;
MO_Music._isMacIE = MO_navigator.appName == MO_MSIE && MO_navigator.platform == 'MacPPC';
MO_Music._showAtLeastOneInstance = MO_navigator.appName == 'Netscape' && MO_navigator.appVersion >= '5' && MO_navigator.platform == 'Win32';

/*** Public Instance Methods ***/

MO_addPublicInstanceMethods ('endVolumeFade','getOutputVolume','magicEmbed','pause','play','setVolume','stop');

/*** Private Instance Methods ***/

MO_addPrivateInstanceMethods ('enable','execCallback','execOnPause','execOnPlay','execOnReady','execOnStop','extend','on','updateAudibleState','updateMetaEventsEnabled');

/*** Public Static Methods ***/

MO_addPublicStaticMethods ('magicEmbed');

/*Scruncher:Leave Next Comment*/
/**********
     TITLE: Music Object Component - Advanced
   VERSION: 3.4.0.7
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: Music Object Component - Basic
**********/

function MO_arrayOfObjects (_objectsToCreate,_objectConstructor) {
	var _result = new Array ();
	for (var _objectNo = 0; _objectNo < _objectsToCreate; _objectNo++)
		_result [_objectNo] = (typeof _objectConstructor == 'function') ? new _objectConstructor () : eval (_objectConstructor)
	;
	return _result;
}

function mo_extend () {
	for (var _extenderNo = 0; _extenderNo < MO_Music._extenders.length; _extenderNo++)
		MO_Music._extenders [_extenderNo] (this)
	;
}

function MO_addInstanceExtender (_extender) {
	MO_Music._extenders [MO_Music._extenders.length] = _extender;
}

function MO_addPublicCallbacks () {
	var
		_args = MO_addPublicCallbacks.arguments,
		_isInstanceCallback = true,
		_callbacks = MO_Music.publicInstanceCallbacks
	;
	for (var _argNo = 0; _argNo < _args.length; _argNo++) {
		var _callback = _args [_argNo];
		if (_callback.charAt (0) == '[') {
			_isInstanceCallback = _callback.toLowerCase ().indexOf ('instance') != -1;
			_callbacks = MO_Music ['public' + (_isInstanceCallback ? 'Instance' : 'Static') + 'Callbacks'];
		} else {
			_callbacks [_callbacks.length] = _callback;
			window ['mo_' + _callback] = new Function ('handler',(_isInstanceCallback ? 'this.' : 'Music.') + _callback + 'Handler = handler;' + ((_isInstanceCallback && _callback == 'onMetaEvent') ? 'this.updateMetaEventsEnabled ();' : ''));
			_isInstanceCallback ? MO_addPublicInstanceMethods (_callback) : MO_addPublicStaticMethods (_callback);
		}
	}
}

function mo_updateMetaEventsEnabled () {
	if (this.ready) this._playerRef.enableMetaEvents (typeof this.onMetaEventHandler == 'function' || (typeof this.onMetaEventHandlers != MO_undefined && this.onMetaEventHandlers.length > 0));
}

function mo_addCallbackHandler (_callbackType,_handler) {
	var
		_this = MO_instanceOrStatic (this),
		_handlersID = _callbackType + 'Handlers'
	;
	if (typeof _this [_handlersID] == MO_undefined) _this [_handlersID] = new Array ();
	_this [_handlersID] [_this [_handlersID].length] = _handler;
	if (_callbackType == 'onMetaEvent') _this.updateMetaEventsEnabled ();
}

function MO_execHandler (_handler) {
	if (typeof _handler == 'string') eval (_handler);
		else if (typeof _handler == 'function') _handler ();
}

function mo_execCallback (_callbackType) {
	var _this = MO_instanceOrStatic (this);
	if (typeof _this [_callbackType + 'Handler'] != MO_undefined) MO_execHandler (_this [_callbackType + 'Handler']);
	if (typeof _this [_callbackType + 'Handlers'] != MO_undefined) {
		var _handlers = _this [_callbackType + 'Handlers'];
		for (var _handlerNo = 0; _handlerNo < _handlers.length; _handlerNo++)
			MO_execHandler (_handlers [_handlerNo])
		;
	}
}

function mo_updateAudibleState () {
	var _this = this;
	MO_clearTimeout (_this,'updateAudibleStateTimeout');
	var _lastAudible = _this.audible;
	_this.audible = ((_this.usage != 'notes' && _this.isPlaying ()) || _this._notesPlaying > 0) && _this._outputVolume > 0;
	if (_this.audible != _lastAudible) _this.execCallback ('onAudibleStateChange');
}

function mo_execOnMetaEvent (_eventType,_eventValue) {
	if (typeof this.onMetaEventHandler == 'function') this.onMetaEventHandler (_eventType,_eventValue,this);
	this.execMetaEventHandlers (_eventType,_eventValue);
}

function mo_execOnPause () {
	this.endVolumeFade ();
	this.updateAudibleState ();
	this.execCallback ('onPause');
}

function mo_execOnStop () {
	this.endVolumeFade ();
	this.updateAudibleState ();
	this.execCallback ('onStop');
}

function mo_stopAll () {
	for (var _instanceNo = 0; _instanceNo < MO_Music.instances.length; _instanceNo++)
		MO_Music.instances [_instanceNo].stop ()
	;
}

function MO_Fade_setValue (_value) {
	var _this = this;
	if (Math.round (_value) != Math.round (_this._currentValue)) {
		_this.fadeValue = Math.round (_value);
		if (typeof _this._updateHandler == 'function') _this._updateHandler (_this.fadeValue);
	}
	_this._currentValue = _value;
}

function MO_Fade_end () {
	var _this = this;
	if (_this.inProgress) {
		_this.inProgress = false;
		MO_execHandler (_this._endHandler);
		if (_this._restoreStartValue) _this.setValue (_this._startValue);
	}
}

function MO_Fade_advance () {
	var _this = this;
	if (_this.inProgress) {
		_this.setValue (_this._currentValue + _this._step);
		_this.timeElapsed += MO_Music.minFadeInterval;
		if (_this.timeElapsed >= _this._fadeTime) _this.end ();
	}
	return _this.inProgress;
}

function mo_execFades () {
	MO_Music._fadeTimeout = setTimeout ('mo_execFades ()',MO_Music.minFadeInterval);
	var
		_deadFades = 0,
		_fadeNo = 0
	;
	while (_fadeNo < MO_Music._fadeQueue.length - _deadFades) {
		MO_Music._fadeQueue [_fadeNo] = MO_Music._fadeQueue [_fadeNo + _deadFades];
		(MO_Music._fadeQueue [_fadeNo].advance ()) ? _fadeNo++ : _deadFades++;
	}
	MO_Music._fadeQueue.length -= _deadFades;
	if (typeof MO_Music.onFadesUpdatedHandler != MO_undefined) MO_execHandler (MO_Music.onFadesUpdatedHandler);
	if (MO_Music._fadeQueue.length == 0) clearTimeout (MO_Music._fadeTimeout);
}

function MO_Fade_start () {
	var _this = this;
	if (!_this.inProgress) {
		_this.inProgress = true;
		if (_this._fadeTime == 0) {
			if (!_this._restoreStartValue) _this.setValue (_this._endValue);
			_this.end ();
		} else {
			_this.timeElapsed = 0;
			_this.setValue (_this._startValue);
			MO_Music._fadeQueue [MO_Music._fadeQueue.length] = _this;
			if (MO_Music._fadeQueue.length == 1) setTimeout ('mo_execFades ()',MO_Music.minFadeInterval);
		}
	}
}

function MO_Fade_update (_startValue,_endValue,_fadeTime,_updateHandler,_endHandler,_restoreStartValue) {
	var _this = this;
	if (typeof _endValue != 'number') _endValue = 0;
	if (typeof _fadeTime == 'boolean') _fadeTime = _fadeTime ? 1000 : 0;
	if (typeof _fadeTime != 'number') _fadeTime = 1000;
	if (typeof _restoreStartValue != 'boolean') _restoreStartValue = false;
	_updateHandler = (typeof _updateHandler == 'string') ? new Function ('fadeValue',_updateHandler) : _updateHandler;
	if (
		_this.inProgress &&
		_this._endValue == _endValue &&
		_this._fadeTime == _fadeTime
	) {
		_this._endHandler = _endHandler;
		_this._updateHandler = _updateHandler;
	} else {
		var _wasInProgress = _this.inProgress;
		_this.end ();
		_this._restoreStartValue = _restoreStartValue;
		_this._startValue = _startValue;
		_this._endValue = _endValue;
		_this._fadeTime = Math.round (_fadeTime / MO_Music.minFadeInterval) * MO_Music.minFadeInterval;
		_this._endHandler = _endHandler;
		_this._updateHandler = _updateHandler;
		_this._currentValue = -1000;
		_this._step = (_endValue - _startValue) / (_this._fadeTime / MO_Music.minFadeInterval);
		if (_wasInProgress) _this.start ();
	}
}

function MO_Fade (_startValue,_endValue,_fadeTime,_updateHandler,_endHandler,_restoreStartValue) {
	var _this = this;
	/*** Public Instance Properties ***/
	_this.inProgress = false;
	_this.fadeValue = _startValue;

	/*** Public Instance Methods ***/
	_this.start = MO_Fade_start;
	_this.end = MO_Fade_end;
	_this.advance = MO_Fade_advance;
	_this.setValue = MO_Fade_setValue;
	_this.update = MO_Fade_update;

	/*** Initialization ***/
	if (MO_Fade.arguments.length > 0)
		_this.update (_startValue,_endValue,_fadeTime,_updateHandler,_endHandler,_restoreStartValue)
	;
}

function mo_endVolumeFade () {this._volumeFade.end ()}

function mo_fadeVolume (_fromVolume,_toVolume,_fadeTime,_fadeEndHandler,_volumeChangeHandler) {
	if (typeof _volumeChangeHandler != MO_undefined) this.onOutputVolumeChangeHandler = _volumeChangeHandler;
	this._volumeFade.update ((typeof _fromVolume != 'number') ? this.VOLUME : _fromVolume,_toVolume,_fadeTime,this.objectName + '.setVolume (fadeValue)',_fadeEndHandler,typeof _fadeEndHandler == 'string' && (_fadeEndHandler.indexOf ('.stop ()') != -1 || _fadeEndHandler.indexOf ('.pause ()') != -1));
	this._volumeFade.start ();
}

function mo_fadeTo (_toValue,_fadeTime,_fadeEndCallback,_fadeAdvanceCallback) {
	this.fadeVolume (null,_toValue,_fadeTime,_fadeEndCallback,_fadeAdvanceCallback);
}

function mo_setMonophonic (_channelNo,_state) {
	if (_channelNo == 0)
		for (var _channelCount = 1; _channelCount < 17; _channelCount++) this._monophonic [_channelCount] = _state;
		else this._monophonic [_channelNo] = _state;
}

function mo_getMonophonic (_channelNo) {return this._monophonic [_channelNo]}

function mo_noteOn (_channelNo,_param2,_param3,_param4,_param5,_param6) {
	var _this = this;
	if (_this.on ()) {
		var
			_voiceNo,
			_voices = MO_Music._voices,
			_newVoice = MO_Music._newVoice
		;
		if (mo_noteOn.arguments.length >= 5) {
			if (_this._monophonic [_channelNo]) _this.noteOff (_channelNo);
			if (typeof _param4 == 'string') _param4 = mo_getNoteNumber (_param4);
			_newVoice._musicObject = _this;
			_newVoice._timeStamp = ++MO_Music._globalNoteNo;
			_newVoice._channelNo = _channelNo;
			_newVoice._noteNo = _param4;
			var _assignToVoiceNo = -1;
			for (_voiceNo = 0; _voiceNo < MO_Music._polyphony; _voiceNo++) {
				if (_voices [_voiceNo]._channelNo == 0) {
					_assignToVoiceNo = _voiceNo;
					break;
				}
			}
			if (_assignToVoiceNo == -1) {
				_assignToVoiceNo = 0;
				for (_voiceNo = 0; _voiceNo < MO_Music._polyphony; _voiceNo++) {
					if (_voices [_voiceNo]._timeStamp < _voices [_assignToVoiceNo]._timeStamp)
						_assignToVoiceNo = _voiceNo
					;
				}
				_voices [_assignToVoiceNo].free ();
			}
			var _voice = _voices [_assignToVoiceNo];
			_voice._musicObject = _newVoice._musicObject;
			_voice._timeStamp = _newVoice._timeStamp;
			_voice._channelNo = _newVoice._channelNo;
			_voice._noteNo = _newVoice._noteNo;
			(_param2 >= 0 && _param3 >= 0) ? _this._playerRef.noteOn (_channelNo,_param2,_param3,_param4,_param5) : _this._playerRef.noteOn (_channelNo,_param4,_param5);
			if (typeof _param6 == 'number') _voice.noteOffTimeout = setTimeout ('Music.voices [' + _assignToVoiceNo + '].free ()',_param6);
			_this._notesPlaying++;
			_this.updateAudibleState ();
		} else {
			_this.noteOn (_channelNo,-1,-1,_param2,_param3,(typeof _param4 != 'number') ? null : _param4);
		}
	}
}

function mo_noteOff (_channelNo,_noteNo) {
	if (this.on ()) {
		if (typeof _noteNo == 'string') _noteNo = mo_getNoteNumber (_noteNo);
		if (typeof _noteNo != 'number') _noteNo = -1;
		for (var _voiceNo = 0; _voiceNo < MO_Music._polyphony; _voiceNo++) {
			var _voice = MO_Music._voices [_voiceNo];
			if (_voice._musicObject == this && _voice._channelNo == _channelNo && (_voice._noteNo == _noteNo || _noteNo == -1)) _voice.free ();
		}
	}
}

function mo_setProgram (_channelNo,_param2,_param3) {
	if (this.on ()) {
		(typeof _param3 == 'number') ? this._playerRef.setProgram (_channelNo,_param2,_param3) : this._playerRef.setProgram (_channelNo,_param2);
	}
}

function mo_isPaused () {return this.ready ? this._playerRef.IsPaused () : false}
function mo_isPlaying () {return this.ready ? this._playerRef.IsPlaying () : false}

function mo_stubEmbed (_fileURL,_extraAttr) {
	this.magicEmbed (((typeof _fileURL != 'string') ? '' : ('SRC="' + _fileURL + '" ')) + 'WIDTH=0 HEIGHT=0 HIDDEN AUTOSTART=TRUE LOOP=TRUE ' + ((typeof _extraAttr == 'string') ? _extraAttr : ''));
}

function mo_preloadEmbed (_fileURL,_extraAttr) {
	this.stubEmbed (_fileURL,'AUTOSTART=FALSE LOOP=FALSE ' + ((typeof _extraAttr == 'string') ? _extraAttr : ''));
}

function mo_getNoteName (_noteNumber) {
	var _noteNames = new Array ('C','C#','D','D#','E','F','F#','G','G#','A','A#','B');
	return _noteNames [_noteNumber % 12] + (Math.floor (_noteNumber / 12) - 1);
}

function mo_getNoteNumber (_noteName) {
	if (typeof _noteName == 'number') return _noteName;
	var
		_noteOffset = 'c-d-ef-g-a-b'.indexOf (_noteName.charAt (0).toLowerCase ()),
		_result = 0,
		_sharpFlatOffset = 0
	;
	if (_noteOffset != -1) {
		var _sharpFlatPos = _noteName.indexOf ('b',1);
		if (_sharpFlatPos == -1) {
			_sharpFlatPos = _noteName.indexOf ('#',1);
			(_sharpFlatPos == -1) ? (_sharpFlatPos = 0) : (_sharpFlatOffset = 1);
		} else {
			_sharpFlatOffset = -1;
		}
		var _octaveNo = _noteName.substring (_sharpFlatPos + 1) - 0;
		_result =  12 + _octaveNo * 12 + _noteOffset + _sharpFlatOffset;
	}
	return Math.floor (_result);
}

function MO_clearTimeout (_objectHandle,_timeoutName) {
	if (typeof _objectHandle [_timeoutName] != MO_undefined && _objectHandle [_timeoutName] != null)
		clearTimeout (_objectHandle [_timeoutName])
	;
	_objectHandle [_timeoutName] = null;
}

function MO_Voice_free () {
	var _this = this;
	MO_clearTimeout (_this,'noteOffTimeout');
	_this._musicObject._playerRef.noteOff (_this._channelNo,_this._noteNo,127);
	_this._channelNo = 0;
	_this._musicObject._notesPlaying--;
	MO_clearTimeout (_this._musicObject,'updateAudibleStateTimeout');
	_this._musicObject.updateAudibleStateTimeout = setTimeout (_this._musicObject.objectName + '.updateAudibleState ()',_this._musicObject.averageNoteRelease);
}

function MO_Voice () {
	var _this = this;
	/*** Public Properties ***/
	_this._musicObject = null;
	_this._channelNo = _this._noteNo = 0;

	/*** Private Properties ***/
	_this._timeStamp = 0;

	/*** Public Methods ***/
	_this.free = MO_Voice_free;
}

function MO_kill () {
	mo_stopAll ();
	MO_Music.enabled = false;
}

function mo_setAudioDevicePriority (_audioDevicePriority) {
	if (MO_Music._playerIsAtLeast210 && MO_globalThru ()) MO_Music.global.setAudioDevicePriority (_audioDevicePriority);
	MO_Music.embedDefaults.AUDIODEVICEPRIORITY = _audioDevicePriority;
}

function mo_engageAudio (_audioEngaged) {
	if (MO_globalThru ()) MO_Music.global.engageAudio (_audioEngaged);
	MO_Music.embedDefaults.AUDIOENGAGED = _audioEngaged;
	MO_Music.execCallback ('onEngageAudio');
}

function mo_isAudioDeviceShared () {
	var
		_Music = MO_Music,
		_userAgent = MO_navigator.userAgent
	;
	return _Music._playerIsAtLeast210 ? (MO_globalThru () ? _Music.global.isAudioDeviceShared () : null) : (_Music.stringHasAny (_userAgent,'WinNT','Windows NT') ? _Music.meetsMinVersion (_Music.retrieveVersion (_userAgent,1,MO_indexOf (_userAgent,(_userAgent.indexOf ('WinNT') != -1) ? 'WinNT' : 'Windows NT')),'5.0') : !_Music.stringHasAny (_userAgent,'Win9','Windows 9'));
}

function mo_isAudioSupported () {
	return (MO_Music._playerIsAtLeast210 && MO_globalThru ()) ? MO_Music.global.isAudioSupported () : true;
}

function MO_globalThru () {return MO_Music.enabled && MO_Music.global != null}

function mo_doMenuItem (_menuItemName) {if (this.on ()) this._playerRef.doMenuItem (_menuItemName)}
function mo_getAudioDevicePriority () {return MO_Music.embedDefaults.AUDIODEVICEPRIORITY}
function mo_getAutostart () {return this.ready ? this._playerRef.getAutostart () : null}
function mo_getChannelMute (_channelNo) {return this.ready ? this._playerRef.getChannelMute (_channelNo) : null}
function mo_getChannelSolo (_channelNo) {return this.ready ? this._playerRef.getChannelSolo (_channelNo) : null}
function mo_getController (_channelNo,_controllerNo) {return this.ready ? this._playerRef.getController (_channelNo,_controllerNo) : null}
function mo_getFileSize () {return this.ready ? this._playerRef.getFileSize () : null}
function mo_getInfo (_infoField) {return this.ready ? this._playerRef.getInfo (_infoField) : null}
function mo_getLoop () {return this.ready ? this._playerRef.getLoop () : null}
function mo_getPanelDisplay () {return this.ready ? this._playerRef.getPanelDisplay () : null}
function mo_getPanelMode () {return this.ready ? this._playerRef.getPanelMode () : null}
function mo_getPlayer () {return this._playerRef}
function mo_getPlayLength () {return this.ready ? this._playerRef.getPlayLength () : null}
function mo_getPosition () {return this.ready ? this._playerRef.getPosition () : null}
function mo_getProgram (_channelNo) {return this.ready ? this._playerRef.getProgram (_channelNo) : null}
function mo_getReverbType () {return MO_globalThru () ? MO_Music.global.getReverbType () : null}
function mo_getTempo () {return this.ready ? this._playerRef.getTempo () : null}
function mo_getTrackMute (_trackNo) {return this.ready ? this._playerRef.getTrackMute (_trackNo) : null}
function mo_getTrackSolo (_trackNo) {return this.ready ? this._playerRef.getTrackSolo (_trackNo) : null}
function mo_getTransposable (_channelNo) {return this.ready ? this._playerRef.getTransposable (_channelNo) : null}
function mo_getTranspose () {return this.ready ? this._playerRef.getTranspose () : null}
function mo_isAudioEngaged () {return (MO_globalThru () && MO_Music._playerIsAtLeast210) ? MO_Music.global.isAudioEngaged () : null}
function mo_setAutostart (_state) {if (this.on ()) this._playerRef.setAutostart (_state)}
function mo_setChannelMute (_channelNo,_state) {if (this.on ()) this._playerRef.setChannelMute (_channelNo,_state)}
function mo_setChannelSolo (_channelNo,_state) {if (this.on ()) this._playerRef.setChannelSolo (_channelNo,_state)}
function mo_setController (_channelNo,_controllerNo,_controllerValue) {if (this.on ()) this._playerRef.setController (_channelNo,_controllerNo,_controllerValue)}
function mo_setEndTime (_endTime) {if (this.on ()) this._playerRef.setEndTime (_endTime)}
function mo_setGlobalMute (_muteState) {if (MO_globalThru ()) MO_Music.global.setGlobalMute (_muteState)}
function mo_setLoop (_state) {if (this.on ()) this._playerRef.setLoop (_state)}
function mo_setPanelDisplay (_displayType) {if (this.on ()) this._playerRef.setPanelDisplay (_displayType)}
function mo_setPanelMode (_panelMode) {if (this.on ()) this._playerRef.setPanelMode (_panelMode)}
function mo_setPosition (_position) {if (this.on ()) this._playerRef.setPosition (_position)}
function mo_setPlayer (_playerRef) {this._playerRef = _playerRef}
function mo_setReverbType (_reverbType) {if (MO_globalThru ()) MO_Music.global.setReverbType (_reverbType)}
function mo_setStartTime (_startTime) {if (this.on ()) this._playerRef.setStartTime (_startTime)}
function mo_setTempo (_tempo) {if (this.on ()) this._playerRef.setTempo (_tempo)}
function mo_setTrackMute (_trackNo,_state) {if (this.on ()) this._playerRef.setTrackMute (_trackNo,_state)}
function mo_setTrackSolo (_trackNo,_state) {if (this.on ()) this._playerRef.setTrackSolo (_trackNo,_state)}
function mo_setTranspose (_transpose) {if (this.on ()) this._playerRef.setTranspose (_transpose)}
function mo_setTransposable (_channelNo,_state) {if (this.on ()) this._playerRef.setTransposable (_channelNo,_state)}
/*** Placeholder Compatibility Methods ***/

MO_Music.meetsMinVersion = MO_Music.stringHasAny = MO_returnFalse;

/*** Extensibility API Static Properties ***/

MO_Music.global = null;
MO_Music.minFadeInterval = 100;
MO_Music.publicInstanceCallbacks = new Array ();
MO_Music.publicStaticCallbacks = new Array ();

/*** Extensibility API Static Methods ***/

MO_Music.addEventHandler = MO_addEventHandler;
MO_Music.addInstanceExtender = MO_addInstanceExtender;
MO_Music.addPrivateInstanceMethods = MO_addPrivateInstanceMethods;
MO_Music.addPublicCallbacks = MO_addPublicCallbacks;
MO_Music.addPublicInstanceMethods = MO_addPublicInstanceMethods;
MO_Music.addPublicStaticMethods = MO_addPublicStaticMethods;
MO_Music.appendToArray = MO_appendToArray;
MO_Music.arrayOfObjects = MO_arrayOfObjects;
MO_Music.clearTimeout = MO_clearTimeout;
MO_Music.doNothing = MO_doNothing;
MO_Music.execCallback = mo_execCallback;
MO_Music.execHandler = MO_execHandler;
MO_Music.globalThru = MO_globalThru;
MO_Music.indexOf = MO_indexOf;
MO_Music.isInstance = MO_isInstance;
MO_Music.parseAttributes = MO_parseAttributes;
MO_Music.tagAttr = MO_tagAttr;
MO_Music.wireMethod = MO_wireMethod;

/*** Private Static Properties ***/

MO_Music._fadeQueue = new Array ();
MO_Music._extenders = new Array ();
MO_Music._globalNoteNo = 0;
MO_Music._polyphony = 32;
MO_Music.voices = MO_Music._voices = MO_arrayOfObjects (MO_Music._polyphony,MO_Voice);
MO_Music._newVoice = new MO_Voice ();
MO_Music._canFade = true;

/*** Public Instance Methods ***/

mo_playNote = mo_noteOn;
MO_addPublicInstanceMethods ('addCallbackHandler','doMenuItem','fadeTo','fadeVolume','getAutostart','getChannelMute','getChannelSolo','getController','getFileSize','getInfo','getLoop','getMonophonic','getPanelDisplay','getPanelMode','getPlayLength','getPosition','getProgram','getTempo','getTrackMute','getTrackSolo','getTransposable','getTranspose','getVolume','isPaused','isPlaying','noteOff','noteOn','playNote','preloadEmbed','setAutostart','setChannelMute','setChannelSolo','setController','setEndTime','setLoop','setMonophonic','setPanelDisplay','setPanelMode','setPosition','setProgram','setStartTime','setTempo','setTrackMute','setTrackSolo','setTransposable','setTranspose','stubEmbed');

/*** Register Callback Types ***/

MO_addPublicCallbacks (
	'[instance]',
		'onLoad','onAudibleStateChange','onMetaEvent','onOutputVolumeChange','onPause','onPlay','onReady','onStop',
	'[static]',
		'onAudioEngagedChange','onEngageAudio'
);

/*** Private Instance Methods ***/

MO_addPrivateInstanceMethods ('execMetaEventHandlers','execOnMetaEvent','getPlayer','setPlayer');

/*** Instance Extenders ***/

function MO_addAdvancedProperties (_this) {
	_this._monophonic = MO_arrayOfObjects (17,false);
	_this._volumeFade = new MO_Fade ();
	_this._notesPlaying = _this.averageNoteRelease = 0;
	_this.usage = 'music';
	_this.audible = false;
	_this.addCallbackHandler ('onOutputVolumeChange',_this.objectName + '.updateAudibleState ()');
}

MO_addInstanceExtender (MO_addAdvancedProperties);

/*** Public Static Methods ***/

MO_addPublicStaticMethods ('addCallbackHandler','engageAudio','getAudioDevicePriority','getNoteName','getNoteNumber','getReverbType','isAudioDeviceShared','isAudioEngaged','isAudioSupported','kill','setAudioDevicePriority','setGlobalMute','setReverbType','stopAll');

/*** Public Objects ***/

MO_Music.Fade = MO_Fade;

/*Scruncher:Leave Next Comment*/
/**********
     TITLE: Music Object Component - Compatibility
   VERSION: 3.4.0.7
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: -- can stand alone --
**********/

function MO_stringHasAny (_sourceStr) {
	var _args = MO_stringHasAny.arguments;
	for (var _argNo = 1; _argNo < _args.length; _argNo++) {
		if (_sourceStr.indexOf (_args [_argNo]) != -1) return true;
	}
	return false;
}

function MO_listStringHas (_sourceStr,_subStr) {
	return (',' + _sourceStr + ',').indexOf (',' + _subStr + ',') != -1;
}

function MO_openWindow (_URL,_name,_width,_height,_showStatus,_properties,_xpos,_ypos) {
	if (typeof _properties != 'string') _properties = '';
	var _screenDefined = typeof screen != MO_undefined;
	if (typeof _xpos == MO_undefined && _screenDefined)
		_xpos = Math.max (Math.floor ((screen.width - _width - 10) / 2),0)
	;
	if (typeof _ypos == MO_undefined && _screenDefined)
		_ypos = Math.max (Math.floor ((screen.height - _height - 40) / 2),0)
	;
	var _positionStr = (typeof _xpos != MO_undefined && typeof _ypos != MO_undefined) ? ('screenx=' + _xpos + ',screeny=' + _ypos + ',left=' + _xpos + ',top=' + _ypos + ',') : '';
	return window.open (_URL,_name,_properties + ',toolbar=no,location=no,directories=no,status=' + ((typeof _showStatus == 'boolean' && _showStatus) ? 'yes' : 'no') + ',menubar=no,scrollbars=no,resizable=yes,' + _positionStr + 'width=' + _width + ',height=' + _height + ',' + _properties);
}

function mo_promptClose () {
	if (MO_Music._promptWindow != null) {
		MO_Music._promptWindow.close ();
		MO_Music._promptWindow = null;
		window.focus ();
	}
}

function mo_promptWindowKeyPressed (_key) {
	if (_key == 13) {
		eval (MO_Music._promptWindowOkAction);
	} else if (_key == 27) {
		eval (MO_Music._promptWindowCancelAction);
	};
}

function mo_promptUser (_heading,_message,_okText,_okAction,_cancelText,_cancelAction,_showStatus) {
	MO_Music._promptWindow = MO_openWindow ('','mo_promptWindow',500,295,_showStatus);
	if (typeof _okAction != 'string' || _okAction == '') _okAction = 'mo_promptClose ()';
	if (typeof _cancelAction != 'string' || _cancelAction == '') _cancelAction = 'mo_promptClose ()';
	MO_Music._promptWindowOkAction = _okAction;
	MO_Music._promptWindowCancelAction = _cancelAction;
	if (typeof _heading != 'string') _heading = '';
	var _document = MO_Music._promptWindow.document;
	_document.open ('text/html');
	_document.writeln (
		'<HTML><HEAD><TITLE>' + _heading + '</TITLE></HEAD><BODY TEXT=000000 TOPMARGIN=0 LEFTMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0>' +
		'<SCRIPT LANGUAGE=JavaScript>if (typeof Event != "undefined") document.captureEvents (Event.KEYUP); document.onkeyup = new Function ("event","var key = (typeof Event != \\\'undefined\\\') ? event.which : window.event.keyCode; window.opener.mo_promptWindowKeyPressed (key); return false;")</SCRIPT>' +
		'<FORM><TABLE WIDTH=100% HEIGHT=100% BORDER=0 CELLSPACING=0 CELLPADDING=6 BGCOLOR=CCCCCC>' +
		((_heading != '') ? ('<TR><TD VALIGN=TOP><TABLE WIDTH=100% BORDER=1 CELLSPACING=0 CELLPADDING=2 BGCOLOR=AAAAAA><TR><TD ALIGN=CENTER><FONT FACE=ARIAL,HELVETICA,VERDANA SIZE=5>' + _heading + '</FONT></U><BR></TD></TR></TABLE></TD></TR>') : '') +
		'<TR><TD VALIGN=CENTER><FONT FACE=ARIAL,HELVETICA,VERDANA SIZE=3>' + _message + '</FONT>' +
		'</TD></TR><TR><TD VALIGN=BOTTOM>' +
		'<TABLE WIDTH=100% BORDER=0 CELLSPACING=0 CELLPADDING=0><TR><TD COLSPAN=2><HR></TD></TR><TR><TD>' +
		((typeof _cancelText == 'string' && _cancelText != '') ? ('<INPUT TYPE=button VALUE="' + _cancelText + '" ONCLICK="with (window.opener) {' + _cancelAction + '}">') : '') +
		'</TD><TD ALIGN=RIGHT>' +
		((typeof _okText == 'string' && _okText != '') ? ('<INPUT TYPE=button VALUE="' + _okText + '" ONCLICK="with (window.opener) {' + _okAction + '}">') : '') +
		'</TD></TR></TABLE></TD></TR></TABLE></FORM></BODY></HTML>'
	);
	_document.close ();
	MO_Music._promptWindow.focus ();
}

function mo_installPlayer () {
	var _optionsStr = '';
	for (var _option in MO_Music.installerOptions)
		_optionsStr += ((_optionsStr != '') ? '&' : '') + _option + '=' + escape (MO_Music.installerOptions [_option])
	;
	MO_Music._promptWindow = MO_openWindow ('http://www.beatnik.com/to/install-player.html?' + _optionsStr,'mo_promptWindow',500,295,true,'resizable=false');
}

function MO_retrieveVersion (_sourceStr,_direction,_startPos) {
	if (typeof _direction != 'number') _direction = -1;
	var
		_versionStr = '',
		_numerals = '0123456789',
		_openerBrackets = (_direction == 1) ? '({[<' : ')}]>',
		_closerBrackets = (_direction == 1) ? ')}]>' : '({[<',
		_lastPos = (_direction == 1) ? (_sourceStr.length - 1) : 0,
		_charNo = (typeof _startPos == 'number') ? _startPos : ((_direction == 1) ? 0 : (_sourceStr.length - 1)),
		_currentChar,
		_inVersion = false,
		_brackets = ''
	;
	while (_charNo != _lastPos + _direction) {
		_currentChar = _sourceStr.charAt (_charNo);
		var _bracketType = _openerBrackets.indexOf (_currentChar);
		if (_bracketType != -1) {
			_brackets = _closerBrackets.charAt (_bracketType) + _brackets;
		} else if (_closerBrackets.indexOf (_currentChar) != -1) {
			if (_currentChar == _brackets.charAt (0));
			_brackets = _brackets.substr (1);
		} else if (_brackets == '') {
			if (_inVersion || _numerals.indexOf (_currentChar) != -1) {
				if (!_inVersion) _versionStr = '';
				_inVersion = _currentChar == '.' || _numerals.indexOf (_currentChar) != -1;
				if (_inVersion) {
					if (_direction == 1) _versionStr += _currentChar; else _versionStr = _currentChar + _versionStr;
				} else {
					if (_versionStr.indexOf ('.') != -1) _charNo = _lastPos;
				}
			}
		}
		_charNo += _direction;
	}
	return _versionStr;
}

function MO_meetsMinVersion (_versionToTest,_minRequiredVersion) {
	var _versionA = MO_retrieveVersion (_versionToTest);
	return (_versionA + '0.0.0.0.0.0.0.0.0.0.0.0.0'.substring (_versionA.length)) >= MO_retrieveVersion (_minRequiredVersion);
}

function mo_hasMinVersion (_minRequiredVersion) {
	return MO_meetsMinVersion (MO_Music.playerVersion,_minRequiredVersion);
}

function mo_requireMinPlayerVersion (_minVersion) {
	if (MO_meetsMinVersion (_minVersion,MO_Music.requiredMinVersion)) MO_Music.requiredMinVersion = _minVersion;
}

function mo_isPlayerCompatible (_minVersion,_silentIfInadequate,_showCompatibilityPrompt) {
	var _result = false;
	if (MO_Music.clientSupported) {
		if (typeof _minVersion == 'string') mo_requireMinPlayerVersion (_minVersion);
		if (typeof _silentIfInadequate == 'boolean') MO_Music.silentIfInadequate = _silentIfInadequate;
		if (typeof _showCompatibilityPrompt == 'boolean') MO_Music.showCompatibilityPrompt = _showCompatibilityPrompt;
		if (MO_Music.hasPlayer) {
			if (mo_hasMinVersion (MO_Music.requiredMinVersion)) {
				_result = true;
			} else {
				if (MO_Music.clientUpgradable && MO_Music.showCompatibilityPrompt) MO_Music.upgradePrompt ();
			}
		} else {
			if (MO_Music.clientUpgradable && MO_Music.showCompatibilityPrompt) MO_Music.installPrompt ();
		}
	}
	if (!_result && MO_Music.silentIfInadequate) MO_kill ();
	return MO_Music.playerCompatible = _result;
}

function MO_requireJava () {
	if (!MO_Music.ignoreJavaDisabled) {
		MO_Music.ignoreJavaDisabled = true;
		if (MO_Music.clientSupported && MO_Music._javaNeeded) MO_Music.enableJavaPrompt ();
	}
}

function MO_playerSubstitute (_this) {
	var _result = '';
	if (!_this._hideInLayer) {
		var
			_panelW = _this.WIDTH - 0,
			_panelH = _this.HEIGHT - 0,
			_tableDims = MO_layoutAttributes (_this)
		;
		_result = '<TABLE BORDER=0 CELLSPACING=0 CELLPADDING=0' + _tableDims + '><TR><TD></TD></TR></TABLE>';
		if (MO_Music.enabled && MO_Music.clientSupported && MO_Music.clientUpgradable) {
			var
				_getPlayerText = MO_Music.hasPlayer ? MO_Music.upgradePlayerText : MO_Music.getPlayerText,
				_getPlayerLink = '<A HREF="javascript://" ONCLICK="mo_installPlayer (); return false">'
			;
			if (typeof MO_Music.getPlayerImagesPath == 'string' && _panelW == 144 && (_panelH == 60 || _panelH == 45 || _panelH == 15)) {
				_result = _getPlayerLink + '<IMG SRC="' + MO_Music.getPlayerImagesPath + 'get-player-' + _panelW + 'x' + _panelH + '.gif"' + _tableDims + ' ALT="' + _getPlayerText + '" BORDER=0></A>';
			} else {
				if (_panelW >= 90 && _panelH >= 15) _result = '<TABLE BORDER=1 CELLSPACING=0 CELLPADDING=0 BGCOLOR=FFFFFF' + _tableDims + '><TR ALIGN=CENTER VALIGN=CENTER><TD>' + _getPlayerLink + '<FONT FACE="ARIAL,HELVETICA,VERDANA" COLOR=000000 SIZE=' + ((_panelH >= 30) ? '3' : '1')  + '>' + _getPlayerText + '</FONT></A></TD></TR></TABLE>';
			}
		}
	}
	return _result;
}

function MO_setPlayerVersion (_version) {
	MO_Music.playerVersion = MO_retrieveVersion (_version + '');
	MO_Music._playerIsAtLeast210 = mo_hasMinVersion ('2.1.0');
	MO_Music._playerIsAtLeast220 = mo_hasMinVersion ('2.2.0');
}

function MO_updatePlayerCheck () {
	var _Music = MO_Music;
	if (typeof mo_delayPlayerCheck == MO_undefined || !mo_delayPlayerCheck) {
		if (_Music.clientSupported && !_Music._playerChecked) {
			_Music._playerChecked = true;
			if (_Music.playerType == 'Plug-in') {
				var _plugins = MO_navigator.plugins;
				for (var _pluginNo = 0; _pluginNo < _plugins.length; _pluginNo++) {
					if (_plugins [_pluginNo].name.indexOf ('Beatnik') != -1 && _plugins [_pluginNo].name.indexOf ('Stub') == -1) {
						MO_setPlayerVersion (_plugins [_pluginNo].name);
						if (_Music.playerVersion == '') {
							if (_Music.clientPlatform == 'Mac') {
								MO_setPlayerVersion ('1.1.7');
							} else if (_Music.clientPlatform == 'WebTV') {
								MO_setPlayerVersion ('0.9.0');
							}
						}
						_Music.hasPlayer = true;
						break;
					}
				}
				_Music._hasCallbackIssue = mo_hasMinVersion ('1.3') && !_Music._javaNeeded;
			} else {
				document.writeln (
					'<SPAN STYLE="position:absolute;visibility:hidden">' +
					'<OBJECT ID=mo_testInstance CLASSID="CLSID:B384F118-18EE-11D1-95C8-00A024330339" WIDTH=0 HEIGHT=0>' +
					'<PARAM NAME=WIDTH VALUE=0>' +
					'<PARAM NAME=HEIGHT VALUE=0>' +
					'<PARAM NAME=DOCUMENTID VALUE=0>' +
					'<PARAM NAME=AUTOSTART VALUE=FALSE>' +
					'</OBJECT>' +
					'</SPAN>'
				);
				_Music.hasPlayer = typeof document.mo_testInstance.getVersion != MO_undefined;
				if (_Music.hasPlayer) MO_setPlayerVersion (document.mo_testInstance.getVersion ());
			}
		}
	}
}

function MO_checkForPlayer () {
	mo_delayPlayerCheck = false;
	MO_updatePlayerCheck ();
}

function MO_checkClientSupported (_clientToCheck) {
	var _Music = MO_Music;
	if (MO_listStringHas (_clientToCheck.name,_Music.clientName) && MO_listStringHas (_clientToCheck.platform,_Music.clientPlatform)) {
		var _versionRanges = _clientToCheck.versions.split (',');
		for (var _versionRangeNo = 0; _versionRangeNo < _versionRanges.length; _versionRangeNo++) {
			var
				_versionRange = _versionRanges [_versionRangeNo].toLowerCase (),
				_delimPos = (_versionRange.indexOf ('to') != -1) ? _versionRange.indexOf ('to') : _versionRange.length,
				_rangeLower = MO_retrieveVersion (_versionRange.substr (0,_delimPos)),
				_rangeUpper = MO_retrieveVersion (_versionRange.substr (_delimPos + 1))
			;
			if (_rangeUpper == '') _rangeUpper = _rangeLower;
			if ((_rangeLower == '' || MO_meetsMinVersion (_Music.clientVersion,_rangeLower)) && (_rangeUpper == '' || MO_stringHasAny (_versionRange,'and higher','and up') || MO_meetsMinVersion (_rangeUpper,_Music.clientVersion))) {
				_Music.clientKnown = true;
				_Music.clientSupported = !MO_stringHasAny (_versionRange,'except','not');
				if (_Music.clientSupported) {
					_Music.client = _clientToCheck;
					_Music.clientUpgradable = _clientToCheck.upgradable;
				} else {
					_Music.client = null;
					_Music.clientUpgradable = false;
				}
			}
		}
	}
	MO_updatePlayerCheck ();
}

function MO_addKnownClient (_name,_platform,_versions,_upgradable,_customProperties) {
	var _this = MO_Music.knownClients [MO_Music.knownClients.length] = new Object ();
	_this.name = _name;
	_this.platform = _platform;
	_this.versions = _versions;
	_this.upgradable = _upgradable;
	if (typeof _customProperties == 'object') {
		for (var _property in _customProperties) _this [_property] = _customProperties [_property];
	}
	MO_checkClientSupported (_this);
}

function MO_setClientInfo (_appName,_appVersion,_platform,_userAgent) {
	var
		_Music = MO_Music,
		_navigator = MO_navigator,
		_navigatorAppVersion = _navigator.appVersion
	;
	_Music.clientName = (typeof _appName == 'string') ? _appName : _navigator.appName;
	_Music.clientVersion = MO_retrieveVersion ((typeof _appVersion == 'string') ? _appVersion : ((_Music.clientName == MO_MSIE) ? _navigatorAppVersion.substr (_navigatorAppVersion.indexOf ('MSIE')) : _navigatorAppVersion),1);
	_Music.clientKnown = _Music.clientSupported = _Music.clientUpgradable = false;
	_Music.client = null;
	if (typeof _userAgent != 'string') _userAgent = _navigator.userAgent;
	if (MO_stringHasAny (_userAgent,'Win9','Windows 9','WinNT','Windows NT','Win2','Windows 2')) {
		_platform = 'Win32';
	} else if (_userAgent.indexOf ('PPC') != -1) {
		_platform = 'MacPPC';
	} else if (_userAgent.indexOf ('WebTV') != -1) {
		_platform = 'WebTV';
	} else if (typeof _platform != 'string') {
		_platform = (typeof _navigator.platform == MO_undefined) ? 'Unknown' : _navigator.platform;
	}
	_Music.platform = _Music.clientPlatform = _platform;
	for (var _clientNo = 0; _clientNo < _Music.knownClients.length; _clientNo++)
		MO_checkClientSupported (_Music.knownClients [_clientNo])
	;
}

/*** Standalone Support ***/

MO_onlyCompatibility = typeof Music == 'undefined';

if (MO_onlyCompatibility) {
	MO_MSIE = 'Microsoft Internet Explorer';
	MO_navigator = navigator;
	MO_undefined = 'undefined';
	MO_Music = Music = new Object ();
	MO_Music.playerType = (MO_navigator.appName == MO_MSIE && MO_navigator.platform.indexOf ('Win') != -1) ? 'ActiveX' : 'Plug-in';
	MO_addPublicStaticMethods = new Function ('for (var argNo = 0; argNo < arguments.length; argNo++) Music [arguments [argNo]] = window [\'mo_\' + arguments [argNo]];');
}

/*** Public Static Methods ***/

MO_addPublicStaticMethods ('hasMinVersion','installPlayer','isPlayerCompatible','promptClose','promptUser','requireMinPlayerVersion');

/*** Public Static Properties ***/

MO_Music.ignoreJavaDisabled = MO_Music.silentIfInadequate = false;
MO_Music.installerOptions = new Array ();
MO_Music.installerOptions.referrerURL = window.location.href;
MO_Music.installerOptions.inFrameset = top != window;
MO_Music.knownClients = new Array ();
MO_Music.showCompatibilityPrompt = true;

/*** Public Static Properties (read only) ***/

MO_Music.requiredMinVersion = '';
MO_Music.playerCompatible = null;
MO_Music.hasPlayer = false;
MO_setPlayerVersion ('');

/*** Private Static Properties ***/

MO_Music._javaNeeded = MO_navigator.appName == 'Netscape' && MO_navigator.appVersion < '5' && !MO_navigator.javaEnabled ();
MO_Music._playerChecked = false;
MO_Music._promptWindow = null;

/*** Private Static Methods ***/

MO_Music._setPlayerVersion = MO_setPlayerVersion;

/*** Extensibility API Static Methods ***/

MO_Music.addKnownClient = MO_addKnownClient;
MO_Music.checkForPlayer = MO_checkForPlayer;
MO_Music.meetsMinVersion = MO_meetsMinVersion;
MO_Music.openWindow = MO_openWindow;
MO_Music.retrieveVersion = MO_retrieveVersion;
MO_Music.setClientInfo = MO_setClientInfo;
MO_Music.stringHasAny = MO_stringHasAny;

/*** Register Supported Clients ***/

MO_setClientInfo ();
MO_addKnownClient ('Netscape','Win32,MacPPC','4.5 to 5.09,not 1 to 3.1',true);
MO_addKnownClient (MO_MSIE,'Win32,MacPPC','5 to 5.5.9',true);
MO_addKnownClient ('WebTV Plus Receiver,WebTV Satellite Receiver','WebTV','3 and up',false);

/*** English Language Module ***/

function MO_upgradePrompt () {
	mo_promptUser ('Please Upgrade Beatnik','This page has been optimized for the features of version <B>' + MO_Music.requiredMinVersion + ' (or higher)</B> of the Beatnik Player. The currently installed version in your browser is ' + MO_Music.playerVersion + '.<P>If you choose to IGNORE this message, the page will continue to load normally, but may not function properly as designed by the author.','UPGRADE BEATNIK >>>','mo_installPlayer ()','IGNORE','mo_promptClose ()');
}

function MO_installPrompt () {
	mo_promptUser ('Beatnik Enhanced Content !!','This page has been optimized for the audio features of the <B>Beatnik Player</B>. It appears you do not have the Beatnik Player installed.<P>If you choose to IGNORE this message, the page will continue to load normally, except there will be no Beatnik audio.','INSTALL BEATNIK >>>','mo_installPlayer ()','IGNORE','mo_promptClose ()');
}

function MO_enableJavaPrompt () {
	mo_promptUser ('Please Enable Java','This page makes use of the interactive audio features of the Beatnik Player. Java is currently not enabled in your browser. In order for the page to function correctly with Beatnik, <B>you must have Java enabled</B>.<P>This page will continue to load normally, but some interactive audio functionality may be absent.','    OK    ','mo_promptClose ()');
}

MO_Music.upgradePrompt = MO_upgradePrompt;
MO_Music.installPrompt = MO_installPrompt;
MO_Music.enableJavaPrompt = MO_enableJavaPrompt;
MO_Music.upgradePlayerText = 'Upgrade Beatnik';
MO_Music.getPlayerText = 'Get Beatnik';
