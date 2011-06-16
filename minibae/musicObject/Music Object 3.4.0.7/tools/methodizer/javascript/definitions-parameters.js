/**********
     TITLE: Music Object Parameter Definitions
   VERSION: 1.0.0
      TYPE: Shared Data File
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-1999 Beatnik, Inc. All Rights Reserved
  REQUIRES: object-method-and-parameter.js
**********/

new Parameter ('-1','integer','-1','-1',false);
new Parameter ('AutostartBOOL','boolean','true','',true);
new Parameter ('BankNumber0to2','integer','0 GM bank','0 GM bank,1 Special bank,2 User bank',true);
new Parameter ('BOOL','boolean','false','',false);
new Parameter ('ControllerValue0to127','integer','64','0:127',true);
new Parameter ('EndVolume0to100','integer','0','0:100',false);
new Parameter ('INT','integer','0','',false);
new Parameter ('FadeBOOL','boolean','true','',true);
new Parameter ('FadeTimeMsINT','integer','1500','0:300000',true);
new Parameter ('FileUrlSTR','link','','',true);
new Parameter ('Function','function','','',true);
new Parameter ('GMBank','integer','0','0',false);
new Parameter ('LoopingBOOL','boolean','true','',true);
new Parameter ('MenuItemNameSTR','string','About','Copyright,Play,Pause,Stop,PlayURL,Loud,Quiet,Mute,System,Song,Help,About,News,Register',true),
new Parameter ('MuteStateBOOL','boolean','true','',true);
new Parameter ('NoteDurationMsINT','integer','2000','0:200000',true);
new Parameter ('null','function','null','null',false);
new Parameter ('PanelDisplaySTR','string','SYSTEM','SYSTEM,SONG',true);
new Parameter ('PanelModeSTR','string','SCOPE','SCOPE,METERS,COPYRIGHT',true);
new Parameter ('ProgramNumber0to255','integer','0','0:255',true);
new Parameter ('SoloStateBOOL','boolean','true','',true);
new Parameter ('SongPositionMsINT','integer','0','0:100000',true);
new Parameter ('SpecialBank','integer','1','1',false);
new Parameter ('StartVolume0to100','integer','100','0:100',false);
new Parameter ('JavaScriptSTR','string','','',true);
new Parameter ('STR','string','','',false);
new Parameter ('TempoBPM0to300','integer','120','0:300',true);
new Parameter ('TrackNumber1to64','integer','1','1:64',true);
new Parameter ('TrackNumber0to64','integer','1','0:64',true);
new Parameter ('TransposableBOOL','boolean','true','',false);
new Parameter ('UserBank','integer','2','2',false);
new Parameter ('UserBankInstrumentINT','integer','0','0:255',false);
new Parameter ('Velocity0to127','integer','127','0:127',true);
new Parameter ('Volume0to100','integer','100','0:100',true);

new Parameter (
	'InstanceCallbackTypeSTR',
	'string',
	'onPlay',
	'onAudibleStateChange,onLoad,onMetaEvent,onOutputVolumeChange,onReady,onPause,onPlay,onStop',
	true
);

new Parameter (
	'StaticCallbackTypeSTR',
	'string',
	'onAudioEngagedChange',
	'onAudioEngagedChange,onEngageAudio',
	true
);

new Parameter (
	'GroovoidNameSTR',
	'string',
	'UI-BeepClick1',
	'UI-BeepClick1,UI-CarriageReturn,UI-MouseOverDrone1,UI-OpenOrClose1,UI-OpenOrClose2,UI-OpenOrClose3,UI-SimpleClick1,UI-SimpleClick2,UI-SystemBeep,UI-TeletypeBurst2,UI-TypeOneKey,UI-Choice1,UI-Question,UI-Chimes,UI-FunnyBeep,Hits-BlastOff,Hits-Kicker1,Hits-Kicker2,Hits-Kicker5,Hits-LudwigVan,Hits-Magic,Hits-MysteryChime,Hits-SportsOrganCharge,Hits-ZoomAndHit,Fanfare-Arrival,Fanfare-Cascade,Fanfare-Communicator,Fanfare-CorpSting1,Fanfare-Finished,Fanfare-Horserace,Fanfare-Mars,Fanfare-Majesty,Fanfare-News,Fanfare-Presenting,Fanfare-Sports,Background-Beeps,Background-Clock,Background-Dubby,Background-Funky,Background-Groove2,Background-InfoPulse,Background-Latin,Background-Piano,Background-SimpleTyping,Background-Smooth,Background-StillWaiting,Background-Teletype,Misc-CashRegister,Misc-ThruPhoneRing',
	true
);

new Parameter (
	'MidiChannel1to16',
	'integer',
	'01',
	'01,02,03,04,05,06,07,08,09,10 percussion channel,11,12,13,14,15,16',
	true
);

new Parameter (
	'MidiChannel0to16',
	'integer',
	'01',
	'00 all channels 1-16,01,02,03,04,05,06,07,08,09,10 percussion channel,11,12,13,14,15,16',
	true
);

new Parameter (
	'ReverbType1to11',
	'integer',
	'01 no reverb',
	'01 no reverb,02 Igor\'s Closet,03 Igor\'s Garage,04 Igor\'s Acoustic Lab,05 Igor\'s Cavern,06 Igor\'s Dungeon,07 Small Reflections,08 Early Reflections,09 Basement,10 Banquet Hall,11 Catacombs',
	true
);

new Parameter (
	'AudioDevicePriority0to3',
	'integer',
	'1 desire engaged only when focused',
	'0 desire disengaged when focused, 1 desire engaged only when focused,2 always desire engaged (but yield to 0),3 always desire engaged (do not yield to 0)',
	true
);

new Parameter (
	'ControllerNumber0to127',
	'integer',
	'7 volume',
	'0 bank select,1 modulation,7 volume,10 pan,11 expression,91 effect: reverb,93 effect: chorus,98 non registered parameter LSB,99 non registered parameter MSB,100 registered parameter LSB,101 registered parameter MSB,121 reset all controllers',
	true
);

new Parameter (
	'MusicalIntervalsINT',
	'integer',
	'0 no transposition',
	'36 up 3 octaves,35,34,33,32,31,30,29,28,27,26,25,24 up 2 octaves,23,22,21,20,19,18,17,16,15,14,13,12 up 1 octave,11,10,9,8,7,6,5,4,3,2,1,0 no transposition,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12 down 1 octave,-13,-14,-15,-16,-17,-18,-19,-20,-21,-22,-23,-24 down 2 octaves,-25,-26,-27,-28,-29,-30,-31,-32,-33,-34,-35,-36 down 3 octaves',
	false
);

new Parameter (
	'NoteNumber0to127',
	'integer',
	'60 C4 - Middle C',
	'127 G9,126 F#9,125 F9,124 E9,123 D#9,122 D9,121 C#9,120 C9,119 B8,118 A#8,117 A8,116 G#8,115 G8,114 F#8,113 F8,112 E8,111 D#8,110 D8,109 C#8,108 C8,107 B7,106 A#7,105 A7,104 G#7,103 G7,102 F#7,101 F7,100 E7,099 D#7,098 D7,097 C#7,096 C7,095 B6,094 A#6,093 A6,092 G#6,091 G6,090 F#6,089 F6,088 E6,087 D#6,086 D6,085 C#6,084 C6,083 B5,082 A#5,081 A5,080 G#5,079 G5,078 F#5,077 F5,076 E5,075 D#5,074 D5,073 C#5,072 C5,071 B4,070 A#4,069 A4,068 G#4,067 G4,066 F#4,065 F4,064 E4,063 D#4,062 D4,061 C#4,060 C4 - Middle C,059 B3,058 A#3,057 A3,056 G#3,055 G3,054 F#3,053 F3,052 E3,051 D#3,050 D3,049 C#3,048 C3,047 B2,046 A#2,045 A2,044 G#2,043 G2,042 F#2,041 F2,040 E2,039 D#2,038 D2,037 C#2,036 C2,035 B1,034 A#1,033 A1,032 G#1,031 G1,030 F#1,029 F1,028 E1,027 D#1,026 D1,025 C#1,024 C1,023 B0,022 A#0,021 A0,020 G#0,019 G0,018 F#0,017 F0,016 E0,015 D#0,014 D0,013 C#0,012 C0,011 B-1,010 A#-1,009 A-1,008 G#-1,007 G-1,006 F#-1,005 F-1,004 E-1,003 D#-1,002 D-1,001 C#-1,000 C-1',
	true
);

new Parameter (
	'NoteNameSTR',
	'string',
	'C4',
	'G9,F#9,F9,E9,D#9,D9,C#9,C9,B8,A#8,A8,G#8,G8,F#8,F8,E8,D#8,D8,C#8,C8,B7,A#7,A7,G#7,G7,F#7,F7,E7,D#7,D7,C#7,C7,B6,A#6,A6,G#6,G6,F#6,F6,E6,D#6,D6,C#6,C6,B5,A#5,A5,G#5,G5,F#5,F5,E5,D#5,D5,C#5,C5,B4,A#4,A4,G#4,G4,F#4,F4,E4,D#4,D4,C#4,C4,B3,A#3,A3,G#3,G3,F#3,F3,E3,D#3,D3,C#3,C3,B2,A#2,A2,G#2,G2,F#2,F2,E2,D#2,D2,C#2,C2,B1,A#1,A1,G#1,G1,F#1,F1,E1,D#1,D1,C#1,C1,B0,A#0,A0,G#0,G0,F#0,F0,E0,D#0,D0,C#0,C0,B-1,A#-1,A-1,G#-1,G-1,F#-1,F-1,E-1,D#-1,D-1,C#-1,C-1',
	true
);

new Parameter (
	'GMBankInstrumentINT',
	'integer',
	'000 Piano',
	'000 Piano,001 Bright Piano,002 Electric Grand,003 Honky Tonk Piano,004 Electric Piano 1,005 Electric Piano 2,006 Harpsichord,007 Clavinet,008 Celesta,009 Glockenspiel,010 Music Box,011 Vibraphone,012 Marimba,013 Xylophone,014 Tubular Bell,015 Dulcimer,016 Hammond Organ,017 Perc Organ,018 Rock Organ,019 Church Organ,020 Reed Organ,021 Accordion,022 Harmonica,023 Tango Accordion,024 Nylon Str Guitar,025 Steel String Guitar,026 Jazz Electric Gtr,027 Clean Guitar,028 Muted Guitar,029 Overdrive Guitar,030 Distortion Guitar,031 Guitar Harmonics,032 Acoustic Bass,033 Fingered Bass,034 Picked Bass,035 Fretless Bass,036 Slap Bass 1,037 Slap Bass 2,038 Syn Bass 1,039 Syn Bass 2,040 Violin,041 Viola,042 Cello,043 Contrabass,044 Tremolo Strings,045 Pizzicato Strings,046 Orchestral Harp,047 Timpani,048 Ensemble Strings,049 Slow Strings,050 Synth Strings 1,051 Synth Strings 2,052 Choir Aahs,053 Voice Oohs,054 Syn Choir,055 Orchestra Hit,056 Trumpet,057 Trombone,058 Tuba,059 Muted Trumpet,060 French Horn,061 Brass Ensemble,062 Syn Brass 1,063 Syn Brass 2,064 Soprano Sax,065 Alto Sax,066 Tenor Sax,067 Baritone Sax,068 Oboe,069 English Horn,070 Bassoon,071 Clarinet,072 Piccolo,073 Flute,074 Recorder,075 Pan Flute,076 Bottle Blow,077 Shakuhachi,078 Whistle,079 Ocarina,080 Syn Square Wave,081 Syn Saw Wave,082 Syn Calliope,083 Syn Chiff,084 Syn Charang,085 Syn Voice,086 Syn Fifths Saw,087 Syn Brass and Lead,088 Fantasia,089 Warm Pad,090 Polysynth,091 Space Vox,092 Bowed Glass,093 Metal Pad,094 Halo Pad,095 Sweep Pad,096 Ice Rain,097 Soundtrack,098 Crystal,099 Atmosphere,100 Brightness,101 Goblins,102 Echo Drops,103 Sci Fi,104 Sitar,105 Banjo,106 Shamisen,107 Koto,108 Kalimba,109 Bag Pipe,110 Fiddle,111 Shanai,112 Tinkle Bell,113 Agogo,114 Steel Drums,115 Woodblock,116 Taiko Drum,117 Melodic Tom,118 Syn Drum,119 Reverse Cymbal,120 Guitar Fret Noise,121 Breath Noise,122 Seashore,123 Bird,124 Telephone,125 Helicopter,126 Applause,127 Gunshot,155 Hi-Q,156 Slap,157 Scratch Push,158 Scratch Pull,159 Sticks,160 Square Click,161 Metronome Click,162 Metronome Bell,163 Low Kick,164 Kick,165 Rimclik,166 Electric Snare,167 Handclap,168 Acoustic Snare,169 Tom Low,170 Closed Hi-Hat,171 Tom Hi,172 Pedal Hi-Hat,173 Tom Hi,174 Open Hi-Hat,175 Low Mid Tom,176 High Mid Tom,177 Crash Cymbal 1,178 High Tom,179 Ride Cymbal 1,180 Chinese Cymbal,181 Ride Bell,182 Tambourine,183 Splash Cymbal,184 Cowbell,185 Crash Cymbal 2,186 Vibraslap,187 Ride Cymbal 2,188 High Bongo,189 Low Bongo,190 Mute High Conga,191 Open High Conga,192 Low Conga,193 High Timbale,194 Low Timbale,195 High Agogo,196 Low Agogo,197 Cabasa,198 Maracas,199 Short Whistle,200 Long Whistle,201 Short Guiro,202 Long Guiro,203 Claves,204 High Wood Block,205 Low Wood Block,206 Mute Cuica,207 Open Cuica,208 Close Triangle,209 Open Triangle,210 Shaker,211 Jingle Bell,212 Bell Tree,213 Castanets,214 Mute Surdo,215 Open Surdo,222 Brush Snare High,223 Brush Snare Low',
	false
);

new Parameter (
	'SpecialBankInstrumentINT',
	'integer',
	'000 Piano',
	'000 Soft Piano,001 Reflection Piano,002 Flange Piano,003 Octopus Piano,004 Flange Electric Piano,005 Tremolo Electric Piano,006 Chorus Harpsichord,007 Ondioline,008 Curious Chime,009 Swirly Bell,010 Echo Bell,011 Vibraphone 2,012 Soft Marimba,013 Percolator,014 Reverse Bell,015 Percussive Piano,016 Rot. Speaker Organ 1,017 Bright Perc Organ,018 Growl Organ,019 Chorus Church Organ,020 Sawteeth,021 Expressive Accordion,022 Corny Harmonica,023 Squeeze Box,024 Acoustic Bass Gtr,025 Mute Guitar 2,026 Chorus Jazz Guitar,027 Tremolo Guitar,028 Echo Mute Bass,029 Grungey Guitar,030 Flange Guitar,031 Flange Guitar Stab,032 Dark Syn Bass,033 Jungle Bass,034 Jungle Bass 2,035 Chorus Bass,036 Synth Slap Bass,037 Lowrider,038 OB Bass,039 OB Bass 2,040 Synth Violin,041 Robot Threat,042 Formula 1,043 Deep Bass Stab,044 Trill Strings,045 Cello Stroke,046 VibHarp,047 Tuned Drum,048 sfz Strings,049 Moving Strings,050 Heads of Space,051 Slow Synth Strings,052 Slow Choir Aahs,053 Pulsing Voices,054 Cloudscapes,055 Hype Orch Hit,056 Accent Trumpet,057 Accent Trombone,058 Mellow Trombone,059 Muted Trumpet 2,060 French Horn Swell,061 Brass Ensemble 2,062 Syn Brass 3,063 Syn Brass 4,064 Lyrical Sax,065 Accent Sax,066 Slow Sax,067 Staccato Sax,068 Lyrical Oboe,069 Soft Lead Synth,070 Staccato Bassoon,071 Smooth Clarinet,072 Accent Piccolo,073 Lyrical Flute,074 Mello Breath,075 Peruvian,076 Breath Echoes,077 Apparition,078 Humorous Whistle,079 Strobes,080 Teletronic,081 5th Pulse,082 Moon Jelly,083 Ricochet Pad,084 Rock Radiation,085 Solo Vox 2,086 5th Saw Drone,087 Lead Synth 2,088 Fantasy Bells,089 Slow Warm Pad,090 Polysynth 2,091 50\'s Sci-Fi,092 Watery Glass,093 UFOs,094 Soprano,095 Refractions,096 Comet Tails,097 Soundtrack 2,098 Friendly Data,099 Analog Sequence,100 Luminous Voice,101 Goblins 2,102 Tick Blok,103 Droplet,104 Droplet 2,105 Video Game 1,106 Video Game 2,107 Digi-Dodo,108 Fairy Godmother,109 Alarm,110 Hoverbug,111 Crickets,112 Beatnik,113 Teletype,114 Carriage Return,115 Typewriter Key,116 Cash Register,117 Thru Phone Chirp,118 Whipped,119 Pop Click,120 Metal Chirp,121 Poing,122 Metal Spray,123 FlyBy 1,124 FlyBy 2,125 Cosmic Ray,126 SampHold,127 SampHold 2,174 Tablaesque - Low,175 Tablaesque - Hi,176 Nine Inch Kick,177 HippiShake,178 ScienceTom,179 Click,180 Rvs Cymb Pan Long,181 Rvs Cymb Pan Med,182 Rvs Cymb Pan Short,183 Woodstick,184 ElectroBrite,185 SloLaser,186 UFO-by,187 Air Snare,188 Weird Snare,189 Flange Snare,190 Ping Drum,191 Itchy-Scratch,192 Itchy-Scratch 2,193 Hi Metallic Snare,194 Noise Echo,195 Buzzy Hit,196 Rez Hat,197 Bucket Hit,198 Ice Cube,199 Explosive Kick,200 Mutant Cowbell,201 Temple Bell,202 Hi Temple Gong,203 Thuddy Kick,204 Lo Bell,205 Sine Chirp,206 Hollow Metal Hit,207 Woodblock,208 Reverse Cymbal,209 Tablaesque Slap,210 Sonar Blip,211 Sine Hit,212 Sine Block,213 Krell Tone,214 Sine Kick,215 Hi Tone,216 Hi Click,217 Gate Tone,218 Chem Tone,219 Dub Kick',
	false
);

new Parameter (
	'InfoFieldNameSTR',
	'string',
	'title',
	'title,performer,composer,copyright,publisher,use,licensee,term,expiration,notes,index',
	true
);

new Parameter (
	'TestFileUrlSTR',
	'string',
	'../../music/ambient.rmf',
	'../../music/ambient.rmf,../../music/beatnik-echo.rmf,../../music/classical.rmf,../../music/house.rmf,../../music/hyperactive.rmf,../../music/jungle.rmf,../../music/lounge.rmf,../../music/modern-rock.rmf,../../music/mystical.rmf,../../music/sfx-medley.rmf,../../music/stub.rmf,../../music/techno.rmf,../../music/trance.rmf',
	false
);
