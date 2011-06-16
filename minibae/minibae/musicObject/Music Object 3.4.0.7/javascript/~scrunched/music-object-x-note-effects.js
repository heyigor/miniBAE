/**********
     TITLE: Music Object Extension - Note Effects
   VERSION: 1.0.0
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/
function mo_playNoteSlide(_a,_b,_c,_d,_e,_f,_g,_h,_i,_j,_k){_d=Music.getNoteNumber(_d);_e=Music.getNoteNumber((_e!=null&&_e>=0)?_e:_d);if(typeof _g!='number'||_g<0)_g=_f;if(typeof _i!='number'||_i<0)_i=_h;if(typeof _k!='number'||_k<0)_k=100;var _l=Math.floor(_j/_k)+1,_m=0,_n=Math.max(_l-1,1),_o=(_e-_d)/_n,_p=(_g-_f)/_n,_q=(_i-_h)/_n;_r=_d,_s=_f,_t=_h,_u=this.objectName+'.playNote ('+_a+','+((_b>=0&&_b!=null&&_c>=0&&_c!=null)?(_b+','+_c+','):'');_k=Math.floor(_j/_n);for(var _v=0;_v<_l;_v++){setTimeout(_u+Math.floor(_r)+','+_s+','+Math.floor(_t)+')',Math.floor(_m));_m+=_k;_r+=_o;_s+=_p;_t+=_q;}}Music.addPublicInstanceMethods('playNoteSlide');