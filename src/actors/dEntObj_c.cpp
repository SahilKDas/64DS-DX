//cpp
/* dEntObj_c and UnknownVsPlayer. ov075 0x02113ee0..0x02115ab8, 43 functions.
 *
 * The versus-mode entry scene's stage object: four player figures on the
 * entry stage. Each figure runs two pointer-to-member state tables, one for
 * movement (UnknownVsPlayer::mMoveState) and one for animation (mAnimState).
 * The unmangled helpers below are those states and the scene's own
 * bookkeeping; the ROM keeps no names for them.
 *
 * The class name is the ROM's own: _ZTS9dEntObj_c at 0x0211c648, with
 * _ZTI9dEntObj_c at 0x0211c66c and the vtable _ZTV9dEntObj_c at 0x0211c6a0.
 * UnknownVsPlayer has no vtable and no RTTI, so the cartridge gives no name
 * for it and the project's name stays.
 *
 * #pragma defer_codegen off lays .text down in source order, so the file
 * reads in ROM order: the structors, the helpers, the four vtable methods,
 * the classInit factory, then UnknownVsPlayer's constructor.
 */

#include "dEntObj_c.h"
#include "decl_common.h"
#include "common.h"
#include "Clipper.h"
#include "SharedFilePtr.h"

struct BMD_File;
struct BTP_File;

namespace cstd { int fdiv(int a, int b); }
namespace Particle { void RenderAll(); }
namespace G3i {
    void LookAt_(const Vector3 *eye, const Vector3 *at, const Vector3 *up, bool b, Matrix4x3 *m);
}
bool ApproachLinear(short &value, short target, short step);
void CopyToViewMat(const Matrix4x3 *m);

/* Fix12<int> is passed by value at these call boundaries. Spelled as the
 * class type, mwccarm homes the register arguments and the callers grow, so
 * the measured scalar views stay. */
extern "C" {
void _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
    void *anim, void *file, int numBlendFrames, int flags, int speed, unsigned short startFrame);
void _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(
    void *anim, void *file, int flags, int speed, unsigned short startFrame);
void _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(
    void *seq, void *file, int flags, int speed, unsigned short startFrame);
int _ZN7Clipper13Func_02015560ER9Matrix4x3R7Vector35Fix12IiES3_(
    void *clipper, void *mat, void *src, int scale, void *dst);
void _ZN11ShadowModel9InitModelEP9Matrix4x35Fix12IiES3_S3_j(
    void *shadow, Matrix4x3 *mat, int radius, int height, int depth, unsigned char flags);
int _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
    unsigned handle, unsigned id, int x, int y, int z, const Vector3_16 *dir, void *callback);
void _ZN3G3i13PerspectiveW_E5Fix12IiES1_S1_S1_S1_S1_bP9Matrix4x3(
    int sinFov, int cosFov, int aspect, int nearZ, int farZ, int scaleW, bool load, Matrix4x3 *m);


int RandomIntInternal(int *seed);
short Vec3_HorzAngle(const Vector3 *a, const Vector3 *b);
void Vec3_Asr(Vector3 *dst, Vector3 *src, int shift);
void Vec3_MulScalarInPlace(Vector3 *v, int scale);
void MulVec3Mat4x3(Vector3 *v, Matrix4x3 *m, Vector3 *out);
void Matrix4x3_FromTranslation(Matrix4x3 *m, int x, int y, int z);
void Matrix4x3_ApplyInPlaceToRotationY(Matrix4x3 *m, short angY);
int Math_Function_0203b14c(int *p, int target, int a, int b, int c);
unsigned char DecIfAbove0_Byte(unsigned char *p);
int func_0201251c(int a, int b, void *pos, int c);
unsigned int func_02012174(unsigned int a, unsigned int b);
unsigned int func_02012790(unsigned int sound);
void func_020167a4(void *model);
int func_ov075_0211b3d8(void *p);
long long __aeabi_uidiv(unsigned int n, int d);

extern Matrix4x3 data_020a0e68;
extern Clipper data_0209f43c;
extern int data_0209b3ec[];
extern int data_0209e650;
extern short data_02082214[];     /* sine/cosine table, interleaved */
extern short data_02082614[];
extern u8 data_0209fc5c[];
extern signed char data_0209fc64[];

/* SharedFilePtr handles; the loaded file sits at +4. */
extern int data_ov075_0211d384[];
extern int data_ov075_0211d38c[];
extern int data_ov075_0211d394[];
extern int data_ov075_0211d39c[];
extern int data_ov075_0211d3a4[];
extern int data_ov075_0211d3ac[];
extern int data_ov075_0211d3b4[];
extern int data_ov075_0211d3bc[];
extern int data_ov075_0211d3c4[];
extern int data_ov075_0211d3cc[];
extern int data_ov075_0211d3d4[];
extern int data_ov075_0211d3dc[];
extern int data_ov075_0211d3e4[];
extern int data_ov075_0211d3ec[];
extern int data_ov075_0211d3f4[];
extern int data_ov075_0211d3fc[];
extern int data_ov075_0211d404[];
extern int data_ov075_0211d40c[];
extern int data_ov075_0211d414[];
extern int data_ov075_0211d41c[];
extern int data_ov075_0211d424[];
extern int data_ov075_0211d42c[];
extern int *data_ov075_0211c678[];
extern int *data_ov075_0211c688[];

extern unsigned char data_ov075_0211b524[];
extern int data_ov075_0211b534[];
extern int data_ov075_0211b544[];
extern int data_ov075_0211b554[][4];
extern int data_ov075_0211b594[];

extern void *_ZN7fBase_cnwEj(unsigned int size);
extern void *_ZN7fBase_cC2Ev(void *p);
extern void _ZN8Particle10SysTrackerC1Ev(void *p);
extern void *_ZN5ModelC1Ev(void *p);
extern void *_ZN9ModelAnimC1Ev(void *p);
extern void __cxa_vec_ctor(void *base, unsigned int count, unsigned int stride,
                           void (*ctor)(void *), void (*dtor)(void *));
extern void *data_0208e4b8;
extern UnknownVsPlayer *_ZN15UnknownVsPlayerD1Ev(UnknownVsPlayer *object);
extern UnknownVsPlayer *_ZN15UnknownVsPlayerC1Ev(UnknownVsPlayer *object);

int func_ov075_021148f0(UnknownVsPlayer *p);
int func_ov075_02114a6c(UnknownVsPlayer *p);
int func_ov075_02114ac4(UnknownVsPlayer *p, Vector3 *target, Vector3 *eye);
void func_ov075_02114b60(UnknownVsPlayer *p);
void func_ov075_02114be4(UnknownVsPlayer *p);
void func_ov075_02114cd8(UnknownVsPlayer *p);
int func_ov075_02114ddc(UnknownVsPlayer *p, unsigned char kind, unsigned char playerNo, int x);
void func_ov075_02114894(UnknownVsPlayer *p);
void func_ov075_02114904(UnknownVsPlayer *p, int exitX);
int func_ov075_02114988(UnknownVsPlayer *p);
void func_ov075_021149d0(UnknownVsPlayer *p, int targetX);
int func_ov075_02114a58(UnknownVsPlayer *p);
void func_ov075_021151b4(dEntObj_c *self, int playerNo);
int func_ov075_0211524c(dEntObj_c *self, int playerNo);
int func_ov075_02115290(dEntObj_c *self, int playerNo);
void func_ov075_021152d4(dEntObj_c *self);
}

/* A pointer-to-member row: the two per-figure state tables are arrays of
 * these. __sinit_ov075_0211b5e0 fills them, which fixes each index:
 *
 *   data_ov075_0211d56c, movement (mMoveState):
 *     0 none, 1 func_ov075_021147d4, 2 func_ov075_0211478c,
 *     3 func_ov075_0211473c, 4 none, 5 func_ov075_02114560, 6 none,
 *     7 func_ov075_021143e4, 8 func_ov075_02114390, 9 func_ov075_02114300
 *   data_ov075_0211d53c, animation (mAnimState):
 *     0 none, 1 func_ov075_0211427c, 2 func_ov075_02114218,
 *     3 func_ov075_021141b8, 4 func_ov075_021140e4, 5 func_ov075_02114010
 *
 * "none" rows point at the empty func_ov075_02114890 / func_ov075_021142fc. */
typedef void (UnknownVsPlayer::*UnknownVsPlayerState)();
struct UnknownVsPlayerStateRow { UnknownVsPlayerState state; };
extern "C" UnknownVsPlayerStateRow data_ov075_0211d56c[];
extern "C" UnknownVsPlayerStateRow data_ov075_0211d53c[];

typedef struct S48 { int w[12]; } S48;

#define LAUNDER(x) ((char*)(x))

#pragma defer_codegen off

/* -------------------------------------------------------------------------- */
/* 0x02113ee0 _ZN9dEntObj_cD1Ev, 0x02113f54 _ZN9dEntObj_cD0Ev                 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN9dEntObj_cD1Ev
dEntObj_c::~dEntObj_c()
{
}

// @symbol _ZN9dEntObj_cD0Ev
/* The deleting destructor (D0) has no source of its own: the compiler emits
   it from the definition above. */

/* -------------------------------------------------------------------------- */
/* 0x02113fdc _ZN15UnknownVsPlayerD1Ev                                        */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15UnknownVsPlayerD1Ev
UnknownVsPlayer::~UnknownVsPlayer()
{
}

/* Animation state 5, the fast gait. Below half speed it drops back to the
 * state-4 gait; the playback rate follows the speed, and frames 4 and 0x22
 * (the footfalls) play a step sound at the figure's screen position. */
// @symbol func_ov075_02114010
extern "C" void func_ov075_02114010(UnknownVsPlayer *p)
{
    if (p->mSpeed < p->mMaxSpeed / 2) {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
            &p->mModel, (void *)data_ov075_0211d424[1], 4, 0, 0x1000, 0);
        p->mAnimState = 4;
    }
    p->mModel.speed = cstd::fdiv(p->mSpeed, p->mMaxSpeed) + 0x1000;
    if (!p->mModel.WillHitFrame(4)) {
        if (p->mModel.WillHitFrame(0x22) == 0) return;
    }
    Vector3 screenPos;
    _ZN7Clipper13Func_02015560ER9Matrix4x3R7Vector35Fix12IiES3_(
        &data_0209f43c, data_0209b3ec, &p->mPosition, 0, &screenPos);
    func_0201251c(0, 0x20, &screenPos, p->mSpeed);
}

/* Animation state 4, the slow gait: the mirror of state 5, switching up to
 * it at half speed. */
// @symbol func_ov075_021140e4
extern "C" void func_ov075_021140e4(UnknownVsPlayer *p)
{
    if (p->mSpeed >= p->mMaxSpeed / 2) {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
            &p->mModel, (void *)data_ov075_0211d42c[1], 4, 0, 0x1000, 0);
        p->mAnimState = 5;
    }
    p->mModel.speed = cstd::fdiv(p->mSpeed, p->mMaxSpeed) + 0x1000;
    if (!p->mModel.WillHitFrame(4)) {
        if (p->mModel.WillHitFrame(0x22) == 0) return;
    }
    Vector3 screenPos;
    _ZN7Clipper13Func_02015560ER9Matrix4x3R7Vector35Fix12IiES3_(
        &data_0209f43c, data_0209b3ec, &p->mPosition, 0, &screenPos);
    func_0201251c(0, 0x20, &screenPos, p->mSpeed);
}

/* Animation state 3: once the current animation finishes, switch to the
 * one in data_ov075_0211d3ec and drop to state 0. */
// @symbol func_ov075_021141b8
extern "C" int func_ov075_021141b8(UnknownVsPlayer *p)
{
    int finished = p->mModel.Finished();
    if (!finished) return finished;
    _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
        &p->mModel, (void *)data_ov075_0211d3ec[1], 4, 0, 0x1000, 0);
    p->mAnimState = 0;
    return 0;
}

/* Animation state 2: once the current one-shot finishes, start the next one
 * and hand over to state 3. */
// @symbol func_ov075_02114218
extern "C" int func_ov075_02114218(UnknownVsPlayer *p)
{
    int finished = p->mModel.Finished();
    if (!finished) return finished;
    _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
        &p->mModel, (void *)data_ov075_0211d3a4[1], 4, 0x40000000, 0x1000, 0);
    p->mAnimState = 3;
    return 3;
}

/* Animation state 1: step sounds on frames 4 and 0x22 while walking. */
// @symbol func_ov075_0211427c
extern "C" int func_ov075_0211427c(UnknownVsPlayer *p)
{
    int screenPos[2];
    if (p->mModel.WillHitFrame(4) == 0) {
        int hit = p->mModel.WillHitFrame(0x22);
        if (hit == 0) return hit;
    }
    _ZN7Clipper13Func_02015560ER9Matrix4x3R7Vector35Fix12IiES3_(
        &data_0209f43c, data_0209b3ec, &p->mPosition, 0, screenPos);
    return func_0201251c(0, 0x20, screenPos, p->mSpeed);
}

// @symbol func_ov075_021142fc
extern "C" void func_ov075_021142fc(void)
{
}

/* Movement state 9: until the figure is past x = 0x1c2000, keep its
 * particle effect on bone 15. The bone's translation is carried into the
 * scene by the model matrix, then scaled back up by 8 (the render matrices
 * hold positions >> 3). */
// @symbol func_ov075_02114300
extern "C" void func_ov075_02114300(UnknownVsPlayer *p)
{
    if (p->mPosition.x >= 0x1c2000) return;
    Vector3 pos;
    Matrix4x3 *bone = &p->mModel.data.transforms[15];
    MulVec3Mat4x3(&bone->t, &p->mModel.mat4x3, &pos);
    Vec3_MulScalarInPlace(&pos, 0x8000);
    pos.y += 0x28000;
    p->mParticleHandle = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
        p->mParticleHandle, 0x140, pos.x, pos.y, pos.z, (Vector3_16 *)0, (void *)0);
}

/* Movement state 8: frame 0x14 of the animation comes round twice; the
 * first pass only arms the flag, the second plays the player's sound
 * 0x1c and stops. */
// @symbol func_ov075_02114390
extern "C" void func_ov075_02114390(UnknownVsPlayer *p)
{
    if (!p->mModel.WillHitFrame(0x14)) return;
    if (p->mInAir == 0) {
        p->mInAir = 1;
        return;
    }
    func_02012174(p->mPlayerNo, 0x1c);
    p->mMoveState = 0;
}

/* Movement state 7: pick the pose. Characters flagged in data_0209b2f0 take
 * the one fixed animation and go to state 8; the rest draw two bits at a
 * time from a shared random pattern (refilled from data_ov075_0211b524 when
 * it runs out) and go to state 9. The texture sequence starts at a
 * random frame. */
// @symbol func_ov075_021143e4
extern "C" void func_ov075_021143e4(UnknownVsPlayer *p)
{
    void *btp;
    if (data_ov075_0211d380 < 0) {
        data_ov075_0211d380 = data_ov075_0211b524[
            ((unsigned int)RandomIntInternal(&data_0209e650) >> 0x10) % 6];
    }
    if (data_0209b2f0[p->mPlayerNo] != 0) {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
            &p->mModel, (void *)data_ov075_0211d3ac[1], 0, 0, 0x1000, 0);
        btp = (void *)data_ov075_0211d40c[1];
        p->mMoveState = 8;
    } else {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
            &p->mModel, (void *)(data_ov075_0211c688[data_ov075_0211d380 & 3])[1], 0, 0, 0x1000, 0);
        btp = (void *)data_ov075_0211d3dc[1];
        p->mMoveState = 9;
        data_ov075_0211d380 >>= 2;
    }
    TextureSequence::Prepare(*(BMD_File *)data_ov075_0211d3c4[1], *(BTP_File *)btp);
    _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(&p->mTextureSequence, btp, 0, 0x1000, 0);
    {
        unsigned int rv = (unsigned int)RandomIntInternal(&data_0209e650);
        int frames = p->mModel.GetFrameCount();
        long long v = __aeabi_uidiv(rv >> 0x10, frames);
        p->mTextureSequence.currFrame = ((unsigned)((int)(v >> 32) << 0x10)) >> 4;
    }
}

/* Movement state 5: run off the front of the stage. Past the player's
 * take-off line the figure jumps; it walks along its heading, falls under
 * gravity once in the air, and on dropping below the stage sets the
 * fell-off flags and plays the fall sound. Far enough down it parks in
 * state 6. */
// @symbol func_ov075_02114560
extern "C" void func_ov075_02114560(UnknownVsPlayer *p)
{
    if (p->mPosition.z < data_ov075_0211b534[p->mPlayerNo] && p->mInAir == 0) {
        p->mVertSpeed = 0x2a000;
        p->mInAir = 1;
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
            &p->mModel, (void *)data_ov075_0211d41c[1], 4, 0x40000000, 0x1000, 0);
        p->mAnimState = 0;
    }

    p->mPosition.x += (int)(((long long)p->mSpeed
        * data_02082214[((unsigned short)p->mAngleY >> 4) * 2] + 0x800) >> 12);
    p->mPosition.y += p->mVertSpeed;
    p->mPosition.z += (int)(((long long)p->mSpeed
        * data_02082214[((unsigned short)p->mAngleY >> 4) * 2 + 1] + 0x800) >> 12);

    if (p->mPosition.y < 0) {
        p->mPosition.y = 0;
        p->mVertSpeed = 0;
        p->mInAir = 0;
    }

    if (p->mInAir == 0) {
        p->mSpeed += 0x2000;
        if (p->mSpeed > p->mMaxSpeed)
            p->mSpeed = p->mMaxSpeed;
    }

    p->mVertSpeed -= 0x4000;
    if (p->mVertSpeed < -0x1e000)
        p->mVertSpeed = -0x1e000;

    if (p->mPosition.z < -0x30c000) {
        if (p->mFellOff == 0) {
            p->mFellOff = 1;
            p->unk_155 = 1;
            func_02012790(0x121);
        }
    }

    if (p->mPosition.z < -0x3e8000) {
        p->mMoveState = 6;
        p->mAnimState = 0;
    }
}

/* Movement state 3: turn toward the exit at the player's turn rate, then
 * hand over to state 4. */
// @symbol func_ov075_0211473c
extern "C" void func_ov075_0211473c(UnknownVsPlayer *p)
{
    s16 angle = Vec3_HorzAngle(&p->mPosition, &p->mExitPos);
    unsigned char playerNo = p->mPlayerNo;
    short step = data_ov075_0211b52c[playerNo];
    if (ApproachLinear(p->mAngleY, angle, step)) {
        p->mMoveState = 4;
        p->mAnimState = 0;
    }
}

/* Movement state 2: wait out mWaitTimer, then face forward and turn toward
 * the exit (state 3) in the slow gait. */
// @symbol func_ov075_0211478c
extern "C" void func_ov075_0211478c(UnknownVsPlayer *p)
{
    int waiting = DecIfAbove0_Byte(&p->mWaitTimer);
    if (waiting) return;
    p->mAngleY = 0;
    p->mMoveState = 3;
    p->mAnimState = 4;
}

/* Movement state 1: turn toward the target slot, walk there along x without
 * overshooting, turn back to face the camera, and go idle. */
// @symbol func_ov075_021147d4
extern "C" void func_ov075_021147d4(UnknownVsPlayer *p)
{
    if (p->mPosition.x != p->mTargetPos.x) {
        short angle = Vec3_HorzAngle(&p->mPosition, &p->mTargetPos);
        if (ApproachLinear(p->mAngleY, angle, 0x800) == 0) return;
        p->mPosition.x += p->mSpeed;
        if (p->mSpeed >= 0) {
            if (p->mPosition.x > p->mTargetPos.x) p->mPosition.x = p->mTargetPos.x;
            return;
        }
        if (p->mPosition.x < p->mTargetPos.x) p->mPosition.x = p->mTargetPos.x;
        return;
    }
    if (ApproachLinear(p->mAngleY, 0, 0x800) == 0) return;
    func_ov075_02114a6c(p);
}

// @symbol func_ov075_02114890
extern "C" void func_ov075_02114890(void)
{
}

/* Start the run off the stage (movement state 5) from a standstill. */
// @symbol func_ov075_02114894
extern "C" void func_ov075_02114894(UnknownVsPlayer *p)
{
    p->mSpeed = 0;
    p->mVertSpeed = 0;
    _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
        &p->mModel, (void *)data_ov075_0211d424[1], 4, 0, 0x1000, 0);
    p->mMoveState = 5;
    p->mAnimState = 4;
    p->mInAir = 0;
}

/* Waiting at the exit (movement state 4)? */
// @symbol func_ov075_021148f0
extern "C" int func_ov075_021148f0(UnknownVsPlayer *p)
{
    return p->mMoveState == 4;
}

/* Send the figure toward the exit at x = exitX after a short random wait
 * (movement state 2). */
// @symbol func_ov075_02114904
extern "C" void func_ov075_02114904(UnknownVsPlayer *p, int exitX)
{
    _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
        &p->mModel, (void *)data_ov075_0211d384[1], 8, 0x40000000, 0x1000, 0);
    p->mWaitTimer = (((unsigned int)RandomIntInternal(&data_0209e650) >> 0x10) + 0x1e) & 0xf;
    p->mExitPos.x = exitX;
    p->mExitPos.y = 0;
    p->mExitPos.z = -0x3e8000;
    p->mMoveState = 2;
    p->mAnimState = 0;
}

/* The selected figure's one-shot, followed by animation state 3. */
// @symbol func_ov075_02114988
extern "C" int func_ov075_02114988(UnknownVsPlayer *p)
{
    _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
        &p->mModel, (void *)data_ov075_0211d3a4[1], 8, 0x40000000, 0x1000, 0);
    p->mAnimState = 3;
    return 3;
}

/* Walk to a new slot at x = targetX (movement state 1). */
// @symbol func_ov075_021149d0
extern "C" void func_ov075_021149d0(UnknownVsPlayer *p, int targetX)
{
    if (p->mTargetPos.x == targetX)
        return;
    p->mTargetPos.x = targetX;
    if (targetX >= p->mPosition.x)
        p->mSpeed = 0x8000;
    else
        p->mSpeed = -0x8000;
    p->mMoveState = 1;
    p->mAnimState = 1;
    _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
        &p->mModel, (void *)data_ov075_0211d42c[1], 4, 0, 0x1000, 0);
    p->mModel.speed = 0x1000;
}

/* Idle (movement state 0)? */
// @symbol func_ov075_02114a58
extern "C" int func_ov075_02114a58(UnknownVsPlayer *p)
{
    return p->mMoveState == 0;
}

/* Go idle in the player's own standing animation. */
// @symbol func_ov075_02114a6c
extern "C" int func_ov075_02114a6c(UnknownVsPlayer *p)
{
    unsigned char playerNo = p->mPlayerNo;
    _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
        &p->mModel, (void *)data_ov075_0211c678[playerNo][1], 4, 0, 0x1000, 0);
    p->mModel.speed = 0x1000;
    p->mMoveState = 0;
    p->mAnimState = 0;
    return 0;
}

// @symbol func_ov075_02114ac4
/* Camera follow for the focused figure. While it runs off (movement state 5)
 * the eye pulls back with it; once it has dropped away (state 6) the eye
 * rises and eases toward the target's depth, and the target follows the
 * eye's height. Returns nonzero when the view needs rebuilding. */
extern "C" int func_ov075_02114ac4(UnknownVsPlayer *p, Vector3 *at, Vector3 *pos)
{
    int state = p->mMoveState;
    if (state == 5) {
        int back = cstd::fdiv(p->mPosition.z, 0x19000);
        pos->z = back + 0x50000;
        return 1;
    }
    if (state != 6)
        return 0;
    Math_Function_0203b0fc((int *)&pos->y, 0x2bc00, 0x66, 0x1c00);
    Math_Function_0203b14c((int *)&pos->z, at->z, 1, 0x4000, 0x100);
    at->y = pos->y;
    return 1;
}

// @symbol func_ov075_02114b60
/* Put the model's bone translations back to the file's rest pose: bone 0,
 * then bones 2 onward (bone 1 keeps whatever the animation gave it). The
 * file's bone records are 0x40 bytes with the translation at +0x24; the
 * model's are 0x34 bytes with it at +0x20. */
struct VsBoneFileRecord {
    char _pad0[0x24];
    int x, y, z;
    char _pad30[0x10];
};
struct VsBoneRecord {
    char _pad0[0x20];
    int x, y, z;
    char _pad2c[0x8];
};
struct VsBoneFile {
    char _pad0[4];
    u32 numBones;
    VsBoneFileRecord *bones;
};
struct VsBoneComponents {
    VsBoneFile *file;
    char _pad0[4];
    VsBoneRecord *bones;
};
extern "C" void func_ov075_02114b60(UnknownVsPlayer *p)
{
    VsBoneComponents *q = (VsBoneComponents *)&p->mModel.data;
    VsBoneRecord *dst;
    VsBoneFile *file = q->file;
    VsBoneRecord *d = q->bones;
    VsBoneFileRecord *src = file->bones;
    VsBoneFileRecord *from;
    s16 i;
    d->x = src->x;
    d->y = src->y;
    d->z = src->z;
    dst = d + 2;
    from = src + 2;
    for (i = 2; i < file->numBones; i++) {
        dst->x = from->x;
        dst->y = from->y;
        dst->z = from->z;
        dst++;
        from++;
    }
}

// @symbol func_ov075_02114be4
/* Draw one figure: restore its bone translations, pose it, push the palette
 * value into every material and draw the body; then the second model takes
 * the body's matrix and is drawn with the texture sequence applied. */
extern "C" void func_ov075_02114be4(UnknownVsPlayer *p)
{
    BMD_File *file;
    char *mat;

    func_020167a4(&p->mModel);
    func_ov075_02114b60(p);
    func_0204531c((char *)&p->mModel.data, p->mModel.blendWeight);

    {
        ModelComponents *data = (ModelComponents *)LAUNDER(&p->mModel.data);
        unsigned int i;
        file = data->modelFile;
        mat = (char *)data->materials;
        for (i = 0; i < file->numMaterials; i++) {
            *(int *)(mat + 0x20) = p->mMaterialColor;
            mat += sizeof(BMD_Material);
        }
        p->mModel.Model::Render(0);
        ModelAnim *anim = &p->mAnimation;
        anim->Virtual10(data->transforms[15]);
    }

    *(S48 *)&p->mAnimation.mat4x3 = *(S48 *)LAUNDER(&p->mModel.mat4x3);

    {
        ModelComponents *data = (ModelComponents *)LAUNDER(&p->mAnimation.data);
        unsigned int i;
        file = data->modelFile;
        mat = (char *)data->materials;
        for (i = 0; i < file->numMaterials; i++) {
            *(int *)(mat + 0x20) = p->mMaterialColor;
            mat += sizeof(BMD_Material);
        }
    }

    p->mAnimation.Model::Render(0);
    p->mTextureSequence.Update(p->mAnimation.data);
}

// @symbol func_ov075_02114cd8
/* One figure's frame: run its movement state, rebuild the model matrix from
 * position and heading, run its animation state, then advance both
 * animations and place the shadow. */
extern "C" void func_ov075_02114cd8(UnknownVsPlayer *p)
{
    p->unk_155 = 0;
    (p->*data_ov075_0211d56c[p->mMoveState].state)();
    Matrix4x3_FromTranslation(&data_020a0e68, p->mPosition.x >> 3,
                              p->mPosition.y >> 3, p->mPosition.z >> 3);
    Matrix4x3_ApplyInPlaceToRotationY(&data_020a0e68, p->mAngleY);
    *(S48 *)&p->mModel.mat4x3 = *(S48 *)&data_020a0e68;
    (p->*data_ov075_0211d53c[p->mAnimState].state)();
    p->mModel.Advance();
    p->mTextureSequence.Advance();
    _ZN11ShadowModel9InitModelEP9Matrix4x35Fix12IiES3_S3_j(
        &p->mShadow, &p->mModel.mat4x3, 0x50000, 0x1f4000, 0x50000, 0xf);
}

// @symbol func_ov075_02114ddc
/* Set up the figure for player slot playerNo at x: both models, the pose
 * animation for this kind of entry scene, the texture sequence at a random
 * frame, the shadow and the start position. Kinds 0 and 2 start idle; the
 * others start in movement state 7. */
extern "C" int func_ov075_02114ddc(UnknownVsPlayer *p, unsigned char kind, unsigned char playerNo, int x)
{
    void *btp;

    p->mModel.SetFile((BMD_File *)data_ov075_0211d404[1], 1, 1);
    p->mMaterialColor = *(int *)((char *)p->mModel.data.materials + 0x20) + (playerNo << 1);
    p->mAnimation.SetFile((BMD_File *)data_ov075_0211d3c4[1], 1, 1);
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(
        &p->mAnimation, (void *)data_ov075_0211d414[1], 0, 0x1000, 0);

    if (kind == 0 || kind == 2) {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
            &p->mModel, (void *)data_ov075_0211c678[playerNo][1], 0, 0, 0x1000, 0);
        btp = (void *)data_ov075_0211d40c[1];
        p->mMoveState = 0;
    } else {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
            &p->mModel, (void *)data_ov075_0211d3ac[1], 0, 0, 0x1000, 0);
        btp = (void *)data_ov075_0211d40c[1];
        p->mMoveState = 7;
    }

    TextureSequence::Prepare(*(BMD_File *)data_ov075_0211d3c4[1], *(BTP_File *)btp);
    _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(&p->mTextureSequence, btp, 0, 0x1000, 0);

    {
        unsigned rv = (unsigned)RandomIntInternal(&data_0209e650);
        int frames = p->mModel.GetFrameCount();
        p->mTextureSequence.currFrame = (((rv >> 0x10) % (unsigned)frames) << 0x10) >> 4;
    }

    if (p->mShadow.InitCylinder() == 0)
        return 0;

    p->mPosition.x = x;
    p->mPosition.y = 0;
    p->mPosition.z = 0;
    p->mTargetPos.x = p->mPosition.x;
    p->mTargetPos.y = p->mPosition.y;
    p->mTargetPos.z = p->mPosition.z;
    p->mMaxSpeed = data_ov075_0211b544[playerNo];
    p->mAnimState = 0;
    p->mPlayerNo = playerNo;
    return 1;
}

// @symbol func_ov075_02114fa8
/* Send every present player's figure toward its exit, and pick the one the
 * camera follows: player 2, else 1, else 3, else 0. */
extern "C" void func_ov075_02114fa8(void *arg)
{
    dEntObj_c *self = (dEntObj_c *)arg;
    int i;
    unsigned char *present = data_0209fc5c;
    UnknownVsPlayer *player = self->mPlayers;
    for (i = 0; i < 4; i++) {
        if (*present != 0) {
            int x = func_ov075_0211524c(self, i);
            func_ov075_02114904(player, x);
        }
        present++;
        player++;
    }

    if (data_0209fc5c[2] != 0)
        self->mFocusedPlayer = 2;
    else if (data_0209fc5c[1] != 0)
        self->mFocusedPlayer = 1;
    else if (data_0209fc5c[3] != 0)
        self->mFocusedPlayer = 3;
    else
        self->mFocusedPlayer = 0;
    self->mAnimActive = 0;
    self->mState = 1;
}

// @symbol func_ov075_0211505c
/* Stop the selection animation and return the chosen figure to idle. */
extern "C" void func_ov075_0211505c(char *arg)
{
    dEntObj_c *self = (dEntObj_c *)arg;
    int r;
    if (self->mAnimActive == 0) return;
    r = func_0203da9c();
    /* The call leaves the element stride in r1; the cartridge passes it. */
    ((void (*)(UnknownVsPlayer *, int))func_ov075_02114a6c)(&self->mPlayers[r], sizeof(UnknownVsPlayer));
    self->mAnimActive = 0;
}

// @symbol func_ov075_02115098
/* Once every figure is idle, play the selection one-shot on player
 * playerNo's figure (if it is this console's player) and put the selection
 * model over it. Returns 0 while any figure is still moving. */
extern "C" int func_ov075_02115098(char *arg, int playerNo)
{
    dEntObj_c *self = (dEntObj_c *)arg;
    if (self->mAnimActive != 0)
        return 1;
    int i;
    UnknownVsPlayer *player = self->mPlayers;
    for (i = 0; i < 4; i++) {
        if (func_ov075_02114a58(player) == 0)
            return 0;
        player++;
    }
    int me = func_0203da9c();
    if (playerNo == me) {
        func_ov075_02114988(&self->mPlayers[playerNo]);
        func_ov075_021151b4(self, playerNo);
        self->mAnimActive = 1;
    }
    return 1;
}

// @symbol func_ov075_02115134
/* Walk every figure to its slot for the current player count, and play the
 * join or leave sound when the count changed. */
extern "C" void func_ov075_02115134(char *arg)
{
    dEntObj_c *self = (dEntObj_c *)arg;
    s32 i;
    UnknownVsPlayer *player = self->mPlayers;
    u8 count, last;
    for (i = 0; i < 4; i++) {
        func_ov075_021149d0(player, func_ov075_02115290(self, i));
        player++;
    }
    count = data_0209fc50;
    last = self->mPlayerCount;
    if (last > count)
        func_02012790(0x12a);
    else if (last < count)
        func_02012790(0x129);
    self->mPlayerCount = count;
}

// @symbol func_ov075_021151b4
/* Put the selection model over player playerNo's figure, 0x32 units up. */
extern "C" void func_ov075_021151b4(dEntObj_c *self, int playerNo)
{
    UnknownVsPlayer *player = self->mPlayers + playerNo;
    Vector3 *src = &player->mPosition;
    Vector3 v, out;

    v.x = src->x;
    v.y = src->y;
    v.z = src->z;
    v.y += 0x32000;
    Vec3_Asr(&out, &v, 3);
    Matrix4x3_FromTranslation(&data_020a0e68, out.x, out.y, out.z);
    *(S48 *)&self->mModelAnim.mat4x3 = *(S48 *)&data_020a0e68;
}

// @symbol func_ov075_0211524c
/* The exit x for player slot playerNo, from the row for the current player
 * count (data_0209fc64 maps a slot to its column; -1 means the last). */
extern "C" int func_ov075_0211524c(dEntObj_c *self, int playerNo)
{
    unsigned int count = data_0209fc50;
    int row;
    if (count <= 1) {
        row = 0;
    } else {
        row = count - 1;
        playerNo = (unsigned char)data_0209fc64[playerNo];
        if (playerNo < 0) playerNo = 3;
    }
    return *(int *)((char *)data_ov075_0211b594 + row * 16 + playerNo * 4);
}

// @symbol func_ov075_02115290
/* The standing x for player slot playerNo, laid out the same way as the
 * exit table above. */
extern "C" int func_ov075_02115290(dEntObj_c *self, int playerNo)
{
    int count = data_0209fc50;
    int row;
    if ((unsigned int)count <= 1) {
        row = 0;
    } else {
        row = count - 1;
        playerNo = data_0209fc64[playerNo];
        if (playerNo < 0) playerNo = 3;
    }
    return data_ov075_0211b554[row][playerNo];
}

// @symbol func_ov075_021152d4
/* Rebuild the projection and the view from the camera eye and target. */
extern "C" void func_ov075_021152d4(dEntObj_c *self)
{
    Matrix4x3 view;
    _ZN3G3i13PerspectiveW_E5Fix12IiES1_S1_S1_S1_S1_bP9Matrix4x3(
        data_02082614[0xa], data_02082614[0xb], 0x1555, 0x1000, 0x1388000, 0x1000, true, (Matrix4x3 *)0);
    G3i::LookAt_((Vector3 *)&self->mCamPosX, (Vector3 *)&data_ov075_0211c660,
                 (Vector3 *)&self->mCamTargetX, true, &view);
    CopyToViewMat(&view);
    data_0209f43c.Func_020156DC(0x1555, 0x105b, 0x1000, 0x1388000);
}

/* -------------------------------------------------------------------------- */
/* 0x02115388 _ZN9dEntObj_c16CleanupResourcesEv                               */
/* -------------------------------------------------------------------------- */
// @symbol _ZN9dEntObj_c16CleanupResourcesEv
int dEntObj_c::CleanupResources()
{
    CleanCommonModelDataArr();
    ((SharedFilePtr *)(data_ov075_0211d404))->Release();
    ((SharedFilePtr *)(data_ov075_0211d3c4))->Release();
    ((SharedFilePtr *)(data_ov075_0211d414))->Release();
    if (param1 != 1) {
        ((SharedFilePtr *)(data_ov075_0211d394))->Release();
        ((SharedFilePtr *)(data_ov075_0211d3cc))->Release();
        ((SharedFilePtr *)(data_ov075_0211d39c))->Release();
        ((SharedFilePtr *)(data_ov075_0211d3d4))->Release();
        ((SharedFilePtr *)(data_ov075_0211d3a4))->Release();
        ((SharedFilePtr *)(data_ov075_0211d3ec))->Release();
        ((SharedFilePtr *)(data_ov075_0211d384))->Release();
        ((SharedFilePtr *)(data_ov075_0211d424))->Release();
        ((SharedFilePtr *)(data_ov075_0211d42c))->Release();
        ((SharedFilePtr *)(data_ov075_0211d41c))->Release();
    } else {
        ((SharedFilePtr *)(data_ov075_0211d3ac))->Release();
        ((SharedFilePtr *)(data_ov075_0211d3b4))->Release();
        ((SharedFilePtr *)(data_ov075_0211d3f4))->Release();
        ((SharedFilePtr *)(data_ov075_0211d38c))->Release();
        ((SharedFilePtr *)(data_ov075_0211d3dc))->Release();
    }
    ((SharedFilePtr *)(data_ov075_0211d40c))->Release();
    ((SharedFilePtr *)(data_ov075_0211d3fc))->Release();
    if (param1 != 1) {
        ((SharedFilePtr *)(data_ov075_0211d3bc))->Release();
        ((SharedFilePtr *)(data_ov075_0211d3e4))->Release();
    }
    func_ov075_0211b3b8(&unk_e80);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* 0x021154cc _ZN9dEntObj_c6RenderEv                                          */
/* -------------------------------------------------------------------------- */
// @symbol _ZN9dEntObj_c6RenderEv
int dEntObj_c::Render()
{
    mModel.Render(0);
    ShadowModel::RenderAll();
    mParticles.Update();
    int i = 0;
    UnknownVsPlayer *player = mPlayers;
    do {
        func_ov075_02114be4(player);
        i++;
        player++;
    } while (i < 4);
    if (mAnimActive) {
        mModelAnim.Render(0);
    }
    func_ov075_0211b3d8(&unk_e80);
    Particle::RenderAll();
    return 1;
}

/* -------------------------------------------------------------------------- */
/* 0x0211555c _ZN9dEntObj_c8BehaviorEv                                        */
/* -------------------------------------------------------------------------- */
// @symbol _ZN9dEntObj_c8BehaviorEv
int dEntObj_c::Behavior()
{
    if (mSuspended == 0) {
        int i;
        UnknownVsPlayer *player;
        char *walk;
        u8 *present;
        int allAtExit;
        present = data_0209fc5c;
        walk = (char *)this;
        player = mPlayers;
        allAtExit = 1;
        i = 0;
        for (; i < 4; i++, player++, walk += sizeof(UnknownVsPlayer), present += 1) {
            func_ov075_02114cd8(player);
            if (((dEntObj_c *)walk)->mPlayers[0].unk_155) {
                Vector3 *src = &player->mPosition;
                int pos[3];
                pos[0] = src->x;
                pos[1] = src->y;
                pos[2] = src->z;
                func_ov075_0211ab38(&unk_e80, pos);
            }
            if (*present) {
                if (func_ov075_021148f0(player) == 0)
                    allAtExit = 0;
            }
        }

        if (mState == 1 && allAtExit != 0) {
            int j = 0;
            u8 *present2 = data_0209fc5c;
            UnknownVsPlayer *player2 = mPlayers;
            for (; j < 4; j++) {
                if (*present2) func_ov075_02114894(player2);
                present2 += 1;
                player2++;
            }
            mState = 2;
        }
        if (mState != 0) {
            if (func_ov075_02114ac4(&mPlayers[mFocusedPlayer], (Vector3 *)&mCamTargetX,
                                    (Vector3 *)&mCamPosX) != 0)
                func_ov075_021152d4(this);
        }
        if (mAnimActive) {
            int me = func_0203da9c();
            func_ov075_021151b4(this, me);
            mModelAnim.Advance();
        }
        func_ov075_0211b418(&unk_e80);
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* 0x021156e0 _ZN9dEntObj_c13InitResourcesEv                                  */
/* -------------------------------------------------------------------------- */
// @symbol _ZN9dEntObj_c13InitResourcesEv
int dEntObj_c::InitResources()
{
    int i; int kind; UnknownVsPlayer* player;

    InitialiseVramGlobals();
    Model::LoadFile(*(SharedFilePtr*)data_ov075_0211d3fc);
    if (param1 != 1) {
        Model::LoadFile(*(SharedFilePtr*)data_ov075_0211d3bc);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d3e4);
    }
    Model::LoadFile(*(SharedFilePtr*)data_ov075_0211d404);
    Model::LoadFile(*(SharedFilePtr*)data_ov075_0211d3c4);
    Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d414);

    if (param1 != 1) {
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d394);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d3cc);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d39c);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d3d4);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d3a4);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d3ec);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d384);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d424);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d42c);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d41c);
    } else {
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d3ac);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d3b4);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d3f4);
        Animation::LoadFile(*(SharedFilePtr*)data_ov075_0211d38c);
        TextureSequence::LoadFile(*(SharedFilePtr*)data_ov075_0211d3dc);
    }

    TextureSequence::LoadFile(*(SharedFilePtr*)data_ov075_0211d40c);

    _ZN3G3X6SetFogEbiii(0, 0, 2, 0x1000);
    ShadowModel::CleanAll();

    mModel.SetFile(*(BMD_File**)((char*)data_ov075_0211d3fc + 4), 1, -1);

    func_0203c178(&data_020a0e68, 0x7d000, 0x7d000, 0x7d000);
    /* 0x888 is +0x1c inside the Model at 0x86c -- its mat4x3. The cartridge's own
       ~dEntObj_c proves the extent; see tools/dtor_members.py. */
    *(S48*)((char*)&mModel.mat4x3) = *(S48*)&data_020a0e68;

    if (param1 != 1) {
        mModelAnim.SetFile(*(BMD_File**)((char*)data_ov075_0211d3bc + 4), 1, -1);
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&mModelAnim, *(void**)((char*)data_ov075_0211d3e4 + 4), 0, 0x1000, 0);
    }

    func_ov075_0211b458((char*)&unk_e80, (int*)&data_ov075_0211c654, 0);
    mParticles.Initialise();

    player = mPlayers;
    i = 0;
    do {
        kind = param1;
        int r = func_ov075_02115290(this, i);
        if (!func_ov075_02114ddc(player, kind, i, r))
            return 0;
        i++;
        player++;
    } while (i < 4);

    data_ov075_0211d380 = -1;
    mAnimActive = 0;

    if (param1 == 2) {
        int v = func_0203da9c();
        func_ov075_02115098((char*)this, v);
    }

    mCamTargetX = 0;
    mCamPosX = mCamTargetX;
    mCamTargetY = 0x14000;
    mCamPosY = mCamTargetY;
    mCamPosZ = 0x50000;
    mCamTargetZ = -0x8000;

    func_ov075_021152d4(this);

    mSuspended = 0;
    mState = 0;
    mFocusedPlayer = 0;
    mPlayerCount = data_0209fc50;
    if (mPlayerCount < 1)
        mPlayerCount = 1;

    return 1;
}

/* -------------------------------------------------------------------------- */
/* 0x021159f4 dEntObj_c_classInit                                             */
/* -------------------------------------------------------------------------- */
/* Reconstructed source-style name: SM64DS proves dEntObj_c through RTTI,
 * allocation size, vtable identity, and the ENTRY_OBJECT registry profile;
 * later EAD lineage supplies classInit. Exact original spelling is not
 * preserved. The array runtime passes each element address and ignores
 * lifecycle results; explicit function-pointer casts mark that runtime ABI
 * boundary. */
// @symbol dEntObj_c_classInit
extern "C" dEntObj_c* dEntObj_c_classInit(void){
  dEntObj_c* p = (dEntObj_c*)_ZN7fBase_cnwEj(sizeof(dEntObj_c));
  if (p) {
    _ZN7fBase_cC2Ev(p);
    *(void**)p = &data_0208e4b8;
    *(void**)p = _ZTV9dEntObj_c + 2;
    _ZN8Particle10SysTrackerC1Ev(&p->mParticles);
    _ZN5ModelC1Ev(&p->mModel);
    _ZN9ModelAnimC1Ev(&p->mModelAnim);
    __cxa_vec_ctor(p->mPlayers, 4, sizeof(UnknownVsPlayer),
                  (void (*)(void *))_ZN15UnknownVsPlayerC1Ev,
                  (void (*)(void *))_ZN15UnknownVsPlayerD1Ev);
  }
  return p;
}

/* -------------------------------------------------------------------------- */
/* 0x02115a88 _ZN15UnknownVsPlayerC1Ev                                        */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15UnknownVsPlayerC1Ev
UnknownVsPlayer::UnknownVsPlayer()
{
}
