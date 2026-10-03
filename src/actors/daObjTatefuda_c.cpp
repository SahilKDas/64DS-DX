//cpp
/* daObjTatefuda_c: the wooden signpost (TATEFUDA profile, ov002).
 *
 * The whole unit, ov002 .text 0x020badd0..0x020bc3c8, 28 functions: the
 * destructor pair, ten named methods (Kill, attacks, Behavior, Render,
 * InitResources...), and eighteen helpers. Talking to it runs the
 * 0x3dc-byte talk routine (approach, face the reading spot, show the
 * sign's message); pounding, attacking or mega-killing it breaks it.
 *
 * The tree used to call this class SignPost (coined): the cartridge's
 * _ZTS15daObjTatefuda_c at ov002 0x02109ac0, _ZTI at 0x02109ab4 and _ZTV
 * at 0x02109af8 name it daObjTatefuda_c, and daObjTatefuda_c_classInit
 * builds it for the TATEFUDA registry profile.
 *
 * Kill (slot 31, overriding dBgActor_c) is the key function: its real
 * body emits the vtable, the RTTI and the destructor variants, so D1/D0
 * carry no bodies here. Under `#pragma defer_codegen off` the file is
 * ROM-ascending.
 *
 * Known limits:
 * - The eighteen func_ov002_* helpers stay free functions over raw
 *   offsets. Several are reached through the C/PMF state table
 *   (bbd5c/bbda4/bb9fc share one TU-local view), so method conversion
 *   needs per-helper semantic review after the member-naming deslop.
 *   Follow-up lane.
 * - func_ov002_020bafc0 and func_ov002_020bb060 parse as C: their
 *   Matrix4x3 block copies scalarize under C++ (same wall as Bullet
 *   020fed7c and ov062 ba84). `#pragma cplusplus off/on` around the
 *   definitions only.
 * - func_ov002_020bb060, func_ov002_020bbd5c and func_ov002_020bae9c
 *   stay free: their TU-local C-struct views have no class home.
 * - Vec3_ApproachHorz is fixed tree-wide to its int return (promoted
 *   TUs already declared it so; decl_common.h said void).
 * - daObjTatefuda_c_classInit (0x020bc3c8) abuts this run and stays a
 *   one-function source.
 * - dCcAc_c::Init, dBgCh_Actr::Init, DropShadow, Particle::New and the
 *   Player/Sound helpers stay mangled scalar externs (Fix12-by-value
 *   member form is the 6az wall).
 * - SignPost_ClsnFile / SignPost_ModelFile keep their coined BSS names
 *   (historical, like the Spawn aliases).
 * - data_ov002_0210e084 (C/PMF table), the message/volume tables and
 *   g_profile_TATEFUDA are not this TU's data.
 */

/* Includes: union of the legacy files', first-seen in ROM-ascending
 * processing order. NOT verified for header ordering constraints (e.g. a
 * common.h-before-X rule) -- watch for new compile errors after this. */
#include "daObjTatefuda_c.h"
#include "common.h"
#include "dActor_c.h"
#include "Sound.h"
#include "dBgActor_c.h"
#include "Player.h"
#include "types.h"
#include "decl_common.h"
#include "dBgW.h"
#include "SharedFilePtr.h"
#include "dBgCh_Gnd.h"

/* Local shadow declarations carried from the legacy files verbatim.
 * NOT reconciled against real project headers -- check include/*.h for
 * each of these before compiling; a real header should usually win. */
/* shadow struct 'Vec3' */
struct Vec3 { int x, y, z; };

/* shadow struct 'M43' */
struct M43 { int w[12]; };

/* shadow typedef 'C' */
typedef struct C C;

/* Bare Vec3 is used for Obj.pos and locals; the struct form above feeds
 * the elaborated-type uses. */
typedef struct Vec3 Vec3;

/* shadow struct 'Obj' */
struct Obj {
    char _pad0[0x5c];
    Vec3 pos;                 /* 0x5c */
    char _pad1[0x98 - 0x68];
    u32 unk98;                /* 0x98 */
    char _pad2[0xa8 - 0x9c];
    u32 unkA8;                /* 0xa8 */
    char _pad3[0x59c - 0xac];
    struct Obj* unk59C;       /* 0x59c */
    struct Obj* unk5A0;       /* 0x5a0 */
};

/* shadow typedef 's32' */
typedef int s32;

/* shadow struct 'Sub041' */
struct Sub041 {
  virtual void v0(); virtual void v1(); virtual void v2();
  virtual void v3(); virtual void v4(); virtual void v5(int);
};

/* shadow enum 'Bool' */
enum Bool { FALSE, TRUE };

/* shadow struct 'Vector3_16f' */
struct Vector3_16f;

/* shadow struct 'BMD_File' */
struct BMD_File; struct KCL_File; struct dActor_c; struct Vector3; struct Matrix4x3;

/* shadow struct 'CLPS_Block' */
struct CLPS_Block; struct SharedFilePtr; struct Vector3_16;

/* shadow struct 'V3' */
struct V3 { int x, y, z; };

#define FIXMUL(a, b) ((s32)(((s64)(a) * (b) + 0x800) >> 12))
#define LD(p) ((int)(p))

extern "C" {
extern void _ZN10dBgActor_c21UpdateModelPosAndRotYEv(void *c);
extern void _ZN10dBgActor_c19UpdateClsnPosAndRotEv(void *c);
extern void func_ov002_020baf80(char *c);
extern void func_ov002_020bbd5c(C *c, int i);
extern void _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(void *thiz, void *actor, int b, int d, unsigned int e, unsigned int f);
extern void _ZN5dCc_c5ClearEv(void *c);
extern void Matrix4x3_FromRotationY(void *, int);
extern void Vec3_Asr(struct Vector3* d, struct Vector3* s, int sh);
extern void Matrix4x3_FromTranslation(struct Matrix4x3* m, Fix12i x, Fix12i y, Fix12i z);
extern void Matrix4x3_ApplyInPlaceToTranslation(struct Matrix4x3* m, Fix12i x, Fix12i y, Fix12i z);
extern void Matrix4x3_ApplyInPlaceToRotationZXYExt(struct Matrix4x3* m, int x, int y, int z);
extern struct Matrix4x3 data_020a0e68;
extern char *func_ov002_020e496c(void *p);
extern int _ZN6Player14IsFrontSlidingEv(void *p);
extern int _ZN6Player17LostGrabbedObjectEv(void *p);
extern int _Z14ApproachLinearRsss(short *value, short target, short step);
extern void MulMat4x3Mat4x3(void *a, void *b, void *c);
extern void Matrix4x3_ApplyInPlaceToRotationXYZExt(void *m, int x, int y, int z);
extern void Vec3_Lsl(struct Vec3 *d, struct Vec3 *s, int sh);
extern short data_ov002_020ff0d0[];
extern int data_ov002_020ff0d4[];
extern int data_ov002_020ff0d8[];
extern int data_ov002_020ff0dc[];
extern "C" void func_02012694(int a, void *b);
extern "C" void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_( u32 id, Fix12i x, Fix12i y, Fix12i z);

extern void* _ZN8dActor_c10FindWithIDEj(unsigned int id);
extern s16 Vec3_HorzAngle(const struct Vector3* v0, const struct Vector3* v1);
extern int _ZN6Player7TryGrabER8dActor_c(char* p, char* a);
extern int _ZN6Player9StartTalkER7fBase_cb(char* p, char* a, int b);
extern s16 data_02082214[];
extern u8 data_0209d660;
extern u8 data_0209d6bc;
extern u8 data_0209f284;
extern int _ZN6Player12GetTalkStateEv(void *player);
extern s32 Vec3_HorzDist(struct Vector3 *a, struct Vector3 *b);
extern int func_ov002_020bec84(void *player, unsigned int i);
extern int func_ov002_020bec9c(void *player, unsigned int a, int b, int d, unsigned short e);
extern int _ZN6Player12FinishedAnimEv(void *player);
extern void _ZN6Player12ShowMessage2ER7fBase_cjPK7Vector3hh( void *player, void *actor, unsigned int msg, struct Vector3 *pos, unsigned int a, unsigned int b);
extern void func_02012790(int id);
extern int func_ov002_020bb520(char* c);
extern void func_ov002_020bb42c(char* c);
extern void _ZN8dActor_c9UpdatePosEP5dCc_c(void* self, void* c);
extern void dBgCh_Actr_UpdateContinuous_Veneer(void* p);
extern int _ZNK10dBgCh_Actr10IsOnGroundEv(void* p);
extern int _ZNK10dBgCh_Actr8IsOnWallEv(void* p);
extern int _ZNK10dBgCh_Actr12TouchesWaterEv(void* p);
extern void _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(void* thiz, void* v, unsigned a, int b, unsigned c, unsigned d, unsigned e);
extern void _Z14ApproachLinearRiii(int* p, int a, int b);
extern int _ZN8dActor_c13DistToCPlayerEv(void* self);
extern "C" void func_ov002_020bbb14(char* self);
extern int SignPost_ClsnFile[];
extern int SignPost_ModelFile[];
extern "C" unsigned int data_0209b454;
extern "C" void _ZN6Player9DropActorEv(void *self);
extern "C" void func_ov002_020bb060(char *c);
extern "C" u8 DecIfAbove0_Byte(u8 *p);
extern "C" int _ZN10dBgActor_c20UpdateKillByMegaCharEsss5Fix12IiE(void *self, short a, short b, short c, int fix);
extern "C" void *_ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(u32 a, u32 b, int c, int d, int e, const void *v, void *cb);
extern "C" u32 _ZN8Particle6System17NewUnkCallback818Ejj5Fix12IiES2_S2_PK11Vector3_16f(u32 a, u32 b, int c, int d, int e, const Vector3_16f *v);
extern "C" void _ZN8dActor_c19DisappearPoofDustAtERK7Vector3(void *self, const struct Vector3 *vec);
extern "C" void _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(void *self, void *sm, void *m, int a, int b, int c, u32 j);
extern "C" void _ZN10dBgActor_c13IsClsnInRangeE5Fix12IiES1_(void *self, int a, int b);
extern "C" void func_ov002_020bbda4(C *c);
extern "C" void _ZN5dCc_c6UpdateEv(void *self);
extern "C" void func_ov002_020bafc0(char *self);
extern "C" void *_ZN5Model8LoadFileER13SharedFilePtr(void *);
extern "C" void *_ZN7dBgW_Kc8LoadFileER13SharedFilePtr(void *);
extern "C" void _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block( void *self, KCL_File *f, const Matrix4x3 &m, s32 fix, s16 sh, CLPS_Block &b);
extern "C" void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_( void *self, dActor_c *a, s32 radius, s32 height, Vector3_16 *v, Vector3_16 *v2);
extern "C" CLPS_Block data_ov002_0210d714;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 27 -- _ZN15daObjTatefuda_c13InitResourcesEv, 0x020bc240, size 0x188 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_c13InitResourcesEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header */
/* recovered: named members + shared header, real C++ method */
/* Model::LoadFile and dBgW_Kc::LoadFile by their real ROM symbols, carried
   forward from #1554. decl_common.h's ModelLoadFile / MeshColliderLoadFile are
   phantoms -- names no module defines -- and match.py compares relocated words as
   wildcards, so the byte gate never saw it. */
/* THREE OF THE SHADOWS ARE GONE, because daObjTatefuda_c.h now says `daObjTatefuda_c :
   dBgActor_c` and dBgActor_c.h brings in the real dBgActor_c, Model/ModelBase and
   dBgW_KcMbg. Each one is replaced by the thing it was standing in for:

     ModelBase          -> mModel.SetFile, whose real declaration in
                           include/ModelBase.h mangles identically
                           (_ZN9ModelBase7SetFileEP8BMD_Fileii) -- only the
                           return type differed, and that is not mangled.
     dBgActor_c           -> the two calls are unqualified members now.
     dBgW_KcMbg -> its real SetFile takes Fix12<int> BY VALUE, so it
                           cannot be declared as a callable method here without
                           changing how the caller passes the argument
                           (notes/mwccarm-codegen.md 6az). It keeps a scalar
                           extern "C" declaration under its exact ROM symbol,
                           which also FIXES A PHANTOM: the shadow spelled `int`
                           and so emitted a `bl` to
                           ..._Matrix4x3isR10CLPS_Block, which exists nowhere.
                           match.py compares relocated words as wildcards, so
                           nothing caught it.

   The remaining ABI-only calls are dCcAc_c::Init and dBgCh_Actr::Init; they
   have the same Fix12<int> problem with no collision forcing the issue yet. */
/* dBgCh_Actr is the real class now, through this actor's header, and it
   declares StartDetectingWater itself. */
int daObjTatefuda_c::InitResources()
{
    void *mf = _ZN5Model8LoadFileER13SharedFilePtr(&data_ov002_0210e064);
    mModel.SetFile((BMD_File*)mf, 1, -1);
    mShadowModel.InitCuboid();

    int py = mPosY;
    int pz = mPosZ;
    int px = mPosX;
    int py2 = py + 0x64000;
    V3 v = { px, py2, pz };
    dBgCh_Gnd rg;
    rg.SetObjAndPos(*(Vector3*)&v, (dActor_c*)0);
    if (rg.DetectClsn() != 0)
        mPosY = rg.clsnY;

    UpdateModelPosAndRotY();
    UpdateClsnPosAndRot();
    func_ov002_020baf80(((char *)this));

    void *kf = _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(&data_ov002_0210e05c);
    _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
        &mMeshCollider, (KCL_File*)kf, mClsnMat, 0x199, mAngleY, data_ov002_0210d714);

    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(
        &mdCcAc_c, (dActor_c*)((char *)this), 0x64000, 0x64000, 0x4800002, 0x41000);

    mPoundsLeft = 2;
    mHomePosX = mPosX;
    mHomePosY = mPosY;
    mHomePosZ = mPosZ;
    mHomeAngleX = mAngleX;
    mHomeAngleY = mAngleY;
    mHomeAngleZ = mAngleZ;
    mVertAccel = -0x2000;
    mTerminalVelocity = -0x3c000;

    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
        &mWithMeshClsn, (dActor_c*)((char *)this), 0x28000, 0x28000, 0, 0);
    mWithMeshClsn.StartDetectingWater();

    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 26 -- _ZN15daObjTatefuda_c8BehaviorEv, 0x020bbea4, size 0x39c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_c8BehaviorEv
/* daObjTatefuda_c::Behavior -- vtable slot 6. Real C++ method over the shared header.
 *
 * This was an extern "C" free function over a raw `char *c`, with every field
 * reached by literal offset and a local one-word Vector3. Naming the fields is
 * what proved four of them exist at all: 0x354 (mClsnResult), 0x380
 * (mShadowMat) and 0x584/0x588/0x58c (the two particle handles and the break
 * countdown) were all inside explicit `pad_` runs in include/daObjTatefuda_c.h until
 * this body was read. Byte-exact under 2004/b56 after the conversion.
 *
 * What it does, in order: drop the sign if the player carrying it is holding it
 * through a pause; widen the clip radius while it is being carried; run the
 * respawn timer for a sign that is hidden or fully pounded in, and put it back
 * at its home height once the player is far enough away; hand off to
 * dBgActor_c::UpdateKillByMegaChar; run the break countdown, trailing two
 * particles until it expires and poofing on the frame it does; otherwise tick
 * the pound cooldown, refresh collision, and drop the shadow.
 *
 * LD() is a no-op macro the legacy file used to MARK its read-modify-write
 * sites on mFlags. It is kept, with its name, so the marking survives -- it
 * emits nothing, and it is not the reason those sites take an address. */
int daObjTatefuda_c::Behavior()
{
    struct Vector3 v;
    struct Vector3 vec, vec2;

    {
        enum Bool b = (enum Bool)((mFlags & 0x4000000) != 0);
        if (b != FALSE && (data_0209b454 & 0x4000000) && mHoldingPlayer != 0)
            _ZN6Player9DropActorEv(mHoldingPlayer);
    }

    {
        void *p = mHoldingPlayer;
        if (p != 0 && (enum Bool)((mFlags & 0x4000) != 0) != FALSE
            && *(int *)((char *)p + 0xc8) != 0) {
            u32 *fp;
            func_ov002_020bb060((char *)this);
            mClipRadius = 0x20000;
            fp = (u32 *)LD(&mFlags);
            *fp = *fp | 0x4000000;
        } else {
            u32 *fp;
            mClipRadius = 0x10000;
            fp = (u32 *)LD(&mFlags);
            *fp = *fp & ~0x4000000;
        }
    }

    if (mHidden != 0) {
        enum Bool b = (enum Bool)((mFlags & 8) != 0);
        if (b != FALSE && DecIfAbove0_Byte(&mRespawnDelay) == 0
            && _ZN8dActor_c13DistToCPlayerEv(this) > 0x7d0000)
            mHidden = 0;
        return 1;
    }

    if (mPoundsLeft == 0) {
        enum Bool b = (enum Bool)((mFlags & 8) != 0);
        if (b != FALSE && DecIfAbove0_Byte(&mRespawnDelay) == 0
            && _ZN8dActor_c13DistToCPlayerEv(this) > 0x7d0000) {
            mPoundsLeft = 2;
            mPosY = mHomePosY;
            _ZN10dBgActor_c21UpdateModelPosAndRotYEv(this);
            _ZN10dBgActor_c19UpdateClsnPosAndRotEv(this);
            func_ov002_020baf80((char *)this);
        }
    }

    if (_ZN10dBgActor_c20UpdateKillByMegaCharEsss5Fix12IiE(this, -0x2000, 0, 0, 0x46000))
        return 1;

    if (mBreakTimer != 0) {
        int x, y, z;
        if (_ZN4dBgW9IsEnabledEv(&mMeshCollider))
            _ZN4dBgW7DisableEv(&mMeshCollider);
        x = mPosX;
        z = mPosZ;
        y = mPosY + 0x50000;
        ((int *)&v)[0] = x;
        ((int *)&v)[1] = y;
        ((int *)&v)[2] = z;
        if (DecIfAbove0_Byte(&mBreakTimer) != 0) {
            *(void **)&mParticleHandle1 = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
                mParticleHandle1, 0x13a, v.x, v.y, v.z, 0, 0);
            mParticleHandle2 = _ZN8Particle6System17NewUnkCallback818Ejj5Fix12IiES2_S2_PK11Vector3_16f(
                mParticleHandle2, 0x13b, v.x, v.y, v.z, 0);
        } else {
            int x2, y2, z2;
            x2 = mPosX;
            z2 = mPosZ;
            y2 = mPosY + 0x28000;
            ((int *)&vec)[0] = x2;
            ((int *)&vec)[1] = y2;
            ((int *)&vec)[2] = z2;
            vec2 = vec;
            _ZN8dActor_c19DisappearPoofDustAtERK7Vector3(this, &vec2);
            func_ov002_020bae9c((char *)this);
            return 1;
        }
        _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(
            this, &mShadowModel, &mShadowMat, 0x50000, 0x28000, 0x28000, 0xf);
        return 1;
    }

    DecIfAbove0_Byte(&mPoundCooldown);
    if (mHidden == 0)
        _ZN10dBgActor_c13IsClsnInRangeE5Fix12IiES1_(this, 0, 0);
    func_ov002_020bbda4((C *)this);
    _ZN5dCc_c5ClearEv(&mdCcAc_c);
    _ZN5dCc_c6UpdateEv(&mdCcAc_c);
    {
        int s = mClsnResult;
        if (s == 3) {
            func_ov002_020bafc0((char *)this);
        } else if (mPoundsLeft == 2 && (u32)s <= 1) {
            _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(
                this, &mShadowModel, &mShadowMat, 0x50000, 0x28000, 0x28000, 0xf);
        }
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 25 -- _ZN15daObjTatefuda_c6RenderEv, 0x020bbe30, size 0x74 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_c6RenderEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header */
/* recovered: named members + shared header, real C++ method */
int daObjTatefuda_c::Render()
{
  if (mHidden != 0) return 1;
  void* r = mHoldingPlayer;
  if (r != 0) {
    int b = (mFlags & 0x4000) != 0;
    if (b && *(int*)((char*)r+0xc8) != 0) {
      func_ov002_020bb060(((char*)this));
    }
  }
  Sub041* s = (Sub041*)&mModel;
  s->v5(0);
  return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 24 -- _ZN15daObjTatefuda_c16CleanupResourcesEv, 0x020bbdec, size 0x44 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_c16CleanupResourcesEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header */
/* recovered: named members + shared header, real C++ method */
int daObjTatefuda_c::CleanupResources()
{
    if (((dBgW *)&mMeshCollider)->IsEnabled()) {
        ((dBgW *)&mMeshCollider)->Disable();
    }
    ((SharedFilePtr *)(SignPost_ModelFile))->Release();
    ((SharedFilePtr *)(SignPost_ClsnFile))->Release();
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 23 -- func_ov002_020bbda4, 0x020bbda4, size 0x48 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bbda4
struct C; typedef void (C::*PMF)();
struct Entry { PMF pmf[2]; };
extern Entry data_ov002_0210e084[];
struct C { char pad[0x354]; int idx; };
extern "C" void func_ov002_020bbda4(C *c) { int j = c->idx; (c->*data_ov002_0210e084[j].pmf[1])(); }

/* -------------------------------------------------------------------------- */
/* ROM ordinal 22 -- func_ov002_020bbd5c, 0x020bbd5c, size 0x48 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bbd5c
extern "C" void func_ov002_020bbd5c(C *c, int i) { c->idx = i; int j = c->idx; (c->*data_ov002_0210e084[j].pmf[0])(); }

/* -------------------------------------------------------------------------- */
/* ROM ordinal 21 -- func_ov002_020bbd50, 0x020bbd50, size 0xc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bbd50
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020bbd50(int *p)
{
    p[38] = 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 20 -- func_ov002_020bbcb8, 0x020bbcb8, size 0x98 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bbcb8
// func_ov002_020bbcb8 at 0x020bbcb8
// Matched byte-for-byte with mwccarm 1.2/sp2p3 (ov002).
extern "C" void func_ov002_020bbcb8(char* c)
{
    int flags = *(int*)(c + 0xb0);
    bool t;

    t = flags & 0x400;
    if (t != false) {
        func_ov002_020bbd5c((C *)c, 3);
    } else {
        t = flags & 0x2000;
        if (t != false) {
            func_ov002_020bbd5c((C *)c, 4);
        } else {
            t = flags & 0x100;
            if (t == false) {
                func_ov002_020bbd5c((C *)c, 4);
            }
        }
    }

    if (((dBgW *)(c + 0x124))->IsEnabled()) {
        ((dBgW *)(c + 0x124))->Disable();
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 19 -- func_ov002_020bbc78, 0x020bbc78, size 0x40 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bbc78
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020bbc78(char *self)
{
    *(s32 *)(self + 0x98) = 0x50000;
    *(s32 *)(self + 0xa8) = 0xa000;
    *(s32 *)(self + 0x5a0) = *(s32 *)(self + 0x59c);
    *(s32 *)(self + 0x59c) = 0;
    {
        s32 *p = (s32 *)(((int)self + 0x338));
        *p |= 0x2000;
        *p &= ~0x4000000;
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 18 -- func_ov002_020bbb14, 0x020bbb14, size 0x164 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bbb14
/* recovered: shared common types */
void func_ov002_020bbb14(char* self)
{
    int b;
    struct Vector3 vec;
    void* found;
    unsigned id;

    {
        s16* pa = (s16*)(self + 0x8c);
        *pa = *pa + 0x2000;
    }
    _ZN8dActor_c9UpdatePosEP5dCc_c(self, 0);
    dBgCh_Actr_UpdateContinuous_Veneer(self + 0x3c8);

    if (_ZNK10dBgCh_Actr10IsOnGroundEv(self + 0x3c8) != 0 ||
        _ZNK10dBgCh_Actr8IsOnWallEv(self + 0x3c8) != 0 ||
        _ZNK10dBgCh_Actr12TouchesWaterEv(self + 0x3c8) != 0) {
        ((daObjTatefuda_c *)self)->Kill();
        return;
    }

    id = *(unsigned*)(self + 0x344);
    if (id != 0) {
        found = _ZN8dActor_c10FindWithIDEj(id);
        if (found != 0) {
            if (found != *(void**)(self + 0x5a0)) {
                b = *(u16*)((char*)found + 0xc);
                b = b == 0xbf;
                if (b) {
                    vec.x = *(int*)(self + 0x5c);
                    vec.y = *(int*)(self + 0x60);
                    vec.z = *(int*)(self + 0x64);
                    _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(found, &vec, 1, 0xc000, 1, 0, 1);
                }
            }
        }
    }

    _Z14ApproachLinearRiii((int*)(self + 0x98), 0, 0x555);

    if (((dBgW *)(self + 0x124))->IsEnabled() != 0) {
        ((dBgW *)(self + 0x124))->Disable();
    }

    b = *(int*)(self + 0xb0) & 8;
    b = b != 0;
    if (b) {
        if (_ZN8dActor_c13DistToCPlayerEv(self) > 0x7d0000) {
            func_ov002_020bae9c(self);
        }
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 17 -- func_ov002_020bbac8, 0x020bbac8, size 0x4c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bbac8
/* func_ov002_020bbac8 — teleport self to linked actor's position (+100fx up),
 * clear two fields, move the link pointer from 0x59c to 0x5a0.
 * No callees.
 */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020bbac8(struct Obj* self)
{
    struct Obj* other;
    int* py;
    Vec3* src;
    self->unk98 = 0;
    self->unkA8 = 0;
    other = self->unk59C;
    py = (int*)((char*)self + 0x60);
    src = (Vec3*)((char*)other + 0x5c);
    self->pos.x = src->x;
    self->pos.y = src->y;
    self->pos.z = src->z;
    *py += 0x64000;
    self->unk5A0 = self->unk59C;
    self->unk59C = 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 16 -- func_ov002_020bba28, 0x020bba28, size 0xa0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bba28
bool ApproachLinear(short &value, short target, short step);
extern "C" void _ZN8dActor_c9UpdatePosEP5dCc_c(void* self, void* c);
extern "C" void dBgCh_Actr_UpdateContinuous_Veneer(void* p);
extern "C" int _ZNK10dBgCh_Actr10IsOnGroundEv(void* p);
extern "C" int _ZNK10dBgCh_Actr8IsOnWallEv(void* p);
extern "C" int _ZNK10dBgCh_Actr12TouchesWaterEv(void* p);

extern "C" void func_ov002_020bba28(char* self){
    ApproachLinear(*(short *)(self + 0x8c), 0x4000, 0x1000);
    _ZN8dActor_c9UpdatePosEP5dCc_c(self, 0);
    dBgCh_Actr_UpdateContinuous_Veneer(self + 0x3c8);
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(self + 0x3c8)
        || _ZNK10dBgCh_Actr8IsOnWallEv(self + 0x3c8)
        || _ZNK10dBgCh_Actr12TouchesWaterEv(self + 0x3c8)) {
        ((daObjTatefuda_c *)self)->Kill();
    } else {
        func_ov002_020bafc0(self);
        if (((dBgW *)(self + 0x124))->IsEnabled())
            ((dBgW *)(self + 0x124))->Disable();
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 15 -- func_ov002_020bba24, 0x020bba24, size 0x4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bba24
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020bba24(void)
{
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 14 -- func_ov002_020bb9fc, 0x020bb9fc, size 0x28 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bb9fc
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020bb9fc(C* c){
  if(func_ov002_020bb520((char *)c)!=0) return;
  func_ov002_020bb42c((char *)c);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 13 -- func_ov002_020bb9f0, 0x020bb9f0, size 0xc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bb9f0
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020bb9f0(char *p)
{
    p[1421] = 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 12 -- func_ov002_020bb614, 0x020bb614, size 0x3dc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bb614
/* recovered: shared common types */
/* daObjTatefuda_c's talk routine, ov002 0x020bb614, 0x3dc bytes. Called on the
 * signpost (`c` is a daObjTatefuda_c *, the class that owns 0x020bb23c..0x020bc240;
 * see include/daObjTatefuda_c.h) once a player has started talking to it.
 *
 * It walks the player through a three step approach and then shows the sign's
 * message: step 0 turns the player toward the reading spot (or skips straight
 * to step 1 if the player already stands there), step 1 walks the player onto
 * it, step 2 turns the player around to face the sign and then either plays the
 * message straight away or, for a sign that still has pounds left in it, waits
 * out the pound animations first. A talk state other than 0 or 1 aborts the
 * whole thing through func_ov002_020bbd5c.
 *
 * The reading spot is the sign's own position pushed 0x5a000 (or 0x78000 while
 * the sign still has pounds left) forward along the sign's facing angle, using
 * the shared sine table at data_02082214. The message is anchored 0x50000 above
 * the sign itself.
 *
 * The tail is the hint-sign special case: while data_0209d660 is set and this
 * sign carries message 0x74a, data_0209d6bc selects whether the hint flag
 * data_0209f284 is raised (3) or cleared (9), and the sign plays sound 0x24 on
 * the frame the flag comes up. 0x594 remembers the flag's last value so the
 * sound fires once per transition.
 *
 * Vec3_ApproachHorz is declared here rather than taken from decl_common.h
 * because that header types it void, and this body compares its result against
 * zero -- the ROM does `bl` then `cmp r0, #0`, so the function returns a
 * value. decl_common.h is generated, so it is left alone. */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020bb614(char *c)
{
    /* C89: all locals at top. */
    struct Vector3 msgPos;
    struct Vector3 tgt;
    struct Vector3 plPos;
    char *player;
    u16 msgId;
    u8 *p58d;
    s32 scale;
    s32 talk;
    u8 st;
    s32 tx, ty, tz;
    s32 my, mz, mx;
    s32 ang;
    s16 sinV, cosV;
    s32 param;

    msgId = 0;
    param = *(s32 *)(c + 8);
    player = *(char **)(c + 0x598);
    if (param != 0xffff) {
        msgId = (u16)param;
    }
    /* The raised message anchor. The horizontal pair is read first and the
       raised height last: that order is what puts the 0x60 load in the gap the
       ROM leaves after `lslne`, which in turn hands the height r1 and the depth
       r2 the way the ROM colours them. Folding the +0x50000 into the temp
       rather than into the store is the other half of it. */
    mx = *(s32 *)(c + 0x5c);
    mz = *(s32 *)(c + 0x64);
    my = *(s32 *)(c + 0x60) + 0x50000;
    msgPos.x = mx;
    msgPos.y = my;
    msgPos.z = mz;

    scale = 0x5a000;
    if (*(u8 *)(c + 0x58e) == 1) {
        scale = 0x78000;
    }
    tx = *(s32 *)(c + 0x5c);
    tgt.x = tx;
    ty = *(s32 *)(c + 0x60);
    tgt.y = ty;
    tz = *(s32 *)(c + 0x64);
    tgt.z = tz;


    ang = (s32) * (u16 *)(c + 0x8e);
    sinV = data_02082214[(ang >> 4) * 2];
    tx = tx + FIXMUL(scale, sinV);
    tgt.x = tx;

    ang = (s32) * (u16 *)(c + 0x8e);
    cosV = data_02082214[(ang >> 4) * 2 + 1];
    tz = tz + FIXMUL(scale, cosV);
    tgt.z = tz;

    {
        char *pPos = player + 0x5c;
        plPos.x = *(s32 *)(pPos);
        plPos.y = *(s32 *)(pPos + 4);
        plPos.z = *(s32 *)(pPos + 8);
    }

    talk = _ZN6Player12GetTalkStateEv(player);
    switch (talk) {
    case 0:
        st = *(u8 *)(c + 0x58d);
        switch (st) {
        case 0:
            if (Vec3_HorzDist(&plPos, &tgt) < 0x32000) {
                p58d = (u8 *)(c + 0x58d);
                *p58d = (u8)(*p58d + 1);
            } else if (_Z14ApproachLinearRsss(
                           (s16 *)(player + 0x8e),
                           Vec3_HorzAngle(&plPos, &tgt),
                           0x800)
                       != 0) {
                p58d = (u8 *)(c + 0x58d);
                *p58d = (u8)(*p58d + 1);
                func_ov002_020bec9c(player, 1, 0, 0x1000, 0);
            }
            break;
        case 1:
            if (Vec3_ApproachHorz((struct Vector3 *)(player + 0x5c), &tgt, 0xa000) != 0) {
                p58d = (u8 *)(c + 0x58d);
                *p58d = (u8)(*p58d + 1);
            }
            break;
        case 2:
            if (_Z14ApproachLinearRsss(
                    (s16 *)(player + 0x8e),
                    (s16)(*(s16 *)(c + 0x8e) + 0x8000),
                    0x800)
                != 0) {
                if (*(u8 *)(c + 0x58e) == 1) {
                    if (func_ov002_020bec84(player, 1) != 0
                        || func_ov002_020bec84(player, 0) != 0) {
                        func_ov002_020bec9c(player, 2, 0x40000000, 0x1000, 0);
                    } else if (func_ov002_020bec84(player, 2) != 0
                               && _ZN6Player12FinishedAnimEv(player) != 0) {
                        func_ov002_020bec9c(player, 3, 0x40000000, 0x1000, 0);
                    } else if (func_ov002_020bec84(player, 3) != 0
                               && _ZN6Player12FinishedAnimEv(player) != 0) {
                        _ZN6Player12ShowMessage2ER7fBase_cjPK7Vector3hh(
                            player, c, (s16)msgId, &msgPos, 0, 1);
                    }
                } else {
                    func_ov002_020bec9c(player, 0, 0, 0x1000, 0);
                    _ZN6Player12ShowMessage2ER7fBase_cjPK7Vector3hh(
                        player, c, (s16)msgId, &msgPos, 0, 1);
                }
            }
            break;
        }
        break;
    case 1:
        break;
    default:
        func_ov002_020bbd5c((C *)c, 0);
        break;
    }

    if (data_0209d660 != 0 && msgId == 0x74a) {
        switch (data_0209d6bc) {
        case 3:
            data_0209f284 = 1;
            break;
        case 9:
            data_0209f284 = 0;
            break;
        }
    }

    if (*(u8 *)(c + 0x594) != data_0209f284 && data_0209f284 != 0) {
        func_02012790(0x24);
    }
    *(u8 *)(c + 0x594) = data_0209f284;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 11 -- func_ov002_020bb520, 0x020bb520, size 0xf4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bb520
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov002_020bb520(char* self){
  unsigned int id = *(unsigned int*)(self+0x344);
  if (id == 0) return 0;
  if ((*(int*)(self+0x340) & 0x8000000) == 0) return 0;
  if (*(u8*)(self+0x58e) == 0) return 0;
  {
    char* other = (char*)_ZN8dActor_c10FindWithIDEj(id);
    if (other == 0) goto fail;
    {
      int b = (int)(*(u16*)(other+0xc) == 0xbf);
      if (b != false) goto success;
    }
  fail:
    return 0;
  success:
    {
      int ang = Vec3_HorzAngle((struct Vector3*)(self+0x5c), (struct Vector3*)(other+0x5c));
      if (AngleDiff(ang, *(s16*)(self+0x8e)) > 0x4000) return 0;
      *(int*)(self+0x598) = (int)other;
      if (_ZN6Player9StartTalkER7fBase_cb(other, self, 0) == 0) return 0;
      func_ov002_020bbd5c((C *)self, 1);
      return 1;
    }
  }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 10 -- func_ov002_020bb42c, 0x020bb42c, size 0xf4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bb42c
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020bb42c(char* self){
  char* other;
  unsigned int id = *(unsigned int*)(self+0x344);
  if (id == 0) return;
  other = (char*)_ZN8dActor_c10FindWithIDEj(id);
  if (other == 0) return;
  {
    int b = (int)(*(u16*)(other+0xc) == 0xbf);
    if (b == 0) return;
  }
  if ((*(int*)(self+0x340) & 0x40000) != 0) {
    *(u8*)(self+0x58c) = 0x3c;
    return;
  }
  {
    int ang = Vec3_HorzAngle((struct Vector3*)(self+0x5c), (struct Vector3*)(other+0x5c));
    if (*(int*)(other+8) != 2) return;
    if (AngleDiff(ang, *(s16*)(self+0x8e)) <= 0x4000) return;
  }
  if ((*(int*)(self+0x340) & 0x1000) == 0) return;
  if (_ZN6Player7TryGrabER8dActor_c(other, self) == 0) return;
  *(int*)(self+0x59c) = (int)other;
  func_ov002_020bbd5c((C *)self, 2);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 9 -- _ZN15daObjTatefuda_c4KillEv, 0x020bb3b8, size 0x74 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_c4KillEv
/* daObjTatefuda_c::Kill() at ov002 0x020bb3b8, 0x74 bytes -- vtable slot 31.
 *
 * ATTRIBUTED BY THE VTABLE. _ZTV15daObjTatefuda_c (ov002 0x02109af8, and the same
 * address as _ZTV15daObjTatefuda_c) carries 0x020bb3b8 at vtable + 0x7c, which
 * is slot 31, while _ZTV10dBgActor_c carries _ZN10dBgActor_c4KillEv at the same slot
 * and both tables carry dActor_c's 0x020100dc at slot 30. So this is this class's
 * own override of the one virtual dBgActor_c adds. Read out of
 * config/arm9/overlays/ov002/relocs.txt.
 *
 * The signpost does not destroy itself. It plays its particle 0x28000 -- forty
 * 20.12 units -- above where it stands, poofs, plays the break sound and then
 * tails into func_ov002_020bae9c, this class's own still-unnamed reset routine.
 * That is why there is no MarkForDestruction here, unlike dBgActor_c::Kill.
 *
 * The trailing call's return value is dropped: the ROM does `bl`, then the
 * epilogue and `bx lr` with nothing written to r0 in between, which is what a
 * void method calling an int function compiles to.
 *
 * The second Vector3 is memberwise on purpose: Vector3 declares a destructor
 * (types.h), so a whole-object assignment compiles to an ldm/stm pair, four
 * instructions where the ROM has six. Particle::System::NewSimple stays spelled
 * as its mangled name -- its parameters are Fix12<int> BY VALUE and declaring
 * the true types changes how the caller passes them. */
/* This class's own reset routine, still unnamed and still under its func_ov002_
   symbol. It returns int; Kill drops it. */
void daObjTatefuda_c::Kill()
{
    Vector3 pos;
    Vector3 dustPos;
    Fix12i x = mPosX;
    Fix12i y = mPosY + 0x28000;
    Fix12i z = mPosZ;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xe, pos.x, pos.y, pos.z);
    dustPos.x = pos.x;
    dustPos.y = pos.y;
    dustPos.z = pos.z;
    DisappearPoofDustAt(dustPos);
    Sound::PlayBank3(0x41, *(Vector3 *)&mCamSpacePosX);
    func_ov002_020bae9c((char *)this);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 8 -- _ZN15daObjTatefuda_c15OnHitByMegaCharER6Player, 0x020bb374, size 0x44 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_c15OnHitByMegaCharER6Player
/* daObjTatefuda_c::OnHitByMegaChar -- vtable slot 27, ov002 0x020bb374.
 * reloc: _ZTV15daObjTatefuda_c+0x6c -> 0x020bb374, _ZTV10dBgActor_c+0x6c ->
 * 0x02010130 (different, real override).
 *
 * SIGNATURE FROM include/dActor_c.h's OWN SLOT 27, `virtual void
 * OnHitByMegaChar(Player &player)` -- `int` until daObjPile_c::OnHitByMegaChar
 * proved it wrong tree-wide (36bc6d1df). Same body shape
 * src/game/actors/d_a_obj_maruta.cpp records for its own
 * slot 27: dBgActor_c::KillByMegaChar is non-virtual, so the unqualified
 * call is already the direct `bl` the ROM has. mAngleY = mPrevAngleY is
 * dActor_c's own field pair (include/dActor_c.h, 0x08e/0x094). */
void daObjTatefuda_c::OnHitByMegaChar(Player &player)
{
    player.IncMegaKillCount();
    func_02012694(0x1d, &mCamSpacePosX);
    KillByMegaChar(player);
    mAngleY = mPrevAngleY;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 7 -- _ZN15daObjTatefuda_c15OnGroundPoundedER8dActor_c, 0x020bb27c, size 0xf8 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_c15OnGroundPoundedER8dActor_c
/* daObjTatefuda_c::OnGroundPounded -- vtable slot 21, ov002 0x020bb27c.
 * reloc: _ZTV15daObjTatefuda_c+0x54 -> 0x020bb27c, _ZTV10dBgActor_c+0x54 ->
 * 0x02010148 (different, real override).
 *
 * include/dActor_c.h's own slot 21 supplies the signature -- `void`, the
 * tree-wide fix from daObjPile_c::OnGroundPounded (36bc6d1df).
 *
 * mPoundsLeft/mPoundCooldown/mRespawnDelay are this class's own fields (include/daObjTatefuda_c.h);
 * mPoundCooldown and mRespawnDelay were undescribed padding until this method's body
 * proved they are read/written. `&other + 0x703` reads past dActor_c's own
 * span -- same raw-offset reading daObjPile_c::OnGroundPounded records
 * for its own slot 21. */
void daObjTatefuda_c::OnGroundPounded(dActor_c &other)
{
    if (mPoundsLeft == 0) return;
    if (mPoundCooldown != 0) return;
    Sound::PlayBank3(0x62, *(const Vector3 *)&mCamSpacePosX);
    if (other.param1 == 2 || *(unsigned char *)((char *)&other + 0x703) != 0) {
        mPosY -= (mPoundsLeft * 0x2d) << 12;
        mPoundsLeft = 0;
        ((dBgActor_c *)this)->UpdateModelPosAndRotY();
        ((dBgActor_c *)this)->UpdateClsnPosAndRot();
        mRespawnDelay = 0x1e;
    } else {
        mPosY -= 0x2d000;
        mPoundsLeft -= 1;
        ((dBgActor_c *)this)->UpdateModelPosAndRotY();
        ((dBgActor_c *)this)->UpdateClsnPosAndRot();
        mPoundCooldown = 0xf;
        mRespawnDelay = 0x1e;
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 6 -- _ZN15daObjTatefuda_c11OnAttacked1ER8dActor_c, 0x020bb23c, size 0x40 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_c11OnAttacked1ER8dActor_c
/* daObjTatefuda_c::OnAttacked1 -- vtable slot 22, ov002 0x020bb23c.
 * reloc: _ZTV15daObjTatefuda_c+0x58 -> 0x020bb23c, _ZTV10dBgActor_c+0x58 ->
 * 0x02010144 (different, real override).
 *
 * include/dActor_c.h's own slot 22 supplies the signature -- still `int`,
 * unlike slots 21/24/27 (see 36bc6d1df).
 *
 * The pre-migration recovery read `other`'s actorID (dActor_c +0xc) through
 * a shadow struct and dispatched through a bare virtual-call shape (`Base::M`
 * at the shadow's slot 31, this class's own Kill -- include/daObjTatefuda_c.h). An
 * unqualified `Kill()` here is that same virtual dispatch. */
int daObjTatefuda_c::OnAttacked1(dActor_c &other)
{
    int isCode = (other.actorID == 0xce);
    if (isCode) {
        Kill();
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 5 -- func_ov002_020bb060, 0x020bb060, size 0x1dc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bb060
#pragma cplusplus off
void func_ov002_020bb060(char *self)
{
    struct Vec3 v;
    struct Vec3 lo;
    char *result = func_ov002_020e496c(*(void **)(self + 0x59c));
    char *m2 = *(char **)(result + 0x14);
    int r4 = 0;

    if (_ZN6Player14IsFrontSlidingEv(*(void **)(self + 0x59c)))
        r4 = 1;
    if (_ZN6Player17LostGrabbedObjectEv(*(void **)(self + 0x59c))) {
        if ((unsigned int)(*(int *)(result + 0x58) << 4) >> 0x10 < 0xe)
            r4 = 1;
    }

    _Z14ApproachLinearRsss((short *)(self + 0x8c), data_ov002_020ff0d0[r4], 0x1000);

    ((int *)&v)[0] = 0;
    ((int *)&v)[1] = 0;
    ((int *)&v)[2] = 0;
    data_020a0e68 = *(struct Matrix4x3 *)(result + 0x1c);
    MulMat4x3Mat4x3(m2 + 0x2a0, &data_020a0e68, &data_020a0e68);
    v.x = data_020a0e68.t.x;
    v.y = data_020a0e68.t.y;
    v.z = data_020a0e68.t.z;

    *(short *)(self + 0x94) = *(short *)(*(char **)(self + 0x59c) + 0x8e);
    *(short *)(self + 0x8e) = *(short *)(self + 0x94);

    Matrix4x3_FromTranslation(&data_020a0e68, v.x, v.y, v.z);
    Matrix4x3_ApplyInPlaceToTranslation(&data_020a0e68, 0, 0x8c00, 0);
    Matrix4x3_ApplyInPlaceToRotationXYZExt(&data_020a0e68, *(short *)(self + 0x8c), *(short *)(self + 0x8e), *(short *)(self + 0x90));
    Matrix4x3_ApplyInPlaceToTranslation(&data_020a0e68, 0, -0x8c00, 0);
    Matrix4x3_ApplyInPlaceToTranslation(&data_020a0e68, *(int *)((char *)data_ov002_020ff0d4 + r4 * 0xc), *(int *)((char *)data_ov002_020ff0d8 + r4 * 0xc), *(int *)((char *)data_ov002_020ff0dc + r4 * 0xc));

    *(int *)(self + 0x5c) = data_020a0e68.t.x;
    *(int *)(self + 0x60) = data_020a0e68.t.y;
    *(int *)(self + 0x64) = data_020a0e68.t.z;
    Vec3_Lsl(&lo, (struct Vec3 *)(self + 0x5c), 3);
    *(int *)(self + 0x5c) = lo.x;
    *(int *)(self + 0x60) = lo.y;
    *(int *)(self + 0x64) = lo.z;
    *(struct Matrix4x3 *)(self + 0xf0) = data_020a0e68;
}
#pragma cplusplus on

/* -------------------------------------------------------------------------- */
/* ROM ordinal 4 -- func_ov002_020bafc0, 0x020bafc0, size 0xa0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bafc0
/* recovered: shared common types */
#pragma cplusplus off
void func_ov002_020bafc0(char* self){
    struct Vector3 v;
    Vec3_Asr(&v, (struct Vector3*)(self + 0x5c), 3);
    Matrix4x3_FromTranslation(&data_020a0e68, v.x, v.y, v.z);
    Matrix4x3_ApplyInPlaceToTranslation(&data_020a0e68, 0, 0x8c00, 0);
    Matrix4x3_ApplyInPlaceToRotationZXYExt(&data_020a0e68,
        *(s16*)(self + 0x8c), *(s16*)(self + 0x8e), *(s16*)(self + 0x90));
    Matrix4x3_ApplyInPlaceToTranslation(&data_020a0e68, 0, -0x8c00, 0);
    *(struct Matrix4x3*)(self + 0xf0) = data_020a0e68;
}
#pragma cplusplus on

/* -------------------------------------------------------------------------- */
/* ROM ordinal 3 -- func_ov002_020baf80, 0x020baf80, size 0x40 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020baf80
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020baf80(char *t)
{
    Matrix4x3_FromRotationY(t + 0x380, *(short *)(t + 0x8e));
    *(int *)(t + 0x3a4) = *(int *)(t + 0x5c) >> 3;
    *(int *)(t + 0x3a8) = *(int *)(t + 0x60) >> 3;
    *(int *)(t + 0x3ac) = *(int *)(t + 0x64) >> 3;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 2 -- func_ov002_020bae9c, 0x020bae9c, size 0xe4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020bae9c
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020bae9c(char *c)
{
  *((unsigned char *) (c + 0x590)) = 1;
  *((unsigned char *) (c + 0x591)) = 0x1e;
  *((unsigned char *) (c + 0x58c)) = 0;
  *((int *) (c + 0x584)) = 0;
  *((int *) (c + 0x588)) = 0;
  *((int *) (c + 0x5c)) = *((int *) (c + 0x3b0));
  *((int *) (c + 0x60)) = *((int *) (c + 0x3b4));
  *((int *) (c + 0x64)) = *((int *) (c + 0x3b8));
  *((short *) (c + 0x8c)) = *((short *) (c + 0x3bc));
  *((short *) (c + 0x8e)) = *((short *) (c + 0x3be));
  *((short *) (c + 0x90)) = *((short *) (c + 0x3c0));
  *((unsigned char *) (c + 0x58e)) = 2;
  _ZN10dBgActor_c21UpdateModelPosAndRotYEv(c);
  _ZN10dBgActor_c19UpdateClsnPosAndRotEv(c);
  func_ov002_020baf80(c);
  func_ov002_020bbd5c((C *)c, 0);
  *((unsigned char *) (c + 0x31c)) = 0;
  _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(c + 0x320, c, 0x64000, 0x64000, 0x4800002, 0x41000);
  if (_ZN4dBgW9IsEnabledEv(c + 0x124))
  {
    _ZN4dBgW7DisableEv(c + 0x124);
  }
  _ZN5dCc_c5ClearEv(c + 0x320);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 1 -- _ZN15daObjTatefuda_cD0Ev, 0x020bae2c, size 0x70 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_cD0Ev
/* The deleting destructor comes from the key function: Kill is defined in
 * this TU (real body, vtable slot 31), so mwcc emits the vtable, the RTTI
 * and the destructor variants alongside it. No shard tricks remain. */

/* -------------------------------------------------------------------------- */
/* ROM ordinal 0 -- _ZN15daObjTatefuda_cD1Ev, 0x020badd0, size 0x5c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN15daObjTatefuda_cD1Ev
/* The complete-object destructor comes from the key function (see D0). */
