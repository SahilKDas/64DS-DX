//cpp
/* daWanwan2_c -- the unchained Chain Chomp (registry profile WANWAN2).
 *
 * The chomp walks a path (param1's low byte is the path ID) and drags a
 * six-link chain behind it: func_ov100_021437d4 steps the links,
 * func_ov100_02143b68 builds the per-frame model and shadow matrices,
 * func_ov100_0214344c handles contact (some hits set the +0x6a6 timer and
 * double the chomp's scale, a player it touches gets hurt) and
 * func_ov100_021435e8 keeps the actor it spawned at the chain's last link
 * in step. func_ov100_02143370 is the floor probe. Behavior runs a state
 * through the pointer-to-member pair at +0x668: func_ov100_02143b18 enters
 * a state and calls its first member, Behavior calls the second each frame.
 * The only state table, data_ov100_021486f4, is seeded by the module's
 * static initializer from the constants at data_ov100_02148000
 * (func_ov100_02143ae0, the enter) and data_ov100_02147ff8
 * (func_ov100_02143aa4, the execute).
 *
 * Those two constants sit straight after daIbl_c's 31-slot vtable
 * (0x02147f7c..0x02147ff8), which is why a vtable scan read three extra
 * slots into daIbl_c and once labelled func_ov100_02143aa4 a daIbl_c
 * "Kill". Neither is a virtual function of either class: both run on this
 * class's fields.
 *
 * This file is the whole linker unit 0x021431c4..0x021442dc, 16 functions:
 * D1 and D0 (daIbl_c_classInit, the last function of src/actors/daIbl_c.cpp,
 * ends exactly at 0x021431c4 below them), the eight helpers
 * func_ov100_02143370 through func_ov100_02143b68, CleanupResources,
 * OnPendingDestroy, Render, Behavior, InitResources and OnAimedAtWithEgg.
 * The registry factory daWanwan2_c_classInit (0x021442dc..0x021443f4) stays
 * in its own source: see the note at the end of this file. daDoor_c's D1
 * starts at 0x021443f4 above it. The out-of-line destructor is the key
 * function, so this TU also emits the vtable and the RTTI.
 *
 * It replaces the one-function sources for _ZN11daWanwan2_cD1Ev,
 * _ZN11daWanwan2_cD0Ev, func_ov100_02143370 .. func_ov100_02143b68,
 * _ZN11daWanwan2_c16CleanupResourcesEv, _ZN11daWanwan2_c16OnPendingDestroyEv,
 * _ZN11daWanwan2_c6RenderEv, _ZN11daWanwan2_c8BehaviorEv,
 * _ZN11daWanwan2_c13InitResourcesEv and _ZN11daWanwan2_c16OnAimedAtWithEggEv.
 * Each member keeps the provenance notes its source carried.
 *
 * `#pragma defer_codegen off` keeps this file in ROM order.
 *
 * Leftover: the helpers keep their C-ABI cartridge names and reach the
 *   chomp through raw offsets, as does Behavior.
 * Leftover: the callees with Fix12<int> parameters (dActor_c's shadow drop,
 *   Player::Hurt, ModelAnim::SetAnim, dCcAcPos_c::Init) stay spelled as
 *   mangled extern-C free functions.
 */

#pragma defer_codegen off

#include "daWanwan2_c.h"
#include "types.h"
#include "common.h"
#include "SharedFilePtr.h"
#include "PathPtr.h"
#include "dBgCh_Lin.h"

/* Three plain words: the stack vectors of the two helpers that were C,
   which carry none of Vector3's empty destructor. */
struct Vec3i { s32 x, y, z; };

/* func_ov100_02143b68's identity matrix, as twelve words: a copy through the
   C++ Matrix4x3 splits into two block moves here. */
struct M48 { int w[12]; };

/* The state machine's pointer-to-member pair. The `self` type is a stand-in,
   NOT the real dActor_c: a pointer-to-member on a non-polymorphic,
   single-base class is laid out differently from one on the real class, so
   the shape here is codegen, not decoration, and binding it to the real
   dActor_c makes mwccarm abort with an internal compiler error. */
struct ChompPmfSelf;
typedef int (ChompPmfSelf::*ChompPmf)();
struct ChompState {
    ChompPmf enter;     /* func_ov100_02143b18 calls it on entry */
    ChompPmf execute;   /* Behavior calls it every frame */
};
struct ChompPmfSelf {
    char pad[0x668];
    ChompState *state;  /* 0x668, daWanwan2_c::unk_668 */
};

struct Vector3_16;
struct dCc_c;

extern "C" {
unsigned short DecIfAbove0_Short(unsigned short *p);
void _Z14ApproachLinearRiii(int &x, int target, int step);
int __aeabi_idiv(int, int);
void _ZN8dActor_c9UpdatePosEP5dCc_c(dActor_c *thiz, dCc_c *c);
void _ZN8dActor_c15HugeLandingDustEb(dActor_c *thiz, bool b);
dActor_c *_ZN8dActor_c13ClosestPlayerEv(dActor_c *thiz);
void *_ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(u32 id, u32 param, const Vector3 *pos, const Vector3_16 *rot, int area, int unk);
void *_ZN8dActor_c10FindWithIDEj(u32 id);
void _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(void *self, void *sm, void *mtx, int a, int b, unsigned int g);
void _ZN10dCcAcPos_c21SetPosRelativeToActorERK7Vector3(void *clsn, const void *pos);
void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
    dCcAcPos_c *clsn, dActor_c *actor, const Vector3 &offset,
    s32 radius, s32 height, u32 flags, u32 vulnFlags);
void _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(void *p, const void *pos, u32 a, int b, u32 c, u32 d, u32 e);
void _ZN5dCc_c5ClearEv(void *thiz);
int _ZN5dCc_c6UpdateEv(void *thiz);
int func_02012694(int, void *);
void _ZN9Animation7AdvanceEv(void *anim);
int _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(char *anim, void *file, int a, int b, unsigned int u);
void *_ZN7PathPtrC1Ev(void *thiz);
void _ZN7PathPtr6FromIDEj(void *thiz, unsigned int id);
void _ZNK7PathPtr7GetNodeER7Vector3j(void *thiz, Vector3 &out, unsigned int idx);
void Matrix4x3_FromRotationXYZExt(void *m, int x, int y, int z);
void Matrix4x3_FromRotationY(void *m, int angle);
void Matrix4x3_ApplyInPlaceToRotationX(void *m, int angX);
void MulVec3Mat4x3(void *in, void *m, void *out);
void Vec3_Add(void *out, void *a, void *b);
void Vec3_Sub(void *out, void *a, void *b);
void Vec3_MulScalar(void *out, const void *in, int scale);
int Vec3_HorzLen(void *v);
int LenVec3(Vector3 *v);
short Vec3_HorzAngle(const Vector3 *a, const Vector3 *b);
void ApproachAngle(short *cur, short target, int step, int a, int b);
int _ZN4cstd5atan2E5Fix12IiES1_(int y, int x);
void LoadSilverStarAndNumber();
void UnloadSilverStarAndNumber();

extern SharedFilePtr data_ov002_0211092c;
extern SharedFilePtr data_ov100_021486bc;
extern SharedFilePtr data_ov100_021486a4;
extern SharedFilePtr data_ov100_021486ac;
extern SharedFilePtr data_ov100_021486b4;
extern s32 data_ov100_02148008[3];
extern ChompState data_ov100_021486f4;
extern unsigned char data_0209f2d8[];
extern short data_02082214[];
extern char data_020a0e68[];
}

extern Matrix4x3 IDENTITY_MATRIX4X3;

/* -------------------------------------------------------------------------- */
/* ROM ordinals 0 and 1 -- _ZN11daWanwan2_cD1Ev, 0x021431c4, size 0xcc;
 *                         _ZN11daWanwan2_cD0Ev, 0x02143290, size 0xe0 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daWanwan2_cD1Ev
// @symbol _ZN11daWanwan2_cD0Ev
/* recovered: real C++ destructor -- the compiler emits the whole body.
 *
 * One vtable store and ten teardowns, every one a consequence of
 * `struct daWanwan2_c : dEnemyBase_c` and the members that declaration
 * types. Six of them are arrays, and the compiler's own loops reproduce the
 * ROM's __cxa_vec_cleanup calls with the same counts and strides -- which is
 * what makes this body the evidence for the header rather than a
 * transcription of it. D0 is the same teardown followed by dEnemyBase_c's
 * inline operator delete. */
daWanwan2_c::~daWanwan2_c()
{
}

#ifdef _MSC_VER
/* The host needs the ROM's flat D0 name, and MSVC never emits it: it folds
 * the Itanium destructor variants into the one ~daWanwan2_c() above. This
 * arm spells out what the deleting destructor does -- the D1 body, called
 * qualified so it is a direct call, then the class-specific operator
 * delete. Nothing here reaches mwccarm. */
extern "C" daWanwan2_c *_ZN11daWanwan2_cD0Ev(daWanwan2_c *thiz)
{
    thiz->daWanwan2_c::~daWanwan2_c();
    daWanwan2_c::operator delete(thiz);
    return thiz;
}
#endif

/* -------------------------------------------------------------------------- */
/* ROM ordinal 2 -- func_ov100_02143370, 0x02143370, size 0xdc */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02143370
/* The floor probe: a vertical line from 0xa000 above the chomp to 0xb8000
   below it. Returns 1 on a hit. */
extern "C" int func_ov100_02143370(char *c)
{
    Vector3 va;
    Vector3 vb;
    int ya, yb;
    dBgCh_Lin line1;
    dBgCh_Lin line2;
    va.x = 0; va.y = 0; va.z = 0;
    vb.x = 0; vb.y = 0; vb.z = 0;
    va.x = *(int *)(c + 0x5c);
    ya = *(int *)(c + 0x60);
    va.y = ya;
    va.z = *(int *)(c + 0x64);
    vb.x = *(int *)(c + 0x5c);
    yb = *(int *)(c + 0x60);
    vb.y = yb;
    vb.z = *(int *)(c + 0x64);
    va.y = ya + 0xa000;
    vb.y = yb - 0xb8000;
    line1.SetObjAndLine(va, vb, (dActor_c *)c);
    if (line1.DetectClsn()) {
        return 1;
    }
    return 0;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 3 -- func_ov100_0214344c, 0x0214344c, size 0x19c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_0214344c
/* Contact. Moves the collision body to the chomp, then looks at the actor
   whose ID is at +0x134: hit flag 0x4000, flag 0x10, or flag 0x2000 from
   actor type 9 sets the +0x6a6 timer to 0x5a and the scale to 2.0, which
   Behavior eases back to 1.0; a player (actor type 0xbf) not already being
   hurt (+0x6fb) is hurt from the chomp's position. Was a C source; its
   `enum Bool` comparison temporaries are kept. */
enum Bool { FALSE, TRUE };

extern "C" void func_ov100_0214344c(char *self)
{
    Vec3i v;
    char *other;

    int flags;
    u32 id;

    v.x = data_ov100_02148008[0];
    v.y = data_ov100_02148008[1];
    v.z = data_ov100_02148008[2];
    _ZN10dCcAcPos_c21SetPosRelativeToActorERK7Vector3(self + 0x110, &v);
    id = *(u32 *)(self + 0x134);
    if (id == 0) return;
    other = (char *)_ZN8dActor_c10FindWithIDEj(id);
    if (other == 0) return;
    flags = *(int *)(self + 0x130);
    if (flags & 0x4000) {
        *(u16 *)(self + 0x6a6) = 0x5a;
        *(u32 *)(self + 0x80) = 0x2000;
        *(u32 *)(self + 0x84) = *(u32 *)(self + 0x80);
        *(u32 *)(self + 0x88) = *(u32 *)(self + 0x84);
        return;
    }
    if (flags & 0x10) {
        *(u16 *)(self + 0x6a6) = 0x5a;
        *(u32 *)(self + 0x80) = 0x2000;
        *(u32 *)(self + 0x84) = *(u32 *)(self + 0x80);
        *(u32 *)(self + 0x88) = *(u32 *)(self + 0x84);
        return;
    }

    {
        enum Bool b = (enum Bool)(*(u16 *)(other + 0xc) == 9);
        if (b != FALSE && (flags & 0x2000)) {
            *(u16 *)(self + 0x6a6) = 0x5a;
            *(u32 *)(self + 0x80) = 0x2000;
            *(u32 *)(self + 0x84) = *(u32 *)(self + 0x80);
            *(u32 *)(self + 0x88) = *(u32 *)(self + 0x84);
            return;
        }
    }
    {
        enum Bool b = (enum Bool)(*(u16 *)(other + 0xc) == 0xbf);
        if (b == FALSE) return;
    }
    if (*(u8 *)(other + 0x6fb) != 0) return;
    {
        Vec3i pos;
        pos.x = *(int *)(self + 0x5c);
        pos.y = *(int *)(self + 0x60);
        pos.z = *(int *)(self + 0x64);
        _ZN6Player4HurtERK7Vector3j5Fix12IiEjjj(other, &pos, 2, 0xc000, 1, 0, 1);
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 4 -- func_ov100_021435e8, 0x021435e8, size 0x1ec */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_021435e8
/* The actor riding the chain's last link (0x714). It is spawned once --
   actor 0xb3, or 0xb2 plus 0xb4 when data_0209f2d8[0] is 1 -- and its ID is
   kept at +0x6d0; afterwards it is pinned to the link every frame until it
   is gone or reaches state 5, which latches +0x6c8 and stops the updates. */
extern "C" void func_ov100_021435e8(char *c)
{
    if (*(u8 *)(c + 0x6c8) != 0) return;

    int flag = (data_0209f2d8[0] == 1);
    if (!flag) {
        if (*(s32 *)(c + 0x6d0) == 0) {
            void *a = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0xb3, 0,
                (Vector3 *)(c + 0x714), 0, *(signed char *)(c + 0xcc), -1);
            if (a != 0) *(s32 *)(c + 0x6d0) = *(s32 *)((char *)a + 4);
        } else {
            void *a = _ZN8dActor_c10FindWithIDEj(*(s32 *)(c + 0x6d0));
            if (a != 0) {
                if (*(s32 *)((char *)a + 0x440) == 5) {
                    *(u8 *)(c + 0x6c8) = 1;
                    *(s32 *)(c + 0x6d0) = 0;
                    return;
                }
                *(s32 *)((char *)a + 0x5c) = *(s32 *)(c + 0x714);
                *(s32 *)((char *)a + 0x60) = *(s32 *)(c + 0x718);
                *(s32 *)((char *)a + 0x64) = *(s32 *)(c + 0x71c);
                return;
            }
            *(u8 *)(c + 0x6c8) = 1;
            *(s32 *)(c + 0x6d0) = 0;
        }
    } else {
        if (*(s32 *)(c + 0x6d0) == 0) {
            void *a = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0xb2, *(u32 *)(c + 0x6b8) | 0x30,
                (Vector3 *)(c + 0x714), 0, *(signed char *)(c + 0xcc), -1);
            if (a != 0) *(s32 *)(c + 0x6d0) = *(s32 *)((char *)a + 4);
            _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0xb4, *(u32 *)(c + 0x6b8) | 0x30,
                (Vector3 *)(c + 0x714), 0, *(signed char *)(c + 0xcc), -1);
        } else {
            void *a = _ZN8dActor_c10FindWithIDEj(*(s32 *)(c + 0x6d0));
            if (a != 0) {
                if (*(s32 *)((char *)a + 0x440) == 5) {
                    *(u8 *)(c + 0x6c8) = 1;
                    *(s32 *)(c + 0x6d0) = 0;
                    return;
                }
                *(s32 *)((char *)a + 0x5c) = *(s32 *)(c + 0x714);
                *(s32 *)((char *)a + 0x60) = *(s32 *)(c + 0x718);
                *(s32 *)((char *)a + 0x64) = *(s32 *)(c + 0x71c);
                return;
            }
            *(u8 *)(c + 0x6c8) = 1;
            *(s32 *)(c + 0x6d0) = 0;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 5 -- func_ov100_021437d4, 0x021437d4, size 0x2d0 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_021437d4
/* The chain. The anchor is 0xfa000 behind the chomp along its rotation;
   each of the six links (positions at 0x6d8, previous positions at 0x720,
   floor heights at 0x78c) is pulled to 0x32000 from the one before it,
   bobbing on a sine of the frame counter at +0x6a0 and never dropping below
   its floor height. Was a C source; the materialised +0x6a0 address and the
   `added` copy are the shapes the ROM carries. */
extern "C" void func_ov100_021437d4(char *thisx)
{
    Vec3i in, out, cur, delta, added, v48, v54, v60;
    Vec3i *pos, *vel, *src;
    int *heights;
    int loop;
    int angOff;
    int idx, val, ang, hlen, angz;

    pos = (Vec3i *)(thisx + 0x6d8);
    vel = (Vec3i *)(thisx + 0x720);
    heights = (int *)(thisx + 0x78c);

    in.z = -0xfa000; in.x = 0; in.y = 0;
    out.x = 0; out.y = 0; out.z = 0;

    Matrix4x3_FromRotationXYZExt(data_020a0e68, *(s16 *)(thisx + 0x8c), *(s16 *)(thisx + 0x8e), *(s16 *)(thisx + 0x90));
    MulVec3Mat4x3(&in, data_020a0e68, &out);
    Vec3_Add(&added, (Vec3i *)(thisx + 0x5c), &out);

    {
        int ax = added.x, ay = added.y, az = added.z;
        int *p6a0;
        int zin;
        loop = 0;
        cur.x = ax; cur.y = ay; cur.z = az;
        *(int *)(thisx + 0x6d8) = ax;
        ay = cur.y;
        p6a0 = (int *)(int)(thisx + 0x6a0);
        *(int *)(thisx + 0x6dc) = ay;
        {
            int cz = cur.z;
            zin = 0x32000;
            *(int *)(thisx + 0x6e0) = cz;
            {
                int c = *p6a0;
                *p6a0 = c + 1;
            }
        }
        in.z = zin;
        angOff = loop;
        in.x = 0;
        in.y = 0;
    }

    for (; loop < 6; ) {
        if (loop != 0) src = (Vec3i *)((char *)pos - 0xc);
        else src = &cur;
        delta.x = vel->x + (pos->x - src->x);
        delta.z = vel->z + (pos->z - src->z);
        idx = ((unsigned short)(short)(angOff + (*(int *)(thisx + 0x6a0) << 12))) >> 4;
        val = (pos->y + vel->y) - 0x2000
            + (int)(((s64)data_02082214[idx * 2] * (-0xa000) + 0x800) >> 12);
        if (val <= *heights) val = *heights;
        delta.y = val - src->y;
        ang = _ZN4cstd5atan2E5Fix12IiES1_(delta.x, delta.z);
        hlen = Vec3_HorzLen(&delta);
        angz = (s16)(-_ZN4cstd5atan2E5Fix12IiES1_(delta.y, hlen));
        Matrix4x3_FromRotationY(data_020a0e68, ang);
        Matrix4x3_ApplyInPlaceToRotationX(data_020a0e68, angz);
        MulVec3Mat4x3(&in, data_020a0e68, &out);
        vel->x = pos->x; vel->y = pos->y; vel->z = pos->z;
        Vec3_Add(&v48, src, &out);
        pos->x = v48.x; pos->y = v48.y; pos->z = v48.z;
        Vec3_Sub(&v54, pos, vel);
        Vec3_MulScalar(&v60, &v54, 0xbb8);
        vel->x = v60.x; vel->y = v60.y; vel->z = v60.z;
        *heights = *(int *)(thisx + 0x60) - 0xc8000;
        if (*heights - pos->y > 0xc8000) *heights = pos->y;
        angOff += 0x2000;
        vel = (Vec3i *)((char *)vel + 0xc);
        pos = (Vec3i *)((char *)pos + 0xc);
        loop++;
        heights = heights + 1;
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 6 -- func_ov100_02143aa4, 0x02143aa4, size 0x3c */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02143aa4
/* The walking state's execute member (data_ov100_02147ff8, copied into
   data_ov100_021486f4.execute): advance the walk animation at full speed,
   then step the chain, the contact check and the chain-end actor. Formerly
   recovered as "RollingIronBall_Kill" / "daIbl_c::Kill, from vtable slot
   identity": the pointer-to-member constant that holds it follows daIbl_c's
   vtable directly, and that is the whole of the old claim. Was a C source. */
extern "C" int func_ov100_02143aa4(char *c)
{
    *(int *)(c + 0x368) = 4096;
    _ZN9Animation7AdvanceEv((char *)c + 0x35c);
    func_ov100_021437d4(c);
    func_ov100_0214344c(c);
    func_ov100_021435e8(c);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 7 -- func_ov100_02143ae0, 0x02143ae0, size 0x38 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02143ae0
/* The walking state's enter member (data_ov100_02148000): start the walk
   animation from data_ov100_021486ac. Was a C source. */
extern "C" int func_ov100_02143ae0(char *c)
{
    _ZN9ModelAnim7SetAnimEP8BCA_Filei5Fix12IiEj(c + 0x30c,
        *(void **)((char *)&data_ov100_021486ac + 4), 0, 0x1000, 0);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 8 -- func_ov100_02143b18, 0x02143b18, size 0x50 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02143b18
/* Enter a state: store the pair at +0x668 and call its enter member, if it
   has one. */
extern "C" int func_ov100_02143b18(ChompPmfSelf *c, ChompState *p)
{
    c->state = p;
    ChompState *q = c->state;
    if (q->enter == 0) return 1;
    return (c->*(q->enter))();
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 9 -- func_ov100_02143b68, 0x02143b68, size 0x148 */
/* -------------------------------------------------------------------------- */
// @symbol func_ov100_02143b68
/* The per-frame matrices: the body model's rotation and position, its drop
   shadow, then for each of the six links an identity matrix at the link's
   position and a smaller drop shadow. */
extern "C" void func_ov100_02143b68(char *c)
{
    M48 tmp;
    int i;
    char *mdst;
    char *rd;
    char *st;
    char *sm;
    Matrix4x3_FromRotationXYZExt(c + 0x328, *(s16 *)(c + 0x8c), *(s16 *)(c + 0x8e), *(s16 *)(c + 0x90));
    *(int *)(c + 0x34c) = *(int *)(c + 0x5c) >> 3;
    *(int *)(c + 0x350) = *(int *)(c + 0x60) >> 3;
    *(int *)(c + 0x354) = *(int *)(c + 0x64) >> 3;
    _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(c, c + 0x640, c + 0x328, 0x15e000, 0x1f4000, 0xf);
    tmp = *(M48 *)&IDENTITY_MATRIX4X3;
    i = 0;
    mdst = c + 0x370;
    rd = c;
    st = c;
    sm = c + 0x550;
    for (; i < 6; i++) {
        *(M48 *)(mdst + 0x1c) = tmp;
        *(int *)(st + 0x3b0) = *(int *)(rd + 0x6d8) >> 3;
        *(int *)(st + 0x3b4) = *(int *)(rd + 0x6dc) >> 3;
        *(int *)(st + 0x3b8) = *(int *)(rd + 0x6e0) >> 3;
        _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(c, sm, mdst + 0x1c, 0x78000, 0x1f4000, 0xf);
        mdst += 0x50;
        rd += 0xc;
        st += 0x50;
        sm += 0x28;
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 10 -- _ZN11daWanwan2_c16CleanupResourcesEv, 0x02143cb0, size 0x58 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daWanwan2_c16CleanupResourcesEv
int daWanwan2_c::CleanupResources()
{
    data_ov002_0211092c.Release();
    data_ov100_021486bc.Release();
    data_ov100_021486a4.Release();
    data_ov100_021486ac.Release();
    data_ov100_021486b4.Release();
    UnloadSilverStarAndNumber();
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 11 -- _ZN11daWanwan2_c16OnPendingDestroyEv, 0x02143d08, size 0x4 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daWanwan2_c16OnPendingDestroyEv
void daWanwan2_c::OnPendingDestroy()
{
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 12 -- _ZN11daWanwan2_c6RenderEv, 0x02143d0c, size 0x58 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daWanwan2_c6RenderEv
/* recovered: real C++ method over the typed model members. Five of the six
   link models are drawn. */
int daWanwan2_c::Render()
{
    mModelAnim.Render((Vector3 *)&mScaleX);
    for (int i = 0; i < 5; i++)
        mModels[i].Render(0);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 13 -- _ZN11daWanwan2_c8BehaviorEv, 0x02143d64, size 0x324 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daWanwan2_c8BehaviorEv
/* While the +0x6a6 timer runs the chomp's scale eases back to 1.0 and it
   stands still; otherwise it runs the current state's execute member, gets
   0x14 of vertical speed whenever the floor probe hits, spawns actor 0x120
   every 0xc8 frames when data_0209f2d8[0] is 1, and turns toward the next
   node of its path, advancing a node when within 0x190 of it. */
int daWanwan2_c::Behavior()
{
    char *c = (char *)((ChompPmfSelf *)this);
    DecIfAbove0_Short((unsigned short *)(c + 0x6ca));
    DecIfAbove0_Short((unsigned short *)(c + 0x6a8));
    if (DecIfAbove0_Short((unsigned short *)(c + 0x6a6)) != 0) {
        _Z14ApproachLinearRiii(*(int *)(c + 0x80), 0x1000, 0x500);
        *(int *)(c + 0x88) = *(int *)(c + 0x80);
        *(int *)(c + 0x84) = *(int *)(c + 0x88);
        func_ov100_02143b68(c);
        *(int *)(c + 0x98) = 0;
        _ZN8dActor_c9UpdatePosEP5dCc_c(((dActor_c *)this), (dCc_c *)(c + 0x110));
        if (func_ov100_02143370(c) != 0) {
            *(int *)(c + 0xa0) = 0;
        }
        _ZN5dCc_c5ClearEv(c + 0x110);
        _ZN5dCc_c6UpdateEv(c + 0x110);
        return 1;
    }

    *(int *)(c + 0xa0) = -0x3c000;

    {
        ChompState *q = *(ChompState **)(c + 0x668);
        if (q->execute != 0) {
            (((ChompPmfSelf *)this)->*(q->execute))();
        }
    }

    *(int *)(c + 0x98) = 0x17000;
    _ZN8dActor_c9UpdatePosEP5dCc_c(((dActor_c *)this), (dCc_c *)(c + 0x110));

    if (func_ov100_02143370(c) != 0) {
        if (*(unsigned short *)(c + 0x6a8) == 0) {
            func_02012694(0x39, c + 0x74);
        }
        *(int *)(c + 0xa8) = 0x14000;
        _ZN8dActor_c15HugeLandingDustEb(((dActor_c *)this), true);
    }

    int flag = (data_0209f2d8[0] == 1);
    if (flag != 0 && *(unsigned short *)(c + 0x6ca) == 0) {
        int q16 = __aeabi_idiv(0x10000, *(int *)(c + 0x6b4));
        short spd = (short)q16;
        dActor_c *pl = _ZN8dActor_c13ClosestPlayerEv(((dActor_c *)this));
        (void)pl;

        volatile Vector3 v;
        v.x = 0;
        v.y = 4;
        v.z = 0;

        dActor_c *sp = (dActor_c *)_ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(
            0x120, 2, (Vector3 *)(c + 0x5c), (const Vector3_16 *)0,
            *(signed char *)(c + 0xcc), -1);
        if (*(unsigned short *)(c + 0x6a8) == 0) {
            func_02012694(0x3a, c + 0x74);
        }
        if (sp != 0) {
            *(short *)((char *)sp + 0x92) = 0;
            *(short *)((char *)sp + 0x94) = spd;
            *(short *)((char *)sp + 0x96) = 0;
            int vx = v.x;
            int vy = v.y;
            *(int *)((char *)sp + 0xa4) = vx << 12;
            *(int *)((char *)sp + 0xa8) = vy << 12;
            *(int *)((char *)sp + 0xac) = vx << 12;
        }
        *(unsigned short *)(c + 0x6ca) = 0xc8;
    }

    {
        char path[8];
        Vector3 node;
        Vector3 diff;
        _ZN7PathPtrC1Ev(path);
        _ZN7PathPtr6FromIDEj(path, *(unsigned int *)(c + 0x6ac));
        _ZNK7PathPtr7GetNodeER7Vector3j(path, node, *(unsigned int *)(c + 0x6b4));

        Vec3_Sub(&diff, (Vector3 *)(c + 0x5c), &node);

        if (LenVec3(&diff) < 0x190000) {
            *(int *)(c + 0x6b4) += 1;
            if (*(int *)(c + 0x6b4) >= *(int *)(c + 0x6b0)) {
                *(int *)(c + 0x6b4) = 0;
            }
            _ZNK7PathPtr7GetNodeER7Vector3j(path, node, *(unsigned int *)(c + 0x6b4));
        }

        short ang = Vec3_HorzAngle((Vector3 *)(c + 0x5c), &node);
        *(short *)(c + 0x6a4) = ang;

        ApproachAngle((short *)(c + 0x94), *(short *)(c + 0x6a4), 0x10, 0x20, 0x500);

        *(short *)(c + 0x8e) = *(short *)(c + 0x94);

        func_ov100_02143b68(c);
        _ZN5dCc_c5ClearEv(c + 0x110);

        dActor_c *p = _ZN8dActor_c13ClosestPlayerEv(((dActor_c *)this));
        if (p != 0 && *(unsigned char *)((char *)p + 0x6fb) == 0) {
            _ZN5dCc_c6UpdateEv(c + 0x110);
        }
        return 1;
    }
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 14 -- _ZN11daWanwan2_c13InitResourcesEv, 0x02144088, size 0x24c */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daWanwan2_c13InitResourcesEv
int daWanwan2_c::InitResources()
{
    Model::LoadFile(data_ov002_0211092c);
    mModelAnim.SetFile((BMD_File *)Model::LoadFile(data_ov100_021486bc), 1, -1);
    Model::LoadFile(data_ov100_021486a4);
    Animation::LoadFile(data_ov100_021486ac);
    Animation::LoadFile(data_ov100_021486b4);
    LoadSilverStarAndNumber();

    {
        int i = 0;
        Model *model = mModels;
        do {
            model->SetFile(*(BMD_File **)((char *)&data_ov100_021486a4 + 4), 1, -1);
            i++;
            model++;
        } while (i < 6);
    }

    mShadowModel.InitCylinder();
    {
        int i = 0;
        ShadowModel *shadow = mShadowModels;
        do {
            shadow->InitCylinder();
            i++;
            shadow++;
        } while (i < 6);
    }

    unk_6ac = param1 & 0xff;
    unk_6b8 = (param1 >> 8) & 0xf;
    if (unk_6ac == 0xff)
        unk_6ac = 0;

    {
        PathPtr path;
        path.FromID(unk_6ac);
        unk_6b0 = path.NumNodes();
    }

    mVertAccel = -0x2000;
    mTerminalVelocity = -0x3c000;

    {
        Vector3 offset;
        offset.x = data_ov100_02148008[0];
        offset.y = data_ov100_02148008[1];
        offset.z = data_ov100_02148008[2];
        _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(
            &mdCcAcPos_c, this, offset,
            0xaa000, 0x140000, 0x200004, 0x6010);
    }

    unk_6c9 = 0x1f;
    unk_6cc = 3;

    mAngleY = mPrevAngleY;
    *(s16 *)((char *)this + 0x6a4) = mAngleY;

    unk_6d0 = 0;
    unk_6d4 = 0;

    {
        int i = 0;
        char *position = (char *)this;
        do {
            *(s32 *)(position + 0x6d8) = mPosX;
            i++;
            *(s32 *)(position + 0x6dc) = mPosY;
            *(s32 *)(position + 0x6e0) = mPosZ;
            position += sizeof(Vector3);
        } while (i < 6);
    }

    *(s16 *)((char *)this + 0x6ca) = 0xc8;

    {
        PathPtr path;
        path.FromID(unk_6ac);
        unk_6b4 = 1;
        path.GetNode(*(Vector3 *)&mPosX, unk_6b4);
    }

    /* Preserve the ROM's materialized read-modify-write address for mPosY. */
    *(s32 *)((int)this + 0x60) += 0x64000;
    mScaleX = 0x1000;
    mScaleY = 0x1000;
    mScaleZ = 0x1000;

    /* Start in the walking state. */
    func_ov100_02143b18((ChompPmfSelf *)this, &data_ov100_021486f4);
    return 1;
}

/* -------------------------------------------------------------------------- */
/* ROM ordinal 15 -- _ZN11daWanwan2_c16OnAimedAtWithEggEv, 0x021442d4, size 0x8 */
/* -------------------------------------------------------------------------- */
// @symbol _ZN11daWanwan2_c16OnAimedAtWithEggEv
/* recovered from vtable slot identity (slot 29); historical alias
   UnchainedChomp_OnAimedAtWithEgg. */
s32 daWanwan2_c::OnAimedAtWithEgg()
{
    return 0;
}

/* daWanwan2_c_classInit (0x021442dc..0x021443f4) is not in this file. Written
 * as `return new daWanwan2_c;` it comes out 0xa4 bytes for the ROM's 0x118:
 * the ROM constructs mUnk_6d8 and mUnk_720 through
 * __cxa_vec_ctor(..., func_0203d384, _ZN7Vector3D1Ev) and mUnk_768 through
 * __cxa_vec_ctor(..., func_0203d73c, _ZN8Vector3sD1Ev), with an empty
 * constructor function, and types.h's Vector3 and Vector3s declare no
 * constructor, so the implicit one never emits those three calls. The
 * hand-built factory in src/d_a_wanwan2.cpp still reproduces them. */
