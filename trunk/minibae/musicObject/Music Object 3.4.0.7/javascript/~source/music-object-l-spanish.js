/**********
     TITLE: Music Object Language Module - Spanish
   VERSION: 1.1.0
    AUTHOR: Richard Court & Pedro Drummond
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_l5"*/

function MO_upgradePrompt () {
	Music.promptUser ('Por Favor Actualice el Beatnik','Esta p&aacute;gina est&aacute; optimizada para los componentes de la versi&oacute;n <B>' + Music.requiredMinVersion + ' (o superior)</B> del Beatnik Player. La versi&oacute;n actualmente instalada en su browser es ' + Music.playerVersion + '.<P>Si usted prefiere IGNORAR este mensaje, la p&aacute;gina bajar&aacute; normalmente, pero puede dejar de funcionar de acuerdo con la intenci&oacute;n del autor.','ACTUALIZAR EL BEATNIK >>>','Music.installPlayer ()','IGNORAR','Music.promptClose ()');
}

function MO_installPrompt () {
	Music.promptUser ('Contenido Realzado com Beatnik !!','Esta p&aacute;gina est&aacute; optimizada para los componentes de audio interactivo del <B>Beatnik Player</B>. Todo indica que usted no tiene el Beatnik Player instalado.<P>Si usted prefiere IGNORAR este mensaje, la p&aacute;gina bajar&aacute; normalmente, pero sin el &aacute;udio Beatnik.','INSTALAR EL BEATNIK >>>','Music.installPlayer ()','IGNORAR','Music.promptClose ()');
}

function MO_enableJavaPrompt () {
	Music.promptUser ('Por Favor Habilite el Java','Esta p&aacute;gina utiliza los componentes de &aacute;udio interactivo do Beatnik Player. En este momento, la opci&oacute;n Java est&aacute; deshabilitada en su browser. Para que esta p&aacute;gina funcione correctamente con el Beatnik, <B>es preciso rehabilitar el Java</B>.<P>Esta p&aacute;gina bajar&aacute; normalmente, aunque le podr&aacute; faltar alg&uacute;n elemento de interactividad del &aacute;udio.','    OK ','Music.promptClose ()');
}

Music.upgradePrompt = MO_upgradePrompt;
Music.installPrompt = MO_installPrompt;
Music.enableJavaPrompt = MO_enableJavaPrompt;
Music.upgradePlayerText = 'Actualice Beatnik';
Music.getPlayerText = 'Tome Beatnik';
Music.installerOptions.language = 'spanish';
