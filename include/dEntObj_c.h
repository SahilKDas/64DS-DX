/* The multiplayer entry scene object. Its two ROM destructor variants reveal
 * the complete ownership chain: dBase_c, Particle::SysTracker, Model,
 * ModelAnim, and four 0x158-byte player records. The record constructor and
 * destructor were previously anonymous address symbols; their four member
 * constructor/destructor calls prove the typed prefix below, while the array
 * stride proves its trailing extent.
 *
 * The class name is the ROM's own: _ZTS9dEntObj_c at ov075:0x0211c648, with
 * _ZTI9dEntObj_c at 0x0211c66c and the vtable _ZTV9dEntObj_c at 0x0211c6a0.
 * UnknownVsPlayer has no vtable and no RTTI, so the cartridge gives it no
 * name and the project's name stays. */
#ifndef DENTOBJ_C_H
#define DENTOBJ_C_H
#include "types.h"

#ifdef __cplusplus

#include "dBase_c.h"
#include "Particle__SysTracker.h"
#include "Model.h"
#include "ModelAnim.h"
#include "BlendModelAnim.h"
#include "TextureSequence.h"
#include "ShadowModel.h"

/* One figure on the entry stage. mMoveState and mAnimState index the two
 * pointer-to-member state tables in src/actors/dEntObj_c.cpp; the fields
 * after them are named from what those states read and write. */
struct UnknownVsPlayer {
    BlendModelAnim mModel;             /* 0x000 */
    ModelAnim mAnimation;              /* 0x070 */
    TextureSequence mTextureSequence;  /* 0x0d4 */
    ShadowModel mShadow;               /* 0x0e8 */
    s32 mMoveState;                    /* 0x110 */
    s32 mAnimState;                    /* 0x114 */
    Vector3 mPosition;                 /* 0x118 */
    Vector3 mTargetPos;                /* 0x124 - walk target */
    Vector3 mExitPos;                  /* 0x130 - jump-off target */
    s32 mSpeed;                        /* 0x13c */
    s32 mMaxSpeed;                     /* 0x140 */
    s32 mVertSpeed;                    /* 0x144 */
    s32 mMaterialColor;                /* 0x148 - written to +0x20 of every material each
                                          frame; materials[0]'s value + 2 * mPlayerNo */
    u32 mParticleHandle;               /* 0x14c */
    s16 mAngleY;                       /* 0x150 */
    u8 mPlayerNo;                      /* 0x152 - slot in dEntObj_c::mPlayers */
    u8 mInAir;                         /* 0x153 - also a once-flag in movement state 8 */
    u8 mFellOff;                       /* 0x154 */
    u8 unk_155;                        /* 0x155 - set the frame it falls off */
    u8 mWaitTimer;                     /* 0x156 */
    u8 pad_157;

    UnknownVsPlayer();
    ~UnknownVsPlayer();
};

#ifndef SM64DS_PLATFORM_PC
/* ROM layout under mwccarm; host ABI divergence is tracked separately. */
typedef char UnknownVsPlayer_size_must_be_0x158[
    sizeof(UnknownVsPlayer) == 0x158 ? 1 : -1];
#endif

struct dEntObj_c : dBase_c {
    Particle::SysTracker mParticles;   /* 0x050 */
    Model mModel;                      /* 0x86c */
    ModelAnim mModelAnim;              /* 0x8bc */
    UnknownVsPlayer mPlayers[4];       /* 0x920 */
    u8 unk_e80;                        /* 0xe80 */
    u8 pad_e81[0xa7];
    s32 mCamPosX;                      /* 0xf28 */
    s32 mCamPosY;                      /* 0xf2c */
    s32 mCamPosZ;                      /* 0xf30 */
    s32 mCamTargetX;                   /* 0xf34 */
    s32 mCamTargetY;                   /* 0xf38 */
    s32 mCamTargetZ;                   /* 0xf3c */
    u8 mAnimActive;                    /* 0xf40 */
    u8 mState;                         /* 0xf41 */
    u8 mFocusedPlayer;                 /* 0xf42 */
    u8 mPlayerCount;                   /* 0xf43 */
    u8 mSuspended;                     /* 0xf44 */

    virtual ~dEntObj_c();
    virtual int InitResources();
    virtual int CleanupResources();
    virtual int Behavior();
    virtual int Render();
};

#ifndef SM64DS_PLATFORM_PC
/* ROM layout under mwccarm; host ABI divergence is tracked separately. */
typedef char dEntObj_c_size_must_be_0xf48[sizeof(dEntObj_c) == 0xf48 ? 1 : -1];
#endif

#endif /* __cplusplus */

#endif
