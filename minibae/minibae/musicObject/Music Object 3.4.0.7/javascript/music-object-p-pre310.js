/**********
     TITLE: Music Object Patch - Pre 3.1.0
   VERSION: 1.1.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.1.0 or higher)
**********/
function mo_getVersion(){return Music.playerVersion}function mo_isReady(){return this.ready}mo_fadeFromTo=mo_fadeVolume;mo_playGroovoid=mo_play;if(typeof Music!='undefined'){musicObject=Music;mo_installBeatnik=mo_installPlayer;mo_addPublicInstanceMethods('fadeFromTo','getNoteName','getNoteNumber','getVersion','isReady','playGroovoid','stopAll');}