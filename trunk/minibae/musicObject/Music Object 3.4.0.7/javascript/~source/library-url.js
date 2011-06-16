/**********
     TITLE: URL Library
   VERSION: 2.0.2
      TYPE: Code Library
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

/*ScruncherSettings Mappings=",URL"*/

function URL_String_toForwardSlashes () {
	var
		_result = '',
		_endFound = false,
		_lastSlashPos = -1
	;
	while (!_endFound) {
		var _slashPos = this.indexOf ('\\',_lastSlashPos + 1);
		if (_slashPos == -1) {
			_slashPos = this.length;
			_endFound = true;
		}
		_result += this.substring (_lastSlashPos + 1,_slashPos) + (!_endFound ? '/' : '');
		if (_slashPos == this.length - 1) _endFound = true;
		_lastSlashPos = _slashPos;
	}
	return _result;
}

function URL_String_toRelativeURL (_basePath) {
	if (typeof _basePath != 'string') _basePath = baseURL;
	_basePath = _basePath.toForwardSlashes ();
	var
		_relativeURL = this.toForwardSlashes (),
		_result = '',
		_commonStr = _basePath.substring (0,_basePath.lastIndexOf ('/') + 1),
		_matchFound = false
	;
	while (!_matchFound) {
		if (_relativeURL.substring (0,_commonStr.length) == _commonStr) {
			_matchFound = true;
		} else {
			_slashPos = _commonStr.lastIndexOf ('/',_commonStr.length - 2);
			if (_slashPos == -1) _matchFound = true;
			_result += '../';
			_commonStr = _commonStr.substring (0,_slashPos + 1);
		}
	}
	_result += _relativeURL.substring (_commonStr.length);
	return _result;
}

function URL_String_toAbsoluteURL (_basePath) {
	if (typeof _basePath != 'string') _basePath = baseURL;
	_basePath = _basePath.toForwardSlashes ();
	var _relativeURL = this.toForwardSlashes ();
	if (_relativeURL.indexOf (':') != -1 && (_relativeURL.indexOf (':') < _relativeURL.indexOf ('/') || _relativeURL.indexOf ('/') == -1)) {
		return _relativeURL;
	} else {
		var
			_result = _basePath.substring (0,_basePath.lastIndexOf ('/')),
			_endFound = false,
			_lastSlashPos = -1,
			_lopPos
		;
		while (!_endFound) {
			var _slashPos = _relativeURL.indexOf ('/',_lastSlashPos + 1);
			if (_slashPos == -1) {
				_slashPos = _relativeURL.length;
				_endFound = true;
			}
			var _directoryName = _relativeURL.substring (_lastSlashPos + 1,_slashPos);
			if (_directoryName == '..') {
				_lopPos = _result.lastIndexOf ('/');
				_result = (_lopPos == -1) ? '' : _result.substring (0,_lopPos);
			} else {
				if (_result != '') _result += '/';
				_result += _directoryName;
			}
			_lastSlashPos = _slashPos;
		}
		return _result;
	}
}

function URL_String_urlize (_caseConversion,_spacerChar,_illegalCharSubstitute) {
	var
		_validChars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890',
		_validSpacers = '.-_~@#&+',
		_result = '',
		_inWord = false,
		_spacersBetween = false,
		_startFound = false,
		_toUrlize = this
	;
	if (typeof _spacerChar != 'string' || _validSpacers.indexOf (_spacerChar) == -1) _spacerChar = '-';
	if (typeof _illegalCharSubstitute != 'string' || _validSpacers.indexOf (_illegalCharSubstitute) == -1) _illegalCharSubstitute = '';
	if (typeof _caseConversion != 'string') _caseConversion = 'lower';
	if (_caseConversion == 'upper') {
		_toUrlize = _toUrlize.toUpperCase ();
	} else if (_caseConversion == 'lower') {
		_toUrlize = _toUrlize.toLowerCase ();
	}
	for (var _charNo = 0; _charNo < _toUrlize.length; _charNo++) {
		var _currentChar = _toUrlize.charAt (_charNo);
		if (_currentChar == ' ' || _validSpacers.indexOf (_currentChar) != -1) {
			if (_currentChar == ' ') {
				_inWord = false;
			} else {
				if (!_inWord) _spacersBetween = true;
				_result += _currentChar;
			}
		} else {
			if (!_inWord) {
				_inWord = true;
				if (_startFound && !_spacersBetween) _result += _spacerChar;
				_spacersBetween = false;
			}
			_result += (_validChars.indexOf (_currentChar) != -1) ? _currentChar : _illegalCharSubstitute;
			if (!_startFound) _startFound = true;
		}
	}
	return _result;
}

/*** Initialization ***/

String.prototype.toForwardSlashes = URL_String_toForwardSlashes;
String.prototype.toRelativeURL = URL_String_toRelativeURL;
String.prototype.toAbsoluteURL = URL_String_toAbsoluteURL;
String.prototype.urlize = URL_String_urlize;

baseURL = window.location.href.toForwardSlashes ();
baseURL = baseURL.substring (0,baseURL.lastIndexOf ('/') + 1);
