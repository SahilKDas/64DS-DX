//cpp
// @symbol _ZN12daStarBase_c6RenderEv
/* recovered: named members + shared header, real C++ method */
#include "daStarBase_c.h"
extern "C" {

struct Sub {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void m(int);
};
}

int daStarBase_c::Render()
{
    unsigned int b = mFlags;
    if ((b << 30) >> 31) {
        ((Sub *)((char *)&mModel))->m(0);
    }
    return 1;
}
