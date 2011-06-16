/**********
     TITLE: Music Object Extension - Ducking
   VERSION: 1.3.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x5"*/

function MO_duckingSlaveGroupsMatch (_slaveGroupA,_slaveGroupB) {
	var _groupsMatch =
		_slaveGroupA._attackTime == _slaveGroupB._attackTime &&
		_slaveGroupA._sustainLevel == _slaveGroupB._sustainLevel &&
		_slaveGroupA._releaseTime == _slaveGroupB._releaseTime &&
		_slaveGroupA._slaves.length == _slaveGroupB._slaves.length
	;
	for (var _slaveNoA = 0; _slaveNoA < _slaveGroupA._slaves.length && _groupsMatch; _slaveNoA++) {
		var _slaveMatched = false;
		for (var _slaveNoB = 0; _slaveNoB < _slaveGroupB._slaves.length && !_slaveMatched; _slaveNoB++)
			_slaveMatched = _slaveMatched || _slaveGroupA._slaves [_slaveNoA] == _slaveGroupB._slaves [_slaveNoB]
		;
		_groupsMatch = _groupsMatch && _slaveMatched;
	}
	return _groupsMatch;
}

function MO_applyDucking () {
	var
		_tasksDeleted = 0,
		_duckingTaskNo = 0,
		_tasksInProgress = false
	;
	while (_duckingTaskNo < Music.MO_duckingTasks.length - _tasksDeleted) {
		Music.MO_duckingTasks [_duckingTaskNo] = Music.MO_duckingTasks [_duckingTaskNo + _tasksDeleted];
		var _duckingTask = Music.MO_duckingTasks [_duckingTaskNo];
		if (_duckingTask._totalMasters == 0 && !_duckingTask._inProgress) {
			_tasksDeleted++;
		} else {
			if (_duckingTask._inProgress) {
				var _taskSlaves = _duckingTask._slaveGroup._slaves;
				for (var _slaveNo = 0; _slaveNo < _taskSlaves.length; _slaveNo++) {
					var _slave = _taskSlaves [_slaveNo];
					if (!_slave.MO_duckingTouched) {
						_slave.MO_duckingTouched = true;
						_slave.volumeScalars ['[R]ducking'] = _duckingTask._fade.fadeValue;
					} else {
						_slave.volumeScalars ['[R]ducking'] = Math.min (_slave.volumeScalars ['[R]ducking'],_duckingTask._fade.fadeValue);
					}
				}
			}
			_duckingTaskNo++;
		}
		_tasksInProgress = _tasksInProgress || (_duckingTask._inProgress = _duckingTask._fade.inProgress || _duckingTask._requests > 0);
	}
	Music.MO_duckingTasks.length -= _tasksDeleted;
	for (var _instanceNo = 0; _instanceNo < Music.instances.length; _instanceNo++) {
		var _instance = Music.instances [_instanceNo];
		if (_instance.MO_duckingTouched) {
			_instance.setVolume ();
			_instance.MO_duckingTouched = false;
		}
	}
	if (!_tasksInProgress) Music.onFadesUpdatedHandler = null;
}

function mo_updateDuckingTasks () {
	var _this = this;
	for (var _slaveGroupNo = 0; _slaveGroupNo < _this.MO_duckingSlaveGroups.length; _slaveGroupNo++) {
		var _slaveGroup = _this.MO_duckingSlaveGroups [_slaveGroupNo];
		if (_slaveGroup._taskRef != null && !MO_duckingSlaveGroupsMatch (_slaveGroup,_slaveGroup._taskRef._slaveGroup)) {
			if (_this.MO_duckingOn) _slaveGroup._taskRef.masterOff ();
			_slaveGroup._taskRef._totalMasters--;
			_slaveGroup._taskRef = null;
		}
		if (_slaveGroup._taskRef == null) {
			for (var _duckingTaskNo = 0; _duckingTaskNo < Music.MO_duckingTasks.length; _duckingTaskNo++) {
				if (MO_duckingSlaveGroupsMatch (_slaveGroup,Music.MO_duckingTasks [_duckingTaskNo]._slaveGroup)) {
					_slaveGroup._taskRef = Music.MO_duckingTasks [_duckingTaskNo];
					break;
				}
			}
			if (_slaveGroup._taskRef == null) {
				_slaveGroup._taskRef = Music.MO_duckingTasks [Music.MO_duckingTasks.length] = new mo_DuckingTask (_slaveGroup._attackTime,_slaveGroup._sustainLevel,_slaveGroup._releaseTime);
				var _taskSlaves = Music.MO_duckingTasks [Music.MO_duckingTasks.length - 1]._slaveGroup._slaves;
				for (var _slaveNo = 0; _slaveNo < _slaveGroup._slaves.length; _slaveNo++)
					_taskSlaves [_slaveNo] = _slaveGroup._slaves [_slaveNo]
				;
			}
			_slaveGroup._taskRef._totalMasters++;
			if (_this.MO_duckingOn) _slaveGroup._taskRef.masterOn ();
		}
	}
}

function mo_addDuckingSlaves (_slave,_attackTime,_sustainLevel,_releaseTime) {
	var
		_this = this,
		_groupToAddTo = null
	;
	if (typeof _attackTime != 'number') _attackTime = Music.duckingAttackTime;
	if (typeof _sustainLevel != 'number') _sustainLevel = Music.duckingSustainLevel;
	if (typeof _releaseTime != 'number') _releaseTime = Music.duckingReleaseTime;
	_this.removeDuckingSlaves (_slave);
	for (var _slaveGroupNo = 0; _slaveGroupNo < _this.MO_duckingSlaveGroups.length; _slaveGroupNo++) {
		var _slaveGroup = _this.MO_duckingSlaveGroups [_slaveGroupNo];
		if (
			_slaveGroup._attackTime == _attackTime &&
			_slaveGroup._sustainLevel == _sustainLevel &&
			_slaveGroup._releaseTime == _releaseTime
		)
			_groupToAddTo = _slaveGroup
		;
	}
	if (_groupToAddTo == null)
		_groupToAddTo = _this.MO_duckingSlaveGroups [_this.MO_duckingSlaveGroups.length] = new MO_DuckingSlaveGroup (_attackTime,_sustainLevel,_releaseTime)
	;
	var _slaves = (typeof _slave == 'undefined' || _slave == null) ? Music.instances : (Music.isInstance (_slave) ? new Array (_slave) : _slave);
	for (var _slaveNo = 0; _slaveNo < _slaves.length; _slaveNo++) {
		if (_slaves [_slaveNo] != this) _groupToAddTo._slaves [_groupToAddTo._slaves.length] = _slaves [_slaveNo];
	}
	_this.updateDuckingTasks ();
}

function mo_removeDuckingSlaves (_slave) {
	var
		_this = this,
		_duckingSlaveGroups = _this.MO_duckingSlaveGroups,
		_slaves = (typeof _slave == 'undefined' || _slave == null) ? Music.instances : (Music.isInstance (_slave) ? new Array (_slave) : _slave)
	;
	for (var _slaveNo = 0; _slaveNo < _slaves.length; _slaveNo++) {
		var _slaveFound = false;
		for (var _slaveGroupNo = 0; _slaveGroupNo < _duckingSlaveGroups.length && !_slaveFound; _slaveGroupNo++) {
			var _groupSlaves = _duckingSlaveGroups [_slaveGroupNo]._slaves;
			for (var _groupSlaveNo = 0; _groupSlaveNo < _groupSlaves.length; _groupSlaveNo++) {
				if (!_slaveFound && _groupSlaves [_groupSlaveNo] == _slaves [_slaveNo]) _slaveFound = true;
				if (_slaveFound && _groupSlaveNo < _groupSlaves.length - 1) _groupSlaves [_groupSlaveNo] = _groupSlaves [_groupSlaveNo + 1];
			}
			if (_slaveFound) _groupSlaves.length--;
		}
		if (_slaveFound && _groupSlaves.length == 0) {
			for (var _slaveGroupNo; _slaveGroupNo < _duckingSlaveGroups.length - 1; _slaveGroupNo++)
				_duckingSlaveGroups [_slaveGroupNo] = _duckingSlaveGroups [_slaveGroupNo + 1]
			;
			_duckingSlaveGroups [_duckingSlaveGroups.length - 1] = null;
			_duckingSlaveGroups.length--;
		}
	}
	_this.updateDuckingTasks ();
}

function mo_addDuckingMasters (_master,_attackTime,_sustainLevel,_releaseTime) {
	Music.addDuckingRelationships (_master,this,_attackTime,_sustainLevel,_releaseTime);
}

function mo_removeDuckingMasters (_master) {
	Music.removeDuckingRelationships (_master,this);
}

function mo_addDuckingRelationships (_master,_slave,_attackTime,_sustainLevel,_releaseTime) {
	var _masters = (typeof _master == 'undefined' || _master == null) ? Music.instances : (Music.isInstance (_master) ? new Array (_master) : _master);
	for (var _masterNo = 0; _masterNo < _masters.length; _masterNo++)
		_masters [_masterNo].addDuckingSlaves (_slave,_attackTime,_sustainLevel,_releaseTime)
	;
}

function mo_removeDuckingRelationships (_master,_slave) {
	var _masters = (typeof _master == 'undefined' || _master == null) ? Music.instances : (Music.isInstance (_master) ? new Array (_master) : _master);
	for (var _masterNo = 0; _masterNo < _masters.length; _masterNo++)
		_masters [_masterNo].removeDuckingSlaves (_slave)
	;
}

function mo_updateDuckingOn () {
	var _updaterMethod = 'master' + (this.audible ? 'On' : 'Off');
	for (var _slaveGroupNo = 0; _slaveGroupNo < this.MO_duckingSlaveGroups.length; _slaveGroupNo++)
		this.MO_duckingSlaveGroups [_slaveGroupNo]._taskRef [_updaterMethod] ()
	;
}

function mo_getUsage () {return this.usage}
function mo_setUsage (_usage) {this.usage = _usage}

function MO_DuckingSlaveGroup (_attackTime,_sustainLevel,_releaseTime) {
	this._attackTime = _attackTime;
	this._sustainLevel = _sustainLevel;
	this._releaseTime = _releaseTime;
	this._slaves = new Array ();
	this._taskRef = null;
}

function MO_DuckingTask_masterOn () {
	this._requests++;
	if (this._requests > 0) {
		Music.onFadesUpdatedHandler = MO_applyDucking;
		this._inProgress = true;
		this._fade.update (this._fade.fadeValue,this._slaveGroup._sustainLevel,this._slaveGroup._attackTime);
		this._fade.start ();
	}
}

function MO_DuckingTask_masterOff () {
	this._requests--;
	if (this._requests < 1) {
		this._fade.update (this._fade.fadeValue,100,this._slaveGroup._releaseTime);
		this._fade.start ();
	}
}

function mo_DuckingTask (_attackTime,_sustainLevel,_releaseTime) {
	this._slaveGroup = new MO_DuckingSlaveGroup (_attackTime,_sustainLevel,_releaseTime);
	this._fade = new Music.Fade ();
	this._fade.fadeValue = 100;
	this._requests = 0;
	this._totalMasters = 0;
	this._inProgress = false;

	/*** Private Instance Methods ***/
	this.masterOn = MO_DuckingTask_masterOn;
	this.masterOff = MO_DuckingTask_masterOff;
}

/*** Instance Extenders ***/

function MO_duckingInstanceExtender (_this) {
	_this.addCallbackHandler ('onAudibleStateChange',_this.objectName + '.updateDuckingOn ()');
	_this.MO_duckingSlaveGroups = new Array ();
	_this.volumeScalars ['[R]ducking'] = 100;
	_this.MO_duckingTouched = false;
	_this.MO_duckingOn = false;
}

Music.addInstanceExtender (MO_duckingInstanceExtender);

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('addDuckingMasters','addDuckingSlaves','getUsage','removeDuckingMasters','removeDuckingSlaves','setUsage');

/*** Private Instance Methods ***/

Music.addPrivateInstanceMethods ('updateDuckingTasks','updateDuckingOn');

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('addDuckingRelationships','removeDuckingRelationships');

/*** Private Static Properties ***/

Music.MO_duckingTasks = new Array ();

/*** Public Static Properties ***/

Music.duckingAttackTime = 200;
Music.duckingSustainLevel = 0;
Music.duckingReleaseTime = 1500;
