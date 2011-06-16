new ListboxNav (
	'listboxNav',
	'Music Object Extension - Tracing (version history)',
	'../../../../style/images/',
	22,19,
	false,
	new Array (
		new ListboxNav.Item (
			'Introduction',
			'intro.html'
		),

		new ListboxNav.Item (),

		new ListboxNav.Item (
			'Version 1.0.0',
			'versions/version-1.0.0.html'
		)
	)
);

listboxNav.generateNavigation ();
