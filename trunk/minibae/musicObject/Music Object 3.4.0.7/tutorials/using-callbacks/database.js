new ListboxNav (
	'listboxNav',
	'Using Player Callbacks',
	'../../style/images/',
	22,19,
	false,
	new Array (
		new ListboxNav.Item (
			'Introduction',
			'intro.html'
		),

		new ListboxNav.Item (),

		new ListboxNav.Item (
			'PART A: Cross-connecting a Player and HTML Form Elements',
			'cross-connecting/intro.html'
		),
		new ListboxNav.Item (
			'PART A - step 1: Setting up the HTML',
			'cross-connecting/setting-up-the-html.html'
		),
		new ListboxNav.Item (
			'PART A - step 2: Wiring the Connections',
			'cross-connecting/wiring-the-connections.html'
		),

		new ListboxNav.Item (),

		new ListboxNav.Item (
			'PART B: Mutually Exclusive Players',
			'mutually-exclusive-players/intro.html'
		),
		new ListboxNav.Item (
			'PART B - step 1: Setting up the HTML',
			'mutually-exclusive-players/setting-up-the-html.html'
		),
		new ListboxNav.Item (
			'PART B - step 2: Wiring the Connections',
			'mutually-exclusive-players/wiring-the-connections.html'
		)
	)
);

listboxNav.generateNavigation ();
