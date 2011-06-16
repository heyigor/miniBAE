/**********
     TITLE: Music Object Extension - Song Info
   VERSION: 1.1.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 2000-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_xg"*/

function MO_replaceCopyrightSymbols (_sourceStr) {
	var _result = '';
	for (var _charNo = 0; _charNo < _sourceStr.length; _charNo++) {
		var _char = _sourceStr.charAt (_charNo);
		_result += (_char.charCodeAt (0) == 128) ? '&copy;' : _char;
	}
	return _result;
}

function MO_msToTimeStr (_timeInSeconds) {
	var
		_units = new Array (60,60,24,365,1000),
		_result = '',
		_timeRemainder = _timeInSeconds,
		_unitNo = 0,
		_minDigits = 2
	;
	do {
		var
			_unit = _units [_unitNo],
			_newTimeRemainder = Math.floor (_timeRemainder / _unit),
			_unitValue = _timeRemainder - _newTimeRemainder * _unit
		;
		_timeRemainder = _newTimeRemainder;
		if (_unitValue > 0 || _timeRemainder != 0 || _result == '' || _unitNo < _minDigits)
			_result = ((_timeRemainder != 0 || _unitNo < (_minDigits - 1)) ? '0000000000000000'.substr (0,_unit.toString ().length - _unitValue.toString ().length) : '') + _unitValue + ((_result != '') ? ':' : '') + _result
		;
		_unitNo++;
	} while (_unitNo < _units.length && (_timeRemainder > 0 || _unitNo < _minDigits));
	return _result;
}

function mo_showSongInfo (_songInfoFields,_songInfoImageURL,_fieldNameStyle,_fieldValueStyle) {
	if (typeof _songInfoFields != 'string') _songInfoFields = Music.songInfoFields;
	if (typeof _songInfoImageURL != 'string') _songInfoImageURL = Music.songInfoImageURL;
	if (typeof _fieldNameStyle != 'string') _fieldNameStyle = Music.songInfoFieldNameStyle;
	if (typeof _fieldValueStyle != 'string') _fieldValueStyle = Music.songInfoFieldValueStyle;
	var
		_fieldNames = _songInfoFields.toLowerCase ().split (','),
		_songInfoStr = '',
		_fieldTitles = new Array ()
	;
	_fieldTitles ['title'] = 'Song&nbsp;Title';
	_fieldTitles ['composer'] = 'Composer(s)';
	_fieldTitles ['copyright'] = 'Copyright';
	_fieldTitles ['performer'] = 'Performed&nbsp;By';
	_fieldTitles ['publisher'] = 'Publisher&nbsp;Contact&nbsp;Information';
	_fieldTitles ['use'] = 'Use&nbsp;of&nbsp;License';
	_fieldTitles ['licensee'] = 'Licensed&nbsp;to&nbsp;URL';
	_fieldTitles ['term'] = 'License&nbsp;Term';
	_fieldTitles ['expiration'] = 'Expiration&nbsp;Date';
	_fieldTitles ['notes'] = 'Composer&nbsp;Notes';
	_fieldTitles ['index'] = 'Index&nbsp;Number';
	_fieldTitles ['playlength'] = 'Playback&nbsp;Length';
	_fieldTitles ['size'] = 'File&nbsp;Size';
	_fieldTitles ['genre'] = 'Genre';
	_fieldTitles ['subgenre'] = 'Sub-genre';
	_fieldTitles ['tempo description'] = 'Tempo Description';
	_fieldTitles ['original source'] = 'Original Source';
	_songInfoStr += '<TABLE BORDER=0 CELLSPACING=0 CELLPADDING=3>';
	for (var _fieldNo = 0; _fieldNo < _fieldNames.length; _fieldNo++) {
		var _fieldName = _fieldNames [_fieldNo].replace (/ /gi,'');
		if (typeof _fieldTitles [_fieldName] != 'undefined') {
			var _fieldValue = (_fieldName == 'playlength') ? MO_msToTimeStr (Math.floor (this.getPlayLength () / 1000)) : ((_fieldName == 'size') ? ((Math.round (this.getFileSize () / 1024 * 10) / 10) + ' Kb') : ((_fieldName == 'copyright') ? MO_replaceCopyrightSymbols (this.getInfo (_fieldName) + '') : this.getInfo (_fieldName)));
			if (_fieldValue == null) _fieldValue = '';
			var _linkedField = _fieldValue.indexOf ('http://') == 0 || _fieldValue.indexOf ('https://') == 0 || _fieldValue.indexOf ('mailto:') == 0;
			if (_fieldValue == '') _fieldValue = '&nbsp;';
			_songInfoStr += '<TR VALIGN=TOP><TD CLASS="infoFieldName">' + _fieldTitles [_fieldName] + '&nbsp;:&nbsp;</TD><TD CLASS="infoFieldValue">' + (_linkedField ? ('<A HREF="' + _fieldValue + '" TARGET="_blank">') : '') + _fieldValue + (_linkedField ? '</A>' : '') + '</TD></TR>';
		}
	}
	_songInfoStr += '</TABLE>';
	Music.songInfoWindow = Music.openWindow ('','songInfoWindow',500,382);
	var _docHandle = Music.songInfoWindow.document;
	_docHandle.open ('text/html');
	_docHandle.writeln (
		'<HTML><HEAD>\n' +
		'<STYLE TYPE="text/css"><!--\n' +
		'TD.infoFieldName {' + Music.songInfoFieldNameStyle + '}\n' +
		'TD.infoFieldValue {' + Music.songInfoFieldValueStyle + '}\n' +
		'--></STYLE>\n' +
		'<TITLE>Song Information for "' + this.getInfo ('title') + '"</TITLE>' +
		'</HEAD>' +
		'<BODY TEXT=000000' + ((_songInfoImageURL != '') ? (' BACKGROUND="' + _songInfoImageURL + '"') : '') + ' TOPMARGIN=0 LEFTMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0>' +
		'<FORM><TABLE WIDTH=100% HEIGHT=100% BORDER=0 CELLSPACING=0 CELLPADDING=6>' +
		'<TR><TD VALIGN=CENTER ALIGN=CENTER>' + _songInfoStr + '</TD></TR>' +
		'<TR><TD VALIGN=BOTTOM><TABLE WIDTH=100% BORDER=0 CELLSPACING=0 CELLPADDING=0>' +
		'<TR><TD ALIGN=RIGHT><INPUT TYPE=button VALUE="CLOSE" ONCLICK="window.close ()"></TD></TR>' +
		'</TABLE></TD></TR></TABLE></FORM>' +
		'</BODY></HTML>'
	);
	_docHandle.close ();
	if (typeof Event != 'undefined') {
		Music.songInfoWindow.captureEvents (Event.KEYUP);
		Music.songInfoWindow.onKeyUp = new Function ('event','if (event.which == 13 || event.which == 27) Music.songInfoWindow.close (); return false');
	}
	Music.songInfoWindow.focus ();
}

/*** Public Static Properties ***/

Music.songInfoFields = 'TITLE, PLAYLENGTH, SIZE, COMPOSER, COPYRIGHT, PERFORMER, PUBLISHER, USE, LICENSEE, TERM, EXPIRATION, NOTES';
Music.songInfoImageURL = '';
Music.songInfoFieldNameStyle = 'text-align: right; color: #000; font-family: Arial, Helvetica, Verdana; font-size: 10pt; font-weight: bold;';
Music.songInfoFieldValueStyle = 'color: #000; font-family: Arial, Helvetica, Verdana; font-size: 10pt;';

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('showSongInfo');
