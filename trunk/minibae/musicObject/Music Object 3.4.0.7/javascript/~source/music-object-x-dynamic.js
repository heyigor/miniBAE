/**********
     TITLE: Music Object Extension - Dynamic
   VERSION: 1.2.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x6"*/

function mo_kwiklistToArray (_kwiklist) {
	var
		_items = new Array (),
		_optionStart = 0,
		_lastCharPos = -1
	;
	_items [0] = '';
	while (true) {
		var
			_separatorPos = Music.indexOf (_kwiklist,'|',_lastCharPos + 1),
			_openerPos = Music.indexOf (_kwiklist,'[',_lastCharPos + 1),
			_charPos = Math.min (_separatorPos,_openerPos),
			_suffix = _kwiklist.substring (_lastCharPos + 1,_charPos)
		;
		for (var _itemNo = _optionStart; _itemNo < _items.length; _itemNo++) _items [_itemNo] += _suffix;
		if (_charPos == _kwiklist.length) {
			break;
		} else if (_separatorPos == _charPos) {
			_items [_items.length] = '';
			_optionStart = _items.length - 1;
		} else if (_openerPos == _charPos) {
			_lastCharPos = _charPos;
			var _level = 1;
			while (_level > 0) {
				_openerPos = Music.indexOf (_kwiklist,'[',_charPos + 1);
				var _closerPos = Music.indexOf (_kwiklist,']',_charPos + 1);
				_charPos = Math.min (_openerPos,_closerPos);
				_level += (_openerPos == _charPos) ? 1 : -1;
			}
			var _suffixes = mo_kwiklistToArray (_kwiklist.substring (_lastCharPos + 1,_charPos));
			if (_suffixes.length == 1) {
				_suffixes [1] = _suffixes [0];
				_suffixes [0] = '';
			}
			for (var _itemNo = _items.length - 1; _itemNo >= _optionStart; _itemNo--) {
				for (var _suffixNo = _suffixes.length - 1; _suffixNo >= 0; _suffixNo--)
					_items [_optionStart + (_itemNo - _optionStart) * _suffixes.length + _suffixNo] = _items [_itemNo] + _suffixes [_suffixNo]
				;
			}
		}
		_lastCharPos = _charPos;
	}
	return _items;
}

function mo_random (_rangeLimitA,_rangeLimitB) {
	if (typeof _rangeLimitB == 'undefined') {
		return Math.round (Math.random () * (_rangeLimitA - 1)) + 1;
	} else if (_rangeLimitB == null) {
		return _rangeLimitA;
	} else {
		return _rangeLimitA + Math.round (Math.random () * (_rangeLimitB - _rangeLimitA));
	}
}

function MO_Shuffler_reset () {
	for (var _flagNo = 0; _flagNo < this._items.length; _flagNo++) this._flags [_flagNo] = false;
	this._flagsSelected = 0;
}

function MO_Shuffler_next () {
	var
		_this = this,
		_flagsFound = 0,
		_randomFlag = mo_random (_this._items.length - _this._flagsSelected)
	;
	for (var _flagNo = 0; _flagNo < _this._items.length; _flagNo++) {
		if (!_this._flags [_flagNo]) {
			_flagsFound++;
			if (_flagsFound == _randomFlag) {
				_this._flags [_flagNo] = true;
				break;
			}
		}
	}
	_this._flagsSelected++;
	if (_this._flagsSelected == _this._items.length) _this.reset ();
	return _this._items [_flagNo];
}

function mo_Shuffler () {
	/*** Instance State Variables ***/
	var _args = mo_Shuffler.arguments;
	if (_args.length == 1 && typeof _args [0] == 'object') {
		this._items = _args [0];
	} else {
		this._items = new Array ();
		if (_args.length == 1 && typeof _args [0] == 'number') {
			for (var _itemNo = 0; _itemNo < _args [0]; _itemNo++) this._items [_itemNo] = _itemNo + 1;
		} else {
			for (var _itemNo = 0; _itemNo < _args.length; _itemNo++) this._items [_itemNo] = _args [_itemNo];
		}
	}
	this._flags = new Array ();

	/*** Exposed Methods ***/
	this.next = MO_Shuffler_next;
	this.reset = MO_Shuffler_reset;

	/*** Initialization ***/
	this.reset ();
}

function mo_randomFromList () {
	var _args = mo_randomFromList.arguments;
	if (_args.length == 1 && typeof _args [0] == 'object') _args = _args [0];
	return (_args.length == 0) ? '' : _args [mo_random (_args.length) - 1];
}

function mo_playRandomNote (_channelNo,_bankNo,_programNo,_noteMin,_noteMax,_velocityMin,_velocityMax,_durationMin,_durationMax,_volumeMin,_volumeMax,_panA,_panB) {
	if (typeof _volumeMin == 'integer') this.setController (_channelNo,7,mo_random (_volumeMin,_volumeMax));
	if (typeof _panB == 'integer') this.setController (_channelNo,10,mo_random (_panA,_panB));
	this.playNote (
		_channelNo,
		_bankNo,
		_programNo,
		mo_random (Music.getNoteNumber (_noteMin),Music.getNoteNumber (_noteMax)),
		mo_random (_velocityMin,_velocityMax),
		mo_random (_durationMin,_durationMax)
	);
}

function mo_extractGroup (_sourceArray,_groupPrefix) {
	var
		_includeAll = typeof _groupPrefix == 'undefined' || _groupPrefix == null || _groupPrefix == '' || _groupPrefix.indexOf ('[all') == 0,
		_result = new Array ()
	;
	for (var _itemNo = 0; _itemNo < _sourceArray.length; _itemNo++) {
		if (_includeAll || _sourceArray [_itemNo].indexOf (_groupPrefix) == 0)
			_result [_result.length] = _sourceArray [_itemNo]
		;
	}
	return _result;
}

function mo_randomFromGroup (_sourceArray,_groupPrefix) {
	var _subGroup = mo_extractGroup (_sourceArray,_groupPrefix);
	return (_subGroup.length == 0) ? '' : _subGroup [mo_random (_subGroup.length) - 1];
}

function mo_groovoidGroup (_groupPrefix) {return mo_extractGroup (Music.groovoids,_groupPrefix)}
function mo_randomGroovoid (_groupPrefix) {return mo_randomFromGroup (Music.groovoids,_groupPrefix)}

/*** Public Static Properties ***/

Music.groovoids = mo_kwiklistToArray ('UI-[BeepClick1|Chimes|Choice1|Click[1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18]|CarriageReturn|FunnyBeep|Load1|MouseOver[1|2|Drone1]|Notification[1|2|3]|OpenOrClose[1|2|3]|Question|SimpleClick[1|2]|SystemBeep|TeletypeBurst2|TypeOneKey]|Hits-[BlastOff|Kicker[1|2|5]|LudwigVan|Magic|MysteryChime|SportsOrganCharge|ZoomAndHit]|Fanfare-[Arrival|Cascade|CorpSting1|Finished|Horserace|Mars|Majesty|Presenting|Sports]|Background-[Beeps|Clock|Dubby|Funky|Groove2|InfoPulse|Latin|News|Piano|SimpleTyping|Smooth|StillWaiting|Teletype]|Misc-[CashRegister|ThruPhoneRing]');

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('playRandomNote');

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('extractGroup','groovoidGroup','kwiklistToArray','random','randomFromGroup','randomFromList','randomGroovoid','Shuffler');
