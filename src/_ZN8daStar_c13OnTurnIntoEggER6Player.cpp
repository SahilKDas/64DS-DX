//cpp
// @symbol _ZN8daStar_c13OnTurnIntoEggER6Player
// recovered name: PowerStar_OnTurnIntoEgg
/* daStar_c::OnTurnIntoEgg -- vtable slot 19, verified against ov002 relocs.txt:
 * _ZTV8daStar_c (0x0210ab3c) + 0x4c -> 0x020e8edc, exactly this placeholder's
 * former address (former name func_ov002_020e8edc). The ROM body is a
 * tail-call veneer (`ldr ip, [pc]; bx ip`) to func_ov002_020e8e80, passing
 * `this`/`player` straight through unchanged.
 * Matched byte-for-byte with mwccarm 2004/b56 (ov002).
 */
#include "daStar_c.h"
#include "Player.h"

extern "C" void func_ov002_020e8e80(daStar_c *thiz, Player &player);

void daStar_c::OnTurnIntoEgg(Player &player)
{
    return func_ov002_020e8e80(this, player);
}
