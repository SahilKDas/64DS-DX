//cpp
// @symbol _ZN15dScMgCurling2_c6RenderEv
#include "dScMgCurling2_c.h"
// recovered name: dScMgCurling2_c_Render
/* dScMgCurling2_c::Render - recovered from vtable slot identity.
   decl_common.h declares these six with empty (no-args) C++ prototypes --
   fine for the original plain-C file (an empty `()` there means
   "unspecified args"), wrong once this is a real member call passing
   `this`. Declared locally with the real (void*) signature instead of
   including decl_common.h for this file. */
extern "C" {
int func_ov004_020adc1c();
#ifdef _MSC_VER
/* THE SCORE IS PASSED, AS THE ROM PASSES IT. func_ov004_020b19f0 draws the
   number it is given (src/func_ov004_020b19f0.c hands it to
   func_ov004_020b1ea4 as the value). The ROM calls it straight after
   func_ov004_020adc1c (0x020e67f8 / 0x020e67fc) with that call's result, the
   score word, still in r0, so the matched source calls it with no argument.
   On x86 nothing was pushed and the callee drew the stack word above its
   return address (1 on the first frames of this scene) instead of the score.
   The sibling dScMgCurling_c::Render passes the result explicitly; declared
   and called that way, the host passes what r0 held. */
int func_ov004_020b19f0(int value);
#else
int func_ov004_020b19f0();
#endif
void func_ov006_020e4a84(void*);
void func_ov006_020e4fe8(void*);
void func_ov006_020e507c(void*);
void func_ov006_020e38b0(void*);
void func_ov006_020e3bc4(void*);
void func_ov006_020e4b78(void*);
}
s32 dScMgCurling2_c::Render()
{
#ifdef _MSC_VER
    func_ov004_020b19f0(func_ov004_020adc1c());
#else
    func_ov004_020adc1c();
    func_ov004_020b19f0();
#endif
    func_ov006_020e4a84(this);
    func_ov006_020e4fe8(this);
    func_ov006_020e507c(this);
    func_ov006_020e38b0(this);
    func_ov006_020e3bc4(this);
    func_ov006_020e4b78(this);
    return 1;
}
