new ListboxNav (
	'listboxNav',
	'Sonifying a Form',
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
			'STEP 1: Setting up the HTML',
			'steps/setting-up-the-html.html'
		),
		new ListboxNav.Item (
			'STEP 2: Adding the Framework For Sonification',
			'steps/sonification-framework.html'
		),
		new ListboxNav.Item (
			'STEP 3: Adding the Checkbox Sounds',
			'steps/checkbox-sounds.html'
		),
		new ListboxNav.Item (
			'STEP 4: Adding the Radio Button Sounds',
			'steps/radio-button-sounds.html'
		),
		new ListboxNav.Item (
			'STEP 5: Adding the Listbox Sounds',
			'steps/listbox-sounds.html'
		),
		new ListboxNav.Item (
			'STEP 6: Adding the Textbox Sounds',
			'steps/textbox-sounds.html'
		),
		new ListboxNav.Item (
			'STEP 7: Adding the Submit and Reset Button Sounds',
			'steps/button-sounds.html'
		),
		/*
		new ListboxNav.Item (
			'STEP : Adding a Validation Alert Sound',
			'steps/validation-alert-sound.html'
		),
		*/
		new ListboxNav.Item (
			'STEP 8: A Touch of Soothing Music to Round it Off',
			'steps/soothing-music.html'
		),

		new ListboxNav.Item (),

		new ListboxNav.Item (
			'-- ADVANCED STEPS --',
			'steps/advanced-steps.html'
		),
		new ListboxNav.Item (
			'STEP 9: Using the sonifyFormsWithHandlers Method',
			'steps/wizards-handlers.html'
		),
		new ListboxNav.Item (
			'STEP 10: Using the sonifyFormsWithGroovoids Method',
			'steps/wizards-groovoids.html'
		),
		new ListboxNav.Item (
			'STEP 11: Using sonifyFormsWithHandlers to Play Musical Notes',
			'steps/wizards-handlers-notes.html'
		),
		new ListboxNav.Item (
			'STEP 12: The Eerie Halloween Form',
			'steps/wizards-halloween.html'
		)
	)
);

listboxNav.generateNavigation ();
