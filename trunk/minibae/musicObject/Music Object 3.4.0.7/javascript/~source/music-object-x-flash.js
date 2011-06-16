/**********
     TITLE: Music Object Extension - Flash
   VERSION: 1.2.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x8"*/

function mo_doFSCommand (_command,_args) {
	eval (_command + ((_command.indexOf ('(') == -1) ? ('(' + _args + ')') : ''));
}

function mo_wireFlashMovie (_movieName) {
	window [_movieName + '_DoFSCommand'] = mo_doFSCommand;
	if (Music.playerType == 'ActiveX')
		document.writeln (
			'<SCRIPT LANGUAGE="VBScript">' +
			'Sub ' + _movieName + '_FSCommand (ByVal command, ByVal args)' +
			'call mo_doFSCommand (command, args)' +
			'end sub' +
			'</SCRIPT>'
		)
	;
}

function mo_embedFlashMovie (_attrList) {
	var
		_attribs = Music.MO_flashAttribs,
		_attrNames = new Array ('SRC','WIDTH','HEIGHT','ALIGN','HSPACE','VSPACE','SALIGN','PLAY','LOOP','QUALITY','SCALE','MENU','BGCOLOR','BASE')
	;
	for (var _attrNo = 0; _attrNo < _attrNames.length; _attrNo++) _attribs [_attrNames [_attrNo]] = null;
	_attribs.HSPACE = _attribs.VSPACE = '0';
	Music.parseAttributes (_attrList,_attribs);
	var _embedOutput = '';
	if (Music.playerType == 'Plug-in') {
		_embedOutput = '<EMBED TYPE="application/x-shockwave-flash" PLUGINSPAGE="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash"' + Music.tagAttr (_attribs,'NAME') + ' SWLIVECONNECT=' + (_attribs.NAME != null) + ' MAYSCRIPT';
		for (var _attrNo = 0; _attrNo < _attrNames.length; _attrNo++)
			_embedOutput += Music.tagAttr (_attribs,_attrNames [_attrNo])
		;
		_embedOutput += '>';
	} else if (Music.playerType == 'ActiveX') {
		_embedOutput += '<OBJECT' + ((_attribs.NAME != null) ? (' ID="' + _attribs.NAME + '"') : '') + ' WIDTH=' + _attribs.WIDTH + ' HEIGHT=' + _attribs.HEIGHT + Music.tagAttr (_attribs,'ALIGN') + Music.tagAttr (_attribs,'HSPACE') + Music.tagAttr (_attribs,'VSPACE') + ' CLASSID="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" CODEBASE="http://active.macromedia.com/flash/cabs/swflash.cab">\n' + '<PARAM NAME="movie" VALUE="' + _attribs.SRC + '">\n';
		for (var _attrNo = 0; _attrNo < _attrNames.length; _attrNo++) {
			if (_attribs [_attrNames [_attrNo]] != null)
				_embedOutput += '<PARAM NAME="' + _attrNames [_attrNo] + '" VALUE="' + _attribs [_attrNames [_attrNo]] + '">\n'
			;
		}
		_embedOutput += '</OBJECT>';
	}
	document.writeln (_embedOutput);
	if (_attribs.NAME != null) mo_wireFlashMovie (_attribs.NAME);
}

/*** Private Static Properties ***/

Music.MO_flashAttribs = new Array ();

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('embedFlashMovie','wireFlashMovie');
