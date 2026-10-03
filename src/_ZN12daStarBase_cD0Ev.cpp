//cpp
// @symbol _ZN12daStarBase_cD0Ev

#include "daStarBase_c.h"

#ifdef _MSC_VER
/* THE HOST NEEDS THE ROM'S FLAT D0 NAME, AND MSVC NEVER EMITS IT. MSVC
 * folds the Itanium destructor variants into the one ~daStarBase_c() it
 * emits, which the class's D1 file already defines, so compiling the
 * definition below as well would define that symbol twice. This arm
 * spells out, in terms of it, what the deleting destructor this file is
 * enrolled for does: the D1 body, called qualified so it is a direct call
 * even where a header declares the destructor virtual, then the
 * class-specific operator delete. Nothing here reaches mwccarm: it builds
 * the `#else` arm and emits the ROM bytes it always emitted, and the
 * object is byte-identical either way. */
extern "C" daStarBase_c *_ZN12daStarBase_cD0Ev(daStarBase_c *thiz)
{
    thiz->daStarBase_c::~daStarBase_c();    /* the D1 body, through the one host symbol */
    daStarBase_c::operator delete(thiz);  /* the class-specific delete D0 ends with */
    return thiz;
}
#else
daStarBase_c::~daStarBase_c()
{
}
#endif
