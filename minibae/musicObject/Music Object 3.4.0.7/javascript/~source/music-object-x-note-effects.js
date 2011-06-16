/**********
     TITLE: Music Object Extension - Note Effects
   VERSION: 1.0.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_xc"*/

function mo_playNoteSlide (_channelNo,_bankNo,_programNo,_startNote,_endNote,_startVelocity,_endVelocity,_startNoteDuration,_endNoteDuration,_slideTime,_noteInterval) {
	_startNote = Music.getNoteNumber (_startNote);
	_endNote = Music.getNoteNumber ((_endNote != null && _endNote >= 0) ? _endNote : _startNote);
	if (typeof _endVelocity != 'number' || _endVelocity < 0) _endVelocity = _startVelocity;
	if (typeof _endNoteDuration != 'number' || _endNoteDuration < 0) _endNoteDuration = _startNoteDuration;
	if (typeof _noteInterval != 'number' || _noteInterval < 0) _noteInterval = 100;
	var
		_totalNotes = Math.floor (_slideTime / _noteInterval) + 1,
		_noteTime = 0,
		_incDivisor = Math.max (_totalNotes - 1,1),
		_noteInc = (_endNote - _startNote) / _incDivisor,
		_velocityInc = (_endVelocity - _startVelocity) / _incDivisor,
		_durationInc = (_endNoteDuration - _startNoteDuration) / _incDivisor;
		_noteNo = _startNote,
		_velocity = _startVelocity,
		_duration = _startNoteDuration,
		_playNotePrefix = this.objectName + '.playNote (' + _channelNo + ',' + ((_bankNo >= 0 && _bankNo != null && _programNo >= 0 && _programNo != null) ? (_bankNo + ',' + _programNo + ',') : '')
	;
	_noteInterval = Math.floor (_slideTime / _incDivisor);
	for (var _slideNoteNo = 0; _slideNoteNo < _totalNotes; _slideNoteNo++) {
		setTimeout (_playNotePrefix + Math.floor (_noteNo) + ',' + _velocity + ',' + Math.floor (_duration) + ')',Math.floor (_noteTime));
		_noteTime += _noteInterval;
		_noteNo += _noteInc;
		_velocity += _velocityInc;
		_duration += _durationInc;
	}
}

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('playNoteSlide');
