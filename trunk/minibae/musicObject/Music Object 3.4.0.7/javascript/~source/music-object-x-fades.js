/**********
     TITLE: Music Object Extension - Fades
   VERSION: 1.1.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x7"*/

function mo_fade (_fadeName,_startValue,_endValue,_fadeTime,_updateHandler,_endCallback,_endIfInProgress) {
	if (typeof Music.fades [_fadeName] == 'undefined') {
		Music.fades [_fadeName] = new Music.Fade ();
	} else {
		if (typeof _endIfInProgress == 'boolean' && _endIfInProgress) Music.fades [_fadeName].end ();
	}
	Music.fades [_fadeName].update (_startValue,_endValue,_fadeTime,_updateHandler,_endCallback);
	Music.fades [_fadeName].start ();
	return Music.fades [_fadeName];
}

function mo_fadeController (_channelNo,_controllerNo,_startValue,_endValue,_fadeTime,_endCallback) {
	var _result = mo_fade (
		this.objectName + '_fadeController' + _channelNo + _controllerNo,
		(typeof _startValue == 'number') ? _startValue : this.getController (_channelNo,_controllerNo),
		_endValue,
		_fadeTime,
		this.objectName + '.setController (' + _channelNo + ',' + _controllerNo + ',fadeValue)',
		_endCallback
	);
	return _result;
}

function mo_fadeTempo (_startTempo,_endTempo,_fadeTime,_endCallback) {
	var _result = mo_fade (
		this.objectName + '_fadeTempo',
		(typeof _startTempo == 'number') ? _startTempo : this.getTempo (),
		_endTempo,
		_fadeTime,
		this.objectName + '.setTempo (fadeValue)',
		_endCallback
	);
	return _result;
}

function mo_fadeVolumeScalar (_scalarName,_fromLevel,_toLevel,_fadeTime,_endCallback,_volumeChangeHandler) {
	if (typeof _volumeChangeHandler != 'undefined') this.onOutputVolumeChangeHandler = _volumeChangeHandler;
	var _result = this.fade (
		this.objectName + '_fadeVolumeScalar_' + _scalarName,
		(typeof _fromLevel == 'number') ? _fromLevel : this.getVolume (_scalarName),
		_toLevel,
		_fadeTime,
		this.objectName + '.setVolume (\'' + _scalarName + '\',fadeValue)',
		_endCallback
	);
	return _result;
}

/*** Private Static Properties ***/

Music.fades = new Array ();

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('fadeController','fadeTempo','fadeVolumeScalar');

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('fade');
