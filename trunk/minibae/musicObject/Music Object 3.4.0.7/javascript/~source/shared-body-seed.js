/**********
     TITLE: Shared Body Seed
   VERSION: 1.2.0
      TYPE: Shared Code
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

function bodySeed (_defaultURL) {
	var _result = window.location.search;

	if (_result != '') _result = _result.substr (1);
	if (_result == '') {
		if (typeof _defaultURL == 'undefined') _defaultURL = 'index-body.html';
		return _defaultURL;
	} else {
		var _anchorPos = _result.indexOf ('^');
		if (_anchorPos != -1) {
			var _queryPos = _result.indexOf ('?');
			if (_queryPos == -1) _queryPos = _result.length;
			if (_anchorPos < _queryPos) _result = _result.substring (0,_anchorPos) + '#' + _result.substr (_anchorPos + 1);
		}
		return _result;
	}
}
