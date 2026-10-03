//cpp
#pragma defer_codegen off
/**
 * Lethal Lava Land's spinning disc (profile FL_KOMA_D).
 *
 * No fields. InitResources / CleanupResources hand this overlay's
 * model and collision files to daObjKaitendai_c's shared ov002
 * helpers, with a fixed spin step of 0x100. The destructor is out
 * of line: defer_codegen off lays down D1, D0, CleanupResources,
 * InitResources in ROM order.
 *
 * daObjFl_Koma_D_c_classInit is reconstructed (RTTI
 * daObjFl_Koma_D_c, FL_KOMA_D registry) and is not in this text
 * slice. Retail does not store that spelling.
 *
 * Leftover: func_ov002_020b676c / func_ov002_020b66a8 are still the
 *   linker names of daObjKaitendai_c Init/Cleanup (the base leaves
 *   those slots pure virtual). Naming belongs in ov002.
 * Leftover: data_ov022_02113da4 is the model/collision/CLPS row
 *   between typeinfo at 02113d98 and the type name at 02113db0.
 *   Not owned here.
 * Leftover: g_profile_FL_KOMA_D and the classInit factory live
 *   outside this TU.
 * Leftover: #pragma defer_codegen off stays on line 2. Dropping it
 *   still matches each body, but the four functions emit in reverse
 *   ROM order (ordinal pairs (0, 1), (1, 2), (2, 3)).
 * Leftover: an inline empty destructor still matches D1 (size 0x50)
 *   and D0 (size 0x64), but ordinal pair (1, 2) flips, D2 is not
 *   emitted, and 13 RTTI/vtable symbols come out unlicensed.
 * Leftover: #pragma long_calls is not an mwccarm pragma. Deleting
 *   both brackets still matches CleanupResources (size 0x14) and
 *   InitResources (size 0x18). A local s16 angle = 0x100 also
 *   matches InitResources at size 0x18; the immediate stays.
 */

#include "daObjFl_Koma_D_c.h"

/* This descriptor matches the shared helpers in daObjKaitendai_c.cpp. */
struct SharedFilePtr;

struct CLPS_Block;

struct ResourceDescriptor {
    SharedFilePtr *model;
    SharedFilePtr *collision;
    CLPS_Block *clps;
};

typedef char ResourceDescriptor_size_must_be_0x0c[
    sizeof(ResourceDescriptor) == 0x0c ? 1 : -1];

extern "C" {
int func_ov002_020b676c(daObjKaitendai_c *self, ResourceDescriptor *descriptor,
                        s16 angle);
int func_ov002_020b66a8(daObjKaitendai_c *self, ResourceDescriptor *descriptor);
extern ResourceDescriptor data_ov022_02113da4;
}

// @symbol _ZN16daObjFl_Koma_D_cD1Ev
// @symbol _ZN16daObjFl_Koma_D_cD0Ev
daObjFl_Koma_D_c::~daObjFl_Koma_D_c()
{
}

// @symbol _ZN16daObjFl_Koma_D_c16CleanupResourcesEv
int daObjFl_Koma_D_c::CleanupResources()
{
    return func_ov002_020b66a8(this, &data_ov022_02113da4);
}

// @symbol _ZN16daObjFl_Koma_D_c13InitResourcesEv
int daObjFl_Koma_D_c::InitResources()
{
    return func_ov002_020b676c(this, &data_ov022_02113da4, 0x100);
}

#ifdef _MSC_VER
/* Host flat names. Not compiled into the cartridge object. */
extern "C" daObjFl_Koma_D_c *_ZN16daObjFl_Koma_D_cD0Ev(daObjFl_Koma_D_c *thiz)
{
    thiz->daObjFl_Koma_D_c::~daObjFl_Koma_D_c();
    daObjFl_Koma_D_c::operator delete(thiz);
    return thiz;
}

extern "C" daObjFl_Koma_D_c *_ZN19RotatingPlatformLllD0Ev(daObjFl_Koma_D_c *thiz)
{
    return _ZN16daObjFl_Koma_D_cD0Ev(thiz);
}
#endif
