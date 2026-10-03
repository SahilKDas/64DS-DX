//cpp
/* An area-transition trigger box -- ov002/daChRoom_c.
 *
 * ROM RTTI names daChRoom_c; overlay_actors.md VIRTUAL_DOOR(348). Ugly RTTI
 * name is final. mwccarm emits ordinary function sections in reverse source
 * order; keep the factory first. The inline destructor in daChRoom_c.h emits
 * retail D1 then D0 and no D2.
 *
 * daChRoom_c_classInit is reconstructed (RTTI daChRoom_c, CH_ROOM registry).
 * Retail does not store that spelling. Historical alias daChRoom_c_Spawn.
 *
 * deslop
 * Leftover: Vec3_Sub / Vec3_RotateYAndTranslate have no owning header;
 *   tree-wide parameter spellings disagree.
 * Leftover: data_0209f394 is the player table, data_0209f250 the current
 *   player index. Typed here as dActor_c* -- Behavior only uses
 *   dActor_c::mPosX and dActor_c::mAreaId. Player.h is out of scope.
 * Leftover: data_020a0ebc is an arm9 scratch transform; the tree disagrees
 *   on its type (char / int / Vector3 / Triple).
 */

#include "daChRoom_c.h"

extern "C" {
extern void Vec3_Sub(Vector3* out, Vector3* a, Vector3* b);
extern void Vec3_RotateYAndTranslate(Vector3* out, void* m, s16 ang, Vector3* in);
extern void ChangeArea(int);
extern u8 data_0209f250;
extern dActor_c* data_0209f394[];
extern char data_020a0ebc;
}

// @symbol daChRoom_c_classInit
extern "C" daChRoom_c *daChRoom_c_classInit()
{
    return new daChRoom_c();
}

// @symbol _ZN10daChRoom_c13InitResourcesEv
int daChRoom_c::InitResources()
{
  mScaleX=(((param1&0xf)+1)*0x64000)>>1;
  mScaleY=(((param1>>4&0xf)+1)*0x64000);
  mAngleY=-mAngleY;
  return 1;
}

// @symbol _ZN10daChRoom_c8BehaviorEv
int daChRoom_c::Behavior()
{
    dActor_c* obj;
    Vector3 d;
    Vector3 r;
    int v;

    obj = data_0209f394[data_0209f250];
    Vec3_Sub(&d, (Vector3*)&obj->mPosX, (Vector3*)&mPosX);
    Vec3_RotateYAndTranslate(&r, &data_020a0ebc, mAngleY, &d);

    v = r.x;
    if (v < 0) v = -v;
    if (v < mScaleX) {
        if (r.y > -0x96000) {
            if (r.y < mScaleY) {
                int z = r.z;
                int az = (z < 0) ? -z : z;
                if (az > 0x64000 && az < 0x190000) {
                    int area = (z < 0) ? mAngleX : mAngleZ;
                    obj->mAreaId = (char)area;
                    ChangeArea((char)area);
                }
            }
        }
    }
    return 1;
}

// @symbol _ZN10daChRoom_c6RenderEv
int daChRoom_c::Render()
{
    return 1;
}

// @symbol _ZN10daChRoom_c16OnPendingDestroyEv
void daChRoom_c::OnPendingDestroy()
{
}

// @symbol _ZN10daChRoom_c16CleanupResourcesEv
int daChRoom_c::CleanupResources()
{
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 1 -- _ZN10daChRoom_cD0Ev, 0x020b081c, size 0x38 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN10daChRoom_cD0Ev
/* _ZN10daChRoom_cD0Ev (vtable slot 17, the deleting destructor) is NOT
 * hand-written here. A hand-written mangled D0 next to a real out-of-line D1
 * ICEs mwccarm 2004/b56 (ELFgen.c:483); the compiler synthesizes D0 itself from
 * D1. The two legacy files src/_ZN10daChRoom_cD0Ev.cpp and
 * src/_ZN10daChRoom_cD1Ev.cpp had each independently reconstructed the SAME
 * `daChRoom_c::~daChRoom_c(){}` -- the standard D0/D1 collapse artifact of one
 * destructor split across two one-function files. */

/* -------------------------------------------------------------------------- */
/* ROM ordinal 0 -- _ZN10daChRoom_cD1Ev, 0x020b07f8, size 0x24 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN10daChRoom_cD1Ev
/* recovered: real C++ destructor -- the compiler emits the whole body.
 * Vtable slot 16: one vptr store, then the call into ~dActor_c.
 *
 * (no definition here: `virtual ~daChRoom_c() {}` is in include/daChRoom_c.h,
 * and that placement is load-bearing rather than stylistic -- out of line,
 * mwccarm emits D0 before D1 and adds a homeless D2, and objisolate then
 * refuses this whole TU. The header carries the reasoning and the leaf
 * measurement that makes it safe.) */
