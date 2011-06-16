/**********
     TITLE: ListboxNav Object - Page Arrived Reporter
   VERSION: 1.3.1
      TYPE: Object Support Code
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
**********/

function ListboxNav_pageArrived () {
	if (
		typeof parent.frames [0] != 'undefined' &&
		typeof parent.frames [0].listboxNav != 'undefined'
	) {
		parent.frames [0].listboxNav.pageArrived ();
	} else {
		setTimeout ('ListboxNav_pageArrived ()',200);
	}
}

ListboxNav_pageArrived ();
