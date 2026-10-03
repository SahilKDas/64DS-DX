//cpp
/* daObjFl_Amilift_c: a platform that rides a path, ov064 0x02117978..0x02117fb4,
 * 11 functions. It walks between the nodes of a PathPtr, sinks a little while
 * the player stands on it and bobs on a sine table.
 *
 * ROM name from _ZTS17daObjFl_Amilift_c. Vtable _ZTV17daObjFl_Amilift_c
 * at 0x0211bc68. daObjFl_Amilift_c_classInit at 0x02117fe8 and the two
 * functions after InitResources (func_ov064_02117fb4, func_ov064_02117fd4)
 * stay out.
 *
 * STATES: mState indexes a three-entry table of pointers-to-member at
 * data_ov064_0211c750, filled by ov064's __sinit from the constants at
 * 0x0211bc0c, 0x0211bc14 and 0x0211bc1c (the ROM words are, in order,
 * 0x02117c24, 0x02117bdc, 0x02117b8c):
 *
 *     0  func_ov064_02117c24  wait for the player, 20 frames, then walk forward
 *     1  func_ov064_02117bdc  walk forward; at the last node switch to 2
 *     2  func_ov064_02117b8c  walk back after a 20-frame pause; at node 0 go
 *                             to 1 if auto-run, else back to 0
 *
 * #pragma defer_codegen off lays .text down in source order. One out-of-line
 * destructor emits D1 then D0.
 *
 * Known limits:
 * - The three state handlers and their helpers are extern "C" functions named
 *   by address, not daObjFl_Amilift_c members.
 * - dBgW_KcMbg::SetFile, dBgActor_c::IsClsnInRange and cstd::atan2 take
 *   Fix12<int> by value, so they stay mangled calls.
 * - The collider callbacks registered in InitResources
 *   (dBgW::UpdatePosWithTransform and func_ov064_02117fd4) are passed as raw
 *   addresses to func_020393d4 / func_020393c4, which have no header.
 * - Behavior updates mStateFrames and mBobPhase through pointer locals and
 *   re-reads mBobPhase as unsigned; writing both as plain member updates came
 *   out 8 bytes short.
 */

#include "daObjFl_Amilift_c.h"
#include "common.h"
#include "decl_common.h"
#include "SharedFilePtr.h"

/* Lets Behavior call the state table's entries as members of the lift. */
typedef void (daObjFl_Amilift_c::*LiftStateFn)();
struct LiftState {
    LiftStateFn pmf;
};

struct BMD_File;
struct KCL_File;
struct CLPS_Block;

extern "C" {
int Vec3_HorzDist(const void* a, const void* b);
int _Z14ApproachLinearRiii(int* v, int target, int step);
void func_ov064_02117a14(daObjFl_Amilift_c *lift, Vector3* a, Vector3* b);
extern LiftState data_ov064_0211c750[];
extern short data_02082214[];
extern int _ZN10dBgActor_c13IsClsnInRangeE5Fix12IiES1_(void*, int, int);
KCL_File *_ZN7dBgW_Kc8LoadFileER13SharedFilePtr(SharedFilePtr &f);
void _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block( void *self, KCL_File *k, Matrix4x3 *m, int fix, short s, CLPS_Block *clps);
void func_020393d4(void *p, void *v);
void func_020393c4(void *p, void *v);
extern SharedFilePtr data_ov064_0211c730;
extern SharedFilePtr data_ov064_0211c728;
extern CLPS_Block data_ov064_0211bb6c;
extern void _ZN4dBgW22UpdatePosWithTransformERS_P8dActor_cR5dBgPiR7Vector3P10Vector3_16S8_();
extern void Vec3_Sub(Vector3* dst, Vector3* a, Vector3* b);
extern int _ZN4cstd5atan2E5Fix12IiES1_(int y, int x);
}

/* Emission order is ROM order. Do not reorder. */
#pragma defer_codegen off

// @symbol _ZN17daObjFl_Amilift_cD1Ev
// @symbol _ZN17daObjFl_Amilift_cD0Ev
daObjFl_Amilift_c::~daObjFl_Amilift_c()
{
}

// @symbol func_ov064_02117a14
/* Turn the lift to face from node b toward node a. */
extern "C" {
void func_ov064_02117a14(daObjFl_Amilift_c *lift, Vector3* a, Vector3* b){
  Vector3 v;
  Vec3_Sub(&v, a, b);
  lift->mPrevAngleY = (short)_ZN4cstd5atan2E5Fix12IiES1_(v.x, v.z);
}
}

// @symbol func_ov064_02117a44
/* Advance along the path by one frame. Speeds up toward 0xa000 and slows to a
 * stop within 0x69000 of the next node; once stopped there it snaps onto the
 * node, steps mNodeIndex one way or the other (per mState) and turns toward
 * the following node. Returns -1 when it has just reached either end of the
 * path (the index is then pulled back inside the path), else 0 or 1. */
extern "C" int func_ov064_02117a44(daObjFl_Amilift_c *lift) {
  Vector3 a;
  Vector3 b;
  int r;
  lift->UpdatePos(0);
  r = (Vec3_HorzDist(&lift->mPosX, &lift->mToX) < 0x69000) ? 0 : 0xa000;
  if (_Z14ApproachLinearRiii(&lift->mHorzSpeed, r, 0x800) != 0 && r == 0) {
    lift->mFromX = lift->mToX;
    lift->mFromY = lift->mToY;
    lift->mFromZ = lift->mToZ;
    lift->mPosX = lift->mFromX;
    lift->mPosY = lift->mFromY;
    lift->mPosZ = lift->mFromZ;
    r = 1;
    if (lift->mState == daObjFl_Amilift_c::STATE_FORWARD) {
      ++lift->mNodeIndex;
      if (lift->mNodeIndex >= lift->mNodeCount) {
        lift->mNodeIndex = lift->mNodeCount - 2;
        r = -1;
      }
    } else {
      --lift->mNodeIndex;
      if (lift->mNodeIndex < 0) {
        lift->mNodeIndex = r;
        r = -1;
      }
    }
    lift->mPathPtr.GetNode(*(Vector3 *)&lift->mToX, lift->mNodeIndex);
    a.x = lift->mToX;
    a.y = lift->mToY;
    a.z = lift->mToZ;
    b.x = lift->mFromX;
    b.y = lift->mFromY;
    b.z = lift->mFromZ;
    func_ov064_02117a14(lift, &a, &b);
    return r;
  }
  return 0;
}

// @symbol func_ov064_02117b8c
/* State 2, walking back: pause 20 frames, then walk. On reaching node 0 go to
 * the forward state if auto-run, else wait for the player. */
extern "C" {
void func_ov064_02117b8c(void *c) {
    daObjFl_Amilift_c *lift = (daObjFl_Amilift_c *)c;
    unsigned short v = lift->mStateFrames;
    if (v < 0x14) return;
    int r = func_ov064_02117a44(lift);
    if (r != -1) return;
    unsigned char b = lift->mAutoRun;
    if (b == 0) {
        lift->mState = daObjFl_Amilift_c::STATE_WAIT;
    } else {
        lift->mState = daObjFl_Amilift_c::STATE_FORWARD;
    }
}
}

// @symbol func_ov064_02117bdc
/* State 1, walking forward: an auto-run lift pauses 20 frames first. On
 * reaching the last node go to the backward state. */
extern "C" {
int func_ov064_02117bdc(void *c)
{
    daObjFl_Amilift_c *lift = (daObjFl_Amilift_c *)c;
    unsigned char b = lift->mAutoRun;
    if (b == 1) {
        unsigned short v = lift->mStateFrames;
        if (v < 0x14) return v;
    }
    int r = func_ov064_02117a44(lift);
    if (r == -1) {
        r = 2;
        lift->mState = r;
    }
    return r;
}
}

// @symbol func_ov064_02117c24
/* State 0, waiting: while the player is on the lift, count frames (Behavior
 * does the counting); once more than 20 have counted, or at once if auto-run, turn toward
 * the next node and start walking forward. With nobody on it the counter
 * is held at 0. */
extern "C" {
void func_ov064_02117c24(daObjFl_Amilift_c *lift)
{
    if (lift->mRiderOn != 0) {
        if (lift->mAutoRun != 1) {
            if (lift->mStateFrames <= 0x14)
                return;
        }
        lift->mState = daObjFl_Amilift_c::STATE_FORWARD;
        struct Vector3 v0, v1;
        v0.x = lift->mToX;
        v0.y = lift->mToY;
        v0.z = lift->mToZ;
        v1.x = lift->mFromX;
        v1.y = lift->mFromY;
        v1.z = lift->mFromZ;
        func_ov064_02117a14(lift, &v0, &v1);
        return;
    }
    lift->mStateFrames = 0;
}
}

// @symbol _ZN17daObjFl_Amilift_c16CleanupResourcesEv
s32 daObjFl_Amilift_c::CleanupResources() {
    mMeshCollider.Disable();
    data_ov064_0211c730.Release();
    data_ov064_0211c728.Release();
    return 1;
}

// @symbol _ZN17daObjFl_Amilift_c6RenderEv
s32 daObjFl_Amilift_c::Render() {
    mModel.Render(0);
    return 1;
}

// @symbol _ZN17daObjFl_Amilift_c8BehaviorEv
/* Run the state handler, count frames (restarting the count when the state
 * changed), then set the height: the spawn height plus the sink offset (eased
 * to -0x28000 while the player is on it) plus a sine bob, applied only for
 * the model and collider update and restored afterwards. */
s32 daObjFl_Amilift_c::Behavior() {
    int idx = mState;
    (this->*data_ov064_0211c750[idx].pmf)();
    unsigned short* p338 = &mStateFrames;
    *p338 = (unsigned short)(*p338 + 1);
    if (idx != mState) {
        mStateFrames = 0;
    }
    int target = mRiderOn ? -0x28000 : 0;
    if (_Z14ApproachLinearRiii(&mSinkOffset, target, 0x5000)) {
        short* pAng = &mBobPhase;
        *pAng = (short)(*pAng + 0xa00);
        unsigned short h = *(unsigned short *)&mBobPhase;
        short s = data_02082214[(h >> 4) * 2];
        short ten = 10;
        mBobOffset = s * ten;
    }
    int t330 = mHomeY;
    int t320 = mSinkOffset;
    int t324 = mBobOffset;
    int saved = mPosY;
    mPosY = t324 + (t330 + t320);
    UpdateModelPosAndRotY();
    if (_ZN10dBgActor_c13IsClsnInRangeE5Fix12IiES1_(this, 0, 0))
        UpdateClsnPosAndRot();
    mPosY = saved;
    mRiderOn = 0;
    return 1;
}

// @symbol _ZN17daObjFl_Amilift_c13InitResourcesEv
s32 daObjFl_Amilift_c::InitResources() {
    BMD_File *bmd;
    KCL_File *kcl;
    Vector3 nodeA;
    Vector3 nodeB;
    unsigned char b;

    bmd = (BMD_File *)Model::LoadFile(data_ov064_0211c730);
    mModel.SetFile(bmd, 1, -1);

    b = (param1 >> 8) & 1;
    mAutoRun = b;
    b = mAutoRun;
    if (b == 0)
        mState = STATE_WAIT;
    else
        mState = STATE_FORWARD;

    UpdateModelPosAndRotY();
    UpdateClsnPosAndRot();

    kcl = _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(data_ov064_0211c728);
    _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
        &mMeshCollider, kcl, &mClsnMat, 0x199, mAngleY, &data_ov064_0211bb6c);

    func_020393d4(&mMeshCollider, (void *)&_ZN4dBgW22UpdatePosWithTransformERS_P8dActor_cR5dBgPiR7Vector3P10Vector3_16S8_);
    func_020393c4(&mMeshCollider, (void *)&func_ov064_02117fd4);

    mPathPtr.FromID(param1 & 0xf);
    mNodeCount = mPathPtr.NumNodes();
    mNodeIndex = 1;

    mFromX = mPosX;
    mFromY = mPosY;
    mFromZ = mPosZ;

    mPathPtr.GetNode(*(Vector3 *)&mToX, mNodeIndex);

    nodeA.x = mToX;
    nodeA.y = mToY;
    nodeA.z = mToZ;
    nodeB.x = mFromX;
    nodeB.y = mFromY;
    nodeB.z = mFromZ;

    func_ov064_02117a14(this, &nodeA, &nodeB);

    mHomeX = mPosX;
    mHomeY = mPosY;
    mHomeZ = mPosZ;

    return 1;
}
