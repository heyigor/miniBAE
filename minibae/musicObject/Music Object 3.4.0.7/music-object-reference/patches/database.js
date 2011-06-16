new ListboxNav (
	'listboxNav',
	'Music Object Patches',
	'../../style/images/',
	22,19,
	false,
	new Array (
		new ListboxNav.Item (
			'About Music Object Patches',
			'intro.html'
		),

		new ListboxNav.Item (),

		new ListboxNav.Item (
			'Music Object Patch - Pre 3.3.0',
			'patches/pre-3.3.0.html'
		),
		new ListboxNav.Item (
			'Music Object Patch - Pre 3.2.0',
			'patches/pre-3.2.0.html'
		),
		new ListboxNav.Item (
			'Music Object Patch - Pre 3.1.0',
			'patches/pre-3.1.0.html'
		)
	)
);

listboxNav.generateNavigation ();
