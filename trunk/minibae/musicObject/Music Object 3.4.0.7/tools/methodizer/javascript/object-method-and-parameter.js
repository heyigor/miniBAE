/**********
     TITLE: Method & Parameter Objects
   VERSION: 1.0.0
      TYPE: Object Definitions
    AUTHOR: Chris van Rensburg
 COPYRIGHT: 1996-1999 Beatnik, Inc. All Rights Reserved
**********/

function Method (_name,variations,defaultVariation,previewable) {
	this.name = _name;
	this.variations = (typeof variations != 'undefined') ? variations : '';
	this.defaultVariation = (typeof defaultVariation != 'undefined') ? defaultVariation : null;
	this.previewable = (typeof previewable != 'undefined') ? previewable : true;
	Method.instances [_name] = this;
}

Method.instances = new Array ();

function Parameter (_name,_type,_defaultSetting,_options,hasHelp) {
	/*** Constructor Variables ***/
	this.name = _name;
	this.type = _type;
	this.defaultSetting = _defaultSetting;
	this.hasHelp = hasHelp;

	/*** Instance State Variables ***/
	Parameter.current = null;
	if (_type == 'boolean') {
		this.options = new Array ('true','false');
	} else if (typeof _options != 'undefined' && _options != '') {
		if (_options.indexOf (',') != -1) {
			this.options = _options.split (',');
		} else if (_options.indexOf (':') != -1) {
			var minMax = _options.split (':');
			this.minValue = minMax [0];
			this.maxValue = minMax [1];
		} else {
			this.options = new Array ();
			this.options [0] = _options;
		}
	}

	/*** Exposed Methods ***/

	/*** Initialisation ***/
	Parameter.instances [_name] = this;
	return this;
}

Parameter.instances = new Array ();
