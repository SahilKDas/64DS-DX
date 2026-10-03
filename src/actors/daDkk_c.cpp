//cpp
/* daDkk_c: DONKAKU / Grindel, the sliding crusher leaf of daDsnBase_c
 * (sibling daDsn_c / Thwomp in ov091). Overlay actor 162: symbols/overlay_actors.md
 * GRINDEL, ROM debug table DONKAKU. daDgr_c in this overlay is DONGURU, not
 * this class.
 *
 * mwccarm emits ordinary functions in reverse source order, so the
 * definitions below intentionally run from the highest retail address back
 * toward the compiler-owned destructor group. The destructor pair is written
 * by nobody: include/daDkk_c.h defines ~daDkk_c() in the class body, and that
 * alone makes mwccarm emit D1 (0x021118c8) then D0 (0x02111928) at the bottom
 * of the section list, which is the cartridge's own order. See the header for
 * why the in-class form is load-bearing.
 *
 * Known limits:
 * - dBgActor_c::IsClsnInRange, dActor_c::Earthquake and
 *   Particle::System::NewSimple take Fix12<int> by value (see
 *   notes/mwccarm-codegen.md 6az), so they stay mangled TU-local externs.
 * - The 0x39e / 0x39f accesses in func_ov025_021119a4, 021119f4 and 02111a84
 *   keep their (int)this + 0x39e integer-cast form: the named stores CSE.
 * - func_ov091_* are shared daDsnBase leaf helpers; data_ov025_02113814 is
 *   the file-table handle.
 * - Leaf operator new(unsigned long): fBase_c declares none yet.
 * - g_profile_DONKAKU stays overlay data (not this TU).
 * - func_0201267c stays a free function.
 */

/* INCLUDE ORDER IS LOAD-BEARING, the same way it is in the base class's own TU:
 * daDkk_c.h reaches daDsnBase_c.h -> dBgActor_c.h, which includes common.h
 * BEFORE Model.h, fixing Matrix4x3 to common.h's flat `s32 m[12]' spelling --
 * the one every shard here compiled against. Do not hoist math/Matrix.h, and do
 * not move dBgCh_Lin.h above this line. */
#include "daDkk_c.h"
#include "common.h"
#include "decl_common.h"
#include "dBgCh_Lin.h"

bool ApproachLinear(short &value, short target, short step);

/* decl_common.h already declares the address-named symbols this TU touches
 * (func_ov025_021119a4/021119f4/02111a84, func_ov091_02132e64/02132e98/
 * 02132ff4, data_ov025_02113814), all taking char*.
 *
 * These are the ones no header declares. func_0201267c is `void`: its own
 * enrolled definition is `void func_0201267c(unsigned int id, const Vector3
 * *v)`, the tree spells it void in 83 files against 21 for int, and neither
 * call site here reads the result. The `(int, void*)` parameter spelling is
 * the tree's majority shorthand for that signature and links because the
 * symbol is extern "C". */
extern "C" {
extern void func_0201267c(int a, void *b);
extern int _ZN10dBgActor_c13IsClsnInRangeE5Fix12IiES1_(void *p, Fix12i a, Fix12i b);
extern void _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(void *thiz, const Vector3 &v, int f);
extern void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int n, int x, int y, int z);
}

// @symbol daDkk_c_classInit
/* Reconstructed source-style name: SM64DS proves daDkk_c through RTTI,
 * allocation size, vtable identity, and the DONKAKU registry profile;
 * later EAD lineage supplies classInit. Exact original spelling is not
 * preserved. Historical alias: Grindel_Spawn.
 *
 * Every instruction the cartridge has here falls out of the one `new`.
 * 0x02111cf8 loads 928 == 0x3a0 into the header's inline operator new;
 * dBgActor_c::C2, the mid-construction daDsnBase_c vptr, TextureSequence@0x324,
 * ShadowModel@0x338, and this class's vptr all come from the implicit ctor
 * the `new` inlines. The null check is the one `new` itself emits. */
extern "C" daDkk_c *daDkk_c_classInit()
{
    return new daDkk_c();
}

// @symbol _ZN7daDkk_c13InitResourcesEv
/* Vtable slot 0, override of a slot daDsnBase_c leaves pure, and this class's
 * ABI key function -- the first declared virtual that is neither inline nor
 * pure -- so defining it here is what makes this TU emit _ZTV7daDkk_c.
 *
 * Stores the file table pointer at mFileTable, calls daDsnBase_c::Init,
 * then either forces mState to a fixed "already airborne" value or runs a
 * downward raycast from the actor's own position to set mProbeHeight from the
 * collision point it finds. */
int daDkk_c::InitResources()
{
    mFileTable = (s32)data_ov025_02113814;
    int r = Init();
    if (param1 & 1) {
        mState = 6;
    } else {
        mState = 0;
        dBgCh_Lin ray;
        Vector3 va;
        Vector3 vb;
        int x = mPosX;
        vb.x = x;
        int y = mPosY;
        vb.y = y;
        int z = mPosZ;
        va.x = x;
        vb.z = z;
        va.y = y;
        va.z = z;
        vb.y = y + 0x7d0000;
        ray.SetObjAndLine(va, vb, this);
        if (ray.DetectClsn()) {
            Vector3 p1 = ray.GetClsnPos();
            Vector3 p2 = ray.GetClsnPos();
            mProbeHeight = p2.y - 0x190000;
        }
    }
    return r;
}

// @symbol _ZN7daDkk_c8BehaviorEv
/* Vtable slot 6, the other slot daDsnBase_c leaves pure.
 *
 * Switches on mState to one of eight per-state step functions -- five shared
 * with the ov091 siblings, three private to this overlay -- then runs the
 * post-step housekeeping every daDsnBase_c leaf needs. */
int daDkk_c::Behavior()
{
    char *c = (char *)this;
    switch (mState) {
    case 0: func_ov091_02133020(); break;
    case 1: func_ov091_02132ff4(c); break;
    case 2: func_ov091_02132f04(); break;
    case 3: func_ov091_02132e98(c); break;
    case 4: func_ov091_02132e64(c); break;
    case 5: func_ov025_02111a84(); break;
    case 6: func_ov025_021119f4(); break;
    case 7: func_ov025_021119a4(); break;
    }
    UpdateModelPosAndRotY();
    func_ov091_02133098();
    if (_ZN10dBgActor_c13IsClsnInRangeE5Fix12IiES1_(c, 0, 0) != 0 ||
        func_ov091_02132dc0() != 0) {
        UpdateClsnPosAndRot();
    }
    return 1;
}

// @symbol _ZN7daDkk_c19func_ov025_02111a84Ev
/* mState 5, the fall. Integrates the drop, and on reaching the stored ground
 * height snaps to it, shakes the camera, spawns the impact particle and hands
 * over to state 6. */
void daDkk_c::func_ov025_02111a84()
{
    Vector3 v[2];
    UpdatePos(0);
    if (mVertSpeed >= 0)
        mVertAccel = -0x4000;
    else
        mVertAccel = -0x8000;
    if (mPosY > unk_394)
        return;
    mPosY = unk_394;
    v[1].x = mPosX;
    v[1].y = mPosY;
    v[1].z = mPosZ;
    _ZN8dActor_c10EarthquakeERK7Vector35Fix12IiE(this, v[1], 0x7d0000);
    unk_39e = 0x3c;
    *(unsigned char *)(((int)this + 0x39f)) =
        *(unsigned char *)(((int)this + 0x39f)) + 1;
    mState = 6;
    v[0].x = mPosX;
    v[0].y = mPosY;
    v[0].z = mPosZ;
    v[0].y = v[0].y + 0x3c000;
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x2e, v[0].x, v[0].y, v[0].z);
    func_0201267c(0xc7, &mCamSpacePosX);
}

// @symbol _ZN7daDkk_c19func_ov025_021119f4Ev
/* mState 6, the pause after landing. Counts 0x39e down; at zero it either
 * starts the next fall (state 5) or, on the fourth pass, turns to face the
 * opposite way (state 7). */
void daDkk_c::func_ov025_021119f4()
{
    *(u8 *)(((int)this + 0x39e)) =
        *(u8 *)(((int)this + 0x39e)) - 1;
    if (unk_39e != 0) return;
    if (unk_39f != 4) {
        mState = 5;
        mVertSpeed = 0x3c000;
        func_0201267c(0xf4, &mCamSpacePosX);
        return;
    }
    mState = 7;
    unk_39c = (s16)(mAngleY + 0x8000);
}

// @symbol _ZN7daDkk_c19func_ov025_021119a4Ev
/* mState 7, the turn. Steps mAngleY toward the target angle in unk_39c; once
 * ApproachLinear reports it has arrived, copies it into mPrevAngleY, clears the
 * pass counter and goes back to state 6. */
int daDkk_c::func_ov025_021119a4()
{
    int arrived = ApproachLinear(mAngleY, unk_39c, 0x400);
    if (arrived == 0) return arrived;
    mPrevAngleY = mAngleY;
    mState = 6;
    unk_39f = 0;
    unk_39e = 0x28;
    return 0x28;
}

// @symbol _ZN7daDkk_c16OnAimedAtWithEggEv
/* Vtable slot 29, override of dActor_c::OnAimedAtWithEgg. `mov r0,#0xce000; bx
 * lr'. Slot 29's return is added to pos.y (a height), same as daOts. 0xce000
 * is a Fix12i of 206.0 -- much taller than dActor_c's own default of 20.0
 * (0x14000). */
int daDkk_c::OnAimedAtWithEgg()
{
    return 0xce000;
}

/* D1 (0x021118c8) and D0 (0x02111928) are deliberately not written here.
 * include/daDkk_c.h defines ~daDkk_c() in the class body, which is what makes
 * mwccarm emit the pair in the cartridge's D1-then-D0 order with no D2. An
 * out-of-line definition emits D2, D0, D1 instead. Owning the key function
 * above drags both variants in, so no forcing scaffold is needed. */
// @symbol _ZN7daDkk_cD1Ev
// @symbol _ZN7daDkk_cD0Ev
