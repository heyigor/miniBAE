/**********
     TITLE: Music Object Extension - Fades
   VERSION: 1.1.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/
function mo_fade(_a,_b,_c,_d,_e,_f,_g){if(typeof Music.fades[_a]=='undefined'){Music.fades[_a]=new Music.Fade();}else{if(typeof _g=='boolean'&&_g)Music.fades[_a].end();}Music.fades[_a].update(_b,_c,_d,_e,_f);Music.fades[_a].start();return Music.fades[_a];}function mo_fadeController(_h,_i,_b,_c,_d,_f){var _j=mo_fade(this.objectName+'_fadeController'+_h+_i,(typeof _b=='number')?_b:this.getController(_h,_i),_c,_d,this.objectName+'.setController ('+_h+','+_i+',fadeValue)',_f);return _j;}function mo_fadeTempo(_k,_l,_d,_f){var _j=mo_fade(this.objectName+'_fadeTempo',(typeof _k=='number')?_k:this.getTempo(),_l,_d,this.objectName+'.setTempo (fadeValue)',_f);return _j;}function mo_fadeVolumeScalar(_m,_n,_o,_d,_f,_p){if(typeof _p!='undefined')this.onOutputVolumeChangeHandler=_p;var _j=this.fade(
this.objectName+'_fadeVolumeScalar_'+_m,(typeof _n=='number')?_n:this.getVolume(_m),_o,_d,this.objectName+'.setVolume (\''+_m+'\',fadeValue)',_f);return _j;}Music.fades=new Array();Music.addPublicInstanceMethods('fadeController','fadeTempo','fadeVolumeScalar');Music.addPublicStaticMethods('fade');