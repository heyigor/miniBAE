new ListboxNav (
	'listboxNav',
	'DEMOS',
	'../style/images/',
	22,19,
	false,
	new Array (
		new ListboxNav.Item (
			'Introduction',
			'intro.html'
		),

		new ListboxNav.Item (),

		new ListboxNav.Item (
			'The fadeVolume Method',
			'demos/fade-volume/index.html'
		),
		new ListboxNav.Item (
			'The noteOn, noteOff, and playNote Methods',
			'demos/musical-notes.html'
		),
		new ListboxNav.Item (
			'The Chords Extension',
			'demos/chords.html'
		),
		new ListboxNav.Item (
			'The playRandomNote Method',
			'demos/play-random-note.html'
		),
		new ListboxNav.Item (
			'The setTracksMute and setTracksSolo Methods',
			'demos/tracks-mute-solo.html'
		),
/***
		new ListboxNav.Item (
			'The Ducking Extension',
			'demos/ducking.html'
		),
***/
		new ListboxNav.Item (
			'The playNoteSlide Method',
			'demos/play-note-slide.html'
		)
	)
);

listboxNav.generateNavigation ();
