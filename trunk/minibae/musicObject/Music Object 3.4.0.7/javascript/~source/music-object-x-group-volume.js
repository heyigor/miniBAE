/**********
     TITLE: Music Object Extension - Group Volume
   VERSION: 1.0.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 2000-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_xa"*/

function MO_isNothing (_param) {
	return typeof _param == 'undefined' || _param == null || _param == '';
}

function MO_groupScalarName (_groupName) {
	return '[R]GROUP:' + (MO_isNothing (_groupName) ? 'Page Volume' : _groupName);
}

function MO_group (_members) {
	return (typeof _members == 'undefined' || _members == null) ? Music.instances : (Music.isInstance (_members) ? new Array (_members) : _members);
}

function mo_fadeGroupVolume (_groupName,_fromVolume,_toVolume,_fadeTime,_endCallback) {
	if (MO_isNothing (_groupName)) _groupName = 'Page Volume';
	var _fadeName = 'fadeGroupVolume_' + _groupName;
	if (typeof Music.fades [_fadeName] == 'undefined') Music.fades [_fadeName] = new Music.Fade ();
	Music.fades [_fadeName].update ((typeof _fromVolume != 'number') ? Music.getGroupVolume (_groupName) : _fromVolume,_toVolume,_fadeTime,'Music.setGroupVolume (\'' + _groupName + '\',fadeValue)',_endCallback);
	Music.fades [_fadeName].start ();
	return Music.fades [_fadeName];
}

function mo_getGroupVolume (_groupName) {
	var
		_groupScalarName = MO_groupScalarName (_groupName),
		_groupVolume = 100
	;
	for (var _instanceNo = 0; _instanceNo < Music.instances.length; _instanceNo++) {
		var _volumeScalars = Music.instances [_instanceNo].volumeScalars;
		if (typeof _volumeScalars [_groupScalarName] != 'undefined') {
			_groupVolume = _volumeScalars [_groupScalarName];
			break;
		}
	}
	return _groupVolume;
}

function mo_setGroupVolume (_groupName,_volume) {
	var
		_groupScalarName = MO_groupScalarName (_groupName),
		_pageVolumeScalarName = MO_groupScalarName ()
	;
	for (var _instanceNo = 0; _instanceNo < Music.instances.length; _instanceNo++) {
		var _instance = Music.instances [_instanceNo];
		if (_groupScalarName = _pageVolumeScalarName || typeof _instance.volumeScalars [_groupName] != 'undefined')
			_instance.setVolume (_groupName,_volume)
		;
	}
}

function mo_addVolumeGroupMembers (_groupName,_members) {
	_members = MO_group (_members);
	var
		_groupScalarName = MO_groupScalarName (_groupName),
		_groupVolume = mo_getGroupVolume (_groupName)
	;
	for (var _memberNo = 0; _memberNo < _members.length; _memberNo++)
		_members [_memberNo].setVolume (_groupName,_groupVolume)
	;
}

function mo_removeVolumeGroupMembers (_groupName,_members) {
	_members = MO_group (_members);
	var _groupScalarName = MO_groupScalarName (_groupName);
	for (var _memberNo = 0; _memberNo < _members.length; _memberNo++) {
		_members [_memberNo].setVolume (_groupName,-1)
	;
}

function mo_destroyVolumeGroup (_groupName) {Music.setGroupVolume (_groupName,-1)}

function mo_getPageVolume () {return Music.getGroupVolume ()}
function mo_setPageVolume (_volume) {Music.setGroupVolume ('',_volume)}

function mo_fadePageVolume (_fromVolume,_toVolume,_fadeTime,_endCallback) {
	Music.fadeGroupVolume ('',_fromVolume,_toVolume,_fadeTime,_endCallback);
}

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('addVolumeGroupMembers','destroyVolumeGroup','fadeGroupVolume','fadePageVolume','getGroupVolume','getPageVolume','removeVolumeGroupMembers','setGroupVolume','setPageVolume');
