#include "size.h"

int Size::APP_SIZEW = 360;
int Size::APP_SIZEH = 720;

#ifdef ANDROID_SIZE
    double Size::SIZE_FACTOR = 1;
#else
    double Size::SIZE_FACTOR = 0.5;
#endif

int Size::APP_X = 0;
int Size::APP_Y = 0;
