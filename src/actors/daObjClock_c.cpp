//cpp
/* ov013/daObjClock_c -- reconstructed translation unit (9 functions).
 *
 * ROM run 0x021113bc..0x021116ac, plus the class's .data run at
 * 0x021121a4..0x0211227c (_ZTI, _ZTS, the g_profile_CLOCK_LONG and
 * g_profile_CLOCK_SHORT descriptors, and the _ZTV whose address point is
 * 0x02112200). ov013 is the clock painting overlay (not TTC / ov065):
 * CLOCK_PAINTING_HAND_SHORT (292) and CLOCK_PAINTING_HAND_LONG (293) share
 * this class; CLOCK_PAINTING_PENDULUM (294) is the sibling daObjClockHuriko_c.
 *
 * FUNCTION ORDER IS DELIBERATELY THE REVERSE OF THE ROM'S -- mwccarm 2004/b56
 * emits one .text section per function in the reverse of source order, so the
 * highest-address ROM function is written FIRST here. Do not reorder; the
 * destructor pair at the end is the one documented exception, because the
 * compiler picks the order inside a D0/D1 group itself.
 *
 * Members, in ROM address order (each was a one-function shard before this
 * TU took over the run; the shards are gone):
 *   [0] 0x021113bc  daObjClock_c::~daObjClock_c   (D1)
 *   [1] 0x021113ec  daObjClock_c::~daObjClock_c   (D0)
 *   [2] 0x02111430  func_ov013_02111430           (state helper)
 *   [3] 0x02111478  daObjClock_c::CleanupResources
 *   [4] 0x021114a4  daObjClock_c::Render
 *   [5] 0x021114cc  daObjClock_c::Behavior
 *   [6] 0x021115cc  daObjClock_c::InitResources
 *   [7] 0x0211163c  daObjClock_c_classInit_CLOCK_SHORT   (factory, actor 292)
 *   [8] 0x02111674  daObjClock_c_classInit_CLOCK_LONG    (factory, actor 293)
 *
 * Known limits:
 * - data_ov013_021116ac / data_ov013_021116b0: per-hand angular speed and
 *   SharedFilePtr handles. This TU consumes them; overlay .data owns them.
 * - two C-linkage factories stay CLOCK_SHORT / CLOCK_LONG: EAD would name
 *   both daObjClock_c_classInit (not_apply=global_classinit_name_collision).
 * - Matrix4x3_FromRotationZXYExt stays the C helper (no class method).
 * - func_ov013_02111430 is a daObjClock_c method under its ROM address
 *   (S33); symbols.txt carries the mangled spelling.
 */

#include "daObjClock_c.h"
#include "SharedFilePtr.h"

extern "C" {
extern void Matrix4x3_FromRotationZXYExt(void *, int, int, int);
int IsAreaShowing(int areaId);
extern signed char data_02092110[];
extern unsigned char data_0209f2c0[];
extern short data_ov013_021116ac[];
extern SharedFilePtr *data_ov013_021116b0[];
}

/* ONE CLASS, TWO PROFILES. CLOCK_LONG (actor 293) and CLOCK_SHORT (actor 292)
 * each own a descriptor and a separate factory; both install the same
 * vtable at 0x02112200 and the same 0x128 allocation, and InitResources tells
 * them apart at run time by actorID.
 *
 * The factories keep their coined spellings. The registry's
 * factory_rename_recommended is `no` for both rows for exactly this reason
 * (not_apply=global_classinit_name_collision): the EAD convention would name
 * both daObjClock_c_classInit, and two C-linkage definitions cannot share one
 * name. Only the two profile records are renamed, to the spellings the ROM's
 * own debug string table proves -- CLOCK_SHORT at 0x02090230, CLOCK_LONG at
 * 0x0208ffa8. */
// @symbol daObjClock_c_classInit_CLOCK_LONG
extern "C" daObjClock_c *daObjClock_c_classInit_CLOCK_LONG()
{
    return new daObjClock_c();
}

// @symbol daObjClock_c_classInit_CLOCK_SHORT
extern "C" daObjClock_c *daObjClock_c_classInit_CLOCK_SHORT()
{
    return new daObjClock_c();
}

/* The 0x1c actor descriptor, `actor_profile_0x1c` in
 * symbols/profile_reconstruction_registry.tsv. Emitted in SOURCE order, which
 * is why CLOCK_LONG (0x021121c0) is written before CLOCK_SHORT (0x021121dc).
 * The fBase_c constructor passes +4/+6 to behavior/render priority setters.
 * Their names describe those uses. The retained s16 fields, order and values
 * do not establish the original signedness. */
struct ClockSpawnInfo {
    daObjClock_c *(*classInit)();
    s16 profileID;
    s16 drawOrder;
    u32 actorFlags;
    s32 clipOffsetY;
    s32 clipRadius;                 /* 0x1000 == 1.0 */
    s32 clipDistance;
    s32 farDistance;
};
typedef char ClockSpawnInfo_size_must_be_0x1c[sizeof(ClockSpawnInfo) == 0x1c ? 1 : -1];

// @symbol g_profile_CLOCK_LONG
extern "C" ClockSpawnInfo g_profile_CLOCK_LONG = {
    daObjClock_c_classInit_CLOCK_LONG, 0x0125, 0x00a3, 0x00000006,
    0x00064000, 0x000fa000, 0x00c80000, 0x00640000
};

// @symbol g_profile_CLOCK_SHORT
extern "C" ClockSpawnInfo g_profile_CLOCK_SHORT = {
    daObjClock_c_classInit_CLOCK_SHORT, 0x0124, 0x00a2, 0x00000006,
    0x00064000, 0x000fa000, 0x00c80000, 0x00640000
};

// @symbol _ZN12daObjClock_c13InitResourcesEv
/* Hand 0 is the long hand (actor 293), hand 1 the short (actor 292); loads that
 * hand's model file through data_ov013_021116b0[mHandIndex]. */
int daObjClock_c::InitResources()
{
    int isLongHand = (int)(actorID == 0x125);
    if (isLongHand != 0)
        mHandIndex = 0;
    else
        mHandIndex = 1;
    {
        unsigned char index = mHandIndex;
        SharedFilePtr &file = *data_ov013_021116b0[index];
        mModel.SetFile((BMD_File *)Model::LoadFile(file), 1, -1);
    }
    func_ov013_02111430();
    return 1;
}

// @symbol _ZN12daObjClock_c8BehaviorEv
/* With data_02092110[0] <= 0 the hand advances mAngleZ by its per-hand speed
 * in data_ov013_021116ac. With it above 0, the long hand (mHandIndex 0) writes
 * data_0209f2c0[0] from the quadrant of -mAngleZ while its area is showing:
 * below 0x2000 and from 0xe000 up is 3, 0x2000..0x5fff is 0, 0x6000..0x9fff is
 * 2, 0xa000..0xdfff is 1. Either way the model matrix is rebuilt last. */
int daObjClock_c::Behavior()
{
    if (data_02092110[0] <= 0) {
        mAngleZ += data_ov013_021116ac[mHandIndex];
    } else if (IsAreaShowing(mAreaId) && mHandIndex == 0) {
        unsigned short angle = (unsigned short)-mAngleZ;
        if (angle < 0x2000)
            data_0209f2c0[0] = 3;
        else if (angle < 0x6000 && angle >= 0x2000)
            data_0209f2c0[0] = 0;
        else if (angle < 0xa000 && angle >= 0x6000)
            data_0209f2c0[0] = 2;
        else if (angle < 0xe000 && angle >= 0xa000)
            data_0209f2c0[0] = 1;
        else if (angle >= 0xe000)
            data_0209f2c0[0] = 3;
    }
    func_ov013_02111430();
    return 1;
}

// @symbol _ZN12daObjClock_c6RenderEv
int daObjClock_c::Render()
{
    mModel.Render(0);
    return 1;
}

// @symbol _ZN12daObjClock_c16CleanupResourcesEv
int daObjClock_c::CleanupResources()
{
    data_ov013_021116b0[mHandIndex]->Release();
    return 1;
}

// @symbol _ZN12daObjClock_c19func_ov013_02111430Ev
/* Rebuild the owned model matrix from inherited actor angles and position.
 * A daObjClock_c method under its ROM address (S33).
 * The marker above is not decoration -- without it tools/tiers.py folds this
 * body into the preceding member's fragment. */
void daObjClock_c::func_ov013_02111430()
{
    Matrix4x3_FromRotationZXYExt(&mModel.mat4x3, mAngleX, mAngleY, mAngleZ);
    mModel.mat4x3.t.x = mPosX >> 3;
    mModel.mat4x3.t.y = mPosY >> 3;
    mModel.mat4x3.t.z = mPosZ >> 3;
}

// @symbol _ZN12daObjClock_cD1Ev
// @symbol _ZN12daObjClock_cD0Ev
/* Both destructors are emitted from the INLINE `~daObjClock_c() {}` in
 * include/daObjClock_c.h -- there is deliberately no body here.
 *
 * A complete-object destructor stores this class's vtable over the one the base
 * constructor left, destroys members in reverse declaration order, then runs the
 * base subobject destructor; the deleting destructor does that and calls
 * operator delete. All of it follows from `: dActor_c` and the member types, so
 * the compiler writes both bodies and the bytes still reproduce.
 *
 * Defining ~daObjClock_c() out of line HERE instead would break the TU two ways:
 * mwccarm would emit D0 at 0x021113ec's slot before D1 at 0x021113bc's,
 * reversing ROM order so objisolate refuses the entire TU, and it would emit a
 * third symbol, D2, which has no address anywhere in the cartridge. See the
 * vtable comment in the header.
 */
