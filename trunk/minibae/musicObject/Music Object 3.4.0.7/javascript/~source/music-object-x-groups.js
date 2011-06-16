/**********
     TITLE: Music Object Extension - Groups
   VERSION: 1.1.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x9"*/

function mo_getGroupState (_groupType,_stateType,_groupMembers) {
	var
		_getState = 'get' + _groupType + _stateType,
		_maxMembers = _groupType == 'Channel' ? 16 : 64
	;
	if (typeof _groupMembers == 'number') {
		if (_groupMembers == 0) {
			for (var _memberNo = 1; _memberNo <= _maxMembers; _memberNo++) {
				if (!this [_getState] (_memberNo)) return false;
			}
			return true;
		} else {
			return this [_getState] (_groupMembers);
		}
	} else if (typeof _groupMembers == 'string') {
		var _members = _groupMembers.split (',');
		for (var _memberNo = 0; _memberNo < _members.length; _memberNo++) {
			if (!this.getGroupState (_groupType,_stateType,_members [_memberNo] - 0)) return false;
		}
		return true;
	} else {
		return null;
	}
}

function mo_setGroupState (_groupType,_stateType,_groupMembers,_state,_exclusive) {
	_exclusive = typeof _exclusive == 'boolean' && _exclusive;
	var
		_setState = 'set' + _groupType + _stateType,
		_getState = 'get' + _groupType + _stateType,
		_maxMembers = _groupType == 'Channel' ? 16 : 64
	;
	if (typeof _state == 'undefined' || _state == null) _state = true;
	if (typeof _state == 'string' && (_state == 'Toggle Group' || _exclusive)) _state = _state == _stateType || (_state.indexOf ('Toggle') == 0 && !this.getGroupState (_groupType,_stateType,_groupMembers));
	if (typeof _groupMembers == 'number' && _groupMembers == 0) {
		for (var _memberNo = 1; _memberNo <= _maxMembers; _memberNo++)
			this [_setState] (_memberNo,(typeof _state == 'string') ? (_state == _stateType || (_state == 'Toggle' && !this [_getState] (_memberNo))) : _state)
		;
	} else {
		if (typeof _groupMembers == 'string') {
			var _members = _groupMembers.split (',');
		} else {
			var _members = new Array ();
			if (typeof _groupMembers == 'number') _members [0] = _groupMembers;
		}
		if (_exclusive) {
			var _memberSet = new Array ();
			for (var _memberNo = 0; _memberNo < _members.length; _memberNo++)
				_memberSet [_members [_memberNo - 0]] = true;
			;
			for (var _memberNo = 1; _memberNo <= _maxMembers; _memberNo++)
				this [_setState] (_memberNo,(typeof _memberSet [_memberNo] == 'boolean') ? _state : !_state)
			;
		} else {
			for (var _memberNo = 0; _memberNo < _members.length; _memberNo++)
				this [_setState] (_members [_memberNo] - 0,(typeof _state == 'string') ? (_state == _stateType || (_state == 'Toggle' && !this [_getState] (_members [_memberNo] - 0))) : _state)
			;
		}
	}
}

function mo_getChannelsMute (_channelGroup) {
	return this.getGroupState ('Channel','Mute',_channelGroup);
}

function mo_getChannelsSolo (_channelGroup) {
	return this.getGroupState ('Channel','Solo',_channelGroup);
}

function mo_getTracksMute (_trackGroup) {
	return this.getGroupState ('Track','Mute',_trackGroup);
}

function mo_getTracksSolo (_trackGroup) {
	return this.getGroupState ('Track','Solo',_trackGroup);
}

function mo_setChannelsMute (_channelGroup,_state,_exclusive) {
	this.setGroupState ('Channel','Mute',_channelGroup,_state,_exclusive);
}

function mo_setChannelsSolo (_channelGroup,_state,_exclusive) {
	this.setGroupState ('Channel','Solo',_channelGroup,_state,_exclusive);
}

function mo_setTracksMute (_trackGroup,_state,_exclusive) {
	this.setGroupState ('Track','Mute',_trackGroup,_state,_exclusive);
}

function mo_setTracksSolo (_trackGroup,_state,_exclusive) {
	this.setGroupState ('Track','Solo',_trackGroup,_state,_exclusive);
}

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('getChannelsMute','getChannelsSolo','getGroupState','getTracksMute','getTracksSolo','setChannelsMute','setChannelsSolo','setGroupState','setTracksMute','setTracksSolo');
