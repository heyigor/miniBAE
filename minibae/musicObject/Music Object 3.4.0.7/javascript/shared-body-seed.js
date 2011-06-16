/**********
     TITLE: Shared Body Seed
   VERSION: 1.2.0
      TYPE: Shared Code
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/
function bodySeed(_a){var _b=window.location.search;if(_b!='')_b=_b.substr(1);if(_b==''){if(typeof _a=='undefined')_a='index-body.html';return _a;}else{var _c=_b.indexOf('^');if(_c!= -1){var _d=_b.indexOf('?');if(_d== -1)_d=_b.length;if(_c<_d)_b=_b.substring(0,_c)+'#'+_b.substr(_c+1);}return _b;}}