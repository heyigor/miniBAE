/**********
     TITLE: Music Object Extension - Instruments
   VERSION: 1.1.1
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_xb"*/

function mo_getProgramName (_p1,_p2) {
	if (Music.isInstance (this)) {
		return Music.getProgramName (this.getController (_p1,0),this.getProgram (_p1));
	} else {
		return (_p1 == 2) ? ('User Program ' + _p2) : Music.programNames [_p1] [_p2];
	}
}

function mo_getPercussionName (_bankNo,_noteNo) {
	return Music.getProgramName (_bankNo,128 + _noteNo);
}

/*** Public Static Properties ***/

Music.programNames = new Array (
	new Array (
		/*** GM Chromatic ***/
		'Piano','Bright Piano','Electric Grand','Honky Tonk Piano','Electric Piano 1','Electric Piano 2','Harpsichord','Clavinet','Celesta','Glockenspiel','Music Box','Vibraphone','Marimba','Xylophone','Tubular Bell','Dulcimer','Hammond Organ','Perc Organ','Rock Organ','Church Organ','Reed Organ','Accordion','Harmonica','Tango Accordion','Nylon Str Guitar','Steel String Guitar','Jazz Electric Gtr','Clean Guitar','Muted Guitar','Overdrive Guitar','Distortion Guitar','Guitar Harmonics','Acoustic Bass','Fingered Bass','Picked Bass','Fretless Bass','Slap Bass 1','Slap Bass 2','Syn Bass 1','Syn Bass 2','Violin','Viola','Cello','Contrabass','Tremolo Strings','Pizzicato Strings','Orchestral Harp','Timpani','Ensemble Strings','Slow Strings','Synth Strings 1','Synth Strings 2','Choir Aahs','Voice Oohs','Syn Choir','Orchestra Hit','Trumpet','Trombone','Tuba','Muted Trumpet','French Horn','Brass Ensemble','Syn Brass 1','Syn Brass 2','Soprano Sax','Alto Sax','Tenor Sax','Baritone Sax','Oboe','English Horn','Bassoon','Clarinet','Piccolo','Flute','Recorder','Pan Flute','Bottle Blow','Shakuhachi','Whistle','Ocarina','Syn Square Wave','Syn Saw Wave','Syn Calliope','Syn Chiff','Syn Charang','Syn Voice','Syn Fifths Saw','Syn Brass and Lead','Fantasia','Warm Pad','Polysynth','Space Vox','Bowed Glass','Metal Pad','Halo Pad','Sweep Pad','Ice Rain','Soundtrack','Crystal','Atmosphere','Brightness','Goblins','Echo Drops','Sci Fi','Sitar','Banjo','Shamisen','Koto','Kalimba','Bag Pipe','Fiddle','Shanai','Tinkle Bell','Agogo','Steel Drums','Woodblock','Taiko Drum','Melodic Tom','Syn Drum','Reverse Cymbal','Guitar Fret Noise','Breath Noise','Seashore','Bird','Telephone','Helicopter','Applause','Gunshot',
		/*** GM Percussion ***/
		'','','','','','','','','','','','','','','','','','','','','','','','','','','','Hi-Q','Slap','Scratch Push','Scratch Pull','Sticks','Square Click','Metronome Click','Metronome Bell','Low Kick','Kick','Rimclik','Electric Snare','Handclap','Acoustic Snare','Tom Low','Closed Hi-Hat','Tom Hi','Pedal Hi-Hat','Tom Hi','Open Hi-Hat','Low Mid Tom','High Mid Tom','Crash Cymbal 1','High Tom','Ride Cymbal 1','Chinese Cymbal','Ride Bell','Tambourine','Splash Cymbal','Cowbell','Crash Cymbal 2','Vibraslap','Ride Cymbal 2','High Bongo','Low Bongo','Mute High Conga','Open High Conga','Low Conga','High Timbale','Low Timbale','High Agogo','Low Agogo','Cabasa','Maracas','Short Whistle','Long Whistle','Short Guiro','Long Guiro','Claves','High Wood Block','Low Wood Block','Mute Cuica','Open Cuica','Close Triangle','Open Triangle','Shaker','Jingle Bell','Bell Tree','Castanets','Mute Surdo','Open Surdo','','','','','','','Brush Snare High','Brush Snare Low','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','',''
	),
	new Array (
		/*** Special Chromatic ***/
		'Soft Piano','Reflection Piano','Flange Piano','Octopus Piano','Flange Electric Piano','Tremolo Electric Piano','Chorus Harpsichord','Ondioline','Curious Chime','Swirly Bell','Echo Bell','Vibraphone 2','Soft Marimba','Percolator','Reverse Bell','Percussive Piano','Rot. Speaker Organ 1','Bright Perc Organ','Growl Organ','Chorus Church Organ','Sawteeth','Expressive Accordion','Corny Harmonica','Squeeze Box','Acoustic Bass Gtr','Mute Guitar 2','Chorus Jazz Guitar','Tremolo Guitar','Echo Mute Bass','Grungey Guitar','Flange Guitar','Flange Guitar Stab','Dark Syn Bass','Jungle Bass','Jungle Bass 2','Chorus Bass','Synth Slap Bass','Lowrider','OB Bass','OB Bass 2','Synth Violin','Robot Threat','Formula 1','Deep Bass Stab','Trill Strings','Cello Stroke','VibHarp','Tuned Drum','sfz Strings','Moving Strings','Heads of Space','Slow Synth Strings','Slow Choir Aahs','Pulsing Voices','Cloudscapes','Hype Orch Hit','Accent Trumpet','Accent Trombone','Mellow Trombone','Muted Trumpet 2','French Horn Swell','Brass Ensemble 2','Syn Brass 3','Syn Brass 4','Lyrical Sax','Accent Sax','Slow Sax','Staccato Sax','Lyrical Oboe','Soft Lead Synth','Staccato Bassoon','Smooth Clarinet','Accent Piccolo','Lyrical Flute','Mello Breath','Peruvian','Breath Echoes','Apparition','Humorous Whistle','Strobes','Teletronic','5th Pulse','Moon Jelly','Ricochet Pad','Rock Radiation','Solo Vox 2','5th Saw Drone','Lead Synth 2','Fantasy Bells','Slow Warm Pad','Polysynth 2','50\'s Sci-Fi','Watery Glass','UFOs','Soprano','Refractions','Comet Tails','Soundtrack 2','Friendly Data','Analog Sequence','Luminous Voice','Goblins 2','Tick Blok','Droplet','Droplet 2','Video Game 1','Video Game 2','Digi-Dodo','Fairy Godmother','Alarm','Hoverbug','Crickets','Beatnik','Teletype','Carriage Return','Typewriter Key','Cash Register','Thru Phone Chirp','Whipped','Pop Click','Metal Chirp','Poing','Metal Spray','FlyBy 1','FlyBy 2','Cosmic Ray','SampHold','SampHold 2',
		/*** Special Percussion ***/
		'','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','Tablaesque - Low','Tablaesque - Hi','Nine Inch Kick','HippiShake','ScienceTom','Click','Rvs Cymb Pan Long','Rvs Cymb Pan Med','Rvs Cymb Pan Short','Woodstick','ElectroBrite','SloLaser','UFO-by','Air Snare','Weird Snare','Flange Snare','Ping Drum','Itchy-Scratch','Itchy-Scratch 2','Hi Metallic Snare','Noise Echo','Buzzy Hit','Rez Hat','Bucket Hit','Ice Cube','Explosive Kick','Mutant Cowbell','Temple Bell','Hi Temple Gong','Thuddy Kick','Lo Bell','Sine Chirp','Hollow Metal Hit','Woodblock','Reverse Cymbal','Tablaesque Slap','Sonar Blip','Sine Hit','Sine Block','Krell Tone','Sine Kick','Hi Tone','Hi Click','Gate Tone','Chem Tone','Dub Kick','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','','',''
	)
);

/*** Public Instance Methods ***/

Music.addPublicInstanceMethods ('getProgramName');

/*** Public Static Methods ***/

Music.addPublicStaticMethods ('getProgramName','getPercussionName');
