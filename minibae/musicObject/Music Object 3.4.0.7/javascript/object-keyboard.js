/**********
     TITLE: Keyboard Object
   VERSION: 1.2.0
      TYPE: Object Definition
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/
function noteArea(_a,_b,_c,_d,_e,_f){document.write('<AREA SHAPE=RECT HREF="javascript:parent.indexHeader.setNote ('+_a+','+_b+')" ONCLICK="parent.indexHeader.setNote ('+_a+','+_b+'); return false" ONMOUSEOVER="parent.indexHeader.keyOver ('+_a+','+_b+'); return true" COORDS='+_c+','+_d+','+_e+','+_f+'>');}function keyboardOctave(_a){document.write('<MAP NAME="octave'+_a+'">');noteArea(_a,1,6,0,10,36);noteArea(_a,3,14,0,18,36);noteArea(_a,6,30,0,34,36);noteArea(_a,8,38,0,42,36);noteArea(_a,10,46,0,50,36);noteArea(_a,0,0,0,8,56);noteArea(_a,2,8,0,16,56);noteArea(_a,4,16,0,24,56);noteArea(_a,5,24,0,32,56);noteArea(_a,7,32,0,40,56);noteArea(_a,9,40,0,48,56);noteArea(_a,11,48,0,55,56);document.write('</MAP>');
document.write('<IMG SRC="../../images/keyboard-octave.gif" WIDTH=56 HEIGHT=57 ALT="Octave '+_a+'" BORDER=0 USEMAP="#octave'+_a+'" HSPACE=0>');}function keyboard(octaves){document.writeln('<TABLE CELLSPACING=0 CELLPADDING=0 BORDER=0>'+'<TR>');for(var _a=0;_a<=octaves-1;_a++){document.write('<TD>');keyboardOctave(_a);document.writeln('</TD>');}document.writeln('</TR>'+'</TABLE>');}