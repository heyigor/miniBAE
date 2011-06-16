/**********
     TITLE: Launch Methodizer
   VERSION: 1.1.0
      TYPE: Shared Code
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object-c-compatibility.js (3.2.0 or higher)
**********/

methodizerPath = '';

function launchMethodizer (_method,_variation) {
	var
		_instructionsStr = ((typeof _method == 'string') ? ('method=' + _method + ((typeof _variation == 'string') ? ('&variation=' + _variation) : '')) : ''),
		_methodizerWindow = Music.openWindow ('','methodizerWindow',980,500)
	;
	if (typeof _methodizerWindow.wizardFrame != 'undefined') {
		_methodizerWindow.focus ();
		_methodizerWindow.wizardFrame.readInstructions (_instructionsStr);
	} else {
		_methodizerWindow.location.href = methodizerPath + 'index.html?' + _instructionsStr;
	}
} 
