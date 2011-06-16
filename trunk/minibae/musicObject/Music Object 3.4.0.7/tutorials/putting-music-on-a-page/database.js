new ListboxNav (
	'listboxNav',
	'Putting Music On a Page',
	'../../style/images/',
	22,19,
	false,
	new Array (
		new ListboxNav.Item (
			'Introduction',
			'steps/intro.html'
		),

		new ListboxNav.Item (),

		new ListboxNav.Item (
			'STEP 1: A Simple EMBED',
			'steps/a-simple-embed.html'
		),
		new ListboxNav.Item (
			'STEP 2: The DISPLAY and MODE attribute',
			'steps/display-and-mode.html'
		),
		new ListboxNav.Item (
			'STEP 3: Scope Out the Music',
			'steps/scope.html'
		),
		new ListboxNav.Item (
			'STEP 4: Dr. Frankenscope',
			'steps/frankenscope.html'
		),
		new ListboxNav.Item (
			'STEP 5: Tiny Little Strips',
			'steps/tiny-little-strips.html'
		),
		new ListboxNav.Item (
			'STEP 6: Embedding a Mute Button',
			'steps/embedding-a-mute-button.html'
		),
		new ListboxNav.Item (
			'STEP 7: A Gratuitous Scope',
			'steps/gratuitous-scope.html'
		),
		new ListboxNav.Item (
			'STEP 8: Music You Cannot See',
			'steps/hidden.html'
		),
		new ListboxNav.Item (
			'STEP 9: Other Control Panel Layouts',
			'steps/other-layouts.html'
		),
		new ListboxNav.Item (
			'STEP 10: Embedding Groovoids',
			'steps/embedding-groovoids.html'
		),
		new ListboxNav.Item (
			'STEP 11: Embedding Other File Formats (MIDI, WAV, AIFF, AU)',
			'steps/other-file-formats.html'
		),
		new ListboxNav.Item (
			'STEP 12: Dealing with MIME Types',
			'steps/mime-types.html'
		),
		new ListboxNav.Item (
			'STEP 13: Browser Compatibility: EMBED meets OBJECT',
			'steps/embed-meets-object.html'
		),
		new ListboxNav.Item (
			'STEP 14: Browser Compatibility: Using Music Object',
			'steps/using-music-object.html'
		),
		new ListboxNav.Item (
			'STEP 15: Creating Scriptable Instances',
			'steps/scriptable-instances.html'
		)
	)
);

listboxNav.generateNavigation ();
