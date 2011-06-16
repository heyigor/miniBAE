/**********
     TITLE: Keyboard Object
   VERSION: 1.2.0
      TYPE: Object Definition
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

function noteArea (_octaveNo,_noteOffset,_x1,_y1,_x2,_y2) {
	document.write ('<AREA SHAPE=RECT HREF="javascript:parent.indexHeader.setNote (' + _octaveNo + ',' + _noteOffset + ')" ONCLICK="parent.indexHeader.setNote (' + _octaveNo + ',' + _noteOffset + '); return false" ONMOUSEOVER="parent.indexHeader.keyOver (' + _octaveNo + ',' + _noteOffset + '); return true" COORDS=' + _x1 + ',' + _y1 + ',' + _x2 + ',' + _y2 + '>');
}

function keyboardOctave (_octaveNo) {
	document.write ('<MAP NAME="octave' + _octaveNo + '">');
	noteArea (_octaveNo,1,6,0,10,36);
	noteArea (_octaveNo,3,14,0,18,36);
	noteArea (_octaveNo,6,30,0,34,36);
	noteArea (_octaveNo,8,38,0,42,36);
	noteArea (_octaveNo,10,46,0,50,36);
	noteArea (_octaveNo,0,0,0,8,56);
	noteArea (_octaveNo,2,8,0,16,56);
	noteArea (_octaveNo,4,16,0,24,56);
	noteArea (_octaveNo,5,24,0,32,56);
	noteArea (_octaveNo,7,32,0,40,56);
	noteArea (_octaveNo,9,40,0,48,56);
	noteArea (_octaveNo,11,48,0,55,56);
	document.write ('</MAP>');
	document.write ('<IMG SRC="../../images/keyboard-octave.gif" WIDTH=56 HEIGHT=57 ALT="Octave ' + _octaveNo + '" BORDER=0 USEMAP="#octave' + _octaveNo + '" HSPACE=0>');
}

function keyboard (octaves) {
	document.writeln (
		'<TABLE CELLSPACING=0 CELLPADDING=0 BORDER=0>' +
		'<TR>'
	);
	for (var _octaveNo = 0; _octaveNo <= octaves - 1; _octaveNo++) {
		document.write ('<TD>');
		keyboardOctave (_octaveNo);
		document.writeln ('</TD>');
	}
	document.writeln (
		'</TR>' +
		'</TABLE>'
	);
}
