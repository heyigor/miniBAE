new ListboxNav (
	'listboxNav',
	'Sample Code & Examples',
	'../style/images/',
	22,19,
	false,
	new Array (
		new ListboxNav.Item (
			'Music List',
			'music-list/index.html'
		),
		new ListboxNav.Item (
			'Playing Notes',
			'playing-notes/index.html'
		),
		new ListboxNav.Item (
			'Custom Control Panel',
			'control-panel/index.html'
		),
		new ListboxNav.Item (
			'The Musical Dice',
			'musical-dice/index.html'
		),
		new ListboxNav.Item (
			'Hyperactivity',
			'hyperactive/index.html'
		),
		new ListboxNav.Item (
			'The Mixer',
			'mixer/index.html'
		),
		new ListboxNav.Item (
			'A JavaScript Seashore',
			'seashore/index.html'
		),
		new ListboxNav.Item (
			'Realtime Stocks',
			'realtime-stocks/index.html'
		)
	)
);

listboxNav.generateNavigation ();
