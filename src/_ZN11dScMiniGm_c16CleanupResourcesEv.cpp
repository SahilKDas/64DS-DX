//cpp
// @symbol _ZN11dScMiniGm_c16CleanupResourcesEv
/* recovered: real C++ method */
/* dScMiniGm_c::CleanupResources() -- vtable slot 3. Tears the running minigame
 * down only if one was actually selected (data_0208a174[0] >= 0), then drops
 * the voice group. */
#include "dScMiniGm_c.h"
#include "decl_common.h"

extern "C" void _ZN5Sound21UnsetPlayerVoiceGroupEv(void);

s32 dScMiniGm_c::CleanupResources()
{
    if (data_0208a174[0] >= 0) {
#ifdef _MSC_VER
        /* THE SELECTED GAME IS PASSED, AS THE ROM PASSES IT. The ROM loads
           data_0208a174[0] into r1 for the test at 0x020c1660 and branches to
           func_ov005_020c0030 with it still there; that body
           (src/func_ov005_020c0030.c) takes it as its second parameter and
           turns it into a save-flag bit number. decl_common.h names one
           parameter, which byte-matches on ARM; on x86 the second was never
           pushed and the callee read a code address (0x0047fa8c) as the game
           index when the menu handed over to a minigame. Called with both, the host
           passes what r1 held. */
        ((void (*)(int, int))func_ov005_020c0030)((int)this, data_0208a174[0]);
#else
        func_ov005_020c0030((int)this);
#endif
    }
    _ZN5Sound21UnsetPlayerVoiceGroupEv();
    return 1;
}
