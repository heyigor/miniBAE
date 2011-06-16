/**********
     TITLE: Music Object Extension - Channels
   VERSION: 1.0.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x1"*/

function mo_midiMsbLsbToInt (_MSB,_LSB) {
	return _MSB << 7 + _LSB;
}

function mo_setChannelParameter (_channelNo,_paramNumber,_value,_registered) {
	var _offset = _registered ? 2 : 0;
	this.setController (_channelNo,99 + _offset,_paramNumber >> 7);
	this.setController (_channelNo,98 + _offset,_paramNumber & 127);
	this.setController (_channelNo,6,_value);
	this.MO_channelParameters [_registered] [_channelNo + ',' + _paramNumber] = _value;
}

function mo_getChannelParameter (_channelNo,_paramNumber,_registered) {
	var
		_paramsArray = this.MO_channelParameters [_registered],
		_paramIndex = _channelNo + ',' + _paramNumber
	;
	return (typeof _paramsArray [_paramIndex] != 'undefined') ? _paramsArray [_paramIndex] : -1;
}

function mo_setChannelRpn (_channelNo,_paramNumber,_value) {
	this.setChannelParameter (_channelNo,_paramNumber,_value,true);
}

function mo_getChannelRpn (_channelNo,_paramNumber) {
	return this.getChannelParameter (_channelNo,_paramNumber,true);
}

function mo_setChannelNrpn (_channelNo,_paramNumber,_value) {
	this.setChannelParameter (_channelNo,_paramNumber,_value,false);
}

function mo_getChannelNrpn (_channelNo,_paramNumber) {
	return this.getChannelParameter (_channelNo,_paramNumber,false);
}

function mo_setChannelMode (_channelNo,_mode) {
	this.setChannelNrpn (_channelNo,640,_mode);
}

function mo_getChannelMode (_channelNo) {
	return this.getChannelNrpn (_channelNo,640);
}

function mo_setChannelPitchBendRange (_channelNo,_pitchBendRange) {
	this.setChannelRpn (_channelNo,0,_pitchBendRange);
}

function mo_getChannelPitchBendRange (_channelNo) {
	return this.getChannelRpn (_channelNo,0);
}

function mo_resetAllControllers (_channelNo) {
	this.setController (_channelNo,121,0);
}

function mo_getControllerName (_controllerNo) {
	return (typeof Music.controllerNames [_controllerNo] != 'undefined') ? Music.controllerNames [_controllerNo] : ('MIDI Controller ' + _controllerNo);
}

/*** Instance Extender ***/

function MO_channelsInstanceExtender (_this) {
	_this.MO_channelParameters = new Array ();
	_this.MO_channelParameters [false] = new Array ();
	_this.MO_channelParameters [true] = new Array ();
}

Music.addInstanceExtender (MO_channelsInstanceExtender);

/*** Initialization ***/

function MO_initialize () {
	/*** Public Static Properties ***/
	var _controllerNames = Music.controllerNames = new Array ();
	_controllerNames [0] = 'Bank Number';
	_controllerNames [1] = 'Modulation';
	_controllerNames [7] = 'Volume';
	_controllerNames [10] = 'Pan';
	_controllerNames [11] = 'Expression';
	_controllerNames [91] = 'Reverb Level';
	_controllerNames [93] = 'Chorus Level';
	/*** Channel Controller Get & Set Methods ***/
	for (var _controllerNo in _controllerNames) {
		var
			_controllerID = _controllerNames [_controllerNo].replace (/ /gi,''),
			_suffix = 'Channel' + _controllerID
		;
		window ['mo_get' + _suffix] = new Function (
			'channelNo',
			'return this.getController (channelNo,' + _controllerNo + ')'
		);
		window ['mo_set' + _suffix] = new Function (
			'channelNo',
			'controllerValue',
			'this.setController (channelNo,' + _controllerNo + ',controllerValue)'
		);
		Music.addPublicInstanceMethods ('get' + _suffix,'set' + _suffix);
	}
}

MO_initialize ();

/*** Private Instance Methods ***/

Music.addPrivateInstanceMethods ('getChannelParameter','setChannelParameter');

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('getChannelMode','getChannelNrpn','getChannelPitchBendRange','getChannelRpn','resetAllControllers','setChannelMode','setChannelNrpn','setChannelPitchBendRange','setChannelRpn');

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('getControllerName','midiMsbLsbToInt');
