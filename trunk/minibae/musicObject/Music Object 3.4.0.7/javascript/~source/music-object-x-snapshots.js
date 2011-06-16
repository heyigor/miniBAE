/**********
     TITLE: Music Object Extension - Snapshots
   VERSION: 1.0.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 2000-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_xf"*/

function MO_Controller (_number,_name) {
	this._number = _number;
	this._name = _name;
}

function mo_getEntireGroupState (_groupType,_stateType) {
	var
		_getState = 'get' + _groupType + _stateType,
		_maxMembers = _groupType == 'Channel' ? 16 : 64,
		_groupState = new Array ()
	;
	for (var _memberNo = 1; _memberNo <= _maxMembers; _memberNo++)
		_groupState [_memberNo] = this [_getState] (_memberNo)
	;
	return _groupState;
}

function mo_setEntireGroupState (_groupType,_stateType,_groupState) {
	var
		_setState = 'set' + _groupType + _stateType,
		_maxMembers = _groupType == 'Channel' ? 16 : 64
	;
	for (var _memberNo = 1; _memberNo <= _maxMembers; _memberNo++)
		this [_setState] (_memberNo,_groupState [_memberNo])
	;
}

function mo_isSnapshotDefined (_snapshotName) {
	return typeof this.snapshots != 'undefined' && typeof this.snapshots [_snapshotName] != 'undefined';
}

function mo_captureSnapshot (_snapshotName,_snapshotFilter) {
	var _this = this;
	if (typeof _this.snapshots == 'undefined') _this.snapshots = new Array ();
	if (!_this.isSnapshotDefined (_snapshotName)) {
		var _snapshot = _this.snapshots [_snapshotName] = new Object ();
		_snapshot.controllers = new Array ();
		_snapshot.filter = new Array ();
		_this.resetSnapshotFilter (_snapshotName,'OFF');
		_this.setSnapshotFilter (_snapshotName,_snapshotFilter);
	} else {
		if (typeof _snapshotFilter == 'string') _this.setSnapshotFilter (_snapshotName,_snapshotFilter);
	}
	_snapshot.volume = _this.getVolume ();
	_snapshot.transpose = _this.getTranspose ();
	_snapshot.tempo = _this.getTempo ();
	_snapshot.position = _this.getPosition ();
	_snapshot.channelMute = _this.getEntireGroupState ('Channel','Mute');
	_snapshot.channelSolo = _this.getEntireGroupState ('Channel','Solo');
	_snapshot.trackMute = _this.getEntireGroupState ('Track','Mute');
	_snapshot.trackSolo = _this.getEntireGroupState ('Track','Solo');
	for (var _controllerNo = 0; _controllerNo < Music.controllers.length; _controllerNo++) {
		var _controller = Music.controllers [_controllerNo];
		if (typeof _snapshot.controllers [_controller._name] == 'undefined') _snapshot.controllers [_controller._name] = new Array ();
		var _controllerState = _snapshot.controllers [_controller._name];
		for (var _channelNo = 1; _channelNo < 17; _channelNo++)
			_controllerState [_channelNo] = _this.getController [_channelNo,_controller._number]
		;
	}
}

function MO_stringToBoolean (_value) {
	_value = _value.toUpperCase ();
	return _value == 'TRUE' || _value == 'ON' || _value == '1';
}

function mo_recallSnapshot (_snapshotName,_snapshotFilter) {
	var _this = this;
	if (_this.isSnapshotDefined (_snapshotName)) {
		if (typeof _snapshotFilter == 'string') _this.setSnapshotFilter (_snapshotName,_snapshotFilter);
		var
			_snapshot = _this.snapshots [_snapshotName],
			_filter = _snapshot.filter
		;
		if (MO_stringToBoolean (_filter.VOLUME) _this.setVolume (_snapshot.volume);
		if (MO_stringToBoolean (_filter.TRANSPOSE) _this.setTranspose (_snapshot.transpose);
		if (MO_stringToBoolean (_filter.TEMPO) _this.setTempo (_snapshot.tempo);
		if (MO_stringToBoolean (_filter.POSITION) _this.setPosition (_snapshot.position);
		if (MO_stringToBoolean (_filter.CHANNELMUTE) _this.setEntireGroupState ('Channel','Mute',_snapshot.channelMute);
		if (MO_stringToBoolean (_filter.CHANNELSOLO) _this.setEntireGroupState ('Channel','Solo',_snapshot.channelSolo);
		if (MO_stringToBoolean (_filter.TRACKMUTE) _this.setEntireGroupState ('Track','Mute',_snapshot.trackMute);
		if (MO_stringToBoolean (_filter.TRACKSOLO) _this.setEntireGroupState ('Track','Solo',_snapshot.trackSolo);
		for (var _controllerNo = 0; _controllerNo < Music.controllers.length; _controllerNo++) {
			var _controller = Music.controllers [_controllerNo];
			if (MO_stringToBoolean (_filter [_controller._name]) {
				var _controllerState = _snapshot.controllers [_controller._name];
				for (var _channelNo = 1; _channelNo < 17; _channelNo++)
					_this.setController [_channelNo,_controller._number,_controllerState [_channelNo]]
				;
			}
		}
	}
}

function mo_removeSnapshot (_snapshotName) {
	if (this.isSnapshotDefined (_snapshotName)) this.snapshots [_snapshotName] = null;
}

function mo_getSnapshotFilter (_snapshotName) {
	var _result = '';
	if (this.isSnapshotDefined (_snapshotName)) {
		var _snapshot = this.snapshots [_snapshotName];
		for (var _filterOptionNo = 0; _filterOptionNo < Music.snapshotFilterOptions.length; _filterOptionNo++) {
			var _filterOptionName = Music.snapshotFilterOptions [_filterOptionNo];
			_result += ((_result != '') ? ' ' : '') + _filterOptionName + '=' + (_snapshot.filter [_filterOptionName] + '').toUpperCase ();
		}
	}
	return _result;
}

function mo_setSnapshotFilter (_snapshotName,_snapshotFilter) {
	if (this.isSnapshotDefined (_snapshotName)) {
		Music.parseAttributes ((typeof _snapshotFilter == 'string') ? _snapshotFilter : Music.defaultSnapshotFilter,this.snapshots [_snapshotName].filter);
	}
}

function mo_resetSnapshotFilter (_snapshotName,_state) {
	if (typeof _state == 'undefined') _state = 'ON';
	if (this.isSnapshotDefined (_snapshotName)) {
		_state = _state + '';
		var _snapshot = this.snapshots [_snapshotName];
		for (var _filterOptionNo = 0; _filterOptionNo < Music.snapshotFilterOptions.length; _filterOptionNo++)
			_snapshot.filter [Music.snapshotFilterOptions [_filterOptionNo]] = _state
		;
	}
}

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('captureSnapshot','getSnapshotFilter','isSnapshotDefined','recallSnapshot','removeSnapshot','resetSnapshotFilter','setSnapshotFilter');

/*** Private Instance Methods ***/

Music.addPrivateInstanceMethods ('getEntireGroupState','setEntireGroupState');

/*** Public Static Properties ***/

Music.defaultSnapshotFilter = 'CHANNELMUTE=ON CHANNELSOLO=ON CHANNELMODULATION=ON CHANNELVOLUME=ON CHANNELPAN=ON CHANNELEXPRESSION=ON CHANNELSUSTAIN=ON CHANNELREVERBTYPE=ON CHANNELREVERBLEVEL=ON CHANNELCHORUSLEVEL=ON POSITION=ON TEMPO=ON TRANSPOSE=ON VOLUME=ON';

/*** Private Static Properties ***/

Music.controllers = new Array (
	new MO_Controller (1,'CHANNELMODULATION'),
	new MO_Controller (7,'CHANNELVOLUME'),
	new MO_Controller (10,'CHANNELPAN'),
	new MO_Controller (11,'CHANNELEXPRESSION'),
	new MO_Controller (64,'CHANNELSUSTAIN'),
	new MO_Controller (90,'CHANNELREVERBTYPE'),
	new MO_Controller (91,'CHANNELREVERBLEVEL'),
	new MO_Controller (93,'CHANNELCHORUSLEVEL')
);

Music.snapshotFilterOptions = new Array ('CHANNELMUTE','CHANNELSOLO','CHANNELMODULATION','CHANNELVOLUME','CHANNELPAN','CHANNELEXPRESSION','CHANNELSUSTAIN','CHANNELREVERBTYPE','CHANNELREVERBLEVEL','CHANNELCHORUSLEVEL','POSITION','TEMPO','TRANSPOSE','VOLUME');
