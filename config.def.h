/* See LICENSE file for copyright and license details. */

/* appearance */

#include <X11/XF86keysym.h>

static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 38;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = {
        "monospace:size=12",
        "fontawesome:size=12:antialias=true:autohint=true"
};
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan1[]       = "#005577";
static const char col_cyan2[]       = "#0038ff";
static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
    [SchemeSel]  = { col_gray4, col_cyan1, col_cyan2  },
};
static const unsigned int gappx     = 6;        /* gap pixel between windows */

/* tagging */
static const char *tags[] = { "\uf120", "\uf907", "\uf121", "\uf1bc", "*" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 10;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { ">>=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "st", NULL };
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb",
        col_gray1, "-nf", col_gray3, "-sb", col_cyan1, "-sf", col_gray4, NULL };
/* personaliced */
static const char *upvol[]   = {"amixer", "-D", "pulse", "sset", "Master", "5%+", NULL};
static const char *downvol[] = {"amixer", "-D", "pulse", "sset", "Master", "5%-", NULL};
static const char *mutevol[] = {"amixer", "-D", "pulse", "sset", "Master", "0%",  NULL};
static const char *brightup[]   = { "xbacklight", "-inc", "10", NULL };
static const char *brightdown[] = { "xbacklight", "-dec", "10", NULL };
static const char *pfullscreen[] = { "scrot", "-d", "1", "%Y-%m-%d-%H:%M:%S.jpg", "-e", "mv $f ~/Imágenes/Screenshots/", NULL };
static const char *pthisscreen[] = { "scrot", "--focused", "-d", "1", "%Y-%m-%d-%H:%M:%S.jpg", "-e", "mv $f ~/Imágenes/Screenshots/", NULL };
/* La sig linea no tiene interactividad. ¿Debo hacer SHCMD(scrot --select -d 1 %Y-%m-%d-%H:%M:%S.jpg -e mv $f ~/Imágenes/Screenshots/ NULL ) ?*/
static const char *psomescreen[] = { "scrot", "--select", "-d", "1", "%Y-%m-%d-%H:%M:%S.jpg", "-e", "mv $f ~/Imágenes/Screenshots/", NULL };

static Key keys[] = {
    /* personaliced */
    { 0,              XF86XK_AudioMute,        spawn,          {.v = mutevol } },
    { 0,              XF86XK_AudioLowerVolume, spawn,          {.v = downvol } },
    { 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = upvol } },
    { 0,               XF86XK_MonBrightnessUp, spawn,          {.v = brightup } },
    { 0,             XF86XK_MonBrightnessDown, spawn,          {.v = brightdown } },
    { 0,                            XK_Print,  spawn,          {.v = pfullscreen } },
    { ControlMask,                  XK_Print,  spawn,          {.v = pthisscreen } },
    { ShiftMask,                    XK_Print,  spawn,          {.v = psomescreen } },
    { MODKEY,                       XK_F1,     spawn,          SHCMD("firefox") },
    /* modifier                     key        function        argument */
    { MODKEY|ShiftMask,             XK_d,      spawn,          {.v = dmenucmd } },
    { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_Return, zoom,           {0} },
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
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
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

