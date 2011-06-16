/**********
     TITLE: Open Centered Window
   VERSION: 1.0.0
      TYPE: Shared Code
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

function openWindowCenter (_URL,_width,_height) {
	var	_positionStr = '';
	if (typeof screen != 'undefined') {
		_xpos = Math.floor ((screen.width - _width - 10) / 2);
		_ypos = Math.floor ((screen.height - _height - 40) / 2);
		_positionStr = 'screenx=' + _xpos + ',screeny=' + _ypos + ',left=' + _xpos + ',top=' + _ypos + ',';
	}
	centeredWindow = window.open (_URL,'centeredWindow','toolbar=no,location=no,' + 'directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,' + _positionStr + 'width=' + _width + ',height=' + _height);
	centeredWindow.focus ();
	return centeredWindow;
}
