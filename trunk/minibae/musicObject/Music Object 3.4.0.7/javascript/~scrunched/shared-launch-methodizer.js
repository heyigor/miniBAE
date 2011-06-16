/**********
     TITLE: Launch Methodizer
   VERSION: 1.1.0
      TYPE: Shared Code
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object-c-compatibility.js (3.2.0 or higher)
**********/
methodizerPath='';function launchMethodizer(_a,_b){var _c=((typeof _a=='string')?('method='+_a+((typeof _b=='string')?('&variation='+_b):'')):''),_d=Music.openWindow('','methodizerWindow',980,500);if(typeof _d.wizardFrame!='undefined'){_d.focus();_d.wizardFrame.readInstructions(_c);}else{_d.location.href=methodizerPath+'index.html?'+_c;}}