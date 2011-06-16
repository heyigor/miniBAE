new ListboxNav (
	'listboxNav',
	'Authoring Issues',
	'../../style/images/',
	22,19,
	false,
	new Array (
		new ListboxNav.Item (
			'Introduction',
			'issues/intro.html'
		),

		new ListboxNav.Item (),

		new ListboxNav.Item (
			'Issues for Internet Explorer on the MacOS',
			'issues/msie-mac.html'
		),
		new ListboxNav.Item (
			'Netscape 6 and Automatic Sonification',
			'issues/netscape-6-bug.html'
		),
		new ListboxNav.Item (
			'Systems with Incompatible Sound Hardware',
			'issues/incompatible-hardware.html'
		),
		new ListboxNav.Item (
			'Opening Sonified Windows in IE',
			'issues/opening-windows-in-ie.html'
		),
		new ListboxNav.Item (
			'Embedding Hidden Instances in Table Cells',
			'issues/hidden-in-table-cells.html'
		),
		new ListboxNav.Item (
			'Clicking on links before media has finished downloading',
			'issues/premature-click.html'
		),
		new ListboxNav.Item (
			'Setting window.status and Playing Groovoids or Music Files',
			'issues/window-status.html'
		),
		new ListboxNav.Item (
			'\'SCRIPT SRC=\' and file server problems',
			'issues/script-src.html'
		),
		new ListboxNav.Item (
			'Using EMBED in multiple frames',
			'issues/embed-in-frames.html'
		),
		new ListboxNav.Item (
			'Using EMBED in tables',
			'issues/embed-in-tables.html'
		),
		new ListboxNav.Item (
			'Beatnik ActiveX Control Persisting',
			'issues/activex-persisting.html'
		)
	)
);

listboxNav.generateNavigation ();
