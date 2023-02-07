/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

/* appearance */
static unsigned int borderpx      = 2;        /* border pixel of windows */
static unsigned int snap          = 32;       /* snap pixel */
static const unsigned int gappih  = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv  = 10;       /* vert inner gap between windows */
static const unsigned int gappoh  = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov  = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps        = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating  = 0;        /* 1 means swallow floating windows by default */
static int showbar                = 1;        /* 0 means no bar */
static int topbar                 = 1;        /* 0 means bottom bar */
static char font[]                = "monospace:size=10";
static char dmenufont[]           = "monospace:size=10";
static const char *fonts[]        = { "monospace:size=10", "NotoColorEmoji:pixelsize=10:antialias=true:autohint=true"  };
static char normbgcolor[]         = "#504945";
static char normbordercolor[]     = "#444444";
static char normfgcolor[]         = "#fbf1c7";
static char selfgcolor[]          = "#fe8019";
static char selbordercolor[]      = "#d65d0e";
static char selbgcolor[]          = "#3c3836";
static char dmfgcolor[]           = "#ffffff";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",                      NULL,     NULL,        0,           1,         0,         0,        -1 },
	{ TERMCLASS,                   NULL,     NULL,        0,           0,         1,         0,        -1 },
	{ "St",                        NULL,     NULL,        0,           0,         1,         0,        -1 },
	{                              NULL,     NULL,     "Event Tester", 0,         0,         0,         1,      -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_v,     ACTION##stack,  {.i = 0 } }, \
	/*
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },
	*/

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }


#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", dmfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "font",               STRING,  &font },
		{ "dmenufont",          STRING,  &dmenufont },
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",               INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",              FLOAT,   &mfact },
};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                    XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                       focus)
	STACKKEYS(MODKEY|ShiftMask,             push)
	{ MODKEY,                    XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                    XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                    XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|Mod4Mask,           XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask, XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask,           XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask, XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,           XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask, XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,           XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask, XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,           XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask, XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,           XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask, XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,           XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask, XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,           XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod4Mask|ShiftMask, XK_0,      defaultgaps,    {0} },
	{ MODKEY,                    XK_Tab,    view,           {0} },
	{ MODKEY,                    XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                    XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                    XK_m,      setlayout,      {.v = &layouts[2]} },
	/* TZ MOD */
	{ MODKEY|ControlMask,        XK_space,  setlayout,      {.v = &layouts[11]} },

	/* TZ MOD */
	{ MODKEY,                    XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,          XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,          XK_f,      togglefullscr,  {0} },
	{ MODKEY,                    XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,          XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                    XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                    XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,          XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,          XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                     XK_1,                      0)
	TAGKEYS(                     XK_2,                      1)
	TAGKEYS(                     XK_3,                      2)
	TAGKEYS(                     XK_4,                      3)
	TAGKEYS(                     XK_5,                      4)
	TAGKEYS(                     XK_6,                      5)
	TAGKEYS(                     XK_7,                      6)
	TAGKEYS(                     XK_8,                      7)
	TAGKEYS(                     XK_9,                      8)
	{ MODKEY|ShiftMask,          XK_BackSpace,  quit,       {0} },

	/* Tom Zurita's Mods */
	{ MODKEY,                    XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                    XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,          XK_Return, togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                    XK_apostrophe,  zoom,       {0} },

	/* Tom Zurita's Mods			 - Volume */
	{ MODKEY,                    XK_minus,       spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,          XK_minus,       spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%-; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                    XK_equal,       spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,          XK_equal,       spawn,		SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%+; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,          XK_m,           spawn,     SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                    XK_BackSpace,   spawn,		{.v = (const char*[]){ "sysact", NULL } } },
	{ MODKEY,                    XK_q,           killclient,        {0} },
	{ MODKEY,                    XK_w,           spawn,		{.v = (const char*[]){ BROWSER, NULL } } },
	{ MODKEY|ShiftMask,          XK_w,           spawn,		{.v = (const char*[]){ TERMINAL, "-e", "sudo", "nmtui", NULL } } },
	{ MODKEY,                    XK_r,           spawn,		{.v = (const char*[]){ TERMINAL, "-e", "lfub", NULL } } },
	{ MODKEY|ShiftMask,          XK_r,           spawn,		{.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
	{ MODKEY,                    XK_s,           togglesticky,      {0} },
	{ MODKEY,                    XK_Page_Up,     shiftview,	{ .i = -1 } },
	{ MODKEY,                    XK_Page_Down,   shiftview,	{ .i = +1 } },
	{ MODKEY,                	 XK_Insert,      spawn,		SHCMD("xdotool type $(grep -v '^#' ~/.local/share/tarbs/snippets | dmenu -i -l 50 | cut -d' ' -f1)") },
	{ MODKEY,                	 XK_F1,          spawn,		SHCMD("groff -mom /usr/local/share/dwm/tarbs.mom -Tpdf | zathura -") },
	{ MODKEY,                	 XK_F2,          spawn,		{.v = (const char*[]){ "tutorialvids", NULL } } },
	{ MODKEY,                	 XK_F3,          spawn,		{.v = (const char*[]){ "displayselect", NULL } } },
	{ MODKEY,                	 XK_F4,          spawn,		SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                	 XK_F6,          spawn,		{.v = (const char*[]){ "torwrap", NULL } } },
	{ MODKEY,                	 XK_F7,          spawn,		{.v = (const char*[]){ "td-toggle", NULL } } },
	{ MODKEY,                	 XK_F8,          spawn,		{.v = (const char*[]){ "mw", "-Y", NULL } } },
	{ MODKEY,                	 XK_F9,          spawn,		{.v = (const char*[]){ "mounter", NULL } } },
	{ MODKEY,                	 XK_F10,         spawn,		{.v = (const char*[]){ "unmounter", NULL } } },
	{ MODKEY,                	 XK_F11,         spawn,		SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
	{ MODKEY,                	 XK_F12,         spawn,		SHCMD("remaps") },
	{ 0,                     	 XK_Print,  	 spawn,		SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,             	 XK_Print,  	 spawn,		{.v = (const char*[]){ "maimpick", NULL } } },
	{ MODKEY,                	 XK_Print,  	 spawn,		{.v = (const char*[]){ "dmenurecord", NULL } } },
	{ MODKEY|ShiftMask,      	 XK_Print,  	 spawn,		{.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
	{ MODKEY,                	 XK_Delete, 	 spawn,		{.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
    { MODKEY,                	 XK_e,      	 spawn,     SHCMD("emacsclient -c -a 'emacs'") },
	{ MODKEY|ControlMask,	 	 XK_b,      	 spawn,     SHCMD("emacsclient -c -a 'emacs' --eval '(ibuffer)'") },
	{ MODKEY|ControlMask,	 	 XK_d,      	 spawn,     SHCMD("emacsclient -c -a 'emacs' --eval '(dired nil)'") },
	{ MODKEY|ControlMask,	 	 XK_i,      	 spawn,     SHCMD("emacsclient -c -a 'emacs' --eval '(erc)'") },
	{ MODKEY|ControlMask,	 	 XK_n,      	 spawn,     SHCMD("emacsclient -c -a 'emacs' --eval '(elfeed)'") },
	{ MODKEY|ControlMask,	 	 XK_s,      	 spawn,     SHCMD("emacsclient -c -a 'emacs' --eval '(eshell)'") },
	{ MODKEY|ControlMask,	 	 XK_v,      	 spawn,     SHCMD("emacsclient -c -a 'emacs' --eval '(+vterm/here nil)'") },
	{ MODKEY|ControlMask,	 	 XK_w,      	 spawn,     SHCMD("emacsclient -c -a 'emacs' --eval '(doom/window-maximize-buffer(eww \"distro.tube\"))'") },

	{ MODKEY,                    XK_Scroll_Lock, spawn,    SHCMD("killall screenkey || screenkey &") },
	{ 0, XF86XK_AudioMute,		   spawn,                  SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,  spawn,                  SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,  spawn,                  SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,		   spawn,                  {.v = (const char*[]){ "mpc", "prev", NULL } } },
	{ 0, XF86XK_AudioNext,		   spawn,                  {.v = (const char*[]){ "mpc",  "next", NULL } } },
	{ 0, XF86XK_AudioPause,		   spawn,                  {.v = (const char*[]){ "mpc", "pause", NULL } } },
	{ 0, XF86XK_AudioPlay,		   spawn,                  {.v = (const char*[]){ "mpc", "play", NULL } } },
	{ 0, XF86XK_AudioStop,		   spawn,                  {.v = (const char*[]){ "mpc", "stop", NULL } } },
	{ 0, XF86XK_AudioRewind,	   spawn,                  {.v = (const char*[]){ "mpc", "seek", "-10", NULL } } },
	{ 0, XF86XK_AudioForward,	   spawn,                  {.v = (const char*[]){ "mpc", "seek", "+10", NULL } } },
	{ 0, XF86XK_AudioMedia,		   spawn,                  {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
	{ 0, XF86XK_AudioMicMute,	   spawn,                  SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	{ 0, XF86XK_PowerOff,          spawn,                  {.v = (const char*[]){ "sysact", NULL } } },
	{ 0, XF86XK_Calculator,		   spawn,                  {.v = (const char*[]){ TERMINAL, "-e", "bc", "-l", NULL } } },
	{ 0, XF86XK_Sleep,             spawn,                  {.v = (const char*[]){ "sudo", "-A", "zzz", NULL } } },
	{ 0, XF86XK_WWW,               spawn,                  {.v = (const char*[]){ BROWSER, NULL } } },
	{ 0, XF86XK_DOS,               spawn,                  {.v = termcmd } },
	{ 0, XF86XK_ScreenSaver,	   spawn,                  SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") }, { 0, XF86XK_TaskPane,		   spawn,                  {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } }, { 0, XF86XK_Mail,              spawn,                  SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0, XF86XK_MyComputer,		   spawn,                  {.v = (const char*[]){ TERMINAL, "-e",  "lfub",  "/", NULL } } },
	/* { 0, XF86XK_Battery,        spawn,                  SHCMD("") }, */
	{ 0, XF86XK_Launch1,		   spawn,                  {.v = (const char*[]){ "xset", "dpms", "force", "off", NULL } } },
	{ 0, XF86XK_TouchpadToggle,	   spawn,                  SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOff,	   spawn,                  {.v = (const char*[]){ "synclient", "TouchpadOff=1", NULL } } },
	{ 0, XF86XK_TouchpadOn,		   spawn,                  {.v = (const char*[]){ "synclient", "TouchpadOff=0", NULL } } },
	{ 0, XF86XK_MonBrightnessUp,   spawn,                  {.v = (const char*[]){ "xbacklight", "-inc", "15", NULL } } },
	{ 0, XF86XK_MonBrightnessDown, spawn,                  {.v = (const char*[]){ "xbacklight", "-dec", "15", NULL } } },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	/* Tom Z Mod */
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkRootWin,           0,              Button2,        togglebar,      {0} },
};
