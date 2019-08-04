#pragma once

#ifdef ANDROID_SIZE
    #define TITLE_FONT_SIZE 20
    #define FONT_SIZE_0  15
    #define FONT_SIZE_1  18
    #define FONT_SIZE_2  20
    #define FONT_SIZE_3  28
    #define WINDOW_SIZEW 720
    #define WINDOW_SIZEH 1310
#else
    #define TITLE_FONT_SIZE 20
    #define FONT_SIZE_0 10
    #define FONT_SIZE_1 13
    #define FONT_SIZE_2 15
    #define FONT_SIZE_3 15
    #define WINDOW_SIZEW 960
    #define WINDOW_SIZEH 1310
    #define THIRDPLAYER_COLUMN
#endif

class Size {
public:

static int APP_SIZEW;
static int APP_SIZEH;
static double SIZE_FACTOR;
static int APP_X;
static int APP_Y;
};
