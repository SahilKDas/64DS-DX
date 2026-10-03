//cpp
// @symbol _ZN12daStarBase_c7CollectEv
#include "daStarBase_c.h"

extern "C" {
extern void func_02012694(int a, void* p);
extern void _ZN5dCc_c5ClearEv(char* t);
extern void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int id, int a, int b, int d);
}

void daStarBase_c::Collect()
{
    func_02012694(0x53, (char *)this + 0x74);
    {
        unsigned char* f = &mFlags;
        *f = (*f & ~1) | 1;
        *f &= ~2;
    }
    _ZN5dCc_c5ClearEv((char *)&mdCcAcPos_c);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x12c, mPosX, mPosY, mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x12d, mPosX, mPosY, mPosZ);
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x12e, mPosX, mPosY, mPosZ);
}
