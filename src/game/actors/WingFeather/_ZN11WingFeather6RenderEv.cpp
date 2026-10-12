//cpp
// @symbol _ZN11WingFeather6RenderEv
/* recovered: named members + shared header, real C++ method */
#include "WingFeather.h"

int WingFeather::Render()
{
    /* the last 45 frames on the ground blink: odd values skip the draw */
    u8 life = mLifeTimer;
    if (life < 0x2d) {
        if (life & 1) return 1;
    }
    mModel.Render(0);
    return 1;
}
