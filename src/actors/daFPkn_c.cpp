//cpp
/* daFPkn_c -- the fire piranha plant (PAKUN2 / FIREPAKUN / FIREPAKUN_S).
 * ov084 .text 0x0212d248..0x0212eaf0, twenty-one functions: D1, D0, nine
 * helpers and states, the seven vtable methods, then the three classInit
 * registry factories.
 *
 * NAME: daFPkn_c is the cartridge's RTTI spelling. The word before the
 * vtable address point 0x02130b28 (0x02130b24) relocates to _ZTI8daFPkn_c
 * at 0x02130ac0, which reads [__si_class_type_info, _ZTS8daFPkn_c
 * (0x02130ab4, "8daFPkn_c"), _ZTI12dEnemyBase_c (ov002 0x021081c0)].
 *
 * THE DESTRUCTOR IS THE KEY FUNCTION. It is defined first under
 * `#pragma defer_codegen off`, so mwccarm emits each function as it is
 * parsed and the file is written in ROM order: D1 (0x0212d248) and D0
 * (0x0212d288) first, then a D2 the cartridge has no home for (licensed
 * as deadstrip), and this object carries _ZTV8daFPkn_c and the RTTI chain
 * as vague linkage. The ROM keeps the table; the promotion is text-only.
 *
 * The run's left neighbour is daRedBombhei_c_classInit (0x0212d200), another
 * class's factory. daFPkn_c_classInit_PAKUN2/_FIREPAKUN_S/_FIREPAKUN are
 * reconstructed names (RTTI daFPkn_c, the three registry profiles); retail
 * does not store them. Historical aliases: FirePiranhaPlant_Spawn,
 * FirePiranhaPlantSmall_Spawn and FirePiranhaPlantBig_Spawn. The daPkn_c
 * run begins immediately after, at 0x0212eaf0.
 *
 * Known limits:
 * - ModelAnim::SetAnim, dCcAc_c::Init, dCcAcPos_c::Init,
 *   Particle::System::New, dActor_c::SpawnFireball and SpawnCoins,
 *   Player::Hurt and Player::Bounce are called by their mangled names. Each
 *   symbol carries a Fix12<int> by value; a Fix12<int> local for SetAnim's
 *   speed changes InitResources.
 * - The shared files and tables keep their address names; the static
 *   initializer and the classInit sources name them too.
 * - The int flags (cmp, isFirepakun, inYoshiMouth and the like) and the gotos
 *   in CheckClsnHits, StateGrow and StateSpit are kept from the byte-matching
 *   recovery; the comment at each site names the direct spelling that DIFFs.
 * - The hit-flag mask 0x66ff0, the mFlags bit 0x10000000 and the Sound and
 *   particle ids stay numeric; nothing in the repo names them.
 */

#include "decl_common.h"
#include "daFPkn_c.h"
#include "SharedFilePtr.h"
#include "Player.h"
#include "Sound.h"
#include "Particle__System.h"

#pragma defer_codegen off

/* Second word is the loaded BCA. Same two words as daPkn_c's PknSharedFile. */
struct PknSharedFile {
    int id;
    void *file;
};
typedef char PknSharedFile_size_must_be_0x8[sizeof(PknSharedFile) == 0x8 ? 1 : -1];

/* UpdateClsnOffset's stack locals: three bone-angle sums, then the
   position shifted down by 3. */
struct Locals {
    s16 acc[3];
    int tmp[3];
};

/* Actor ids, from symbols/actor_debug_names.tsv. */
enum {
    ACTOR_STAR = 178,
    ACTOR_PLAYER = 191,
    ACTOR_PAKUN = 250,
    ACTOR_FIREPAKUN = 251,
    ACTOR_FIREPAKUN_S = 252,
    ACTOR_PAKUN2 = 253
};

/* mState, as Behavior dispatches it. State 4 has no case body: StateWait sets
   it after a group member is defeated with mRespawnMode == 1. */
enum {
    STATE_INIT = 0,
    STATE_WAIT = 1,
    STATE_SPIT = 2,
    STATE_GROW = 3,
    STATE_DEFEATED = 4
};

/* dActor_c::mFlags bits written by actor code (see dActor_c.h). */
enum {
    MFLAG_YOSHI_MOUTH_A = 0x20000,
    MFLAG_YOSHI_MOUTH_B = 0x40000
};

/* dCc_c::hitFlags bit, per the table in dCc_c.h. */
enum {
    HIT_MEGA_CHARACTER = 0x10
};

int ApproachLinear(int &value, int target, int step);
bool ApproachLinear(short &value, short target, short step);

extern "C" {
void _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(void *self, void *f, int a, int b, unsigned short cc);
void _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(void *self, void *a, int r, int h, unsigned int e, unsigned int g);
void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(void *self, void *a, const Vector3 *v, int r, int h, unsigned int e, unsigned int g);
u32 _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
    u32 a, u32 b, Fix12i c, Fix12i d, Fix12i e, const void *f, void *g);
void _ZN6Player6BounceE5Fix12IiE(void *p, int fix);
int _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(void *self, const void *pos, u32 a, int fix, u32 b, u32 c, u32 d);
void _ZN8dActor_c13SpawnFireballERK7Vector3PK10Vector3_165Fix12IiES7_j(
    void *self, const void *pos, const void *v16, int a, int b, u32 g);
void _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(void *self, const void *pos, unsigned int a, int b, short c);

void LoadBlueCoinModel(void *c);
void UnloadBlueCoinModel(void *);
void func_02012694(u32 id, void *pos);
void func_0201267c(unsigned int id, const void *pos);
short Vec3_HorzAngle(const void *a, const void *b);
int IsStarCollectedInCurLevel(unsigned int flag);
void SetStarMarker(int i, void *self, int v);
void Matrix4x3_FromRotationY(void *m, int angle);
void Vec3_Asr(void *d, void *s, int sh);
void Matrix4x3_FromTranslation(void *m, int x, int y, int z);
void MulMat4x3Mat4x3(void *d, void *a, void *b);
void Vec3_LslInPlace(void *v, int sh);
void SubVec3(void *a, void *b, void *c);

extern SharedFilePtr data_ov084_02130dfc;   /* the plant's model */
extern SharedFilePtr *data_ov084_021302f4[];
extern SharedFilePtr data_ov002_0210da38;
extern PknSharedFile data_ov084_02130df4;   /* idle animation */
extern PknSharedFile data_ov084_02130e24;   /* death animation */
extern PknSharedFile data_ov084_02130e14;   /* lunge animation */
extern PknSharedFile data_ov084_02130e04;   /* spit animation */
extern int data_ov084_0213029c[];           /* SpawnDeathBurst reach per frame */
extern int data_ov084_021302c4[];           /* SpawnDeathBurst height per frame */
extern u8 data_ov084_02130294[];            /* UpdateClsnOffset's five bone indices */
extern s32 data_020a0e68[];                 /* the shared scratch matrix */
extern s16 data_02082214[];                 /* the sin and cos table, two shorts a step */
}

/* One vtable store and four destructor calls, every one a consequence of
 * `struct daFPkn_c : dEnemyBase_c` and the members that declaration types,
 * destroyed in reverse declaration order, then dEnemyBase_c::~dEnemyBase_c.
 * That body is the evidence for the header: each member's size closes
 * exactly on the next one's offset. D0 ends in dEnemyBase_c's inline
 * operator delete, reachable because dEnemyBase_c is the immediate base. */
// @symbol _ZN8daFPkn_cD1Ev
// @symbol _ZN8daFPkn_cD0Ev
daFPkn_c::~daFPkn_c()
{
}

// @symbol _ZN8daFPkn_c15SpawnDeathSmokeEv
void daFPkn_c::SpawnDeathSmoke()
{
    Particle::System *particle;
    if (mModelAnim.file != data_ov084_02130e24.file)
        return;

    mParticleHandle1 = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
        mParticleHandle1, 0xfc, mPosX, mPosY + 0x1e000, mPosZ, 0, 0);
    if (mParticleHandle1 != 0) {
        particle = Particle::System::FromUniqueID(mParticleHandle1);
        if (particle != 0) {
            particle->callbackScale = (short)(Fix12i)(((long long)mMaxScale * 0x2800 + 0x800) >> 12);
        }
    }

    mParticleHandle2 = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
        mParticleHandle2, 0xfd, mPosX, mPosY + 0x1e000, mPosZ, 0, 0);
    if (mParticleHandle2 == 0)
        return;
    particle = Particle::System::FromUniqueID(mParticleHandle2);
    if (particle == 0)
        return;
    particle->callbackScale = (short)(Fix12i)(((long long)mMaxScale * 0x2800 + 0x800) >> 12);
}

// @symbol _ZN8daFPkn_c15SpawnDeathBurstEv
void daFPkn_c::SpawnDeathBurst()
{
    Vector3 pos;
    int frame, burstReach, product;

    if (mModelAnim.file != data_ov084_02130e24.file)
        return;
    frame = (int)((unsigned)(mModelAnim.currFrame << 4) >> 16);
    if (frame >= 0xa)
        return;

    pos.x = mPosX;
    pos.y = mPosY;
    pos.z = mPosZ;
    burstReach = data_ov084_0213029c[frame];
    product = burstReach * data_02082214[((u16)mAngleY >> 4) * 2];
    pos.x = pos.x + (int)(((long long)product * mScale + 0x800) >> 12);
    product = burstReach * data_02082214[((u16)mAngleY >> 4) * 2 + 1];
    pos.z = pos.z + (int)(((long long)product * mScale + 0x800) >> 12);
    pos.y = pos.y + mScale * data_ov084_021302c4[frame];
    mParticleHandle1 = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
        mParticleHandle1, 0xfb, pos.x, pos.y, pos.z, 0, 0);
}

/* Empty. StateWait passes this in r0 and nothing reads it. */
// @symbol _ZN8daFPkn_c21OnGroupMemberDefeatedEv
void daFPkn_c::OnGroupMemberDefeated()
{
}

// @symbol _ZN8daFPkn_c16UpdateClsnOffsetEv
void daFPkn_c::UpdateClsnOffset()
{
    struct Locals locals;
    int scaledSinAcc0, sinAcc1, cosAcc1, scaledCosAcc0, isFirepakun;

    Matrix4x3_FromRotationY(&mModelAnim.mat4x3, mAngleY);
    mModelAnim.mat4x3.m[9] = mPosX >> 3;
    mModelAnim.mat4x3.m[10] = mPosY >> 3;
    mModelAnim.mat4x3.m[11] = mPosZ >> 3;

    if ((u32)(mState - STATE_SPIT) > 1) return;
    if (mScale != mMaxScale) return;

    locals.acc[0] = 0;
    locals.acc[1] = 0;
    locals.acc[2] = 0;
    {
        char* base = (char *)mModelAnim.data.bones;
        int i;
        for (i = 0; i < 5; i++) {
            u8* p = data_ov084_02130294 + i;
            locals.acc[0] = (s16)(locals.acc[0] + *(s16*)(base + *p * 0x34 + 0x1a));
            locals.acc[1] = (s16)(locals.acc[1] + *(s16*)(base + *p * 0x34 + 0x1c));
            locals.acc[2] = (s16)(locals.acc[2] + *(s16*)(base + *p * 0x34 + 0x1e));
        }
    }

    mClsnOffset.x = 0;
    mClsnOffset.y = 0;
    mClsnOffset.z = 0;

    Vec3_Asr(locals.tmp, &mPosX, 3);
    Matrix4x3_FromTranslation(
        data_020a0e68,
        locals.tmp[0],
        locals.tmp[1],
        locals.tmp[2]
    );
    MulMat4x3Mat4x3(&mModelAnim.data.transforms[6], data_020a0e68, data_020a0e68);
    mClsnOffset.x = data_020a0e68[0x24 / 4];
    mClsnOffset.y = data_020a0e68[0x28 / 4];
    mClsnOffset.z = data_020a0e68[0x2c / 4];
    Vec3_LslInPlace(&mClsnOffset, 3);
    SubVec3(&mClsnOffset, &mPosX, &mClsnOffset);

    scaledSinAcc0 = data_02082214[((u16)locals.acc[0] >> 4) * 2] * 0x32;
    sinAcc1 = data_02082214[((u16)locals.acc[1] >> 4) * 2];

    mClsnOffset.x = mClsnOffset.x + (int)(((s64)scaledSinAcc0 * sinAcc1 + 0x800) >> 12);

    /* The int isFirepakun keeps the ROM's compare; a direct test DIFFs. */
    isFirepakun = (actorID == ACTOR_FIREPAKUN);
    if (isFirepakun != false) {
        scaledCosAcc0 = data_02082214[((u16)locals.acc[0] >> 4) * 2 + 1] * 0x32;
        mClsnOffset.y = mClsnOffset.y - (0x19000 - scaledCosAcc0);
    } else {
        scaledCosAcc0 = data_02082214[((u16)locals.acc[0] >> 4) * 2 + 1] * 0x32;
        mClsnOffset.y = mClsnOffset.y - (0x32000 - scaledCosAcc0);
    }

    cosAcc1 = data_02082214[((u16)locals.acc[1] >> 4) * 2 + 1];
    mClsnOffset.z = mClsnOffset.z + (int)(((s64)scaledSinAcc0 * cosAcc1 + 0x800) >> 12);

    mClsnOffset.x = (int)(((s64)mClsnOffset.x * mScale + 0x800) >> 12);
    mClsnOffset.y = (int)(((s64)mClsnOffset.y * mScale + 0x800) >> 12);
    mClsnOffset.z = (int)(((s64)mClsnOffset.z * mScale + 0x800) >> 12);
}

// @symbol _ZN8daFPkn_c13CheckClsnHitsEv
void daFPkn_c::CheckClsnHits()
{
    dActor_c *hitter;
    int cmp;
    int hitBits;
    Vector3 pos1;
    Vector3 pos2;
    u32 otherId;

    /* The two gotos share the defeat path; a goto-free nesting of it DIFFs. */
    otherId = mdCcAc_c.otherOwner;
    if (otherId == 0)
        goto second;

    hitBits = mdCcAc_c.hitFlags & 0x66ff0;
    if (hitBits != 0) {
        cmp = (int)(actorID == ACTOR_FIREPAKUN);
        if (cmp != 0)
            func_02012694(0x1e, &mCamSpacePosX);
        else
            Sound::PlayBank0(0xa, (Vector3 &)mCamSpacePosX);
    activate_path:
        cmp = (int)(actorID == ACTOR_FIREPAKUN_S);
        if (cmp != 0) {
            unk_108 = 1;
            SpawnCoin();
            KillAndTrackInDeathTable();
            Sound::PlayBank0(0xa, (Vector3 &)mCamSpacePosX);
        } else {
            mState = STATE_WAIT;
            mSpinCount = 0xa;
            mSpinSpeed = 0x1f40;
            mdCcAc_c.flags |= 1;
            mFlags &= ~0x10000000;
            _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&mModelAnim, data_ov084_02130e24.file, 0x40000000, 0x1000, 0);
            mSuppressDeathReward = 0;
            mParticleHandle1 = 0;
        }
        if ((mdCcAc_c.hitFlags & HIT_MEGA_CHARACTER) == 0)
            goto second;
        Sound::PlayBank0(0xa, (Vector3 &)mCamSpacePosX);
        hitter = dActor_c::FindWithID(mdCcAc_c.otherOwner);
        if (hitter == 0)
            goto second;
        ((Player *)hitter)->IncMegaKillCount();
        func_02012694(0x1d, &mCamSpacePosX);
        goto second;
    }

    hitter = dActor_c::FindWithID(otherId);
    if (hitter == 0)
        goto second;
    cmp = (int)(hitter->actorID == ACTOR_PLAYER);
    if (cmp == 0)
        goto second;
    if (((Player *)hitter)->mIsMetal != 0) {
        Sound::PlayBank0(0xa, (Vector3 &)mCamSpacePosX);
        goto activate_path;
    }
    cmp = (int)(actorID == ACTOR_FIREPAKUN_S);
    if (cmp != 0) {
        if (JumpedOnByPlayer(mdCcAc_c, *(Player *)hitter) != 0) {
            Sound::PlayBank0(0xb6, (Vector3 &)mCamSpacePosX);
            _ZN6Player6BounceE5Fix12IiE(hitter, 0x28000);
            goto activate_path;
        }
    }
    if (((Player *)hitter)->mIsVanish != 0)
        goto second;
    cmp = (int)(actorID == ACTOR_FIREPAKUN);
    if (cmp != 0)
        goto second;
    pos1.x = mPosX;
    pos1.y = mPosY;
    pos1.z = mPosZ;
    _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(hitter, &pos1, 2, 0xc000, 1, 0, 1);

second:
    otherId = mdCcAcPos_c.otherOwner;
    if (otherId == 0)
        return;
    hitter = dActor_c::FindWithID(otherId);
    if (hitter == 0)
        return;
    cmp = (int)(hitter->actorID == ACTOR_PLAYER);
    if (cmp == 0)
        return;

    hitBits = mdCcAcPos_c.hitFlags & 0x66ff0;
    if (hitBits != 0) {
        if ((hitBits & HIT_MEGA_CHARACTER) != 0) {
            ((Player *)hitter)->IncMegaKillCount();
            func_02012694(0x1d, &mCamSpacePosX);
        } else {
            cmp = (int)(actorID == ACTOR_FIREPAKUN);
            if (cmp != 0)
                func_02012694(0x1e, &mCamSpacePosX);
            else
                Sound::PlayBank0(0xa, (Vector3 &)mCamSpacePosX);
        }
        mState = STATE_WAIT;
        mSpinCount = 0xa;
        mSpinSpeed = 0x1f40;
        mdCcAc_c.flags |= 1;
        mFlags &= ~0x10000000;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&mModelAnim, data_ov084_02130e24.file, 0x40000000, 0x1000, 0);
        mSuppressDeathReward = 0;
        mParticleHandle1 = 0;
        return;
    }

    if (((Player *)hitter)->mIsMetal != 0)
        return;
    if (((Player *)hitter)->mIsVanish != 0)
        return;
    pos2.x = mPosX;
    pos2.y = mPosY;
    pos2.z = mPosZ;
    _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(hitter, &pos2, 2, 0xc000, 1, 0, 1);
}

// @symbol _ZN8daFPkn_c9StateGrowEv
void daFPkn_c::StateGrow()
{
    Player *player;
    Vector3 playerPos;
    short targetAngle;
    dActor_c *spawned;
    int *src;

    if (ApproachLinear(mScale, mMaxScale, mScaleRate) == 0) {
        goto tail;  /* an early return here DIFFs */
    }

    if (mModelAnim.WillHitFrame(0x10) ||
        mModelAnim.WillHitFrame(0x20) ||
        mModelAnim.WillHitFrame(0x34) ||
        mModelAnim.WillHitFrame(0x4b)) {
        func_0201267c(0xc0, &mCamSpacePosX);
    }

    targetAngle = mAngleY;
    player = ClosestPlayer();
    /* Read through src before the null test; direct member reads DIFF. */
    src = &player->mPosX;
    playerPos.x = src[0];
    playerPos.y = src[1];
    playerPos.z = src[2];
    if (player != 0) {
        targetAngle = Vec3_HorzAngle(&mPosX, &playerPos);
    }
    ApproachLinear(mAngleY, targetAngle, 0x400);

    if (mSuppressDeathReward == 1) {
        spawned = dActor_c::Spawn(ACTOR_PAKUN, 0, (Vector3 &)mPosX, (Vector3_16 *)&mAngleX, mAreaId, -1);
        if (spawned == 0) return;

        mSuppressDeathReward = 2;
        func_ov084_0212ec04((char*)spawned, (short)((unsigned int)(mModelAnim.currFrame << 4) >> 16));
        mdCcAc_c.flags |= 1;
        mdCcAcPos_c.flags |= 1;
        return;
    }

    MarkForDestruction();
    return;

tail:
    mdCcAc_c.flags |= 1;
}

// @symbol _ZN8daFPkn_c9StateSpitEv
void daFPkn_c::StateSpit()
{
    Vector3 pos;
    s16 targetAngle;
    int cmp;
    Player *player;

    if (ApproachLinear(mScale, mMaxScale, mScaleRate) == 0)
        goto cold;  /* the ROM places this branch last */

    /* The int flag cmp keeps the ROM's compare; a direct test DIFFs. */
    if (mModelAnim.Finished() != 0) {
        cmp = (int)(actorID == ACTOR_FIREPAKUN_S);
        if (cmp != 0)
            func_0201267c(0xe3, &mCamSpacePosX);
        else
            func_0201267c(0x120, &mCamSpacePosX);
        mState = STATE_WAIT;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&mModelAnim, data_ov084_02130e1c[1], 0, 0x1000, 0);
    } else {
        if ((u16)mStateTimer < 0x3a) {
            targetAngle = mAngleY;
            player = ClosestPlayer();
            if (player != 0)
                targetAngle = Vec3_HorzAngle(&mPosX, &player->mPosX);
            ApproachLinear(mAngleY, targetAngle, 0x400);
        }
    }

    if (mModelAnim.WillHitFrame(0x3a) == 0)
        return;

    cmp = (int)(actorID == ACTOR_FIREPAKUN_S);
    if (cmp != 0)
        func_0201267c(0x105, &mCamSpacePosX);
    else
        func_0201267c(0x122, &mCamSpacePosX);

    pos.x = mPosX;
    pos.y = mPosY;
    pos.z = mPosZ;
    pos.x += (s32)(((s64)(mMaxScale * 0x3c) * data_02082214[((u16)mAngleY >> 4) * 2] + 0x800) >> 12);
    pos.z += (s32)(((s64)(mMaxScale * 0x3c) * data_02082214[((u16)mAngleY >> 4) * 2 + 1] + 0x800) >> 12);
    pos.y += mMaxScale * 0x5a;
    mAngleX = 0x1000;

    _ZN8dActor_c13SpawnFireballERK7Vector3PK10Vector3_165Fix12IiES7_j(
        this, &pos, &mAngleX, 0x1e000, 0xa000, 3);
    return;

cold:
    if (mScale <= (mMaxScale >> 1))
        return;
    mdCcAc_c.flags &= ~1;
    mFlags |= 0x10000000;
}

// @symbol _ZN8daFPkn_c9StateWaitEv
void daFPkn_c::StateWait()
{
    Vector3 coinPos;
    Vector3 coinPos2;
    daFPkn_c *leader;
    Player *player;
    int dist;
    int cmp;

    if (mSpinCount != 0) {
        mAngleY += mSpinSpeed;
        ApproachLinear(mSpinSpeed, 0, 0xc8);
        SpawnDeathBurst();
        if (mModelAnim.Finished() == 0)
            return;
        mSpinCount--;
        if (mSpinCount != 0)
            return;
        func_02012694(0x11f, &mCamSpacePosX);
        mParticleHandle2 = 0;
        mParticleHandle1 = mParticleHandle2;
        return;
    }

    SpawnDeathSmoke();
    if (ApproachLinear(mScale, 0, mScaleRate) == 0)
        return;
    mFlags &= ~0x10000000;
    mdCcAc_c.flags |= 1;

    /* The int flag cmp keeps the ROM's compares; direct tests DIFF. */
    if (mEmerged != 0) {
        mEmerged = 0;
        cmp = 0;
        if (actorID == ACTOR_FIREPAKUN)
            cmp = 1;
        if (cmp != false) {
            leader = (daFPkn_c *)dActor_c::FindWithID(mGroupLeaderID);
            if (leader == 0)
                return;
            leader->mGroupAliveCount--;
            if (mSuppressDeathReward != 0)
                return;
            leader->mGroupDefeatedCount++;
            if (mAlive != 0) {
                coinPos.x = mPosX;
                coinPos.y = mPosY;
                coinPos.z = mPosZ;
                _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(this, &coinPos, 2, 0xa000, 0);
            }
            if (leader->mGroupDefeatedCount == 5) {
                dActor_c::Spawn(ACTOR_STAR, mStarID | 0x40, (Vector3 &)mPosX, 0, mAreaId, -1);
                leader->KillAndTrackInDeathTable();
                KillAndTrackInDeathTable();
                return;
            }
            OnGroupMemberDefeated();
            if (mRespawnMode != 1) {
                KillAndTrackInDeathTable();
                return;
            }
            TrackInDeathTable();
            mState = STATE_DEFEATED;
            return;
        }
        cmp = actorID == ACTOR_PAKUN2;
        if (cmp == false)
            return;
        if (mSuppressDeathReward != 0)
            return;
        coinPos2.x = mPosX;
        coinPos2.y = mPosY;
        coinPos2.z = mPosZ;
        _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(this, &coinPos2, 1, 0xa000, 0);
        KillAndTrackInDeathTable();
        return;
    }

    dist = DistToCPlayer();
    cmp = actorID == ACTOR_FIREPAKUN;
    if (cmp != false) {
        leader = (daFPkn_c *)dActor_c::FindWithID(mGroupLeaderID);
        if (leader == 0)
            return;
    }
    if ((u16)mStateTimer <= 0x64)
        return;
    if (dist <= 0x64000)
        return;
    if (dist >= 0x320000)
        return;
    cmp = actorID == ACTOR_FIREPAKUN;
    if (cmp != false) {
        if (leader->mGroupAliveCount >= 2)
            return;
    }
    cmp = actorID == ACTOR_FIREPAKUN_S;
    if (cmp != false)
        func_0201267c(0x104, &mCamSpacePosX);
    else
        func_0201267c(0x121, &mCamSpacePosX);
    cmp = 1;
    mEmerged = 1;
    if (actorID != ACTOR_FIREPAKUN)
        cmp = 0;
    if (cmp != false)
        leader->mGroupAliveCount++;
    cmp = actorID == ACTOR_PAKUN2;
    if (cmp != false) {
        mState = STATE_GROW;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&mModelAnim, data_ov084_02130e14.file, 0x40000000, 0x1000, 0);
    } else {
        mState = STATE_SPIT;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&mModelAnim, data_ov084_02130e04.file, 0x40000000, 0x1000, 0);
        mModelAnim.currFrame = 0;
    }
    cmp = actorID == ACTOR_FIREPAKUN;
    if (cmp != false) {
        if (leader->mStarMarkerIdx >= 0) {
            int marker;
            if (IsStarCollectedInCurLevel(mStarID) != 0)
                marker = 3;
            else
                marker = 2;
            SetStarMarker(leader->mStarMarkerIdx, this, marker);
            leader->mMarkedMemberID = uniqueID;
        }
    }
    player = ClosestPlayer();
    if (player == 0)
        return;
    mAngleY = Vec3_HorzAngle(&mPosX, &player->mPosX);
}

// @symbol _ZN8daFPkn_c9StateInitEv
int daFPkn_c::StateInit()
{
    daFPkn_c *member;
    if (mRespawnMode == 0) {
        mStarMarkerIdx = -1;
        mGroupLeaderID = uniqueID;
        mMarkedMemberID = uniqueID;
        mRespawnMode = 1;
        member = 0;
        for (;;) {  /* a while loop over the assignment DIFFs */
            member = (daFPkn_c *)FindWithActorID(ACTOR_FIREPAKUN, member);
            if (member == 0) break;
            if (member != this) {
                member->mRespawnMode = 2;
                member->mGroupLeaderID = uniqueID;
            }
        }
    }
    mState = STATE_WAIT;
    return 1;
}

// @symbol _ZN8daFPkn_c16CleanupResourcesEv
int daFPkn_c::CleanupResources()
{
    data_ov084_02130dfc.Release();
    for (int i = 0; i < 6; i++)
        data_ov084_021302f4[i]->Release();
    data_ov002_0210da38.Release();
    UnloadBlueCoinModel(this);
    return 1;
}

// @symbol _ZN8daFPkn_c6RenderEv
/* The int inMouth keeps the ROM's flag test; a plain `||` DIFFs. */
int daFPkn_c::Render()
{
    int scale = mScale;
    int inMouth;
    if (scale == 0 || (inMouth = (mFlags & MFLAG_YOSHI_MOUTH_B) != 0, inMouth != 0)) {
        return 1;
    }
    Vector3 size;
    size.x = scale;
    size.y = scale;
    size.z = scale;
    mModelAnim.Render(&size);
    return 1;
}

// @symbol _ZN8daFPkn_c8BehaviorEv
int daFPkn_c::Behavior()
{
    MakeVanishLuigiWork(mdCcAc_c);
    /* The int flags inYoshiMouth and isFirepakunS keep the ROM's tests; direct tests DIFF. */
    int inYoshiMouth = (mFlags & (MFLAG_YOSHI_MOUTH_A | MFLAG_YOSHI_MOUTH_B)) != 0;
    if (inYoshiMouth != 0) {
        UpdateClsnOffset();
        return 1;
    }
    mModelAnim.Advance();
    int prevState = mState;
    switch (prevState) {
    case STATE_INIT:
        StateInit();
        break;
    case STATE_WAIT:
        StateWait();
        break;
    case STATE_SPIT:
        StateSpit();
        break;
    case STATE_GROW:
        StateGrow();
        break;
    case STATE_DEFEATED:
        break;
    }
    /* mStateTimer is s16; the ROM counts it unsigned, and `(u16)mStateTimer + 1` DIFFs. */
    {
        unsigned short *timer = (unsigned short *)&mStateTimer;
        *timer = *timer + 1;
        if (prevState != mState)
            *timer = 0;
    }
    CheckClsnHits();
    UpdateClsnOffset();
    mdCcAc_c.Clear();
    mdCcAc_c.radius = mScale * mClsnRadiusFactor;
    mdCcAc_c.height = mScale * mClsnHeightFactor;
    mdCcAc_c.Update();
    mdCcAcPos_c.Clear();
    int isFirepakunS = actorID == ACTOR_FIREPAKUN_S;
    if (isFirepakunS == 0
        && (unsigned int)(mState - STATE_SPIT) <= 1
        && mScale == mMaxScale) {
        mdCcAcPos_c.SetPosRelativeToActor(mClsnOffset);
        mdCcAcPos_c.Update();
    }
    return 1;
}

// @symbol _ZN8daFPkn_c13InitResourcesEv
int daFPkn_c::InitResources()
{
    int i;
    Vector3 clsnPos;
    int type;
    int cmp;

    mModelAnim.SetFile((BMD_File *)Model::LoadFile(data_ov084_02130dfc), 1, -1);

    for (i = 0; i < 6; i++)
        Animation::LoadFile(*data_ov084_021302f4[i]);

    Model::LoadFile(data_ov002_0210da38);
    LoadBlueCoinModel(this);

    /* A real SetAnim call with a Fix12<int> speed DIFFs; see the file header. */
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(
        &mModelAnim, data_ov084_02130df4.file, 0x40000000, 0x1000, 0);

    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(
        &mdCcAc_c, this, 0, 0, 0x200001, 0x66fe0);

    clsnPos.x = 0;
    clsnPos.y = 0;
    clsnPos.z = 0;
    _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
        &mdCcAcPos_c, this, &clsnPos, 0x4b000, 0x64000, 0x200002, 0x66fe0);

    mScale = 0;
    mRespawnMode = 0;
    mState = STATE_INIT;
    mGroupLeaderID = 0;
    mMarkedMemberID = 0;
    mGroupAliveCount = 0;
    mGroupDefeatedCount = 0;
    mEmerged = 0;
    mSpinCount = 0;
    mSuppressDeathReward = 1;
    mParticleHandle2 = 0;
    mParticleHandle1 = mParticleHandle2;

    type = actorID;
    /* type and cmp keep the ROM's compares; direct tests DIFF. */
    cmp = (type == ACTOR_FIREPAKUN_S);
    if (cmp != 0) {
        mClsnRadiusFactor = 0x3c;
        mClsnHeightFactor = 0xaa;
        mMaxScale = 0x800;
        mScaleRate = 0x52;
        mState = STATE_WAIT;
        mdCcAc_c.vulnFlags |= 0x8000;
    } else {
        cmp = (type == ACTOR_PAKUN2);
        if (cmp != 0) {
            mClsnRadiusFactor = 0x28;
            mClsnHeightFactor = 0xaa;
            mMaxScale = 0x1000;
            mScaleRate = 0xa4;
            mState = STATE_WAIT;
        } else {
            mClsnRadiusFactor = 0x28;
            mClsnHeightFactor = 0x96;
            mMaxScale = 0x2000;
            mScaleRate = 0x147;
            mdCcAcPos_c.radius = 0x64000;
            mdCcAcPos_c.height = 0x64000;
            if (GetBitInDeathTable() != 0)
                mAlive = 0;
            else
                mAlive = 1;
        }
    }

    mStarID = (unsigned char)(param1 & 0xf);
    return 1;
}

// @symbol _ZN8daFPkn_c16OnAimedAtWithEggEv
s32 daFPkn_c::OnAimedAtWithEgg() {
    if (mdCcAc_c.flags & 1)
        return mScale * 100;
    int reach = mScale * mClsnHeightFactor >> 1;
    int floor = mScale * 100;
    if (reach <= floor)
        reach = floor;
    return reach;
}

// @symbol _ZN8daFPkn_c13OnTurnIntoEggER6Player
void daFPkn_c::OnTurnIntoEgg(Player &player)
{
    GivePlayerCoins(player, 1, 0);
    KillAndTrackInDeathTable();
}

// @symbol _ZN8daFPkn_c13OnYoshiTryEatEv
/* Two steps; `return actorID == ACTOR_FIREPAKUN_S ? 4 : 0` DIFFs. */
s32 daFPkn_c::OnYoshiTryEat() {
    int r;
    if (actorID == ACTOR_FIREPAKUN_S)
        r = 1;
    else
        r = 0;
    if (r != 0)
        r = 4;
    else
        r = 0;
    return r;
}

// @symbol daFPkn_c_classInit_PAKUN2
extern "C" daFPkn_c *daFPkn_c_classInit_PAKUN2()
{
    return new daFPkn_c();
}

// @symbol daFPkn_c_classInit_FIREPAKUN_S
extern "C" daFPkn_c *daFPkn_c_classInit_FIREPAKUN_S()
{
    return new daFPkn_c();
}

// @symbol daFPkn_c_classInit_FIREPAKUN
extern "C" daFPkn_c *daFPkn_c_classInit_FIREPAKUN()
{
    return new daFPkn_c();
}
