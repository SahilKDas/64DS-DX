#ifndef DAOBJPOWERUPITEM_C_H
#define DAOBJPOWERUPITEM_C_H

/* RECONSTRUCTED NAMES USED IN THIS HEADER. SM64DS RTTI names the
 * implementation(s) below; the registry profile object and the factory
 * spelling are Tier B reconstructions -- evidence-bounded proposals, not
 * recovered SM64DS symbols. Exact original spellings are not preserved.
 *
 *   daObjPowerUpItem_c -- daObjPowerUpItem_c_classInit (was PowerFlower_Spawn), g_profile_POWER_UP_ITEM (was PowerFlower_SpawnInfo)
 */
#include "types.h"
#include "dActor_c.h"
#include "Model.h"
#include "ShadowModel.h"
#include "dCcAc_c.h"
#include "dBgCh_Actr.h"

/* TWO WITNESSES:
 *
 *   daObjPowerUpItem_c_classInit  fBase_c::operator new(972 = 0x3cc),
 *       dActor_c::dActor_c(), stores _ZTV18daObjPowerUpItem_c, then the five
 *       members below in this order.
 *   _ZN18daObjPowerUpItem_cD0Ev  the same five members destroyed in reverse,
 *       then ~dActor_c.
 *
 * SIZE 0x3cc is the factory's own literal; the two bytes mLifeTimer (0x3ca) and
 * mWobbleTimer (0x3cb) close exactly on it under 4-byte alignment.
 *
 * Everything below 0x0d0 duplicated dActor_c's own fields under placeholder
 * names -- dActor_c ends at exactly 0x0d0. Consumer fields were repointed to
 * the inherited dActor_c/fBase_c names: mParam -> param1, unk_08e ->
 * mAngleY, unk_09c -> mVertAccel, unk_0a0 -> mTerminalVelocity, unk_0b0 ->
 * mFlags (mPosX/Y/Z and mScaleX/Y/Z already shared dActor_c's names).
 *
 * mShadowModel was mistyped `u8` at 0x174 in the generated header --
 * daObjPowerUpItem_c_classInit calls _ZN11ShadowModelC1Ev at that offset, so it is the
 * real 0x28-byte member (0x174..0x19c). The 0x30 bytes at 0x19c..0x1cc are
 * the shadow matrix: func_ov002_020b993c copies mOpenModel's matrix there and
 * overwrites its Y translation with mGroundY >> 3.
 *
 * THE VTABLE was diffed slot by slot against _ZTV8dActor_c. daObjPowerUpItem_c
 * overrides slot 0 (InitResources), slot 3 (CleanupResources), slot 6
 * (Behavior) and slot 9 (Render) -- all still fBase_c's own slots in
 * dActor_c -- plus slot 18 (OnYoshiTryEat). Every other slot holds the
 * base's own word and is inherited, so it is deliberately not redeclared
 * here.
 */
struct daObjPowerUpItem_c : dActor_c {
    u8  pad_0d0[0x4];
    /* Model member, named by _ZN5ModelD1Ev at +0xd4 -- a relocation the ROM build checks. */
    Model mCloseModel;            /* 0x0d4 -- SetFile'd from gPFlowerCloseModelFile */
    /* Model member, named by the class's own destructor calling
       Model's D1 at +0x124. [_ZN18daObjPowerUpItem_cD0Ev.c] */
    Model mOpenModel;            /* 0x124 -- SetFile'd from gPFlowerOpenModelFile */
    /* ShadowModel member, named by daObjPowerUpItem_c_classInit's own C1 call and the
       class's own destructor's D1 call at +0x174.
       [d_a_obj_power_up_item.c, _ZN18daObjPowerUpItem_cD0Ev.c] */
    ShadowModel mShadowModel;            /* 0x174 */
    Matrix4x3 mShadowMat;            /* 0x19c -- the open model's matrix, with Y set from mGroundY */
    /* dCcAc_c member, named by the class's own destructor calling
       dCcAc_c's D1 at +0x1cc. [_ZN18daObjPowerUpItem_cD0Ev.c] */
    dCcAc_c mdCcAc_c;            /* 0x1cc */
    /* dBgCh_Actr member, named by the class's own destructor calling
       dBgCh_Actr's D1 at +0x200. [_ZN18daObjPowerUpItem_cD0Ev.c] */
    dBgCh_Actr mWithMeshClsn;            /* 0x200 */
    /* The ground height under the flower: InitResources raycasts a dBgCh_Gnd
       from (mPos with Y + 0x14000) and stores the hit height (+0x44 of the
       ground object), falling back to that probe Y when nothing is hit.
       [_ZN18daObjPowerUpItem_c13InitResourcesEv.cpp] */
    s32 mGroundY;            /* 0x3bc */
    /* Render switches on it to pick which model to draw: 0 -> mCloseModel,
       1 and 2 -> mOpenModel. [_ZN18daObjPowerUpItem_c6RenderEv.cpp] */
    s32 mState;            /* 0x3c0 */
    /* Particle effect handle: passed back into Particle::System::New as its
       first argument each frame, cleared to 0 when a state ends. */
    u32 mEffectHandle;            /* 0x3c4 */
    /* Angle stepped by 0x2000 per frame while the flower is popping; its top
       twelve bits index the s16 table data_02082214 that scales mScaleX/Y. */
    u16 mWobbleAngle;            /* 0x3c8 */
    /* Seeded 0xb4 (180 frames, three seconds) in InitResources. Render skips
       drawing on odd values once it is below 0x2d, so the flower blinks through
       its last 45 frames -- the standard "about to disappear" tell.
       [_ZN18daObjPowerUpItem_c13InitResourcesEv.cpp, _ZN18daObjPowerUpItem_c6RenderEv.cpp] */
    u8  mLifeTimer;            /* 0x3ca */
    /* Set to 0x1b when the pop starts; func_ov002_020b92c4 counts it down. */
    u8  mWobbleTimer;            /* 0x3cb */

    virtual ~daObjPowerUpItem_c();            /* slots 16 (D1), 17 (D0) */

    virtual s32  InitResources();         /* slot  0 */
    virtual s32  CleanupResources();      /* slot  3 */
    virtual s32  Behavior();         /* slot  6 */
    virtual s32  Render();           /* slot  9 */
    virtual s32  OnYoshiTryEat();         /* slot 18 */
};

#ifndef SM64DS_PLATFORM_PC
/* ROM layout under mwccarm; host ABI divergence is tracked separately. */
typedef char daObjPowerUpItem_c_size_must_be_0x3cc[sizeof(daObjPowerUpItem_c) == 0x3cc ? 1 : -1];
#endif

#endif
