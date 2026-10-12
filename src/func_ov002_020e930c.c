#include "types.h"
/* func_ov002_020e930c at 0x020e930c
 *
 * Matched byte-for-byte with mwccarm 1.2/sp2p3 (ov002).
 */
extern void* _ZN8dActor_c10FindWithIDEj(unsigned int id);
extern int _ZN5Event6GetBitEj(unsigned int bit);
extern int func_ov002_020e8ef0(void* a, void* b);
#ifdef _MSC_VER
/* THE MARKER IS PASSED AS THE RECEIVER, AS THE ROM DOES IT. The ROM calls
   StarMarker::Collect at 0x020e93c0 and 0x020e9438, each straight after a
   FindWithID (0x020e93ac, 0x020e9424) whose result is still in r0: that r0
   is Collect's `this`. Declared with no parameters, the host call pushed
   nothing and the face read its receiver from a stack slot nobody wrote, so
   the glass container kept its drawn bit after its star was taken. */
extern void _ZN10StarMarker7CollectEv(void* self);
#else
extern void _ZN10StarMarker7CollectEv(void);
#endif

void func_ov002_020e930c(void* self) {
    char* a = (char*)self;
    void* o;
    char* b;
    int flags;
    unsigned int id;

    id = *(unsigned int*)(a + 0x134);
    if (id == 0) return;
    o = _ZN8dActor_c10FindWithIDEj(id);
    if (o == 0) return;
    b = (char*)o;

    flags = *(int*)(a + 0x130);
    if (flags & 0x400000) {
        if (*(u8*)(b + 0x709) != 0) return;
        if (_ZN5Event6GetBitEj(0x1e) != 0) return;
        if (func_ov002_020e8ef0(self, o) == 0) return;
        if (*(int*)(a + 0x43c) != 6) return;
#ifdef _MSC_VER
        o = _ZN8dActor_c10FindWithIDEj(*(unsigned int*)(a + 0x434));
        if (o == 0) return;
        _ZN10StarMarker7CollectEv(o);
#else
        if (_ZN8dActor_c10FindWithIDEj(*(unsigned int*)(a + 0x434)) == 0) return;
        _ZN10StarMarker7CollectEv();
#endif
    } else {
        if (flags & 0x8000) {
            if (*(u8*)(b + 0x709) != 0) return;
            if (_ZN5Event6GetBitEj(0x1e) != 0) return;
            if (*(int*)(a + 0x43c) != 6) return;
#ifdef _MSC_VER
            o = _ZN8dActor_c10FindWithIDEj(*(unsigned int*)(a + 0x434));
            if (o == 0) return;
            _ZN10StarMarker7CollectEv(o);
#else
            if (_ZN8dActor_c10FindWithIDEj(*(unsigned int*)(a + 0x434)) == 0) return;
            _ZN10StarMarker7CollectEv();
#endif
        }
    }
}
