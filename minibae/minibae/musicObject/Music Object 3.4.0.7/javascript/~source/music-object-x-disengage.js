/**********
     TITLE: Music Object Extension - Auto Disengage
   VERSION: 1.0.0.17
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 2000-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_x4"*/

function mo_mediaLinkClicked () {
	Music.MO_mediaLinkClicked = true;
	Music.MO_windowFocused = false;
	if (Music.autoDisengageEnabled) {
		Music.engageAudio (false);
		if (Music.mediaLinkShutoutTime != 0) {
			Music.inLinkShutout = true;
			setTimeout ('Music.inLinkShutout = false',Music.mediaLinkShutoutTime);
		}
	}
}

function mo_initializeAutoDisengage () {
	var _Music = Music;
	if (_Music.mediaLinkInterception && !_Music.isAudioDeviceShared () && (_Music.mediaLinksNoOnClicks || _Music.clientName != 'Netscape' || !_Music.meetsMinVersion (_Music.clientVersion,'5'))) {
		var
			_links = document.links,
			_audioFileExtensions = _Music.MO_audioFileExtensions.toLowerCase ()
		;
		for (var _linkNo = 0; _linkNo < _links.length; _linkNo++) {
			var
				_link = _links [_linkNo],
				_extensionPos = _link.href.lastIndexOf ('.'),
				_extension = _link.href.substr (_extensionPos + 1).toLowerCase ()
			;
			if (_audioFileExtensions.indexOf ('|' + _extension + '|') != -1)
				_Music.addEventHandler (window,_link,'onclick','mo_mediaLinkClicked ()')
			;
		}
	}
}

function mo_autoDisengageWindowOnClick () {
	if (Music.MO_mediaLinkClicked) {
		Music.MO_mediaLinkClicked = false;
	} else {
		Music.MO_windowFocused = true;
		window.focus ();
	}
}

function mo_autoDisengageWindowOnFocus () {
	Music.MO_windowFocused = true;
}

function mo_autoDisengageWindowOnBlur () {
	Music.MO_windowFocused = false;
}

function mo_updateAutoDisengage (_instanceAudible) {
	var _Music = Music;
	_Music.MO_audibleInstances += _instanceAudible ? 1 : -1;
	if (_instanceAudible) {
		_Music.clearTimeout (_Music,'autoDisengageTimeout');
		if (_Music.autoDisengageEnabled && !_Music.inLinkShutout && (_Music.autoEngageMode == 'always' || (_Music.autoEngageMode == 'when focused' && _Music.MO_windowFocused)))
			_Music.engageAudio (true)
		;
	} else if (_Music.autoDisengageEnabled && _Music.MO_audibleInstances <= 0) {
		_Music.autoDisengageTimeout = setTimeout ('Music.engageAudio (false);',_Music.autoDisengageDelay);
	}
}

function mo_addMediaFileExtensions (_newAudioFileExtensions) {
	Music.MO_audioFileExtensions += '*' + _newAudioFileExtensions;
}

/*** Instance Extenders ***/

function MO_autoDisengageInstanceExtender (_this) {
	_this.addCallbackHandler ('onAudibleStateChange','mo_updateAutoDisengage (' + _this.objectName + '.audible)');
}

Music.addInstanceExtender (MO_autoDisengageInstanceExtender);

/*** Private Static Properties ***/

Music.MO_audibleInstances = 0;
Music.autoDisengageTimeout = null;
Music.MO_windowFocused = true;
Music.inLinkShutout = false;
Music.MO_audioFileExtensions =
	'QuickTime |aif|aifc|aiff|au|kar|mid|midi|mov|qcp|qt|qts|rts|rtsp|sd2|smf|snd|ulw|wav|' +
	'Cakewalk |mff|mid|rmi|smf|' +
	'Windows Media Player |au|avi|m1v|m3u|mpa|mpe|mpeg|mpv2|snd|wav|' +
	'RealPlayer |aif|aiff|au|avi|m3u|mp1|mp2|mp3|mpa|mpeg|mpg|mpga|mpv|pls|ra|ram|rf|rm|rmm|rnx|rv|ssm|wav|xpl|' +
	'Liquid Player |la1|lar|lav|lavs|lqp|lqt|lsl|' +
	'Winamp |669|asx|cda|dsm|far|it|m3u|med|mid|mod|mp2|mp3|mpg|mtm|pls|s3m|stm|ult|voc|wav|xm|'
;

/*** Public Static Properties ***/

Music.autoDisengageDelay = 500;
Music.autoDisengageEnabled = true;
Music.autoEngageMode = 'always';
Music.mediaLinkInterception = true;
Music.mediaLinkShutoutTime = 4000;
Music.mediaLinksNoOnClicks = false;

/*** Initialization ***/

Music.addEventHandler (window,window,'onload','mo_initializeAutoDisengage ()');
Music.addEventHandler (window,window,'onfocus','mo_autoDisengageWindowOnFocus ()');
Music.addEventHandler (window,window,'onblur','mo_autoDisengageWindowOnBlur ()');

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('addMediaFileExtensions','mediaLinkClicked');
