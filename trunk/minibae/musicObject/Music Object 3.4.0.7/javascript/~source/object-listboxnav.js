/**********
     TITLE: ListboxNav Object
   VERSION: 1.4.0
      TYPE: Object Definition
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: library-url.js
**********/

/*ScruncherSettings Mappings=",ListboxNav"*/

function ListboxNav_Item (_title,_url) {
	/*** Constructor parameters ***/
	if (typeof _title != 'undefined' && typeof _url != 'undefined') {
		this._title = _title;
		this._url = _url;
	} else {
		this._title = ' ';
		this._url = '';
	}
}

function ListboxNav_pageArrived () {
	var
		_this = this,
		_newCurrentItem = -1,
		_selectHandle = document [_this._objectName + 'Form'] [_this._objectName + 'Select'],
		_arrivedURL = _this._viewFrame.location.href.toForwardSlashes ()
	;
	if (_arrivedURL.indexOf ('#') != -1) _arrivedURL = _arrivedURL.substr (0,_arrivedURL.indexOf ('#'));
	for (var _itemNo = 0; _itemNo < _this._items.length; _itemNo++) {
		if (_arrivedURL == baseURL + _this._items [_itemNo]._url) {
			_newCurrentItem = _itemNo;
			break;
		}
	}
	if (_newCurrentItem != -1 && _newCurrentItem != _this._currentItem) {
		_this._currentItem = _newCurrentItem;
		for (var _optionNo = 0; _optionNo < _selectHandle.options.length; _optionNo++) {
			if (_selectHandle.options [_optionNo].text == _this._items [_this._currentItem]._title) {
				_selectHandle.selectedIndex = _optionNo;
				break;
			}
		}
		_this._updateArrows ();
	}
}

function ListboxNav_select () {
	var
		_this = this,
		_selectHandle = document [_this._objectName + 'Form'] [_this._objectName + 'Select'],
		_currentTitle = _selectHandle.options [_selectHandle.selectedIndex].text,
		_itemFound = false
	;
	if (_currentTitle != '' && _currentTitle != ' ') {
		for (var _itemNo = 0; _itemNo < _this._items.length && !_itemFound; _itemNo++)
			_itemFound = _this._items [_itemNo]._title == _currentTitle
		;
	}
	if (_itemFound) {
		_this._currentItem = _itemNo - 1;
		_this._viewFrame.location.href = baseURL + _this._items [_this._currentItem]._url;
		_this._updateArrows ();
	} else {
		_selectHandle.selectedIndex = _this._currentItem;
	}
}

function ListboxNav_advance (_direction) {
	var
		_this = this,
		_selectHandle = document [_this._objectName + 'Form'] [_this._objectName + 'Select'],
		_options = _selectHandle.options,
		_newOptionNo = _selectHandle.selectedIndex,
		_noEntryFound = true
	;
	while (_noEntryFound) {
		_newOptionNo += _direction;
		if (_newOptionNo < 0) {
			_newOptionNo = _options.length - 1
		} else if (_newOptionNo > _options.length - 1) {
			_newOptionNo = 0;
		}
		_noEntryFound = _options [_newOptionNo].text == '' || _options [_newOptionNo].text == ' ';
	}
	_selectHandle.selectedIndex = _newOptionNo;
	_this.select ();
}

function ListboxNav_selectEntry (_entryText) {
	var
		_this = this,
		_selectHandle = document [_this._objectName + 'Form'] [_this._objectName + 'Select']
	;
	for (var _entryNo = 0; _entryNo < _selectHandle.options.length; _entryNo++) {
		if (_selectHandle.options [_entryNo].text == _entryText) {
			_selectHandle.selectedIndex = _entryNo;
			_this.select ();
			break;
		}
	}
}

function ListboxNav_isArrowActive (_direction) {
	return this._navigationWrapping || this._currentItem != ((_direction == -1) ? 0 : (this._items.length - 1));
}

function ListboxNav_updateArrow (_direction) {
	var
		_this = this,
		_arrow = _this._arrows [_direction],
		_state = _arrow._isClicked ? 'clicked' : (_this._isArrowActive (_direction) ? (_arrow._isOver ? 'over' : 'normal') : 'grayed')
	;
	if (_state != _arrow._state) {
		_arrow._state = _state;
		document [_this._objectName + 'Form'] [_this._objectName + '_' + _this._directionNames [_direction]].src = _arrow [_state].src;
	}
}

function ListboxNav_updateArrows () {
	this._updateArrow (-1);
	this._updateArrow (1);
}

function ListboxNav_arrowOver (_direction) {
	var _this = this;
	clearTimeout (_this._unclickTimeout);
	_this._arrows [_direction]._isOver = true;
	_this._arrows [_direction]._isClicked = false;
	if (_this._isArrowActive (_direction)) window.status = (_direction == -1) ? 'PREVIOUS' : 'NEXT';
	_this._updateArrows ();
}

function ListboxNav_arrowOut (_direction) {
	var _this = this;
	clearTimeout (_this._unclickTimeout);
	_this._arrows [_direction]._isOver = false;
	_this._arrows [_direction]._isClicked = false;
	if (_this._isArrowActive (_direction)) window.status = window.defaultStatus;
	_this._updateArrows ();
}

function ListboxNav_arrowClick (_direction) {
	var _this = this;
	clearTimeout (_this._unclickTimeout);
	_this._arrows [_direction]._isOver = false;
	_this._arrows [_direction]._isClicked = true;
	if (_this._isArrowActive (_direction)) {
		_this._advance (_direction);
		_this._unclickTimeout = setTimeout (_this._objectName + '.arrowOver (' + _direction + ')',150);
	}
}

function ListboxNav_generateNavigation () {
	var
		_this = this,
		_outputStr = ''
	;
	window.onload = new Function (_this._objectName + '.pageArrived ()');
	_outputStr =
		'<CENTER>' +
		'<TABLE WIDTH=100% BORDER=0 CELLSPACING=0 CELLPADDING=2>' +
		((_this._title != '') ? ('<TR VALIGN=TOP><TD ALIGN=CENTER><FONT FACE="ARIAL,HELVETICA,VERDANA" SIZE=4>' + _this._title + '</FONT></TD></TR>') : '') +
		'<TR>' +
		'<TD ALIGN=CENTER VALIGN=CENTER>' +
		'<FORM NAME="' + _this._objectName + 'Form">' +
		'<TABLE BORDER=0 CELLSPACING=0 CELLPADDING=0><TR VALIGN=CENTER>' +
		'<TD><A HREF="javascript:' + _this._objectName + '.arrowClick (-1)" ONCLICK="' + _this._objectName + '.arrowClick (-1); return false" ONMOUSEOVER="' + _this._objectName + '.arrowOver (-1); return true" ONMOUSEOUT="' + _this._objectName + '.arrowOut (-1); return true"><IMG SRC="' + _this._imagePath + 'arrow-' + _this._directionNames [-1] + (_this._isArrowActive (-1) ? '' : '-grayed') + '.gif" NAME="' + _this._objectName + '_' + _this._directionNames [-1] + '" WIDTH=' + _this._arrowWidth + ' HEIGHT=' + _this._arrowHeight + ' HSPACE=5 ALT="PREVIOUS" BORDER=0 HSPACE=0 VSPACE=5></A></TD>' +
		'<TD>' +
		'<SELECT NAME="' + _this._objectName + 'Select" ONCHANGE="' + _this._objectName + '.select ()">'
	;
	for (var _itemNo = 0; _itemNo < _this._items.length; _itemNo++)
		_outputStr += '<OPTION>' + _this._items [_itemNo]._title
	;
	_outputStr +=
		'</SELECT>' +
		'</TD>' +
		'<TD><A HREF="javascript:' + _this._objectName + '.arrowClick (1)" ONCLICK="' + _this._objectName + '.arrowClick (1); return false" ONMOUSEOVER="' + _this._objectName + '.arrowOver (1); return true" ONMOUSEOUT="' + _this._objectName + '.arrowOut (1); return true"><IMG SRC="' + _this._imagePath + 'arrow-' + _this._directionNames [1] + (_this._isArrowActive (1) ? '' : '-grayed') + '.gif" NAME="' + _this._objectName + '_' + _this._directionNames [1] + '" WIDTH=' + _this._arrowWidth + ' HEIGHT=' + _this._arrowHeight + ' HSPACE=5 ALT="NEXT" BORDER=0 HSPACE=0 VSPACE=5></A></TD>' +
		'</TR></TABLE>' +
		'</FORM>' +
		'</TD>' +
		'</TR>' +
		'</TABLE>' +
		'</CENTER>'
	;
	document.writeln (_outputStr);
}

function ListboxNav_gotoSectionAlternatePage (_alternatePage) {
	if (this._currentItem != -1) {
		var _sectionBaseURL = this._items [this._currentItem]._url;
		_sectionBaseURL = _sectionBaseURL.substring (0,_sectionBaseURL.lastIndexOf ('/'));
		this._viewFrame.location.href = _sectionBaseURL + '/' + _alternatePage;
	}
}

function ListboxNav_newArrowImage (_arrowSubURL) {
	var
		_this = this,
		_image = new Image (_this._arrowWidth,_this._arrowHeight)
	;
	_image.src = _this._imagePath + 'arrow-' + _arrowSubURL + '.gif';
	return _image;
}

function ListboxNav (_objectName,_title,_imagePath,_arrowWidth,_arrowHeight,_navigationWrapping,_items) {
	var _this = this;

	/*** Private Instance Methods ***/
	_this._advance = ListboxNav_advance;
	_this.arrowOver = ListboxNav_arrowOver;
	_this.arrowOut = ListboxNav_arrowOut;
	_this.arrowClick = ListboxNav_arrowClick;
	_this._updateArrows = ListboxNav_updateArrows;
	_this._isArrowActive = ListboxNav_isArrowActive;
	_this._newArrowImage = ListboxNav_newArrowImage;
	_this.select = ListboxNav_select;
	_this.selectEntry = ListboxNav_selectEntry;
	_this._updateArrow = ListboxNav_updateArrow;

	/*** Public Instance Methods ***/
	_this.pageArrived = ListboxNav_pageArrived;
	_this.generateNavigation = ListboxNav_generateNavigation;
	_this.gotoSectionAlternatePage = ListboxNav_gotoSectionAlternatePage;

	/*** Private Instance Properties ***/
	_this._objectName = _objectName;
	_this._title = _title;
	_this._imagePath = _imagePath;
	_this._arrowWidth = _arrowWidth;
	_this._arrowHeight = _arrowHeight;
	_this._navigationWrapping = _navigationWrapping;
	_this._items = _items;

	_this._viewFrame = parent [_objectName + 'Body'];
	_this._currentItem = 0;
	_this._categories = new Array ();
	_this._arrows = new Object ();
	var _directionNames = _this._directionNames = new Array ();
	_directionNames [-1] = 'left';
	_directionNames [1] = 'right';
	for (var _direction in _directionNames) {
		var
			_directionName = _directionNames [_direction],
			_arrow = _this._arrows [_direction] = new Object ()
		;
		_arrow.normal = _this._newArrowImage (_directionName);
		_arrow.over = _this._newArrowImage (_directionName + '-over');
		_arrow.clicked = _this._newArrowImage (_directionName + '-clicked');
		if (!_navigationWrapping) _arrow.grayed = _this._newArrowImage (_directionName + '-grayed');
		_arrow._isClicked = _arrow._isOver = false;
		_arrow._state = '';
	}
	_this._unclickTimeout = setTimeout ('//',0);

	/*** Initialisation ***/
	window [_objectName] = _this;
}

/*** Public Static Methods ***/

ListboxNav.Item = ListboxNav_Item;
