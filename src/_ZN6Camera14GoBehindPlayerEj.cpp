//cpp
// @symbol _ZN6Camera14GoBehindPlayerEj
/* recovered: named members + shared header, real C++ method, declarations from a shared header */
#include "decl_common.h"
/* recovered: named members + shared header, real C++ method */
#include "Camera.h"
extern "C" {
extern unsigned char data_0209f250;
extern signed char data_02092110;
extern void _ZN6Camera11ChangeStateEPNS_5StateE(void *self, void *st);
#ifdef _MSC_VER
extern void *data_0209f394[];
#endif
}

void Camera::GoBehindPlayer(unsigned int j)
{
    int slot4, slot8, slotc;

    if (j != data_0209f250)
        return;
    if (data_02092110 >= 0)
        return;

    /* different launder spellings to defeat CSE across the call */
    *(unsigned int *)(((int)&mFlags)) &= 0xfffffaf7u;
    func_0200cb58((void *)((int)this), 0xa);
    *(unsigned int *)(((long long)(int)((int)&mFlags))) |= 4u;

    slot4 = mState_13c;
    slotc = 0;
#ifdef _MSC_VER
    /* THE VIEW SLOT HOLDS WHAT THE CARTRIDGE'S STACK LEAVES IN IT. The ROM
       never writes slot8 either (0x0200d304: only sp+4, sp+0xc and sp are
       stored before the call at 0x0200d398), and func_0200c66c reads it back
       as its view object whenever the floor names no view (col2 == 0x3f).
       On the cartridge that word is the caller's player pointer, pushed by
       the call made just before this one: daStarGate_c::St_OpenClose_Main
       reaches it through func_ov100_02144fcc -> func_02012be0's push of r6
       (the player, 0x0214598c / 0x02012be0), and the Door's open callback
       func_ov100_02144730 through Animation::WillHitFrame's push of r4 (the
       player, 0x021448c0 / 0x02015a98). j is that player's index (checked
       above). On the host the slot held whatever the last call left there,
       and a star door's close faulted reading it (eax 0x47). */
    slot8 = (int)data_0209f394[j];
#endif
    func_0200c66c((void *)((int)this), (void *)(mTargetPlayer + 0x5c), &slot8, &slot4, &slotc);
    if (slot4 == (int)&data_020873dc)
        return;
    if (slot4 == (int)&data_0208742c)
        return;
    _ZN6Camera11ChangeStateEPNS_5StateE((void *)((int)this), &data_0209b0e8);
}
