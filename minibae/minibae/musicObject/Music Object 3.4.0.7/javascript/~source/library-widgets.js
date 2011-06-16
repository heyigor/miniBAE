/**********
     TITLE: Graphic Widgets Library
   VERSION: 1.2.0
      TYPE: Code Library
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

isJavaScript1_1 = typeof Image != null;
baseURL = window.location.href;
baseURL = baseURL.substring (0,baseURL.lastIndexOf ('/') + 1);

/*** Slider Object Code ***/

function Slider_getNotch (_sliderValue) {
	return this.notches - (Math.floor ((_sliderValue * this.notches) / (this.maxValue - this.minValue + 1)) + 1) + 1;
}

function Slider_setValue (_newValue) {
	var _this = this;
	_newValue = Math.min (_this.maxValue,Math.max (_this.minValue,_newValue));
	if (_newValue != _this.value) {
		_this.value = _newValue;
		_this.update (_this.getNotch (_this.value));
	}
}

function Slider_update (_notchID) {
	var _this = this;
	if (_notchID != _this.currentNotch) {
		var _lastNotch = _this.currentNotch;
		_this.currentNotch = _notchID;
		if (isJavaScript1_1) {
			document [_this.objectName + 'Notch' + _this.currentNotch].src = _this.nob.src;
			document [_this.objectName + 'Notch' + _lastNotch].src = _this.back.src;
		}
	}
}

function Slider_mouseout () {
	this.blurTimeout = setTimeout (this.objectName + '.blur ()',250);
}

function Slider_mouseover (_notchID) {
	var _this = this;
	if (_this != Slider_currentFocus && Slider_currentFocus != null) Slider_currentFocus.blur ();
	if (_this.focused) {
		clearTimeout (_this.blurTimeout);
		_this.update (_notchID);
		_this.value = _this.minValue + Math.round ((_this.maxValue - _this.minValue) * (_this.notches - _this.currentNotch) / (_this.notches - 1));
		eval (_this.onChange);
	}
}

function Slider_blur () {
	var _this = this;
	if (_this.focused) {
		clearTimeout (_this.blurTimeout);
		if (isJavaScript1_1) {
			var _prefix = _this.objectName + 'Frame';
			document [_prefix + 'TL'].src = _this.frameBlurTL.src;
			document [_prefix + 'TR'].src = _this.frameBlurTR.src;
			document [_prefix + 'HT'].src = _this.frameBlurH.src;
			document [_prefix + 'VL'].src = _this.frameBlurV.src;
			document [_prefix + 'VR'].src = _this.frameBlurV.src;
			document [_prefix + 'BL'].src = _this.frameBlurBL.src;
			document [_prefix + 'BR'].src = _this.frameBlurBR.src;
			document [_prefix + 'HB'].src = _this.frameBlurH.src;
		}
		_this.focused = false;
	}
	Slider_currentFocus = null;
}

function Slider_focus () {
	var _this = this;
	if (_this != Slider_currentFocus) {
		if (Slider_currentFocus != null) Slider_currentFocus.blur ();
		Slider_currentFocus = _this;
		clearTimeout (_this.blurTimeout);
		if (isJavaScript1_1) {
			var _prefix = _this.objectName + 'Frame';
			document [_prefix + 'TL'].src = _this.frameFocusTL.src;
			document [_prefix + 'TR'].src = _this.frameFocusTR.src;
			document [_prefix + 'HT'].src = _this.frameFocusH.src;
			document [_prefix + 'VL'].src = _this.frameFocusV.src;
			document [_prefix + 'VR'].src = _this.frameFocusV.src;
			document [_prefix + 'BL'].src = _this.frameFocusBL.src;
			document [_prefix + 'BR'].src = _this.frameFocusBR.src;
			document [_prefix + 'HB'].src = _this.frameFocusH.src;
		}
		_this.focused = true;
	}
}

function Slider_click (_notchID) {
	if (!this.focused) this.focus (); else this.blur ();
	this.mouseover (_notchID);
}

function Slider (_objectName,_onChange,_minValue,_maxValue,_initValue,_notches,_width,_height,_frameWidth,_imagesPath) {
	var _this = this;
	if (typeof _imagesPath != 'string') _imagesPath = 'slider/';
	/*** Constructor Properties ***/
	_this.objectName = _objectName;
	_this.onChange = _onChange;
	_this.minValue = _minValue;
	_this.maxValue = _maxValue;
	_this.value = _initValue;
	_this.notches = _notches;
	_this.width = _width;
	_this.height = _height;
	_this.frameWidth = _frameWidth;
	_this.imagesPath = _imagesPath;

	/*** Internal Methods ***/
	_this.getNotch = Slider_getNotch;
	_this.update = Slider_update;
	_this.mouseover = Slider_mouseover;
	_this.mouseout = Slider_mouseout;
	_this.click = Slider_click;

	/*** Exposed Methods ***/
	_this.focus = Slider_focus;
	_this.blur = Slider_blur;
	_this.setValue = Slider_setValue;

	/*** Instance State Variables ***/
	if (isJavaScript1_1) {
		var _prefix = baseURL + _this.imagesPath;

		_this.frameBlurTL = new Image (_frameWidth,_frameWidth);
		_this.frameBlurTR = new Image (_frameWidth,_frameWidth);
		_this.frameBlurBL = new Image (_frameWidth,_frameWidth);
		_this.frameBlurBR = new Image (_frameWidth,_frameWidth);
		_this.frameBlurH = new Image (_width,_frameWidth);
		_this.frameBlurV = new Image (_frameWidth,_height * _this.notches);
		_this.frameBlurTL.src = _prefix + 'blur-tl.gif';
		_this.frameBlurTR.src = _prefix + 'blur-tr.gif';
		_this.frameBlurBL.src = _prefix + 'blur-bl.gif';
		_this.frameBlurBR.src = _prefix + 'blur-br.gif';
		_this.frameBlurH.src = _prefix + 'blur-h.gif';
		_this.frameBlurV.src = _prefix + 'blur-v.gif';

		_this.frameFocusTL = new Image (_frameWidth,_frameWidth);
		_this.frameFocusTR = new Image (_frameWidth,_frameWidth);
		_this.frameFocusBL = new Image (_frameWidth,_frameWidth);
		_this.frameFocusBR = new Image (_frameWidth,_frameWidth);
		_this.frameFocusH = new Image (_width,_frameWidth);
		_this.frameFocusV = new Image (_frameWidth,_height * _notches);
		_this.frameFocusTL.src = _prefix + 'focus-tl.gif';
		_this.frameFocusTR.src = _prefix + 'focus-tr.gif';
		_this.frameFocusBL.src = _prefix + 'focus-bl.gif';
		_this.frameFocusBR.src = _prefix + 'focus-br.gif';
		_this.frameFocusH.src = _prefix + 'focus-h.gif';
		_this.frameFocusV.src = _prefix + 'focus-v.gif';

		_this.nob = new Image (_width,_height);
		_this.nob.src = _prefix + 'nob.gif';
		_this.back = new Image (_width,_height);
		_this.back.src = _prefix + 'back.gif';
	}
	_this.currentNotch = _this.getNotch (_this.value);
	_this.focused = false;
	_this.blurTimeout = setTimeout ('//',0);

	/*** Initialisation ***/
	document.writeln (
		'<TABLE BORDER=0 CELLSPACING=0 CELLPADDING=0>' +
		'<TR><TD><IMG SRC="' + _imagesPath + 'blur-tl.gif" WIDTH=' + _frameWidth + ' HEIGHT=' + _frameWidth + ' NAME="' + _objectName +
		'FrameTL"></TD><TD><IMG SRC="' + _imagesPath + 'blur-h.gif" WIDTH=' + _width + ' HEIGHT=' + _frameWidth + ' NAME="' + _objectName +
		'FrameHT"></TD><TD><IMG SRC="' + _imagesPath + 'blur-tr.gif" WIDTH=' + _frameWidth + ' HEIGHT=' + _frameWidth +
		' NAME="' + _objectName + 'FrameTR"></TD></TR>' + '<TR><TD><IMG SRC="' + _imagesPath + 'blur-v.gif" WIDTH=' + _frameWidth +
		' HEIGHT=' + _height * _notches + ' NAME="' + _objectName + 'FrameVL"></TD><TD>'
	);
	for (var _notchNo = 1; _notchNo <= _notches; _notchNo++) {
		document.writeln (
			'<A HREF="javascript://" ONCLICK="' + _objectName + '.click (' + _notchNo + '); return false" ONMOUSEOVER="' +
			_objectName + '.mouseover (' + _notchNo + ')" ONMOUSEOUT="' + _objectName +
			'.mouseout ()"><IMG SRC="' + _imagesPath + ((_notchNo == _this.currentNotch) ? 'nob' : 'back') +
			'.gif" BORDER=0 HSPACE=0 VSPACE=0 WIDTH=' + _width + ' HEIGHT=' + _height + ' NAME="'
			+ _objectName + 'Notch' + _notchNo + '"><BR></A>'
		);
	}
	document.writeln (
		'</TD><TD><IMG SRC="' + _imagesPath + 'blur-v.gif" WIDTH=' + _frameWidth + ' HEIGHT=' + _height * _notches +
		' NAME="' + _objectName + 'FrameVR"></TD></TR>' +
		'<TR><TD><IMG SRC="' + _imagesPath + 'blur-bl.gif" WIDTH=' + _frameWidth + ' HEIGHT=' + _frameWidth + ' NAME="' + _objectName +
		'FrameBL"></TD><TD><IMG SRC="' + _imagesPath + 'blur-h.gif" WIDTH=' + _width + ' HEIGHT=' + _frameWidth +
		' NAME="' + _objectName	+ 'FrameHB"></TD><TD><IMG SRC="' + _imagesPath + 'blur-br.gif" WIDTH=' + _frameWidth +
		' HEIGHT=' + _frameWidth + ' NAME="' + _objectName + 'FrameBR"></TD></TR>' +
		'</TABLE>'
	);
	window [_objectName] = _this;
}

Slider_currentFocus = null;
