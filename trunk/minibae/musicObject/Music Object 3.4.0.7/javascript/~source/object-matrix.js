/**********
     TITLE: Matrix Object
   VERSION: 1.2.0
      TYPE: Object Definition
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

/*ScruncherSettings Mappings=",Matrix"*/

function Matrix_segmentClick (_segmentX,_segmentY) {
	this.segmentOver (_segmentX,_segmentY);
	this._selectFunction (this._valueX,this._valueY);
}

function Matrix_segmentOver (_segmentX,_segmentY) {
	var _this = this;
	if (_segmentX != _this._currentX || _segmentY != _this._currentY) {
		if (_segmentX != _this._currentX) {
			if (_this._currentX != 0)
				document.images [_this._objectName + 'PointerX-' + _this._currentX].src = _this._pointerBlankImgX.src
			;
			_this._currentX = _segmentX;
			document.images [_this._objectName + 'PointerX-' + _this._currentX].src = _this._pointerImgX.src;
			_this._valueX = _this._rangeMinX + (_this._currentX - 1) * (_this._rangeMaxX / (_this._segmentsX - 1));
			if (_this._roundValueX) {_this._valueX = Math.round (_this._valueX)}
			document [_this._objectName + 'Form'].valueX.value = _this._valueX;
		}
		if (_segmentY != _this._currentY) {
			if (_this._currentY != 0)
				document.images [_this._objectName + 'PointerY-' + _this._currentY].src = _this._pointerBlankImgY.src
			;
			_this._currentY = _segmentY;
			document.images [_this._objectName + 'PointerY-' + _this._currentY].src = _this._pointerImgY.src;
			_this._valueY = _this._rangeMinY + (_this._currentY - 1) * (_this._rangeMaxY / (_this._segmentsY - 1));
			if (_this._roundValueY) _this._valueY = Math.round (_this._valueY);
			document [_this._objectName + 'Form'].valueY.value = _this._valueY;
		}
		window.status = this._axisNameX + ' = ' + this._valueX + '   :   ' + this._axisNameY + ' = ' + this._valueY;
	}
}

function Matrix_generate () {
	var
		_this = this,
		_incX = _this._sizeX / _this._segmentsX,
		_incY = _this._sizeY / _this._segmentsY,
		_paramsStr,_posX,_posY
	;
	document.writeln ('<MAP NAME="' + _this._objectName + 'Imagemap">');
	_posY = _this._sizeY;
	for (var _segmentNoY = 1; _segmentNoY <= _this._segmentsY; _segmentNoY++) {
		_posX = 0;
		for (var _segmentNoX = 1; _segmentNoX <= _this._segmentsX; _segmentNoX++) {
			window.defaultStatus = _segmentNoX + ':' + _segmentNoY;
			_paramsStr = ' (' + _segmentNoX + ',' + _segmentNoY + ')';
			document.writeln (
				'<AREA' +
				' SHAPE=RECT' +
				' COORDS=' +
					Math.round (_posX) + ',' +
					Math.round (_posY - _incY) + ',' +
					Math.round (_posX + _incX) + ',' +
					Math.round (_posY) +
				' HREF="javascript:' + _this._objectName + '.segmentClick' + _paramsStr + '"' +
				' ONCLICK="' + _this._objectName + '.segmentClick' + _paramsStr + '; return false"' +
				' ONMOUSEOVER="' + _this._objectName + '.segmentOver' + _paramsStr + '; return true"' +
				'>'
			);
			_posX += _incX;
		}
		_posY -= _incY;
	}
	document.writeln ('</MAP>');
	document.writeln (
		'<FORM NAME="' + _this._objectName + 'Form">' +
		'<TABLE BORDER=0 CELLSPACING=0 CELLPADDING=0 BGCOLOR=000000>' +
		'<TR VALIGN=CENTER>' +
		'<TD>' +
		'<TABLE HEIGHT=' + _this._sizeY + ' BORDER=0 CELLSPACING=0 CELLPADDING=0>' +
		'<TR><TD ALIGN=RIGHT VALIGN=TOP><FONT SIZE=2 COLOR=FF0000>' + _this._rangeMaxY + '&nbsp;&nbsp;</FONT></TD></TR>' +
		'<TR><TD ALIGN=CENTER VALIGN=CENTER><FONT FACE="ARIAL,HELVETICA,VERDANA" COLOR=FFFFFF>' + _this._axisNameY + '</FONT>&nbsp;&nbsp;<BR><INPUT TYPE=text SIZE=5 NAME="valueY" VALUE=""></TD></TR>' +
		'<TR><TD ALIGN=RIGHT VALIGN=BOTTOM><FONT SIZE=2 COLOR=FF0000>' + _this._rangeMinY + '&nbsp;&nbsp;</FONT></TD></TR>' +
		'</TABLE>' +
		'</TD><TD>' +
		'<TABLE BORDER=0 CELLSPACING=0 CELLPADDING=0 BGCOLOR=333333>'
	);
	for (var _segmentNoY = _this._segmentsY; _segmentNoY >= 1; _segmentNoY--) {
		document.writeln ('<TR><TD><IMG SRC="images/black.gif" WIDTH=' + _this._segmentSizeY + ' HEIGHT=' + _this._segmentSizeY + ' NAME="' + _this._objectName + 'PointerY-' + _segmentNoY + '" BORDER=0 HSPACE=0 VSPACE=1></TD></TR>');
	}
	document.writeln (
		'</TABLE>' +
		'</TD>' +
		'<TD><IMG SRC="images/matrix-bg.gif" WIDTH=' + _this._sizeX + ' HEIGHT=' + _this._sizeY + ' BORDER=0 HSPACE=3 VSPACE=3 USEMAP="#' + _this._objectName + 'Imagemap"></TD>' +
		'</TR>' +
		'<TR VALIGN=CENTER ALIGN=CENTER><TD COLSPAN=2 ROWSPAN=2><IMG SRC="images/black.gif" WIDTH=1 HEIGHT=1 BORDER=0 HSPACE=0 VSPACE=0></TD><TD>' +
		'<TABLE BORDER=0 CELLSPACING=0 CELLPADDING=0 BGCOLOR=333333><TR>'
	);
	for (var _segmentNoX = 1; _segmentNoX <= _this._segmentsX; _segmentNoX++) {
		document.writeln ('<TD><IMG SRC="images/black.gif" WIDTH=' + _this._segmentSizeX + ' HEIGHT=' + _this._segmentSizeX + ' NAME="' + _this._objectName + 'PointerX-' + _segmentNoX + '" BORDER=0 HSPACE=1 VSPACE=0></TD>');
	}
	document.writeln (
		'</TR></TABLE>' +
		'</TD></TR><TR VALIGN=CENTER ALIGN=CENTER>' +
		'<TD>' +
		'<TABLE WIDTH=' + _this._sizeX + ' BORDER=0 CELLSPACING=0 CELLPADDING=0><TR VALIGN=TOP>' +
		'<TD ALIGN=LEFT><FONT SIZE=2 COLOR=FF0000>' + _this._rangeMinX + '</FONT></TD>' +
		'<TD ALIGN=CENTER><FONT FACE="ARIAL,HELVETICA,VERDANA" COLOR=FFFFFF>' + _this._axisNameX + '</FONT><BR><INPUT TYPE=text SIZE=5 NAME="valueX" VALUE=""></TD>' +
		'<TD ALIGN=RIGHT><FONT SIZE=2 COLOR=FF0000>' + _this._rangeMaxX + '</FONT></TD>' +
		'</TR></TABLE>' +
		'</TD>' +
		'</TR></TABLE>' +
		'</FORM>'
	);
}

function Matrix (
		_objectName,
		_axisNameX,_axisNameY,
		_roundValueX,_roundValueY,
		_rangeMinX,_rangeMaxX,
		_rangeMinY,_rangeMaxY,
		_sizeX,_sizeY,
		_segmentsX,_segmentsY,
		_selectFunction
	) {
	/*** Private Instance Properties ***/
	this._objectName = _objectName;
	this._axisNameX = _axisNameX;
	this._axisNameY = _axisNameY;
	this._roundValueX = _roundValueX;
	this._roundValueY = _roundValueY;
	this._rangeMinX = _rangeMinX;
	this._rangeMaxX = _rangeMaxX;
	this._rangeMinY = _rangeMinY;
	this._rangeMaxY = _rangeMaxY;
	this._sizeX = _sizeX;
	this._sizeY = _sizeY;
	this._segmentsX = _segmentsX;
	this._segmentsY = _segmentsY;
	this._selectFunction = _selectFunction;

	this._currentX = 0;
	this._currentY = 0;
	this._segmentSizeX = Math.floor ((_sizeX - (_segmentsX - 1) * 2) / _segmentsX);
	this._segmentSizeY = Math.floor ((_sizeY - (_segmentsY - 1) * 2) / _segmentsY);
	this._pointerImgX = new Image (this._segmentSizeX,this._segmentSizeX);
	this._pointerImgX.src = 'images/matrix-pointer-x.gif';
	this._pointerBlankImgX = new Image (this._segmentSizeX,this._segmentSizeX);
	this._pointerBlankImgX.src = 'images/black.gif';
	this._pointerImgY = new Image (this._segmentSizeY,this._segmentSizeY);
	this._pointerImgY.src = 'images/matrix-pointer-y.gif';
	this._pointerBlankImgY = new Image (this._segmentSizeY,this._segmentSizeY);
	this._pointerBlankImgY.src = 'images/black.gif';
	this._valueX = null;
	this._valueY = null;

	/*** Public Instance Methods ***/
	this.segmentOver = Matrix_segmentOver;
	this.segmentClick = Matrix_segmentClick;
	this.generate = Matrix_generate;

	/*** Initialization ***/
	window [_objectName] = this;
}
