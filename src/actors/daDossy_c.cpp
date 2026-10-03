//cpp
/* daDossy_c (DOSSY, Dorrie) and daDossyCap_c (DOSSY_CAP, the cap she
 * wears), with the DorriePlatform element type of daDossy_c::mPlatforms.
 * One translation unit: ov065 0x02117f40..0x021196d8, 29 functions.
 *
 * WHY ONE FILE. The run is one contiguous linker unit that holds both
 * classes' destructors, classInit factories and virtuals, the platform
 * element's C1/D1, and the state helpers daDossy_c's state table
 * (data_ov065_0211d7fc) dispatches to. Both class names are the cartridge's
 * own RTTI spellings: _ZTS9daDossy_c at 0x0211cd34 and _ZTS12daDossyCap_c at
 * 0x0211cd58, each __si_class_type_info naming _ZTI8dActor_c as its base.
 * Both destructors are inline in their class bodies and InitResources is the
 * first non-inline virtual of each, so this TU owns both vtables
 * (_ZTV9daDossy_c at 0x0211ce48, _ZTV12daDossyCap_c at 0x0211cdc4) and emits
 * each D1/D0 pair at the bottom of the run, where the cartridge has them.
 *
 * EMISSION ORDER. Functions are written in descending ROM order; mwccarm
 * emits .text in reverse source order, so the object comes out ascending.
 *
 * FILE-WIDE PRAGMAS. `opt_common_subs off` and `opt_strength_reduction off`
 * are both needed, and both must be file-wide. Measured with
 * tools/tubuild.py verify: without them func_ov065_02118838 DIFFs, either one
 * alone still DIFFs, and a push/pop bracket around that one function does not
 * bind. The legacy per-file `opt_propagation off` that daDossy_c::Behavior's
 * old source carried is not needed here.
 *
 * DESLOP LEFTOVERS, each kept because the named form does not link or does
 * not match:
 * - Methods whose mangled names carry Fix12<int> by value (ModelAnim::SetAnim,
 *   dCcAc_c::Init, dCcAcPos_c::Init, dBgCh_Actr::Init, dBgW_KcMbg::SetFile,
 *   cstd::atan2) are still called by their literal mangled names with plain
 *   int arguments. Measured: calling ModelAnim::SetAnim as a member with a
 *   `Fix12<int> speed = { 0x1000 }` argument in func_ov065_021182e4 changes
 *   that function's code (the by-value class argument does not travel as a
 *   bare register constant); see notes/mwccarm-codegen.md 6az.
 * - func_ov065_021180d4, 02118838, 02118c4c, 02118cc4 and 02119210 keep the
 *   char* / unsigned char* parameter that include/decl_common.h already
 *   declares for them, and view it as the class through a local pointer.
 *   Retyping the parameter is a declaration-agreement change for every
 *   includer, so it is left for a pass that owns decl_common.h.
 * - func_ov065_02118838 keeps its `(i << 9)` platform addressing and its
 *   hoisted constant locals: a typed DorriePlatform walk or mPlatforms[i]
 *   indexing DIFFs, as it does in CleanupResources and InitResources, which
 *   step a char* by sizeof(DorriePlatform) for the same reason.
 * - daDossyCap_c::Render tests its cap-icon flag as `(flags << 30) >> 31`;
 *   the `>> 1 & 1` spelling selects asr/ands instead of lsl/lsrs.
 * - Data keeps its address names (data_ov065_*), including the state table.
 * - The two long_calls veneers (021195bc, 021195d0) forward to the collision
 *   callbacks with a function-pointer cast, the shape the cartridge has.
 */

#pragma opt_common_subs off
#pragma opt_strength_reduction off
#include "common.h"
#include "daDossy_c.h"
#include "daDossyCap_c.h"
#include "dBgW.h"
#include "SharedFilePtr.h"
#include "Player.h"

typedef enum { FALSE = 0, TRUE = 1 } BOOL;

extern "C" {
extern s16 data_02082214[];
extern Matrix4x3 data_020a0e68;
extern u8 data_ov065_0211c078[];
extern char data_ov065_0211d748[];
extern void *data_ov065_0211d768[];
extern void *data_ov065_0211d770[];
extern void *data_ov065_0211cd68[];
extern SharedFilePtr data_ov002_0210d9c0;
extern SharedFilePtr data_ov065_0211d720;
extern SharedFilePtr *data_ov065_0211c080[];
extern SharedFilePtr *data_ov065_0211c08c[];

extern void _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(void* self, void* bca, int n, int fix, u32 flags);
extern void func_0201267c(u32 id, char* p);
extern int _ZN4cstd5atan2E5Fix12IiES1_(int a, int b);
void Matrix4x3_FromTranslation(Matrix4x3 *m, s32 x, s32 y, s32 z);
void Matrix4x3_ApplyInPlaceToRotationX(Matrix4x3 *m, s16 a);
void Matrix4x3_ApplyInPlaceToRotationY(Matrix4x3 *m, s16 a);
void Matrix4x3_ApplyInPlaceToRotationZ(Matrix4x3 *m, s16 a);
void MulMat4x3Mat4x3(const Matrix4x3 *a, const Matrix4x3 *b, Matrix4x3 *o);
void SubVec3(const Vector3 *a, const Vector3 *b, Vector3 *o);
void AddVec3(const Vector3 *a, const Vector3 *b, Vector3 *o);
void Vec3_LslInPlace(Vector3 *v, s32 sh);
extern void Vec3_Asr(void* d, void* s, int sh);
extern void Matrix4x3_FromRotationY(void *, int);
extern void _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(void* self, void* kcl, void* mtx, Fix12i r, short s, void* clps);
extern void func_020393d4(void* p, void* v);
extern void func_020393c4(void* p, void* v);
extern void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(void* self, void* a, Fix12i r, Fix12i h, void* p, void* q);
extern void _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(void* self, void* a, Fix12i r, Fix12i h, unsigned int e, unsigned int g);
extern void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(void* self, void* a, void* pos, Fix12i r, Fix12i h, unsigned int e, unsigned int g);
extern int Vec3_HorzDist(void* a, void* b);
extern short Vec3_HorzAngle(void* a, void* b);
extern int AngleDiff(int a, int b);
extern void dBgCh_Actr_UpdateContinuous_Veneer(void* p);
extern u16 DecIfAbove0_Short(void *);

int func_ov065_021180d4(char* self);
int func_ov065_02118248(daDossy_c *dossy);
void func_ov065_021182e4(daDossy_c *dossy);
void func_ov065_021183c8(daDossy_c *dossy);
void func_ov065_02118634(daDossy_c *dossy);
void func_ov065_02118838(char *c);
void func_ov065_02118c4c(char* c);
void func_ov065_02118cc4(char *t);
int func_ov065_02119210(unsigned char* c);
void func_ov065_0211956c(daDossy_c *dossy, dActor_c *other);
void func_ov065_02119594(daDossy_c *dossy, dActor_c *other);
int func_ov065_021195bc(void *a, void *b, void *c);
int func_ov065_021195d0(void *a, void *b, void *c);
}

int ApproachLinear(int &value, int target, int step);
bool ApproachLinear(short &value, short target, short step);

/* The loaded file behind a SharedFilePtr is its second word. */
#define LOADED_FILE(ptr) ((void *)((int *)&(ptr))[1])

typedef void (daDossy_c::*StateFunc)();
extern "C" StateFunc data_ov065_0211d7fc[];

/* ROM ordinal 28 */
// @symbol _ZN14DorriePlatformC1Ev
DorriePlatform::DorriePlatform()
{
}

/* ROM ordinal 27 */
// @symbol daDossy_c_classInit
extern "C" daDossy_c *daDossy_c_classInit(void)
{
    return new daDossy_c();
}

/* ROM ordinal 26 */
// @symbol daDossyCap_c_classInit
extern "C" daDossyCap_c *daDossyCap_c_classInit(void)
{
    return new daDossyCap_c();
}

/* ROM ordinal 25 */
// @symbol _ZN12daDossyCap_c13OnYoshiTryEatEv
int daDossyCap_c::OnYoshiTryEat()
{
    return 4;
}

/* ROM ordinal 24 */
#pragma long_calls on
// @symbol func_ov065_021195d0
extern "C" int func_ov065_021195d0(void *a, void *b, void *c)
{
    return ((int (*)(void *, void *))func_ov065_02119594)(b, c);
}
#pragma long_calls off

/* ROM ordinal 23 */
#pragma long_calls on
// @symbol func_ov065_021195bc
extern "C" int func_ov065_021195bc(void *a, void *b, void *c)
{
    return ((int (*)(void *, void *))func_ov065_0211956c)(b, c);
}
#pragma long_calls off

/* ROM ordinal 22 */
// @symbol func_ov065_02119594
extern "C" void func_ov065_02119594(daDossy_c *dossy, dActor_c *other)
{
    BOOL isPlayer = (other->actorID == 0xbf) ? TRUE : FALSE;
    if (isPlayer) {
        dossy->mClsnState = 1;
        dossy->mClsnPlayer = other;
    }
}

/* ROM ordinal 21 */
// @symbol func_ov065_0211956c
extern "C" void func_ov065_0211956c(daDossy_c *dossy, dActor_c *other)
{
    BOOL isPlayer = (other->actorID == 0xbf) ? TRUE : FALSE;
    if (isPlayer) {
        dossy->mClsnState = 2;
        dossy->mClsnPlayer = other;
    }
}

/* ROM ordinal 20 */
// @symbol _ZN12daDossyCap_c13InitResourcesEv
int daDossyCap_c::InitResources()
{
    mModel.SetFile((BMD_File *)LOADED_FILE(data_ov002_0210d9c0), 1, -1);
    mCapIcon.func_ov001_020ab228((char *)this, 2, 2, 0);
    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(&mdCcAc_c, this, 0x96000, 0x96000, 0x100002, 0x8000);
    mCarrier = 0;
    return 1;
}

/* ROM ordinal 19 */
// @symbol _ZN9daDossy_c13InitResourcesEv
int daDossy_c::InitResources()
{
    int i;
    void* f = Model::LoadFile(data_ov065_0211d720);
    mModelAnim.SetFile((BMD_File *)f, 1, -1);
    Model::LoadFile(data_ov002_0210d9c0);
    for (i = 0; i < 3; i++)
        Animation::LoadFile(*data_ov065_0211c080[i]);
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&mModelAnim, data_ov065_0211d770[1], 0, 0x1000, 0);
    func_ov065_02118cc4((char *)this);
    func_ov065_02118838((char *)this);

    {
        int j;
        char* mtx = (char *)&mPlatforms[0].mClsnNextMat;
        char* mmc = (char *)&mPlatforms[0].mClsn;
        for (j = 0; j < 7; j++) {
            KCL_File *kcl = (KCL_File *)dBgW_Kc::LoadFile(*data_ov065_0211c08c[j]);
            _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
                mmc, kcl, mtx, 0x1000, mAngleY, data_ov065_0211cd68[j]);
            if ((unsigned)j <= 2) {
                func_020393d4(mmc, (void *)&dBgW::UpdatePosAndAngs);
                if (j == 2)
                    func_020393c4(mmc, (void *)func_ov065_021195bc);
                else
                    func_020393c4(mmc, (void *)func_ov065_021195d0);
            }
            ((dBgW_KcMbg *)mmc)->Enable(this);
            mtx += sizeof(DorriePlatform);
            mmc += sizeof(DorriePlatform);
        }
    }

    {
        mSpawnPosX = mPosX;
        mSpawnPosY = mPosY;
        mSpawnPosZ = mPosZ;
        *(int*)(((int)((char*)this) + 0x5c)) += 0x7d0000;
    }
    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
        &mWithMeshClsn, this, 0x1e0000, 0xa0000, 0, 0);
    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(
        &mCylClsn1, this, 0xdc000, 0xfa000, 2, 0x20);
    {
        int v[3];
        v[0] = 0;
        v[1] = 0x50000;
        v[2] = 0x150000;
        _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
            &mCylClsn2, this, v, 0x60000, 0x1b0000, 2, 0x400000);
    }
    {
        mHomePosX = mPosX;
        mHomePosY = mPosY;
        mHomePosZ = mPosZ;
        mStateTimer = 0;
        mClsnState = 0;
        mClsnPlayer = 0;
        mRider = 0;
        mSinkHeight = 0;
        mPushDownHeight = mSinkHeight;
    }
    if ((param1 & 0xff) == 1)
        mHasCap = 1;
    else
        mHasCap = 0;
    mCap = 0;
    if (mHasCap != 0) {
        mCapPosX = 0;
        mCapPosY = 0;
        mCapPosZ = 0;
        func_ov065_02118838((char *)this);
        {
            daDossyCap_c *cap = (daDossyCap_c *)dActor_c::Spawn(
                0xa9, 0, *(Vector3 *)&mPosX, (Vector3_16 *)&mAngleX, mAreaId, -1);
            if (cap != 0) {
                mCap = cap;
                cap->mCarrier = (s32)this;
            } else {
                mHasCap = 0;
            }
        }
    }
    return 1;
}

/* ROM ordinal 18 */
// @symbol func_ov065_02119210
extern "C" int func_ov065_02119210(unsigned char *c)
{
    daDossyCap_c *cap = (daDossyCap_c *)c;
    int flags = cap->mCapIcon.mFlags << 30;
    return ((unsigned)flags >> 31) != 0;
}

/* ROM ordinal 17 */
// @symbol _ZN12daDossyCap_c8BehaviorEv
int daDossyCap_c::Behavior()
{
    daDossy_c *carrier = (daDossy_c *)mCarrier;
    if (carrier == 0)
        return 1;
    int eaten = (mFlags & 0x20000) != 0;
    if (eaten != 0) {
        dActor_c *spit = dActor_c::Spawn(0x10d, 0x1210, *(Vector3 *)&mPosX,
                                         (Vector3_16 *)&mAngleX, mAreaId, -1);
        if (spit != 0) {
            *(s32 *)((char *)spit + 0xd0) = mEatingPlayer;
            ((Player *)mEatingPlayer)->mObjInMouth = (s32)spit;
            spit->mFlags |= 0x20000;
            mFlags &= ~0x20000;
            mEatingPlayer = 0;
            mCapIcon.Unlink();
            mCapIcon.func_ov001_020ab228((char *)this, 2, 0, 0);
        }
        return 1;
    }
    s32 *capPos = &carrier->mCapPosX;
    mPosX = capPos[0];
    mPosY = capPos[1];
    mPosZ = capPos[2];
    mAngleX = ((daDossy_c *)mCarrier)->mHeadRotX;
    mAngleY = ((daDossy_c *)mCarrier)->mAngleY;
    unsigned int flags = mCapIcon.mFlags;
    if ((flags << 30) >> 31) {
        func_ov065_02118c4c((char *)this);
        if (func_ov065_021180d4((char *)this) != 0) {
            mdCcAc_c.Clear();
            return 1;
        }
        mdCcAc_c.Clear();
        mdCcAc_c.Update();
    }
    return 1;
}

/* ROM ordinal 16 */
// @symbol _ZN9daDossy_c8BehaviorEv
int daDossy_c::Behavior()
{
    int d;
    int outer;
    int inner;

    if (mClsnState != 0)
        ApproachLinear(mSinkHeight, 0xa000, 0x1000);
    else
        ApproachLinear(mSinkHeight, 0, 0x1000);

    (this->*data_ov065_0211d7fc[mState])();

    mDistToCenter = Vec3_HorzDist(&mPosX, &mSpawnPosX);
    mAngToCenter = Vec3_HorzAngle(&mPosX, &mSpawnPosX);

    d = (short)AngleDiff(mAngToCenter, mAngleY);

    {
        short sv = data_02082214[((unsigned short)d >> 4) * 2 + 1];
        short cv = data_02082214[((unsigned short)mPlatforms[1].mRot.x >> 4) * 2];
        int sm = sv * 0x190;
        int t = (int)(((long long)sm * cv + 0x800) >> 12);
        if (d < 0x4000) {
            inner = t + 0x5f8000;
            outer = 0x97c000;
        } else {
            inner = 0x5f8000;
            outer = t + 0x97c000;
        }
    }

    if (mDistToCenter >= outer) {
        mPosX = mSpawnPosX
            - (int)(((long long)outer * data_02082214[((unsigned short)mAngToCenter >> 4) * 2] + 0x800) >> 12);
        mPosZ = mSpawnPosZ
            - (int)(((long long)outer * data_02082214[((unsigned short)mAngToCenter >> 4) * 2 + 1] + 0x800) >> 12);
    } else if (mDistToCenter <= inner) {
        mPosX = mSpawnPosX
            - (int)(((long long)inner * data_02082214[((unsigned short)mAngToCenter >> 4) * 2] + 0x800) >> 12);
        mPosZ = mSpawnPosZ
            - (int)(((long long)inner * data_02082214[((unsigned short)mAngToCenter >> 4) * 2 + 1] + 0x800) >> 12);
    }

    UpdatePos(0);
    dBgCh_Actr_UpdateContinuous_Veneer(&mWithMeshClsn);
    mPosY = mSpawnPosY - mSinkHeight - mPushDownHeight;
    mModelAnim.Advance();
    func_ov065_02118cc4((char *)this);
    func_ov065_02118838((char *)this);
    func_ov065_02118248(this);
    mClsnState = 0;
    mClsnPlayer = 0;
    return 1;
}

/* ROM ordinal 15 */
// @symbol _ZN12daDossyCap_c6RenderEv
int daDossyCap_c::Render()
{
    unsigned int flags = mCapIcon.mFlags;
    if ((flags << 30) >> 31) {
        Vector3 scale;
        scale.x = 0x2c00;
        scale.y = 0x2c00;
        scale.z = 0x2c00;
        mModel.Render(&scale);
    }
    return 1;
}

/* ROM ordinal 14 */
// @symbol _ZN9daDossy_c6RenderEv
int daDossy_c::Render()
{
    mModelAnim.Render(0);
    return 1;
}

/* ROM ordinal 13 */
// @symbol _ZN9daDossy_c16CleanupResourcesEv
int daDossy_c::CleanupResources()
{
    int i;
    char *clsn;
    data_ov002_0210d9c0.Release();
    clsn = (char *)&mPlatforms[0].mClsn;
    for (i = 0; i < 7; i++) {
        ((dBgW_KcMbg *)clsn)->Disable();
        data_ov065_0211c08c[i]->Release();
        clsn += sizeof(DorriePlatform);
    }
    for (i = 0; i < 3; i++)
        data_ov065_0211c080[i]->Release();
    data_ov065_0211d720.Release();
    return 1;
}

/* ROM ordinal 12 */
// @symbol func_ov065_02118cc4
extern "C" void func_ov065_02118cc4(char *t)
{
    daDossy_c *dossy = (daDossy_c *)t;
    Matrix4x3_FromRotationY(&dossy->mModelAnim.mat4x3, dossy->mAngleY);
    dossy->mModelAnim.mat4x3.m[9] = dossy->mPosX >> 3;
    dossy->mModelAnim.mat4x3.m[10] = dossy->mPosY >> 3;
    dossy->mModelAnim.mat4x3.m[11] = dossy->mPosZ >> 3;
}

/* ROM ordinal 11 */
// @symbol func_ov065_02118c4c
extern "C" void func_ov065_02118c4c(char *c)
{
    daDossyCap_c *cap = (daDossyCap_c *)c;
    Vector3 pos;
    Vec3_Asr(&pos, &cap->mPosX, 3);
    Matrix4x3_FromTranslation(&data_020a0e68, pos.x, pos.y, pos.z);
    Matrix4x3_ApplyInPlaceToRotationY(&data_020a0e68, cap->mAngleY);
    Matrix4x3_ApplyInPlaceToRotationX(&data_020a0e68, cap->mAngleX);
    cap->mModel.mat4x3 = data_020a0e68;
}

/* ROM ordinal 10 */
// @symbol func_ov065_02118838
extern "C" void func_ov065_02118838(char *c)
{
    daDossy_c *dossy = (daDossy_c *)c;
    char *pm;
    char *pk;
    s32 zero;
    s32 three;
    Vector3s rot;
    Vector3 v;
    Matrix4x3 base;
    s32 i;
    u8 *tbl;

    Matrix4x3_FromTranslation(&data_020a0e68, dossy->mPosX, dossy->mPosY, dossy->mPosZ);
    Matrix4x3_ApplyInPlaceToRotationY(&data_020a0e68, dossy->mAngleY);
    base = data_020a0e68;
    pm = c + 0x150;
    pk = c + 0x180;
    zero = 0;
    three = 3;
    tbl = data_ov065_0211c078;
    for (i = 0; i < 7; i++) {
        char *ent = *(char **)(c + 0xfc) + *tbl * 0x34;
        {
            u16 tz = *(u16 *)(ent + 0x1e);
            u16 ty = *(u16 *)(ent + 0x1c);
            u16 tx = *(u16 *)(ent + 0x1a);
            rot.x = tx;
            rot.y = ty;
            rot.z = tz;
        }
        *(Vector3s *)(c + (i << 9) + 0x348) = rot;
        if (i == 2) {
            *(s16 *)(c + (i << 9) + 0x348) += dossy->mPlatforms[1].mRot.x;
            *(s16 *)(c + (i << 9) + 0x34a) += dossy->mPlatforms[1].mRot.y;
            *(s16 *)(c + (i << 9) + 0x34c) += dossy->mPlatforms[1].mRot.z;
        }
        v.x = zero;
        v.y = zero;
        v.z = zero;
        {
            Matrix4x3 *m = &data_020a0e68;
            *m = base;
            MulMat4x3Mat4x3((Matrix4x3 *)(*(char **)(c + 0x100) + *tbl * 0x30), m, m);
            v.x = m->m[9];
            v.y = m->m[10];
            v.z = m->m[11];
        }
        SubVec3(&v, (Vector3 *)&dossy->mPosX, &v);
        Vec3_LslInPlace(&v, three);
        AddVec3(&v, (Vector3 *)&dossy->mPosX, &v);
        if (i == 2) {
            s32 *cx = &dossy->mCapPosX;
            s32 *cy = &dossy->mCapPosY;
            s32 *cz = &dossy->mCapPosZ;
            s32 *hx = &dossy->mHomePosX;
            s32 *hy = &dossy->mHomePosY;
            s32 *hz = &dossy->mHomePosZ;
            s32 k1e = 0x1e;
            s32 k82 = 0x82;
            s32 k96 = 0x96;
            s32 rnd = 0x800;
            s32 scaled;
            dossy->mHeadRotX = dossy->mPlatforms[2].mRot.x;
            dossy->mHomePosX = v.x;
            dossy->mHomePosY = v.y;
            dossy->mHomePosZ = v.z;
            scaled = data_02082214[((u16)dossy->mHeadRotX >> 4) * 2] * k96;
            dossy->mCapPosX = dossy->mHomePosX;
            dossy->mCapPosY = dossy->mHomePosY;
            dossy->mCapPosZ = dossy->mHomePosZ;
            *cx += (s32)(((s64)scaled * data_02082214[((u16)dossy->mAngleY >> 4) * 2] + rnd) >> 12);
            *cy += 0x8c000 - data_02082214[((u16)dossy->mHeadRotX >> 4) * 2] * k1e;
            *cz += (s32)(((s64)scaled * data_02082214[((u16)dossy->mAngleY >> 4) * 2 + 1] + rnd) >> 12);
            *hx += data_02082214[((u16)dossy->mAngleY >> 4) * 2] * k82;
            *hy += 0x50000;
            *hz += data_02082214[((u16)dossy->mAngleY >> 4) * 2 + 1] * k82;
        }
        Matrix4x3_FromTranslation(&data_020a0e68, v.x, v.y, v.z);
        Matrix4x3_ApplyInPlaceToRotationY(&data_020a0e68, (s16)(dossy->mAngleY + *(s16 *)(c + (i << 9) + 0x34a)));
        Matrix4x3_ApplyInPlaceToRotationX(&data_020a0e68, *(s16 *)(c + (i << 9) + 0x348));
        Matrix4x3_ApplyInPlaceToRotationZ(&data_020a0e68, *(s16 *)(c + (i << 9) + 0x34c));
        *(Matrix4x3 *)(c + (i << 9) + 0x150) = data_020a0e68;
        ((dBgW_KcMbg *)pk)->Transform(*(Matrix4x3 *)pm, dossy->mAngleY);
        pm += 0x200;
        pk += 0x200;
        tbl++;
    }
}

/* ROM ordinal 9 */
// @symbol func_ov065_02118634
extern "C" void func_ov065_02118634(daDossy_c *dossy)
{
    int landed = 0;

    {
        u32 id = dossy->mCylClsn2.otherOwner;
        if (id != 0) {
            Player *player = (Player *)dActor_c::FindWithID(id);
            if (player != 0) {
                if (player->mIsAirborne == 0) landed = 1;
            }
        }
    }

    if (dossy->mCylClsn1.otherOwner != 0 || landed != 0) {
        dossy->mState++;
        dossy->mHorzSpeed = 0;
        dossy->mAngVelY = 0;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&dossy->mModelAnim, data_ov065_0211d768[1], 0x40000000, 0x1000, 0);
        dossy->mCylClsn1.Clear();
        dossy->mCylClsn2.Clear();
        dossy->mStateTimer = 0;
        dossy->mStateState = 0;
        dossy->mPushDownHeight = dossy->mStateState;
        func_0201267c(0xe6, (char *)&dossy->mCamSpacePosX);
    } else {
        s16 target;
        int speed;
        dActor_c *player = dossy->mClsnPlayer;
        if (player != 0) {
            target = player->mAngleY;
            speed = 0x7000;
        } else {
            int a = _ZN4cstd5atan2E5Fix12IiES1_(0x7d0000, dossy->mDistToCenter - 0x7d0000);
            s16 base = dossy->mAngToCenter;
            s16 diff = (s16)(dossy->mAngleY - base);
            if (diff < 0) a = (s16)(-a);
            target = (s16)(base + a);
            speed = 0x3800;
        }
        ApproachLinear(dossy->mHorzSpeed, speed, 0x400);

        {
            int d = (s16)(target - dossy->mAngleY);
            ApproachLinear(dossy->mAngVelY, (s16)(d / 100), 5);
        }

        dossy->mAngleY += dossy->mAngVelY;
        dossy->mPrevAngleY = dossy->mAngleY;

        dossy->mCylClsn1.Clear();
        dossy->mCylClsn2.Clear();
        dossy->mCylClsn1.Update();
        {
            Vector3 offset;
            offset.x = 0;
            offset.y = 0x50000;
            offset.z = 0x150000;
            dossy->mCylClsn2.SetPosRelativeToActor(offset);
        }
        dossy->mCylClsn2.Update();
    }
}

/* ROM ordinal 8 */
// @symbol func_ov065_021183c8
extern "C" void func_ov065_021183c8(daDossy_c *dossy)
{
    Vector3 playerPos;
    u8 st = dossy->mStateState;

    if (st == 0) {
        dossy->mPushDownHeight += 0xa000;
        if (dossy->mPushDownHeight >= 0x64000) {
            dossy->mPushDownHeight = 0x64000;
            dossy->mStateState++;
        }
    } else if (st == 1) {
        dossy->mPushDownHeight -= 0x5000;
        if (dossy->mPushDownHeight <= 0) {
            dossy->mPushDownHeight = 0;
            dossy->mStateState++;
        }
    }

    if (dossy->mModelAnim.Finished() == 0)
        return;

    dossy->mStateTimer++;
    DecIfAbove0_Short(&dossy->mUnkTimer);

    if (dossy->mClsnPlayer != 0) {
        if (dossy->mClsnState == 2 && dossy->mUnkTimer == 0) {
            if (dossy->mCap != 0 && ((int (*)(int))func_ov065_02119210)((int)dossy->mCap) != 0)
                goto tail;
            playerPos = *(Vector3 *)&dossy->mClsnPlayer->mPosX;
            if (playerPos.y < dossy->mHomePosY)
                goto tail;
            if (Vec3_HorzDist(&playerPos, &dossy->mHomePosX) >= 0x88000)
                goto tail;
            if (((Player *)dossy->mClsnPlayer)->SetNoControlState(5, -1, 0) == 0)
                goto tail;
            dossy->mState++;
            dossy->mStateState = 0;
            _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&dossy->mModelAnim,
                (void *)*(int *)(data_ov065_0211d748 + 4), 0x40000000, 0x1000, 0);
            dossy->mRider = dossy->mClsnPlayer;
        }
    tail:
        if (Vec3_HorzDist(&dossy->mPosX, &dossy->mClsnPlayer->mPosX) < 0x1f4000)
            dossy->mStateTimer = 0;
    } else {
        if (dossy->mStateTimer > 0x96) {
            dossy->mState++;
            dossy->mStateState = 0;
            _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&dossy->mModelAnim,
                (void *)*(int *)(data_ov065_0211d748 + 4), 0x40000000, 0x1000, 0);
        }
        dossy->mStateTimer++;
    }
}

/* ROM ordinal 7 */
// @symbol func_ov065_021182e4
extern "C" void func_ov065_021182e4(daDossy_c *dossy)
{
    switch (dossy->mStateState) {
    case 0:
        if (dossy->mModelAnim.Finished() == 0) return;
        dossy->mStateState = 1;
        _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(&dossy->mModelAnim, data_ov065_0211d770[1], 0, 0x1000, 0);
        if (dossy->mRider == 0) return;
        ((Player *)dossy->mRider)->Unk_020ca150(5);
        return;
    case 1:
        if (dossy->mRider != 0) {
            if (((Player *)dossy->mRider)->Unk_020c9e5c(5) != 0) return;
        }
        dossy->mState = 0;
        dossy->mStateState = 0;
        dossy->mRider = 0;
        return;
    }
}

/* ROM ordinal 6 */
// @symbol func_ov065_02118248
extern "C" int func_ov065_02118248(daDossy_c *dossy)
{
    int hidden = (int)((dossy->mFlags & 8) != 0);
    if (hidden != 0) {
        int i;
        char *clsn = (char *)&dossy->mPlatforms[0].mClsn;
        for (i = 0; i < 7; i++) {
            if (((dBgW_KcMbg *)clsn)->IsEnabled())
                ((dBgW_KcMbg *)clsn)->Disable();
            clsn += sizeof(DorriePlatform);
        }
        return 1;
    }
    char *clsn = (char *)&dossy->mPlatforms[0].mClsn;
    int i;
    for (i = 0; i < 7; i++) {
        if (!((dBgW_KcMbg *)clsn)->IsEnabled())
            ((dBgW_KcMbg *)clsn)->Enable(dossy);
        clsn += sizeof(DorriePlatform);
    }
    return 0;
}

/* ROM ordinal 5 */
// @symbol func_ov065_021180d4
extern "C" int func_ov065_021180d4(char *self)
{
    daDossyCap_c *cap = (daDossyCap_c *)self;
    Player *player;
    u32 id;

    id = cap->mdCcAc_c.otherOwner;
    if (id == 0) goto fail;
    if (!(cap->mdCcAc_c.hitFlags & 0x400000)) goto fail;

    player = (Player *)dActor_c::FindWithID(id);
    if (player == 0) goto fail;

    {
        int isPlayer = (int)(player->actorID == 0xbf);
        if (isPlayer == 0) goto fail;
    }
    if (player->mIsAirborne != 0) goto fail;
    if (player->Unk_020c9e5c(5) != 0) goto fail;

    {
        Vector3 pos;
        Vector3 *playerPos;
        int idx;
        int speed;

        playerPos = (Vector3 *)&player->mPosX;
        pos.x = playerPos->x;
        pos.y = playerPos->y;
        pos.z = playerPos->z;

        idx = (u16)player->mAngleY >> 4;
        speed = player->mHorzSpeed;

        pos.y += 0x50000;
        pos.x += (int)(((s64)speed *
                           data_02082214[idx * 2] + 0x800) >> 0xc);
        pos.z += (int)(((s64)speed *
                           data_02082214[idx * 2 + 1] + 0x800) >> 0xc);

        if (dActor_c::Spawn(0x10d, 0x1210, pos, (Vector3_16 *)&cap->mAngleX,
                            cap->mAreaId, -1) != 0)
        {
            cap->mCapIcon.Unlink();
            cap->mCapIcon.func_ov001_020ab228(self, 2, 0, 0);
            ((daDossy_c *)cap->mCarrier)->mUnkTimer = 0x1e;
        }
    }

    return 1;

fail:
    return 0;
}

/* ROM ordinal 4 */
// @symbol _ZN14DorriePlatformD1Ev
DorriePlatform::~DorriePlatform()
{
}
