new ListboxNav (
	'listboxNav',
	'Randomizing Music On a Web Page',
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
			'STEP 1: The Numbered File Approach',
			'steps/numbered-file.html'
		),
		new ListboxNav.Item (
			'STEP 2: The Select-from-a-list Approach',
			'steps/randomfromlist-function.html'
		),
		new ListboxNav.Item (
			'STEP 3: Sourcing in the randomFromList Method',
			'steps/randomfromlist-sourced-in.html'
		),
		new ListboxNav.Item (
			'STEP 4: Using randomFromList to Play a Random Groovoid',
			'steps/randomfromlist-groovoid.html'
		),
		new ListboxNav.Item (
			'STEP 5: Creating a randomGroovoid Function',
			'steps/randomgroovoid-function.html'
		),
		new ListboxNav.Item (
			'STEP 6: Sourcing in the randomGroovoid Static Method',
			'steps/randomgroovoid-sourced-in.html'
		),
		new ListboxNav.Item (
			'STEP 7: Examples Using the randomGroovoid Static Method',
			'steps/randomgroovoid-examples.html'
		),
		new ListboxNav.Item (
			'STEP 8: Cool File of the Day',
			'steps/file-of-the-day.html'
		)
	)
);

listboxNav.generateNavigation ();
