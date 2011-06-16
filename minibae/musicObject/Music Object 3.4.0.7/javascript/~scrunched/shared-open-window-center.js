/**********
     TITLE: Open Centered Window
   VERSION: 1.0.0
      TYPE: Shared Code
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/
function openWindowCenter(_a,_b,_c){var _d='';if(typeof screen!='undefined'){_e=Math.floor((screen.width-_b-10)/2);_f=Math.floor((screen.height-_c-40)/2);_d='screenx='+_e+',screeny='+_f+',left='+_e+',top='+_f+',';}centeredWindow=window.open(_a,'centeredWindow','toolbar=no,location=no,'+'directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,'+_d+'width='+_b+',height='+_c);centeredWindow.focus();return centeredWindow;}