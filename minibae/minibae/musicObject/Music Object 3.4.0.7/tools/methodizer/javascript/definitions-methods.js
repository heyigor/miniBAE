/**********
     TITLE: Music Object Instance Method Definitions
   VERSION: 1.0.0
      TYPE: Shared Data File
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-1999 Beatnik, Inc. All Rights Reserved
  REQUIRES: object-method-and-parameter.js
**********/

methods = new Array (
	new Method ('addCallbackHandler','InstanceCallbackTypeSTR,Handler[Function|JavaScriptSTR]','InstanceCallbackTypeSTR,HandlerJavaScriptSTR'),
	new Method ('doMenuItem','MenuItemNameSTR'),
	new Method ('endVolumeFade'),
	new Method ('fadeTo','FadeToVolume0to100[,FadeTimeMsINT]','FadeToVolume0to100,FadeTimeMsINT'),
	new Method (
		'fadeVolume',
//		'[|[StartVolume0to100|[[StartVolume0to100|null],[EndVolume0to100|[[EndVolume0to100|null],[[Fade[BOOL|TimeMsINT]|[[[Fade[BOOL|TimeMsINT]|null],[FadeEndCallback[Function|JavaScriptSTR]|[[FadeEndCallback[Function|JavaScriptSTR]|null],FadeStepCallback[Function|JavaScriptSTR]]]]]]]]]]]]',
		'[StartVolume0to100|null],EndVolume0to100[|,Fade[BOOL|TimeMsINT][|,FadeEndCallback[Function|JavaScriptSTR][|,FadeStepCallback[Function|JavaScriptSTR]]]]',
		'StartVolume0to100,EndVolume0to100,FadeTimeMsINT,FadeEndCallbackJavaScriptSTR,FadeStepCallbackJavaScriptSTR'
	),
	new Method ('getAutostart'),
	new Method ('getChannelMute','MidiChannel1to16'),
	new Method ('getChannelSolo','MidiChannel1to16'),
	new Method ('getController','MidiChannel1to16,ControllerNumber0to127'),
	new Method ('getFileSize'),
	new Method ('getInfo','InfoFieldNameSTR'),
	new Method ('getLoop'),
	new Method ('getMonophonic','MidiChannel1to16'),
	new Method ('getPanelDisplay'),
	new Method ('getPanelMode'),
	new Method ('getPlayLength'),
	new Method ('getPosition'),
	new Method ('getProgram','MidiChannel1to16'),
	new Method ('getTempo'),
	new Method ('getTrackMute','TrackNumber1to64'),
	new Method ('getTrackSolo','TrackNumber1to64'),
	new Method ('getTranspose'),
	new Method ('getTransposable','MidiChannel1to16'),
	new Method ('getVolume','[VolumeScalarNameSTR]'),
	new Method ('getOutputVolume'),
	new Method ('isPaused'),
	new Method ('isPlaying'),
	new Method ('magicEmbed','EmbedLineSTR',null,false),
	new Method ('noteOff','MidiChannel1to16[|,Note[Number0to127|NameSTR]]','MidiChannel1to16,NoteNumber0to127'),
	new Method (
		'noteOn',
		'MidiChannel1to16,[|[GMBank,GM|SpecialBank,Special|UserBank,User]BankInstrumentINT,|BankNumber0to2,ProgramNumber0to255,]Note[Number0to127|NameSTR],Velocity0to127[,NoteDurationMsINT]',
		'MidiChannel1to16,NoteNumber0to127,Velocity0to127'
	),
	new Method ('onAudibleStateChange','[|Handler[Function|JavaScriptSTR]|null]','HandlerJavaScriptSTR'),
	new Method ('onLoad','[|Handler[Function|JavaScriptSTR]|null]','HandlerJavaScriptSTR'),
	new Method ('onMetaEvent','[|HandlerFunction|null]','HandlerFunction'),
	new Method ('onOutputVolumeChange','[|Handler[Function|JavaScriptSTR]|null]','HandlerJavaScriptSTR'),
	new Method ('onPause','[|Handler[Function|JavaScriptSTR]|null]','HandlerJavaScriptSTR'),
	new Method ('onPlay','[|Handler[Function|JavaScriptSTR]|null]','HandlerJavaScriptSTR'),
	new Method ('onReady','[|Handler[Function|JavaScriptSTR]|null]','HandlerJavaScriptSTR'),
	new Method ('onStop','[|Handler[Function|JavaScriptSTR]|null]','HandlerJavaScriptSTR'),
	new Method ('pause','[|Fade[BOOL|TimeMsINT]]',''),
	new Method ('play','[|LoopingBOOL|[LoopingBOOL,][[|Test]FileUrlSTR|GroovoidNameSTR]]',''),
	new Method (
		'playNote',
		'MidiChannel1to16,[|[GMBank,GM|SpecialBank,Special|UserBank,User]BankInstrumentINT,|BankNumber0to2,ProgramNumber0to255,]Note[Number0to127|NameSTR],Velocity0to127,NoteDurationMsINT',
		'MidiChannel1to16,NoteNumber0to127,Velocity0to127,NoteDurationMsINT'
	),
	new Method ('preloadEmbed','FileUrlSTR[,ExtraEmbedAttributesSTR]','FileUrlSTR',false),
	new Method ('setAutostart','AutostartBOOL'),
	new Method ('setChannelMute','MidiChannel0to16,MuteStateBOOL'),
	new Method ('setChannelSolo','MidiChannel0to16,SoloStateBOOL'),
	new Method ('setController','MidiChannel0to16,ControllerNumber0to127,ControllerValue0to127'),
	new Method ('setEndTime','[SongPositionMsINT]','SongPositionMsINT'),
	new Method ('setLoop','LoopingBOOL'),
	new Method ('setMonophonic','MidiChannel0to16,MonophonicStateBOOL'),
	new Method ('setPanelDisplay','PanelDisplaySTR'),
	new Method ('setPanelMode','PanelModeSTR'),
	new Method ('setPosition','SongPositionMsINT'),
	new Method (
		'setProgram',
		'MidiChannel0to16,[[GMBank,GM|SpecialBank,Special|UserBank,User]BankInstrumentINT|[BankNumber0to2,]ProgramNumber0to255]',
		'MidiChannel0to16,ProgramNumber0to255'
	),
	new Method ('setStartTime','[SongPositionMsINT]','SongPositionMsINT'),
	new Method ('setTempo','TempoBPM0to300'),
	new Method ('setTrackMute','TrackNumber0to64,MuteStateBOOL'),
	new Method ('setTrackSolo','TrackNumber0to64,SoloStateBOOL'),
	new Method ('setTransposable','MidiChannel0to16,TransposableBOOL'),
	new Method ('setTranspose','MusicalIntervalsINT'),
	new Method ('setVolume','[|Volume0to100|VolumeScalarNameSTR,[Volume0to100|-1]]','Volume0to100'),
	new Method ('stop','[|FadeBOOL|FadeTimeMsINT]',''),
	new Method ('stubEmbed','FileUrlSTR',null,false),
	/*** Static Methods ***/
	new Method ('Music.addCallbackHandler','StaticCallbackTypeSTR,Handler[Function|JavaScriptSTR]','StaticCallbackTypeSTR,HandlerJavaScriptSTR'),
	new Method ('Music.onAudioEngagedChange','[|Handler[Function|JavaScriptSTR]|null]','HandlerJavaScriptSTR'),
	new Method ('Music.onEngageAudio','[|Handler[Function|JavaScriptSTR]|null]','HandlerJavaScriptSTR'),
	new Method ('Music.engageAudio','AudioEngagedBOOL'),
	new Method ('Music.getAudioDevicePriority'),
	new Method ('Music.getNoteName','NoteNumber0to127'),
	new Method ('Music.getNoteNumber','NoteNameSTR'),
	new Method ('Music.getReverbType'),
	new Method ('Music.hasMinVersion','MinimumVersionSTR'),
	new Method ('Music.installPlayer'),
	new Method ('Music.isAudioDeviceShared'),
	new Method ('Music.isAudioEngaged'),
	new Method ('Music.isAudioSupported'),
	new Method ('Music.magicEmbed','EmbedLineSTR',null,false),
	new Method ('Music.setAudioDevicePriority','AudioDevicePriority0to3'),
	new Method ('Music.setGlobalMute','MuteStateBOOL'),
	new Method ('Music.setReverbType','ReverbType1to11'),
	new Method (
		'Music.isPlayerCompatible',
		'[|MinimumVersionSTR|[[MinimumVersionSTR|null],[SilentIfInadequateBOOL|[[SilentIfInadequateBOOL|null],ShowCompatibilityPromptBOOL]]]]',
		'MinimumVersionSTR,SilentIfInadequateBOOL,ShowCompatibilityPromptBOOL'
	),
	new Method ('Music.promptClose'),
	new Method (
		'Music.promptUser',
//		'HeadingSTR,MessageSTR[|,[OkTextSTR|[[OkTextSTR|null],[OkJavaScriptSTR|[[OkJavaScriptSTR|null],[CancelTextSTR|[[CancelTextSTR|null],[CancelJavaScriptSTR|[[CancelJavaScriptSTR|null],ShowStatusBarBOOL]]]]]]]]]',
		'HeadingSTR,MessageSTR[|,OkTextSTR[|,OkJavaScriptSTR[|,CancelTextSTR[|,CancelJavaScriptSTR[|,ShowStatusBarBOOL]]]]]',
		'HeadingSTR,MessageSTR,OkTextSTR,OkJavaScriptSTR,CancelTextSTR,CancelJavaScriptSTR,ShowStatusBarBOOL'
	),
	new Method ('Music.stopAll','')
);
