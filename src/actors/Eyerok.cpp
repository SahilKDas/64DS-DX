//cpp
/* Eyerok, the two-handed pyramid boss (.text 0x0211603c..0x0211a2e4,
 * 59 functions). ROM RTTI daIwante_c (_ZTS10daIwante_c ov066:0x0211ad30);
 * this tree keeps the coined name.
 *
 * The unit's two destructors (D1 0x02115ee0, D0 0x02115f84) stay in their
 * own files: ~Eyerok is the key function, and defining it here would emit
 * the vtable and RTTI as _ZTS6Eyerok, a name the ROM does not have. The
 * registry factory daIwante_c_classInit (0x0211a370, src/d_a_iwante.cpp)
 * is not folded: two free functions sit between it and this unit.
 *
 * Source is ROM-ascending under defer_codegen off. Do not reorder.
 *
 * Leftover: the func_ov066 helpers keep linker names, and most of them still
 * address the object by raw offset. State dispatch stays an incomplete-class
 * pointer-to-member. EVec3 and M48 are plain word structs standing in for
 * Vector3 and Matrix4x3. Vec4 is an unused stack object with a destructor.
 */

/* Turns off deferred codegen, which does two things at once here: it makes
 * the bracketed opt_common_subs / opt_strength_reduction pair around
 * _ZN6Eyerok8BehaviorEv bind to that member alone instead of leaking
 * file-wide, and it flips .text emission from reverse-source to source
 * order -- which is why this file is written ROM-ascending. */
#pragma defer_codegen off
/* Includes. decl_common.h is DELIBERATELY NOT included: it types
 * func_ov066_02119454 as returning void, and this TU *defines* that member --
 * conforming the definition to decl_common.h's spelling costs the match
 * (measured with tools/match.py: int -> MATCH, void -> no match). Every
 * symbol decl_common.h would have supplied is declared below instead, with
 * the spelling the shards actually matched under. */
#include "Eyerok.h"
#include "types.h"
#include "dBgW.h"
#include "common.h"
#include "decl_Message.h"
#include "SharedFilePtr.h"
#include "TextureSequence.h"
#include "Player.h"
#include "Message.h"

/* EVec3 is three plain words: unlike a Vector3, a local of it has no destructor. */
struct EVec3 { int x, y, z; };
struct C;
typedef int (C::*PMF)();
struct State { char pad[8]; PMF fn; };
struct CLPS_Block;

extern "C" {
/* ---- ov066 .bss: 8-byte SharedFilePtr slots (0x0211ae14..0x0211aebc) ---- */
extern int data_ov066_0211ae14[];
extern int data_ov066_0211ae1c[];
extern int data_ov066_0211ae24[];
extern int data_ov066_0211ae2c[];
extern int data_ov066_0211ae34[];
extern int data_ov066_0211ae3c[];
extern int data_ov066_0211ae44[];
extern int data_ov066_0211ae4c[];
extern int data_ov066_0211ae54[];
extern int data_ov066_0211ae5c[];
extern int data_ov066_0211ae64[];
extern int data_ov066_0211ae6c[];
extern int data_ov066_0211ae74[];
extern int data_ov066_0211ae7c[];
extern int data_ov066_0211ae84[];
extern int data_ov066_0211ae8c[];
extern int data_ov066_0211ae94[];
extern int data_ov066_0211ae9c[];
extern int data_ov066_0211aea4[];
extern int data_ov066_0211aeac[];
extern int data_ov066_0211aeb4[];
extern int data_ov066_0211aebc[];

/* ---- ov066 .bss / .data byte flags and counters ---- */
extern unsigned char data_ov066_0211ae00;
extern unsigned char data_ov066_0211ae04;
extern unsigned char data_ov066_0211ae08;
extern unsigned char data_ov066_0211ae0c;
extern unsigned char data_ov066_0211ae10;
extern unsigned char data_ov066_0211abe0;
extern int data_ov066_0211abe4;
extern int data_ov066_0211ad18[];

/* ---- ov066 .bss state descriptors, 0x10 bytes each ---- */
extern char data_ov066_0211afcc;
extern char data_ov066_0211afdc;
extern char data_ov066_0211afec;
extern char data_ov066_0211affc;
extern char data_ov066_0211b00c;
extern char data_ov066_0211b01c;
extern char data_ov066_0211b02c;
extern char data_ov066_0211b03c;
extern char data_ov066_0211b04c;
extern int data_ov066_0211b05c[];
extern char data_ov066_0211b06c;
extern char data_ov066_0211b07c;
extern char data_ov066_0211b08c;
extern int data_ov066_0211b09c[];
extern char data_ov066_0211b0ac;
extern char data_ov066_0211b0bc;
extern char data_ov066_0211b0cc;
extern char data_ov066_0211b0dc;
extern char data_ov066_0211b0ec;

/* ---- ov025 .data: CLPS blocks. ov025 is the overlay resident below ov066
 *      at these addresses (tools/overlay_residency.py rules out every other
 *      candidate) ---- */
extern CLPS_Block data_ov025_02112c08;
extern CLPS_Block data_ov025_02112c88;
extern CLPS_Block data_ov025_02112ca8;
extern CLPS_Block data_ov025_02112cc8;
extern CLPS_Block data_ov025_02112d48;

/* ---- arm9 data ---- */
extern int data_0209e650;
extern void *data_0209f318;
extern int data_020a0e68[];

/* ---- arm9 helpers (unmangled ROM names) ---- */
extern int AngleDiff(int a, int b);
extern int ApproachAngle(s16 *angle, int target, int a, int b, int max);
extern unsigned short DecIfAbove0_Short(unsigned short *p);
extern void Matrix4x3_ApplyInPlaceToRotationXYZExt(void *m, int x, int y, int z);
extern void Matrix4x3_FromRotationY(void *m, short ang);
extern void Matrix4x3_FromTranslation(void *m, int x, int y, int z);
extern void MulVec3Mat4x3(void *a, void *m, void *b);
extern int RandomIntInternal(int *seed);
extern int Vec3_ApproachHorz(void *out, void *a, int maxStep);
extern int Vec3_Dist(const void *a, const void *b);
extern void Vec3_Asr(void *d, void *s, int sh);
extern int Vec3_HorzDist(const void *a, const void *b);
extern s16 Vec3_HorzAngle(const void *a, const void *b);
extern void func_0200d8c8(void *cam, void *v, int strength);
extern void func_020092c4(void *cam, void *out, void *target);
extern void func_02011cfc(void);
extern void func_02011d2c(void);
extern void func_02012694(int a, void *p);
extern void func_020393c4(void *p, void *v);
extern void func_020393d4(void *p, void *v);
extern void func_020398fc(void *p);

/* ---- arm9 / ov002 methods, mangled ROM spelling ---- */
extern void _Z14ApproachLinearR7Vector3RKS_5Fix12IiE(void *out, void *tgt, int step);
extern void _Z14ApproachLinearRiii(int *r, int target, int step);
extern void _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(void *self, void *kcl, void *mtx, int fix, short s, void *clps);
extern void _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(void *self, void *actor, Vector3 *v, s32 f1, s32 f2, u32 a, u32 b);
extern void _ZN11ShadowModel12InitCylinderEv(void *self);
extern void _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(void *self, void *bca, int a, int b, int fix, unsigned short t);
extern void _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(void *self, void *btp, int a, int fix, unsigned int b);
extern void _ZN15TextureSequence8LoadFileER13SharedFilePtr(void *sfp);
extern void *_ZN5Model8LoadFileER13SharedFilePtr(void *sfp);
extern void _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(unsigned int a, int b);
extern void _ZN5Sound22LoadAndSetMusic_Layer3Ej(unsigned int a);
extern void _ZN5Sound22StopLoadedMusic_Layer3Ev(void);
extern void _ZN6Camera9SetFlag_3Ev(void *cam);
extern void _ZN6Player16IncMegaKillCountEv(void *p);
extern void _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(void *sfp);
extern void _ZN7fBase_c18MarkForDestructionEv(void *self);
extern void _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(unsigned int a, int x, int y, int z);
extern u32 _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(u32 a, u32 b, int x, int y, int z, const void *v, void *cb);
extern void *_ZN8dActor_c10FindWithIDEj(unsigned int id);
extern void *_ZN8dActor_c13ClosestPlayerEv(void *self);
extern void _ZN8dActor_c15HugeLandingDustEb(void *self, int b);
extern void _ZN8dActor_c16TriplePoofDustAtERK7Vector3(void *self, const void *v);
extern int _ZN8dActor_c18HorzAngleToCPlayerEv(void *self);
extern void *_ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(u32 id, u32 b, Vector3 *pos, void *p, int e, int f);
extern u8 _ZN8dActor_c9TrackStarEjj(void *actor, u32 a, u32 b);
extern void _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(void *self, void *sm, void *m, int rad, int h, unsigned int u);
extern int _ZN9Animation8FinishedEv(void *self);
extern void _ZN9Animation8LoadFileER13SharedFilePtr(void *sfp);
extern int _ZN9ModelBase7SetFileEP8BMD_Fileii(void *self, void *bmd, int a, int b);
extern int _ZNK9Animation13GetFrameCountEv(void *self);

/* ---- the dBgW callback veneer just past this unit (0x0211a35c, its own
 *      src/ file) ---- */
extern int func_ov066_0211a35c(void *a, void *b, void *c);

/* ---- this TU's own members, forward-declared: the file is written
 *      ROM-ascending, so a member that calls one defined further down
 *      needs a declaration first ---- */
extern void func_ov066_021194a4(char *c);
extern void func_ov066_021194fc(char *c);
extern int func_ov066_02119454(void *c, void *p);
}

typedef struct { int w[12]; } M48;

// @symbol func_ov066_0211603c
extern "C" {
int func_ov066_0211603c(char *self)
{
    enum Bool { FALSE, TRUE };
    char *actor;
    int flags;
    int hit;
    unsigned int id;
    u16 type;
    enum Bool is_player;

    id = *(unsigned int *)(self + 0x344);
    if (id == 0)
        goto fail;

    actor = (char *)_ZN8dActor_c10FindWithIDEj(id);
    if (actor == 0)
        return 0;

    if (AngleDiff(*(s16 *)(self + 0x8e), _ZN8dActor_c18HorzAngleToCPlayerEv(self)) >= 0x4000)
        return 0;

    type = *(u16 *)(actor + 0xc);
    hit = 0;
    flags = *(int *)(self + 0x340);
    is_player = (enum Bool)(type == 0xbf);
    if (is_player == FALSE)
        goto other;

    if (*(u8 *)(actor + 0x6f9) == 1) {
        (*(s8 *)(((int)self + 0x4d8)))--;
        hit = 1;
    }
    if (flags & 0x10) {
        (*(s8 *)(((int)self + 0x4d8)))--;
        if (*(s8 *)(self + 0x4d8) <= 0)
            _ZN6Player16IncMegaKillCountEv(actor);
        hit = 1;
    }

other:
    if (hit == 0) {
        if (flags & 0x427e0) {
            if (flags & 0x40) {
                if (*(int *)(actor + 8) == 2)
                    (*(s8 *)(((int)self + 0x4d8)))--;
            }
            if (flags & 0x40000)
                *(s16 *)(self + 0x4d4) = 1;
            (*(s8 *)(((int)self + 0x4d8)))--;
            hit = 1;
        }
    }

    if (hit == 0)
        goto fail;

    if (*(s8 *)(self + 0x4d8) > 0) {
        if (*(int *)(self + 0x49c) == 2) {
            _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
                self + 0x360, (void *)data_ov066_0211ae5c[1], 4, 0x40000000, 0x1000, 0);
            _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(
                self + 0x448, (void *)data_ov066_0211ae3c[1], 0x40000000, 0x1000, 0);
        } else {
            _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
                self + 0x360, (void *)data_ov066_0211ae84[1], 4, 0x40000000, 0x1000, 0);
            _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(
                self + 0x448, (void *)data_ov066_0211aebc[1], 0x40000000, 0x1000, 0);
        }
        func_02012694(0x141, self + 0x74);
        return 1;
    }

    {
        u8 x = data_ov066_0211ae08;
        u8 y = data_ov066_0211abe0;
        int side = *(int *)(self + 0x49c);
        data_ov066_0211abe0 = (u8)(y ^ side);
        data_ov066_0211ae08 = (u8)(x + 1);
        data_ov066_0211abe4 = -3;
    }
    func_02012694(0x142, self + 0x74);
    func_ov066_02119454(self, &data_ov066_0211b07c);
    return 2;

fail:
    return 0;
}
}

// @symbol func_ov066_021162e8
extern "C" {
void func_ov066_021162e8(void *c)
{
    int *p = (int *)((char *)c + 0x338);
    *p |= 2;
    *(int *)((char *)c + 0x324) = 0x64000;
    *(int *)((char *)c + 0x328) = 0x64000;
    data_ov066_0211ad18[0] = 0;
    data_ov066_0211ad18[1] = 0x20000;
    data_ov066_0211ad18[2] = -0x10000;
}
}

// @symbol func_ov066_0211632c
extern "C" {
void func_ov066_0211632c(void *thiz)
{
    char *self = (char *)thiz;
    int *p338 = (int *)(self + 0x338);
    *p338 &= ~2;
    *(int *)(self + 0x324) = 0x9c000;
    *(int *)(self + 0x328) = 0x164000;
    if (*(int *)(self + 0x49c) == 2)
        data_ov066_0211ad18[0] = 0x55000;
    else
        data_ov066_0211ad18[0] = -0x55000;
    data_ov066_0211ad18[1] = -0xc0000;
    data_ov066_0211ad18[2] = 0x80000;
}
}

// @symbol func_ov066_02116390
extern "C" {
void func_ov066_02116390(void *thiz)
{
    char *c = (char *)thiz;
    DecIfAbove0_Short((unsigned short *)(c + 0x66c));
    if (*(unsigned short *)(c + 0x66c) != 0)
        return;
    if (*(unsigned char *)(c + 0x66e) == 0) {
        if (*(int *)(c + 0x49c) == 2)
            _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x448, (void *)data_ov066_0211ae2c[1], 0x40000000, 0x1000, 0);
        else
            _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x448, (void *)data_ov066_0211ae9c[1], 0x40000000, 0x1000, 0);
        *(unsigned short *)(c + 0x66c) = (((unsigned int)RandomIntInternal(&data_0209e650) >> 8) & 0xf) * 2 + 0x32;
    } else {
        if (*(int *)(c + 0x49c) == 2)
            _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x448, (void *)data_ov066_0211ae3c[1], 0x40000000, 0x1000, 0);
        else
            _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x448, (void *)data_ov066_0211aebc[1], 0x40000000, 0x1000, 0);
        *(unsigned short *)(c + 0x66c) = 8;
    }
    *(unsigned char *)(((int)c + 0x66e)) ^= 1;
}
}

// @symbol func_ov066_021164ec
extern "C" {
void func_ov066_021164ec(void *thiz)
{
    char *c = (char *)thiz;
    if (*(int *)(c + 0x498) != 0) return;
    if ((unsigned short)(*(int *)(c + 0x3b8) >> 0xc) != 0) return;
    *(int *)(((int)c + 0x33c)) |= 0x427f0;
    *(int *)(c + 0xb0) = 0x10000000;
    *(int *)(((int)c + 0x338)) |= 2;
    if (*(int *)(c + 0x49c) == 2) {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(c + 0x360, (void *)data_ov066_0211ae64[1], 4, 0, 0x1000, 0);
    } else {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(c + 0x360, (void *)data_ov066_0211ae44[1], 4, 0, 0x1000, 0);
    }
    *(int *)(c + 0x498) = 1;
}
}

// @symbol func_ov066_021165cc
extern "C" {
void func_ov066_021165cc(void *thiz)
{
    char *c = (char *)thiz;
    if (*(int *)(c + 0x49c) == 2) {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
            c + 0x360, (void *)data_ov066_0211ae54[1], 4, 0x40000000, 0x1000, 0);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(
            c + 0x448, (void *)data_ov066_0211ae3c[1], 0x40000000, 0x1000, 0);
    } else {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(
            c + 0x360, (void *)data_ov066_0211ae94[1], 4, 0x40000000, 0x1000, 0);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(
            c + 0x448, (void *)data_ov066_0211aebc[1], 0x40000000, 0x1000, 0);
    }
    *(int *)(c + 0x3bc) = 0x1000;
    {
        int *p = (int *)(((int)c + 0x33c));
        *p = *p & 0xfffbd82f;
        *(int *)(c + 0xb0) = 0;
    }
}
}

// @symbol func_ov066_021166c8
extern "C" {
void func_ov066_021166c8(void *thiz)
{
    unsigned char *c = (unsigned char *)thiz;

    if (*(int *)(c + 0x49c) == 2) {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(c + 0x360, (void *)data_ov066_0211ae54[1], 4, 0x40000000, 0x1000, 0);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x448, (void *)data_ov066_0211ae2c[1], 0x40000000, 0x1000, 0);
    } else {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(c + 0x360, (void *)data_ov066_0211ae94[1], 4, 0x40000000, 0x1000, 0);
        _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(c + 0x448, (void *)data_ov066_0211ae9c[1], 0x40000000, 0x1000, 0);
    }

    if (((dBgW *)(c + 0x674))->IsEnabled() != 0)
        ((dBgW *)(c + 0x674))->Disable();

    if (*(int *)(c + 0x49c) == 1) {
        _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(c + 0x674, (void *)data_ov066_0211ae34[1], c + 0x83c, 0x199,
                                   *(short *)(c + 0x8e), &data_ov025_02112cc8);
    } else {
        _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(c + 0x674, (void *)data_ov066_0211ae1c[1], c + 0x83c, 0x199,
                                   *(short *)(c + 0x8e), &data_ov025_02112c88);
    }

    func_020393d4(c + 0x674, (void *)&dBgW::UpdatePosWithTransform);
    func_020393c4(c + 0x674, (void *)func_ov066_0211a35c);
    func_020398fc(c + 0x674);
    ((dBgW *)(c + 0x674))->Enable((dActor_c *)(c));

    {
        int n = _ZNK9Animation13GetFrameCountEv(c + 0x3b0);
        *(int *)(c + 0x3b8) = (int)(((unsigned int)((n - 1) << 0x10)) >> 4);
        *(int *)(c + 0x3bc) = -0x1000;
    }
}
}

// @symbol func_ov066_021168b0
extern "C" {
int func_ov066_021168b0(void *thiz)
{
    char *c = (char *)thiz;
    if (*(int *)(c + 0x4a0) == 0) {
        if (data_ov066_0211ae0c != 3) return 0;
        *(int *)(c + 0x4a0) = 1;
        data_ov066_0211ae0c = 0;
    }
    return 1;
}
}

// @symbol func_ov066_021168ec
extern "C" {
int func_ov066_021168ec(void *c)
{
    unsigned char m;
    if (*(int *)((char *)c + 0x9c) != 0) return 0;
    m = data_ov066_0211ae04;
    if (m == 3) {
        if (*(void **)((char *)c + 0x48c) != (void *)&data_ov066_0211b06c) {
            func_ov066_02119454(c, (void *)&data_ov066_0211b06c);
            return 3;
        }
    }
    if ((unsigned char)(m + 0xfc) <= 5
        && *(void **)((char *)c + 0x48c) == (void *)&data_ov066_0211b06c) {
        if (m == 4) { func_ov066_02119454(c, (void *)&data_ov066_0211b08c); return data_ov066_0211ae04; }
        if (m == 5) { func_ov066_02119454(c, (void *)&data_ov066_0211b0bc); return data_ov066_0211ae04; }
        if (m == 6) { func_ov066_02119454(c, (void *)&data_ov066_0211b0ec); return data_ov066_0211ae04; }
        if (m == 7) { func_ov066_02119454(c, (void *)&data_ov066_0211afec); return data_ov066_0211ae04; }
        if (m == 8) { func_ov066_02119454(c, (void *)&data_ov066_0211b01c); return data_ov066_0211ae04; }
        if (m == 9) { func_ov066_02119454(c, (void *)&data_ov066_0211b04c); return data_ov066_0211ae04; }
    }
    return 0;
}
}

// @symbol func_ov066_02116a68
extern "C" {
int func_ov066_02116a68(void *self)
{
    volatile int dummy[3];
    (void)dummy;
    char *p = (char *)_ZN8dActor_c13ClosestPlayerEv(self);
    int dist = *(int *)(p + 0x64);
    int lo = (int)0xff3ae000;
    if (dist < lo) return lo;
    int lo2 = -(int)0xb50000;
    if (dist < lo2) return lo2;
    int hi = (int)0xff598000;
    if (dist >= hi) return 0;
    return hi;
}
}

// @symbol func_ov066_02116ac4
extern "C" {
void func_ov066_02116ac4(void *thiz, int strength)
{
    char *c = (char *)thiz;
    volatile int s0, s1, s2;
    func_0200d8c8(data_0209f318, c + 0x5c, strength);
    s0 = *(int *)(c + 0x5c);
    s1 = *(int *)(c + 0x60);
    s2 = *(int *)(c + 0x64);
    if (*(int *)(c + 0x49c) == 1)
        *(int *)(((long)c + 0x5c)) -= 0x80000;
    else
        *(int *)(((long)c + 0x5c)) += 0x80000;
    *(int *)(((long)c + 0x64)) += 0x80000;
    _ZN8dActor_c15HugeLandingDustEb(c, 1);
    func_02012694(0x143, c + 0x74);
    *(int *)(c + 0x5c) = s0;
    *(int *)(c + 0x60) = s1;
    *(int *)(c + 0x64) = s2;
}
}

// @symbol func_ov066_02116b78
extern "C" {
int func_ov066_02116b78(void *thiz)
{
    char *c = (char *)thiz;
    int b1 = 0x320;
    int lo = -0x320;
    int ip = *(int *)(c + 0x49c);
    if (ip == 2) { b1 = 0x384; lo = -0x384; }
    unsigned char flag = data_ov066_0211abe0;
    int v = *(int *)(c + 0x5c);
    if (flag != 3) lo -= 0x50;
    lo = lo << 0xc;
    if (v < lo) {
        *(int *)(c + 0x5c) = lo;
        *(int *)(c + 0x98) = 0;
        return 1;
    }
    b1 = 0x320;
    if (ip == 1) b1 = 0x384;
    if (flag != 3) b1 += 0x50;
    b1 = b1 << 0xc;
    if (v > b1) {
        *(int *)(c + 0x5c) = b1;
        *(int *)(c + 0x98) = 0;
        return 1;
    }
    int z = *(int *)(c + 0x64);
    int n = 0xff0ff000;
    if (z < n) {
        *(int *)(c + 0x64) = n;
        *(int *)(c + 0x98) = 0;
        return 1;
    }
    n = 0xff8c6000;
    if (flag != 3) n += 0x8c000;
    if (z > n) {
        *(int *)(c + 0x64) = n;
        *(int *)(c + 0x98) = 0;
        return 1;
    }
    return 0;
}
}

// @symbol func_ov066_02116c6c
extern "C" {
int func_ov066_02116c6c(char *c)
{
    if ((unsigned int)((unsigned int)(*(unsigned int *)(c + 0x3b8) << 4) >> 0x10) > 0xc) {
        *(int *)(c + 0x98) = 0;
    }
    if (_ZN9Animation8FinishedEv(c + 0x3b0) != 0) {
        _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x7c, *(int *)(c + 0x5c), *(int *)(c + 0x60), *(int *)(c + 0x64));
        _ZN8Particle6System9NewSimpleEj5Fix12IiES2_S2_(0x7d, *(int *)(c + 0x5c), *(int *)(c + 0x60), *(int *)(c + 0x64));
        {
            EVec3 v;
            v.x = *(int *)(c + 0x5c);
            v.y = *(int *)(c + 0x60);
            v.z = *(int *)(c + 0x64);
            _ZN8dActor_c16TriplePoofDustAtERK7Vector3(c, &v);
        }
        func_02012694(0x146, c + 0x74);
        _ZN7fBase_c18MarkForDestructionEv(c);
    }
    return 1;
}
}

// @symbol func_ov066_02116d14
extern "C" {
int func_ov066_02116d14(char *c)
{
    *(int *)(c + 0x494) = 0;
    *(int *)(c + 0x498) = 0;
    *(short *)(c + 0x4d0) = 0;
    *(int *)(c + 0x4a0) = 0;
    *(int *)(c + 0x98) = -0xa000;
    if (*(int *)(c + 0x49c) == 2) {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(c + 0x360, (void *)data_ov066_0211aea4[1], 4, 0x40000000, 0x1000, 0);
    } else {
        _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(c + 0x360, (void *)data_ov066_0211ae8c[1], 4, 0x40000000, 0x1000, 0);
    }
    return 1;
}
}

// @symbol func_ov066_02116db0
extern "C" {
int func_ov066_02116db0(void *thiz)
{
    char *c = (char *)thiz;
    EVec3 in, out;
    s16 ang;
    int r;

    switch (*(int *)(c + 0x4a0)) {
    case 0:
        func_ov066_021166c8(c);
        *(int *)(c + 0x4a0) = 1;
        break;

    case 1:
        if (*(int *)(c + 0x498) == 0) {
            char *p = (char *)_ZN8dActor_c13ClosestPlayerEv(c);
            if (p != 0) {
                EVec3 *pp = (EVec3 *)(((int)p + 0x5c));
                *(int *)(c + 0x4bc) = pp->x;
                *(int *)(c + 0x4c0) = pp->y;
                *(int *)(c + 0x4c4) = pp->z;

                in.x = 0;
                in.y = 0;
                in.z = 0;
                out.x = 0;
                out.y = 0;
                out.z = 0;
                in.z = 0x3e8000;

                ang = Vec3_HorzAngle(c + 0x5c, c + 0x4bc);
                Matrix4x3_FromRotationY(data_020a0e68, ang);
                MulVec3Mat4x3(&in, data_020a0e68, &out);

                *(int *)(((int)c + 0x4bc)) += out.x;
                *(int *)(((int)c + 0x4c4)) += out.z;
            }

            if ((unsigned short)(*(int *)(c + 0x3b8) >> 0xc) == 0)
                func_02012694(0x140, c + 0x74);

            if ((unsigned short)(*(int *)(c + 0x3b8) >> 0xc) == 0) {
                func_ov066_021164ec(c);
                func_02012694(0x144, c + 0x74);
            }

            ang = Vec3_HorzAngle(c + 0x5c, c + 0x4bc);
            ApproachAngle((s16 *)(c + 0x8e), ang, 2, 0x400, 0x200);
        }

        if (*(int *)(c + 0x498) == 1) {
            r = func_ov066_0211603c(c);
            if (r != 0) {
                if (r == 1)
                    *(int *)(c + 0x4a0) = 2;
                break;
            }
            Vec3_ApproachHorz(c + 0x5c, c + 0x4bc, 0x37000);
            if (func_ov066_02116b78(c) == 1 || Vec3_HorzDist(c + 0x5c, c + 0x4bc) <= 0x37000) {
                func_ov066_021165cc(c);
                *(int *)(c + 0x4a0) = 3;
            }
        }
        break;

    /* case 3 body before case 2 to match ROM placement */
    case 3:
        if (_ZN9Animation8FinishedEv(c + 0x3b0) != 0) {
            if (((dBgW *)(c + 0x674))->IsEnabled() != 0)
                ((dBgW *)(c + 0x674))->Disable();
            if (*(int *)(c + 0x49c) == 1)
                _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
                    c + 0x674, (void *)data_ov066_0211ae14[1], c + 0x83c, 0x199,
                    *(s16 *)(c + 0x8e), &data_ov025_02112c08);
            else
                _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
                    c + 0x674, (void *)data_ov066_0211aeac[1], c + 0x83c, 0x199,
                    *(s16 *)(c + 0x8e), &data_ov025_02112d48);
            func_020393d4(c + 0x674, (void *)&dBgW::UpdatePosWithTransform);
            func_020393c4(c + 0x674, (void *)func_ov066_0211a35c);
            func_020398fc(c + 0x674);
            ((dBgW *)(c + 0x674))->Enable((dActor_c *)c);
            *(int *)(c + 0x4a0) = 4;
        }
        break;

    case 2:
        if (_ZN9Animation8FinishedEv(c + 0x3b0) != 0) {
            func_ov066_021165cc(c);
            *(int *)(c + 0x4a0) = 3;
        }
        break;

    case 4:
        func_ov066_021162e8(c);
        ApproachAngle((s16 *)(c + 0x8e), 0, 2, 0x400, 0x200);
        Vec3_ApproachHorz(c + 0x5c, c + 0x4a4, 0x37000);
        if (Vec3_HorzDist(c + 0x5c, c + 0x4a4) <= 0x37000) {
            *(int *)(c + 0x5c) = *(int *)(c + 0x4a4);
            *(int *)(c + 0x60) = *(int *)(c + 0x4a8);
            *(int *)(c + 0x64) = *(int *)(c + 0x4ac);
            data_ov066_0211ae08 = 2;
            *(s16 *)(c + 0x8e) = 0;
            *(int *)(c + 0x4a0) = 5;
        }
        break;

    case 5:
        if (data_ov066_0211ae04 != 9)
            func_ov066_02119454(c, &data_ov066_0211b06c);
        break;
    }
    return 1;
}
}

// @symbol func_ov066_02117190
extern "C" {
int func_ov066_02117190(char *p)
{
    *(int *)(p + 0x494) = 0;
    *(int *)(p + 0x498) = 0;
    *(short *)(p + 0x4d0) = 0;
    *(int *)(p + 0x4a0) = 0;
    return 1;
}
}

// @symbol func_ov066_021171b0
extern "C" {
int func_ov066_021171b0(void *thiz)
{
    char *c = (char *)thiz;

    switch (*(int *)(c + 0x4a0)) {
    case 0: {
        char *p = (char *)_ZN8dActor_c13ClosestPlayerEv(c);
        if (p == 0)
            break;
        {
            EVec3 *pp = (EVec3 *)(((int)p + 0x5c));
            *(int *)(c + 0x4bc) = pp->x;
            *(int *)(c + 0x4c0) = pp->y;
            *(int *)(c + 0x4c4) = pp->z;
        }
        *(int *)(((int)c + 0x4c4)) -= 0xc8000;
        if (*(int *)(c + 0x4c4) < (int)0xff3ae000) {
            *(int *)(c + 0x4c4) = (int)0xff3ae000;
        } else if (*(int *)(c + 0x4c4) > (int)0xff8c6000) {
            *(int *)(c + 0x4c4) = (int)0xff8c6000;
        }
        if (data_ov066_0211ae0c == 1) {
            *(short *)(c + 0x94) = -0x4000;
            *(int *)(c + 0x4bc) = 0x334000;
            if (*(int *)(c + 0x49c) == 1) {
                *(int *)(((int)c + 0x4bc)) -= 0xf2000;
            }
        } else {
            *(short *)(c + 0x94) = 0x4000;
            *(int *)(c + 0x4bc) = (int)0xffe8e000;
            if (*(int *)(c + 0x49c) == 1) {
                *(int *)(((int)c + 0x4bc)) -= 0xf2000;
            }
        }
        func_02012694(0x144, c + 0x74);
        *(int *)(c + 0x4c0) = *(int *)(c + 0x4a8) + 0x1c2000;
        *(int *)(c + 0x4a0) = 1;
        break;
    }
    case 1:
        _Z14ApproachLinearR7Vector3RKS_5Fix12IiE(c + 0x5c, c + 0x4bc, 0x28000);
        if (Vec3_Dist(c + 0x5c, c + 0x4bc) > 0x28000)
            break;
        *(int *)(c + 0x5c) = *(int *)(c + 0x4bc);
        *(int *)(c + 0x60) = *(int *)(c + 0x4c0);
        *(int *)(c + 0x64) = *(int *)(c + 0x4c4);
        data_ov066_0211ae00 |= *(int *)(c + 0x49c);
        if (data_ov066_0211ae00 != 3)
            break;
        *(unsigned short *)(c + 0x4d0) = 0xa;
        if (data_ov066_0211ae0c == 1) {
            if (*(int *)(c + 0x49c) == 2)
                *(unsigned short *)(c + 0x4d0) = 0x12;
        } else {
            if (*(int *)(c + 0x49c) == 1)
                *(unsigned short *)(c + 0x4d0) = 0x12;
        }
        *(int *)(c + 0x4c0) = *(int *)(c + 0x4a8) + 0x1a000;
        *(int *)(c + 0x4a0) = 2;
        break;
    case 2:
        if (*(unsigned short *)(c + 0x4d0) != 0)
            break;
        _Z14ApproachLinearR7Vector3RKS_5Fix12IiE(c + 0x5c, c + 0x4bc, 0x32000);
        if (Vec3_Dist(c + 0x5c, c + 0x4bc) > 0x32000)
            break;
        *(int *)(c + 0x5c) = *(int *)(c + 0x4bc);
        *(int *)(c + 0x60) = *(int *)(c + 0x4c0);
        *(int *)(c + 0x64) = *(int *)(c + 0x4c4);
        func_ov066_02116ac4(c, 0x7d0000);
        *(unsigned short *)(c + 0x4d0) = 0xf;
        *(int *)(c + 0x4a0) = 3;
        break;
    case 3: {
        unsigned short st = *(unsigned short *)(c + 0x4d0);
        if (st != 0) {
            if (st != 1)
                break;
            *(int *)(c + 0xa8) = 0x7c000;
            *(int *)(c + 0x9c) = -0x14000;
            *(int *)(c + 0x98) = 0x1e000;
            *(int *)(c + 0xb0) = 0x2000000;
            break;
        }
        if (*(int *)(c + 0x9c) == 0)
            break;
        if (*(int *)(c + 0x4a8) < *(int *)(c + 0x60))
            break;
        *(int *)(c + 0x60) = *(int *)(c + 0x4a8);
        *(int *)(c + 0xa8) = 0;
        *(int *)(c + 0x9c) = 0;
        *(int *)(c + 0x98) = 0;
        func_ov066_02116ac4(c, 0x7d0000);
        *(unsigned short *)(c + 0x4d0) = 0xf;
        *(int *)(((int)c + 0x494)) += 1;
        if (*(int *)(c + 0x494) < 3)
            *(int *)(c + 0x4a0) = 3;
        else
            *(int *)(c + 0x4a0) = 4;
        break;
    }
    case 4:
        if (*(unsigned short *)(c + 0x4d0) == 1) {
            data_ov066_0211ae00 ^= *(int *)(c + 0x49c);
        }
        if (data_ov066_0211ae00 != 0)
            break;
        Vec3_ApproachHorz(c + 0x5c, c + 0x4a4, 0x28000);
        if (Vec3_HorzDist(c + 0x5c, c + 0x4a4) > 0x28000)
            break;
        data_ov066_0211ae0c = 0;
        *(int *)(c + 0x5c) = *(int *)(c + 0x4a4);
        *(int *)(c + 0x60) = *(int *)(c + 0x4a8);
        *(int *)(c + 0x64) = *(int *)(c + 0x4ac);
        data_ov066_0211ae08 += 1;
        *(int *)(c + 0x4a0) = 5;
        break;
    case 5:
        if (data_ov066_0211ae04 == 8)
            break;
        *(int *)(c + 0xb0) = 0;
        func_ov066_02119454(c, &data_ov066_0211b06c);
        break;
    }
    return 1;
}
}

// @symbol func_ov066_021175bc
extern "C" {
int func_ov066_021175bc(char *r0)
{
    int r3 = 0;
    *(int *)(r0 + 0x494) = r3;
    *(int *)(r0 + 0x498) = r3;
    char *r1 = r0 + 0x400;
    char *r2 = (char *)&data_ov066_0211ae00;
    *(short *)(r1 + 0xd0) = r3;
    *r2 = r3;
    *(int *)(r0 + 0x4a0) = r3;
    return 1;
}
}

// @symbol func_ov066_021175e8
extern "C" {
int func_ov066_021175e8(void *thiz)
{
    char *c = (char *)thiz;
    EVec3 v;

    switch (*(int *)(c + 0x4a0)) {
    case 0:
        if (data_ov066_0211ae0c == *(int *)(c + 0x49c)) {
            char *p = (char *)_ZN8dActor_c13ClosestPlayerEv(c);
            if (p != 0) {
                EVec3 *pp = (EVec3 *)(((int)p + 0x5c));
                *(int *)(c + 0x4bc) = pp->x;
                *(int *)(c + 0x4c0) = pp->y;
                *(int *)(c + 0x4c4) = pp->z;
                *(int *)(((int)c + 0x4c4)) -= 0xc8000;
                if (*(int *)(c + 0x49c) == 1)
                    *(int *)(((int)c + 0x4bc)) += 0x58000;
                else
                    *(int *)(((int)c + 0x4bc)) -= 0x58000;
                if (*(int *)(c + 0x4c4) < -0xc52000)
                    *(int *)(c + 0x4c4) = -0xc52000;
                else if (*(int *)(c + 0x4c4) > -0x73a000)
                    *(int *)(c + 0x4c4) = -0x73a000;
            }
            func_02012694(0x144, c + 0x74);
            *(int *)(c + 0x4c0) = *(int *)(c + 0x4a8) + 0x1c2000;
            *(int *)(c + 0x4a0) = 1;
        } else if (*(unsigned short *)(c + 0x4d0) == 0) {
            func_ov066_021166c8(c);
            *(int *)(c + 0x4a0) = 4;
        }
        break;
    case 1:
        _Z14ApproachLinearR7Vector3RKS_5Fix12IiE(c + 0x5c, c + 0x4bc, 0x28000);
        if (Vec3_Dist(c + 0x5c, c + 0x4bc) <= 0x28000) {
            *(unsigned short *)(c + 0x4d0) = 0xa;
            *(int *)(c + 0xb0) = 0x2000000;
            *(int *)(c + 0x5c) = *(int *)(c + 0x4bc);
            *(int *)(c + 0x60) = *(int *)(c + 0x4c0);
            *(int *)(c + 0x64) = *(int *)(c + 0x4c4);
            *(int *)(c + 0x4c0) = *(int *)(c + 0x4a8);
            *(int *)(c + 0x4a0) = 2;
        }
        break;
    case 2:
        if (*(unsigned short *)(c + 0x4d0) == 0)
            _Z14ApproachLinearR7Vector3RKS_5Fix12IiE(c + 0x5c, c + 0x4bc, 0x32000);
        if (Vec3_Dist(c + 0x5c, c + 0x4bc) <= 0x32000) {
            func_ov066_02116ac4(c, 0x7d0000);
            *(int *)(c + 0x5c) = *(int *)(c + 0x4bc);
            *(int *)(c + 0x60) = *(int *)(c + 0x4c0);
            *(int *)(c + 0x64) = *(int *)(c + 0x4c4);
            *(unsigned short *)(c + 0x4d0) = 0xa;
            *(int *)(c + 0x4a0) = 7;
            if (func_ov066_02116a68(c) == 0) {
                char *p = (char *)_ZN8dActor_c13ClosestPlayerEv(c);
                if (p != 0) {
                    EVec3 *pp = (EVec3 *)(((int)p + 0x5c));
                    v.x = pp->x;
                    v.y = pp->y;
                    v.z = pp->z;
                    *(unsigned short *)(c + 0x4d0) = 0x24;
                    if (Vec3_HorzDist(c + 0x5c, &v) < 0x400000) {
                        if (v.x < *(int *)(c + 0x5c))
                            *(short *)(c + 0x94) = -0x4000;
                        else
                            *(short *)(c + 0x94) = 0x4000;
                        *(int *)(c + 0x4c8) = 0;
                        *(int *)(c + 0x98) = 0;
                        *(int *)(c + 0x4a0) = 3;
                    }
                }
            }
        }
        break;
    case 3:
        if (*(int *)(c + 0x4c8) < 0x2710) {
            if (*(unsigned short *)(c + 0x4d0) != 0)
                *(int *)(((int)c + 0x4c8)) += 0x1a;
            else
                *(int *)(((int)c + 0x4c8)) += 0x130;
        }
        _Z14ApproachLinearRiii((int *)(c + 0x98), 0x258000, *(int *)(c + 0x4c8));
        if (func_ov066_02116b78(c) == 1) {
            *(int *)(c + 0x98) = 0;
            *(short *)(c + 0x94) = 0;
            *(int *)(c + 0x4a0) = 7;
        }
        break;
    case 4:
        func_ov066_021164ec(c);
        if ((unsigned short)(*(int *)(c + 0x3b8) >> 0xc) == 0)
            func_02012694(0x140, c + 0x74);
        if (*(int *)(c + 0x498) == 1) {
            int r = func_ov066_0211603c(c);
            func_ov066_02116390(c);
            if (r != 0) {
                if (r == 1)
                    *(int *)(c + 0x4a0) = 5;
                break;
            }
        }
        if (data_ov066_0211ae08 != 0) {
            func_ov066_021165cc(c);
            *(int *)(c + 0x4a0) = 6;
        }
        break;
    case 5:
        if (_ZN9Animation8FinishedEv(c + 0x3b0) != 0) {
            func_ov066_021165cc(c);
            *(int *)(c + 0x4a0) = 6;
        }
        break;
    case 6:
        if (_ZN9Animation8FinishedEv(c + 0x3b0) != 0) {
            func_ov066_021162e8(c);
            if (((dBgW *)(c + 0x674))->IsEnabled() != 0)
                ((dBgW *)(c + 0x674))->Disable();
            if (*(int *)(c + 0x49c) == 1)
                _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
                    c + 0x674, (void *)data_ov066_0211ae14[1], c + 0x83c, 0x199,
                    *(short *)(c + 0x8e), &data_ov025_02112c08);
            else
                _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
                    c + 0x674, (void *)data_ov066_0211aeac[1], c + 0x83c, 0x199,
                    *(short *)(c + 0x8e), &data_ov025_02112d48);
            func_020393d4(c + 0x674, (void *)&dBgW::UpdatePosWithTransform);
            func_020393c4(c + 0x674, (void *)func_ov066_0211a35c);
            func_020398fc(c + 0x674);
            ((dBgW *)(c + 0x674))->Enable((dActor_c *)c);
            data_ov066_0211ae08 += 1;
            *(int *)(c + 0x4a0) = 8;
        }
        break;
    case 7:
        if (*(unsigned short *)(c + 0x4d0) == 0) {
            Vec3_ApproachHorz(c + 0x5c, c + 0x4a4, 0x28000);
            if (Vec3_HorzDist(c + 0x5c, c + 0x4a4) <= 0x28000) {
                data_ov066_0211ae0c ^= *(int *)(c + 0x49c);
                *(int *)(c + 0x5c) = *(int *)(c + 0x4a4);
                *(int *)(c + 0x60) = *(int *)(c + 0x4a8);
                *(int *)(c + 0x64) = *(int *)(c + 0x4ac);
                data_ov066_0211ae08 += 1;
                if (data_ov066_0211abe0 != 3)
                    data_ov066_0211ae08 += 1;
                *(int *)(c + 0x4a0) = 8;
            }
        }
        break;
    case 8:
        if (data_ov066_0211ae04 != 7) {
            *(int *)(c + 0xb0) = 0;
            func_ov066_02119454(c, &data_ov066_0211b06c);
        }
        break;
    }
    return 1;
}
}

// @symbol func_ov066_02117bd0
extern "C" {
int func_ov066_02117bd0(char *p)
{
    *(int *)(p + 0x494) = 0;
    *(int *)(p + 0x498) = 0;
    *(short *)(p + 0x4d0) = 0;
    *(int *)(p + 0x4a0) = 0;
    return 1;
}
}

// @symbol func_ov066_02117bf0
extern "C" {
int func_ov066_02117bf0(void *thiz)
{
    char *c = (char *)thiz;
    EVec3 v;

    switch (*(int *)(c + 0x4a0)) {
    case 0:
        if (data_ov066_0211ae0c == *(int *)(c + 0x49c)) {
            char *p = (char *)_ZN8dActor_c13ClosestPlayerEv(c);
            if (p != 0) {
                EVec3 *pp = (EVec3 *)(((int)p + 0x5c));
                *(int *)(c + 0x4bc) = pp->x;
                *(int *)(c + 0x4c0) = pp->y;
                *(int *)(c + 0x4c4) = pp->z;
                *(int *)(((int)c + 0x4c4)) -= 0xc8000;
                if (*(int *)(c + 0x4c4) < -0xc52000)
                    *(int *)(c + 0x4c4) = -0xc52000;
                else if (*(int *)(c + 0x4c4) > -0x73a000)
                    *(int *)(c + 0x4c4) = -0x73a000;
            }
            if (*(int *)(c + 0x4c4) > -0xa68000) {
                if (*(int *)(c + 0x4bc) < 0) {
                    if (*(int *)(c + 0x49c) == 1)
                        *(int *)(((int)c + 0x4bc)) += 0x1c2000;
                    else
                        *(int *)(((int)c + 0x4bc)) += 0x12c000;
                } else {
                    if (*(int *)(c + 0x49c) == 1)
                        *(int *)(((int)c + 0x4bc)) -= 0x12c000;
                    else
                        *(int *)(((int)c + 0x4bc)) -= 0x1c2000;
                }
            }
            func_02012694(0x144, c + 0x74);
            *(int *)(c + 0x4a0) = 1;
        } else if (*(unsigned short *)(c + 0x4d0) == 0) {
            func_ov066_021166c8(c);
            *(int *)(c + 0x4a0) = 3;
        }
        break;
    case 1:
        Vec3_ApproachHorz(c + 0x5c, c + 0x4bc, 0x28000);
        if (Vec3_HorzDist(c + 0x5c, c + 0x4bc) <= 0x28000) {
            *(int *)(c + 0x4a0) = 6;
            if (func_ov066_02116a68(c) == 0) {
                char *p = (char *)_ZN8dActor_c13ClosestPlayerEv(c);
                if (p != 0) {
                    EVec3 *pp = (EVec3 *)(((int)p + 0x5c));
                    v.x = pp->x;
                    v.y = pp->y;
                    v.z = pp->z;
                    *(unsigned short *)(c + 0x4d0) = 0x24;
                    if (Vec3_HorzDist(c + 0x5c, &v) < 0x400000) {
                        if (v.x < *(int *)(c + 0x5c))
                            *(short *)(c + 0x94) = -0x4000;
                        else
                            *(short *)(c + 0x94) = 0x4000;
                        *(int *)(c + 0x4c8) = 0;
                        *(int *)(c + 0x98) = 0;
                        *(int *)(c + 0x4a0) = 2;
                    }
                }
            }
        }
        break;
    case 2:
        if (*(int *)(c + 0x4c8) < 0x2710) {
            if (*(unsigned short *)(c + 0x4d0) != 0)
                *(int *)(((int)c + 0x4c8)) += 0x1a;
            else
                *(int *)(((int)c + 0x4c8)) += 0x130;
        }
        _Z14ApproachLinearRiii((int *)(c + 0x98), 0x258000, *(int *)(c + 0x4c8));
        if (func_ov066_02116b78(c) == 1) {
            *(int *)(c + 0x98) = 0;
            *(short *)(c + 0x94) = 0;
            *(int *)(c + 0x4a0) = 6;
        }
        break;
    case 3:
        func_ov066_021164ec(c);
        if ((unsigned short)(*(int *)(c + 0x3b8) >> 0xc) == 0)
            func_02012694(0x140, c + 0x74);
        if (*(int *)(c + 0x498) == 1) {
            int r = func_ov066_0211603c(c);
            func_ov066_02116390(c);
            if (r != 0) {
                if (r == 1)
                    *(int *)(c + 0x4a0) = 4;
                break;
            }
        }
        if (data_ov066_0211ae08 != 0) {
            func_ov066_021165cc(c);
            *(int *)(c + 0x4a0) = 5;
        }
        break;
    case 4:
        if (_ZN9Animation8FinishedEv(c + 0x3b0) != 0) {
            func_ov066_021165cc(c);
            *(int *)(c + 0x4a0) = 5;
        }
        break;
    case 5:
        if (_ZN9Animation8FinishedEv(c + 0x3b0) != 0) {
            func_ov066_021162e8(c);
            if (((dBgW *)(c + 0x674))->IsEnabled() != 0)
                ((dBgW *)(c + 0x674))->Disable();
            if (*(int *)(c + 0x49c) == 1)
                _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
                    c + 0x674, (void *)data_ov066_0211ae14[1], c + 0x83c, 0x199,
                    *(short *)(c + 0x8e), &data_ov025_02112c08);
            else
                _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
                    c + 0x674, (void *)data_ov066_0211aeac[1], c + 0x83c, 0x199,
                    *(short *)(c + 0x8e), &data_ov025_02112d48);
            func_020393d4(c + 0x674, (void *)&dBgW::UpdatePosWithTransform);
            func_020393c4(c + 0x674, (void *)func_ov066_0211a35c);
            func_020398fc(c + 0x674);
            ((dBgW *)(c + 0x674))->Enable((dActor_c *)c);
            data_ov066_0211ae08 += 1;
            *(int *)(c + 0x4a0) = 7;
        }
        break;
    case 6:
        Vec3_ApproachHorz(c + 0x5c, c + 0x4a4, 0x28000);
        if (Vec3_HorzDist(c + 0x5c, c + 0x4a4) <= 0x28000) {
            data_ov066_0211ae0c ^= *(int *)(c + 0x49c);
            *(int *)(c + 0x5c) = *(int *)(c + 0x4a4);
            *(int *)(c + 0x60) = *(int *)(c + 0x4a8);
            *(int *)(c + 0x64) = *(int *)(c + 0x4ac);
            data_ov066_0211ae08 += 1;
            if (data_ov066_0211abe0 != 3)
                data_ov066_0211ae08 += 1;
            *(int *)(c + 0x4a0) = 7;
        }
        break;
    case 7:
        if (data_ov066_0211ae04 != 6)
            func_ov066_02119454(c, &data_ov066_0211b06c);
        break;
    }
    return 1;
}
}

// @symbol func_ov066_02118168
extern "C" {
int func_ov066_02118168(char *p)
{
    *(int *)(p + 0x494) = 0;
    *(int *)(p + 0x498) = 0;
    *(short *)(p + 0x4d0) = 0;
    *(int *)(p + 0x4a0) = 0;
    return 1;
}
}

// @symbol func_ov066_02118188
extern "C" {
int func_ov066_02118188(void *thiz)
{
    char *c = (char *)thiz;

    switch (*(int *)(c + 0x4a0)) {
    case 0:
        if (*(unsigned short *)(c + 0x4d0) != 0)
            break;
        if (data_ov066_0211ae0c == *(int *)(c + 0x49c)) {
            *(int *)(c + 0x9c) = -0xa000;
            *(int *)(c + 0xa8) = 0x64000;
            *(int *)(c + 0xb0) = 0x2000000;
            *(int *)(c + 0x4a0) = 1;
        } else {
            func_ov066_021166c8(c);
            *(int *)(c + 0x4a0) = 2;
        }
        break;

    case 1:
        if (*(int *)(c + 0x9c) == 0)
            break;
        if (*(int *)(c + 0x4a8) < *(int *)(c + 0x60))
            break;
        *(int *)(c + 0x60) = *(int *)(c + 0x4a8);
        *(int *)(c + 0xa8) = 0;
        *(int *)(c + 0x9c) = 0;
        func_ov066_02116ac4(thiz, 0x7d0000);
        data_ov066_0211ae08 += 1;
        if (data_ov066_0211abe0 != 3)
            data_ov066_0211ae08 += 1;
        *(int *)(c + 0x4a0) = 5;
        break;

    case 2:
        if (*(int *)(c + 0x498) == 1) {
            if (data_ov066_0211ae08 != 0) {
                if (*(int *)(c + 0x494) > 0x14) {
                    *(int *)(c + 0x494) = 0;
                    func_ov066_021165cc(c);
                    *(int *)(c + 0x4a0) = 4;
                    break;
                }
                *(int *)(((int)c + 0x494)) += 1;
            }
        }

        if ((unsigned short)(*(int *)(c + 0x3b8) >> 0xc) == 0) {
            func_02012694(0x140, c + 0x74);
        }

        func_ov066_021164ec(c);

        if (*(int *)(c + 0x498) != 1)
            break;

        func_ov066_02116390(c);
        {
            int r = func_ov066_0211603c(c);
            if (r == 0)
                break;
            if (r == 1) {
                *(int *)(c + 0x4a0) = 3;
            }
        }
        break;

    case 3:
        if (_ZN9Animation8FinishedEv(c + 0x3b0) == 0)
            break;
        func_ov066_021165cc(c);
        *(int *)(c + 0x4a0) = 4;
        break;

    case 4:
        if (_ZN9Animation8FinishedEv(c + 0x3b0) == 0)
            break;
        func_ov066_021162e8(c);
        if (((dBgW *)(c + 0x674))->IsEnabled() != 0)
            ((dBgW *)(c + 0x674))->Disable();

        if (*(int *)(c + 0x49c) == 1) {
            _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
                c + 0x674, (void *)data_ov066_0211ae14[1], c + 0x83c, 0x199,
                *(short *)(c + 0x8e), &data_ov025_02112c08);
        } else {
            _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(
                c + 0x674, (void *)data_ov066_0211aeac[1], c + 0x83c, 0x199,
                *(short *)(c + 0x8e), &data_ov025_02112d48);
        }

        func_020393d4(c + 0x674, (void *)&dBgW::UpdatePosWithTransform);
        func_020393c4(c + 0x674, (void *)func_ov066_0211a35c);
        func_020398fc(c + 0x674);
        ((dBgW *)(c + 0x674))->Enable((dActor_c *)c);

        data_ov066_0211ae08 += 1;
        *(int *)(c + 0x4a0) = 5;
        break;

    case 5:
        if (data_ov066_0211ae04 == 5)
            break;
        *(int *)(c + 0xb0) = 0;
        func_ov066_02119454(c, &data_ov066_0211b06c);
        break;
    }

    return 1;
}
}

// @symbol func_ov066_021184c0
extern "C" {
int func_ov066_021184c0(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 0;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_021184e0
extern "C" {
int func_ov066_021184e0(char *c)
{
    Eyerok *self = (Eyerok *)c;
    if (func_ov066_021168ec(c) != 0 && func_ov066_021168ec(c) != 4) {
        self->mFlags = 0;
        func_ov066_021162e8((int *)c);
        return 1;
    }
    func_ov066_0211632c(c);
    switch (self->mSubState) {
    case 0:
        if (data_ov066_0211ae0c == self->mPartIdx) {
            int *p = &self->mSubState;
            self->mVertAccel = -0x14000;
            self->mVertSpeed = 0x64000;
            *p = *p + 1;
        }
        break;
    case 1:
        if (self->mVertAccel != 0) {
            if (self->mRestPosY >= self->mPosY) {
                self->mPosY = self->mRestPosY;
                self->mVertSpeed = 0;
                self->mVertAccel = 0;
                func_ov066_02116ac4(c, 0x7d0000);
                if ((data_ov066_0211ae0c & self->mPartIdx) != 0)
                    data_ov066_0211ae0c ^= self->mPartIdx;
                self->mSubState = 0;
            }
        }
        break;
    }
    return 1;
}
}

// @symbol func_ov066_021185e4
extern "C" {
int func_ov066_021185e4(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 0;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_02118604
extern "C" {
int func_ov066_02118604(void *c) {
    int r = func_ov066_021168ec(c);
    if (r != 0) {
        data_ov066_0211ae0c ^= ((Eyerok *)c)->mPartIdx;
        if (data_ov066_0211abe0 != 3) {
            data_ov066_0211ae0c |= 3;
        }
    }
    return 1;
}
}

// @symbol func_ov066_02118658
extern "C" {
int func_ov066_02118658(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 0;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_02118678
extern "C" {
int func_ov066_02118678(char* c)
{
    Eyerok *self = (Eyerok *)c;
    if (self->mStateWork0 == 0) {
        if (data_ov066_0211ae04 == 2) {
            self->mBlendModelAnim.speed = 0x1000;
            if (self->mPartIdx == 1) {
                self->mVertSpeed = 0x2d000;
                self->mVertAccel = -0x2000;
            } else {
                self->mVertSpeed = 0xa000;
                self->mVertAccel = -0x800;
            }
            self->mStateWork0 = 1;
            func_02012694(0x144, &self->mCamSpacePosX);
        }
        return 1;
    }

    Vec3_ApproachHorz(&self->mPosX, &self->mRestPosX, 0x14000);
    if (self->mVertAccel != 0) {
        int v = self->mRestPosY;
        if (v >= self->mPosY) {
            self->mPosY = v;
            self->mVertSpeed = 0;
            self->mVertAccel = 0;
            func_ov066_02116ac4(c, 0x7d0000);
        }
    }

    if (self->mVertAccel == 0
        && Vec3_HorzDist(&self->mPosX, &self->mRestPosX) <= 0x14000
        && self->mBlendModelAnim.Finished()) {
        ((dBgW *)(&self->mMeshCollider2))->Enable((dActor_c *)c);
        data_ov066_0211ae0c |= self->mPartIdx;
        func_ov066_02119454(c, &data_ov066_0211b06c);
    }
    return 1;
}
}

// @symbol func_ov066_021187c8
extern "C" {
int func_ov066_021187c8(char* c){
    Eyerok *self = (Eyerok *)c;
  if(self->mPartIdx == 2){
    _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(&self->mBlendModelAnim, (void*)data_ov066_0211ae74[1], 4, 0x40000000, 0x1000, 0);
    _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(&self->mTextureSequence, (void*)data_ov066_0211ae3c[1], 0x40000000, 0x1000, 0);
  } else {
    _ZN14BlendModelAnim7SetAnimER8BCA_Fileii5Fix12IiEt(&self->mBlendModelAnim, (void*)data_ov066_0211ae7c[1], 4, 0x40000000, 0x1000, 0);
    _ZN15TextureSequence7SetFileER8BTP_Filei5Fix12IiEj(&self->mTextureSequence, (void*)data_ov066_0211aebc[1], 0x40000000, 0x1000, 0);
  }
  self->mBlendModelAnim.speed = 0;
  self->mStateWork0 = 0;
  self->mStateWork1 = 0;
  return 1;
}
}

// @symbol func_ov066_021188b0
extern "C" {
int func_ov066_021188b0(char* c){
    Eyerok *self = (Eyerok *)c;
  if(data_ov066_0211abe0==0){
    self->mTimer2=0x64;
    func_ov066_02119454(c, &data_ov066_0211b0ac);
    return 1;
  }
  if(data_ov066_0211ae08>=2){
    data_ov066_0211ae04=3;
    func_ov066_02119454(c, &data_ov066_0211b0cc);
  }
  return 1;
}
}

// @symbol func_ov066_02118934
extern "C" {
int func_ov066_02118934(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 0;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_02118954
extern "C" {
s32 func_ov066_02118954(char* c) {
    s32 r = func_ov066_021168b0(c);
    if (r == 0) {
        return 1;
    }
    ((Eyerok *)c)->mPickCount = 0;
    *(char*)&data_ov066_0211ae10 = 0;
    func_ov066_02119454(c, &data_ov066_0211b03c);
    return 1;
}
}

// @symbol func_ov066_021189a0
extern "C" {
int func_ov066_021189a0(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 0;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_021189c0
extern "C" {
int RandomIntInternal(int* seed);
int func_ov066_021189c0(char* c){
  if(func_ov066_021168b0(c) == 0) return 1;
  if((((unsigned int)RandomIntInternal(&data_0209e650) >> 0x1f) & 1) == 0)
    data_ov066_0211ae0c = 2;
  else
    data_ov066_0211ae0c = 1;
  ((Eyerok *)c)->mPickCount = 0;
  func_ov066_02119454(c, &data_ov066_0211b03c);
  return 1;
}
}

// @symbol func_ov066_02118a30
extern "C" {
int func_ov066_02118a30(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 0;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_02118a50
extern "C" {
s32 func_ov066_02118a50(char* c) {
    s32 r = func_ov066_021168b0(c);
    if (r == 0) return 1;
    if (data_ov066_0211abe0 == 3) {
        if (!(data_ov066_0211ae10 & 1)) data_ov066_0211ae0c = 2;
        else data_ov066_0211ae0c = 1;
    } else {
        data_ov066_0211ae0c = data_ov066_0211abe0;
    }
    {
        unsigned char* p = (unsigned char*)((int)&((Eyerok *)c)->mPickCount);
        *p += 1;
    }
    data_ov066_0211ae10 += 1;
    data_ov066_0211ae10 &= 1;
    func_ov066_02119454(c, &data_ov066_0211b03c);
    return 1;
}
}

// @symbol func_ov066_02118b08
extern "C" {
int func_ov066_02118b08(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 0;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_02118b28
extern "C" {
s32 func_ov066_02118b28(char* c) {
    s32 r = func_ov066_021168b0(c);
    if (r == 0) return 1;
    if (data_ov066_0211abe0 == 3) {
        if (!(data_ov066_0211ae10 & 1)) data_ov066_0211ae0c = 2;
        else data_ov066_0211ae0c = 1;
    } else {
        data_ov066_0211ae0c = data_ov066_0211abe0;
    }
    {
        unsigned char* p = (unsigned char*)((int)&((Eyerok *)c)->mPickCount);
        *p += 1;
    }
    data_ov066_0211ae10 += 1;
    data_ov066_0211ae10 &= 1;
    func_ov066_02119454(c, &data_ov066_0211b03c);
    return 1;
}
}

// @symbol func_ov066_02118be0
extern "C" {
int func_ov066_02118be0(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 0;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_02118c00
extern "C" {
s32 func_ov066_02118c00(char* c) {
    s32 r = func_ov066_021168b0(c);
    if (r == 0) return 1;
    if (data_ov066_0211abe0 == 3) {
        if (!(data_ov066_0211ae10 & 1)) data_ov066_0211ae0c = 2;
        else data_ov066_0211ae0c = 1;
    } else {
        data_ov066_0211ae0c = data_ov066_0211abe0;
    }
    {
        unsigned char* p = (unsigned char*)((int)&((Eyerok *)c)->mPickCount);
        *p += 1;
    }
    data_ov066_0211ae10 += 1;
    data_ov066_0211ae10 &= 1;
    func_ov066_02119454(c, &data_ov066_0211b03c);
    return 1;
}
}

// @symbol func_ov066_02118cb8
extern "C" {
int func_ov066_02118cb8(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 30;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_02118cdc
extern "C" {

int func_ov066_02118cdc(char* c) {
    Eyerok *self = (Eyerok *)c;
    if (func_ov066_021168b0(c) == 0)
        return 1;
    if (self->mTimer1 == 0) {
        if (func_ov066_02116a68(c) != (int)0xff3ae000) {
            if (data_ov066_0211ae0c == 0) {
                data_ov066_0211ae04 = 3;
                self->mTimer2 = 0x1e;
                func_ov066_02119454(c, &data_ov066_0211b0cc);
            }
            return 1;
        }
    }
    if (data_ov066_0211ae0c == 0) {
        if (data_ov066_0211abe0 == 3) {
            if (self->mStateWork0 == 0)
                data_ov066_0211ae0c = 1;
            else
                data_ov066_0211ae0c = 2;
        } else {
            data_ov066_0211ae0c = data_ov066_0211abe0;
        }
        volatile int* tmp = (volatile int*)((int)&self->mStateWork0);
        *tmp = *tmp + 1;
        *tmp = *tmp & 1;
    }
    return 1;
}
}

// @symbol func_ov066_02118de0
extern "C" {
int func_ov066_02118de0(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 30;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_02118e04
extern "C" {
int func_ov066_02118e04(void* self)
{
    u8* c = (u8*)self;
    void* p = ((dActor_c *)self)->ClosestPlayer();
    int coinFlip;
    int v;

    if (data_ov066_0211abe0 == 0) {
        ((Eyerok *)c)->mTimer2 = 0x64;
        func_ov066_02119454(c, &data_ov066_0211b0ac);
        return 1;
    }

    if (p == 0 || ((Eyerok *)c)->mTimer2 != 0)
        return 1;

    coinFlip = ((unsigned int)RandomIntInternal(&data_0209e650) >> 31) & 1;

    data_ov066_0211ae08 = 0;
    data_ov066_0211ae0c = 0;
    v = func_ov066_02116a68(c);
    if (v == (int)0xff3ae000) {
        data_ov066_0211ae04 = 4;
        func_ov066_02119454(c, &data_ov066_0211b0dc);
        return 1;
    }

    if ((int)((Eyerok *)c)->mPickCount > data_ov066_0211abe4 + 3) {
        if (data_ov066_0211abe0 == 3) {
            data_ov066_0211abe4++;
            data_ov066_0211abe4 &= 1;
            data_ov066_0211ae04 = 8;
            func_ov066_02119454(c, &data_ov066_0211b00c);
        } else {
            data_ov066_0211abe4 = -3;
            data_ov066_0211ae04 = 9;
            func_ov066_02119454(c, &data_ov066_0211b02c);
        }
        return 1;
    }

    v = func_ov066_02116a68(c);
    if (v == -0xb50000) {
        data_ov066_0211ae04 = 5;
        func_ov066_02119454(c, &data_ov066_0211afcc);
        return 1;
    }

    if (coinFlip == 0) {
        data_ov066_0211ae04 = 7;
        func_ov066_02119454(c, &data_ov066_0211affc);
    } else {
        data_ov066_0211ae04 = 6;
        func_ov066_02119454(c, &data_ov066_0211afdc);
    }
    return 1;
}
}

// @symbol func_ov066_0211901c
extern "C" {
int func_ov066_0211901c(char *p)
{
    Eyerok *self = (Eyerok *)p;
    self->mStateWork0 = 0;
    self->mStateWork1 = 0;
    self->mTimer1 = 0;
    self->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_0211903c
extern "C" {
int func_ov066_0211903c(char* self) {
    Eyerok *obj = (Eyerok *)self;
    struct Vector3 v1, v2, in, out, star;
    void* cam;
    int msgid;

    if (obj->mTimer2) return 1;

    cam = data_0209f318;
    if (((Eyerok *)self)->mSubState == 0) {
        _ZN6Camera9SetFlag_3Ev(cam);
        ((Eyerok *)self)->mTalkPlayer = (Player *)((dActor_c *)self)->ClosestPlayer();
        if (((Eyerok *)self)->mTalkPlayer != 0)
            ((Player *)(((Eyerok *)self)->mTalkPlayer))->SetNoControlState(5, -1, 0);
        ((Eyerok *)self)->mSubState = 1;
    } else {
        v1.x = ((Eyerok *)self)->mPosX;
        v1.y = ((Eyerok *)self)->mPosY;
        v1.z = ((Eyerok *)self)->mPosZ;
        v2.x = ((Eyerok *)self)->mPosX;
        v2.y = ((Eyerok *)self)->mPosY;
        v2.z = ((Eyerok *)self)->mPosZ;
        v1.y += 0x100000;
        v2.x += 0x10000;
        v2.y += 0x100000;
        v2.z += 0x564000;
        func_020092c4(cam, (char*)cam + 0x80, &v1);
        func_020092c4(cam, (char*)cam + 0x8c, &v2);
    }

    if (data_ov066_0211abe0 == 3) {
        if (data_ov066_0211ae0c != 3) return 1;
    }

    if (((Eyerok *)self)->mStateWork1 == 0) {
        if (((Eyerok *)self)->mTalkPlayer != 0) {
            in.x = 0; in.y = 0; in.z = 0;
            out.x = 0; out.y = 0; out.z = 0;
            in.y = 0x32000;
            in.z = -0x32000;

            Matrix4x3_FromRotationY(data_020a0e68, 0);
            MulVec3Mat4x3(&in, data_020a0e68, &out);

            out.x += ((Eyerok *)self)->mPosX;
            out.y += ((Eyerok *)self)->mPosY;
            out.z += ((Eyerok *)self)->mPosZ;

            msgid = 0xb8;
            if (data_ov066_0211abe0 == 0) {
                msgid = 0xb9;
                _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(0x14, 0x15666);
            }

            ((Eyerok *)self)->mTalkPlayer->mStateFlags |= 0x400;
            Message::PrepareTalk();
            if (((Player *)(((Eyerok *)self)->mTalkPlayer))->ShowMessage(*(fBase_c *)self, msgid, &out, 0, 0) == 1) {
                ((Eyerok *)self)->mStateWork1 = 1;
                func_02012694(0x145, &((Eyerok *)self)->mCamSpacePosX);
            }
        }
    } else {
        if (((Eyerok *)self)->mTalkPlayer != 0) {
            if (((Player *)(((Eyerok *)self)->mTalkPlayer))->GetTalkState() < 0) {
                *(int*)(((int)cam + 0x154)) &= ~8;
                Message::EndTalk();
                if (data_ov066_0211abe0 == 3) {
                    _ZN5Sound22LoadAndSetMusic_Layer3Ej(0x2d);
                    func_02011d2c();
                    func_ov066_02119454(self, &data_ov066_0211b0cc);
                } else {
                    _ZN5Sound17ChangeMusicVolumeEj5Fix12IiE(0x7f, 0x15666);
                    _ZN5Sound22StopLoadedMusic_Layer3Ev();
                    func_02011cfc();
                    star.x = 0;
                    star.y = (int)0xffa24000;
                    star.z = (int)0xff1b4000;
                    ((dActor_c *)self)->UntrackAndSpawnStar(*(signed char*)(&((Eyerok *)self)->mStarTracked), obj->mStarId, star, 4);
                    ((fBase_c *)self)->MarkForDestruction();
                }
            }
        }
    }
    return 1;
}
}

// @symbol func_ov066_02119348
extern "C" {
int func_ov066_02119348(void *c)
{
    if (((dBgW *)((char *)&((Eyerok *)c)->mMeshCollider2))->IsEnabled() != 0) {
        ((dBgW *)((char *)&((Eyerok *)c)->mMeshCollider2))->Disable();
    }
    ((Eyerok *)c)->mStateWork0 = 0;
    ((Eyerok *)c)->mStateWork1 = 0;
    ((Eyerok *)c)->mTimer1 = 0;
    ((Eyerok *)c)->mSubState = 0;
    return 1;
}
}

// @symbol func_ov066_02119398
struct Vec4 { int a, b, c, d; ~Vec4(){} };
extern "C" {

int func_ov066_02119398(char* c)
{
    Vec4 sp;
    /* Member loads of the player's position come out a different size.
       The base pointer is what matches. */
    char* p = (char *)((dActor_c *)c)->ClosestPlayer();
    if (p != 0) {
        char* playerPos = p + 0x5c;
        int v1 = *(int*)(playerPos + 4);
        int v2 = *(int*)(playerPos + 8);
        if (v1 < -0x300000) {
            int f = (int)((((Eyerok *)c)->mFlags & 8) != 0);
            if (f == 0) {
                if (v2 < -0xd70000) {
                    data_ov066_0211ae08 += 1;
                }
            }
        }
    }
    if (data_ov066_0211ae08 > 2) {
        data_ov066_0211ae08 = 0;
        data_ov066_0211ae04 = 2;
        func_ov066_02119454(c, &data_ov066_0211b0ac);
    }
    return 1;
}
}

// @symbol func_ov066_0211944c
extern "C" {
int func_ov066_0211944c(void)
{
    return 1;
}
}

// @symbol func_ov066_02119454
struct C { char pad[0x48c]; PMF *pp; };
extern "C" int func_ov066_02119454(void *cv, void *pv) { C *c = (C *)cv; PMF *p = (PMF *)pv; c->pp = p; PMF *q = c->pp; if (*q == 0) return 1; return (c->**q)(); }

// @symbol func_ov066_021194a4
extern "C" void func_ov066_021194a4(char *c) {
  Matrix4x3_FromRotationY(&((Eyerok *)c)->mClsnMat2, ((Eyerok *)c)->mAngleY);
  ((Eyerok *)c)->mClsnMat2.t.x = ((Eyerok *)c)->mPosX;
  ((Eyerok *)c)->mClsnMat2.t.y = ((Eyerok *)c)->mPosY;
  ((Eyerok *)c)->mClsnMat2.t.z = ((Eyerok *)c)->mPosZ;
  ((dBgW_KcMbg *)(&((Eyerok *)c)->mMeshCollider2))->Transform(((Eyerok *)c)->mClsnMat2, ((Eyerok *)c)->mAngleY);
}

// @symbol func_ov066_021194fc
extern "C" {


void func_ov066_021194fc(char* c)
{
    Eyerok *self = (Eyerok *)c;
    int v[3];
    Vec3_Asr(v, &self->mPosX, 3);
    Matrix4x3_FromTranslation(data_020a0e68, v[0], v[1], v[2]);
    Matrix4x3_ApplyInPlaceToRotationXYZExt(data_020a0e68, self->mAngleX, self->mAngleY, self->mAngleZ);
    if (self->mPartIdx == 0)
        *(M48 *)&self->mModel2.mat4x3 = *(M48*)data_020a0e68;
    else
        *(M48 *)&self->mBlendModelAnim.mat4x3 = *(M48*)data_020a0e68;
    if (self->mPartIdx == 0)
        return;
    if (self->mRestPosY >= self->mPosY)
        return;
    {
        int d;
        if (self->mPartIdx == 2)
            d = 0x64000;
        else
            d = -0x64000;
        Matrix4x3_FromTranslation(data_020a0e68,
            (self->mPosX + d) >> 3,
            (self->mPosY - 0x8000) >> 3,
            (self->mPosZ + 0xa0000) >> 3);
    }
    *(M48 *)self->mShadowMtx = *(M48*)data_020a0e68;
    _ZN8dActor_c19DropShadowRadHeightER11ShadowModelR9Matrix4x35Fix12IiES5_j(
        c, &self->mShadowModel, self->mShadowMtx, 0x140000, 0x258000, 0xf);
}
}

// @symbol _ZN6Eyerok16CleanupResourcesEv
int Eyerok::CleanupResources()
{
  if(((dBgW *)&mMeshCollider2)->IsEnabled())
    ((dBgW *)&mMeshCollider2)->Disable();
  if(mPartIdx==0){
    ((SharedFilePtr *)(data_ov066_0211ae6c))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae4c))->Release();
    ((SharedFilePtr *)(data_ov066_0211aeb4))->Release();
    ((SharedFilePtr *)(data_ov066_0211aebc))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae9c))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae3c))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae2c))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae5c))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae84))->Release();
    ((SharedFilePtr *)(data_ov066_0211aea4))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae8c))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae54))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae94))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae64))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae44))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae74))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae7c))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae24))->Release();
    ((SharedFilePtr *)(data_ov066_0211aeac))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae14))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae1c))->Release();
    ((SharedFilePtr *)(data_ov066_0211ae34))->Release();
  }
  return 1;
}

// @symbol _ZN6Eyerok16OnPendingDestroyEv
/* Eyerok::OnPendingDestroy -- vtable slot 12. The ROM body is empty: the
 * override exists only to occupy the slot. */
void Eyerok::OnPendingDestroy()
{
}

// @symbol _ZN6Eyerok6RenderEv
int Eyerok::Render()
{
  if (mPartIdx == 0) {
    if (data_ov066_0211ae04 == 1) {
      mModel2.Render(0);
    }
    return 1;
  }
  if (data_ov066_0211ae04 == 1) return 1;
  ((TextureSequence *)&mTextureSequence)->TextureSequence::Update(mBlendModelAnim.data);
  mBlendModelAnim.Render(0);
  return 1;
}


/* Bracketed, and it binds only because of the file-top
 * `#pragma defer_codegen off`: with codegen deferred (mwccarm 2004/b56s
 * default) a bracketed opt_* pragma does not bind and these two go
 * file-global, which costs func_ov066_021184e0 (4 words) and
 * func_ov066_021194fc (a size change).  Deleting them outright instead
 * costs _ZN6Eyerok8BehaviorEv: 33/34, a 999-word content divergence plus one
 * wrong relocation destination, with the size UNCHANGED at 0x4b0.  (An earlier
 * revision of this comment said 0x4b0 -> 0x4ac; that size change belongs to a
 * ROM-descending arrangement, not this one.  Re-measured by negative control on
 * the shipped source.) */
#pragma opt_common_subs off
#pragma opt_strength_reduction off
// @symbol _ZN6Eyerok8BehaviorEv
/* Eyerok::Behavior -- vtable slot 6. Real C++ method over the shared header.
 * EVec3 is a local plain-int triple (stack temps); callees whose ROM symbols
 * carry by-value/ref class parameters keep their literal mangled extern "C"
 * spellings. */
int Eyerok::Behavior()
{
    char *c = (char *)this;

    DecIfAbove0_Short(&mTimer1);
    DecIfAbove0_Short(&mTimer2);

    {
        State *st = *(State **)&mState;
        if (*(int *)((char *)st + 8) != 0)
            (((C *)c)->*(st->fn))();
    }

    if (mDustCounter != 0) {
        if ((mDustCounter & 1) == 0) {
            int rnd = RandomIntInternal(&data_0209e650);
            int off = (mDustCounter >> 1) * 0xc;
            int base_dc = 0x4dc;
            int base_e4 = 0x4e4;
            char *bx = c + base_dc;
            char *bz = c + base_e4;
            char *by = c + 0x4e0;
            int *px;
            int *pz;
            int *py;
            int zero;
            EVec3 vin;
            EVec3 vout;
            *(int *)(bx + off) = mPosX;
            *(int *)(by + off) = mPosY;
            *(int *)(bz + off) = mPosZ;
            px = (int *)(bx + off);
            py = (int *)(by + off);
            pz = (int *)(bz + off);
            zero = 0;
            vin.x = zero;
            vin.y = zero;
            vin.z = zero;
            vout.x = zero;
            vout.y = zero;
            vout.z = zero;
            if (mState != (void *)&data_ov066_0211b07c) {
                if (mAngleY != 0) {
                    vin.z = (0x7e - (((rnd >> 8) & 0x3f) << 2)) << 12;
                    Matrix4x3_FromRotationY(data_020a0e68, (s16)(mAngleY - 0x4000));
                    MulVec3Mat4x3(&vin, data_020a0e68, &vout);
                    *px += vout.x;
                    *pz += vout.z;
                } else {
                    if (((rnd >> 16) & 1) == 0)
                        *px += (((rnd >> 8) & 3) * 0x28) << 12;
                    else
                        *px -= (((rnd >> 8) & 3) * 0x28) << 12;
                    *pz += 0x19000;
                }
                *py += ((mDustCounter * 0xa) + 0x23) << 12;
            } else {
                if (mAngleY != 0) {
                    vin.z = (0x7e - (((rnd >> 8) & 0x3f) << 2)) << 12;
                    Matrix4x3_FromRotationY(data_020a0e68, (s16)(mAngleY - 0x4000));
                    MulVec3Mat4x3(&vin, data_020a0e68, &vout);
                    *px += vout.x;
                    *pz += vout.z;
                } else {
                    int a = ((rnd >> 24) & 7) * 0x1e;
                    int b = ((rnd >> 16) & 7) * 0x1e;
                    *pz -= 0x64000;
                    *px += (0x69 - a) << 12;
                    *pz += (0x69 - b) << 12;
                }
                *py += 0x96000;
            }
        }

        {
            int i = 0;
            char *cur = c;
            u32 id0 = 0x13a;
            u32 id1 = 0x13b;
            int z0 = 0;
            for (; i < 0x14; i++) {
                if (*(int *)(cur + 0x4dc) != 0 || *(int *)(cur + 0x4e0) != 0 || *(int *)(cur + 0x4e4) != 0) {
                    mDustParticle1[i] =
                        _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
                            mDustParticle1[i], id0,
                            *(int *)(cur + 0x4dc), *(int *)(cur + 0x4e0), *(int *)(cur + 0x4e4),
                            (void *)z0, (void *)z0);
                    mDustParticle2[i] =
                        _ZN8Particle6System3NewEjj5Fix12IiES2_S2_PK11Vector3_16fPNS_8CallbackE(
                            mDustParticle2[i], id1,
                            *(int *)(cur + 0x4dc), *(int *)(cur + 0x4e0), *(int *)(cur + 0x4e4),
                            (void *)z0, (void *)z0);
                }
                cur += 0xc;
            }
        }

        {
            /* c400 + 0xd4 is mDustCounter reached the long way round -- the
               ROM materialises c + 0x400 first and offsets from it, and
               spelling that step away is not free. */
            int o4d4 = 0x4d4;
            u16 *p = &mDustCounter;
            u16 v = *p;
            char *c400 = c + 0x400;
            *p = (u16)(v + 1);
            if (*(u16 *)(c400 + 0xd4) > 0x26) {
                int j = 0;
                char *q = c;
                *(u16 *)(c400 + 0xd4) = (u16)j;
                for (; j < 0x14; j++) {
                    *(int *)(q + 0x4dc) = 0;
                    *(int *)(q + 0x4e0) = 0;
                    *(int *)(q + 0x4e4) = 0;
                    q += 0xc;
                }
            }
        }
    }

    if (mPartIdx == 0) {
        func_ov066_021194fc(c);
        if (((dBgW *)&mMeshCollider2)->IsEnabled() != 0)
            func_ov066_021194a4(c);
        return 1;
    }

    {
        EVec3 vrel;
        mRestPosY = mSpawnPosY + 0x8000;
        ((dActor_c *)c)->UpdatePos(0);
        mdCcAcPos_c.pos.x = mPosX;
        mdCcAcPos_c.pos.y = mPosY;
        mdCcAcPos_c.pos.z = mPosZ;
        vrel.x = data_ov066_0211ad18[0];
        vrel.y = data_ov066_0211ad18[1];
        vrel.z = data_ov066_0211ad18[2];
        ((dCcAcPos_c *)&mdCcAcPos_c)->SetPosRelativeToActor(*(Vector3 *)&vrel);
        func_ov066_021194fc(c);
        if (((dBgW *)&mMeshCollider2)->IsEnabled() != 0)
            func_ov066_021194a4(c);
        ((dCc_c *)&mdCcAcPos_c)->Clear();
        ((dCc_c *)&mdCcAcPos_c)->dCc_c::Update();
        ((BlendModelAnim *)&mBlendModelAnim)->Advance();
        ((Animation *)&mTextureSequence)->Advance();
    }
    return 1;
}


#pragma opt_strength_reduction on
#pragma opt_common_subs on

// @symbol _ZN6Eyerok13InitResourcesEv
int Eyerok::InitResources()
{
    char *c = (char *)((void *)this);
    Vector3 v;
    Vector3 w;

    mPartIdx = (s32)param1 & 0xFF;
    if (mPartIdx == 0xFF)
        mPartIdx = 0;
    mStarId = (param1 >> 0xC) & 0xF;
    mStarTracked = _ZN8dActor_c9TrackStarEjj(c, mStarId, 2);
    if (mPartIdx > 2)
        mPartIdx = 0;

    switch (mPartIdx) {
    case 0:
        _ZN9ModelBase7SetFileEP8BMD_Fileii(&mModel2, _ZN5Model8LoadFileER13SharedFilePtr(data_ov066_0211ae6c), 1, -1);
        _ZN5Model8LoadFileER13SharedFilePtr(data_ov066_0211ae4c);
        _ZN5Model8LoadFileER13SharedFilePtr(data_ov066_0211aeb4);
        _ZN15TextureSequence8LoadFileER13SharedFilePtr(data_ov066_0211aebc);
        _ZN15TextureSequence8LoadFileER13SharedFilePtr(data_ov066_0211ae9c);
        _ZN15TextureSequence8LoadFileER13SharedFilePtr(data_ov066_0211ae3c);
        _ZN15TextureSequence8LoadFileER13SharedFilePtr(data_ov066_0211ae2c);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211ae5c);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211ae84);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211aea4);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211ae8c);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211ae54);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211ae94);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211ae64);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211ae44);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211ae74);
        _ZN9Animation8LoadFileER13SharedFilePtr(data_ov066_0211ae7c);
        _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(data_ov066_0211ae24);
        _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(data_ov066_0211aeac);
        _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(data_ov066_0211ae14);
        _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(data_ov066_0211ae1c);
        _ZN7dBgW_Kc8LoadFileER13SharedFilePtr(data_ov066_0211ae34);
        break;
    case 1:
        if (_ZN9ModelBase7SetFileEP8BMD_Fileii(&mBlendModelAnim, (void *)data_ov066_0211ae4c[1], 1, -1) == 0)
            return 0;
        TextureSequence::Prepare(*(BMD_File *)data_ov066_0211ae4c[1], *(BTP_File *)data_ov066_0211aebc[1]);
        TextureSequence::Prepare(*(BMD_File *)data_ov066_0211ae4c[1], *(BTP_File *)data_ov066_0211ae9c[1]);
        break;
    case 2:
        if (_ZN9ModelBase7SetFileEP8BMD_Fileii(&mBlendModelAnim, (void *)data_ov066_0211aeb4[1], 1, -1) == 0)
            return 0;
        TextureSequence::Prepare(*(BMD_File *)data_ov066_0211aeb4[1], *(BTP_File *)data_ov066_0211ae3c[1]);
        TextureSequence::Prepare(*(BMD_File *)data_ov066_0211aeb4[1], *(BTP_File *)data_ov066_0211ae2c[1]);
        break;
    }

    if (mPartIdx != 0) {
        _ZN11ShadowModel12InitCylinderEv(&mShadowModel);
        w.x = data_ov066_0211ad18[0];
        w.y = data_ov066_0211ad18[1];
        w.z = data_ov066_0211ad18[2];
        _ZN10dCcAcPos_c4InitEP8dActor_cRK7Vector35Fix12IiES6_jj(&mdCcAcPos_c, c, &w, 0x64000, 0x64000, 0x200002, 0);
    }

    {
        /* NOT mDustPos[i]: the ROM walks a running char* and re-derives the
           three stores from it. Spelling this as `Vector3 *p = mDustPos; p->x
           = 0; ... p += 1;` costs the function its size -- measured. */
        int i = 0;
        char *p = c;
        do {
            *(s32 *)(p + 0x4DC) = 0;
            *(s32 *)(p + 0x4E0) = 0;
            i += 1;
            *(s32 *)(p + 0x4E4) = 0;
            p += 0xC;
        } while (i < 0x14);
    }

    mTerminalVelocity = -0x64000;
    mHandUniqueID1 = 0;
    mHandUniqueID2 = 0;

    if (mPartIdx == 0) {
        void *r;
        mPosZ -= 0x7C000;
        mRestPosX = mPosX;
        mRestPosY = mPosY;
        mRestPosZ = mPosZ;
        v.x = mPosX;
        v.y = mPosY;
        v.z = mPosZ;
        v.x += 0x193000;
        r = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0xB0, 1, &v, 0, mAreaId, -1);
        if (r != 0)
            mHandUniqueID1 = *(s32 *)((char *)r + 4);
        v.x = mPosX;
        v.x -= 0x18C000;
        r = _ZN8dActor_c5SpawnEjjRK7Vector3PK10Vector3_16as(0xB0, 2, &v, 0, mAreaId, -1);
        if (r != 0)
            mHandUniqueID2 = *(s32 *)((char *)r + 4);
        data_ov066_0211ae10 = 0;
        data_ov066_0211ae08 = 0;
        data_ov066_0211ae0c = 0;
        data_ov066_0211abe4 = 1;
        data_ov066_0211ae04 = 1;
        data_ov066_0211abe0 = 3;
        _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(&mMeshCollider2, (void *)data_ov066_0211ae24[1], &mClsnMat2, 0x199, mAngleY, &data_ov025_02112ca8);
        func_020393d4(&mMeshCollider2, (void *)&dBgW::UpdatePosWithTransform);
        func_020393c4(&mMeshCollider2, (void *)func_ov066_0211a35c);
        ((dBgW *)&mMeshCollider2)->Enable(this);
        mTimer2 = 0x64;
        func_ov066_02119454(c, data_ov066_0211b09c);
    } else {
        mRestPosX = mPosX;
        mRestPosY = mPosY;
        mRestPosZ = mPosZ;
        mSpawnPosX = mPosX;
        mSpawnPosY = mPosY;
        mSpawnPosZ = mPosZ;
        if (mPartIdx == 1) {
            _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(&mMeshCollider2, (void *)data_ov066_0211ae14[1], &mClsnMat2, 0x199, mAngleY, &data_ov025_02112c08);
            mRestPosX -= 0x31F000;
        } else {
            _ZN10dBgW_KcMbg7SetFileEP8KCL_FileRK9Matrix4x35Fix12IiEsR10CLPS_Block(&mMeshCollider2, (void *)data_ov066_0211aeac[1], &mClsnMat2, 0x199, mAngleY, &data_ov025_02112d48);
            mRestPosX += 0x31F000;
        }
        func_020393d4(&mMeshCollider2, (void *)&dBgW::UpdatePosWithTransform);
        func_020393c4(&mMeshCollider2, (void *)func_ov066_0211a35c);
        func_020398fc(&mMeshCollider2);
        mRestPosZ -= 0x32000;
        unk_4d8 = 3;
        data_ov066_0211ae00 = 0;
        func_ov066_02119454(c, data_ov066_0211b05c);
    }
    return 1;
}

// @symbol _ZN6Eyerok16OnAimedAtWithEggEv
/* Slot 29, attributed by the vtable: _ZTV6Eyerok + 4*29 = 0x0211ad64 + 0x74
   = 0x0211ade8, and config/arm9/overlays/ov066/relocs.txt relocates
   0x0211ade8 -> 0x0211a2dc. */
int Eyerok::OnAimedAtWithEgg()
{
    return 163840;
}
