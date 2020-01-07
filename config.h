/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx		= 1;		/* border pixel of windows */
static const unsigned int snap			= 8;		/* snap pixel */
static const unsigned int systraypinning        = 0;		/* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing        = 1;		/* systray spacing */
static const int systraypinningfailfirst        = 1;		/* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray                    = 1;		/* 0 means no systray */
static const int showbar			= 1;		/* 0 means no bar */
static const int topbar				= 1;		/* 0 means bottom bar */
static const char *fonts[]			= { "Ubuntu Mono:size=8" };
static const char dmenufont[]			= "Ubuntu Condensed:size=8";
static const char col_01[]			= "#1c1f22";	/* Color de la barra de estado en reposo*/
static const char col_02[]			= "#1c1f22";	/* Color del borde de pantalla desenfocada */
static const char col_03[]			= "#ffffff";	/* Color de las letras de la barra de estado */
static const char col_04[]			= "#02ff00";	/* Color del nombre y etiqueta que está en uso */
static const char col_05[]			= "#ffffff";	/* Color del borde de la ventana enfocada */
static const char col_06[]			= "#1c1f22";	/* Color barra de estado cuando está en uso */
static const char *colors[][3]      = {
         /*               fg             bg              border   */
         [SchemeNorm] =  { col_03,       col_01,         col_02 },
         [SchemeSel]  =  { col_04,       col_06,         col_05 },
};

/* tagging */
static const char *tags[] = { "1:", "2:", "3:", "4:", "5:" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class		instance	title			tags mask	isfloating	monitor */
        { "Gimp",               NULL,           NULL,                   1 << 3,         0,              -1 },
        { "Firefox",		NULL,           NULL,                   1 << 1,         0,              -1 },
        { "Surf",               NULL,           NULL,                   1 << 1,         0,              -1 },
        { "Galculator",         NULL,           NULL,                   0,              1,              -1 },
        { "Openshot",           NULL,           NULL,                   1 << 3,         0,              -1 },
        { "Telegram",           NULL,           NULL,                   1 << 2,         0,              -1 },
        { "Lxappearance",       NULL,           NULL,                   0,              1,              -1 },
        { "Thunderbird",        NULL,           NULL,                   1 << 2,         0,              -1 },
        { "Thunderbird",        NULL,           "Filtros de mensajes",  1 << 2,         1,              -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "T",      tile },    /* first entry is default */
	{ "F",      NULL },    /* no layout function means floating behavior */
	{ "M",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_01, "-nf", col_03, "-sb", col_06, "-sf", col_04, NULL };
static const char *termcmd[]  = { "st", "-t", "Enter the Void", NULL };

/* Comandos personalizados */
static const char       *menucmd[]		= { "menu-apagar", NULL };
static const char       *screenshotcmd[]	= { "screenshot", NULL };
static const char       *firefoxcmd[]		= { "firefox", NULL };
static const char       *telegramcmd[]		= { "Telegram", NULL };
static const char       *gimpcmd[]		= { "gimp", NULL };
static const char       *openshotcmd[]		= { "openshot", NULL };
static const char       *musiccmd[]		= { "st", "-e", "ncmpcpp", NULL };
static const char       scratchpadname[]	= "scratchpad";
static const char       *scratchpadcmd[]	= { "st", "-t", "scratchpad", "-g", "140x40+400+220", NULL };

/*      Control de volumen ALSA         */
//static const char       *mutecmd[]      = { "amixer", "-q", "set", "Master", "toggle", NULL };
//static const char       *volupcmd[]     = { "amixer", "-q", "set", "Master", "2%+", "unmute", NULL };
//static const char       *voldowncmd[]   = { "amixer", "-q", "set", "Master", "2%-", "unmute", NULL };

/*      Control de Voumen PulseAudio    */
static const char	*mutecmd[]	= { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char	*volupcmd[]	= { "pactl", "set-sink-volume", "0", "+2%", NULL };
static const char	*voldowncmd[]	= { "pactl", "set-sink-volume", "0", "-2%", NULL };

/*      Control del brillo de pantalla  */
//static const char *brightnessup[] = { "xbacklight", "-inc", "1.5"};
//static const char *brightnessdown[] = { "xbacklight", "-dec", "1.5"};
static const char       *brightnessup[]         = { "light", "-A", "0.5"};
static const char       *brightnessdown[]       = { "light", "-U", "0.5"};

static Key keys[] = {
	/* modifier                     key                             function        argument */
	{ 0,				XK_F12,				togglescratch,	{.v = scratchpadcmd} },
	{ MODKEY,                       XK_d,				spawn,          {.v = dmenucmd } },
	{ MODKEY,			XK_Return,			spawn,		{.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

/* Atajos personalizados */
        { MODKEY,                       XK_F2,                          spawn,          {.v = firefoxcmd } },
        { ControlMask,                  XK_F2,                          spawn,          SHCMD("tabbed -cr 2 surf -pe x -z 0.75 duck.com") },
        { MODKEY,                       XK_F3,                          spawn,          {.v = telegramcmd } },
        { MODKEY,                       XK_F4,                          spawn,          {.v = gimpcmd } },
        { MODKEY,                       XK_F5,                          spawn,          {.v = openshotcmd } },
        { MODKEY,                       XK_x,                           spawn,          {.v = menucmd } },		// Atajo menú Apagar
        { MODKEY,                       XK_c,                           spawn,          {.v = screenshotcmd } },        // Captura de pantalla de área seleccionada
        { MODKEY,                       XK_F10,                         spawn,          {.v = musiccmd } },

/*      Modos de captura de pantalla    */
        { 0,                            XK_Print,                       spawn,          SHCMD("scrot /tmp/'%F_%T.png' -e 'xclip -selection c -t image/png < $f'") }, // Captura de pantalla en el portapapeles
        { ControlMask,                  XK_Print,                       spawn,          SHCMD("scrot -q 100 '%F_%H%M%S_$wx$h.png' -e 'mv $f /home/skynet/Datos/Capturas/'; sleep 1; exec notify-send -t 5000 --icon=/home/skynet/.icons/screenshot.png 'Captura de pantalla' 'guardada en: ~/Datos/Capturas'") },
        { MODKEY|ShiftMask,             XK_Print,                       spawn,          SHCMD("scrot -u -q 100 'Focus_%F_%H%M%S_$wx$h.png' -e 'mv $f /home/skynet/Datos/Capturas/focus'; sleep 1; exec notify-send -t 5000 --icon=/home/skynet/.icons/window.png 'Ventana enfocada' 'guardada en: ~/Datos/Capturas/focus'") },
					
/*      Atajos multimedia       */
        { 0,                            XF86XK_AudioPlay,               spawn,          SHCMD("mpc toggle") },
        { 0,                            XF86XK_AudioPrev,               spawn,          SHCMD("mpc prev") },
        { 0,                            XF86XK_AudioNext,               spawn,          SHCMD("mpc next") },
        { 0,                            XF86XK_AudioStop,               spawn,          SHCMD("mpc stop") },
        { 0,                            XF86XK_AudioMute,               spawn,          {.v = mutecmd } },
        { 0,                            XF86XK_AudioLowerVolume,        spawn,          {.v = voldowncmd } },
        { 0,                            XF86XK_AudioRaiseVolume,        spawn,          {.v = volupcmd } },

/*      Ajustar brillo de la pantalla   */
        { 0,                            XF86XK_MonBrightnessUp,         spawn,          {.v = brightnessup} },
        { 0,                            XF86XK_MonBrightnessDown,       spawn,          {.v = brightnessdown} },

/* ---------------------------------------------------------------------------------------------- */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

