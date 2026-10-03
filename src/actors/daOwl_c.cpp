//cpp
/* The owl actor, daOwl_c (registry profile OWL). It waits unrendered until the
 * player comes near, then hovers around an anchor point, talks to the player
 * (message 0xa2) and, once the talk is over, can pick the player up and carry
 * it.
 * 22 functions, .text 0x02135700..0x021367e8.
 *
 * NAME: _ZTS7daOwl_c is "7daOwl_c" at ov094 0x02136a10; _ZTI at 0x02136a28
 * reads [__si_class_type_info, that string, _ZTI12dEnemyBase_c]. The vtable's
 * address point is 0x02136a58, and slots 16/17 hold D1/D0 below. The tree
 * previously called the class HootTheOwl (coined; vtable address only).
 *
 * STATES: the actor's state is a pointer to a pair of pointers-to-member
 * (enter, main). ov094's __sinit builds five of them from constants in .data.
 * Names are read off what each main handler does:
 *
 *     0x02136b40  dormant   invisible (Render skips it); waits for the player
 *     0x02136b60  hover     bobs at the anchor, wanders, starts the talk
 *     0x02136b50  talk      faces the player until the message ends
 *     0x02136b70  carry     pinned to the rider's matrix
 *     0x02136b30  return    fades out, reappears above the anchor, fades in
 *
 * The out-of-line destructor is the key function, so this TU emits _ZTV/_ZTI/
 * _ZTS. Under `#pragma defer_codegen off` it comes out D1 (0x02135700), D0
 * (0x02135748), then a D2 the cartridge has no home for (manifest: deadstrip);
 * the same pragma lays .text down in source order, so this file is ROM-ascending.
 * daOwl_c_classInit (0x02136798..0x021367e8, historical alias
 * HootTheOwl_Spawn) now appends after InitResources, at the end of source
 * order:
 * fBase_c::operator new(size_t) forwards `return new daOwl_c();` to the same
 * _ZN7fBase_cnwEj(1016) allocator the loose factory called by hand, and
 * daOwl_c has no user-declared constructor, so the inherited dEnemyBase_c
 * ctor plus the vtable store plus the four member subobjects in field order
 * (dCcAcPos_c, dBgCh_Actr, ModelAnim, ShadowModel) come from the implicit
 * default constructor with zero mangled calls.
 *
 * Known limits:
 * - The state handlers are extern "C" functions named by address that take the
 *   actor as an untyped pointer; they are not yet daOwl_c members. The header
 *   comment on each says which state it belongs to.
 * - ModelAnim::SetAnim, dCcAcPos_c::Init, dBgCh_Actr::Init and
 *   dActor_c::DropShadowRadHeight take Fix12<int> by value, so they stay
 *   mangled calls; Sound::PlayLong and dBgCh_Actr::Unk_0203589c have no member
 *   declaration in a shared header yet.
 * - unk_3e8 is scratch that each state uses differently: a fade phase (return),
 *   a sound latch (carry) and a bobbing phase (hover). It is left unnamed.
 * - The rider's matrix pointer (+0xc8) and the model's own matrix (0x328) and
 *   bone-matrix pointer (0x320) are reached at raw offsets: no header names
 *   them. Written as offsets into mModelAnim the compiler hoists the base and
 *   the bytes change, so they stay offsets from the actor.
 * - State-timer reads use a u16 cast because the ROM loads mStateTimer
 *   unsigned; the member is s16.
 * - Two spellings look redundant but are measured: the actor-id test in
 *   func_ov094_021357a4 goes through a local (`t`, `eq`), and the three dead
 *   stores to `d` in func_ov094_02136024 stay; dropping either changes the
 *   bytes.
 * - RandomIntInternal stays unsigned so func_ov094_02135e64 keeps its logical
 *   shift, and Behavior's double test of the carry state is what the ROM does.
 */

#pragma defer_codegen off

#include "decl_common.h"
#include "daOwl_c.h"
#include "SharedFilePtr.h"
#include "Player.h"

/* decl_common.h includes common.h, so Matrix4x3 is the flat s32 m[12]
 * spelling. math/Matrix.h (via daOwl_c.h -> ModelAnim.h) stands down. */
typedef char Matrix4x3_size_must_be_0x30[sizeof(Matrix4x3) == 0x30 ? 1 : -1];

struct OwlVec {
    int x, y, z;
};
typedef char OwlVec_size_must_be_0x0c[sizeof(OwlVec) == 0x0c ? 1 : -1];

struct V6 {
    int v[6];
};
typedef char V6_size_must_be_0x18[sizeof(V6) == 0x18 ? 1 : -1];

enum {
    ACTOR_PLAYER = 0xbf,
    OPACITY_MAX = 0x1f,
    TALK_IDLE = 0,
    TALK_TALKING = 1,
    TALK_DONE = 2
};

#define OWL_STATE(sym)      ((daOwl_c::State *)(sym))
#define OWL_STATE_DORMANT   OWL_STATE(data_ov094_02136b40)
#define OWL_STATE_HOVER     OWL_STATE(data_ov094_02136b60)
#define OWL_STATE_TALK      OWL_STATE(data_ov094_02136b50)
#define OWL_STATE_CARRY     OWL_STATE(data_ov094_02136b70)
#define OWL_STATE_RETURN    OWL_STATE(data_ov094_02136b30)

/* mStateTimer is s16 but every read in the ROM is an unsigned halfword load. */
#define OWL_TIMER(o)        (*(u16 *)&(o)->mStateTimer)

/* Whole animation frame of the ModelAnim, as the ROM extracts it. */
#define OWL_FRAME(o)        ((u16)((o)->mModelAnim.currFrame >> 12))

extern "C" {
extern char data_ov094_02136b40[];
extern char data_ov094_02136b60[];
extern signed char data_0209f2f8;
extern unsigned char data_0209f220;
extern short data_02082214[];
extern int data_0209e650[];
extern Matrix4x3 data_020a0e68;

int func_ov094_02136188(daOwl_c *owl, daOwl_c::State *state);
void func_ov094_021357a4(char *c);

int func_ov002_020df840(void *a, void *b, void *d);
void _Z14ApproachLinearR7Vector3RKS_5Fix12IiE(Vector3 *a, const Vector3 *b, Fix12i f);
int _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(void *self, void *bca, int a, int fix, unsigned int b);
int _ZN5Sound8PlayLongEjjjRK7Vector3s(unsigned int a, unsigned int b, unsigned int cc, void *pos, unsigned int d);
int func_ov002_020df7f4(void *c);
int func_ov002_020df7ac(void *thiz);
void _ZN10dBgCh_Actr12Unk_0203589cEv(void *self);
int func_02012694(int a, void *pos);
int ApproachAngle(short *p, int target, int a, int b, int c);
int Vec3_Dist(const void *a, const void *b);
short Vec3_HorzAngle(const void *a, const void *b);
short Vec3_VertAngle(const void *a, const void *b);
unsigned int RandomIntInternal(void *seed);
void _Z14ApproachLinearRiii(int *x, int target, int step);
void Matrix4x3_FromRotationY(void *m, int angle);
void MulVec3Mat4x3(void *dst, void *mat, void *src);
void Vec3_Sub(OwlVec *out, OwlVec *a, OwlVec *b);
int LenVec3(OwlVec *v);
void Vec3_Asr(Vector3 *d, Vector3 *s, int sh);
void Matrix4x3_FromTranslation(Matrix4x3 *m, Fix12i x, Fix12i y, Fix12i z);
void Matrix4x3_ApplyInPlaceToRotationXYZExt(void *m, int x, int y, int z);
void Matrix4x3_ApplyInPlaceToTranslation(Matrix4x3 *m, Fix12i x, Fix12i y, Fix12i z);
void MulMat4x3Mat4x3(const int *a, const int *b, int *dst);
void _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(
    void *self, void *sm, Matrix4x3 *m, Fix12i fx, int t, unsigned int u);
void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
    void *, void *, int *, int, int, unsigned int, unsigned int);
void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
    void *, void *, int, int, void *, int);
int IsStarCollectedInCurLevel(int);
void DecIfAbove0_Short(void *);
}

// @symbol _ZN7daOwl_cD1Ev
// @symbol _ZN7daOwl_cD0Ev
/* The whole body is compiler-emitted: one vptr store, then ShadowModel
 * (0x370), ModelAnim (0x30c), dBgCh_Actr (0x150) and dCcAcPos_c (0x110) in
 * reverse declaration order, then dEnemyBase_c::~dEnemyBase_c. D1 is the
 * evidence for the header's four member types; D0 adds dEnemyBase_c's inline
 * operator delete. */
daOwl_c::~daOwl_c()
{
}

// @symbol func_ov094_021357a4
/* Hover, once the talk is over: if the player is touching the owl, grab it.
 * The contact owner is the actor id in the hit-box; only the player (0xbf)
 * counts. Attaches the player, zeroes the owl's velocity and enters the carry
 * state; if the attach is refused the rider is dropped again. */
extern "C" void func_ov094_021357a4(char *c)
{
    daOwl_c *owl = (daOwl_c *)c;
    int st[3];
    OwlVec tmp;
    tmp.x = data_ov094_02136a1c[0];
    tmp.y = data_ov094_02136a1c[1];
    tmp.z = data_ov094_02136a1c[2];
    owl->mdCcAcPos_c.SetPosRelativeToActor(*(Vector3 *)&tmp);
    if (owl->mdCcAcPos_c.otherOwner == 0)
        return;
    dActor_c *other = dActor_c::FindWithID(owl->mdCcAcPos_c.otherOwner);
    if (other == 0)
        return;
    int t = other->actorID;
    unsigned eq = (t == ACTOR_PLAYER);
    if (!eq)
        return;
    owl->mRider = (Player *)other;
    st[0] = 0;
    st[1] = 0;
    st[2] = 0;
    st[1] = 0x1838000;
    if (data_0209f2f8 == 0x16)
        st[1] = 0x1194000;
    owl->unk_0a4 = 0;
    owl->mVertSpeed = 0;
    owl->unk_0ac = 0;
    if (func_ov002_020df840(owl->mRider, owl, st) != 1) {
        owl->mRider = 0;
        return;
    }
    func_ov094_02136188(owl, OWL_STATE_CARRY);
}

// @symbol func_ov094_021358b4
/* Main of the return state. Phase 0 fades the owl out (with an upward speed);
 * once invisible it jumps to just above the anchor and starts phase 1, which
 * eases it onto the anchor while fading back in, then goes to hover. */
extern "C" int func_ov094_021358b4(void *t)
{
    daOwl_c *owl = (daOwl_c *)t;
    if (owl->unk_3e8 == 0) {
        if (owl->mOpacity != 0) {
            owl->mOpacity--;
            owl->mPrevAngleX = 0;
            owl->mVertSpeed = 0x14000;
        } else {
            owl->unk_3e8 = 1;
            owl->unk_0a4 = 0;
            owl->mVertSpeed = 0;
            owl->unk_0ac = 0;
            owl->mPosX = owl->mAnchorX;
            owl->mPosY = owl->mAnchorY;
            owl->mPosZ = owl->mAnchorZ;
            owl->mPosY += 0x64000;
        }
    } else {
        _Z14ApproachLinearR7Vector3RKS_5Fix12IiE((Vector3 *)&owl->mPosX, (Vector3 *)&owl->mAnchorX, 0x2000);
        if (owl->mOpacity < OPACITY_MAX) {
            owl->mOpacity++;
        } else {
            owl->mOpacity = OPACITY_MAX;
            func_ov094_02136188(owl, OWL_STATE_HOVER);
        }
    }
    return 1;
}

// @symbol func_ov094_0213598c
/* Enter of the return state: restart the fade and play the animation
 * data_ov094_02136af8[1] at double speed. */
extern "C" int func_ov094_0213598c(char *c)
{
    daOwl_c *owl = (daOwl_c *)c;
    owl->unk_3e8 = 0;
    owl->mStateTimer = 0;
    owl->mAnimSpeed = 0x2000;
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&owl->mModelAnim, (void *)data_ov094_02136af8[1], 0, 0x1000, 0);
    return 1;
}

// @symbol func_ov094_021359d8
/* Main of the carry state. Once the 30-frame timer has run out it keeps
 * sound 0x18c playing, plays sound 0x139 once per animation loop, levels the pitch, and
 * hands the rider back (to the return state) when the rider lets go, is lost,
 * or the owl hits a wall. */
extern "C" int func_ov094_021359d8(void *thiz)
{
    daOwl_c *owl = (daOwl_c *)thiz;
    Player *rider;

    if (OWL_TIMER(owl) == 1) {
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&owl->mModelAnim, (void *)data_ov094_02136ae8[1], 0, 0x1000, 0);
        owl->mAnimSpeed = 0x1000;
    }

    if (OWL_TIMER(owl) == 0) {
        owl->mSoundHandle = _ZN5Sound8PlayLongEjjjRK7Vector3s(owl->mSoundHandle, 3, 0x18c, &owl->mCamSpacePosX, 0);
    }

    if (OWL_TIMER(owl) != 0) {
        if (owl->unk_3e8 == 0 && OWL_FRAME(owl) <= 2) {
            func_02012694(0x139, &owl->mCamSpacePosX);
            owl->unk_3e8 = 1;
        } else {
            if (OWL_FRAME(owl) > 2)
                owl->unk_3e8 = 0;
        }
    }

    rider = owl->mRider;
    if (rider != 0 && func_ov002_020df7f4(rider) == 1) {
        OWL_TIMER(owl) = 0xa;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&owl->mModelAnim, (void *)data_ov094_02136af8[1], 0, 0x1000, 0);
    }

    ApproachAngle(&owl->mPrevAngleX, 0, 0xa, 0x200, 0x100);

    if (owl->mWithMeshClsn.IsOnWall() != 0 || func_02035638((u8 *)&owl->mWithMeshClsn) != 0) {
        rider = owl->mRider;
        if (rider != 0 && func_ov002_020df7ac(rider) != 0) {
            _ZN10dBgCh_Actr12Unk_0203589cEv(&owl->mWithMeshClsn);
            owl->mRider = 0;
            owl->unk_3e8 = 0;
            owl->mStateTimer = 0;
            func_ov094_02136188(owl, OWL_STATE_RETURN);
            return 1;
        }
    }

    rider = owl->mRider;
    if (rider == 0) {
        goto cleanup;
    }
    if (rider == 0) {
        goto end;
    }
    if (func_ov002_020df7f4(rider) < 0) {
cleanup:
        owl->mRider = 0;
        owl->unk_3e8 = 0;
        owl->mStateTimer = 0;
        func_ov094_02136188(owl, OWL_STATE_RETURN);
    }
end:
    return 1;
}

// @symbol func_ov094_02135bd4
/* Enter of the carry state: 30-frame timer, animation data_ov094_02136af8[1] at double speed. */
extern "C" int func_ov094_02135bd4(void *c)
{
    daOwl_c *owl = (daOwl_c *)c;
    owl->mStateTimer = 0x1e;
    owl->unk_3e8 = 0;
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&owl->mModelAnim, (void *)data_ov094_02136af8[1], 0, 0x1000, 0);
    owl->mAnimSpeed = 0x2000;
    return 1;
}

// @symbol func_ov094_02135c28
/* Main of the hover state. Until a conversation has happened it tries to
 * start one with the player (message 0xa2) and, if that takes, enters the talk
 * state. Otherwise it wanders: when it is too far from the anchor it turns
 * back toward it, else it picks a random heading and duration. It bobs on a
 * sine table around the anchor's height and moves along its heading. */
extern "C" int func_ov094_02135c28(void *thiz)
{
    daOwl_c *owl = (daOwl_c *)thiz;
    int result;
    int ang;
    int *p3e8;
    short tbl;
    int idx;
    V6 buf;

    if (owl->mTalkState == TALK_IDLE) {
        if (owl->ClosestPlayer() != 0) {
            Player *player = owl->mTalkTarget;
            if (player != 0) {
                if ((u16)(player->mStateFlags & 0x800) == 0) {
                    if (player->StartTalk(*owl, 1) != 0) {
                        if (owl->mTalkTarget->ShowMessage(*owl, 0xa2, (const Vector3 *)&owl->mPosX, 0, 0) == 1) {
                            func_02012694(0x176, &owl->mCamSpacePosX);
                            owl->mTalkState = TALK_TALKING;
                            func_ov094_02136188(owl, OWL_STATE_TALK);
                            return 1;
                        }
                    }
                }
            }
        }
    }

    if (Vec3_Dist(&owl->mPosX, &owl->mAnchorX) > 0x190000) {
        owl->mStateTimer = 0x32;
        owl->mWanderAngleY = Vec3_HorzAngle(&owl->mPosX, &owl->mAnchorX);
    } else if (OWL_TIMER(owl) == 0) {
        owl->mWanderAngleY = (s16)(((u32)RandomIntInternal(data_0209e650) >> 8) << 12);
        owl->mStateTimer = (s16)((((u32)RandomIntInternal(data_0209e650) >> 8) & 0x3f) + 0x32);
    }

    ApproachAngle(&owl->mPrevAngleY, owl->mWanderAngleY, 0xa, 0x200, 0x100);
    ApproachAngle(&owl->mPrevAngleX, 0, 0xa, 0x200, 0x100);

    buf.v[0] = 0;
    buf.v[1] = 0;
    buf.v[2] = 0x5000;
    buf.v[3] = 0;
    buf.v[4] = 0;
    buf.v[5] = 0;

    p3e8 = &owl->unk_3e8;
    *p3e8 += 0x200;
    ang = owl->unk_3e8;
    idx = ((u16)(short)ang >> 4) * 2;
    tbl = data_02082214[idx];
    result = (int)(((long long)tbl * 0x64000 + 0x800) >> 12);
    _Z14ApproachLinearRiii(&owl->mPosY, owl->mAnchorY + result, 0x3000);

    Matrix4x3_FromRotationY(&data_020a0e68, owl->mPrevAngleY);
    MulVec3Mat4x3(&buf, &data_020a0e68, &owl->unk_0a4);
    return 1;
}

// @symbol func_ov094_02135e64
/* Enter of the hover state: animation data_ov094_02136af0[1] at normal speed, random heading and a
 * 50..113 frame wander timer. */
extern "C" int func_ov094_02135e64(char *c)
{
    daOwl_c *owl = (daOwl_c *)c;
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&owl->mModelAnim, (void *)data_ov094_02136af0[1], 0, 0x1000, 0);
    owl->mWanderAngleY = (short)((RandomIntInternal(data_0209e650) >> 8) << 0xc);
    owl->mStateTimer = (short)(((RandomIntInternal(data_0209e650) >> 8) & 0x3f) + 0x32);
    owl->mAnimSpeed = 0x1000;
    return 1;
}

// @symbol func_ov094_02135ee0
/* Main of the talk state: turn to face the player (yaw, then pitch toward a
 * point 200 units above its feet). When the player's talk state goes negative
 * the conversation is over; mark it done and return to hover. */
extern "C" int func_ov094_02135ee0(void *self)
{
    daOwl_c *owl = (daOwl_c *)self;
    if (owl->mTalkTarget == 0) {
        func_ov094_02136188(owl, OWL_STATE_HOVER);
        return 1;
    }
    Vector3 v = *(Vector3 *)&owl->mTalkTarget->mPosX;
    Vector3 w;
    w.x = v.x;
    w.y = v.y;
    w.z = v.z;
    ApproachAngle(&owl->mPrevAngleY, Vec3_HorzAngle(&owl->mPosX, &w), 0xa, 0x200, 0x100);
    Vector3 u;
    v.y += 0xc8000;
    u.x = v.x - 0;
    u.y = v.y;
    u.z = v.z;
    ApproachAngle(&owl->mPrevAngleX, Vec3_VertAngle(&owl->mPosX, &u), 0xa, 0x200, 0x100);
    if (owl->mTalkTarget->GetTalkState() < 0) {
        owl->mTalkState = TALK_DONE;
        func_ov094_02136188(owl, OWL_STATE_HOVER);
    }
    return 1;
}

// @symbol func_ov094_02135fe0
/* Enter of the talk state: stop, and play the animation data_ov094_02136af0[1]. */
extern "C" int func_ov094_02135fe0(char *c)
{
    daOwl_c *owl = (daOwl_c *)c;
    owl->unk_0a4 = 0;
    owl->mVertSpeed = 0;
    owl->unk_0ac = 0;
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&owl->mModelAnim, (void *)data_ov094_02136af0[1], 0, 0x1000, 0);
    return 1;
}

// @symbol func_ov094_02136024
/* Main of the dormant state. Watches the nearest player and keeps the owl 400
 * units above that player's ground height. When the player's position is
 * within 40 units of the owl (LenVec3 < 0x28000) it sets the anchor 800 units
 * from the owl along a fixed yaw, faces it, and enters hover.
 */
extern "C" int func_ov094_02136024(char *c)
{
    daOwl_c *owl = (daOwl_c *)c;
    Player *p = owl->ClosestPlayer();
    if (p != 0 && *(int *)((char *)p + 0x37c) != 0) {
        char *ip = (char *)&p->mPosX;
        OwlVec pp;
        OwlVec d;
        OwlVec *selfpos = (OwlVec *)&owl->mPosX;
        pp.x = *(int *)(ip + 0);
        pp.y = *(int *)(ip + 4);
        pp.z = *(int *)(ip + 8);
        owl->mPosY = p->mGroundY + 0x190000;
        owl->mTalkTarget = p;
        Vec3_Sub(&d, selfpos, &pp);
        if (LenVec3(&d) < 0x28000) {
            d.x = owl->mPosX;
            d.y = owl->mPosY;
            d.z = owl->mPosZ;
            d.x = 0;
            d.y = 0;
            d.z = 0x320000;
            Matrix4x3_FromRotationY(&data_020a0e68, 0x4000);
            MulVec3Mat4x3(&d, &data_020a0e68, (OwlVec *)&owl->mAnchorX);
            {
                int *px = &owl->mAnchorX;
                int *py = &owl->mAnchorY;
                int *pz = &owl->mAnchorZ;
                *px = *px + owl->mPosX;
                *py = *py + owl->mPosY;
                *pz = *pz + owl->mPosZ;
            }
            owl->mPrevAngleY = Vec3_HorzAngle((OwlVec *)&owl->mPosX, (OwlVec *)&owl->mAnchorX);
            owl->mAngleY = owl->mPrevAngleY;
            func_ov094_02136188(owl, OWL_STATE_HOVER);
        }
    }
    return 1;
}

// @symbol func_ov094_02136150
/* Enter of the dormant state: animation data_ov094_02136af0[1]. */
extern "C" int func_ov094_02136150(char *c)
{
    daOwl_c *owl = (daOwl_c *)c;
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&owl->mModelAnim, *(void **)((char *)data_ov094_02136af0 + 4), 0, 0x1000, 0);
    return 1;
}

// @symbol func_ov094_02136188
/* Switch state: store it, then run its enter handler if it has one. */
extern "C" int func_ov094_02136188(daOwl_c *owl, daOwl_c::State *state)
{
    owl->mCurrentState = state;
    daOwl_c::State *s = owl->mCurrentState;
    if (s->mEnter == 0)
        return 1;
    return (owl->*(s->mEnter))();
}

// @symbol func_ov094_021361d8
/* Per-frame model setup: world matrix from position (>> 3) and angles, the
 * model's opacity, its shadow matrix, and the drop shadow. */
extern "C" void func_ov094_021361d8(void *raw)
{
    daOwl_c *owl = (daOwl_c *)raw;
    Matrix4x3 out;
    OwlVec v;
    Vec3_Asr((Vector3 *)&v, (Vector3 *)&owl->mPosX, 3);
    Matrix4x3_FromTranslation(&data_020a0e68, v.x, v.y, v.z);
    Matrix4x3_ApplyInPlaceToRotationXYZExt(&data_020a0e68,
        owl->mAngleX, owl->mAngleY, owl->mAngleZ);
    owl->mModelAnim.ApplyOpacity(owl->mOpacity, 1);
    *(Matrix4x3 *)((char *)owl + 0x328) = data_020a0e68;
    MulMat4x3Mat4x3((const int *)(*(char **)((char *)owl + 0x320) + 0x30),
        (const int *)((char *)owl + 0x328), out.m);
    Matrix4x3_FromTranslation(&data_020a0e68,
        owl->mPosX >> 3,
        (owl->mPosY - 0x38000) >> 3,
        owl->mPosZ >> 3);
    owl->mShadowMat = data_020a0e68;
    _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(
        owl, &owl->mShadowModel, &owl->mShadowMat, 0x64000, 0x320000, 0xf);
}

// @symbol func_ov094_021362e0
/* Carry state's placement: take the rider's matrix (pointer at rider + 0xc8),
 * offset and rotate it, and use the result as the owl's own position, yaw and
 * model matrix. */
extern "C" void func_ov094_021362e0(void *raw)
{
    daOwl_c *owl = (daOwl_c *)raw;
    OwlVec v;
    Player *m;
    int z = 0;
    if (owl->mRider == 0)
        return;
    *(volatile Fix12i *)&v.x = z;
    *(volatile Fix12i *)&v.y = z;
    *(volatile Fix12i *)&v.z = z;
    m = owl->mRider;
    data_020a0e68 = *(Matrix4x3 *)(*(char **)((char *)m + 0xc8));
    Matrix4x3_ApplyInPlaceToTranslation(&data_020a0e68, 0x3000, z, z);
    Matrix4x3_ApplyInPlaceToRotationXYZExt(&data_020a0e68, -0x6000, 0x1000, 0x4000);
    v.x = data_020a0e68.m[9];
    v.y = data_020a0e68.m[10];
    v.z = data_020a0e68.m[11];
    owl->mPosX = v.x << 3;
    owl->mPosY = v.y << 3;
    owl->mPosZ = v.z << 3;
    owl->mPrevAngleY = owl->mRider->mAngleY;
    owl->mPrevAngleX = (s16)z;
    *(Matrix4x3 *)((char *)owl + 0x328) = data_020a0e68;
}

// @symbol _ZN7daOwl_c16CleanupResourcesEv
int daOwl_c::CleanupResources()
{
    ((SharedFilePtr *)data_ov094_02136ae0)->Release();
    ((SharedFilePtr *)data_ov094_02136af8)->Release();
    ((SharedFilePtr *)data_ov094_02136ae8)->Release();
    ((SharedFilePtr *)data_ov094_02136af0)->Release();
    return 1;
}

// @symbol _ZN7daOwl_c16OnPendingDestroyEv
void daOwl_c::OnPendingDestroy()
{
}

// @symbol _ZN7daOwl_c6RenderEv
int daOwl_c::Render()
{
    if (mCurrentState == OWL_STATE_DORMANT)
        return 1;
    mModelAnim.Render(0);
    return 1;
}

// @symbol _ZN7daOwl_c8BehaviorEv
int daOwl_c::Behavior()
{
    DecIfAbove0_Short(&mStateTimer);
    {
        State *o = mCurrentState;
        /* Read the handler word. `&o->mMain` materialises the whole pmf. */
        if (*(int *)((char *)o + 8) != 0) {
            (this->*(o->mMain))();
        }
    }
    if (mCurrentState == OWL_STATE_DORMANT)
        return 1;
    mModelAnim.speed = mAnimSpeed;
    mModelAnim.Advance();
    {
        State *m = mCurrentState;
        if ((m == OWL_STATE_TALK || m == OWL_STATE_HOVER ||
             m == OWL_STATE_RETURN) &&
            (unsigned short)(mModelAnim.currFrame >> 0xc) == 0) {
            func_02012694(0x139, &mCamSpacePosX);
        }
    }
    /* The ROM tests this state twice; the else is unreachable. */
    if (mCurrentState == OWL_STATE_CARRY) {
        if (mCurrentState == OWL_STATE_CARRY) {
            func_ov094_021362e0(this);
            mAngleX = mPrevAngleX;
            mAngleY = mPrevAngleY;
            mAngleZ = mPrevAngleZ;
            UpdateWMClsn(mWithMeshClsn, 0);
        } else {
            func_ov094_021361d8(this);
        }
        return 1;
    }
    {
        int fallSpeed = mVertSpeed + mVertAccel;
        int clamped = mTerminalVelocity;
        int keep = unk_0ac;
        if (fallSpeed >= clamped)
            clamped = fallSpeed;
        mVertSpeed = clamped;
        unk_0ac = keep;
    }
    UpdatePosWithOnlySpeed(&mdCcAcPos_c);
    mAngleX = mPrevAngleX;
    mAngleY = mPrevAngleY;
    mAngleZ = mPrevAngleZ;
    func_ov094_021361d8(this);
    if (mCurrentState == OWL_STATE_HOVER && mTalkState == TALK_DONE) {
        func_ov094_021357a4((char *)this);
    }
    mdCcAcPos_c.Clear();
    mdCcAcPos_c.Update();
    return 1;
}

// @symbol _ZN7daOwl_c13InitResourcesEv
int daOwl_c::InitResources()
{
    int v0[3];
    void *f;

    f = Model::LoadFile(*(SharedFilePtr *)data_ov094_02136ae0);
    mModelAnim.SetFile((BMD_File *)f, 1, -1);
    mShadowModel.InitCylinder();
    Animation::LoadFile(*(SharedFilePtr *)data_ov094_02136af8);
    Animation::LoadFile(*(SharedFilePtr *)data_ov094_02136ae8);
    Animation::LoadFile(*(SharedFilePtr *)data_ov094_02136af0);
    v0[0] = data_ov094_02136a1c[0];
    v0[1] = data_ov094_02136a1c[1];
    v0[2] = data_ov094_02136a1c[2];
    _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(&mdCcAcPos_c, this, v0, 0x64000, 0x64000, 0x800004, 0);

    mRider = 0;
    mdCcAcPos_c.flags |= 2;
    mTerminalVelocity = -0x1e000;
    mAnimSpeed = 0x1000;
    mOpacity = OPACITY_MAX;
    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(&mWithMeshClsn, this, 0x50000, 0x64000, (void *)0, 0);
    func_ov094_02136188(this, OWL_STATE_DORMANT);

    if (data_0209f2f8 != 7)
        goto ret1;
    if (data_0209f220 != 1) {
        if (IsStarCollectedInCurLevel(1) != 0)
            goto ret1;
    }
    MarkForDestruction();
    return 0;
ret1:
    return 1;
}

// @symbol daOwl_c_classInit
extern "C" daOwl_c *daOwl_c_classInit()
{
    return new daOwl_c();
}
