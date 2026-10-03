//cpp
/* daYegg_c -- the Yoshi egg (registry profile YOSHI_EGG).
 *
 * The egg a Yoshi carries and throws. The state table at 0x02110a5c holds
 * two member-function pointers per state, enter and execute, and the state
 * number lives at +0x3f0: func_ov002_020ed63c installs a state and runs its
 * enter, func_ov002_020ed684 runs the current execute from Behavior. While
 * held it follows the player at +0x38c (func_ov002_020ed0d4,
 * func_ov002_020ed5b0); once thrown it homes on the nearest actor it has
 * not hit yet (func_ov002_020edb3c), and on contact or after five hits it bursts
 * (func_ov002_020edca4), paying out coins, a blue coin or the tracked star
 * as param1 asks (func_ov002_020ec610 / 020ec628 / 020ec640 read the bits of
 * +0x428).
 *
 * This file is the whole linker unit 0x020ec56c..0x020ee42c, 33 functions:
 * D1 and D0 (src/game/actors/d_a_warpkun.cpp ends exactly at 0x020ec56c
 * below them), the twenty-six helpers func_ov002_020ec610 through
 * func_ov002_020eddc4, CleanupResources, Render, Behavior, InitResources,
 * and last the registry factory daYegg_c_classInit (0x020ee3d8);
 * src/actors/dBgActor_c.cpp starts exactly at 0x020ee42c above it. The
 * out-of-line destructor is the key function, so this TU also emits the
 * vtable and the RTTI.
 *
 * It replaces the one-function sources for _ZN8daYegg_cD1Ev,
 * _ZN8daYegg_cD0Ev, func_ov002_020ec610 .. func_ov002_020eddc4,
 * _ZN8daYegg_c16CleanupResourcesEv, _ZN8daYegg_c6RenderEv,
 * _ZN8daYegg_c8BehaviorEv, _ZN8daYegg_c13InitResourcesEv and
 * daYegg_c_classInit. Each member keeps the provenance notes its source
 * carried.
 *
 * `#pragma defer_codegen off` keeps this file in ROM order. It also scopes
 * opt_strength_reduction off to func_ov002_020edb3c and InitResources, the
 * two members whose sources carried it; with codegen deferred the pragma is
 * last-wins over the whole file.
 *
 * Leftover: most helpers keep their C-ABI cartridge names and reach the egg
 *   through raw offsets. daYegg_c.h names the fields InitResources and
 *   Behavior use; the rest is padding there, and naming it is its own change.
 * Leftover: the callees with Fix12<int> parameters (ModelAnim::SetAnim,
 *   dCcAc_c::Init, dBgCh_Actr::Init, the shadow drops, Particle::System::New)
 *   stay spelled as mangled extern-C free functions. A real method call homes
 *   a class-typed by-value argument and size-DIFFs the caller
 *   (notes/mwccarm-codegen.md 6az).
 */

#pragma defer_codegen off

#include "daYegg_c.h"
#include "types.h"
#include "common.h"
#include "SharedFilePtr.h"
#include "Player.h"

struct dBgPi;

/* Three plain words: the stack vectors the C helpers build, which carry
   none of Vector3's empty destructor. */
struct Vec3i { s32 x, y, z; };
struct Vec3_16f { s16 x, y, z; };

/* The scratch matrix at 0x020a0e68, as twelve words. */
struct M48 { int w[12]; };
struct Spawn { struct Vec3_16f rot; struct Vec3i pos; };

/* func_ov002_020ecd18's vector, copied by hand: the by-value result of the
   followed actor's slot 30 comes back through it. */
struct Vec3 { s32 x, y, z; Vec3() {} Vec3(const Vec3 &o) { x = o.x; y = o.y; z = o.z; } };

/* The followed actor seen through its vtable, as far as func_ov002_020ecd18
   reaches: slot 30 returns a position by value. */
struct VObj {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual struct Vec3 v30();
};

/* The same actor as func_ov002_020edb3c asks it: slot 20 is a yes/no. */
struct VObjQuery {
    virtual int s0(); virtual int s1(); virtual int s2(); virtual int s3();
    virtual int s4(); virtual int s5(); virtual int s6(); virtual int s7();
    virtual int s8(); virtual int s9(); virtual int s10(); virtual int s11();
    virtual int s12(); virtual int s13(); virtual int s14(); virtual int s15();
    virtual int s16(); virtual int s17(); virtual int s18(); virtual int s19();
    virtual int s20();
};

/* A ground probe on the stack, opaque; its constructor and destructor are
   called by name. */
typedef struct dBgCh_Gnd { char buf[0x68 - 0x18]; } dBgCh_Gnd;

/* The state table at 0x02110a5c: two member-function pointers per state,
   enter and execute, indexed by the state number at +0x3f0. The receiver is
   completed after the pointer type is formed, as the legacy shards did. */
struct C; typedef void (C::*PMF)();
struct Entry { PMF pmf[2]; };
extern Entry data_ov002_02110a5c[];
struct C { char pad[0x3f0]; int idx; };

extern "C" {
/* Defined below. */
int func_ov002_020ec610(unsigned char *p);
int func_ov002_020ec628(unsigned char *p);
unsigned char func_ov002_020ec640(unsigned char *p);
int func_ov002_020ec654(unsigned char *p);
void func_ov002_020ec670(char *self, int arg);
void func_ov002_020ec728(char *c);
void func_ov002_020ec80c(char *a, void *b, int count, int sl, short arg5);
void func_ov002_020ecb0c(void *arg0);
void func_ov002_020ecd18(void *arg0);
void func_ov002_020ed63c(C *c, int i);
void func_ov002_020ed684(C *c);
int func_ov002_020ed6cc(void *c);
void func_ov002_020ed738(char *c);
void func_ov002_020ed7f8(void *self);
void func_ov002_020ed998(char *c);
int func_ov002_020edb3c(char *self, int a1, int best);
void func_ov002_020edca4(char *self);
int func_ov002_020eddc4(char *self);

/* Data. 0x0210e6b0 and 0x0210eb78 are the two shared animation files; +4 of
   each is the file the shared loader put there. */
extern char data_ov002_0210e6b0[];
extern char data_ov002_0210eb78[];
extern s16 data_ov002_021000a8[];
extern s16 data_02082214[];
extern int data_0209e650;
extern M48 data_020a0e68;

extern int func_0203567c(int p);
extern int func_02035638(u8 *p);
extern void func_ov002_020e7218(char *a, char *b, int c);
extern char *data_ov002_021000a0[];
extern void Matrix4x3_FromRotationZXYExt(void *m, int x, int y, int z);
extern int _ZNK5dBgPi9GetClsnIDEv(struct dBgPi *thiz);
extern char *_ZN8dActor_c10FindWithIDEj(u32 id);
extern char *_ZN8dActor_c4NextEPKS_(const void *prev);
extern char *_ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(unsigned int a, unsigned int b, const void *v, const void *w, int e, int f);
/* Was spelled `func_02123804`, a name no symbols.txt defines. ov002's relocs
   record the call at 0x020ec718 as `overlays(77,78,79,80)`: four same-base
   overlays each put a function at 0x02123804. The call is gated on actor
   types 0xa4 and 0xa5, BATAN and BATANKING, both daBtn_c in ov079, and
   ov079's function there is daBtn_c's hit reaction, which takes the body and
   the actor that hit it -- the call's (actor, egg). */
extern void func_ov079_02123804(struct daBtn_c *self, dActor_c *other);
extern void _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(void *thiz, const void *v, unsigned int n, Fix12i f, short s);
extern int RandomIntInternal(int *seed);
extern void _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(void *anim, void *file, int a, int b, unsigned int u);
extern void _Z15ApproachLinear2Rsss(s16 *p, short a, short b);
extern int func_ov002_020d5f98(void *c, unsigned char *a, unsigned char *b);
extern int func_ov002_020d6048(void *c);
extern void _ZN7fBase_c18MarkForDestructionEv(void *c);
extern void _ZN5dCc_c5ClearEv(void *p);
extern void _ZN5dCc_c6UpdateEv(void *p);
extern int _ZNK10dBgCh_Actr10IsOnGroundEv(void *p);
extern s32 _ZNK10dBgCh_Actr8IsOnWallEv(void *self);
extern s32 _ZNK10dBgCh_Actr14GetResultFlag1Ev(void *self);
extern void *_ZNK10dBgCh_Actr14GetFloorResultEv(void *self);
extern void _ZN8dActor_c9UpdatePosEP5dCc_c(void *c, void *cc);
extern void dBgCh_Actr_UpdateContinuous_Veneer(void *p);
extern void dBgCh_Actr_UpdateDiscreteNoLava_veneer(void *self);
extern s32 _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(u32 a, u32 b, s32 x, s32 y, s32 z, struct Vec3_16f *rot, void *cb);
extern void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(u32 a, int b, int c, int d);
extern void _ZN8Particle20RunningSlidingDustAtE5Fix12IiES1_S1_(s32 x, s32 y, s32 z);
extern s32 _ZN5Sound8PlayLongEjjjRK7Vector3s(u32 a, u32 b, u32 c, void *v, u32 d);
extern void _ZN5Sound13PlayCharVoiceEjjRK7Vector3(u32 a, u32 b, const void *pos);
extern s32 _Z14ApproachLinearR7Vector3RKS_5Fix12IiE(void *dst, const void *src, s32 step);
extern s16 Vec3_HorzAngle(const void *a, const void *b);
extern int Vec3_HorzDist(const void *a, const void *b);
extern int Vec3_Dist(const void *a, const void *b);
extern void Vec3_Add(void *out, const void *a, const void *b);
extern void MulVec3Mat4x3(const void *in, const void *m, void *out);
extern void func_0203568c(void *p, int v);
extern void _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(void *mc, void *a, Fix12i r, unsigned int h, unsigned int x, unsigned int y);
extern void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(void *self, void *actor, int radius, int height, void *a, void *b);
extern unsigned char DecIfAbove0_Byte(unsigned char *p);
extern int _ZN4cstd4fdivEii(int a, int b);
extern void Math_Function_0203b14c(void *ptr, int target, int rate, int limit, int step);
extern void ApproachAngle(void *cur, short target, int divisor, int band, int maxStep);
extern void _Z11UpdateAngleRssis(short *a, int b, int c, short d);
extern int func_02010844(void *unused, void *v, short angle);
extern void _ZNK11SurfaceInfo12CopyNormalToER7Vector3(void *self, void *out);
extern void *_ZN9dBgCh_GndC1Ev(dBgCh_Gnd *self);
extern void _ZN9dBgCh_Gnd12SetObjAndPosERK7Vector3P8dActor_c(dBgCh_Gnd *self, const void *v, void *actor);
extern int _ZN9dBgCh_Gnd10DetectClsnEv(dBgCh_Gnd *self);
extern void _ZN9dBgCh_GndD1Ev(dBgCh_Gnd *self);
extern void Matrix4x3_FromRotationY(void *m, int angle);
extern void Matrix4x3_FromTranslation(M48 *m, int x, int y, int z);
extern void Matrix4x3_ApplyInPlaceToTranslation(M48 *m, int x, int y, int z);
extern void Matrix4x3_ApplyInPlaceToRotationZXYExt(M48 *m, int x, int y, int z);
extern void Matrix4x3_ApplyInPlaceToRotationXYZExt(M48 *m, int x, int y, int z);
extern int func_ov002_020cf700(void *g);
extern int func_ov002_020d0d2c(void *g);
extern void _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(void *self, void *shadow, void *mtx, int fix, int t, unsigned int n);
extern void _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(void *self, void *shadow, void *mtx, int fix, int t1, int t2, unsigned int n);
extern void _ZN8dActor_c11UntrackStarERa(void *self, signed char *r);
extern unsigned char _ZN8dActor_c9TrackStarEjj(void *self, u32 star, u32 kind);
extern void _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(void *self, const void *pos, u32 a, int fix, u32 b, u32 c, u32 d);
extern void LoadBlueCoinModel(void *self);
extern void UnloadBlueCoinModel(void *self);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinals 0 and 1 -- _ZN8daYegg_cD1Ev, 0x020ec56c, size 0x48;
 *                         _ZN8daYegg_cD0Ev, 0x020ec5b4, size 0x5c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daYegg_cD1Ev
// @symbol _ZN8daYegg_cD0Ev
/* One native destructor definition emits both ROM variants. D1 is one vtable
 * store and five destructor calls, every one a consequence of
 * `struct daYegg_c : dEnemyBase_c` and the four members that declaration
 * types, destroyed in reverse declaration order, then dEnemyBase_c; the
 * class adds no member with a destructor of its own -- a Player pointer and
 * scalars. D0 is the same destruction followed by the inherited actor-heap
 * operator delete. Being the first out-of-line virtual, it makes this file the
 * key function's home, so the vtable and RTTI are emitted here too. */
daYegg_c::~daYegg_c()
{
}

#ifdef _MSC_VER
/* The host uses the flat ROM D0 name, which MSVC never emits: it folds the
 * Itanium destructor variants into the one ~daYegg_c() above. This arm
 * spells out what the deleting destructor does -- the D1 body, called
 * qualified so it is a direct call, then the class-specific operator delete.
 * Nothing here reaches mwccarm. */
extern "C" daYegg_c *_ZN8daYegg_cD0Ev(daYegg_c *thiz)
{
    thiz->daYegg_c::~daYegg_c();
    daYegg_c::operator delete(thiz);
    return thiz;
}
#endif

/* -------------------------------------------------------------------------- */
/* ROM ordinal 2 -- func_ov002_020ec610, 0x020ec610, size 0x18 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec610
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov002_020ec610(unsigned char *p){
  return ((p[0x428] >> 7) & 1) != 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 3 -- func_ov002_020ec628, 0x020ec628, size 0x18 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec628
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov002_020ec628(unsigned char *p){
  return ((p[0x428] >> 6) & 1) != 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 4 -- func_ov002_020ec640, 0x020ec640, size 0x14 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec640
extern "C" {  /* .c-derived member: C linkage for the whole block */
unsigned char func_ov002_020ec640(unsigned char *p){
  return (p[0x428] >> 2) & 0xf;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 5 -- func_ov002_020ec654, 0x020ec654, size 0x1c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec654
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov002_020ec654(unsigned char *p) {
    return (((p[0x428] & 3) - 1) & 1) != 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 6 -- func_ov002_020ec670, 0x020ec670, size 0xb8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec670
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ec670(char* self, int arg)
{
    struct dBgPi* cr;
    struct dActor_c* actor;
    u16 type;

    cr = (struct dBgPi*)func_0203567c(arg);
    if (_ZNK5dBgPi9GetClsnIDEv(cr) == -1) return;

    actor = (struct dActor_c*)_ZN8dActor_c10FindWithIDEj(_ZNK5dBgPi9GetClsnIDEv(cr));
    if (actor == 0) return;

    type = *(u16*)((char*)actor + 0xc);
    {
        int t = (int)(type == 0xa1);
        if (t != 0) {
            *(u8*)((char*)actor + 0x3a2) = 1;
            return;
        }
    }
    {
        int t = (int)(type == 0xa4);
        if (t != 0) goto docall;
    }
    {
        int t = (int)(type == 0xa5);
        if (t == 0) return;
    }
docall:
    func_ov079_02123804((struct daBtn_c *)actor, (dActor_c *)self);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 7 -- func_ov002_020ec728, 0x020ec728, size 0xe4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec728
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ec728(char* c)
{
    unsigned int idx = *(unsigned char*)(c + 0x420);
    unsigned int n;
    struct Vector3 vec;
    if (idx >= 5) return;
    if (*(unsigned char*)(c + idx + 0x421) != 0) return;
    n = func_ov002_020ec640((unsigned char*)c);
    {
        int tx = *(int*)(c + 0x5c);
        int tz = *(int*)(c + 0x64);
        int ty = *(int*)(c + 0x60) + 0x28000;
        vec.x = tx;
        vec.y = ty;
        vec.z = tz;
    }
    if (n != 0) {
        struct Vector3 v2;
        v2.x = vec.x;
        v2.y = vec.y;
        v2.z = vec.z;
        _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(c, &v2, n, 0x2000, 0);
    }
    if (func_ov002_020ec628((unsigned char*)c)) {
        struct Vector3 v3;
        v3.x = *(int*)&vec.x;
        v3.y = *(int*)&vec.y;
        v3.z = *(int*)&vec.z;
        func_ov002_020ec80c(c, &v3, 1, 0x2000, 0);
    }
    *(unsigned char*)(c + *(unsigned char*)(c + 0x420) + 0x421) = 1;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 8 -- func_ov002_020ec80c, 0x020ec80c, size 0x12c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec80c
extern "C" {  /* .c-derived member: C linkage for the whole block */
    void func_ov002_020ec80c(char* a, void* b, int count, int sl, short arg5)
    {
        unsigned int r;
        int zero;
        char* n;
        int rv;
        int prev;
        int i;
        prev = 0xff;
        if (count > 1) {
            if (sl < 0x4000) sl = 0x4000;
        }
        i = 0;
        if (count <= 0) return;
        zero = i;
        do {
            n = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x122, 2, b, 0, *(signed char*)(a + 0xcc), -1);
            if (n != 0) {
                do {
                    rv = (int)(((unsigned int)RandomIntInternal(&data_0209e650) >> 16) << 27) >> 16;
                } while (rv == prev);
                r = (unsigned int)RandomIntInternal(&data_0209e650);
                r = r >> 16;
                *(short*)(n + 0x92) = zero;
                sl = sl * ((r % 50) + 100) / 100;
                prev = rv;
                *(short*)(n + 0x94) = arg5 + rv;
                *(short*)(n + 0x96) = zero;
                *(int*)(n + 0x98) = sl;
            }
            i++;
        } while (i < count);
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 9 -- func_ov002_020ec938, 0x020ec938, size 0x40 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec938
extern "C" {
void func_ov002_020ec938(char* c){
    _ZN8dActor_c9UpdatePosEP5dCc_c(c, c + 0x110);
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x144))
        func_ov002_020edca4(c);
    dBgCh_Actr_UpdateContinuous_Veneer(c + 0x144);
    _ZN5dCc_c5ClearEv(c + 0x110);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 10 -- func_ov002_020ec978, 0x020ec978, size 0x4c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec978
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ec978(char *p) {
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(p + 0x300, *(void **)(data_ov002_0210eb78 + 4), 0, 0x1000, 0);
    *(int *)(p + 0x98) = 0;
    *(int *)(p + 0x9c) = -0x2000;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 11 -- func_ov002_020ec9c4, 0x020ec9c4, size 0x110 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ec9c4
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ec9c4(char* c){
  if (*(unsigned char*)(c + 0x41d) != 0){
    _Z15ApproachLinear2Rsss((s16*)(c + 0x8c), 0, 0x400);
    _Z15ApproachLinear2Rsss((s16*)(c + 0x90), 0, 0x400);
    if (*(s16*)(c + 0x8c) == 0){
      if (*(s16*)(c + 0x8e) == 0)
        *(unsigned char*)(c + 0x41d) = 0;
    }
  } else {
    unsigned char a, b;
    if (func_ov002_020d5f98(*(void**)(c + 0x38c), &a, &b)){
      unsigned char ip;
      a = a * 2;
      ip = a;
      *(s16*)(c + 0x8c) = b * data_ov002_021000a8[ip];
      *(s16*)(c + 0x90) = b * data_ov002_021000a8[ip + 1];
      *(unsigned char*)(c + 0x41d) = 1;
    }
  }
  if (!func_ov002_020d6048(*(void**)(c + 0x38c)))
    _ZN7fBase_c18MarkForDestructionEv(c);
  _ZN5dCc_c5ClearEv(c + 0x110);
  _ZN5dCc_c6UpdateEv(c + 0x110);
  if (!_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x144))
    _ZN8dActor_c9UpdatePosEP5dCc_c(c, c + 0x110);
  dBgCh_Actr_UpdateContinuous_Veneer(c + 0x144);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 12 -- func_ov002_020ecad4, 0x020ecad4, size 0x38 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ecad4
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ecad4(char *p) {
    *(int *)(p + 0x98) = 0;
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(p + 0x300, *(void **)(data_ov002_0210eb78 + 4), 0, 0x1000, 0);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 13 -- func_ov002_020ecb0c, 0x020ecb0c, size 0x20c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ecb0c
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ecb0c(void *arg0)
{
    char *c = (char *)arg0;
    char *o;
    u32 id;
    s32 flag;
    s32 b;
    u32 fb0;
    s32 onwall;
    struct Spawn sp;
    s16 rx;
    s16 rz;
    u16 ang;
    struct Vec3_16f *pr;
    s32 pz;
    s32 px;

    id = *(u32 *)(c + 0x410);
    o = 0;
    if (id != 0)
        o = _ZN8dActor_c10FindWithIDEj(id);
    if (o != 0) {
        b = (s32)((*(u32 *)(o + 0xb0) & 0x10000000) != 0);
        if (b == 0) {
            func_ov002_020ec728(c);
            func_ov002_020ed63c((C *)c, 1);
            return;
        }
    }
    fb0 = *(u32 *)(c + 0xb0);
    flag = 0;
    b = (s32)((fb0 & 0x100) != 0);
    if (b == 0) {
        b = (s32)((fb0 & 0x2000) != 0);
        if (b != 0) {
            func_ov002_020ed63c((C *)c, 3);
        } else {
            b = (s32)((fb0 & 0x400) != 0);
            if (b != 0) {
                s32 py = *(volatile s32 *)(c + 0x60) + 0x28000;
                pz = *(s32 *)(c + 0x64);
                px = *(s32 *)(c + 0x5c);
                sp.pos.y = py;
                sp.pos.z = pz;
                sp.pos.x = px;
                ang = *(u16 *)(c + 0x8e);
                onwall = data_02082214[(ang >> 4) * 2];
                rx = (s16)onwall;
                sp.rot.y = 0;
                pr = &sp.rot;
                sp.rot.x = rx;
                ang = *(u16 *)(c + 0x8e);
                rz = data_02082214[(ang >> 4) * 2 + 1];
                sp.rot.z = rz;
                *(s32 *)(c + 0x414) = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
                    *(u32 *)(c + 0x414), 0x2c, sp.pos.x, sp.pos.y, pz, pr, 0);
                *(s32 *)(c + 0x418) = _ZN5Sound8PlayLongEjjjRK7Vector3s(
                    *(u32 *)(c + 0x418), 3, 0x93, c + 0x74, 0);
                flag = 1;
                func_ov002_020ed738(c);
                if (func_ov002_020eddc4(c) != 0)
                    return;
                onwall = _ZNK10dBgCh_Actr8IsOnWallEv(c + 0x144);
                if ((onwall | func_02035638((u8 *)(c + 0x144))) != 0) {
                    func_ov002_020edca4(c);
                    return;
                }
            }
        }
    }
    if (func_ov002_020ed6cc(c) != 0) {
        func_ov002_020edca4(c);
        return;
    }
    _ZN5dCc_c5ClearEv(c + 0x110);
    if (flag == 1)
        _ZN5dCc_c6UpdateEv(c + 0x110);
    _ZN8dActor_c9UpdatePosEP5dCc_c(c, c + 0x110);
    dBgCh_Actr_UpdateContinuous_Veneer(c + 0x144);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 14 -- func_ov002_020ecd18, 0x020ecd18, size 0x27c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ecd18
void func_ov002_020ecd18(void *arg0)
{
    char *c = (char *)arg0;
    char *o;
    s32 flag;
    s32 b;
    u32 fb0;

    o = 0;
    if (*(u32 *)(c + 0x410) != 0)
        o = _ZN8dActor_c10FindWithIDEj(*(u32 *)(c + 0x410));
    if (o != 0) {
        b = (s32)((*(u32 *)(o + 0xb0) & 0x10000000) != 0);
        if (b == 0) {
            func_ov002_020ed63c((C *)c, 1);
            return;
        }
    }
    if (*(s32 *)(c + 0x410) != 0 && o == 0) {
        func_ov002_020ed63c((C *)c, 1);
        return;
    }
    fb0 = *(u32 *)(c + 0xb0);
    flag = 0;
    b = (s32)((fb0 & 0x100) != 0);
    if (b == 0) {
        b = (s32)((fb0 & 0x2000) != 0);
        if (b != 0) {
            func_ov002_020ed63c((C *)c, 3);
        } else {
            b = (s32)((fb0 & 0x400) != 0);
            if (b != 0) {
                if (_ZNK10dBgCh_Actr14GetResultFlag1Ev(c + 0x144) != 0) {
                    func_ov002_020ec670(c, (int)(c + 0x144));
                    func_ov002_020edca4(c);
                    return;
                }
                if (o != 0) {
                    struct Vec3 tmp = ((struct VObj *)o)->v30();
                    if (_Z14ApproachLinearR7Vector3RKS_5Fix12IiE(
                            (struct Vec3 *)(c + 0x5c), &tmp, *(s32 *)(c + 0x98)) != 0) {
                        switch (*(u16 *)(o + 0xc)) {
                        case 0xbd:
                        case 0xbe:
                        case 0xc6:
                        case 0xca:
                        case 0xdb:
                        case 0x151:
                            func_ov002_020edca4(c);
                            return;
                        default:
                            func_ov002_020ec728(c);
                            func_ov002_020ed63c((C *)c, 1);
                            return;
                        }
                    }
                    *(s16 *)(c + 0x8e) = Vec3_HorzAngle((struct Vec3 *)(c + 0x5c), &tmp);
                }
                _ZN8Particle20RunningSlidingDustAtE5Fix12IiES1_S1_(
                    *(s32 *)(c + 0x5c), *(s32 *)(c + 0x60), *(s32 *)(c + 0x64));
                flag = 1;
                if (func_ov002_020eddc4(c) != 0)
                    return;
            }
        }
    }
    if (func_ov002_020ed6cc(c) != 0) {
        func_ov002_020edca4(c);
        return;
    }
    _ZN5dCc_c5ClearEv(c + 0x110);
    if (flag == 1)
        _ZN5dCc_c6UpdateEv(c + 0x110);
    if (o == 0)
        _ZN8dActor_c9UpdatePosEP5dCc_c(c, c + 0x110);
    dBgCh_Actr_UpdateContinuous_Veneer(c + 0x144);
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 15 -- func_ov002_020ecf94, 0x020ecf94, size 0x34 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ecf94
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ecf94(unsigned char *p)
{
    if (func_ov002_020ec654(p))
        func_ov002_020ecb0c(p);
    else
        func_ov002_020ecd18(p);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 16 -- func_ov002_020ecfc8, 0x020ecfc8, size 0x10c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ecfc8
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ecfc8(char* c){
  char* r5;
  *(int*)(c + 0x410) = 0;
  _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0x300, *(void **)(data_ov002_0210e6b0 + 4), 0, 0x1000, 0);
  *(int*)(c + 0x98) = 0x64000;
  if (!func_ov002_020ec654((unsigned char*)c)){
    *(int*)(c + 0x9c) = 0;
    r5 = (char*)func_ov002_020edb3c(c, 0, 0x7d0000);
    func_0203568c(c + 0x144, 0x2a000);
  } else {
    *(int*)(c + 0x9c) = -0xa000;
    r5 = (char*)func_ov002_020edb3c(c, 0, 0xfa0000);
  }
  if (r5){
    *(s16*)(c + 0x8e) = Vec3_HorzAngle((struct Vector3*)(c + 0x5c), (struct Vector3*)(r5 + 0x5c));
  } else {
    if (*(unsigned char*)(c + 0x41c)){
      if (!func_ov002_020ec654((unsigned char*)c))
        func_ov002_020edca4(c);
    }
  }
  _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(c + 0x110, c, 0x64000, 0xc8000, 0x202000, 0);
  *(s16*)(c + 0x94) = *(s16*)(c + 0x8e);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 17 -- func_ov002_020ed0d4, 0x020ed0d4, size 0x4dc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ed0d4
// recovered name: daWarpkun_c_Kill
/* recovered: shared common types, renamed to Class_Method, declarations from a shared header */
/* recovered: shared common types, renamed to Class_Method */
/* daWarpkun_c::Kill - recovered from vtable slot identity */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ed0d4(char* self)
{
    struct Vector3 in, out;
    char* com;
    int* p;
    volatile struct Vector3 t1, t2;
    struct Vector3 v1, v2;
    struct Vector3 sumA, sumB, sumC;
    int *pt1, *pt2, *pv1, *pv2;

    com = *(char**)(self + 0x38c);
    *(int*)(self + 0x3f4) = -0x68000;
    p = (int*)(com + 0xa4);
    *(int*)(self + 0xa4) = p[0];
    *(int*)(self + 0xa8) = p[1];
    *(int*)(self + 0xac) = p[2];
    *(int*)(self + 0x98) = *(int*)(com + 0x98);

    if (DecIfAbove0_Byte((unsigned char*)(self + 0x41e)) == 0) {
        in.x = 0; in.y = 0; in.z = 0;
        out.x = 0; out.y = 0; out.z = 0;
        in.z = *(int*)(self + 0x3f4);

        Matrix4x3_FromRotationZXYExt(&data_020a0e68, *(short*)(self + 0x3ea), *(short*)(self + 0x3ec), *(short*)(self + 0x3ee));
        MulVec3Mat4x3(&in, &data_020a0e68, &out);

        if (*(int*)(*(char**)(self + 0x38c) + 0x37c) != 0
            || func_ov002_020cf700(*(char**)(self + 0x38c)) != 0
            || func_ov002_020d0d2c(*(char**)(self + 0x38c)) != 0) {
            if (_ZNK10dBgCh_Actr10IsOnGroundEv(self + 0x144) != 0) {
                *(short*)(self + 0x3e4) = *(short*)(*(char**)(self + 0x38c) + 0x8c);
                *(short*)(self + 0x3e8) = *(short*)(*(char**)(self + 0x38c) + 0x90);
                pt1 = (int*)(com + 0x68);
                pt2 = (int*)(com + 0x5c);
                t1.x = pt1[0];
                t1.y = pt1[1];
                t1.z = pt1[2];
                t2.x = pt2[0];
                t2.y = pt2[1];
                t2.z = pt2[2];
                Vec3_Add(&sumA, com + 0x5c, &out);
                *(int*)(self + 0x3d8) = sumA.x;
                *(int*)(self + 0x3dc) = sumA.y;
                *(int*)(self + 0x3e0) = sumA.z;
            } else {
                *(short*)(self + 0x3e4) = -0x4000;
                *(short*)(self + 0x3e8) = 0;
                Vec3_Add(&sumB, com + 0x5c, &out);
                *(int*)(self + 0x3d8) = sumB.x;
                *(int*)(self + 0x3dc) = sumB.y;
                *(int*)(self + 0x3e0) = sumB.z;
            }
        } else {
            *(short*)(self + 0x3e4) = *(short*)(com + 0x8c);
            *(short*)(self + 0x3e8) = *(short*)(com + 0x90);
            pv1 = (int*)(com + 0x68);
            pv2 = (int*)(com + 0x5c);
            v1.x = pv1[0];
            v1.y = pv1[1];
            v1.z = pv1[2];
            v2.x = pv2[0];
            v2.y = pv2[1];
            v2.z = pv2[2];
            if (Vec3_HorzDist(&v1, &v2) >= 0x5000) {
                *(short*)(self + 0x3e6) = Vec3_HorzAngle(&v1, &v2);
            }
            Vec3_Add(&sumC, com + 0x5c, &out);
            *(int*)(self + 0x3d8) = sumC.x;
            *(int*)(self + 0x3dc) = sumC.y;
            *(int*)(self + 0x3e0) = sumC.z;
        }
        *(unsigned char*)(self + 0x41e) = 0;
    }

    {
        int fd = _ZN4cstd4fdivEii(0x1000, 0x3000);
        Math_Function_0203b14c(self + 0x5c, *(int*)(self + 0x3d8), fd, 0x3e8000, 4);
        Math_Function_0203b14c(self + 0x60, *(int*)(self + 0x3dc), fd, 0x3e8000, 4);
        Math_Function_0203b14c(self + 0x64, *(int*)(self + 0x3e0), fd, 0x3e8000, 4);
    }

    ApproachAngle(self + 0x3ea, *(short*)(self + 0x3e4), 8, 0x4000, 0x100);
    ApproachAngle(self + 0x3ec, *(short*)(self + 0x3e6), 8, 0x4000, 0x100);
    ApproachAngle(self + 0x3ee, *(short*)(self + 0x3e8), 8, 0x4000, 0x100);

    *(short*)(self + 0x8e) = *(short*)(self + 0x3ec);

    {
        int v98 = *(int*)(self + 0x98);
        int t = v98 >> 5;
        if (t > 0x1000) t = 0x1000;
        if (t < 0) t = 0;
        t = (t + 0x1000) >> 8;
        unsigned char byteVal = (unsigned char)t;
        int idx = byteVal & 0x3f;
        int speed = idx << 8;
        if (*(unsigned char*)(*(char**)(self + 0x38c) + 0x6de) != 0) speed = 0;
        if (v98 != 0) {
            _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(self + 0x300, *(void **)(data_ov002_0210e6b0 + 4), 0, speed, 0);
        } else {
            _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(self + 0x300, *(void **)(data_ov002_0210eb78 + 4), 0, speed, 0);
        }
    }

    if (func_ov002_020ec654((unsigned char*)self) == 0) {
        int a4 = *(int*)(self + 0xa4);
        if (a4 < 0) a4 = -a4;
        *(int*)(self + 0xa4) = a4;
        int ac = *(int*)(self + 0xac);
        if (ac < 0) ac = -ac;
        *(int*)(self + 0xac) = ac;
        a4 = *(int*)(self + 0xa4);
        if (a4 > 0x20000) { a4 = 0x20000; *(int*)(self + 0xa4) = a4; }
        a4 = *(int*)(self + 0xa4);
        {
            int fd1 = _ZN4cstd4fdivEii(a4, 0x20000);
            int r1 = (int)(((long long)fd1 * 0xe39 + 0x800) >> 12);
            short a4ang = (short)(-r1);
            ac = *(int*)(self + 0xac);
            if (ac > 0x20000) { ac = 0x20000; *(int*)(self + 0xac) = ac; }
            ac = *(int*)(self + 0xac);
            {
                int fd2 = _ZN4cstd4fdivEii(ac, 0x20000);
                int r2 = (int)(((long long)fd2 * 0xe39 + 0x800) >> 12);
                *(short*)(self + 0x8c) = a4ang;
                *(short*)(self + 0x90) = (short)r2;
            }
        }
    } else {
        func_ov002_020ed738(self);
    }

    _ZN5dCc_c5ClearEv(self + 0x110);
    {
        int dist = Vec3_Dist(self + 0x5c, com + 0x5c);
        if (*(unsigned char*)(*(char**)(self + 0x38c) + 0x709) != 0) return;
        if (dist >= 0x190000) return;
        dBgCh_Actr_UpdateDiscreteNoLava_veneer(self + 0x144);
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 18 -- func_ov002_020ed5b0, 0x020ed5b0, size 0x8c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ed5b0
// Snaps this object to the actor it follows (+0x38c): copies its position
// (three words at +0x5c) and Y angle (+0x8e), mirrors the position into
// +0x3d8, and copies its rotation triple (+0x8c) into both +0x3e4 and +0x3ea.
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ed5b0(char* c)
{
    char* src;
    int* sp;
    short* m;
    src = *(char**)(c + 0x38c);
    sp = (int*)(src + 0x5c);
    *(int*)(c + 0x5c) = sp[0];
    *(int*)(c + 0x60) = sp[1];
    *(int*)(c + 0x64) = sp[2];
    src = *(char**)(c + 0x38c);
    *(short*)(c + 0x8e) = *(short*)(src + 0x8e);
    *(int*)(c + 0x3d8) = *(int*)(c + 0x5c);
    *(int*)(c + 0x3dc) = *(int*)(c + 0x60);
    *(int*)(c + 0x3e0) = *(int*)(c + 0x64);
    src = *(char**)(c + 0x38c);
    m = (short*)(src + 0x8c);
    *(short*)(c + 0x3e4) = m[0];
    *(short*)(c + 0x3e6) = m[1];
    *(short*)(c + 0x3e8) = m[2];
    src = *(char**)(c + 0x38c);
    m = (short*)(src + 0x8c);
    *(short*)(c + 0x3ea) = m[0];
    *(short*)(c + 0x3ec) = m[1];
    *(short*)(c + 0x3ee) = m[2];
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 19 -- func_ov002_020ed63c, 0x020ed63c, size 0x48 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ed63c
extern "C" void func_ov002_020ed63c(C *c, int i) { c->idx = i; int j = c->idx; (c->*data_ov002_02110a5c[j].pmf[0])(); }

/* -------------------------------------------------------------------------- */
/* ROM ordinal 20 -- func_ov002_020ed684, 0x020ed684, size 0x48 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ed684
extern "C" void func_ov002_020ed684(C *c) { int j = c->idx; (c->*data_ov002_02110a5c[j].pmf[1])(); }

/* -------------------------------------------------------------------------- */
/* ROM ordinal 21 -- func_ov002_020ed6cc, 0x020ed6cc, size 0x6c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ed6cc
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov002_020ed6cc(void* c) {
  Fix12i d = Vec3_HorzDist((struct Vector3*)((char*)c+0x3cc), (struct Vector3*)((char*)c+0x5c));
  *(int*)((char*)c+0x3cc) = *(int*)((char*)c+0x5c);
  *(int*)((char*)c+0x3d0) = *(int*)((char*)c+0x60);
  *(int*)((char*)c+0x3d4) = *(int*)((char*)c+0x64);
  if (d >= 0x32000) goto fail;
  if (DecIfAbove0_Byte((unsigned char*)((char*)c+0x41f))) goto ret0;
  return 1;
fail:
  *(unsigned char*)((char*)c+0x41f) = 0xf;
ret0:
  return 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 22 -- func_ov002_020ed738, 0x020ed738, size 0xc0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ed738
/* recovered: shared common types */
extern "C" {
void func_ov002_020ed738(char* c) {
    int e4 = 0;
    int e6 = 0;
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c+0x144)) {
        struct Vector3 n;
        void* fr = _ZNK10dBgCh_Actr14GetFloorResultEv(c+0x144);
        _ZNK11SurfaceInfo12CopyNormalToER7Vector3((char*)fr+4, &n);
        e4 = func_02010844(c, &n, *(short*)(c+0x8e));
        e6 = func_02010844(c, &n, (short)(*(short*)(c+0x8e) - 0x4000));
    } else {
        if (*(int*)(c+0x3f0) == 0) {
            short* p = *(short**)(c+0x38c);
            if (p != 0) {
                e4 = p[0x8c/2];
                e6 = p[0x90/2];
            }
        }
    }
    _Z11UpdateAngleRssis((short*)(c+0x8c), e4, 4, 0x1000);
    _Z11UpdateAngleRssis((short*)(c+0x90), e6, 4, 0x1000);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 23 -- func_ov002_020ed7f8, 0x020ed7f8, size 0x1a0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ed7f8
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020ed7f8(void *self)
{
    char *c = (char*)self;
    dBgCh_Gnd rg;
    struct Vec3i v;
    int r5;
    int r4;
    int b;

    if (*(unsigned char*)(*(char**)(c + 0x38c) + 0x6f5) < 1)
        return;

    v.x = *(int*)(c + 0x5c);
    v.y = *(int*)(c + 0x60);
    v.z = *(int*)(c + 0x64);
    v.y += 0x28000;
    _ZN9dBgCh_GndC1Ev(&rg);
    _ZN9dBgCh_Gnd12SetObjAndPosERK7Vector3P8dActor_c(&rg, &v, 0);
    r4 = v.y;
    if (_ZN9dBgCh_Gnd10DetectClsnEv(&rg))
        r4 = *(int*)((char*)&rg + 0x44);
    r5 = *(int*)(c + 0x60) - r4;
    if (r5 <= 0x1000) r5 = 0x1000;
    r4 = 0x50000 - (int)(((long long)r5 * 0x180 + 0x800) >> 12);
    if (r4 < 0xa000) r4 = 0xa000;
    Matrix4x3_FromRotationY(c + 0x390, *(short*)(c + 0x8e));
    *(int*)(c + 0x3b4) = *(int*)(c + 0x5c) >> 3;
    *(int*)(c + 0x3b8) = *(int*)(c + 0x60) >> 3;
    *(int*)(c + 0x3bc) = *(int*)(c + 0x64) >> 3;
    b = (*(int*)(c + 0xb0) & 0x40000) ? 1 : 0;
    if (b == 0
        && *(int*)(*(char**)(c + 0x38c) + 0x37c) == 0
        && !func_ov002_020cf700(*(void**)(c + 0x38c))
        && !func_ov002_020d0d2c(*(void**)(c + 0x38c)))
    {
        if (func_ov002_020ec654((unsigned char*)c) == 0) {
            _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(
                c, c + 0x364, c + 0x390, r4, r5 + 0x28000, 0xf);
        } else {
            _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(
                c, c + 0x364, c + 0x390, r4, r5 + 0x28000, r4, 0xf);
        }
    }
    _ZN9dBgCh_GndD1Ev(&rg);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 24 -- func_ov002_020ed998, 0x020ed998, size 0x1a4 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020ed998
extern "C" {
void func_ov002_020ed998(char *c)
{
    int on = (*(int *)(c + 0xb0) & 0x100) != 0;
    if (on && *(int *)(c + 0x38c)) {
        volatile struct Vec3i v;
        int t9, t10, t11;
        v.x = 0;
        v.y = 0;
        v.z = 0;
        data_020a0e68 = *(M48 *)(*(char **)(*(char **)(c + 0x38c) + 0xc8));
        Matrix4x3_ApplyInPlaceToTranslation(&data_020a0e68, 0x3f, 9, 0xb);
        Matrix4x3_ApplyInPlaceToRotationZXYExt(&data_020a0e68, 0xffffb60b, 0xfffff3e9, 0xffffd6c1);
        t9  = ((volatile M48 *)&data_020a0e68)->w[9];
        t10 = ((volatile M48 *)&data_020a0e68)->w[10];
        t11 = ((volatile M48 *)&data_020a0e68)->w[11];
        v.y = t10;
        v.z = t11;
        v.x = t9;
        *(int *)(c + 0x5c) = t9 << 3;
        *(int *)(c + 0x60) = v.y << 3;
        *(int *)(c + 0x64) = v.z << 3;
        return;
    }
    Matrix4x3_FromTranslation(&data_020a0e68,
        (int)((((long long)*(int *)(c + 0x5c) << 9) + 0x800) >> 12),
        (int)((((long long)(*(int *)(c + 0x60) + 0x14000) << 9) + 0x800) >> 12),
        (int)((((long long)*(int *)(c + 0x64) << 9) + 0x800) >> 12));
    Matrix4x3_ApplyInPlaceToRotationXYZExt(&data_020a0e68,
        *(short *)(c + 0x8c), *(short *)(c + 0x8e), *(short *)(c + 0x90));
    *(M48 *)(c + 0x31c) = data_020a0e68;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 25 -- func_ov002_020edb3c, 0x020edb3c, size 0x168 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020edb3c
extern "C" {
#pragma opt_strength_reduction off
int func_ov002_020edb3c(char *self, int a1, int best)
{
    char *found;
    char *actor;
    int matched;
    int b;
    struct P { int a, b, c, d; };
    volatile struct P _p;
    struct P *_q = (struct P *)&_p;

    if (*(unsigned char *)(self + 0x41c) >= 5) {
        if (func_ov002_020ec654((unsigned char *)self) == 0) {
            func_ov002_020edca4(self);
        }
        return 0;
    }

    found = 0;
    actor = (char *)_ZN8dActor_c4NextEPKS_(0);
    if (actor == 0) goto end;

loop:
    if (actor == self) goto next;
    if (actor == *(char **)(self + 0x38c)) goto next;
    b = (*(int *)(actor + 0xb0) & 0x10000000) != 0;
    if (!b) goto next;
    b = (*(int *)(actor + 0xb0) & 8) != 0;
    if (b) goto next;
    matched = 0;
    for (int i = 0; i < 5; i++) {
        int bv = *(int *)(self + (i << 2) + 0x3fc);
        if (bv == *(int *)(actor + 4)) matched = 1;
    }
    if (matched != 0) goto next;
    if (func_ov002_020ec654((unsigned char *)self) != 0) {
        if (((VObjQuery *)actor)->s20() == 0) goto next;
    }
    {
        int d = Vec3_Dist(self + 0x5c, actor + 0x5c);
        if (d < best) {
            best = d;
            found = actor;
        }
    }
next:
    actor = (char *)_ZN8dActor_c4NextEPKS_(actor);
    if (actor != 0) goto loop;

end:
    if (found != 0) {
        int idx = *(unsigned char *)(self + 0x41c);
        *(int *)(self + (idx << 2) + 0x3fc) = *(int *)(found + 4);
        *(int *)(self + 0x410) = *(int *)(found + 4);
        *(unsigned char *)(self + 0x41c) += 1;
    }
    return (int)found;
}
#pragma opt_strength_reduction on
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 26 -- func_ov002_020edca4, 0x020edca4, size 0x120 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020edca4
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov002_020edca4(char* self)
{
    struct Vector3 pos;
    char* spawned;
    int xv, zv, yv, sy;

    if (*(u8*)(self + 0x426) != 0) return;

    func_ov002_020ec728(self);

    yv = *(int*)(self + 0x60);
    zv = *(int*)(self + 0x64);
    sy = yv + 0x28000;
    xv = *(int*)(self + 0x5c);
    pos.x = xv;
    pos.y = sy;
    pos.z = zv;
    if (func_ov002_020ec610((unsigned char*)self) != 0) {
        _ZN8dActor_c11UntrackStarERa((struct dActor_c*)self, (signed char*)(self + 0x427));

        spawned = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0xb3, 0x10,
            &pos, 0, *(signed char*)(*(char**)(self + 0x38c) + 0xcc), -1);
        if (spawned != 0) {
            func_ov002_020e7218((char*)spawned, *(char**)(self + 0x38c), 1);
        }
    }

    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x3f, *(int*)(self + 0x5c), *(int*)(self + 0x60), *(int*)(self + 0x64));
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x40, *(int*)(self + 0x5c), *(int*)(self + 0x60), *(int*)(self + 0x64));
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x41, *(int*)(self + 0x5c), *(int*)(self + 0x60), *(int*)(self + 0x64));
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x42, *(int*)(self + 0x5c), *(int*)(self + 0x60), *(int*)(self + 0x64));
    _ZN7fBase_c18MarkForDestructionEv((struct dActor_c*)self);
    _ZN5Sound13PlayCharVoiceEjjRK7Vector3(0, 0x103, (const struct Vector3*)(self + 0x74));

    *(u8*)(self + 0x426) = 1;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 27 -- func_ov002_020eddc4, 0x020eddc4, size 0x190 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov002_020eddc4
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
/* func_ov002_020eddc4 at 0x020eddc4
 *
 * Matched byte-for-byte with mwccarm 1.2/sp2p3 (ov002).
 */
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov002_020eddc4(char* self)
{
    struct dActor_c* actor;
    struct dActor_c* other;
    u32 id;
    u32 flags;
    u16 type;

    id = *(u32*)(self + 0x134);
    if (id == 0) goto fail;

    actor = (struct dActor_c*)_ZN8dActor_c10FindWithIDEj(id);
    if (actor == 0) goto fail;

    if (actor == *(struct dActor_c**)(self + 0x38c)) goto fail;

    {
        int t = (int)(*(u16*)((char*)actor + 0xc) == 0xbf);
        if (t != 0) {
            flags = *(u32*)(self + 0x130);
            if (flags & 0x8000) {
                func_ov002_020ed63c((C *)self, 3);
                return 1;
            }
            if (!(flags & 0x26fe0)) {
                struct Vector3 pos;
                pos.x = *(int*)(self + 0x5c);
                pos.y = *(int*)(self + 0x60);
                pos.z = *(int*)(self + 0x64);
                _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(actor, &pos, 1, 0xc000, 1, 0, 1);
            }
        }
    }

    type = *(u16*)((char*)actor + 0xc);
    switch (type) {
    case 0xbd:
    case 0xbe:
    case 0xc6:
    case 0xca:
    case 0xdb:
    case 0x151:
        func_ov002_020edca4(self);
        return 1;
    }

    other = 0;
    id = *(u32*)(self + 0x410);
    if (id != 0) {
        other = (struct dActor_c*)_ZN8dActor_c10FindWithIDEj(id);
    }
    if (other != actor) goto fail;

    func_ov002_020ec728(self);
    func_ov002_020ed63c((C *)self, 1);
    return 1;

fail:
    return 0;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 28 -- _ZN8daYegg_c16CleanupResourcesEv, 0x020edf54, size 0x44 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daYegg_c16CleanupResourcesEv
/* recovered: named members + shared header, real C++ method */
int daYegg_c::CleanupResources()
{
  ((SharedFilePtr *)(&data_ov002_0210e6b0))->Release();
  ((SharedFilePtr *)(&data_ov002_0210eb78))->Release();
  if (func_ov002_020ec628((unsigned char *)this) != 0)
    UnloadBlueCoinModel(((void*)this));
  return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 29 -- _ZN8daYegg_c6RenderEv, 0x020edf98, size 0x78 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daYegg_c6RenderEv
/* recovered: named members + shared header, real C++ method -- vtable slot 9 */
int daYegg_c::Render()
{
    /* The temporary is load-bearing and must not be folded into the `if`, the
       same way it is in daBakubaku_c::Render: `if (mFlags & 0x40000)` tests the
       masked word directly, while the ROM materialises the 0/1 first. Folding it
       changes the function's SIZE, which is what a `999 word(s) differ` says. */
    int b = (int)((mFlags & 0x40000) != 0);
    if (b) return 1;

    if (mPlayer->IsInsideOfCannon()) return 1;
    if (mPlayer->mOpacity < 1) return 1;
    mModelAnim.Render(0);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 30 -- _ZN8daYegg_c8BehaviorEv, 0x020ee010, size 0x134 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daYegg_c8BehaviorEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header */
/* recovered: named members + shared header, real C++ method */
int daYegg_c::Behavior()
{
    Vector3 vin;
    Vector3 vmid;
    Vector3 vout;
    func_ov002_020ed684((C *)this);
    mModelAnim.Advance();
    if (unk_3f0 != 1) {
        if (mPlayer->mOpacity < 0xa) {
            /* BOTH oddities below are load-bearing, measured one at a time.

               `ang` must stay a pointer: reading the three angles as
               mPlayer->mAngleX/Y/Z instead reloads mPlayer per field and
               changes the function's size.

               `vin.z` really is written twice. Dropping the dead first store
               also changes the size, so the ROM's own source had it. */
            s16 *ang;
            vin.z = 0;
            vin.z = -0x68000;
            vin.x = 0;
            vin.y = 0;
            vmid.x = 0;
            vmid.y = 0;
            vmid.z = 0;
            ang = &mPlayer->mAngleX;
            Matrix4x3_FromRotationZXYExt(&data_020a0e68, ang[0], ang[1], ang[2]);
            MulVec3Mat4x3(&vin, &data_020a0e68, &vmid);
            Vec3_Add(&vout, &mPlayer->mPosX, &vmid);
            mPosX = vout.x;
            mPosY = vout.y;
            mPosZ = vout.z;
        }
        mModelAnim.ApplyOpacity(mPlayer->mOpacity, 0);
    } else {
        mModelAnim.ApplyOpacity(0x1f, 0);
    }
    func_ov002_020ed998((char *)this);
    func_ov002_020ed7f8(this);
    if (unk_421[unk_420] != 0)
        unk_420++;
    if (unk_420 >= 5) {
        func_ov002_020edca4((char *)this);
    }
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 31 -- _ZN8daYegg_c13InitResourcesEv, 0x020ee144, size 0x294 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN8daYegg_c13InitResourcesEv
/* recovered: named members + shared header, real C++ method
 *
 * Vtable slot 0. This was the last daYegg_c function still written as a C free
 * function reaching every field through a raw offset -- it named not one member,
 * which is why it could not even include its own header once daYegg_c became a
 * real dEnemyBase_c subclass.
 */
#pragma opt_strength_reduction off
int daYegg_c::InitResources()
{
    int idx;
    int i;

    unk_428 = (u8)(param1 >> 4);

    idx = 0;
    if (func_ov002_020ec654((unsigned char *)this) != 0)
        idx = 1;

    Animation::LoadFile(*(SharedFilePtr *)data_ov002_0210e6b0);
    Animation::LoadFile(*(SharedFilePtr *)data_ov002_0210eb78);
    if (mModelAnim.SetFile(*(BMD_File **)(data_ov002_021000a0[idx] + 4), 1, -1) == 0)
        return 0;

    /* The predicate is asked a SECOND time rather than reusing idx: the ROM calls
       0x020ec654 twice, and folding it into the index above loses a bl. */
    if (func_ov002_020ec654((unsigned char *)this) == 0) {
        if (mShadowModel.InitCylinder() == 0)
            return 0;
    } else {
        if (mShadowModel.InitCuboid() == 0)
            return 0;
    }

    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(
        &mModelAnim, *(void **)(data_ov002_0210eb78 + 4), 0, 0x1000, 0);

    mAreaId = -1;
    mSpawnPosX = mPosX;
    mSpawnPosY = mPosY;
    mSpawnPosZ = mPosZ;
    mVertAccel = -0x2000;
    mTerminalVelocity = -0x3c000;
    unk_3f0 = param1 & 3;

    switch (unk_3f0) {
    case 0:
    case 1:
        mScaleX = 0x1000;
        mScaleY = 0x1000;
        mScaleZ = 0x1000;
        _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(
            &mdCcAc_c, this, 0x46000, 0x8c000, 0x200002, 0xa08000);
        _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
            &mWithMeshClsn, this, 0x28000, 0x28000, 0, 0);
        break;
    case 2:
        mScaleX = 0x2000;
        mScaleY = 0x2000;
        mScaleZ = 0x2000;
        _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(
            &mdCcAc_c, this, 0x78000, 0xa0000, 0x200002, 0xa08000);
        _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
            &mWithMeshClsn, this, 0x64000, 0x64000, 0, 0);
        break;
    default:
        break;
    }

    for (i = 0; i < 5; i++)
        unk_3fc[i] = 0;

    unk_3ea = mAngleX;
    unk_3ec = mAngleY;
    unk_3ee = mAngleZ;
    unk_3e4 = mAngleX;
    unk_3e6 = mAngleY;
    unk_3e8 = mAngleZ;
    unk_41f = 0xf;

    if (func_ov002_020ec628((unsigned char *)this) != 0)
        LoadBlueCoinModel((char *)this);
    if (func_ov002_020ec610((unsigned char *)this) != 0)
        mStarSlot = _ZN8dActor_c9TrackStarEjj(this, 0, 1);
    return 1;
}
#pragma opt_strength_reduction on

/* -------------------------------------------------------------------------- */
/* ROM ordinal 32 -- daYegg_c_classInit, 0x020ee3d8, size 0x54 */
/* -------------------------------------------------------------------------- */
// @symbol daYegg_c_classInit
/* recovered: vtable identified, globals resolved */
/* Reconstructed source-style name: SM64DS proves daYegg_c through RTTI,
 * allocation size, vtable identity, and the YOSHI_EGG registry profile;
 * later EAD lineage supplies classInit. Exact original spelling is not
 * preserved. Historical alias: YoshiEgg_Spawn.
 *
 * `new daYegg_c` is the whole sequence the loose factory spelled by hand:
 * fBase_c::operator new(0x42c), dEnemyBase_c's base constructor, the vptr
 * store, then the four member constructors in declaration order. */
extern "C" daYegg_c *daYegg_c_classInit(void)
{
    return new daYegg_c;
}
