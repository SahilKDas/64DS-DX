//cpp
/* daKpaFire_c: Bowser's Koopa-fire (KOOPAFIRE profile, ov060).
 *
 * The whole unit, ov060 .text 0x02116484..0x02117938, 26 functions: the
 * destructor pair, twenty helpers, CleanupResources, Render, Behavior and
 * InitResources. Eight variants from param1 & 7 (plus a sub-variant at
 * >> 4 & 3): gravity-falling flames with mesh and cylinder collision,
 * drawn by the particle system (Render only reports success). Behavior
 * runs the variant's handler from the data_ov060_0211afb4 PMF table;
 * InitResources runs its data_ov060_0211af74 ActorFn, then finishes setup.
 *
 * The tree used to call this class BowserFire (coined): the cartridge's
 * _ZTS11daKpaFire_c at ov060 0x0211a7c0, _ZTI at 0x0211a7b4 and _ZTV at
 * 0x0211a7f4 name it daKpaFire_c, and daKpaFire_c_classInit builds it
 * for the KOOPAFIRE registry profile.
 *
 * The out-of-line destructor is the key function, so this TU emits the
 * vtable and RTTI (manifest: deadstrip-data against the homed triple).
 * The file is ROM-descending: codegen is deferred, so .text comes out in
 * reverse source order. The destructor alone is bracketed by
 * `#pragma defer_codegen off/on`, so it is generated as it is parsed --
 * ahead of every deferred function -- and comes out D1, D0, then a D2 the
 * cartridge has no home for (manifest: deadstrip). Left deferred it comes
 * out D2, D0, D1 and the ROM's D1-before-D0 pair at 0x02116484 inverts.
 *
 * Known limits:
 * - The twenty func_ov060_* helpers stay free functions over raw offsets.
 *   Their bodies address this as bytes (no member arrows anywhere), and
 *   several are reached through the variant PMF/ActorFn tables rather than
 *   direct calls, so method conversion needs per-helper semantic review
 *   after the member-naming deslop -- verify alone cannot catch a
 *   wrong-this (bytes still match). Follow-up lane, same as the other
 *   promoted TUs' offset-soup leftovers.
 * - func_ov060_02117624 stays free AND parses as C: its Matrix4x3 block
 *   store scalarizes under C++ (same wall as Bullet 020fed7c and ov062
 *   ba84). `#pragma cplusplus off/on` around the definition only.
 * - func_ov060_02116740 and func_ov060_021172c8 stay free: decl_common.h
 *   declares them for other users.
 * - daKpaFire_c_classInit (0x02117938) abuts this run and stays a
 *   one-function C source (src/d_a_kpa_fire.c).
 * - dCcAc_c::Init, dBgCh_Actr::Init, DropShadowRadHeight, Particle::New
 *   and SaveData helpers stay mangled scalar externs (Fix12-by-value
 *   member form is the 6az wall).
 * - Behavior's `*(int *)((char *)this + 0x370) += 1` counter has no member
 *   yet (the header still calls 0x370 padding); mFrameCount at 0x374 is
 *   the named one.
 * - data_ov060_0211af74 / 0211afb4 / 0211934c / 02119358 / 02119364 and
 *   g_profile_KOOPAFIRE are not this TU's data.
 */

/* Includes: union of the legacy files', first-seen in ROM-ascending
 * processing order. NOT verified for header ordering constraints (e.g. a
 * common.h-before-X rule) -- watch for new compile errors after this. */
#include "daKpaFire_c.h"
#include "types.h"
#include "common.h"
#include "decl_common.h"
#include "dBgCh_Actr.h"
#include "decl_dBgCh_Actr.h"
#include "dBgCh_Gnd.h"
#include "dActor_c.h"

/* Local shadow declarations carried from the legacy files verbatim.
 * NOT reconciled against real project headers -- check include/*.h for
 * each of these before compiling; a real header should usually win. */
/* PMF-table views. Behavior calls data_ov060_0211afb4[mVariant].pmf and
 * InitResources calls data_ov060_0211af74[mVariant] as pointer-to-member
 * on this; the legacy shards proved swapping their empty stand-ins for
 * the real dActor_c byte-identical under the pin (same survivor shape as
 * Bullet's 020fed2c keeper). */
struct Vector3_16f;
struct dCc_c;
typedef void (dActor_c::*PMF)();
struct Entry { PMF pmf; };
typedef void (dActor_c::*ActorFn)();

#define ML(p) ((int*)(int)(p))
#define MLS(p) ((short*)(int)(p))

extern "C" {
extern u32 _ZN8Particle6System17NewUnkCallback818Ejj5Fix12IiES2_S2_PK11Vector3_16f( u32 a, u32 b, Fix12i c, Fix12i d, Fix12i e, const struct Vector3_16f* f);
extern u32 _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE( u32 a, u32 b, Fix12i c, Fix12i d, Fix12i e, const void* f, void* g);
extern void* _ZN8Particle6System12FromUniqueIDEj(u32 id);
extern void* _ZN8dActor_c13ClosestPlayerEv(void* self);
extern short Vec3_HorzAngle(const struct Vector3* a, const struct Vector3* b);
extern char* _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(int a, int b, const void *pos, const void *rot, int e, int f);
extern void _ZN7fBase_c18MarkForDestructionEv(void* a);
extern "C" void func_ov060_02116518(char *self, u32 kind, int a2, int a3);
extern Fix12i Vec3_HorzDist(const Vector3* a, const Vector3* b);
extern int RandomIntInternal(int* seed);
extern int data_0209e650[];
extern void _ZN8dActor_c9UpdatePosEP5dCc_c(char* self, void* c);
extern void func_ov060_0211712c(char *p);
extern int func_ov060_021172c8(unsigned char *p, unsigned int n);
extern int _ZNK10dBgCh_Actr10IsOnGroundEv(void *c);
extern int data_ov060_02119358[];
extern int data_ov060_0211934c[];
extern short data_02082214[];
extern void func_ov060_021172e0(void *self);
void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(u32 id, Fix12i x, Fix12i y, Fix12i z);
void* _ZN8dActor_c15FindWithActorIDEjPS_(u32 id, void* prev);
int _ZN8SaveData19IsCharacterUnlockedEj(u32 c);
extern char* _ZN8dActor_c10FindWithIDEj(unsigned int id);
extern int func_ov060_02111c68(char *c);
extern int _ZNK9Animation13GetFrameCountEv(void *anim);
extern u32 _ZN5Sound8PlayLongEjjjRK7Vector3s(u32 a, u32 b, u32 c, void *pos, u32 d);
extern u16 data_ov060_02119364[];
extern "C" Entry data_ov060_0211afb4[];
extern "C" void dBgCh_Actr_UpdateDiscreteNoLava_veneer(void *p);
extern int _ZN11ShadowModel12InitCylinderEv(void *self);
extern void _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(void *self, void *actor, int a, int b, unsigned int c, unsigned int d);
extern void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(void *self, void *actor, int a, int b, void *v, int c);
extern int _ZN9dBgCh_Gnd10DetectClsnEv(dBgCh_Gnd *self);
extern ActorFn data_ov060_0211af74[];
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 25 -- _ZN11daKpaFire_c13InitResourcesEv, 0x02117790, size 0x1a8 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaFire_c13InitResourcesEv
/* recovered: named members + shared header, real C++ method
 *
 * daKpaFire_c holds no file references of its own -- daKpa_c loads and frees the
 * whole fight -- so this sets up collision and state rather than resources.
 *
 * All three shadow declarations are gone:
 *   - `struct Vector3 { int x, y, z; }`   -> the real types.h Vector3.
 *   - `struct dBgCh_Gnd { ... }`      -> the real dBgCh_Gnd.h.
 *   - `struct dActor_c { }`                  -> the real dActor_c.h.
 * ...along with the magic offsets on `char *c`, now named daKpaFire_c members.
 *
 * The dBgCh_Gnd one is the interesting fix. The stand-in declared
 * `int floor[12]` at 0x14 and then read `floor[12]` -- one PAST its own bound,
 * so the index was a magic offset in disguise: 0x14 + 12*4 = 0x44. The real
 * header names that field `clsnY` and documents it as the search seed on entry
 * and the hit on exit, which is exactly how it is used here.
 *
 * The `dActor_c` one was NOT obviously safe and was measured rather than assumed.
 * It types the pointer-to-member dispatch through data_ov060_0211af74, and a
 * pointer to member of a POLYMORPHIC class need not share a representation
 * with one of an empty class. Under the pin it does: swapping the empty
 * stand-in for the real dActor_c is byte-identical.
 *
 * The `|= 1` at 0x2e8 was briefly named as a daKpaFire_c field of its own. It
 * is not one. 0x2d0 + 0x18 lands inside mdCcAc_c, and
 * dCc_c::flags is at 0x18, documented as "bit 0 makes Update bail" --
 * which is precisely what setting bit 0 does, and precisely what this branch
 * wants when mVariant is zero. Same mistake, and same correction, as Player's
 * `mBodyClsnFlags`.
 *
 * The doubled write to pos.y is the ROM's own shape and is kept verbatim: the
 * seed is read into a local, stored, then overwritten with seed + 0x32000.
 */
int daKpaFire_c::InitResources()
{
    Vector3 pos;

    if (_ZN11ShadowModel12InitCylinderEv(&this->mShadowModel) == 0)
        return 0;

    _ZN7dCcAc_c4InitEP8dActor_c5Fix12IiES3_jj(
        &this->mdCcAc_c, this, 0x28000, 0x50000, 0x200002, 0);
    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
        &this->mWithMeshClsn, this, 0x32000, 0x32000, 0, 0);

    this->mVertAccel = -0x4000;
    this->mTerminalVelocity = -0x1e000;
    this->mVariant = this->param1 & 7;
    this->mFrameCount = 0;
    if (this->mVariant == 0)
        this->mDropsShadow = 0;
    else
        this->mDropsShadow = 1;
    this->mTimer = 0;
    this->mVariant_378 = ((unsigned int)this->param1 >> 4) & 3;
    if (this->mVariant == 0)
        this->mdCcAc_c.flags |= 1;
    this->mShadowRadiusScale = 0x2000;
    this->mParticleHandle_380 = 0;
    this->mParticleHandle_37c = this->mParticleHandle_380;
    this->mUniqueID_2cc = 0;

    /* constructed here (not at function top: the ROM constructs after the
       collider setup), destroyed at the single exit below -- both synthesized */
    dBgCh_Gnd rc;
    {
        int p60;
        pos.x = this->mPosX;
        p60 = this->mPosY;
        pos.y = p60;
        pos.z = this->mPosZ;
        pos.y = p60 + 0x32000;
    }
    rc.SetObjAndPos(pos, 0);
    if (_ZN9dBgCh_Gnd10DetectClsnEv(&rc))
        this->mGroundY = rc.clsnY;
    else
        this->mGroundY = this->mPosY;

    (((dActor_c *)this)->*data_ov060_0211af74[this->mVariant])();

    this->mSoundHandle = 0;
    this->mSoundID = 0;
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 24 -- _ZN11daKpaFire_c8BehaviorEv, 0x021176d4, size 0xbc */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaFire_c8BehaviorEv
/* recovered: named members + shared header, real C++ method, declarations from a shared header */
/* recovered: named members + shared header, real C++ method */
int daKpaFire_c::Behavior()
{
    dActor_c *self = (dActor_c*)((char *)this);
    *(int*)(((int)((char *)this) + 0x370)) += 1;
    (self->*data_ov060_0211afb4[mVariant].pmf)();
    mFrameCount += 1;
    if (mVertAccel != 0) {
        dBgCh_Actr_UpdateDiscreteNoLava_veneer((char *)&mWithMeshClsn);
        if (mVariant != 4) {
            if (_ZNK10dBgCh_Actr10IsOnGroundEv((char *)&mWithMeshClsn) != 0) {
                mVertSpeed = 0;
                mVertAccel = 0;
            }
        }
    }
    func_ov060_02116740(((char *)this));
    func_ov060_02117624(((char *)this));
    mdCcAc_c.Clear();
    mdCcAc_c.Update();
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 23 -- _ZN11daKpaFire_c6RenderEv, 0x021176cc, size 0x8 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaFire_c6RenderEv
/* recovered: shared header, real C++ method
 *
 * `return 1` and nothing else -- the whole ROM body is `mov r0,#1; bx lr`.
 * The flame is drawn by the particle system, so the render slot only has to
 * report success.
 */
int daKpaFire_c::Render()
{
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 22 -- _ZN11daKpaFire_c16CleanupResourcesEv, 0x021176c4, size 0x8 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaFire_c16CleanupResourcesEv
/* recovered: shared header, real C++ method
 *
 * `return 1` with no releases, which is the finding rather than a stub:
 * daKpaFire_c holds no SharedFilePtr of its own. daKpa_c loads and frees the
 * whole fight's files -- 0x1c models, six more, and three singles -- and the
 * fire it breathes borrows from that set without taking a reference.
 */
int daKpaFire_c::CleanupResources()
{
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 21 -- func_ov060_02117624, 0x02117624, size 0xa0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02117624
/* C block store (same wall as Bullet 020fed7c and ov062 ba84): the Matrix4x3
 * copy scalarizes under C++. Parsed as C; the TU returns to C++ after it. */
/* recovered: shared common types */
#include "common.h"
extern "C" {

void Matrix4x3_FromTranslation(void *m, int x, int y, int z);
void _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(void *thisp, void *sm, void *mtx, int rad, int t, unsigned int j);
extern Matrix4x3 data_020a0e68;
}
#pragma cplusplus off
void func_ov060_02117624(char *c) {
    if (*(unsigned char*)(c+0x379) == 0) return;
    Matrix4x3_FromTranslation(&data_020a0e68, *(int*)(c+0x5c)>>3, *(int*)(c+0x364)>>3, *(int*)(c+0x64)>>3);
    *(Matrix4x3*)(c+0x32c) = data_020a0e68;
    _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(c, c+0x304, c+0x32c, *(int*)(c+0x368) * *(int*)(c+0x360), 0x1e000, 0xf);
}
#pragma cplusplus on

/* -------------------------------------------------------------------------- */
/* ROM ordinal 20 -- func_ov060_0211747c, 0x0211747c, size 0x1a8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_0211747c
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_0211747c(char *self)
{
    char *o;
    int n;
    int ang;
    int i;
    u16 *t;
    int s1;
    int s0;
    volatile int v[3];
    int a;
    int b;
    int *p;

    o = (char *)_ZN8dActor_c10FindWithIDEj(*(u32 *)(self + 0x2cc));
    if (o == 0) {
        _ZN7fBase_c18MarkForDestructionEv(self);
        return;
    }

    if (*(int *)(o + 0x40c) != 0xf)
        return;

    n = func_ov060_02111c68(o);
    if (n < 0)
        return;

    ang = *(u16 *)(o + 0x8e);
    i = (ang >> 4) * 2;
    p = (int *)(o + 0x5c);
    v[0] = p[0];
    v[1] = p[1];
    s1 = data_02082214[i + 1];
    s0 = data_02082214[i];
    v[2] = p[2];
    t = data_ov060_02119364;

    if (n == _ZNK9Animation13GetFrameCountEv(o + 0x124))
        n = 0;

    t += n * 5;

    if (*(u32 *)(self + 0x388) != 0x180)
        *(u32 *)(self + 0x384) = 0;
    *(u32 *)(self + 0x388) = 0x180;
    *(u32 *)(self + 0x384) = _ZN5Sound8PlayLongEjjjRK7Vector3s(
        *(u32 *)(self + 0x384), 3, *(u32 *)(self + 0x388), self + 0x74, 0);

    a = t[0];
    b = t[2] + 0x14;
    *(int *)(self + 0x5c) = v[0] + (b * s0 + a * s1);
    *(int *)(self + 0x60) = v[1] + ((t[1] - 0x5a) << 12);
    *(int *)(self + 0x64) = v[2] + (b * s1 - a * s0);
    *(short *)(self + 0x92) = (short)(t[4] + 0x818);
    *(short *)(self + 0x94) = (short)(t[3] + ang - 0x4700);

    if (n & 1)
        return;

    _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, 5, self + 0x5c, self + 0x92,
                                                 *(signed char *)(self + 0xcc), -1);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 19 -- func_ov060_021172e0, 0x021172e0, size 0x19c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021172e0
/* recovered: shared common types */
extern "C" void func_ov060_021172e0(void* self)
{
    char* sl = (char*)self;

    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(
        0x9f, *(Fix12i*)(sl + 0x5c), *(Fix12i*)(sl + 0x60), *(Fix12i*)(sl + 0x64));
    _ZN7fBase_c18MarkForDestructionEv(sl);

    if ((((u32)RandomIntInternal(&data_0209e650[0]) >> 0x10) % 10) >= 2) {
        return;
    }

    {
        void* p = _ZN8dActor_c13ClosestPlayerEv(sl);
        char* sb;
        if (p != 0 && *(int*)((char*)p + 8) == 3 &&
            (sb = (char*)_ZN8dActor_c15FindWithActorIDEjPS_(0x117, 0)) != 0 &&
            *(unsigned char*)(sb + 0x42b) == 0) {
            int c;
            int idx;
            int mask = 0;
            for (c = 0; c < 3; c++) {
                if (_ZN8SaveData19IsCharacterUnlockedEj(c) != 0) {
                    mask = (mask | (1 << c)) & 0xff;
                }
            }
            do {
                idx = ((u32)RandomIntInternal(&data_0209e650[0]) >> 0x10) % 3;
            } while ((mask & (1 << idx)) == 0);
            {
                void* r = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(
                    0x10d, (idx << 8) | 0xb, (struct Vector3*)(sl + 0x5c),
                    (struct Vector3_16*)(sl + 0x8c), *(s8*)(sl + 0xcc), -1);
                if (r != 0) {
                    *(unsigned char*)(sb + 0x42b) = 1;
                }
            }
            return;
        }
    }

    {
        void* r = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(
            0x120, 0, (struct Vector3*)(sl + 0x5c), 0, *(s8*)(sl + 0xcc), -1);
        if (r != 0) {
            *(int*)((char*)r + 0xa4) = 0;
            *(int*)((char*)r + 0xa8) = 0;
            *(int*)((char*)r + 0xac) = 0;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 18 -- func_ov060_021172c8, 0x021172c8, size 0x18 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021172c8
extern "C" {  /* .c-derived member: C linkage for the whole block */
int func_ov060_021172c8(unsigned char *p, unsigned int n){
  return *(unsigned short*)(p + 0x374) > n;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 17 -- func_ov060_0211722c, 0x0211722c, size 0x9c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_0211722c
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_0211722c(char *c)
{
    unsigned int r;
    *(short*)(c+0x94) = (short)((unsigned int)RandomIntInternal(&data_0209e650[0]) >> 0x10);
    r = (unsigned int)RandomIntInternal(&data_0209e650[0]) >> 0x10;
    if (r % 10 < 2)
        *(int*)(c+0xa8) = 0x1e000;
    else
        *(int*)(c+0xa8) = 0xa000;
    *(int*)(c+0x98) = 0x5000;
    *(int*)(c+0x9c) = -0x1000;
    *(int*)(c+0x360) = (int)(((unsigned int)RandomIntInternal(&data_0209e650[0]) >> 16) & 0xfff) + 0x1000;
    *(int*)(c+0x368) = 0x10;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 16 -- func_ov060_021171e8, 0x021171e8, size 0x44 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021171e8
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021171e8(char *c)
{
    unsigned int r = RandomIntInternal(data_0209e650);
    *(short *)(c + 0x94) = r >> 0x10;
    *(int *)(c + 0xa8) = 0xa000;
    *(int *)(c + 0x98) = 0;
    *(int *)(c + 0x360) = 0x6000;
    *(int *)(c + 0x368) = 0x10;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 15 -- func_ov060_0211712c, 0x0211712c, size 0xbc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_0211712c
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_0211712c(char *p)
{
    short ang = (short)(((*(int *)(p + 0x370) + *(unsigned short *)(p + 0x376)) & 0x3f) << 10);
    int bi = (unsigned short)ang >> 4;
    int *px = (int *)(p + 0x5c);
    int *pz;
    *px = *px + ((data_02082214[((int)*(unsigned short *)(p + 0x94) >> 4) * 2]
                  * data_02082214[bi * 2]) << 2) / 0x1000;
    pz = (int *)(p + 0x64);
    *pz = *pz + ((data_02082214[((int)*(unsigned short *)(p + 0x94) >> 4) * 2 + 1]
                  * data_02082214[bi * 2 + 1]) << 2) / 0x1000;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 14 -- func_ov060_02116f90, 0x02116f90, size 0x19c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02116f90
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02116f90(char *self) {
    _ZN8dActor_c9UpdatePosEP5dCc_c(self, 0);
    if (*(int *)(self + 0xa8) < -0x4000)
        *(int *)(self + 0xa8) = -0x4000;

    if (*(int *)(self + 0x36c) == 0) {
        *(int *)(self + 0x37c) = _ZN8Particle6System17NewUnkCallback818Ejj5Fix12IiES2_S2_PK11Vector3_16f(
            *(int *)(self + 0x37c), 0x9e, *(int *)(self + 0x5c), *(int *)(self + 0x60) + 0x37000, *(int *)(self + 0x64), 0);
        *(int *)(((int)self + 0x2e8)) |= 1;
        func_ov060_0211712c(self);
        if (_ZNK10dBgCh_Actr10IsOnGroundEv(self + 0x110)) {
            (*(int *)(((int)self + 0x36c)))++;
            if (*(int *)(self + 0x35c) == 7) {
                *(int *)(self + 0x360) = 0x6000;
            } else {
                *(int *)(self + 0x360) = ((u32)RandomIntInternal(&data_0209e650[0]) >> 16 & 0xfff) * 2 + 0x6000;
            }
            *(int *)(self + 0x98) = 0;
            *(int *)(self + 0xa8) = 0;
            *(int *)(self + 0x9c) = 0;
            *(int *)(self + 0x380) = 0;
            *(int *)(self + 0x37c) = *(int *)(self + 0x380);
        }
    } else {
        func_ov060_02116518(self, 0x9c, 1, *(int *)(self + 0x360) * 0xc);
        *(int *)(((int)self + 0x2e8)) &= ~1;
        if (*(u16 *)(self + 0x374) > *(int *)(self + 0x360) * 0xa / 0x1000 + 5) {
            *(int *)(((int)self + 0x360)) -= 0x266;
            if (*(int *)(self + 0x360) <= 0)
                func_ov060_021172e0(self);
        }
    }
    if (*(int *)(self + 0x60) < 0)
        _ZN7fBase_c18MarkForDestructionEv(self);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 13 -- func_ov060_02116f74, 0x02116f74, size 0x1c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02116f74
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02116f74(char *p)
{
    *(int *)(p + 0x98) = 30;
    *(int *)(p + 0x360) = 8192;
    *(int *)(p + 0x368) = 16;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 12 -- func_ov060_02116d78, 0x02116d78, size 0x1fc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02116d78
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02116d78(char *c)
{

    if (*(int*)(c+0x360) < 0x5000) *ML(c+0x360) += 0x255;
    if (*(short*)(c+0x92) > 0x800) *MLS(c+0x92) -= 0x200;

    {
        int i0 = (*(unsigned short*)(c+0x92)) >> 4;
        int i1 = (*(unsigned short*)(c+0x94)) >> 4;
        i0 = i0 * 2;
        i1 = i1 * 2 + 1;
        int s0 = data_02082214[i0];
        int speed = *(int*)(c+0x98);
        int s1 = data_02082214[i1];
        *(int*)(c+0xa4) = (s1 * (speed * s0)) / 896;
    }
    {
        int speed = *(int*)(c+0x98);
        int s0 = data_02082214[((*(unsigned short*)(c+0x92))>>4)*2];
        *(int*)(c+0xa8) = (-speed) * s0;
    }
    {
        int s0 = data_02082214[((*(unsigned short*)(c+0x92))>>4)*2];
        int s1 = data_02082214[((*(unsigned short*)(c+0x94))>>4)*2];
        int speed = *(int*)(c+0x98);
        *(int*)(c+0xac) = (s1 * ((-speed) * s0)) / 896;
    }
    *(int*)(c+0x5c) += *(int*)(c+0xa4);
    *(int*)(c+0x60) += *(int*)(c+0xa8);
    *(int*)(c+0x64) += *(int*)(c+0xac);

    func_ov060_02116518(c, 0x9a, 1, *(int*)(c+0x360) * 0xc);
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c+0x110) != 0) {
        int r = RandomIntInternal(&data_0209e650[0]);
        unsigned int r3 = (unsigned int)r >> 16;
        if (r3 % 17 != 0)
            _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, 6, (Vector3*)(c+0x5c), 0, *(signed char*)(c+0xcc), -1);
        _ZN7fBase_c18MarkForDestructionEv(c);
    }
    if (*(unsigned short*)(c+0x374) < 0x3c) return;
    _ZN7fBase_c18MarkForDestructionEv(c);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 11 -- func_ov060_02116c68, 0x02116c68, size 0x110 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02116c68
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02116c68(char *c)
{
  unsigned int r;
  *(short *)(c + 0x94) = (short)(((unsigned int)RandomIntInternal(&data_0209e650[0])) >> 0x10);
  if (*(unsigned char *)(c + 0x378) != 0)
  {
    r = ((unsigned int)RandomIntInternal(&data_0209e650[0])) >> 0x10;
    *(int *)(c + 0x98) = (int)((r % 10) << 0xc) >> 1;
  }
  else
  {
    r = ((unsigned int)RandomIntInternal(&data_0209e650[0])) >> 0x10;
    *(int *)(c + 0x98) = (int)((r % 10) * 0x7000) >> 1;
  }

  r = ((unsigned int)RandomIntInternal(&data_0209e650[0])) >> 0x10;
  *(int *)(c + 0xa8) = (r % 10) << 0xc;
  *(int *)(c + 0x9c) = -0x1000;

  *(short *)(c + 0x376) = (short)(((unsigned int)RandomIntInternal(&data_0209e650[0])) >> 0x10);
  *(int *)(c + 0xa0) = data_ov060_0211934c[*(unsigned char *)(c + 0x378)];
  *(int *)(c + 0x360) = 0x2000;
  *(int *)(c + 0x368) = 0x10;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 10 -- func_ov060_02116b68, 0x02116b68, size 0x100 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02116b68
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02116b68(char *c)
{
    func_ov060_02116518(c, data_ov060_02119358[*(unsigned char*)(c + 0x378)], 0, 0x46000);
    func_ov060_0211712c(c);
    if (func_ov060_021172c8((unsigned char *)c, 0x1c2))
        _ZN7fBase_c18MarkForDestructionEv(c);
    _ZN8dActor_c9UpdatePosEP5dCc_c(c, 0);
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x110)) {
        if (*(unsigned char*)(c + 0x378) == 0)
            _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, 7, c + 0x5c, 0, *(signed char*)(c + 0xcc), -1);
        else
            _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, 3, c + 0x5c, 0, *(signed char*)(c + 0xcc), -1);
        _ZN7fBase_c18MarkForDestructionEv(c);
    } else {
        if (*(int*)(c + 0x60) >= 0) return;
        _ZN7fBase_c18MarkForDestructionEv(c);
    }
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 9 -- func_ov060_02116b18, 0x02116b18, size 0x50 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02116b18
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02116b18(char* c){
  *(int*)(c+0xa8) = 0x7000;
  *(int*)(c+0x98) = 0x11800;
  *(int*)(c+0x360) = 0x2000;
  *(int*)(c+0x9c) = 0x1000;
  *(unsigned short*)(c+0x376) = (unsigned int)RandomIntInternal(data_0209e650) >> 0x10;
  *(int*)(c+0x368) = 0x10;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 8 -- func_ov060_021169f8, 0x021169f8, size 0x120 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021169f8
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021169f8(char* sl)
{
    int i;
    char* a;
    if (*(int*)(sl+0x360) < 0x5000) {
        *(int*)(((int)sl + 0x360)) += 0x200;
    }
    _ZN8dActor_c9UpdatePosEP5dCc_c(sl, 0);
    func_ov060_02116518(sl, 0x9a, 1, 0x78000);
    if (*(unsigned short*)(sl+0x374) <= 0x14)
        return;
    if (*(unsigned char*)(sl+0x378) == 0) {
        for (i = 0; i < 3; i++) {
            a = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, 2, (struct Vector3*)(sl+0x5c), (struct Vector3_16*)(sl+0x92), *(signed char*)(sl+0xcc), -1);
            *(int*)(a+0x360) = 0x5000;
        }
    } else {
        a = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, 0x12, (struct Vector3*)(sl+0x5c), (struct Vector3_16*)(sl+0x92), *(signed char*)(sl+0xcc), -1);
        *(int*)(a+0x360) = 0x8000;
        a = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, 0x22, (struct Vector3*)(sl+0x5c), (struct Vector3_16*)(sl+0x92), *(signed char*)(sl+0xcc), -1);
        *(int*)(a+0x360) = 0x8000;
    }
    _ZN7fBase_c18MarkForDestructionEv(sl);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 7 -- func_ov060_021169b0, 0x021169b0, size 0x48 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021169b0
// recovered name: Bowser_Kill
/* recovered: renamed to Class_Method, declarations from a shared header */
/* recovered: renamed to Class_Method */
/* daKpa_c::Kill - recovered from vtable slot identity */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021169b0(char* thiz) {
    *(int*)(thiz + 0xa8) = 0x1e000;
    *(int*)(thiz + 0x98) = 0xf000;
    *(short*)(thiz + 0x376) = (unsigned int)RandomIntInternal(&data_0209e650[0]) >> 16;
    _ZN10dBgCh_Actr13SetLimMovFlagEv(thiz + 0x110);
    *(int*)(thiz + 0x368) = 0x10;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 6 -- func_ov060_021168c4, 0x021168c4, size 0xec */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021168c4
/* recovered: shared common types, declarations from a shared header */
/* recovered: shared common types */
extern "C" void func_ov060_021168c4(char* c)
{
    char* r4;
    if (*(unsigned short*)(c + 0x374) == 0) {
        dActor_c* a = dActor_c::FindWithActorID(0x117, 0);
        r4 = (char*)a;
        *(int*)(c + 0x2cc) = *(int*)(r4 + 4);
    } else {
        r4 = (char*)dActor_c::FindWithID(*(unsigned int*)(c + 0x2cc));
    }
    if (((dBgCh_Actr*)(c + 0x110))->JustHitGround() != 0) {
        *(int*)(c + 0xa8) = 0x1e000;
    }
    ((dActor_c*)c)->UpdatePos((dCc_c*)0);
    func_ov060_02116518(c, 0xa6, 0, 0x32000);
    if (func_ov060_021172c8((unsigned char*)c, 0x96) != 0) {
        ((fBase_c*)c)->MarkForDestruction();
    }
    if (r4 == 0) return;
    if (*(int*)(r4 + 0x410) != 0) return;
    if (Vec3_HorzDist((Vector3*)(c + 0x5c), (Vector3*)(r4 + 0x5c)) >= 0x96000) return;
    ((fBase_c*)c)->MarkForDestruction();
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 5 -- func_ov060_021167ec, 0x021167ec, size 0xd8 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021167ec
/* recovered: shared common types */
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021167ec(char* c)
{
    char* p;
    if (*(unsigned short*)(c + 0x374) != 0) return;
    p = (char*)_ZN8dActor_c13ClosestPlayerEv(c);
    if (p == 0) return;
    *(short*)(c + 0x94) = Vec3_HorzAngle((struct Vector3*)(c + 0x5c), (struct Vector3*)(p + 0x5c));
    *(int*)(c + 0x360) = 0x5000;
    {
        int i = 0;
        int ang = 0;
        do {
            struct dActor_c* a = (struct dActor_c*)_ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0x118, 4, (struct Vector3*)(c + 0x5c), (const struct Vector3_16*)0, *(signed char*)(c + 0xcc), -1);
            short v = *(short*)(c + 0x94) + ang;
            a->mPrevAngleX = 0;
            a->mPrevAngleY = v;
            a->mPrevAngleZ = 0;
            *(int*)((char*)a + 0x360) = *(int*)(c + 0x360);
            i++;
            ang += 0x5555;
        } while (i < 3);
    }
    _ZN7fBase_c18MarkForDestructionEv(c);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 4 -- func_ov060_021167c8, 0x021167c8, size 0x24 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_021167c8
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_021167c8(char *c)
{
    *(int *)(((int)c + 0x2e8)) |= 1;
    *(int *)(c + 0x360) = 0x2000;
    *(int *)(c + 0x368) = 0x10;
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 3 -- func_ov060_02116740, 0x02116740, size 0x88 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02116740
extern "C" {
extern void _ZN6Player4BurnEv(void* p);
void func_ov060_02116740(char* c){
  unsigned int id = *(unsigned int*)(c+0x2f4);
  if(id==0) return;
  char* p = _ZN8dActor_c10FindWithIDEj(id);
  if(p==0) return;
  int b = (int)(*(unsigned short*)(p+0xc) == 0xbf);
  if(b==0) return;
  if(*(unsigned char*)(p+0x6f9)!=0) return;
  if(*(unsigned char*)(p+0x6fb)!=0) return;
  _ZN6Player4BurnEv(p);
}
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 2 -- func_ov060_02116518, 0x02116518, size 0x228 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov060_02116518
extern "C" {  /* .c-derived member: C linkage for the whole block */
void func_ov060_02116518(char* self, u32 kind, int a2, int a3)
{
    void* o;

    if (kind == 0xa2 || kind == 0xa4) {
        *(u32*)(self + 0x37c) = _ZN8Particle6System17NewUnkCallback818Ejj5Fix12IiES2_S2_PK11Vector3_16f(
            *(u32*)(self + 0x37c), kind, *(int*)(self + 0x5c), *(int*)(self + 0x60) + a3, *(int*)(self + 0x64), 0);
    } else {
        *(u32*)(self + 0x37c) = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
            *(u32*)(self + 0x37c), kind, *(int*)(self + 0x5c), *(int*)(self + 0x60) + a3, *(int*)(self + 0x64), 0, 0);
    }

    if (*(u32*)(self + 0x37c) != 0 && a2 != 0) {
        o = _ZN8Particle6System12FromUniqueIDEj(*(u32*)(self + 0x37c));
        if (*(int*)(self + 0x360) >= 0x8000) { *(int*)((char*)o + 0x50) = 0x7fff; } else { *(int*)((char*)o + 0x50) = (short)*(int*)(self + 0x360); }

        if (kind == 0x9a) {
            *(int*)((char*)o + 0x44) = (int)(((long long)(*(int*)(self + 0x360)) * 0x2800 + 0x800) >> 12);
            *(int*)((char*)o + 0x4c) = (short)(Fix12i)(((long long)(*(int*)(self + 0x360)) * 0xa66 + 0x800) >> 12);
        }
    }

    *(u32*)(self + 0x380) = _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
        *(u32*)(self + 0x380), kind + 1, *(int*)(self + 0x5c), *(int*)(self + 0x60) + a3, *(int*)(self + 0x64), 0, 0);
    if (*(u32*)(self + 0x380) == 0)
        return;
    if (a2 == 0)
        return;

    o = _ZN8Particle6System12FromUniqueIDEj(*(u32*)(self + 0x380));
    if (*(int*)(self + 0x360) >= 0x8000) { *(int*)((char*)o + 0x50) = 0x7fff; } else { *(int*)((char*)o + 0x50) = (short)*(int*)(self + 0x360); }

    if (kind != 0x9a)
        return;

    *(int*)((char*)o + 0x44) = (int)(((long long)(*(int*)(self + 0x360)) * 0x2800 + 0x800) >> 12);
    *(int*)((char*)o + 0x4c) = (short)(Fix12i)(((long long)(*(int*)(self + 0x360)) * 0xa66 + 0x800) >> 12);
}
}

/* -------------------------------------------------------------------------- */
/* D1 then D0 from one out-of-line definition; the homeless D2 is deadstripped. */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daKpaFire_cD1Ev
// @symbol _ZN11daKpaFire_cD0Ev
/* The compiler writes both bodies: store this vtable, destroy the members in
 * reverse declaration order, chain to dEnemyBase_c, then (D0) the inline
 * operator delete. */
#pragma defer_codegen off
daKpaFire_c::~daKpaFire_c()
{
}
#pragma defer_codegen on
