/**********
     TITLE: Music Object Extension - Chords
   VERSION: 1.1.0
    AUTHOR: Sal Orlando & Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x2"*/

function MO_lowerCharAt (_string,_charPos) {
	return _string.charAt (_charPos).toLowerCase ();
}

function MO_getAccidental (_string,_charPos) {
	return (_string.charAt (_charPos) == '#') ? 1 : ((_string.charAt (_charPos) == 'b') ? -1 : 0);
}

function MO_absAccidental (_string,_charPos) {
	return Math.abs (MO_getAccidental (_string,_charPos));
}

function mo_getChordNotes (_chordName,_octaveNo,_chordRoot) {
	var
		_minMaj = false,
		_diminished = false,
		_tones = new Array (Music.getNoteNumber (_chordName.charAt (0)),0,null,4,null,7,null,null,12,null,null,null,null,null),
		_charPos = 1,
		_offset = 0,
		_octaveOffset = (typeof _octaveNo == 'number') ? (_octaveNo * 12) : 0
	;

	_tones [0] += MO_getAccidental (_chordName,_charPos);
	_charPos += MO_absAccidental (_chordName,_charPos);

	if (MO_lowerCharAt (_chordName,_charPos) == 'm') {
		_minMaj = true;
		_charPos++;
		_tones [3] = 3;
		var _currentChar = MO_lowerCharAt (_chordName,_charPos);
		if (_currentChar == 'a' || _currentChar == 'i') _charPos += 2;
		if (_currentChar == 'a') _tones [3]++;
	}

	if (MO_lowerCharAt (_chordName,_charPos) == 'd') {
		_charPos++;
		_diminished = true;
		_tones [3] = 3;
		_tones [5] = 6;
		_tones [8] = null;
		_charPos += 2;
	}

	_offset += MO_getAccidental (_chordName,_charPos);
	_charPos += MO_absAccidental (_chordName,_charPos);
	if (_chordName.charAt (_charPos) == '7') {
		_tones [7] = (_minMaj ? (_tones [3] + 7) : 10) + _offset;
		if (_diminished) _tones [7]--;
		_tones [8] = null;
		_charPos++;
		_offset = 0;
	}

	if (MO_lowerCharAt (_chordName,_charPos) == 's') {
		_tones [3] = null;
		_tones [4] = 5;
		_charPos += 3;
		if (_chordName.charAt (_charPos) == '2') {
			_tones [4] = null;
			_tones [2] = 2;
			_charPos++;
		} else if (_chordName.charAt (_charPos) == '4') _charPos++;
	}

	_offset += MO_getAccidental (_chordName,_charPos);
	_charPos += MO_absAccidental (_chordName,_charPos);
	if (_chordName.charAt (_charPos) == '9') {
		_tones [7] = _minMaj ? (_tones [3] + 7) : 10;
		_tones [9] = 14 + _offset;
		_tones [8] = null;
		_charPos++;
		_offset = 0;
	}

	_offset += MO_getAccidental (_chordName,_charPos);
	_charPos += MO_absAccidental (_chordName,_charPos);
	if (_chordName.charAt (_charPos) == '1') {
		_charPos++;
		_tones [8] = null;
		if (_tones [7] != null) _tones [7] = _minMaj ? (7 + _tones [3]) : 10;
		_tones [9] = 14;
		_tones [5] = null;
		if (MO_lowerCharAt (_chordName,_charPos) == '1') {
			_tones [11] = 17 + _offset;
			_charPos++;
		} else if (MO_lowerCharAt (_chordName,_charPos) == '3') {
			_tones [13] = 21 + _offset;
			_charPos++;
		}
		_offset = 0;
	}

	_offset += MO_getAccidental (_chordName,_charPos);
	_charPos += MO_absAccidental (_chordName,_charPos);
	if (_chordName.charAt (_charPos) == '5') {
		_tones [5] = 7 + _offset;
		_charPos++;
		_offset = 0;
	}

	_offset += MO_getAccidental (_chordName,_charPos);
	_charPos += MO_absAccidental (_chordName,_charPos);
	if (_chordName.charAt (_charPos) == '6') {
		_tones [6] = 5 + _tones [3] + _offset;
		_charPos++;
		_offset = 0;
	}

	if (_chordName.charAt (_charPos) == '+') _tones [5] = 8;

	_tones [1] = (((typeof _chordRoot == 'string' && _chordRoot != '') || typeof _chordRoot == 'number') ? Music.getNoteNumber (_chordRoot) : _tones [0]) + _octaveOffset;

	for (var _offsetNo = 2; _offsetNo < _tones.length; _offsetNo++) {
		if (typeof _tones [_offsetNo] == 'number') _tones [_offsetNo] += _tones [0] + _octaveOffset;
	}

	_tones [0] = null;

	return _tones;
}

function mo_playNotes (_channelNo,_bankNo,_programNo,_notes,_velocity,_duration) {
	var
		_playStr = '',
		_playNotePrefix = 'this.playNote (' + _channelNo + ',' + ((typeof _bankNo == 'number' && typeof _programNo == 'number') ? (_bankNo + ',' + _programNo + ',') : ''),
		_playNoteSuffix = ',' + ((typeof _velocity == 'number') ? _velocity : 127) + ',' + ((typeof _duration == 'number') ? _duration : 1000) + ');'
	;
	if (typeof _notes == 'string') _notes = _notes.split (',');
	for (var _noteNo = 0; _noteNo < _notes.length; _noteNo++) {
		if (typeof _notes [_noteNo] == 'number' || typeof _notes [_noteNo] == 'string')
			_playStr += _playNotePrefix + Music.getNoteNumber (_notes [_noteNo]) + _playNoteSuffix;
		;
	}
	eval (_playStr);
}

function mo_playChord (_channelNo,_bankNo,_programNo,_chordName,_octaveNo,_chordRoot,_velocity,_duration) {
	this.playNotes (_channelNo,_bankNo,_programNo,mo_getChordNotes (_chordName,_octaveNo,_chordRoot),_velocity,_duration);
}

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('playChord','playNotes');

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('getChordNotes');
