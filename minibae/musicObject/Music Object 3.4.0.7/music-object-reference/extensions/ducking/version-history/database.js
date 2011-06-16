new ListboxNav (
	'listboxNav',
	'Music Object Extension - Ducking (version history)',
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
			'Version 1.3.0',
			'versions/version-1.3.0.html'
		),
		new ListboxNav.Item (
			'Version 1.2.0',
			'versions/version-1.2.0.html'
		)
	)
);

listboxNav.generateNavigation ();
