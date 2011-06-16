/**********
     TITLE: Music Object Extension - Clip
   VERSION: 1.2.0.3
    AUTHOR: Chris van Rensburg & Chris Ticknor
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_xe"*/

function MO_Clip_replaceSubstring (_sourceStr,_searchStr,_replaceWithStr) {
	_searchStr = _searchStr.toLowerCase ();
	var
		_result = _sourceStr,
		_charPos = 0
	;
	while (true) {
		_charPos = _result.toLowerCase ().indexOf (_searchStr,_charPos);
		if (_charPos == -1) {
			break;
		} else {
			_result = _result.substring (0,_charPos) + _replaceWithStr + _result.substring (_charPos + _searchStr.length);
			_charPos += _replaceWithStr.length;
		}
	}
	return _result;
}

function MO_Clip_seedIntoTemplate (_templateStr,_seedStr,_seedSeedStr) {
	return (_seedStr.charAt (0) == '*') ? MO_Clip_replaceSubstring (_templateStr,'[*]',_seedStr.substr (1)) : MO_Clip_replaceSubstring (_seedStr,'[*]',_seedSeedStr);
}

function MO_Clip_stringHasAudioFileExtension (_sourceStr) {
	return ('|.xmf|.rmf|.mid|.wav|.aif|.aiff|.au|'.indexOf ('|' + _sourceStr.substr (_sourceStr.lastIndexOf ('.')).toLowerCase () + '|') != -1);
}

function MO_Clip_changeImage (_newImageUrl) {
	if (typeof document.images [this._objectName + 'PreviewButton'] != 'undefined') {
		document.images [this._objectName + 'PreviewButton'].src = _newImageUrl;
	}
}

function MO_Clip_ready () {
	var _this = this;
	_this._loaded = true;
	if (_this._imageUrl != _this._imageWhileLoadingUrl) _this.changeImage (_this._imageUrl);
	if (MO_Clip._playWhenLoadedInstance == _this) _this.play ();
}

function MO_Clip_stubLoaded () {
	var _musicObject = this.musicObject;
	_musicObject.onLoad (this._objectName + '.ready ()');
	_musicObject.play (this._musicUrl);
} 

function MO_Clip_onPlayHandler () {
	if (this._imageWhilePlayingUrl != this._imageUrl) this.changeImage (this._imageWhilePlayingUrl);
}

function MO_Clip_onStopHandler () {
	var _this = this;
	if (_this._imageWhilePlayingUrl != _this._imageUrl) _this.changeImage (_this._imageUrl);
	if (!_this._mouseIsOver && _this.hovering) {
		Music.clearTimeout (_this,'hoverResetTimeout');
		_this.hoverResetTimeout = setTimeout (_this._objectName + '.hovering = false',_this._hoverResetDelay);
	}
}

function MO_Clip_play (_loop) {
	var _this = this;
	if (typeof _loop != 'undefined') _this._loop = _loop;
	if (_this._loaded) {
		MO_Clip._playWhenLoadedInstance = null;
		Music.clearTimeout (_this,'hoverTimeout');
		if (MO_Clip._lastInstance != null)
			if (MO_Clip._lastInstance != _this) {
				MO_Clip._lastInstance.musicObject.stop (MO_Clip._lastInstance._fadeOutTime);
			} else {
				if (_this._restartIfPlaying) _this.musicObject.stop (0);
			}
		MO_Clip._lastInstance = _this;
		_this.musicObject.play (_this._loop);
	} else {
		MO_Clip._playWhenLoadedInstance = _this;
	}
}

function MO_Clip_stop () {
	if (this == MO_Clip._playWhenLoadedInstance) MO_Clip._playWhenLoadedInstance = null;
	if (this._loaded) this.musicObject.stop (this._fadeOutTime);
}

function MO_Clip_hover () {
	this.hovering = true;
	this.play ();
}

function MO_Clip_mouseover () {
	var _this = this;
	_this._mouseIsOver = true;
	if (MO_Clip._lastInstance != null && _this != MO_Clip._lastInstance) {
		MO_Clip._lastInstance._mouseIsOver = false;
		if (MO_Clip._lastInstance.hovering) {
			Music.clearTimeout (_this,'hoverResetTimeout');
			MO_Clip._lastInstance.hovering = false;
		}
	}
	Music.clearTimeout (_this,'hoverTimeout');
	if (_this._triggerType.indexOf ('over') != -1) {
		if (_this._hoverDelay > 0 && !_this.hovering) {
			_this.hoverTimeout = setTimeout (_this._objectName + '.hover ()',_this._hoverDelay);
		} else {
			_this.play ();
		}
	}
}

function MO_Clip_mouseout () {
	var _this = this;
	_this._mouseIsOver = false;
	Music.clearTimeout (_this,'hoverTimeout');
	if (_this._stopOnMouseOut) _this.stop (_this._fadeOutTime);
}

function MO_Clip_click () {
	Music.clearTimeout (this,'hoverTimeout');
	if (this._triggerType.indexOf ('click') != -1) this.play ();
}

function MO_Clip_insertUI (_linkHref,_linkTarget,_linkAlt) {
	var
		_this = this,
		_linkSpecified = typeof _linkHref == 'string'
	;
	document.writeln ('<A HREF="' + (_linkSpecified ? _linkHref : 'javascript://') + '" ONCLICK="' + _this._objectName + '.click ();' + (_linkSpecified ? '' : ' return false') + '"' + ((typeof _linkTarget == 'string') ? (' TARGET="' + _linkTarget + '"') : '') + ' ONMOUSEOVER="' + _this._objectName + '.mouseover (); return true" ONMOUSEOUT="' + _this._objectName + '.mouseout (); return false"><IMG SRC="' + _this._imageWhileLoadingUrl + '" NAME="' + _this._objectName + 'PreviewButton" BORDER=0 WIDTH=' + _this._imageWidth + ' HEIGHT=' + _this._imageHeight + ((typeof _linkAlt == 'string') ? (' ALT="' + _linkAlt + '"') : '') + '></A>');
}

function mo_Clip (_seed,_musicUrl,_imageUrl,_imageWhileLoadingUrl,_imageWhilePlayingUrl,_imageWidth,_imageHeight,_triggerType,_fadeOutTime,_stopOnMouseOut,_restartIfPlaying,_hoverDelay,_hoverResetDelay) {
	var
		_this = this,
		_Clip = MO_Clip
	;

	/*** Private Instance Properties ***/
	_this._objectName = 'mo_Clip_GIN' + _Clip._generatedInstanceNames++;
	if (MO_Clip_stringHasAudioFileExtension (_seed)) {
		if (typeof _musicUrl != 'string') _musicUrl = _seed;
		_seed = _this._objectName;
	}
	if (typeof _musicUrl != 'string') _musicUrl = '*' + _seed;
	_this._musicUrl = MO_Clip_seedIntoTemplate (_Clip.musicUrl,_musicUrl,_seed);
	if (!MO_Clip_stringHasAudioFileExtension (_this._musicUrl)) _this._musicUrl += '.rmf';
	if (typeof _imageUrl != 'string') _imageUrl = '*' + _seed;
	if (typeof _imageWhileLoadingUrl != 'string') _imageWhileLoadingUrl = _imageUrl;
	if (typeof _imageWhilePlayingUrl != 'string') _imageWhilePlayingUrl = _imageUrl;
	_this._imageUrl = _Clip.imagePath + MO_Clip_seedIntoTemplate (_Clip.imageUrl,_imageUrl,_seed);
	_this._imageWhileLoadingUrl = _Clip.imagePath + MO_Clip_seedIntoTemplate ((_Clip.imageWhileLoadingUrl != null) ? _Clip.imageWhileLoadingUrl : _Clip.imageUrl,_imageWhileLoadingUrl,_seed);
	_this._imageWhilePlayingUrl = _Clip.imagePath + MO_Clip_seedIntoTemplate ((_Clip.imageWhilePlayingUrl != null) ? _Clip.imageWhilePlayingUrl : _Clip.imageUrl,_imageWhilePlayingUrl,_seed);
	_this._imageWidth = (typeof _imageWidth == 'number') ? _imageWidth : _Clip.imageWidth;
	_this._imageHeight = (typeof _imageHeight == 'number') ? _imageHeight : _Clip.imageHeight;
	_this._triggerType = MO_Clip_replaceSubstring ((typeof _triggerType == 'string') ? _triggerType : _Clip.triggerType,' ','').toLowerCase ();
	_this._fadeOutTime = (typeof _fadeOutTime == 'number') ? _fadeOutTime : _Clip.fadeOutTime;
	_this._stopOnMouseOut = (typeof _stopOnMouseOut == 'boolean') ? _stopOnMouseOut : _Clip.stopOnMouseOut;
	_this._restartIfPlaying = (typeof _restartIfPlaying == 'boolean') ? _restartIfPlaying : _Clip.restartIfPlaying;
	if (_this._restartIfPlaying == null) _this._restartIfPlaying = _this._triggerType.indexOf ('over') != -1;
	_this._hoverDelay = (typeof _hoverDelay == 'number') ? _hoverDelay : _Clip.hoverDelay;
	_this._hoverResetDelay = (typeof _hoverResetDelay == 'number') ? _hoverResetDelay : _Clip.hoverResetDelay;
	_this._loaded = false;
	_this._mouseIsOver = false;
	_this.hovering = false;
	_this._loop = false;

	/*** Public Instance Methods ***/
	_this.createPreviewButton = _this.insertUI = MO_Clip_insertUI;
	_this.play = MO_Clip_play;
	_this.stop = MO_Clip_stop;

	/*** Private Instance Methods ***/
	_this.stubLoaded = MO_Clip_stubLoaded;
	_this.ready = MO_Clip_ready;
	_this.mouseover = MO_Clip_mouseover;
	_this.mouseout = MO_Clip_mouseout;
	_this.click = MO_Clip_click;
	_this.hover = MO_Clip_hover;
	_this.changeImage = MO_Clip_changeImage;
	_this.onStopHandler = MO_Clip_onStopHandler;
	_this.onPlayHandler = MO_Clip_onPlayHandler;

	/*** Initialization ***/
	var _stubUrl = ((_Clip.stubPath != null) ? _Clip.stubPath : MO_Clip_replaceSubstring (_Clip.musicPath,'[*]','stub'));
	if (_stubUrl.substr (_stubUrl.length - 4).toLowerCase () != '.rmf') _stubUrl += 'stub.rmf';
	var _musicObject = _this.musicObject = new Music ();
	_musicObject.onLoad (_this._objectName + '.stubLoaded ()');
	_musicObject.onPlay (_this._objectName + '.onPlayHandler ()');
	_musicObject.onStop (_this._objectName + '.onStopHandler ()');
	_musicObject.preloadEmbed (_stubUrl,'LOOP=' + _this._loop);
	window [_this._objectName] = window [_seed] = _Clip.instances [_Clip.instances.length] = _this;
}

MO_Clip = mo_Clip;

function mo_newClips () {
	var _args = mo_newClips.arguments;
	for (var _argNo = 0; _argNo < _args.length; _argNo++) new MO_Clip (_args [_argNo]);
}

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('Clip','newClips');

/*** Public Static Properties ***/
MO_Clip.fadeOutTime = 500;
MO_Clip.hoverDelay = 350;
MO_Clip.hoverResetDelay = 1000;
MO_Clip.imagePath = '';
MO_Clip.imageHeight = 13;
MO_Clip.imageUrl = 'preview-ready.gif';
MO_Clip.imageWhileLoadingUrl = 'not-ready.gif';
MO_Clip.imageWhilePlayingUrl = 'preview-playing.gif';
MO_Clip.imageWidth = 18;
MO_Clip.instances = new Array ();
MO_Clip.musicUrl = '[*].rmf';
MO_Clip.restartIfPlaying = null;
MO_Clip.triggerType = 'click';
MO_Clip.stopOnMouseOut = false;
MO_Clip.stubPath = null;

/*** Private Static Properties ***/
MO_Clip._lastInstance = null;
MO_Clip._generatedInstanceNames = 0;
MO_Clip._playWhenLoadedInstance = null;
