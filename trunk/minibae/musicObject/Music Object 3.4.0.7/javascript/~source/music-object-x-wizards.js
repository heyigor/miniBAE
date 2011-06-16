/**********
     TITLE: Music Object Extension - Wizards
   VERSION: 1.2.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_xi"*/

function MO_wizardsEnabled () {
	return Music.noManualHandlers || Music.clientName != 'Netscape' || !Music.meetsMinVersion (Music.clientVersion,'5');
}

function MO_escapeStr (_sourceStr) {
	var _result = '';
	for (var _charNo = 0; _charNo < _sourceStr.length; _charNo++) {
		var _currentChar = _sourceStr.charAt (_charNo);
		if (_currentChar == "'" || _currentChar == '"' || _currentChar == '\\') _result += '\\';
		_result += _currentChar;
	}
	return _result;
}

function MO_wireElement (_windowHandle,_element,_elementName) {
	var _args = MO_wireElement.arguments;
	for (var _argNo = 3; _argNo < _args.length;_argNo++) {
		var _eventName = _args [_argNo];
		Music.addEventHandler (_windowHandle,_element,_eventName,'eval (mo_' + _elementName + _eventName.charAt (0).toUpperCase () + _eventName.substr (1) + 'Handler)');
	}
}

function MO_wireForm (_windowHandle,_formHandle) {
	for (var _elementNo = 0; _elementNo < _formHandle.elements.length; _elementNo++) {
		var _element = _formHandle.elements [_elementNo];
		if (_element.type == 'checkbox') {
			Music.addEventHandler (_windowHandle,_element,'onclick','eval (this.checked ? mo_checkboxCheckHandler : mo_checkboxUncheckHandler)');
		} else if (_element.type == 'radio') {
			MO_wireElement (_windowHandle,_element,'radio','onclick');
		} else if (_element.type == 'select-one') {
			MO_wireElement (_windowHandle,_element,'list','onfocus','onchange','onblur');
		} else if (_element.type == 'select-multiple') {
			MO_wireElement (_windowHandle,_element,'multilist','onfocus','onchange','onblur');
		} else if (_element.type == 'text') {
			MO_wireElement (_windowHandle,_element,'textbox','onfocus','onblur');
		} else if (_element.type == 'textarea') {
			MO_wireElement (_windowHandle,_element,'textarea','onfocus','onblur');
		} else if (_element.type == 'button') {
			MO_wireElement (_windowHandle,_element,'button','onclick');
		} else if (_element.type == 'reset') {
			MO_wireElement (_windowHandle,_element,'reset','onclick');
		} else if (_element.type == 'submit') {
			MO_wireElement (_windowHandle,_element,'submit','onclick');
		}
	}
}

function MO_joinStatements () {
	var
		_args = MO_joinStatements.arguments,
		_result = ''
	;
	for (var _argNo = 0; _argNo < _args.length; _argNo++)
		_result += (_args [_argNo] != '') ? (((_result != '') ? '; ' : '') + _args [_argNo]) : ''
	;
	return _result;
}

function MO_updateHandler (_objectHandle,_handlerName,_newValue,_updateType) {
	var _oldValue = (typeof _objectHandle [_handlerName] == 'undefined') ? '' : _objectHandle [_handlerName];
	_objectHandle [_handlerName] = (_updateType == 'replace') ? _newValue : ((_updateType == 'merge with') ? ((_newValue != '') ? _newValue : _oldValue) : ((_updateType == 'add to') ? MO_joinStatements (_oldValue,_newValue) : ((_updateType == 'fill in') ? ((_oldValue == '') ? _newValue : _oldValue) : _oldValue)));
}

function mo_sonifyFormsWithHandlers (_checkboxCheck,_checkboxUncheck,_radioButton,_listFocus,_listChoose,_listBlur,_multilistFocus,_multilistChoose,_multilistBlur,_textboxFocus,_textboxBlur,_buttonClick,_formReset,_formSubmit,_updateType,_windowHandle) {
	if (MO_wizardsEnabled ()) {
		_windowHandle = (typeof _windowHandle != 'undefined') ? _windowHandle : ((typeof this.document != 'undefined') ? this : window);
		if (typeof _updateType != 'string') _updateType = 'add to';
		if (typeof _windowHandle.mo_formsSonified == 'undefined') {
			var _docHandle = _windowHandle.document;
			for (var _formNo = 0; _formNo < _docHandle.forms.length; _formNo++)
				MO_wireForm (_windowHandle,_docHandle.forms [_formNo])
			;
			if (typeof _docHandle.layers != 'undefined') {
				for (var _layerNo = 0; _layerNo < _docHandle.layers.length; _layerNo++) {
					var _layerDoc = _docHandle.layers [_layerNo].document;
					for (var _formNo = 0; _formNo < _layerDoc.forms.length; _formNo++)
						MO_wireForm (_windowHandle,_layerDoc.forms [_formNo])
					;
				}
			}
			_windowHandle.mo_formsSonified = true;
		}
		var
			_events = new Array ('checkboxCheck','checkboxUncheck','radioOnclick','listOnfocus','listOnchange','listOnblur','multilistOnfocus','multilistOnchange','multilistOnblur','textboxOnfocus','textboxOnblur','buttonOnclick','resetOnclick','submitOnclick'),
			_args = mo_sonifyFormsWithHandlers.arguments
		;
		for (var _eventNo = 0; _eventNo < _events.length; _eventNo++)
			MO_updateHandler (_windowHandle,'mo_' + _events [_eventNo] + 'Handler',_args [_eventNo],_updateType)
		;
		MO_updateHandler (_windowHandle,'mo_textareaOnfocusHandler',_textboxFocus,_updateType);
		MO_updateHandler (_windowHandle,'mo_textareaOnblurHandler',_textboxBlur,_updateType);
	}
}

function MO_playWizardSoundStr (_groovoidName) {
	return (_groovoidName > ' ') ? ((typeof Music.wizardTarget == 'string') ? ('b_playGroovoid (\'' + Music.wizardTarget + '\',\'auto\',\'' + _groovoidName + '\',100)') : (Music.wizardTarget.objectName + '.play (\'' + _groovoidName + '\')')) : '';
}

function MO_stopWizardSoundStr (_groovoidName) {
	return (_groovoidName > ' ') ? ((typeof Music.wizardTarget == 'string') ? ('b_stopGroovoid (\'' + Music.wizardTarget + '\',\'' + _groovoidName + '\',150)') : (Music.wizardTarget.objectName + '.stop (150)')) : '';
}

function mo_sonifyFormsWithGroovoids (_target,_checkboxCheck,_checkboxUncheck,_radioButton,_listFocus,_listChoose,_listBlur,_multilistFocus,_multilistChoose,_multilistBlur,_textboxFocus,_textboxBlur,_buttonClick,_formReset,_formSubmit,_updateType,_windowHandle) {
	if (MO_wizardsEnabled () && Music.enabled) {
		Music.wizardTarget = _target;
		mo_sonifyFormsWithHandlers (
			MO_playWizardSoundStr (_checkboxCheck),
			MO_playWizardSoundStr (_checkboxUncheck),
			MO_playWizardSoundStr (_radioButton),
			MO_playWizardSoundStr (_listFocus),
			MO_joinStatements (MO_stopWizardSoundStr (_listFocus),MO_playWizardSoundStr (_listChoose)),
			MO_joinStatements (MO_stopWizardSoundStr (_listFocus),MO_playWizardSoundStr (_listBlur)),
			MO_playWizardSoundStr (_multilistFocus),
			MO_joinStatements (MO_stopWizardSoundStr (_multilistFocus),MO_playWizardSoundStr (_multilistChoose)),
			MO_joinStatements (MO_stopWizardSoundStr (_multilistFocus),MO_playWizardSoundStr (_multilistBlur)),
			MO_playWizardSoundStr (_textboxFocus),
			MO_joinStatements (MO_stopWizardSoundStr (_textboxFocus),MO_playWizardSoundStr (_textboxBlur)),
			MO_playWizardSoundStr (_buttonClick),
			MO_playWizardSoundStr (_formReset),
			MO_playWizardSoundStr (_formSubmit),
			_updateType,
			(typeof _windowHandle != 'undefined') ? _windowHandle : ((typeof this.document != 'undefined') ? this : window)
		);
	}
}

function mo_addLinkLabel () {}

function MO_linkHandlersStr (_link) {
	return ('' + _link.onmouseover) + ('' + _link.onmouseout) + ('' + _link.onclick);
}

function MO_linkHasLabel (_link,_label) {
	var _linkHandlersStr = MO_linkHandlersStr (_link);
	return _linkHandlersStr.indexOf ('addLinkLabel') != -1 && Music.stringHasAny (_linkHandlersStr,"'" + _label + "'",'"' + _label + '"');
}

function MO_wireLink (_windowHandle,_link) {
	if (typeof _link ['mo_wired'] == 'undefined') {
		Music.addEventHandler (_windowHandle,_link,'onclick','eval (this.mo_onclick); eval (this.mo_clearHover)');
		Music.addEventHandler (_windowHandle,_link,'onmouseover','eval (this.mo_onmouseover); eval (this.mo_setHover)');
		Music.addEventHandler (_windowHandle,_link,'onmouseout','eval (this.mo_onmouseout); eval (this.mo_clearHover)');
		_link ['mo_wired'] = true;
	}
}

function MO_linkHandler (_linkNoStr,_handler) {
	return ((_handler != '') ? _linkNoStr : '') + _handler;
}

function mo_sonifyLinksWithHandlers (_linkClick,_linkOver,_linkOut,_linkHover,_hoverDelay,_startLinkLabel,_endLinkLabel,_linkLabelPrefix,_updateType,_windowHandle) {
	if (MO_wizardsEnabled ()) {
		_windowHandle = (typeof _windowHandle != 'undefined') ? _windowHandle : ((typeof this.document != 'undefined') ? this : window);
		var
			_docLinks = _windowHandle.document.links,
			_startLinkNo = 0,
			_endLinkNo = _docLinks.length - 1
		;
		if (typeof _hoverDelay != 'number') _hoverDelay = 350;
		if (typeof _linkLabelPrefix != 'string') _linkLabelPrefix = '';
		if (typeof _updateType != 'string') _updateType = 'add to';
		if (typeof _startLinkLabel == 'string' && _startLinkLabel != '[first link]' && _startLinkLabel != '') {
			for (var _linkNo = 0; _linkNo < _docLinks.length; _linkNo++) {
				if (MO_linkHasLabel (_docLinks [_linkNo],_startLinkLabel)) {
					_startLinkNo = _linkNo;
					break;
				}
			}
		}
		if (typeof _endLinkLabel == 'string' && _endLinkLabel != '[last link]' && _endLinkLabel != '') {
			for (var _linkNo = 0; _linkNo < _docLinks.length; _linkNo++) {
				if (MO_linkHasLabel (_docLinks [_linkNo],_endLinkLabel)) {
					_endLinkNo = _linkNo;
					break;
				}
			}
		}
		var
			_isHover = (typeof _linkHover == 'string' && _linkHover != ''),
			_isReversed = _startLinkNo > _endLinkNo,
			_linkInc = _isReversed ? -1 : 1
		;
		var
			_linkNo = _startLinkNo,
			_linksFound = 0,
			_linksToScan = (_endLinkNo - _startLinkNo) * _linkInc + 1
		;
		for (var _linksScanned = 0; _linksScanned < _linksToScan; _linksScanned++) {
			var
				_link = _docLinks [_linkNo],
				_linkHandlersStr = MO_linkHandlersStr (_link)
			;
			if (_linkLabelPrefix == '' || (_linkHandlersStr.indexOf ('addLinkLabel') != -1 && Music.stringHasAny (_linkHandlersStr,"'" + _linkLabelPrefix,'"' + _linkLabelPrefix))) {
				var _linkNoStr = 'linkNo = ' + _linksFound + ';';
				MO_wireLink (_windowHandle,_link);
				if (_isHover) Music.MO_hoverTimeouts++;
				MO_updateHandler (_link,'mo_onclick',MO_linkHandler (_linkNoStr,_linkClick),_updateType);
				MO_updateHandler (_link,'mo_onmouseover',MO_linkHandler (_linkNoStr,_linkOver),_updateType);
				MO_updateHandler (_link,'mo_onmouseout',MO_linkHandler (_linkNoStr,_linkOut),_updateType);
				MO_updateHandler (_link,'mo_setHover',(_isHover ? ('Music.hoverTimeout' + Music.MO_hoverTimeouts + ' = setTimeout (\'' + MO_linkHandler (_linkNoStr,MO_escapeStr (_linkHover)) + '\',' + _hoverDelay + ')') : ''),_updateType);
				MO_updateHandler (_link,'mo_clearHover',(_isHover ? 'clearTimeout (Music.hoverTimeout' + Music.MO_hoverTimeouts + ')' : ''),_updateType);
				_linksFound++;
			}
			_linkNo += _linkInc;
		}
	}
}

function mo_sonifyLinksWithGroovoids (_target,_linkClick,_linkOver,_linkOut,_linkHover,_hoverDelay,_startLinkLabel,_endLinkLabel,_linkLabelPrefix,_updateType,_windowHandle) {
	if (MO_wizardsEnabled () && Music.enabled) {
		Music.wizardTarget = _target;
		mo_sonifyLinksWithHandlers (
			MO_joinStatements (
				MO_stopWizardSoundStr (_linkOver),
				MO_stopWizardSoundStr (_linkHover),
				MO_playWizardSoundStr (_linkClick)
			),
			MO_playWizardSoundStr (_linkOver),
			MO_joinStatements (
				MO_stopWizardSoundStr (_linkOver),
				MO_stopWizardSoundStr (_linkHover),
				MO_stopWizardSoundStr (_linkClick),
				MO_playWizardSoundStr (_linkOut)
			),
			MO_playWizardSoundStr (_linkHover),
			_hoverDelay,
			_startLinkLabel,
			_endLinkLabel,
			_linkLabelPrefix,
			_updateType,
			(typeof _windowHandle != 'undefined') ? _windowHandle : ((typeof this.document != 'undefined') ? this : window)
		);
	}
}

/*** Private Static Properties ***/

Music.MO_hoverTimeouts = 0;

/*** Public Static Properties ***/

Music.noManualHandlers = false;

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('addLinkLabel','sonifyFormsWithGroovoids','sonifyFormsWithHandlers','sonifyLinksWithGroovoids','sonifyLinksWithHandlers');
