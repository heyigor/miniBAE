/**********
     TITLE: Scripting Interface Mimic - Host Finder Code
   VERSION: 1.0.6
    AUTHOR: Chris van Rensburg, Sean Echevarria
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

/***

The code in this module should be repeatedly evaluated by all browser windows until the code returns a result indicating that a match has been found. The actual code that is passed should be dynamically generated by substituting appropriate values for the 'SUBSTITUTE[]' blocks in the structure.

SUBSTITUTE[WINDOWREF] = value of the EMBED tag's WINDOWREF attribute
SUBSTITUTE[ID] = value of the EMBED tag's ID attribute

return codes have the following meaning...

0: no match found
	- next steps:
		1] check all other windows
		2] if no match found in any windows, wait until next call from browser's event loop to repeat matching
1: match found, but common support code has not been pushed in
	- next steps:
		1] push in support code
		2] initialize instance
2: match found, and common support code has already been pushed in
	- next step: initialize instance

***/

try{SUBSTITUTE[WINDOWREF]SUBSTITUTE[ID];(typeof SUBSTITUTE[WINDOWREF]mo_EMBED_initializeInstance!='undefined')?2:1}catch(_a){0}

/**********
     TITLE: Scripting Interface Mimic - Support Code
   VERSION: 1.0.6
    AUTHOR: Chris van Rensburg, Sean Echevarria
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

/***

The code in this module should be evaluated by the window that contains the matching EMBED instance, in order to push in the support code for the Scripting Interface Mimic. The actual code that is passed should be dynamically generated by substituting appropriate values for the 'SUBSTITUTE[]' blocks in the structure.

SUBSTITUTE[WINDOWREF] = value of the EMBED tag's WINDOWREF attribute

return codes have the following meaning...

1: indicates that the support code was correctly and completely parsed by the JavaScript interpreter
'' (null string): indicates that there was a failure in setting up the document with the support code

***/

	"MO_E_a=function(){"
	"window.MO_E_b=function(){"
	"var _b=this._b;"
	"this._b='';"
	"return _b;"
	"};"
	"window.MO_E_c=function(_c){"
	"var"
	" _d=this,"
	"_e=arguments,"
	"_f"
	";"
	"_d._b+=_c;"
	"for(var _g=1;_g<_e.length-1;_g+=2){"
	"if(typeof _e[_g+1]!='undefined'){"
	"_f=_e[_g+1];"
	"_d._b+=((_g==1)?'?':'&')+_e[_g]+'='+((typeof _f=='string')?escape(_f).replace(new RegExp('\\\\+','g'),'%2B').replace(new RegExp('\\\\.','g'),'%2E').replace(new RegExp('/','g'),'%2F'):_f);"
	"}"
	"}"
	"_d._b+=';';"
	"};"
	"window.MO_E_d=function(_h,_i){"
	"var _j=this.channels[_h].controllers;"
	"return(typeof _j[_i]!='undefined')?_j[_i]:0;"
	"};"
	"window.MO_E_e=function(_h){return this.channels[_h].mute};"
	"window.MO_E_f=function(_h){return this.channels[_h].solo};"
	"window.MO_E_g=function(_h){return this.channels[_h].program};"
	"window.MO_E_h=function(_k){return this.tracks[_k].mute};"
	"window.MO_E_i=function(_k){return this.tracks[_k].solo};"
	"window.MO_E_j=function(_h){return this.channels[_h].transposable};"
	"window.MO_E_k=function(_l,_m,_n,_o,_p){"
	"if(typeof _o=='undefined')"
	"this._q('noteOn','channel',_l,'note',_m,'velocity',_n);"
	"else"
	" this._q('noteOn','channel',_l,'bank',_m,'program',_n,'note',_o,'velocity',_p);"
	"};"
	"window.MO_E_l=function(_l,_m){"
	"this._q('play',(typeof _l=='boolean')?'loop':'count',_l,'url',_m);"
	"};"
	"window.MO_E_m=function(_r){"
	"var _s=_r.toLowerCase();"
	"return(typeof this.songInfo[_s]!='undefined')?this.songInfo[_s]:'';"
	"};"
	"window.MO_E_n=function(){"
	"return this._t+((this.playing&&this._u!=0)?((new Date()).getTime()-this._u):0);"
	"};"
	"window.MO_E_o=function(_l,_m,_n){"
	"if(typeof _n!='undefined')"
	"this._q('setProgram','channel',_l,'bank',_m,'program',_n);"
	"else"
	" this._q('setProgram','channel',_l,'program',_m);"
	"};"
	"window.MO_E_p=function(_t){"
	"this._t=_t;"
	"this._u=(new Date()).getTime();"
	"};"
	"window.MO_E_q=function(_v){"
	"var"
	" _e=MO_E_q.arguments,"
	"_w=_x=''"
	";"
	"for(_y=1;_y<_e.length;_y++){"
	"_w+=',\"'+_e[_y]+'\",'+_e[_y];"
	"_x+=((_x!='')?',':'')+_e[_y];"
	"}"
	"eval('window.mo_EMBED_'+_v+' = function ('+_x+') {this.queueTask (\"'+_v+'\"'+_w+')}');"
	"};"
	"window.MO_E_r=function(_z,_A){"
	"eval('window.mo_EMBED_'+((typeof _A=='string')?_A:'get')+_z.charAt(0).toUpperCase()+_z.substr(1)+' = function () {return this.'+_z+'}');"
	"};"
	"MO_E_q('doMenuItem','item');"
	"MO_E_q('enableCallbacks','enable');"
	"MO_E_q('enableMetaEvents','enable');"
	"MO_E_q('engageAudio','engage');"
	"MO_E_q('noteOff','channel','note','velocity');"
	"MO_E_q('pause');"
	"MO_E_q('setAudioDevicePriority','audioDevicePriority');"
	"MO_E_q('setAutostart','autostart');"
	"MO_E_q('setChannelMute','channel','mute');"
	"MO_E_q('setChannelSolo','channel','solo');"
	"MO_E_q('setController','channel','controller','value');"
	"MO_E_q('setEndTime','position');"
	"MO_E_q('setGlobalMute','mute');"
	"MO_E_q('setLoop','loop');"
	"MO_E_q('setPanelDisplay','display');"
	"MO_E_q('setPanelMode','mode');"
	"MO_E_q('setPosition','position');"
	"MO_E_q('setReverbType','reverb');"
	"MO_E_q('setStartTime','position');"
	"MO_E_q('setTempo','tempo');"
	"MO_E_q('setTrackMute','track','mute');"
	"MO_E_q('setTrackSolo','track','solo');"
	"MO_E_q('setTransposable','channel','transposable');"
	"MO_E_q('setTranspose','intervals');"
	"MO_E_q('setVolume','volume');"
	"MO_E_q('stop','fade');"
	"MO_E_r('audioDevicePriority');"
	"MO_E_r('audioDeviceShared','is');"
	"MO_E_r('audioEngaged','is');"
	"MO_E_r('audioSupported','is');"
	"MO_E_r('autostart');"
	"MO_E_r('fileSize');"
	"MO_E_r('loop');"
	"MO_E_r('panelDisplay');"
	"MO_E_r('panelMode');"
	"MO_E_r('paused','Is');"
	"MO_E_r('playing','Is');"
	"MO_E_r('playLength');"
	"MO_E_r('reverbType');"
	"MO_E_r('tempo');"
	"MO_E_r('transpose');"
	"MO_E_r('version');"
	"window.mo_EMBED_initializeInstance=function(_d){"
	"_d.getQueuedTasks=MO_E_b;"
	"_d.recordPosition=MO_E_p;"
	"_d.getChannelMute=MO_E_e;"
	"_d.getChannelSolo=MO_E_f;"
	"_d.getController=MO_E_d;"
	"_d.getInfo=MO_E_m;"
	"_d.getPosition=MO_E_n;"
	"_d.getProgram=MO_E_g;"
	"_d.getTrackMute=MO_E_h;"
	"_d.getTrackSolo=MO_E_i;"
	"_d.getTransposable=MO_E_j;"
	"_d.noteOn=MO_E_k;"
	"_d.play=MO_E_l;"
	"_d.setProgram=MO_E_o;"
	"_d.doMenuItem=mo_EMBED_doMenuItem;"
	"_d.enableCallbacks=mo_EMBED_enableCallbacks;"
	"_d.enableMetaEvents=mo_EMBED_enableMetaEvents;"
	"_d.engageAudio=mo_EMBED_engageAudio;"
	"_d.noteOff=mo_EMBED_noteOff;"
	"_d.pause=mo_EMBED_pause;"
	"_d.setAudioDevicePriority=mo_EMBED_setAudioDevicePriority;"
	"_d.setAutostart=mo_EMBED_setAutostart;"
	"_d.setChannelMute=mo_EMBED_setChannelMute;"
	"_d.setChannelSolo=mo_EMBED_setChannelSolo;"
	"_d.setChannelSolo=mo_EMBED_setChannelSolo;"
	"_d.setController=mo_EMBED_setController;"
	"_d.setEndTime=mo_EMBED_setEndTime;"
	"_d.setGlobalMute=mo_EMBED_setGlobalMute;"
	"_d.setLoop=mo_EMBED_setLoop;"
	"_d.setPanelDisplay=mo_EMBED_setPanelDisplay;"
	"_d.setPanelMode=mo_EMBED_setPanelMode;"
	"_d.setPosition=mo_EMBED_setPosition;"
	"_d.setReverbType=mo_EMBED_setReverbType;"
	"_d.setStartTime=mo_EMBED_setStartTime;"
	"_d.setTempo=mo_EMBED_setTempo;"
	"_d.setTrackMute=mo_EMBED_setTrackMute;"
	"_d.setTrackSolo=mo_EMBED_setTrackSolo;"
	"_d.setTransposable=mo_EMBED_setTransposable;"
	"_d.setTranspose=mo_EMBED_setTranspose;"
	"_d.setVolume=mo_EMBED_setVolume;"
	"_d.stop=mo_EMBED_stop;"
	"_d.getAudioDevicePriority=mo_EMBED_getAudioDevicePriority;"
	"_d.isAudioDeviceShared=mo_EMBED_isAudioDeviceShared;"
	"_d.isAudioEngaged=mo_EMBED_isAudioEngaged;"
	"_d.isAudioSupported=mo_EMBED_isAudioSupported;"
	"_d.getAutostart=mo_EMBED_getAutostart;"
	"_d.getFileSize=mo_EMBED_getFileSize;"
	"_d.getLoop=mo_EMBED_getLoop;"
	"_d.getPanelDisplay=mo_EMBED_getPanelDisplay;"
	"_d.getPanelMode=mo_EMBED_getPanelMode;"
	"_d.IsPaused=mo_EMBED_IsPaused;"
	"_d.IsPlaying=mo_EMBED_IsPlaying;"
	"_d.getPlayLength=mo_EMBED_getPlayLength;"
	"_d.getReverbType=mo_EMBED_getReverbType;"
	"_d.getTempo=mo_EMBED_getTempo;"
	"_d.getTranspose=mo_EMBED_getTranspose;"
	"_d.getVersion=mo_EMBED_getVersion;"
	"_d._q=_d.queueTask=MO_E_c;"
	"_d._b='';"
	"_d._t=_d._u=0;"
	"_d.autostart=_d.loop=_d.playing=_d.paused=_d.ready=false;"
	"_d.playLength=_d.tempo=_d.fileSize=_d.reverbType=_d.volume=_d.transpose=_d.startTime=_d.endTime=0;"
	"_d.version=_d.panelMode=_d.panelDisplay='';"
	"_d.songInfo=new Object();"
	"_d.channels=new Array();"
	"for(var _h=1;_h<=16;_h++){"
	"_d.channels[_h]={"
	"mute:false,"
	"solo:false,"
	"transposable:false,"
	"program:0,"
	"controllers:{}"
	"}"
	"}"
	"_d.tracks=new Array();"
	"for(var _k=1;_k<=64;_k++){"
	"_d.tracks[_k]={"
	"mute:false,"
	"solo:false"
	"}"
	"}"
	"_d.scriptable=true;"
	"return(1);"
	"};"
	"}.toString();"
	"SUBSTITUTE[WINDOWREF]eval(MO_E_a.substring(MO_E_a.indexOf('{')+1,MO_E_a.lastIndexOf('}')));"
	"1;"

/**********
     TITLE: Scripting Interface Mimic - Instance Initializer Code
   VERSION: 1.0.6
    AUTHOR: Chris van Rensburg, Sean Echevarria
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

/***

The code in this module should be evaluated by the window that contains the matching EMBED instance, in order to initialize the EMBED instance with the SIM (Scripting Interface Mimic). The actual code that is passed should be dynamically generated by substituting appropriate values for the 'SUBSTITUTE[]' blocks in the structure.

SUBSTITUTE[WINDOWREF] = value of the EMBED tag's WINDOWREF attribute
SUBSTITUTE[ID] = value of the EMBED tag's ID attribute

return codes have the following meaning...

0: error, instance (or window containing it) may have died
1: no error, instance was successfully initialized

***/

try{SUBSTITUTE[WINDOWREF]mo_EMBED_initializeInstance(SUBSTITUTE[WINDOWREF]SUBSTITUTE[ID])}catch(_a){0}

/**********
     TITLE: Scripting Interface Mimic - Task Interchange Event Code
   VERSION: 1.0.6
    AUTHOR: Chris van Rensburg, Sean Echevarria
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

/***

The code in this module should be evaluated by the window that contains the matching EMBED instance, in order to perform a task interchange with the EMBED instance. The actual code that is passed should be dynamically generated by substituting appropriate values for the 'SUBSTITUTE[]' blocks in the structure.

SUBSTITUTE[WINDOWREF] = value of the EMBED tag's WINDOWREF attribute
SUBSTITUTE[ID] = value of the EMBED tag's ID attribute
SUBSTITUTE[statusUpdateCode] = a block of code, containing all the tasks that should be performed for the EMBED instance (ie. status update, callback execution). This code does not need to be passed to an eval function in the host document, so the code does not need to be passed as a string parameter and, consequently, the code only needs to be well-formed JavaScript code and no additional escaping of quote or backslash characters is required.

return codes have the following meaning...

'ERROR': error, instance (or window containing it) may have died
[any other value]: list of tasks to be executed by plug-in

***/

try{with(SUBSTITUTE[WINDOWREF]SUBSTITUTE[ID]){SUBSTITUTE[statusUpdateCode]getQueuedTasks()}}catch(_a){'ERROR'}