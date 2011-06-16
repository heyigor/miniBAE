/**********
     TITLE: Music Object Language Module - Portuguese
   VERSION: 1.1.0
    AUTHOR: Richard Court
 COPYRIGHT: 1996-2001 Beatnik, Inc. All Rights Reserved
  REQUIRES: music-object.js (3.3.0 or higher)
**********/

/*ScruncherSettings Mappings=",MO=MO_l4"*/

function MO_upgradePrompt () {
	Music.promptUser ('Favor Atualizar Beatnik','Esta p&aacute;gina est&aacute; otimizada para os componentes da vers&atilde;o <B>' + Music.requiredMinVersion + ' (ou superior)</B> do Beatnik Player. A vers&atilde;o atualmente instalada no seu browser &eacute; ' + Music.playerVersion + '.<P>Se voc&ecirc; prefere IGNORAR esta mensagem, a p&aacute;gina carregar&aacute; normalmente, mas pode deixar de funcionar de acordo com as inten&ccedil;&otilde;es do autor.','ATUALIZAR BEATNIK >>>','Music.installPlayer ()','IGNORAR','Music.promptClose ()');
}

function MO_installPrompt () {
	Music.promptUser ('Conte&uacute;do Real&ccedil;ado com Beatnik !!','Esta p&aacute;gina est&aacute; otimizada para os componentes de &aacute;udio interativo do <B>Beatnik Player</B>. Tudo indica que voc&ecirc; n&atilde;o tem o Beatnik Player instalado.<P>Se voc&ecirc; prefere IGNORAR esta mensagem, a p&aacute;gina carregar&aacute; normalmente, mas sem o Beatnik &aacute;udio.','INSTALAR BEATNIK >>>','Music.installPlayer ()','IGNORAR','Music.promptClose ()');
}

function MO_enableJavaPrompt () {
	Music.promptUser ('Favor Habilitar Java','Esta p&aacute;gina utiliza os componentes de &aacute;udio interativo do Beatnik Player. No momento, a op&ccedil;&atilde;o Java est&aacute; deshabilitada no seu browser. Para que esta p&aacute;gina funcione corretamente com Beatnik, <B>&eacute; preciso rehabilitar Java</B>.<P>Esta p&aacute;gina carregar &aacute; normalmente, embora poder&aacute; faltar algum elemento de interatividade de &aacute;udio.','    OK    ','Music.promptClose ()');
}

Music.upgradePrompt = MO_upgradePrompt;
Music.installPrompt = MO_installPrompt;
Music.enableJavaPrompt = MO_enableJavaPrompt;
Music.upgradePlayerText = 'Atualize Beatnik';
Music.getPlayerText = 'Pegue Beatnik';
Music.installerOptions.language = 'portuguese';
