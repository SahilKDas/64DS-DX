//cpp
/* Production translation unit for ov098/daObjBlockS_c.
 * 37 functions, .text 0x02138040..0x02139f70. The small breakable crate
 * (registry profile BLOCK_S).
 *
 * NAME: _ZTS13daObjBlockS_c is "13daObjBlockS_c" at ov098 0x0213c500; _ZTI at
 * 0x0213c4d4 reads [__si_class_type_info, that string, _ZTI10dBgActor_c], and
 * the word before the _ZTV13daObjBlockS_c address point (0x0213c534) points at
 * that _ZTI. The tree previously called the class Crate (coined).
 *
 * The unit runs from the destructor pair (D1 0x02138040, D0 0x021380bc) to
 * daObjBlockS_c_classInit (0x02139f04). The out-of-line destructor is the key
 * function, so this TU emits the vtable and the RTTI chain; D2 has no ROM home.
 *
 * Crate_SetState and func_ov098_02138b70 run the state table at
 * data_ov098_0213c878: one {enter, update} member-pointer pair per state, and
 * mState (0x560) is the index. The state functions between them
 * (0x0213814c..0x02138b18) are reached only through that table and keep their
 * C names on a char pointer, as do the helpers after them; decl_common.h
 * declares three of them that way.
 *
 * Under `#pragma defer_codegen off` .text is laid down in source order, so
 * this file is ROM-ascending.
 */
#include "decl_Actor.h"
#include "decl_common.h"
#include "daObjBlockS_c.h"
#include "dActor_c.h"
#include "Player.h"
#include "SharedFilePtr.h"
#include "dBgW.h"
#include "dBgCh_Actr.h"
#include "dBgCh_Gnd.h"
#include "SurfaceInfo.h"

struct Vector3_16f;

extern "C" {
extern u32 data_0209b454;
extern int data_ov098_0213c4c8[];
extern s16 data_02082214[];
extern int Vec3_HorzDist(const struct Vector3 *a, const struct Vector3 *b);
extern u8 DecIfAbove0_Byte(u8 *p);
extern void *_ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
    u32 a, u32 b, int c, int d, int e, const void *v, void *cb);
extern u32 _ZN8Particle6System17NewUnkCallback818Ejj5Fix12IiES2_S2_PK11Vector3_16f(
    u32 a, u32 b, int c, int d, int e, const Vector3_16f *v);
extern void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int id, int x, int y, int z);
extern void _ZN8Particle20RunningSlidingDustAtE5Fix12IiES1_S1_(s32 x, s32 y, s32 z);
extern void _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
    char *thiz, int f, char *m, int fix, short s, int blk);
extern void _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
    char *thiz, char *actor, int b, int d, void *v, int f);
extern int _ZN10dBgW_KcMbg9TransformERK9Matrix4x3s(void *thiz, Matrix4x3 &m, short ang);

extern int _ZN8dActor_c13DistToCPlayerEv(void *self);
extern void _ZN8dActor_c9UpdatePosEP5dCc_c(void *self, void *clsn);
extern void *_ZN8dActor_c10FindWithIDEj(u32 id);
extern void _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(void *self, void *pos, unsigned int n, int speed, short ang);
extern short _ZN8dActor_c12ReflectAngleE5Fix12IiES1_s(void *self, int a, int b, short c);
extern void _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(
    void *self, void *shadow, void *mtx, int fix, int t1, int t2, unsigned int n);
extern char *_ZN8dActor_c11UpdateCarryER6PlayerRK7Vector3(char *self, char *player, const struct Vector3 *v);
extern int _ZN10dBgActor_c21IsClsnInRangeOnScreenE5Fix12IiES1_(void *self, int a, int b);
extern void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
    void *thiz, void *actor, void *vec, int fix, int t, unsigned u1, unsigned u2);
extern void _ZN5dCc_c5ClearEv(void *cc);
extern void _ZN5dCc_c6UpdateEv(void *cc);

extern void dBgCh_Actr_UpdateContinuous_Veneer(void *p);
extern int _ZNK10dBgCh_Actr10IsOnGroundEv(void *p);
extern void *_ZNK10dBgCh_Actr14GetFloorResultEv(void *p);
extern int SurfaceInfo_TestFlag0x20(int *p);
extern int func_0203567c(int p);
extern int func_02037e38(u32 *p);
extern int func_02037e58(u32 *p);
extern void _ZNK11SurfaceInfo12CopyNormalToER7Vector3(void *self, Vector3 *out);
extern int func_ov002_020ef228(void *c, int arg);
extern int func_ov002_020f030c(int x);
extern int func_ov002_020e496c(char *c);
extern s16 func_02010844(void *unused, Vector3 *v, s16 angle);
extern int _ZNK5dBgPi9GetClsnIDEv(int self);
extern dBgPi *_ZNK10dBgCh_Actr13GetWallResultEv(const dBgCh_Actr *self);

extern void _ZN6Player9DropActorEv(void *p);
extern int _ZN6Player7TryGrabER8dActor_c(void *player, void *actor);
extern void _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(void *p, struct Vector3 *pos, u32 a, int b, u32 d, u32 e, u32 f);
extern int _ZN6Player14IsFrontSlidingEv(char *p);
extern int _ZN6Player17LostGrabbedObjectEv(char *p);

extern void _ZN5Sound9PlayBank3EjRK7Vector3(u32 id, const struct Vector3 *pos);
extern u32 _ZN5Sound8PlayLongEjjjRK7Vector3s(u32 a, u32 b, u32 c, const Vector3 *pos, u32 e);

extern int _Z14ApproachLinearRiii(int *a, int b, int c);
extern void _Z11UpdateAngleRssis(s16 *a, s16 b, int c, s16 d);
extern s16 _ZN4cstd5atan2E5Fix12IiES1_(s32 y, s32 x);
extern int _ZN4cstd4fdivEii(int a, int b);
extern s32 Vec3_HorzLen(const Vector3 *v0);
extern void Vec3_MulScalarInPlace(int *v, int s);
extern void Vec3_Add(Vector3 *out, Vector3 *a, Vector3 *b);
extern void Matrix4x3_FromRotationY(void *m, int angle);
extern void Math_Function_0203b14c(char *dst, int a, int b, int c, int d);

void Crate_SetState(char *c, int i);
void func_ov098_02138b70(char *c);
int func_ov098_02138bb8(char *c);
int func_ov098_02138bfc(char *c);
void func_ov098_02138ce0(char *c);
void func_ov098_02138e08(char *c);
void func_ov098_02138e6c(char *c);
void func_ov098_021390ec(char *c);
int func_ov098_02139228(char *c);
void func_ov098_021396a4(char *c);
void func_ov098_021397c8(char *c);
void func_ov098_02139850(char *c);
}

enum Bool { FALSE, TRUE };

struct Sub {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void m5(int);
};

/* The state table: data_ov098_0213c878 holds one {enter, update} pair of
   member pointers per state, indexed by the word at 0x560. */
struct BlockSStateHost;
typedef void (BlockSStateHost::*BlockSStateFn)();
struct BlockSStateEntry { BlockSStateFn fn[2]; };
extern BlockSStateEntry data_ov098_0213c878[];
struct BlockSStateHost { char pad[0x560]; int idx; };

struct BlockSW4 { u32 w[4]; };
extern "C" BlockSW4 data_ov098_0213c4e0;
extern "C" BlockSW4 data_ov098_0213c4f0;

#pragma defer_codegen off

// @symbol _ZN13daObjBlockS_cD1Ev
// @symbol _ZN13daObjBlockS_cD0Ev
/* Destroys dCcAcPos_c x2, ShadowModel and dBgCh_Actr in reverse declaration
 * order, then dBgActor_c's inline destructor stores its own vptr and destroys
 * its dBgW_KcMbg and Model before chaining to dActor_c. D0 then returns the
 * object through the inline operator delete. */
daObjBlockS_c::~daObjBlockS_c()
{
}

// @symbol func_ov098_0213814c
extern "C" void func_ov098_0213814c(char *c)
{
    func_ov098_02138ce0(c);
    unsigned b = (unsigned)((*(int *)(c + 0xb0) & 8) != 0);
    if (b != 0 && _ZN8dActor_c13DistToCPlayerEv(c) > 0x7d0000) {
        Crate_SetState(c, 0);
        return;
    }
    _ZN5dCc_c5ClearEv(c + 0x564);
    _ZN5dCc_c5ClearEv(c + 0x5a4);
    func_ov098_02139850(c);
    func_ov098_021397c8(c);
    if (((dBgW *)(c + 0x124))->IsEnabled())
        ((dBgW *)(c + 0x124))->Disable();
}

// @symbol func_ov098_021381e8
extern "C" void func_ov098_021381e8(char *c)
{
    if (((dBgW *)(c + 0x124))->IsEnabled())
        ((dBgW *)(c + 0x124))->Disable();
    func_ov098_02138ce0(c);
    func_ov098_02139850(c);
    func_ov098_021397c8(c);
    (*(int *)(((int)c + 0xb0))) &= ~0xe0000;
}

// @symbol func_ov098_02138238
extern "C" void func_ov098_02138238(char *c)
{
    unsigned int flags = *(unsigned int *)(c + 0xb0);
    int t1;
    t1 = flags & 0x80000;
    t1 = t1 != 0;

    if (t1 != 0) {
        Crate_SetState(c, 3);
    } else {
        int t2;
        t2 = flags & 0x20000;
        t2 = t2 != 0;
        if (t2 == 0) {
            int t3;
            t3 = flags & 0x40000;
            t3 = t3 != 0;
            if (t3 == 0) {
                Crate_SetState(c, 4);
            }
        }
    }

    {
        unsigned int flags2 = *(unsigned int *)(c + 0xb0);
        int t4;
        t4 = flags2 & 0x20000;
        t4 = t4 != 0;
        if (t4 == 0) {
            void *p = *(void **)(c + 0xd0);
            if (p != 0) {
                int *src = (int *)(int)((char *)p + 0x5c);
                *(int *)(c + 0x5c) = src[0];
                *(int *)(c + 0x60) = src[1];
                *(int *)(c + 0x64) = src[2];
            }
        }
    }

    _ZN5dCc_c5ClearEv(c + 0x564);
    func_ov098_02139850(c);
    if (!((dBgW *)(c + 0x124))->IsEnabled()) return;
    ((dBgW *)(c + 0x124))->Disable();
}

// @symbol func_ov098_02138318
extern "C" void func_ov098_02138318(char *c)
{
    *(int *)(c + 0x5f0) = 0;
    *(int *)(c + 0x5f4) = 0;
    *(int *)(c + 0x98) = 0;
    *(int *)(((int)c + 0x57c)) &= ~0x2000;
}

// @symbol func_ov098_02138344
extern "C" void func_ov098_02138344(char *c)
{
    _ZN8dActor_c9UpdatePosEP5dCc_c(c, c + 0x564);
    dBgCh_Actr_UpdateContinuous_Veneer(c + 0x320);
    _Z14ApproachLinearRiii((int *)(c + 0x98), 0, 0x555);
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x320) != 0) {
        _ZN5Sound9PlayBank3EjRK7Vector3(0x51, (struct Vector3 *)(c + 0x74));
        DecIfAbove0_Byte((unsigned char *)(c + 0x604));
        *(int *)(c + 0xa8) = *(unsigned char *)(c + 0x604) * 0xa000;
        *(int *)(c + 0x98) = *(unsigned char *)(c + 0x604) * 0x5000;
    }
    if (*(int *)(c + 0x98) == 0 && *(unsigned char *)(c + 0x604) == 0) {
        Crate_SetState(c, 0);
    }
    func_ov098_02139228(c);
    func_ov098_02138e6c(c);
    func_ov098_021390ec(c);
    if (func_ov098_02138bb8(c) != 0) {
        ((daObjBlockS_c *)c)->Kill();
    }
    _ZN5dCc_c5ClearEv(c + 0x564);
    _ZN5dCc_c6UpdateEv(c + 0x564);
    func_ov098_02139850(c);
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x320) == 0) {
        func_ov098_021396a4(c);
    }
    if (_ZN10dBgActor_c21IsClsnInRangeOnScreenE5Fix12IiES1_(c, 0, 0) != 0) {
        func_ov098_021397c8(c);
    }
}

// @symbol func_ov098_02138484
extern "C" void func_ov098_02138484(char *c)
{
    *(int *)(c + 0x5f0) = 0;
    *(int *)(c + 0x5f4) = 0;
    *(int *)(((int)c + 0xb0)) &= ~0x80000;
    *(unsigned char *)(c + 0x604) = 3;
    *(int *)(c + 0xa8) = *(unsigned char *)(c + 0x604) * 0xa000;
    *(int *)(c + 0x98) = *(unsigned char *)(c + 0x604) * 0x5000;
    *(int *)(c + 0xd0) = 0;
    *(int *)(c + 0x5e4) = 0;
    *(int *)(((int)c + 0x57c)) &= ~0x2000;
}

// @symbol func_ov098_021384fc
extern "C" void func_ov098_021384fc(char *c)
{
    _ZN8dActor_c9UpdatePosEP5dCc_c(c, c + 0x564);
    dBgCh_Actr_UpdateContinuous_Veneer(c + 0x320);
    if (func_ov098_02139228(c)) {
        Crate_SetState(c, 0);
        return;
    }
    _Z14ApproachLinearRiii((int *)(c + 0x98), 0, 0x555);
    func_ov098_02138e6c(c);
    func_ov098_021390ec(c);
    if (func_ov098_02138bb8(c)) {
        ((daObjBlockS_c *)c)->Kill();
    }
    _ZN5dCc_c5ClearEv(c + 0x564);
    _ZN5dCc_c6UpdateEv(c + 0x564);
    func_ov098_02139850(c);
    if (!_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x320)) {
        func_ov098_021396a4(c);
    }
    if (((dBgW *)(c + 0x124))->IsEnabled()) {
        ((dBgW *)(c + 0x124))->Disable();
    }
}

// @symbol func_ov098_021385e0
extern "C" void func_ov098_021385e0(char *self)
{
    char *obj;
    short angle;
    int idx;
    int cosv, sinv;

    *(int *)(self + 0x5f0) = 0;
    *(int *)(self + 0x5f4) = 0;

    obj = *(char **)(self + 0xd0);
    angle = *(short *)(obj + 0x8e);
    *(short *)(self + 0x8e) = angle;
    *(short *)(self + 0x94) = *(short *)(self + 0x8e);

    obj = *(char **)(self + 0xd0);
    {
        int *osrc = (int *)(obj + 0x5c);
        *(int *)(self + 0x5c) = osrc[0];
        *(int *)(self + 0x60) = osrc[1];
        *(int *)(self + 0x64) = osrc[2];
    }

    idx = (unsigned short)*(short *)(self + 0x8e) >> 4;
    cosv = data_02082214[idx * 2];
    *(int *)(self + 0x5c) = *(int *)(self + 0x5c) + (int)(((long long)cosv * 0x50000 + 0x800) >> 12);

    *(int *)(self + 0x60) = *(int *)(self + 0x60) + 0x50000;

    idx = (unsigned short)*(short *)(self + 0x8e) >> 4;
    sinv = data_02082214[idx * 2 + 1];
    *(int *)(self + 0x64) = *(int *)(self + 0x64) + (int)(((long long)sinv * 0x50000 + 0x800) >> 12);

    *(int *)(self + 0xb0) &= ~0x80000;

    *(int *)(self + 0x98) = 0x1e000;
    *(int *)(self + 0xa8) = 0xf000;
    *(char **)(self + 0x5e8) = *(char **)(self + 0xd0);
    *(char **)(self + 0xd0) = 0;
    *(int *)(self + 0x57c) |= 0x2000;
}

// @symbol func_ov098_02138734
extern "C" void func_ov098_02138734(char *c)
{
    _ZN8dActor_c9UpdatePosEP5dCc_c(c, c + 0x564);
    dBgCh_Actr_UpdateContinuous_Veneer(c + 0x320);
    if (func_ov098_02139228(c)) {
        Crate_SetState(c, 0);
        return;
    }
    _Z14ApproachLinearRiii((int *)(c + 0x98), 0, 0x555);
    func_ov098_02138e6c(c);
    func_ov098_021390ec(c);
    if (func_ov098_02138bb8(c)) {
        ((daObjBlockS_c *)c)->Kill();
    }
    _ZN5dCc_c5ClearEv(c + 0x564);
    _ZN5dCc_c6UpdateEv(c + 0x564);
    func_ov098_02139850(c);
    if (!_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x320)) {
        func_ov098_021396a4(c);
    }
    if (((dBgW *)(c + 0x124))->IsEnabled()) {
        ((dBgW *)(c + 0x124))->Disable();
    }
}

// @symbol func_ov098_02138818
extern "C" void func_ov098_02138818(char *self)
{
    BlockSW4 arr1;
    BlockSW4 arr2;
    void *p;
    u32 idx;

    *(int *)(self + 0x5f0) = 0;
    *(int *)(self + 0x5f4) = 0;

    arr1 = data_ov098_0213c4e0;
    arr2 = data_ov098_0213c4f0;

    p = *(void **)(self + 0x5e4);
    idx = 0;
    if (p != 0) {
        idx = *(u32 *)((char *)p + 8);
        if (idx > 4) idx = 4;
    }

    *(u32 *)(self + 0x98) = arr1.w[idx];
    *(u32 *)(self + 0xa8) = arr2.w[idx];

    *(void **)(self + 0x5e8) = *(void **)(self + 0x5e4);
    *(void **)(self + 0x5e4) = 0;

    *(int *)(self + 0x57c) |= 0x2000;
}

// @symbol func_ov098_021388bc
extern "C" void func_ov098_021388bc(char *c)
{
    int flags;
    bool t;

    dBgCh_Actr_UpdateContinuous_Veneer(c + 0x320);
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x320) != 0) {
        if (SurfaceInfo_TestFlag0x20((int *)((char *)_ZNK10dBgCh_Actr14GetFloorResultEv(c + 0x320) + 4)) != 0) {
            void *p = *(void **)(c + 0x5e4);
            if (p != 0) {
                _ZN6Player9DropActorEv(p);
                ((daObjBlockS_c *)c)->Kill();
                return;
            }
        }
    }

    flags = *(int *)(c + 0xb0);
    t = flags & 0x400;
    if (t != false) {
        Crate_SetState(c, 2);
    } else {
        t = flags & 0x2000;
        if (t != false) {
            Crate_SetState(c, 4);
        } else {
            t = flags & 0x100;
            if (t == false) {
                Crate_SetState(c, 0);
            }
        }
    }

    _ZN5dCc_c5ClearEv(c + 0x564);
    func_ov098_02139850(c);
    func_ov098_021396a4(c);
    if (((dBgW *)(c + 0x124))->IsEnabled()) {
        ((dBgW *)(c + 0x124))->Disable();
    }
}

// @symbol func_ov098_021389cc
extern "C" void func_ov098_021389cc(char *c)
{
    *(int *)(c + 0x98) = 0;
    *(int *)(((int)c + 0x57c)) &= ~0x2000;
    *(int *)(c + 0x5f0) = 0;
    *(int *)(c + 0x5f4) = 0;
}

// @symbol func_ov098_021389f8
extern "C" void func_ov098_021389f8(char *c)
{
    int flag = (*(int *)(c + 0xb0) & 8) != 0;
    if (flag) {
        if (*(int *)(c + 0x98) == 0) {
            if (*(int *)(c + 0xa8) == 0) {
                return;
            }
        }
    }
    _ZN8dActor_c9UpdatePosEP5dCc_c(c, c + 0x564);
    dBgCh_Actr_UpdateContinuous_Veneer(c + 0x320);
    func_ov098_02139228(c);
    func_ov098_02138e6c(c);
    func_ov098_021390ec(c);
    if (func_ov098_02138bb8(c) || func_ov098_02138bfc(c)) {
        ((daObjBlockS_c *)c)->Kill();
    }
    _ZN5dCc_c5ClearEv(c + 0x564);
    _ZN5dCc_c6UpdateEv(c + 0x564);
    _ZN5dCc_c5ClearEv(c + 0x5a4);
    _ZN5dCc_c6UpdateEv(c + 0x5a4);
    func_ov098_02139850(c);
    if (!_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x320)) {
        func_ov098_021396a4(c);
    }
    if (!_ZN10dBgActor_c21IsClsnInRangeOnScreenE5Fix12IiES1_(c, 0x600000, 0)) {
        return;
    }
    func_ov098_021397c8(c);
}

// @symbol func_ov098_02138b18
extern "C" void func_ov098_02138b18(char *p)
{
    *(int *)(p + 0x5e4) = 0;
    *(int *)(p + 0xd0) = 0;
}

// @symbol Crate_SetState
extern "C" void Crate_SetState(char *self, int i)
{
    BlockSStateHost *c = (BlockSStateHost *)self;
    c->idx = i;
    int j = c->idx;
    (c->*data_ov098_0213c878[j].fn[0])();
}

// @symbol func_ov098_02138b70
extern "C" void func_ov098_02138b70(char *self)
{
    BlockSStateHost *c = (BlockSStateHost *)self;
    int j = c->idx;
    (c->*data_ov098_0213c878[j].fn[1])();
}

// @symbol func_ov098_02138bb8
extern "C" int func_ov098_02138bb8(char *c)
{
    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x320))
    {
        if (SurfaceInfo_TestFlag0x20((int *)((char *)_ZNK10dBgCh_Actr14GetFloorResultEv(c + 0x320) + 4)))
            return 1;
    }
    return 0;
}

// @symbol func_ov098_02138bfc
extern "C" int func_ov098_02138bfc(char *c)
{
    int r;
    void *a;
    unsigned short type;
    if (!func_02035638((unsigned char *)(c + 0x320))) return 0;
    r = func_0203567c((int)(c + 0x320));
    if (_ZNK5dBgPi9GetClsnIDEv(r) == -1) return 0;
    a = _ZN8dActor_c10FindWithIDEj((unsigned int)_ZNK5dBgPi9GetClsnIDEv(r));
    if (!a) return 0;
    type = *(unsigned short *)((char *)a + 0xc);
    {
        Bool b;
        b = (Bool)(type == 0x135); if (b) goto out;
        b = (Bool)(type == 0xa2); if (b) goto out;
        b = (Bool)(type == 0xa3); if (b) goto out;
        b = (Bool)(type == 0xa1); if (b) goto out;
        b = (Bool)(type == 0xa4); if (!b) return 0;
    out:
        return 1;
    }
}

// @symbol func_ov098_02138ce0
extern "C" void func_ov098_02138ce0(char *c)
{
    struct Vector3 zero, a1, a2;

    *(int *)(c + 0x5f0) = 0;
    *(int *)(c + 0x5f4) = 0;
    *(int *)(c + 0x5fc) = 0;
    *(int *)(c + 0x600) = 0;
    *(char *)(c + 0x606) = 0;
    *(int *)(c + 0x5c) = *(int *)(c + 0x4e8);
    *(int *)(c + 0x60) = *(int *)(c + 0x4ec);
    *(int *)(c + 0x64) = *(int *)(c + 0x4f0);
    *(short *)(c + 0x8c) = *(short *)(c + 0x500);
    *(short *)(c + 0x8e) = *(short *)(c + 0x502);
    *(short *)(c + 0x90) = *(short *)(c + 0x504);
    *(int *)(c + 0x98) = 0;
    *(int *)(c + 0xa4) = 0;
    *(int *)(c + 0xa8) = 0;
    *(int *)(c + 0xac) = 0;

    ((struct Vector3 *)(((long long)(int)&zero)))->x = 0;
    ((struct Vector3 *)(((long long)(int)&zero)))->y = 0;
    ((struct Vector3 *)(((long long)(int)&zero)))->z = 0;

    a1.x = zero.x;
    a1.y = zero.y;
    a1.z = zero.z;
    _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
        c + 0x564, c, &a1, 0x3c000, 0x6e000, 0x200002, 0x4d390);

    a2.x = zero.x;
    a2.y = zero.y;
    a2.z = zero.z;
    _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
        c + 0x5a4, c, &a2, 0x3c000, 0x1e000, 0x800004, 0);
}

// @symbol func_ov098_02138e08
extern "C" void func_ov098_02138e08(char *c)
{
    int v[3];
    if (*(unsigned char *)(c + 0x607) == 1) return;
    v[0] = *(int *)(c + 0x5c);
    v[1] = *(int *)(c + 0x60);
    v[2] = *(int *)(c + 0x64);
    _ZN8dActor_c10SpawnCoinsERK7Vector3j5Fix12IiEs(c, v, 3, 0xf000, 0);
    *(unsigned char *)(c + 0x607) = 1;
}

// @symbol func_ov098_02138e6c
extern "C" void func_ov098_02138e6c(char *c)
{
    void *a;
    u32 fl;
    int b;

    if (*(u8 *)(c + 0x606) != 0) return;

    fl = *(u32 *)(c + 0xb0);
    {
        int b1 = (int)((fl & 0x20000) != 0);
        if (b1 != 0) {
            Crate_SetState(c, 5);
        } else {
            int b2 = (int)((fl & 0x40000) != 0);
            if (b2 != 0) {
                Crate_SetState(c, 5);
            }
        }
    }

    if (*(u32 *)(c + 0x588) == 0) return;

    if ((*(u32 *)(c + 0x584) & 0x40000) != 0) {
        u32 *pp = (u32 *)(((int)c + 0x57c));
        *(u8 *)(c + 0x606) = 0x3c;
        *pp = *pp & ~0x8000u;
    }
    if ((*(u32 *)(c + 0x584) & 0x4000) != 0) {
        daObjBlockS_c *o = (daObjBlockS_c *)c;
        o->Kill();
    }

    a = _ZN8dActor_c10FindWithIDEj(*(u32 *)(c + 0x588));
    if (a == 0) return;
    {
        int bf = (int)(*(u16 *)((char *)a + 0xc) == 0xbf);
        if (bf == 0) return;
    }

    fl = *(u32 *)(c + 0x584);
    if ((fl & 0x380) != 0) {
        daObjBlockS_c *o = (daObjBlockS_c *)c;
        o->Kill();
        return;
    }
    if ((fl & 0x10) != 0) {
        daObjBlockS_c *o = (daObjBlockS_c *)c;
        o->Kill();
        return;
    }
    if ((fl & 0x1000) != 0) {
        if (_ZN6Player7TryGrabER8dActor_c(a, c) == 0) return;
        *(void **)(c + 0x5e4) = a;
        Crate_SetState(c, 1);
        return;
    }

    if (*(u32 *)(c + 0x560) != 2) return;
    b = 1;
    if (*(void **)(c + 0x5e8) == a) b = 0;
    if (b == 0) return;

    {
        struct Vector3 v;
        v.x = *(int *)(c + 0x5c);
        v.y = *(int *)(c + 0x60);
        v.z = *(int *)(c + 0x64);
        _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(a, &v, 1, 0xc000, 1, 0, 1);
    }
}

// @symbol _ZN13daObjBlockS_c4KillEv
/* Slot 31. Puffs a simple particle and a dust poof at the crate's own
 * position raised 0x28000, plays the break sample from the camera-space
 * position, and parks the crate in state 6. */
void daObjBlockS_c::Kill()
{
    Vector3 vec;
    Vector3 vec2;
    int x, y, z;
    func_ov098_02138e08((char *)this);
    x = mPosX;
    y = mPosY + 0x28000;
    z = mPosZ;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0xe, vec.x, vec.y, vec.z);
    ((int *)&vec2)[0] = ((int *)&vec)[0];
    ((int *)&vec2)[1] = ((int *)&vec)[1];
    ((int *)&vec2)[2] = ((int *)&vec)[2];
    _ZN8dActor_c19DisappearPoofDustAtERK7Vector3((char *)this, &vec2);
    _ZN5Sound9PlayBank3EjRK7Vector3(0x41, (Vector3 *)&mCamSpacePosX);
    Crate_SetState((char *)this, 6);
}

// @symbol func_ov098_021390ec
extern "C" void func_ov098_021390ec(char *cc)
{
    daObjBlockS_c *c = (daObjBlockS_c *)cc;
    if (DecIfAbove0_Byte((u8 *)((char *)c + 0x605)) != 0)
        return;
    if (((dBgCh_Actr *)((char *)c + 0x320))->IsOnWall() != 0) {
        dBgPi *wr = _ZNK10dBgCh_Actr13GetWallResultEv((dBgCh_Actr *)((char *)c + 0x320));
        if (wr->GetClsnID() != -1) {
            dActor_c *a = dActor_c::FindWithID((u32)wr->GetClsnID());
            if (a != 0) {
                int isF = (*(unsigned short *)((char *)a + 0xc) == 0xf);
                if (isF == 0) {
                    c->Kill();
                    return;
                }
            }
        }
    }
    if (func_ov002_020ef228((char *)c + 0x320, (int)c) != 0) {
        *(u8 *)((char *)c + 0x605) = 3;
        return;
    }
    if (((dBgCh_Actr *)((char *)c + 0x320))->IsOnWall() == 0)
        return;
    if (*(int *)((char *)c + 0x98) > 0x14000) {
        c->Kill();
        return;
    }
    Vector3 v;
    ((SurfaceInfo *)((char *)_ZNK10dBgCh_Actr13GetWallResultEv((dBgCh_Actr *)((char *)c + 0x320)) + 4))->CopyNormalTo(v);
    *(s16 *)((char *)c + 0x94) =
        _ZN8dActor_c12ReflectAngleE5Fix12IiES1_s(c, v.x, v.z, *(s16 *)((char *)c + 0x94));
}

#pragma push
// @symbol func_ov098_02139228
extern "C" int func_ov098_02139228(char *c)
{

#pragma opt_propagation off
    int base = 2;
    void *fr;
    int n;
    int ang;
    int newAng;
    int hl;
    int i94, iang;
    Vector3 a1;
    Vector3 a2;
    Vector3 sum;

    if (_ZNK10dBgCh_Actr10IsOnGroundEv(c + 0x320) == 0)
        return 0;

    fr = _ZNK10dBgCh_Actr14GetFloorResultEv(c + 0x320);
    switch (func_02037e38((u32 *)((char *)fr + 4))) {
    case 6:
        *(s32 *)(c + 0x5f0) = 0x1e000;
        break;
    case 7:
        *(s32 *)(c + 0x5f0) = 0x2d000;
        break;
    case 8:
        *(s32 *)(c + 0x5f0) = 0x3c000;
        break;
    case 9:
        *(s32 *)(c + 0x5f0) = 0x64000;
        if (*(s32 *)(c + 0x5f0) == *(s32 *)(c + 0x5f4)) {
            Crate_SetState(c, 6);
            return 0;
        }
        break;
    }

    _Z14ApproachLinearRiii((int *)(c + 0x5f4), *(s32 *)(c + 0x5f0), 0x800);

    if ((*(s32 *)(c + 0x4e0) | *(s32 *)(c + 0x98)) == 0) {
        int *p = (int *)(((int)c + 0x57c));
        *p &= ~0x2000;
        return 0;
    }

    if (*(s32 *)(c + 0xa8) < -0xb000) {
        ((daObjBlockS_c *)c)->Kill();
    }

    _ZNK11SurfaceInfo12CopyNormalToER7Vector3((char *)fr + 4, (Vector3 *)(c + 0x4dc));
    n = func_02037e58((u32 *)((char *)fr + 4));
    ang = _ZN4cstd5atan2E5Fix12IiES1_(*(s32 *)(c + 0x4dc), *(s32 *)(c + 0x4e4));
    hl = func_ov002_020f02c8(n);
    func_ov002_020f030c(n);


    {
        u16 r_94 = *(u16 *)(c + 0x94);
        int sa = (u16)ang;
        sa >>= 4;
        int s94 = r_94;
        s94 >>= 4;
        int li94 = s94 * 2;
        s32 v98 = *(s32 *)(c + 0x98);
        s16 cos94 = data_02082214[li94];
        s16 sin94 = data_02082214[li94 + 1];
        a1.y = 0;
        a2.y = 0;
        s32 a1x = (s32)(((s64)v98 * cos94 + 0x800) >> 12);
        a1.z = (s32)(((s64)v98 * sin94 + 0x800) >> 12);
        s16 cosa = data_02082214[sa * 2];
        s16 sina = data_02082214[sa * 2 + 1];
        a2.x = (s32)(((s64)hl * cosa + 0x800) >> 12);
        a2.z = (s32)(((s64)hl * sina + 0x800) >> 12);
        a1.x = a1x;
    }
    Vec3_MulScalarInPlace(&a2.x, Vec3_HorzLen((Vector3 *)(c + 0x4dc)));
    Vec3_Add(&sum, &a1, &a2);
    newAng = _ZN4cstd5atan2E5Fix12IiES1_(sum.x, sum.z);
    *(s32 *)(c + 0x98) = Vec3_HorzLen(&sum);
    if (*(s32 *)(c + 0x98) > 0x64000)
        *(s32 *)(c + 0x98) = 0x64000;
    *(s16 *)(c + 0x94) = newAng;
    AngleDiff(*(s16 *)(c + 0x94), *(s16 *)(c + 0x8e));

    {
        s32 m0 = (s32)(((s64)*(s32 *)(c + 0x4dc) * *(s32 *)(c + 0xa4) + 0x800) >> 12);
        s32 m1 = (s32)(((s64)*(s32 *)(c + 0x4e4) * *(s32 *)(c + 0xac) + 0x800) >> 12);
        *(s32 *)(c + 0xa8) = -(_ZN4cstd4fdivEii(m0 + m1, *(s32 *)(c + 0x4e0)) + 0x8000);
    }

    if (func_ov002_020f035c(n, *(s32 *)(c + 0x4e0)) != 0 && *(s32 *)(c + 0x98) > 0x5000) {
        int q0 = *(s16 *)(c + 0x8c);
        int q1;
        int sd;
        if (q0 < 0) q0 = ((-q0) << 16) >> 16;
        if (q0 < 0x10) {
            q1 = *(s16 *)(c + 0x90);
            if (q1 < 0) q1 = ((-q1) << 16) >> 16;
            if (q1 < 0x10) {
                s32 v = *(s32 *)(c + 0x98);
                sd = _ZN4cstd4fdivEii((s32)(((s64)v * 8 + 0x800) >> 12), 0xa);
                if (sd < 0) sd = -sd;
                if (*(s32 *)(c + 0xa8) > 0xa000)
                    _ZN5Sound9PlayBank3EjRK7Vector3(0x51, (Vector3 *)(c + 0x74));
                if (*(s32 *)(c + 0xa8) > sd)
                    *(s32 *)(c + 0xa8) = sd;
            }
        }
    }

    if (_Z14ApproachLinearRiii((int *)(c + 0x98), 0, 0x800) == 0
        && *(s32 *)(c + 0x5f0) != 0xa0
        && *(s32 *)(c + 0x98) > 0xa000) {
        _ZN8Particle20RunningSlidingDustAtE5Fix12IiES1_S1_(*(s32 *)(c + 0x5c), *(s32 *)(c + 0x60), *(s32 *)(c + 0x64));
        *(u32 *)(c + 0x5f8) = _ZN5Sound8PlayLongEjjjRK7Vector3s(*(u32 *)(c + 0x5f8), 3, 0x93, (Vector3 *)(c + 0x74), 0);
    }

    {
        s16 r5 = func_02010844(c, (Vector3 *)(c + 0x4dc), *(s16 *)(c + 0x8e));
        s16 r4 = func_02010844(c, (Vector3 *)(c + 0x4dc), (s16)(*(s16 *)(c + 0x8e) - 0x4000));
        _Z11UpdateAngleRssis((s16 *)(c + 0x8c), r5, 4, 0x1000);
        _Z11UpdateAngleRssis((s16 *)(c + 0x90), r4, 4, 0x1000);
    }
    return 1 | (base & 0);
}
#pragma pop

// @symbol func_ov098_021396a4
extern "C" void func_ov098_021396a4(char *c)
{
    struct Vector3 v;
    int r5;
    int r4;

    v.x = *(int *)(c + 0x5c);
    v.y = *(int *)(c + 0x60);
    v.z = *(int *)(c + 0x64);
    v.y -= 0xa000;
    dBgCh_Gnd rg;
    rg.SetObjAndPos(v, 0);
    *(int *)(c + 0x5ec) = v.y;
    if (rg.DetectClsn()) {
        *(int *)(c + 0x5ec) = rg.clsnY;
    }
    r5 = *(int *)(c + 0x60) - *(int *)(c + 0x5ec);
    if (r5 <= 0x1000) r5 = 0x1000;
    r4 = 0x64000 - (int)(((long long)r5 * 0x180 + 0x800) >> 12);
    if (r4 < 0xa000) r4 = 0xa000;
    Matrix4x3_FromRotationY(c + 0x530, *(short *)(c + 0x8e));
    *(int *)(c + 0x554) = *(int *)(c + 0x5c) >> 3;
    *(int *)(c + 0x558) = (*(int *)(c + 0x60) - 0x14000) >> 3;
    *(int *)(c + 0x55c) = *(int *)(c + 0x64) >> 3;
    _ZN8dActor_c18DropShadowScaleXYZER11ShadowModelR9Matrix4x35Fix12IiES5_S5_j(
        c, c + 0x508, c + 0x530, r4, r5 + 0x28000, r4, 0xf);
}

struct BlockSClsnMtx { char p[0x2ec]; Matrix4x3 m; };

// @symbol func_ov098_021397c8
extern "C" void func_ov098_021397c8(char *self)
{
    BlockSClsnMtx *o = (BlockSClsnMtx *)self;
    volatile int tmp[3];
    tmp[0] = *(int *)(self + 0x5c);
    int origY = *(int *)(self + 0x60);
    tmp[1] = origY;
    tmp[2] = *(int *)(self + 0x64);
    tmp[1] = origY - *(int *)(self + 0x5f4);
    o->m = *(Matrix4x3 *)(self + 0xf0);
    *(int *)(self + 0x310) = tmp[0];
    *(int *)(self + 0x314) = tmp[1];
    *(int *)(self + 0x318) = tmp[2];
    _ZN10dBgW_KcMbg9TransformERK9Matrix4x3s(self + 0x124, o->m, *(short *)(self + 0x8e));
}

// @symbol func_ov098_02139850
extern "C" void func_ov098_02139850(char *self)
{
    char *obj = *(char **)(self + 0x5e4);
    int b;

    if (obj == 0) goto other;
    b = (int)((*(u32 *)(self + 0xb0) & 0x4000) != 0);
    if (b == 0) goto other;
    if (*(int *)(obj + 0xc8) == 0) goto other;

    {
        char *r4 = (char *)func_ov002_020e496c(obj);
        int r5 = 0;
        if (_ZN6Player14IsFrontSlidingEv(*(char **)(self + 0x5e4)) != 0) r5 = 1;
        if (_ZN6Player17LostGrabbedObjectEv(*(char **)(self + 0x5e4)) != 0) {
            if (((u32)*(int *)(r4 + 0x58) << 4) >> 0x10 < 0xe) r5 = 1;
        }
        if (*(int *)(*(char **)(self + 0x5e4) + 8) == 2) {
            r5 = (r5 + 2) & 0xff;
        }
        Math_Function_0203b14c(self + 0x4f4, *(int *)((char *)data_ov098_0213bf60 + r5 * 0xc), 0x800, 0x3e8000, 4);
        Math_Function_0203b14c(self + 0x4f8, *(int *)((char *)data_ov098_0213bf64 + r5 * 0xc), 0x800, 0x3e8000, 4);
        Math_Function_0203b14c(self + 0x4fc, *(int *)((char *)data_ov098_0213bf68 + r5 * 0xc), 0x800, 0x3e8000, 4);
        {
            char *res = _ZN8dActor_c11UpdateCarryER6PlayerRK7Vector3(self, *(char **)(self + 0x5e4), (struct Vector3 *)(self + 0x4f4));
            *(struct Matrix4x3 *)(self + 0xf0) = *(struct Matrix4x3 *)res;
        }
        *(u32 *)(((int)self + 0xb0)) |= 0x4000000;
    }
    return;

other:
    {
        volatile int tmp[3];
        tmp[0] = *(int *)(self + 0x5c);
        tmp[1] = *(int *)(self + 0x60);
        tmp[2] = *(int *)(self + 0x64);
        tmp[1] = *(int *)(self + 0x60) - *(int *)(self + 0x5f4);
        Matrix4x3_FromRotationZXYExt(self + 0xf0, *(s16 *)(self + 0x8c), *(s16 *)(self + 0x8e), *(s16 *)(self + 0x90));
        *(int *)(self + 0x114) = tmp[0] >> 3;
        *(int *)(self + 0x118) = tmp[1] >> 3;
        *(int *)(self + 0x11c) = tmp[2] >> 3;
        *(int *)(self + 0x4f4) = 0;
        *(int *)(self + 0x4f8) = 0;
        *(int *)(self + 0x4fc) = 0;
        if (_ZNK10dBgCh_Actr10IsOnGroundEv(self + 0x320) != 0) {
            *(u32 *)(((int)self + 0xb0)) &= ~0x4000000;
        }
    }
}

// @symbol _ZN13daObjBlockS_c16CleanupResourcesEv
int daObjBlockS_c::CleanupResources()
{
    int *f;
    if (((dBgW *)((char *)&mMeshCollider))->IsEnabled())
        ((dBgW *)((char *)&mMeshCollider))->Disable();
    f = 0;
    if (actorID == 0xc2)
        f = data_ov098_0213c4c8;
    if (f) {
        ((SharedFilePtr *)((void *)f[0]))->Release();
        ((SharedFilePtr *)((void *)f[1]))->Release();
    }
    return 1;
}

// @symbol _ZN13daObjBlockS_c6RenderEv
int daObjBlockS_c::Render()
{
    if (mState == 6)
        return 1;
    {
        int b = (int)((mFlags & 0x40000) != 0);
        if (b)
            return 1;
    }
    Sub *o = (Sub *)&mModel;
    o->m5(0);
    return 1;
}

// @symbol _ZN13daObjBlockS_c8BehaviorEv
int daObjBlockS_c::Behavior()
{
    struct Vector3 v;
    struct Vector3 vec;
    struct Vector3 t;
    struct Vector3 vec2;
    enum Bool b1, b2;
    int x, y, z;

    b1 = (enum Bool)((mFlags & 0x4000000) != 0);
    if (b1 != FALSE && (data_0209b454 & 0x4000000) && mHoldingPlayer) {
        mHoldingPlayer->DropActor();
    }

    b2 = (enum Bool)((mFlags & 8) != 0);
    if (b2 != FALSE
        && Vec3_HorzDist((struct Vector3 *)&mPosX, (const struct Vector3 *)&mHomePosX)
        && DistToCPlayer() > 0x7d0000) {
        Crate_SetState(((char *)this), 6);
        return 1;
    }

    if (mBreakTimer != 0) {
        x = mPosX;
        z = mPosZ;
        y = mPosY + 0x50000;
        ((int *)&v)[0] = x;
        ((int *)&v)[1] = y;
        ((int *)&v)[2] = z;
        if (DecIfAbove0_Byte(&mBreakTimer)) {
            mParticleHandle1 = (u32)_ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
                mParticleHandle1, 0x13a, v.x, v.y, v.z, 0, 0);
            mParticleHandle2 = _ZN8Particle6System17NewUnkCallback818Ejj5Fix12IiES2_S2_PK11Vector3_16f(
                mParticleHandle2, 0x13b, v.x, v.y, v.z, 0);
            goto done;
        }
        func_ov098_02138e08(((char *)this));
        x = mPosX;
        z = mPosZ;
        y = mPosY + 0x28000;
        vec.x = x;
        vec.y = y;
        vec.z = z;
        ((int *)&vec2)[0] = ((int *)&vec)[0];
        ((int *)&vec2)[1] = ((int *)&vec)[1];
        ((int *)&vec2)[2] = ((int *)&vec)[2];
        _ZN8dActor_c19DisappearPoofDustAtERK7Vector3(((char *)this), &vec2);
        Crate_SetState(((char *)this), 6);
        return 1;
    }

done:
    ((int *)&t)[0] = mPosX;
    ((int *)&t)[1] = mPosY;
    ((int *)&t)[2] = mPosZ;
    ((int *)&t)[1] = t.y - mClsnYOffset;
    mdCcAcPos_c1.pos.x = t.x;
    mdCcAcPos_c1.pos.y = t.y;
    mdCcAcPos_c1.pos.z = t.z;
    mdCcAcPos_c2.pos.x = t.x;
    mdCcAcPos_c2.pos.y = t.y;
    mdCcAcPos_c2.pos.z = t.z;
    func_ov098_02138b70(((char *)this));
    return 1;
}

// @symbol _ZN13daObjBlockS_c13InitResourcesEv
int daObjBlockS_c::InitResources()
{
    char *f = 0;
    if (actorID == 0xc2)
        f = (char *)data_ov098_0213c4c8;
    if (f == 0)
        return 0;
    mHomePosX = mPosX;
    mHomePosY = mPosY;
    mHomePosZ = mPosZ;
    mHomeAngleX = mAngleX;
    mHomeAngleY = mAngleY;
    mHomeAngleZ = mAngleZ;
    mModel.SetFile((BMD_File *)Model::LoadFile(**(SharedFilePtr **)(f)), 1, -1);
    mShadowModel.InitCuboid();
    _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
        (char *)&mMeshCollider, (int)dBgW_Kc::LoadFile(**(SharedFilePtr **)(f + 4)),
        (char *)&mClsnMat, 0x199, mAngleY, *(int *)(f + 8));
    _ZN10dBgCh_Actr4InitEP8dActor_c5Fix12IiES3_P10Vector3_16S5_(
        (char *)&mWithMeshClsn, ((char *)this), 0x28000, 0x28000, 0, 0);
    mWithMeshClsn.StartDetectingWater();
    mVertAccel = -0x2000;
    mTerminalVelocity = -0x3c000;
    Crate_SetState(((char *)this), 0);
    /* dBgActor_c's own generic 0xd0..0xd4 pad (include/dBgActor_c.h), not a
       daObjBlockS_c field -- reused here by raw offset, same idiom as
       daObjRc_Guruguru_c's tail-padding field. */
    *(s32 *)(((char *)this) + 0xd0) = 0;
    func_ov098_02138ce0(((char *)this));
    return 1;
}

// @symbol _ZN13daObjBlockS_c15OnGroundPoundedER8dActor_c
/* Slot 21 (include/dActor_c.h), recovered from vtable slot identity.
 * Ground-pounding the crate just calls its own Kill (slot 31); the
 * `dActor_c &other` parameter is unused, matching the ROM body -- it loads
 * the vtable slot and calls through it without ever touching r1. */
void daObjBlockS_c::OnGroundPounded(dActor_c &other)
{
    Kill();
}

// @symbol _ZN13daObjBlockS_c13OnYoshiTryEatEv
/* Slot 18. Returns 6 (edible) unless the crate is already breaking: Behavior
 * counts mBreakTimer down once a frame and poofs the crate when it reaches 0,
 * and while that is running Yoshi gets nothing. */
int daObjBlockS_c::OnYoshiTryEat()
{
    unsigned char v = mBreakTimer;
    if (v != 0)
        return 0;
    return 6;
}

// @symbol _ZN13daObjBlockS_c13OnTurnIntoEggER6Player
/* Slot 19. Pays out three coins the first time only, tracked by mCoinsPaid:
 * handed over directly if the player is mid cap-collect, otherwise banked on
 * the egg. Either way the crate goes to state 6. The shared actor hook
 * returns void, so the method ends after the state change with no invented
 * result. */
void daObjBlockS_c::OnTurnIntoEgg(Player &player)
{
    char *r4 = (char *)&player;
    if (((Player *)r4)->IsCollectingCap()) {
        if (mCoinsPaid != 1) {
            ((dActor_c *)this)->GivePlayerCoins(*(Player *)r4, 3, 0);
            mCoinsPaid = 1;
        }
    } else {
        unsigned int count = 0;
        if (mCoinsPaid != 1) {
            mCoinsPaid = 1;
            count = 3;
        }
        ((Player *)r4)->RegisterEggCoinCount(count, 0, 0);
    }
    Crate_SetState((char *)this, 6);
}

// @symbol daObjBlockS_c_classInit
/* Reconstructed source-style name: SM64DS proves daObjBlockS_c through RTTI,
 * allocation size, vtable identity, and the BLOCK_S registry profile;
 * later EAD lineage supplies classInit. Exact original spelling is not
 * preserved. Historical alias: Crate_Spawn. */
extern "C" daObjBlockS_c *daObjBlockS_c_classInit()
{
    return new daObjBlockS_c();
}
