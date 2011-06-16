new ListboxNav (
	'listboxNav',
	'Sonifying a Navigation Bar',
	'../../style/images/',
	22,19,
	false,
	new Array (
		new ListboxNav.Item (
			'STEP 1: Setting Up The Basic Navbar HTML',
			'steps/setting-up-the-html.html'
		),
		new ListboxNav.Item (
			'STEP 2: Adding the Framework For Sonification',
			'steps/sonification-framework.html'
		),
		new ListboxNav.Item (
			'STEP 3: Adding the Mouseover Sounds',
			'steps/adding-mouseover-sounds.html'
		),
		new ListboxNav.Item (
			'STEP 4: Adding the Click Sounds',
			'steps/adding-click-sounds.html'
		),
		new ListboxNav.Item (
			'STEP 5: Notes That Sustain Only During Mouseover',
			'steps/mouseover-sustained-notes.html'
		),
		new ListboxNav.Item (
			'STEP 6: Do It With Groovoids',
			'steps/do-it-with-groovoids.html'
		),
		new ListboxNav.Item (
			'STEP 7: Combining Groovoids With Note Playing',
			'steps/groovoids-and-notes.html'
		),
		new ListboxNav.Item (
			'STEP 8: Some Sneaky Scripting',
			'steps/some-sneaky-scripting.html'
		)
	)
);

listboxNav.generateNavigation ();
