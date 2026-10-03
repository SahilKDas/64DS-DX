//cpp
// @symbol _ZN12daStarBase_c16CleanupResourcesEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header */
#include "decl_common.h"
/* recovered: named members + shared header, real C++ method */
#include "daStarBase_c.h"
#include "SharedFilePtr.h"
extern char data_ov002_0211092c;
extern SharedFilePtr data_ov002_0210d9a8;

int daStarBase_c::CleanupResources()
{
    if (mState != 0) {
        ((SharedFilePtr *)(&data_ov002_0211092c))->Release();
        data_ov002_0210d9a8.Release();
    } else {
        ((SharedFilePtr *)(&data_ov002_0211093c))->Release();
    }
    return 1;
}
