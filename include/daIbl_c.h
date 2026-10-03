#ifndef DAIBL_C_H
#define DAIBL_C_H

#include "types.h"

/* Derives from dEnemyBase_c, and TWO INDEPENDENT WITNESSES agree on the layout:
 * the class's own destructor `_ZN7daIbl_cD1Ev` destroys each member, and
 * `daIbl_c_classInit` constructs the same types at the same offsets before
 * storing `_ZTV7daIbl_c`. Everything this header used to restate below
 * 0x110 belongs to dEnemyBase_c and dActor_c and is inherited now.
 *
 * The members close on each other, which is what makes the layout a
 * reading rather than a guess:
 *
 *     0x110 dBgCh_Actr               0x1bc   -> 0x2cc
 *     0x2cc Model                      0x50    -> 0x31c
 *     0x31c ShadowModel                0x28    -> 0x344
 *     0x374 dCcAc_c         0x34    -> 0x3a8
 *     0x3f4 PathPtr                    0x8     -> 0x3fc
 *
 * THE FIFTH MEMBER HAS NO DESTRUCTOR, so only the factory witnesses it:
 * daIbl_c_classInit constructs a PathPtr at 0x3f4 that the destructor
 * never destroys, PathPtr's being trivial. 0x3f4 + 8 is 0x3fc, exactly the
 * allocation literal -- the layout does not close without it. Reading only
 * the destructor leaves the class eight bytes short.
 *
 * Every function of the class reproduces, in one translation unit:
 * src/actors/daIbl_c.cpp is the whole linker unit 0x02141f04..0x021431c4,
 * factory included.
 *
 * SIZE IS THE ROM'S OWN: `daIbl_c_classInit` calls
 * `fBase_c::operator new(1020)` -- 0x3fc -- and stores this class's
 * vtable, so that literal IS this class's sizeof.
 *
 * SM64DS RTTI names the implementation daIbl_c: _ZTS7daIbl_c at
 * 0x02147f40, _ZTI7daIbl_c at 0x02147f4c, and the vtable _ZTV7daIbl_c at
 * 0x02147f7c that the factory and destructor store. The reconstructed
 * factory daIbl_c_classInit (historical alias
 * RollingIronBall_Spawn) constructs it for the IRONBALL
 * registry profile.
 */

#include "dEnemyBase_c.h"
#include "Model.h"
#include "dCcAc_c.h"
#include "PathPtr.h"
#include "ShadowModel.h"
#include "dBgCh_Actr.h"

struct daIbl_c : dEnemyBase_c {
    dBgCh_Actr                 mWithMeshClsn;         /* 0x110 */
    Model                        mModel;                /* 0x2cc */
    ShadowModel                  mShadowModel;          /* 0x31c */
    u8  pad_344[0x30];
    dCcAc_c           mdCcAc_c;   /* 0x374 */
    s32                          unk_3a8;               /* 0x3a8 -- zeroed by InitResources; no reader */
    /* The scale Render hands to the model: it passes &mDrawScaleX as the scale
       argument, where the rest of this family passes dActor_c's own &mScaleX.
       InitResources writes 0x1000 (1.0) to all three, or 0x800 (0.5) in the one
       level that uses the small ball. */
    s32                          mDrawScaleX;           /* 0x3ac */
    s32                          mDrawScaleY;           /* 0x3b0 */
    s32                          mDrawScaleZ;           /* 0x3b4 */
    u8  pad_3b8[0x2];
    s16                          unk_3ba;               /* 0x3ba -- an angle: func_ov100_0214233c
                                                            fills it and InitResources copies it
                                                            straight into mPrevAngleY */
    u8  pad_3bc[0x4];
    s32                          unk_3c0;               /* 0x3c0 -- per-level distance, kind 0 only */
    s32                          unk_3c4;               /* 0x3c4 -- per-level distance, kind 0 only */
    s32                          unk_3c8;               /* 0x3c8 -- per-level, seeded from data_02092138 */
    s32                          unk_3cc;               /* 0x3cc -- zeroed by InitResources */
    /* param1's low nibble, consumed immediately (param1 is then shifted down by
       four so the next nibble is the path ID). InitResources switches on it --
       0 is the static ball, 1 the free-rolling one, 2 and 4 the path followers --
       Behavior indexes its handler table with it, and Render skips kind 0. */
    u8                           mVariant;              /* 0x3d0 */
    u8  pad_3d1[0x1];
    u8                           unk_3d2;               /* 0x3d2 -- zeroed by InitResources */
    u8  pad_3d3[0x1];
    s32                          mNumPathNodes;         /* 0x3d4 -- PathPtr::NumNodes() */
    s32                          mPathNodeIndex;        /* 0x3d8 -- index passed to PathPtr::GetNode */
    s32                          mSpawnPosX;            /* 0x3dc */
    s32                          mSpawnPosY;            /* 0x3e0 */
    s32                          mSpawnPosZ;            /* 0x3e4 */
    /* Where PathPtr::GetNode writes the node it was asked for; InitResources
       compares it against the actor's own position to decide whether to skip
       ahead one node. */
    s32                          mNextNodePosX;         /* 0x3e8 */
    s32                          mNextNodePosY;         /* 0x3ec */
    s32                          mNextNodePosZ;         /* 0x3f0 */
    PathPtr                      mPathPtr;              /* 0x3f4 */

    /* --- vtable --- */
    virtual ~daIbl_c();

    virtual s32   OnAimedAtWithEgg();      /* slot 29 */

    int Behavior();
    int CleanupResources();
    int InitResources();
    int Render();
};

#ifndef SM64DS_PLATFORM_PC
/* ROM layout under mwccarm; host ABI divergence is tracked separately. */
typedef char daIbl_c_size_must_be_0x3fc[sizeof(daIbl_c) == 0x3fc ? 1 : -1];
#endif

#endif /* DAIBL_C_H */
