//cpp
// @symbol _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE
/* recovered: named members + shared header */
#include "dActor_c.h"
extern "C" {
#ifdef _MSC_VER
/* THE STRENGTH IS FORWARDED, AS THE ROM FORWARDS IT. The ROM body at
   0x02010168 only replaces r0 (this) with the camera and tail-branches to
   func_0200d8c8, so the position in r1 and the strength in r2 reach it
   untouched and the two-argument call below byte-matches. On x86 the third
   argument has to be pushed: without it func_0200d8c8 read the stack slot
   above its arguments (a stack address, 0x001af054) as the strength instead
   of the caller's 0x320000..0x860000, and the camera shake radius of every
   crusher, Whomp and boss landing was wrong. Declared with the strength, the
   host passes what r2 held. */
extern void func_0200d8c8(void*, int, int);
#else
extern void func_0200d8c8(void*, int);
#endif
extern void* data_0209f318;

#ifdef _MSC_VER
void _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(void* v, int f, int strength) {
    func_0200d8c8(data_0209f318, f, strength);
}
#else
void _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(void* v, int f) {
    func_0200d8c8(data_0209f318, f);
}
#endif
}
