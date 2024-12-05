/*
 *   _____ _____
 *  |_   _|__  /   Tom Zurita
 *    | |   / /    http://tomzurita.com
 *    | |  / /_    tzurita@gmail.com
 *    |_| /____|
 *
 */

/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "alacritty"
#define TERMCLASS "Alacritty"
#define BROWSER "firefox"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char dmenufont[]       = "monospace:size=10";
static const char *fonts[]        = { "monospace:size=10", "NotoColorEmoji:pixelsize=10:antialias=true:autohint=true"  };
/* Doom One Theme */
static const char col_gray1[]       = "#2E3440"; // Background color (dark grayish-blue)
static const char col_gray2[]       = "#4C566A"; // Inactive window border color (slightly lighter gray)
static const char col_gray3[]       = "#D8DEE9"; // Foreground color (light gray for text)
static const char col_gray4[]       = "#ECEFF4"; // Active foreground color (white for selected text)
static const char col_cyan[]        = "#81A1C1"; // Selected border color (Nord blue)
static const char col_blue[]        = "#3B4252"; // Selected background color (grayish-blue)
static const char col_dmfg[]        = "#88C0D0"; // DM foreground color (Nord aqua/cyan)

static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_blue,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "Alacritty",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|M|",      centeredmaster },
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "[D]",      deck },
	{ "[]=",      tile },    /* first entry is default */
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

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

#include "shift-tools.c"
#include <X11/XF86keysym.h>
#include "movestack.c"

static const Key keys[] = {
	/* modifier                     key        function        argument */
	/* Main */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_v,      focusstack,     {.i = 0 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
        { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
        { MODKEY,                       XK_f,      togglefullscr,  {0} },
	/* Layouts */
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_z,      incrgaps,       {.i = +3 } },
	{ MODKEY,                       XK_x,      incrgaps,       {.i = -3 } },
	{ MODKEY,                       XK_a,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_a,      defaultgaps,    {0} },
//	{ MODKEY|Mod1Mask,              XK_z,      incrgaps,       {.i = +1 } },
//	{ MODKEY|Mod1Mask|ShiftMask,    XK_x,      incrgaps,       {.i = -1 } },
//	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
//	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
//	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
//	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
//	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
//	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
//	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
//	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
//	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
//	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
//	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
//	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
//	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
//	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_i,      setlayout,      {.v = &layouts[0]} }, /* centeredmaster */
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} }, /* monocle */
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[2]} }, /* bstack */
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[3]} }, /* bstackhorizontal */
	{ MODKEY|ShiftMask,             XK_d,      setlayout,      {.v = &layouts[4]} }, /* deck */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[5]} }, /* tile */
	{ MODKEY|ShiftMask,             XK_i,      setlayout,      {.v = &layouts[6]} }, /* centeredfloatingmaster */
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	/* Tags/Workspaces */
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
        { MODKEY,                       XK_Left,   focusmon,       {.i = -1 } },
        { MODKEY|ShiftMask,             XK_Left,   tagmon,         {.i = -1 } },
        { MODKEY,                       XK_Right,  focusmon,       {.i = +1 } },
        { MODKEY|ShiftMask,             XK_Right,  tagmon,         {.i = +1 } },
        { MODKEY,                       XK_g,           shiftview, { .i = -1 } },
        { MODKEY,                       XK_semicolon,   shiftview, { .i = 1 } },
        { MODKEY,                       XK_Page_Up,     shiftview, { .i = -1 } },
        { MODKEY,                       XK_Page_Down,   shiftview, { .i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	/* Basic Prorams */
	{ MODKEY,                       XK_r,      spawn,          {.v = (const char*[]){ TERMINAL, "-e", "lfub", NULL } } },
	{ MODKEY|ShiftMask,             XK_r,      spawn,          {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
	{ MODKEY,                       XK_w,      spawn,          {.v = (const char*[]){ BROWSER, NULL } } },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = (const char*[]){ TERMINAL, "-e", "sudo", "nmtui", NULL } } },
        { MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD(TERMINAL " -e newsboat ; pkill -RTMIN+6 dwmblocks") },
        { MODKEY,                       XK_p,      spawn,          {.v = (const char*[]){ "passmenu", NULL } } },
        { MODKEY,                       XK_e,      spawn,     	   SHCMD("emacsclient -c -a 'emacs'") },
        { MODKEY|ControlMask,           XK_b,      spawn,          SHCMD("emacsclient -c -a 'emacs' --eval '(ibuffer)'") },
        { MODKEY|ControlMask,           XK_d,      spawn,          SHCMD("emacsclient -c -a 'emacs' --eval '(dired nil)'") },
        { MODKEY|ControlMask,           XK_i,      spawn,          SHCMD("emacsclient -c -a 'emacs' --eval '(erc)'") },
        { MODKEY|ControlMask,           XK_n,      spawn,          SHCMD("emacsclient -c -a 'emacs' --eval '(elfeed)'") },
        { MODKEY|ControlMask,           XK_s,      spawn,          SHCMD("emacsclient -c -a 'emacs' --eval '(eshell)'") },
        { MODKEY|ControlMask,           XK_v,      spawn,          SHCMD("emacsclient -c -a 'emacs' --eval '(+vterm/here nil)'") },
        { MODKEY|ControlMask,           XK_w,      spawn,          SHCMD("emacsclient -c -a 'emacs' --eval '(doom/window-maximize-buffer(eww \"distro.tube\"))'") },
	/* System */
	{ MODKEY,                       XK_BackSpace,   spawn,     {.v = (const char*[]){ "sysact", NULL } } },
	{ MODKEY,                       XK_F1,     spawn,          SHCMD("groff -mom /usr/local/share/dwm/tarbs.mom -Tpdf | zathura -") },
	{ MODKEY,                       XK_F2,     spawn,          {.v = (const char*[]){ "tutorialvids", NULL } } },
	{ MODKEY,                       XK_F3,     spawn,          {.v = (const char*[]){ "displayselect", NULL } } },
	{ MODKEY,                       XK_F6,     spawn,          {.v = (const char*[]){ "torwrap", NULL } } },
	{ MODKEY,                       XK_F7,     spawn,          {.v = (const char*[]){ "td-toggle", NULL } } },
	{ MODKEY,                       XK_F8,     spawn,          {.v = (const char*[]){ "mw", "-Y", NULL } } },
	{ MODKEY,                       XK_F9,     spawn,          {.v = (const char*[]){ "mounter", NULL } } },
	{ MODKEY,                       XK_F10,    spawn,          {.v = (const char*[]){ "unmounter", NULL } } },
	{ MODKEY,                       XK_F11,    spawn,          SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
	{ MODKEY,                       XK_F12,    spawn,          SHCMD("remaps") },
	{ MODKEY,                       XK_grave,  spawn,          {.v = (const char*[]){ "dmenuunicode", NULL } } },
	/* Audio */
	{ MODKEY,                       XK_minus,  spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_minus,  spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%-; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_equal,  spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_equal,  spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%+; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") },
	/* Recording */
	{ 0,                            XK_Print,  spawn,          SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,                    XK_Print,  spawn,          {.v = (const char*[]){ "maimpick", NULL } } },
	{ MODKEY,                       XK_Print,  spawn,          {.v = (const char*[]){ "dmenurecord", NULL } } },
	{ MODKEY,                       XK_Delete, spawn,          {.v = (const char*[]){ "dmenurecord", "kill", NULL } } },

	/* XF86 Keys */
	{ 0,            XF86XK_AudioLowerVolume,   spawn,          SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof dwmblocks)") },
        { 0,            XF86XK_AudioMute,          spawn,          SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
        { 0,            XF86XK_AudioRaiseVolume,   spawn,     	   SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%+; kill -44 $(pidof dwmblocks)") },
        { 0,            XF86XK_AudioLowerVolume,   spawn,     	   SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 3%-; kill -44 $(pidof dwmblocks)") },
        { 0,            XF86XK_AudioPrev,          spawn,          {.v = (const char*[]){ "mpc", "prev", NULL } } },
        { 0,            XF86XK_AudioNext,          spawn,          {.v = (const char*[]){ "mpc",  "next", NULL } } },
        { 0,            XF86XK_AudioPause,         spawn,          {.v = (const char*[]){ "mpc", "pause", NULL } } },
        { 0,            XF86XK_AudioPlay,          spawn,          {.v = (const char*[]){ "mpc", "play", NULL } } },
        { 0,            XF86XK_AudioStop,          spawn,          {.v = (const char*[]){ "mpc", "stop", NULL } } },
        { 0,            XF86XK_AudioRewind,        spawn,          {.v = (const char*[]){ "mpc", "seek", "-10", NULL } } },
        { 0,            XF86XK_AudioForward,       spawn,          {.v = (const char*[]){ "mpc", "seek", "+10", NULL } } },
        { 0,            XF86XK_AudioMedia,         spawn,          {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
        { 0,            XF86XK_AudioMicMute,       spawn,          SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
        { 0,            XF86XK_PowerOff,           spawn,          {.v = (const char*[]){ "sysact", NULL } } },
        { 0,            XF86XK_Calculator,         spawn,          {.v = (const char*[]){ TERMINAL, "-e", "bc", "-l", NULL } } },
        { 0,            XF86XK_Sleep,              spawn,          {.v = (const char*[]){ "sudo", "-A", "zzz", NULL } } },
        { 0,            XF86XK_WWW,                spawn,          {.v = (const char*[]){ BROWSER, NULL } } },
        { 0,            XF86XK_DOS,                spawn,          {.v = termcmd } },
        { 0,            XF86XK_ScreenSaver,        spawn,          SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
        { 0,            XF86XK_TaskPane,           spawn,          {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
        { 0,            XF86XK_Mail,               spawn,          SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
        { 0,            XF86XK_MyComputer,         spawn,          {.v = (const char*[]){ TERMINAL, "-e",  "lfub",  "/", NULL } } },
        /* { 0, XF86XK_Battery,        				 spawn,     	 SHCMD("") }, */
        { 0,            XF86XK_Launch1,            spawn,          {.v = (const char*[]){ "xset", "dpms", "force", "off", NULL } } },
        { 0,            XF86XK_TouchpadToggle,     spawn,          SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
        { 0,            XF86XK_TouchpadOff,        spawn,          {.v = (const char*[]){ "synclient", "TouchpadOff=1", NULL } } },
        { 0,            XF86XK_TouchpadOn,	   spawn,          {.v = (const char*[]){ "synclient", "TouchpadOff=0", NULL } } },
        { 0,            XF86XK_MonBrightnessUp,    spawn,     	   {.v = (const char*[]){ "xbacklight", "-inc", "15", NULL } } },
        { 0,            XF86XK_MonBrightnessDown,  spawn,     	   {.v = (const char*[]){ "xbacklight", "-dec", "15", NULL } } },
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
};

