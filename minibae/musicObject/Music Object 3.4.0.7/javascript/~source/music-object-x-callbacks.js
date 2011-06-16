/**********
     TITLE: Music Object Extension - Callbacks
   VERSION: 1.2.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x0"*/

function mo_execMetaEventHandlers (_eventType,_eventValue) {
	if (typeof this.onMetaEventHandlers != 'undefined') {
		for (var _handlerNo = 0; _handlerNo < this.onMetaEventHandlers.length; _handlerNo++)
			this.onMetaEventHandlers [_handlerNo] (_eventType,_eventValue,this)
		;
	}
}

function mo_removeCallbackHandlers (_type,_handler) {
	var
		_isInstance = Music.isInstance (this),
		_this = _isInstance ? this : Music,
		_allTypes = typeof _type == 'undefined' || _type == null || _type == '[all]',
		_allHandlers = typeof _handler == 'undefined' || _handler == null || _handler == '[all]',
		_callbacks = Music ['public' + (_isInstance ? 'Instance' : 'Static') + 'Callbacks'],
		_result = false
	;
	for (var _callbackNo = 0; _callbackNo < _callbacks.length; _callbackNo++) {
		var _callbackName = _callbacks [_callbackNo];
		if ((_allTypes || _callbackName == _type) && typeof _this [_callbackName + 'Handlers'] != 'undefined') {
			var _handlers = _this [_callbackName + 'Handlers'];
			if (_allHandlers) {
				_handlers.length = 0;
				_result = true;
			} else {
				var
					_handlersDeleted = 0,
					_deleteHandler = false
				;
				for (var _handlerNo = 0; _handlerNo < _handlers.length - _handlersDeleted; _handlerNo += _deleteHandler ? 0 : 1) {
					if (_handlersDeleted > 0) _handlers [_handlerNo] = _handlers [_handlerNo + _handlersDeleted];
					_deleteHandler = _allHandlers || _handlers [_handlerNo] == _handler;
					if (_deleteHandler) _handlersDeleted++;
				}
				_handlers.length -= _handlersDeleted;
				_result = _handlersDeleted > 0;
			}
		}
	}
	if (_isInstance && _type == 'onMetaEvent' || _allTypes) _this.updateMetaEventsEnabled ();
	return _result;
}

function mo_clearCallbackHandlers (_callbackType) {
	if (typeof this [_callbackType + 'Handlers'] != 'undefined') this [_callbackType + 'Handlers'].length = 0;
	if (_callbackType == 'onMetaEvent') this.updateMetaEventsEnabled ();
	this [_callbackType] (null);
}

function MO_MetaEvent (_type,_value,_handler) {
	this.type = _type;
	this.value = _value;
	this.handler = _handler;
}

function MO_handleMetaEvent (_type,_value,_instanceID) {
	var
		_allTypes = _type == '[all]',
		_allValues = _value == '[all]'
	;
	for (var _handlerNo = 0; _handlerNo < _instanceID.MO_metaEventMap.length; _handlerNo++) {
		var _metaEventHandler = _instanceID.MO_metaEventMap [_handlerNo];
		if ((_allTypes || _metaEventHandler.type == _type) && (_allValues || _metaEventHandler.value == _value))
			_metaEventHandler.handler (_type,_value,_instanceID)
		;
	}
}

function mo_addMetaEventHandler (_type,_value,_handler,_replace) {
	if (typeof this.MO_metaEventMap == 'undefined') this.MO_metaEventMap = new Array ();
	if (typeof _replace == 'boolean' && _replace) this.removeMetaEventHandlers (_type,_value);
	if (this.MO_metaEventMap.length == 0) this.addCallbackHandler ('onMetaEvent',MO_handleMetaEvent);
	this.MO_metaEventMap [this.MO_metaEventMap.length] = new MO_MetaEvent (_type,_value,_handler);
}

function mo_removeMetaEventHandlers (_type,_value,_handler) {
	if (typeof this.MO_metaEventMap != 'undefined') {
		var
			_handlersDeleted = 0,
			_deleteHandler = false,
			_allTypes = typeof _type == 'undefined' || _type == null || _type == '[all]',
			_allValues = typeof _value == 'undefined' || _value == null || _value == '[all]',
			_allHandlers = typeof _handler == 'undefined' || _handler == null || _handler == '[all]',
			_metaEventMap = this.MO_metaEventMap
		;
		if (_allTypes && _allValues && _allHandlers) {
			_metaEventMap.length = 0;
		} else {
			for (var _handlerNo = 0; _handlerNo < _metaEventMap.length - _handlersDeleted; _handlerNo += _deleteHandler ? 0 : 1) {
				var _handlerA = _metaEventMap [_handlerNo];
				if (_handlersDeleted > 0) {
					var	_handlerB = _metaEventMap [_handlerNo + _handlersDeleted];
					_handlerA.type = _handlerB.type;
					_handlerA.value = _handlerB.value;
					_handlerA.handler = _handlerB.handler;
				}
				_deleteHandler =
					(_allTypes || _handlerA.type == _type) &&
					(_allValues || _handlerA.value == _value) &&
					(_allHandlers || _handlerA.handler == _handler)
				;
				if (_deleteHandler) _handlersDeleted++;
			}
			_metaEventMap.length -= _handlersDeleted;
		}
		if (_metaEventMap.length == 0) this.removeOnMetaEvent (MO_handleMetaEvent);
	}
}

function mo_clearCallbacks () {
	var
		_this = this,
		_callbacks = Music.publicInstanceCallbacks
	;
	_this.removeMetaEventHandlers ();
	_this.removeCallbackHandlers ();
	for (var _callbackNo = 0; _callbackNo < _callbacks.length; _callbackNo++)
		_this [_callbackType + 'Handler'] = null
	;
}

function MO_callbacksExecOnReady () {
	var _Music = Music;
	_Music.MO_totalInstancesReady++;
	if (!_Music.MO_firstReadyCalled) {
		_Music.MO_firstReadyCalled = true;
		_Music.execCallback ('onFirstReady');
	}
	if (_Music.MO_totalInstancesReady == _Music.instances.length) _Music.execCallback ('onReady');
}

/*** Instance Extenders ***/

function MO_callbacksInstanceExtender (_this) {
	_this.addCallbackHandler ('onReady',MO_callbacksExecOnReady);
}

Music.addInstanceExtender (MO_callbacksInstanceExtender);

/*** Create "Add", "Remove", and "Clear" Functions ***/

function MO_initializeCallbacksExtension () {
	for (var _callbackNo = 0; _callbackNo < Music.publicInstanceCallbacks.length; _callbackNo++) {
		var
			_callbackName = Music.publicInstanceCallbacks [_callbackNo],
			_callbackNameUpper = _callbackName.charAt (0).toUpperCase () + _callbackName.substr (1)
		;
		window ['mo_add' + _callbackNameUpper] = new Function (
			'handler',
			'this.addCallbackHandler (\'' + _callbackName + '\',handler)'
		);
		window ['mo_remove' + _callbackNameUpper] = new Function (
			'handler',
			'this.removeCallbackHandlers (\'' + _callbackName + '\',handler)'
		);
		window ['mo_clear' + _callbackNameUpper] = new Function (
			'handler',
			'this.clearCallbackHandlers (\'' + _callbackName + '\')'
		);
		Music.addPublicInstanceMethods (
			'add' + _callbackNameUpper,
			'remove' + _callbackNameUpper,
			'clear' + _callbackNameUpper
		);
	}
}

MO_initializeCallbacksExtension ();

/*** Register Callback Types ***/

Music.addPublicCallbacks ('[static]','onFirstReady','onReady');

/*** Private Static Properties ***/

Music.MO_totalInstancesReady = 0;
Music.MO_firstReadyCalled = false;

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('addMetaEventHandler','clearCallbacks','clearCallbackHandlers','removeMetaEventHandlers','removeCallbackHandlers');

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('removeCallbackHandlers');
