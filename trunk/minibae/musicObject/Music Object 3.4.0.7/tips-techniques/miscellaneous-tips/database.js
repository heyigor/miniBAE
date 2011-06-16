new ListboxNav (
	'listboxNav',
	'Miscellaneous Tips',
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
			'GENERAL TIP: Viewing Source Code',
			'tips/viewing-source-code.html'
		),
		new ListboxNav.Item (
			'JAVASCRIPT TIP: Daisy-chained Assignments',
			'tips/daisy-chained-assignments.html'
		),
		new ListboxNav.Item (
			'JAVASCRIPT TIP: Shorthand Conditions',
			'tips/shorthand-conditions.html'
		)
	)
);

listboxNav.generateNavigation ();
